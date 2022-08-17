#include <cstdint>
#include <iostream>
#include <vector>

#include "test/extensions/filters/network/common/redis/codec_fuzz_tests/decoder_callback_impl.h"
#include "test/fuzz/fuzz_runner.h"

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

    const size_t maxStringSize = 96 * 1024;

    DecoderCallbackImpl decoder_callback_impl;
    std::string generatedString =
        provider.ConsumeRandomLengthString(maxStringSize);

    decoder_callback_impl.buffer_.add(generatedString);

    try {
      decoder_callback_impl.decoder_.decode(decoder_callback_impl.buffer_);
    } catch (Common::Redis::ProtocolError& e) {
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