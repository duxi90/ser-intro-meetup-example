load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def com_github_CLIUtils_CLI11_repository():
    ## CLI11 command line library
    tag = "2.1.2"
    http_archive(
        name = "com_github_CLIUtils_CLI11",
        build_file = Label("//third_party/cli11:bazel.BUILD"),
        sha256 = "26291377e892ba0e5b4972cdfd4a2ab3bf53af8dac1f4ea8fe0d1376b625c8cb",
        strip_prefix = "CLI11-{}".format(tag),
        urls = ["https://github.com/CLIUtils/CLI11/archive/v{}.tar.gz".format(tag)],
    )

def com_github_manu343726_ctti_repository():
    ## Compile Time Type Information for C++
    new_git_repository(
        name = "com_github_manu343726_ctti",
        build_file = Label("//third_party/ctti:bazel.BUILD"),
        # TODO 493: Update to 0.0.2 once it comes out
        commit = "d7e9828b82ce7a6321465fbd84f9bccb772c7f43",
        shallow_since = "1547901997 +0100",
        remote = "https://github.com/Manu343726/ctti.git",
    )

def com_github_nlohmann_json_repository():
    ## JSON
    http_archive(
        name = "com_github_nlohmann_json",
        build_file = Label("//third_party/json:bazel.BUILD"),
        sha256 = "6bea5877b1541d353bd77bdfbdb2696333ae5ed8f9e8cc22df657192218cad91",
        urls = ["https://github.com/nlohmann/json/releases/download/v3.9.1/include.zip"],
    )

def com_github_jbeder_yaml_cpp_repository():
    ## YAML-CPP
    tag = "0.6.3"
    http_archive(
        name = "com_github_jbeder_yaml_cpp",
        build_file = Label("//third_party/yaml_cpp:bazel.BUILD"),
        sha256 = "77ea1b90b3718aa0c324207cb29418f5bced2354c2e483a9523d98c3460af1ed",
        strip_prefix = "yaml-cpp-yaml-cpp-{}".format(tag),
        urls = ["https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-{}.tar.gz".format(tag)],
    )

def com_github_fmtlib_fmt_repository():
    ## fmtlib (std::format)
    tag = "8.1.1"
    http_archive(
        name = "com_github_fmtlib_fmt",
        sha256 = "f2aad1a340d27c0b22cf23268c1cbdffb8472a242b95daf2d2311eed8d4948fc",
        strip_prefix = "fmt-{}".format(tag),
        # from https://github.com/fmtlib/fmt/tree/b6f4ceaed0a0a24ccf575fab6c56dd50ccf6f1a9/support/bazel#using-fmt-as-a-dependency
        patch_cmds = [
            "mv support/bazel/.bazelrc .bazelrc",
            "mv support/bazel/.bazelversion .bazelversion",
            "mv support/bazel/BUILD.bazel BUILD.bazel",
            "mv support/bazel/WORKSPACE.bazel WORKSPACE.bazel",
        ],
        patch_cmds_win = [
            "Move-Item -Path support/bazel/.bazelrc -Destination .bazelrc",
            "Move-Item -Path support/bazel/.bazelversion -Destination .bazelversion",
            "Move-Item -Path support/bazel/BUILD.bazel -Destination BUILD.bazel",
            "Move-Item -Path support/bazel/WORKSPACE.bazel -Destination WORKSPACE.bazel",
        ],
        urls = ["https://github.com/fmtlib/fmt/archive/{}.zip".format(tag)],
    )

def com_github_martinmoene_expected_lite_repository():
    ## Expected lite (std::expected)
    tag = "0.6.1"
    http_archive(
        name = "com_github_martinmoene_expected_lite",
        build_file = Label("//third_party/expected_lite:bazel.BUILD"),
        sha256 = "91724069aaab7491d48ac0711f9af7fd4fd1a3b0f95406c3a773b7944e67f125",
        strip_prefix = "expected-lite-{}".format(tag),
        urls = ["https://github.com/martinmoene/expected-lite/archive/refs/tags/v{}.tar.gz".format(tag)],
    )

def com_github_pistacheio_pistache_repository():
    ## Pistacheio Pistache library
    commit = "3ec9d7c4f8b828fdd391550fff81b01e72dd6269"
    http_archive(
        name = "com_github_pistacheio_pistache",
        build_file = "@//third_party/pistache:bazel.BUILD",
        sha256 = "90b530113b9d20fdfcb6f664f63e03de1dc255c7f26b2288de64b0d251b9ea52",
        strip_prefix = "pistache-{}".format(commit),
        urls = ["https://github.com/pistacheio/pistache/archive/{}.zip".format(commit)],
    )

def cpp_repositories(excludes = []):
    ##########################################
    # C++ utility libraries

    if "com_github_CLIUtils_CLI11" not in excludes:
        com_github_CLIUtils_CLI11_repository()

    if "com_github_manu343726_ctti" not in excludes:
        com_github_manu343726_ctti_repository()

    ##########################################
    # C++ Data serialization

    if "com_github_nlohmann_json" not in excludes:
        com_github_nlohmann_json_repository()

    if "com_github_jbeder_yaml_cpp" not in excludes:
        com_github_jbeder_yaml_cpp_repository()

    ##########################################
    # C++ STD future

    if "com_github_fmtlib_fmt" not in excludes:
        com_github_fmtlib_fmt_repository()

    if "com_github_martinmoene_expected_lite" not in excludes:
        com_github_martinmoene_expected_lite_repository()

    ##########################################
    # HTTP

    if "com_github_pistacheio_pistache" not in excludes:
        com_github_pistacheio_pistache_repository()
