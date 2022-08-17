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

    RespValue value;
    DecoderCallbackImpl decoder_callback_impl;

    value.type(RespType::Integer);
    value.asInteger() =
        provider.ConsumeIntegralInRange<int64_t>(INT64_MIN, INT64_MAX);

    decoder_callback_impl.encoder_.encode(value, decoder_callback_impl.buffer_);
    decoder_callback_impl.decoder_.decode(decoder_callback_impl.buffer_);
    FUZZ_ASSERT(value.toString() ==
                decoder_callback_impl.decoded_values_[0]->toString());
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