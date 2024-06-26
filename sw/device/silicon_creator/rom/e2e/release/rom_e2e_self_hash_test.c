// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdbool.h>

#include "sw/device/lib/base/memory.h"
#include "sw/device/lib/base/status.h"
#include "sw/device/lib/testing/test_framework/check.h"
#include "sw/device/lib/testing/test_framework/ottf_main.h"
#include "sw/device/silicon_creator/lib/chip_info.h"
#include "sw/device/silicon_creator/lib/drivers/hmac.h"

#include "hw/top_earlgrey/sw/autogen/top_earlgrey.h"
#include "hw/top_earlgrey/sw/autogen/top_earlgrey_memory.h"

OTTF_DEFINE_TEST_CONFIG();

enum {
  // Hash size.
  kSha256HashSizeInBits = 256,
  kSha256HashSizeInBytes = kSha256HashSizeInBits / 8,
  kSha256HashSizeIn32BitWords = kSha256HashSizeInBytes / 4,
};

/**
 * The golden ROM size and hashes expected below are generated using the
 * following instructions. If the ROM is updated, these values must also be
 * updated to prevent CI failures.
 *
 * 1. Build the ROM and query the ROM hashes:
 *    bazel build //sw/device/silicon_creator/rom:rom_hashes
 *    cat bazel-bin/sw/device/silicon_creator/rom/rom_hashes.txt
 *
 * 2. Update the size and golden ROM hashes below (`k*GoldenRomHash`) by
 *    copying the little-endian-32 value arrays from the `rom_hashes.txt`
 *    report.
 */

const size_t kGoldenRomSizeBytes = 32652 - sizeof(chip_info_t);
const uint32_t kSimDvGoldenRomHash[kSha256HashSizeIn32BitWords] = {
    0xdc9a2c64, 0xcf6c75b2, 0x450cb695, 0x2a02780e,
    0x88df3501, 0x89c24073, 0x7d5baa94, 0xc8bbca20,
};
const uint32_t kFpgaCw310GoldenRomHash[kSha256HashSizeIn32BitWords] = {
    0x25fdea26, 0x8dbcfe81, 0x32a3b4ff, 0x059d3837,
    0xef644919, 0xf7a5f401, 0x0f53a45d, 0xc678ecd9,
};

extern const char _chip_info_start[];

// We hash the ROM using the SHA256 algorithm and print the hash to the console.
status_t hash_rom(void) {
  hmac_sha256_init();
  hmac_sha256_update((void *)TOP_EARLGREY_ROM_BASE_ADDR, kGoldenRomSizeBytes);
  hmac_digest_t rom_hash;
  hmac_sha256_final(&rom_hash);
  LOG_INFO("ROM Hash: 0x%08x%08x%08x%08x%08x%08x%08x%08x", rom_hash.digest[7],
           rom_hash.digest[6], rom_hash.digest[5], rom_hash.digest[4],
           rom_hash.digest[3], rom_hash.digest[2], rom_hash.digest[1],
           rom_hash.digest[0]);
  chip_info_t *rom_chip_info = (chip_info_t *)_chip_info_start;
  LOG_INFO("rom_chip_info @ %p:", rom_chip_info);
  LOG_INFO("scm_revision = %08x%08x",
           rom_chip_info->scm_revision.scm_revision_high,
           rom_chip_info->scm_revision.scm_revision_low);
  LOG_INFO("version = %08x", rom_chip_info->version);

  // TODO(#18868) Add checks for the chip_info values we expect to see in the
  // released ROM binary.

  if (kDeviceType == kDeviceSimDV) {
    TRY_CHECK_ARRAYS_EQ(rom_hash.digest, kSimDvGoldenRomHash,
                        ARRAYSIZE(kSimDvGoldenRomHash));
  } else if (kDeviceType == kDeviceFpgaCw310) {
    TRY_CHECK_ARRAYS_EQ(rom_hash.digest, kFpgaCw310GoldenRomHash,
                        ARRAYSIZE(kFpgaCw310GoldenRomHash));
  } else {
    LOG_ERROR("ROM hash not self-checked for this device type: 0x%x",
              kDeviceType);
    return INTERNAL();
  }

  return OK_STATUS();
};

bool test_main(void) {
  status_t test_result = OK_STATUS();
  EXECUTE_TEST(test_result, hash_rom);
  return status_ok(test_result);
}
