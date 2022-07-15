licenses(["permissive"])

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "pistache_build",
    srcs = glob(["src/**/*.cc"]),
    hdrs = glob(
        [
            "include/**/*.h",
            "subprojects/hinnant-date/include/**/*.h",
        ],
        [
            "include/pistache/serializer/**/*.h",
        ],
    ),
    copts = [
        "-Wno-defaulted-function-deleted",
        "-Wno-unused-result",
    ],
    includes = [
        "include/",
        "subprojects/hinnant-date/include/",
    ],
)
