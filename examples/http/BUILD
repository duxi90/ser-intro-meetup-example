cc_library(
    name = "types",
    srcs = ["types.hpp"],
)

cc_binary(
    name = "server",
    srcs = ["server.cpp"],
    deps = [
        "//intro",
        "//third_party/cli11",
        "//third_party/ctti",
        "//third_party/expected_lite",
        "//third_party/fmt",
        "//third_party/json",
        "//third_party/pistache",
        ":types",
    ],
)

cc_binary(
    name = "cli",
    srcs = ["cli.cpp"],
    deps = [
        "//intro",
        "//third_party/cli11",
        "//third_party/fmt",
        "//third_party/json",
        "//third_party/pistache",
        "//third_party/ctti",
        ":types",
    ],
)
