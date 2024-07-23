# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

load("//rules:const.bzl", "CONST", "hex")
load("//rules:linker.bzl", "ld_library")
load("//rules:manifest.bzl", "manifest")
load("//rules/opentitan:defs.bzl", "fpga_params", "opentitan_binary", "opentitan_test")
load(
    "//rules:otp.bzl",
    "STD_OTP_OVERLAYS",
    "otp_image",
)

package(default_visibility = ["//visibility:public"])

ld_library(
    name = "ld_common",
    includes = ["bare_metal_common.ld"],
    deps = [
        "//sw/device:info_sections",
        "//sw/device/silicon_creator/lib/base:static_critical_sections",
    ],
)

ld_library(
    name = "ld_slot_a",
    script = "bare_metal_slot_a.ld",
    deps = [
        ":ld_common",
        "//hw/top_earlgrey/sw/autogen:top_earlgrey_memory",
    ],
)

ld_library(
    name = "ld_slot_b",
    script = "bare_metal_slot_b.ld",
    deps = [
        ":ld_common",
        "//hw/top_earlgrey/sw/autogen:top_earlgrey_memory",
    ],
)

ld_library(
    name = "ld_slot_virtual",
    script = "bare_metal_slot_virtual.ld",
    deps = [
        ":ld_common",
        "//hw/top_earlgrey/sw/autogen:top_earlgrey_memory",
    ],
)

cc_library(
    name = "bare_metal",
    srcs = ["bare_metal.c"],
    hdrs = ["bare_metal.h"],
    deps = [
        "//sw/device/lib/base:macros",
        "//sw/device/silicon_creator/lib:dbg_print",
        "//sw/device/silicon_creator/lib:manifest_def",
        "//sw/device/silicon_creator/lib/base:static_critical",
    ],
)

manifest({
    "name": "manifest",
    "identifier": hex(CONST.OWNER),
})

opentitan_binary(
    name = "bare_metal_slot_a",
    testonly = True,
    srcs = ["bare_metal_start.S"],
    exec_env = [
        "//hw/top_earlgrey:fpga_cw310_rom_ext",
    ],
    linker_script = ":ld_slot_a",
    manifest = ":manifest",
    deps = [
        ":bare_metal",
        "//sw/device/lib/crt",
        "//sw/device/silicon_creator/lib:manifest_def",
    ],
)

opentitan_binary(
    name = "bare_metal_slot_b",
    testonly = True,
    srcs = ["bare_metal_start.S"],
    exec_env = [
        "//hw/top_earlgrey:fpga_cw310_rom_ext",
    ],
    linker_script = ":ld_slot_b",
    manifest = ":manifest",
    deps = [
        ":bare_metal",
        "//sw/device/lib/crt",
        "//sw/device/silicon_creator/lib:manifest_def",
    ],
)

opentitan_binary(
    name = "bare_metal_slot_virtual",
    testonly = True,
    srcs = ["bare_metal_start.S"],
    exec_env = [
        "//hw/top_earlgrey:fpga_cw310_rom_ext",
    ],
    linker_script = ":ld_slot_virtual",
    manifest = ":manifest",
    deps = [
        ":bare_metal",
        "//sw/device/lib/crt",
        "//sw/device/silicon_creator/lib:manifest_def",
    ],
)

################################################################################
# Bare metal ROM_EXT + BL0 test that DOES NOT use OTTF.
# - This test uses stand-alone binaries emitted by the opentitan_binary rule.
# - The test assembles an image containing the ROM_EXT from the exec_env and
#   the binary specified in the cw310 parameters.
################################################################################
BOOT_SUCCESS_MSG = "Bare metal PASS!"

opentitan_test(
    name = "rom_ext_virtual_bare_metal_virtual_boot_test",
    exec_env = {
        "//hw/top_earlgrey:fpga_cw310_rom_ext": None,
    },
    fpga = fpga_params(
        binaries = {
            ":bare_metal_slot_virtual": "firmware",
        },
        exit_success = BOOT_SUCCESS_MSG,
    ),
)

################################################################################
# Bare metal ROM_EXT + BL0 test that DOES use OTTF.
# - This test build a test program in the test rule and then assembles an image
#   containing the ROM_EXT from the exec_env an the compiled test program.
################################################################################
opentitan_test(
    name = "rom_ext_virtual_ottf_bl0_virtual",
    srcs = ["empty_test.c"],
    exec_env = {
        "//hw/top_earlgrey:fpga_cw310_rom_ext": None,
    },
    linker_script = "//sw/device/lib/testing/test_framework:ottf_ld_silicon_owner_slot_virtual",
    manifest = ":manifest",
    deps = [
        "//sw/device/lib/testing/test_framework:ottf_main",
    ],
)

################################################################################
# Owner SPX+ Sigverify Test
################################################################################

otp_image(
    name = "otp_img_secret2_locked_dev_spx_enabled",
    src = "//hw/ip/otp_ctrl/data:otp_json_dev",
    overlays = STD_OTP_OVERLAYS + [
        "//sw/device/silicon_creator/rom_ext/e2e:otp_json_secret2_locked",
        "//sw/device/silicon_creator/rom/e2e/sigverify_spx:otp_json_sigverify_spx_enabled_true",
    ],
)

opentitan_binary(
    name = "bare_metal_slot_a_spx_signed",
    testonly = True,
    srcs = ["bare_metal_start.S"],
    exec_env = [
        "//hw/top_earlgrey:fpga_cw310_rom_ext",
    ],
    linker_script = ":ld_slot_a",
    manifest = ":manifest",
    rsa_key = {"//sw/device/silicon_creator/rom_ext/keys/fake:rom_ext_prod_private_key_0": "prod_key_0"},
    spx_key = {"//sw/device/silicon_creator/rom_ext/keys/fake:rom_ext_test_spx_private_key_0": "test_key_0"},
    deps = [
        ":bare_metal",
        "//sw/device/lib/crt",
        "//sw/device/silicon_creator/lib:manifest_def",
    ],
)

opentitan_test(
    name = "owner_bare_metal_spx",
    exec_env = {
        "//hw/top_earlgrey:fpga_cw310_rom_ext": None,
    },
    fpga = fpga_params(
        binaries = {
            ":bare_metal_slot_a_spx_signed": "firmware",
        },
        exit_success = BOOT_SUCCESS_MSG,
        otp = ":otp_img_secret2_locked_dev_spx_enabled",
    ),
)
