package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

pkg_tar(
    name = "ct-libtensorflow",
    extension = "tar.gz",
    tags = ["manual"],
    deps = [
        ":ct-cheaders",
        ":ct-clib"
    ],
)

pkg_tar(
    name = "ct-cheaders",
    files = ["//tensorflow/c:headers"],
    package_dir = "include/tensorflow/c",
    tags = ["manual"],
)

pkg_tar(
    name = "ct-clib",
    files = ["//tensorflow:libtensorflow.so"],
    package_dir = "lib",
    tags = ["manual"],
)


# This is the entry point for --crosstool_top.
#
# The cc_toolchain rule used is found by:
#
# 1. Finding the appropriate toolchain in the CROSSTOOL file based on the --cpu
#    and --compiler command line flags (if they exist, otherwise using the
#    "default_target_cpu" / "default_toolchain" fields in the CROSSTOOL file)
# 2. Concatenating the "target_cpu" and "compiler" fields of the toolchain in
#    use and using that as a key in the map in the "toolchains" attribute

cc_toolchain_suite(
  name = 'toolchain',
  toolchains = {
    'darwin|clang': ':cc-compiler-clang-llvm',
  },
)


cc_toolchain(
    name = "cc-compiler-clang-llvm",
    all_files = ":empty",
    compiler_files = ":empty",
    cpu = "darwin",
    dwp_files = ":empty",
    dynamic_runtime_libs = [":empty"],
    linker_files = ":empty",
    objcopy_files = ":empty",
    static_runtime_libs = [":empty"],
    strip_files = ":empty",
    supports_param_files = 1,
)
