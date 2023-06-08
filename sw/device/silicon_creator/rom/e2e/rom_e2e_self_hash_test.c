// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdbool.h>

#include "sw/device/lib/base/memory.h"
#include "sw/device/lib/base/status.h"
#include "sw/device/lib/crypto/include/hash.h"
#include "sw/device/lib/testing/test_framework/check.h"
#include "sw/device/lib/testing/test_framework/ottf_main.h"

#include "hw/top_earlgrey/sw/autogen/top_earlgrey.h"
#include "hw/top_earlgrey/sw/autogen/top_earlgrey_memory.h"

OTTF_DEFINE_TEST_CONFIG();

enum {
  // Hash size.
  kSha256HashSizeInBits = 256,
  kSha256HashSizeInBytes = kSha256HashSizeInBits / 8,
  kSha256HashSizeIn32BitWords = kSha256HashSizeInBytes / 4,
};

// TODO(#18686): replace with real golden hashes (when real keys are generated).
/**
 * The golden ROM size and hashes expected below are generated using the
 * following instructions. If the ROM is updated, these values must also be
 * updated to prevent CI failures.
 *
 * 1. Build the ROM with Bazel using:
 *    `bazel build //sw/device/silicon_creator/rom:rom_with_real_keys`
 *    Note, this will build a separate ROM binary for each device, that can both
 *    be located with `./bazelisk.sh outquery-all
 *    //sw/device/silicon_creator/rom:rom_with_real_keys`, including:
 *    a. one for DV simulations: "rom_with_real_keys_sim_dv.bin", and
 *    b. one for CW310 FPGA: "rom_with_real_keys_fpga_cw310.bin".
 *
 * 2. Query and update the ROM size below (`kGoldenRomSizeBytes`) with:
 *    `stat -c %s <path to *.bin>`
 *
 * 3. Compute and update the golden ROM hashes below (`k*GoldenRomHash`) with:
 *    `sha256sum <path to *.bin> | cut -f1 -d' ' | \
 *       sed -e "s/......../0x&,\n/g" | tac`
 *    Note, make sure to reverse the byte order so order is little endian.
 */
const size_t kGoldenRomSizeBytes = 32652;
const uint32_t kSimDvGoldenRomHash[kSha256HashSizeIn32BitWords] = {
    // sha256sum rom_with_real_keys_sim_dv.bin | \
    // cut -f1 -d' ' | sed -e "s/......../0x&,\n/g" | tac
    0xaa23e75d, 0x45ebcbde, 0x39eaba7d, 0x302f3a60,
    0x0f205de0, 0x5c2a621c, 0x34f00f13, 0x6852995c,
};
const uint32_t kFpgaCw310GoldenRomHash[kSha256HashSizeIn32BitWords] = {
    // sha256sum rom_with_real_keys_fpga_cw310.bin | \
    // cut -f1 -d' ' | sed -e "s/......../0x&,\n/g" | tac
    0x5a351c76, 0xcbbcbbdd, 0x127f2ffd, 0x013eda23,
    0xd6f6dd15, 0x75f087eb, 0xf4952d1a, 0x845cf3dd,
};

// We hash the ROM using the SHA256 algorithm and print the hash to the console.
status_t hash_rom(void) {
  uint32_t rom_hash[kSha256HashSizeIn32BitWords];
  crypto_const_uint8_buf_t input = {
      .data = (uint8_t *)TOP_EARLGREY_ROM_BASE_ADDR,
      .len = kGoldenRomSizeBytes,
  };
  crypto_uint8_buf_t output = {
      .data = (uint8_t *)rom_hash,
      .len = kSha256HashSizeInBytes,
  };

  TRY(otcrypto_hash(input, kHashModeSha256, &output));
  LOG_INFO("ROM Hash: 0x%!x", kSha256HashSizeInBytes, rom_hash);
  if (kDeviceType == kDeviceSimDV) {
    TRY_CHECK_ARRAYS_EQ((uint8_t *)output.data, (uint8_t *)kSimDvGoldenRomHash,
                        sizeof(kSimDvGoldenRomHash));
  } else if (kDeviceType == kDeviceFpgaCw310) {
    TRY_CHECK_ARRAYS_EQ((uint8_t *)output.data,
                        (uint8_t *)kFpgaCw310GoldenRomHash,
                        sizeof(kFpgaCw310GoldenRomHash));
  } else {
    LOG_WARNING("ROM hash not self-checked for this device type: 0x%x",
                kDeviceType);
  }

  return OK_STATUS();
};

bool test_main(void) {
  status_t test_result = OK_STATUS();
  EXECUTE_TEST(test_result, hash_rom);
  return status_ok(test_result);
}
