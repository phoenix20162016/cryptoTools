workspace(name = "cryptotools")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

http_archive(
  name = "rules_foreign_cc",
  sha256 = "484fc0e14856b9f7434072bc2662488b3fe84d7798a5b7c92a1feb1a0fa8d088",
  strip_prefix = "rules_foreign_cc-0.8.0",
  url = "https://primihub.oss-cn-beijing.aliyuncs.com/tools/rules_foreign_cc_cn-0.8.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

new_git_repository(
  name = "toolkit_relic",
  build_file = "//bazel:BUILD.relic",
  remote = "https://gitee.com/orzmzp/relic.git",
  commit = "3f616ad64c3e63039277b8c90915607b6a2c504c",
  shallow_since = "1581106153 -0800",
)

git_repository(
  name = "com_github_nelhage_rules_boost",
  commit = "81945736a62fa8490d2ab6bb31705bb04ce4bb6c",
  #branch = "master",
  remote = "https://gitee.com/primihub/rules_boost.git",
  # shallow_since = "1591047380 -0700",
)
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

http_archive(
  name = "github_com_span_lite",
  build_file = "//bazel:BUILD.span_lite",
  strip_prefix = "span-lite-0.10.3",
  urls = [
    "file:///home/cuibo/workspace/package/span-lite-0.10.3.tar.gz",
    "https://github.com/martinmoene/span-lite/archive/refs/tags/v0.10.3.tar.gz",
  ]
)

http_archive(
  name = "github_com_libdivide",
  build_file = "//bazel:BUILD.libdivide",
  strip_prefix = "libdivide-5.0",
  urls = [
    "file:///home/cuibo/workspace/package/libdivide-5.0.tar.gz",
    "https://github.com/ridiculousfish/libdivide/archive/refs/tags/5.0.tar.gz",
  ]
)