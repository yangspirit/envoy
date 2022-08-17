load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_fuzz_test",
    "envoy_cc_test_library",
    "envoy_package",
    "envoy_proto_library",
)

envoy_package()

envoy_cc_test_library(
    name = "decoder_lib",
    srcs = ["decoder_callback_impl.h"],
    deps = [
        "//source/extensions/filters/network/common/redis:codec_lib",
        "//source/common/buffer:buffer_lib",
    ],
)

envoy_cc_fuzz_test(
    name = "codec_bulk_string_fuzz_test",
    srcs = ["codec_bulk_string_fuzz_test.cc"],
    corpus = "codec_corpus",
    deps = [
        ":decoder_lib",
        "//source/common/common:assert_lib",
        "//test/fuzz:utility_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_fuzz_test(
    name = "codec_errors_fuzz_test",
    srcs = ["codec_errors_fuzz_test.cc"],
    corpus = "codec_corpus",
    deps = [
        ":decoder_lib",
        "//source/common/common:assert_lib",
        "//test/fuzz:utility_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_fuzz_test(
    name = "codec_integers_fuzz_test",
    srcs = ["codec_integers_fuzz_test.cc"],
    corpus = "codec_corpus",
    deps = [
        ":decoder_lib",
        "//source/common/common:assert_lib",
        "//test/fuzz:utility_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_fuzz_test(
    name = "codec_malformed_fuzz_test",
    srcs = ["codec_malformed_fuzz_test.cc"],
    corpus = "codec_corpus",
    deps = [
        ":decoder_lib",
        "//source/common/common:assert_lib",
        "//test/fuzz:utility_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_fuzz_test(
    name = "codec_simple_string_fuzz_test",
    srcs = ["codec_simple_string_fuzz_test.cc"],
    corpus = "codec_corpus",
    deps = [
        ":decoder_lib",
        "//source/common/common:assert_lib",
        "//test/fuzz:utility_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_proto_library(
    name = "codec_fuzz_proto",
    srcs = ["codec_fuzz.proto"],
    deps = ["//test/fuzz:common_proto"],
)

envoy_cc_fuzz_test(
    name = "codec_arrays_fuzz_test",
    srcs = ["codec_arrays_fuzz_test.cc"],
    corpus = "codec_corpus",
    deps = [
        ":codec_fuzz_proto_cc_proto",
        ":decoder_lib",
        "//source/common/common:assert_lib",
        "//test/fuzz:utility_lib",
        "//test/test_common:utility_lib",
    ],
)