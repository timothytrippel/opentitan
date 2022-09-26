# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

load(
    "@io_bazel_rules_go//go:deps.bzl",
    "go_download_sdk",
    "go_register_toolchains",
    "go_rules_dependencies",
)
load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies")

def go_deps():
    go_download_sdk(
        name = "go_sdk",
        goos = "linux",
        goarch = "amd64",
        version = "1.18.1",
        sdks = {
            # NOTE: In most cases the whole sdks attribute is not needed.
            # There are 2 "common" reasons you might want it:
            #
            # 1. You need to use an modified GO SDK, or an unsupported version
            #    (for example, a beta or release candidate)
            #
            # 2. You want to avoid the dependency on the index file for the
            #    SHA-256 checksums. In this case, You can get the expected
            #    filenames and checksums from https://go.dev/dl/
            "linux_amd64": ("go1.18.1.linux-amd64.tar.gz", "b3b815f47ababac13810fc6021eb73d65478e0b2db4b09d348eefad9581a2334"),
            "darwin_amd64": ("go1.18.1.darwin-amd64.tar.gz", "3703e9a0db1000f18c0c7b524f3d378aac71219b4715a6a4c5683eb639f41a4d"),
        },
    )
    go_register_toolchains()
    go_rules_dependencies()
    gazelle_dependencies()
