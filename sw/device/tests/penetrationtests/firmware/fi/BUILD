# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ibex_fi",
    srcs = [
        "ibex_fi.S",
        "ibex_fi.c",
    ],
    hdrs = ["ibex_fi.h"],
    deps = [
        "//sw/device/lib/base:csr",
        "//sw/device/lib/base:memory",
        "//sw/device/lib/base:status",
        "//sw/device/lib/dif:flash_ctrl",
        "//sw/device/lib/dif:rv_core_ibex",
        "//sw/device/lib/runtime:log",
        "//sw/device/lib/testing:flash_ctrl_testutils",
        "//sw/device/lib/testing/test_framework:ujson_ottf",
        "//sw/device/lib/ujson",
        "//sw/device/sca/lib:sca",
        "//sw/device/tests/penetrationtests/firmware/lib:sca_lib",
        "//sw/device/tests/penetrationtests/json:ibex_fi_commands",
    ],
)

cc_library(
    name = "otbn_fi",
    srcs = ["otbn_fi.c"],
    hdrs = ["otbn_fi.h"],
    deps = [
        "//sw/device/lib/base:memory",
        "//sw/device/lib/base:status",
        "//sw/device/lib/crypto/drivers:otbn",
        "//sw/device/lib/dif:otbn",
        "//sw/device/lib/runtime:log",
        "//sw/device/lib/testing:entropy_testutils",
        "//sw/device/lib/testing/test_framework:ujson_ottf",
        "//sw/device/lib/ujson",
        "//sw/device/sca/lib:sca",
        "//sw/device/tests/penetrationtests/firmware/fi/otbn:otbn_char_hardware_dmem_op_loop",
        "//sw/device/tests/penetrationtests/firmware/fi/otbn:otbn_char_hardware_reg_op_loop",
        "//sw/device/tests/penetrationtests/firmware/fi/otbn:otbn_char_unrolled_dmem_op_loop",
        "//sw/device/tests/penetrationtests/firmware/fi/otbn:otbn_char_unrolled_reg_op_loop",
        "//sw/device/tests/penetrationtests/json:otbn_fi_commands",
    ],
)
