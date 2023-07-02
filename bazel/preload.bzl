load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")


def cryptoTools_preload(repo_reference = ""):
  reference_index = ""
  if repo_reference:
      reference_index = "@{}".format(repo_reference)

  if "rules_foreign_cc" not in native.existing_rules():
    http_archive(
      name = "rules_foreign_cc",
      sha256 = "484fc0e14856b9f7434072bc2662488b3fe84d7798a5b7c92a1feb1a0fa8d088",
      strip_prefix = "rules_foreign_cc-0.8.0",
      url = "https://primihub.oss-cn-beijing.aliyuncs.com/tools/rules_foreign_cc_cn-0.8.0.tar.gz",
    )

  if "toolkit_relic" not in native.existing_rules():
    new_git_repository(
      name = "toolkit_relic",
      build_file = "{}//bazel:BUILD.relic".format(reference_index),
      remote = "https://gitee.com/orzmzp/relic.git",
      commit = "3f616ad64c3e63039277b8c90915607b6a2c504c",
      shallow_since = "1581106153 -0800",
    )

  if "com_github_nelhage_rules_boost" not in native.existing_rules():
    git_repository(
      name = "com_github_nelhage_rules_boost",
      commit = "81945736a62fa8490d2ab6bb31705bb04ce4bb6c",
      #branch = "master",
      remote = "https://gitee.com/primihub/rules_boost.git",
      # shallow_since = "1591047380 -0700",
    )

  if "github_com_span_lite" not in native.existing_rules():
    http_archive(
      name = "github_com_span_lite",
      build_file = "{}//bazel:BUILD.span_lite".format(reference_index),
      strip_prefix = "span-lite-0.10.3",
      urls = [
        "file:///home/cuibo/workspace/package/span-lite-0.10.3.tar.gz",
        "https://github.com/martinmoene/span-lite/archive/refs/tags/v0.10.3.tar.gz",
      ]
    )