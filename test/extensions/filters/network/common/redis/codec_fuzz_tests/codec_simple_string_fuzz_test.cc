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
  bool HasInvalidChars(std::string input) {
    int length = input.length();
    if (length < 2) return true;
    size_t found = input.find("\r\n");
    if (found != std::string::npos) {
      return true;
    }
    return false;
  }

  DecoderFuzz(const uint8_t* buf, size_t len) {
    FuzzedDataProvider provider(buf, len);

    const size_t maxStringSize = 96 * 1024;

    RespValue value;
    DecoderCallbackImpl decoder_callback_impl;

    std::string generatedString =
        provider.ConsumeRandomLengthString(maxStringSize);

    value.type(RespType::SimpleString);
    value.asString() = generatedString;
    decoder_callback_impl.encoder_.encode(value, decoder_callback_impl.buffer_);

    try {
      decoder_callback_impl.decoder_.decode(decoder_callback_impl.buffer_);
      if (!HasInvalidChars(generatedString)) {
        FUZZ_ASSERT(value.toString() ==
                    decoder_callback_impl.decoded_values_[0]->toString());
      }
    } catch (Common::Redis::ProtocolError& e) {}
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