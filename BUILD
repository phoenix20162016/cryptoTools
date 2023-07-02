package(default_visibility = ["//visibility:public"])

#load("@rules_foreign_cc//tools/build_defs:boost_build.bzl", "boost_build")

#boost_build(
#    name = "boost_fiber",
#    lib_source = "@boost//:all",
#    static_libraries = ["libboost_fiber.a"],
#    user_options = ["--with-fiber"],
#    visibility = ["//visibility:public"],
#    deps = [":boost_context"],
#)

#boost_build(
#    name = "boost_context",
#    lib_source = "@boost//:all",
#    static_libraries = ["libboost_context.a"],
#    user_options = ["--with-context"],
#    visibility = ["//visibility:public"],
#)

genrule(
  name = "cryptoTools_config_h",
  outs = [
    "cryptoTools/Common/config.h",
  ],
  cmd = """
      set -x
      tmpdir="cryptoTools.tmp"
      mkdir -p "$${tmpdir}/cryptoTools/Common"
      echo "#pragma once \r\n \
#define ENABLE_RELIC ON \r\n \
#define ENABLE_CIRCUITS ON \r\n \
#define ENABLE_SPAN_LITE ON \r\n \
#define ENABLE_CPP_14 ON \r\n \
#define ENABLE_BOOST ON \r\n \
#define ENABLE_CPP_14 ON \r\n \
#define ENABLE_NET_LOG ON \r\n \
#define ENABLE_NASM ON \r\n \
#if (defined(_MSC_VER) || defined(__SSE2__)) && defined(ENABLE_SSE) \r\n \
#define ENABLE_SSE_BLAKE2 ON \r\n \
#define OC_ENABLE_SSE2 ON \r\n \
#endif \r\n \
#if (defined(_MSC_VER) || defined(__PCLMUL__)) && defined(ENABLE_SSE) \r\n \
#define OC_ENABLE_PCLMUL \r\n \
#endif \r\n \
#if (defined(_MSC_VER) || defined(__AES__)) && defined(ENABLE_SSE) \r\n \
#define OC_ENABLE_AESNI ON \r\n \
#else \r\n \
#define OC_ENABLE_PORTABLE_AES OFF \r\n \
#endif \r\n \
">"$${tmpdir}"/cryptoTools/Common/config.h
        ls -ltrh "$${tmpdir}"
        mv "$${tmpdir}"/cryptoTools/Common/config.h $(location cryptoTools/Common/config.h)
        rm -r -f -- "$${tmpdir}"
    """,
  visibility = ["//visibility:public"],
)

ENABLE_SSE_COPT = [
  "-DENABLE_SSE=ON",
  "-maes",
  "-msse2",
  "-msse3",
  "-msse4.1",
  "-mpclmul",
]

ENABLE_RELIC_COPT = [
  #"-I@toolkit_relic//:relic/include",
  "-DENABLE_RELIC=ON",
]

ENABLE_RELIC_DEPS = [
  "@toolkit_relic//:relic",
]

cc_library(
  name = "libcryptoTools",
  srcs = glob([
    "cryptoTools/Circuit/*.cpp",
    "cryptoTools/Common/*.cpp",
    "cryptoTools/Crypto/*.cpp",
    "cryptoTools/Crypto/blake2/c/*.c",
    "cryptoTools/Crypto/blake2/sse/*.c",
    "cryptoTools/Network/*.cpp"],
  ),
  hdrs = [":cryptoTools_config_h"] +
    glob([
      "cryptoTools/Circuit/*.h",
      "cryptoTools/Common/*.h",
      "cryptoTools/Common/*.hpp",
      "cryptoTools/Crypto/*.h",
      "cryptoTools/Crypto/blake2/c/*.h",
      "cryptoTools/Crypto/blake2/sse/*.h",
      "cryptoTools/Network/*.h"
    ]),
  includes = [
    #"cryptoTools",
    ":cryptoTools_config_h"
  ],
  copts = [
    "-O0 -g -ggdb -rdynamic",
    "-DENABLE_CIRCUITS=ON",
    "-DENABLE_BOOST=ON",
    "-DRAND=HASHD",
    "-DMULTI=PTHREAD",
    "-DBoost_USE_MULTITHREADED=ON"
  ] + ENABLE_SSE_COPT + ENABLE_RELIC_COPT,
  linkopts = ["-pthread"],
  # strip_include_prefix = "src",
  # Using an empty include_prefix causes Bazel to emit -I instead of -iquote
  # options for the include directory, so that #include <gmp.h> works.
  include_prefix = "",
  linkstatic = True,
  deps = [
    "@boost//:fiber",
    "@boost//:asio",
    "@boost//:variant",
    "@boost//:multiprecision",
    "@boost//:system",
    "@boost//:circular_buffer",
    "@github_com_span_lite//:span_lite",
    "@github_com_libdivide//:libdivide",
  ] + ENABLE_RELIC_DEPS,
)

cc_library(
  name = "tests_cryptoTools",
  srcs = glob([
    "tests_cryptoTools/**/*.cpp"
  ]),
  hdrs = glob([
    "tests_cryptoTools/**/*.h"
  ]),
  copts = [
    " -O0 -g -ggdb -rdynamic",
    "-DENABLE_CIRCUITS=ON",
    "-DENABLE_BOOST=ON",
    "-DBoost_USE_MULTITHREADED=ON"
  ] + ENABLE_SSE_COPT + ENABLE_RELIC_COPT,
  linkopts = ["-pthread"],
  linkstatic = True,
  deps = [
    ":cryptoTools",
  ] + ENABLE_RELIC_DEPS,
)

cc_library(
  name = "lib_frontend_cryptoTools",
  srcs = glob([
    "frontend_cryptoTools/**/*.cpp"
  ]),
  hdrs = glob([
    "frontend_cryptoTools/**/*.h"
  ],),
  copts = [
    " -O0 -g -ggdb -rdynamic",
    "-DENABLE_CIRCUITS=ON",
    "-DENABLE_BOOST=ON",
    "-DBoost_USE_MULTITHREADED=ON",
    "-DENABLE_FULL_GSL=ON",
    "-DENABLE_CPP_14=ON"
  ] + ENABLE_SSE_COPT + ENABLE_RELIC_COPT,
  linkopts = ["-pthread -lstdc++"],
  linkstatic = True,
  deps = [
    ":cryptoTools",
    ":tests_cryptoTools",
  ] + ENABLE_RELIC_DEPS,
)

cc_binary(
  name = "frontend_cryptoTools",
  srcs = glob([
    "frontend_cryptoTools/**/*.cpp",
    "frontend_cryptoTools/**/*.h",
  ]),
  includes = [".", "Tutorials/Network.h"],
  copts = [
    "-O0 -g -ggdb -rdynamic",
    "-DENABLE_CIRCUITS=ON",
    "-DENABLE_BOOST=ON",
  ] + ENABLE_SSE_COPT + ENABLE_RELIC_COPT,
  linkopts = ["-pthread -lstdc++"],
  linkstatic = False,
  deps = [
    ":lib_frontend_cryptoTools",
    ":cryptoTools",
    ":tests_cryptoTools",
  ] + ENABLE_RELIC_DEPS,
)

