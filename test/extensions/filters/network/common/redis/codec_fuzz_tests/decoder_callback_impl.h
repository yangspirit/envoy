#ifndef THIRD_PARTY_ENVOY_MEMORYSTORE_TEST_EXTENSIONS_FILTERS_NETWORK_COMMON_REDIS_CODEC_FUZZ_TESTS_DECODER_CALLBACK_IMPL_H_
#define THIRD_PARTY_ENVOY_MEMORYSTORE_TEST_EXTENSIONS_FILTERS_NETWORK_COMMON_REDIS_CODEC_FUZZ_TESTS_DECODER_CALLBACK_IMPL_H_

#include "source/extensions/filters/network/common/redis/codec_impl.h"
#include "source/common/buffer/buffer_impl.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace Common {
namespace Redis {
namespace Fuzz {

class DecoderCallbackImpl : public DecoderCallbacks {
 public:
  DecoderCallbackImpl(uint64_t resp_data_size_limit = UINT64_MAX)
      : decoder_(*this, resp_data_size_limit) {}

  // RedisProxy::DecoderCallbacks
  void onRespValue(RespValuePtr&& value) override {
    decoded_values_.emplace_back(std::move(value));
  }

  EncoderImpl encoder_;
  DecoderImpl decoder_;
  Buffer::OwnedImpl buffer_;
  std::vector<RespValuePtr> decoded_values_;
};

}  // namespace Fuzz
}  // namespace Redis
}  // namespace Common
}  // namespace NetworkFilters
}  // namespace Extensions
}  // namespace Envoy

#endif  // THIRD_PARTY_ENVOY_MEMORYSTORE_TEST_EXTENSIONS_FILTERS_NETWORK_COMMON_REDIS_CODEC_FUZZ_TESTS_DECODER_CALLBACK_IMPL_H_