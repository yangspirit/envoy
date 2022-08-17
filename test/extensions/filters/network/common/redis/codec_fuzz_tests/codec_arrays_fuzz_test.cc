#include <cstdint>
#include <iostream>
#include <vector>

#include "test/extensions/filters/network/common/redis/codec_fuzz_tests/codec_fuzz.pb.h"
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

class DecoderProtoFuzzer {
 public:
  void Fuzz(const test::extensions::filters::network::common::redis::
                CodecArrayFuzzTestCase& input) {
    uint64_t resp_data_size_limit = 50 * 1024 * 1024;

    DecoderCallbackImpl decoder_callback_impl(resp_data_size_limit);
    std::vector<RespValue> values;

    for (const test::extensions::filters::network::common::redis::Array& array :
         input.arrays()) {
      std::vector<RespValue> respdata;

      for (const test::extensions::filters::network::common::redis::RespData&
               data : array.resp_data()) {
        RespValue temp;
        switch (data.resp_selector_case()) {
          case test::extensions::filters::network::common::redis::RespData::
              kSimpleString: {
            temp.type(RespType::SimpleString);
            temp.asString() = data.simple_string();
            break;
          }
          case test::extensions::filters::network::common::redis::RespData::
              kError: {
            temp.type(RespType::Error);
            temp.asString() = data.error();
            break;
          }
          case test::extensions::filters::network::common::redis::RespData::
              kInteger: {
            temp.type(RespType::Integer);
            temp.asString() = data.integer();
            break;
          }
          case test::extensions::filters::network::common::redis::RespData::
              kBulkString: {
            temp.type(RespType::BulkString);
            temp.asString() = data.bulk_string();
            break;
          }
          default:
            break;
        }
        respdata.push_back(temp);
      }
      RespValue v;
      v.type(RespType::Array);
      v.asArray().swap(respdata);
      values.push_back(v);
    }

    RespValue value;
    value.type(RespType::Array);
    value.asArray().swap(values);
    decoder_callback_impl.encoder_.encode(value, decoder_callback_impl.buffer_);
    try {
      decoder_callback_impl.decoder_.decode(decoder_callback_impl.buffer_);
      FUZZ_ASSERT(value.toString() ==
                  decoder_callback_impl.decoded_values_[0]->toString());
    } catch (Common::Redis::ProtocolError& e) {
    }
  }
};

DEFINE_PROTO_FUZZER(const test::extensions::filters::network::common::redis::
                        CodecArrayFuzzTestCase& input) {
  auto fuzzer = DecoderProtoFuzzer();
  fuzzer.Fuzz(input);
}

}  // namespace Fuzz
}  // namespace Redis
}  // namespace Common
}  // namespace NetworkFilters
}  // namespace Extensions
}  // namespace Envoy