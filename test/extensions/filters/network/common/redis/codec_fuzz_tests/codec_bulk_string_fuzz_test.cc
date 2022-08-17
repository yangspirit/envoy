#include <cstdint>
#include <iostream>
#include <vector>

#include "test/extensions/filters/network/common/redis/codec_fuzz_tests/decoder_callback_impl.h"
#include "source/common/common/assert.h"
#include "test/fuzz/fuzz_runner.h"
#include "test/fuzz/utility.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace Common {
namespace Redis {
namespace Fuzz {

class DecoderFuzz {
 public:
  DecoderFuzz(const uint8_t* buf, size_t len) {
    FuzzedDataProvider provider(buf, len);
    // Max supported size for Bulk Strings is 512MB
    const size_t maxBulkStringSize = 512 * 1024 * 1024;
    uint64_t resp_data_size_limit = 50 * 1024 * 1024;

    RespValue value;
    DecoderCallbackImpl decoder_callback_impl(resp_data_size_limit);

    value.type(RespType::BulkString);
    value.asString() = provider.ConsumeRandomLengthString(maxBulkStringSize);

    decoder_callback_impl.encoder_.encode(value, decoder_callback_impl.buffer_);
    decoder_callback_impl.decoder_.decode(decoder_callback_impl.buffer_);
    if (value.asString().length() <= resp_data_size_limit) {
      FUZZ_ASSERT(value.toString() ==
                  decoder_callback_impl.decoded_values_[0]->toString());
    }
  }
};

DEFINE_FUZZER(const uint8_t* buf, size_t len) {
  DecoderFuzz decoder_instance(buf, len);
}

}  // namespace Fuzz
}  // namespace Redis
}  // namespace Common
}  // namespace NetworkFilters
}  // namespace Extensions
}  // namespace Envoy