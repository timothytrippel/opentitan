# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

load("//rules:opentitan.bzl", "OPENTITAN_CPU")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "bitmaps",
    srcs = [
        "chrome_logo_bitmap.c",
        "chromeos_logo_bitmap.c",
        "google_logo_bitmap.c",
        "ot_logo_bitmap.c",
    ],
    hdrs = ["bitmaps.h"],
    target_compatible_with = [OPENTITAN_CPU],
)
