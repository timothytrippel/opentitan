// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/lib/base/memory.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/dif/dif_entropy_src.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/testing/check.h"
#include "sw/device/lib/testing/entropy_testutils.h"
#include "sw/device/lib/testing/test_framework/ottf.h"

#include "hw/top_earlgrey/sw/autogen/top_earlgrey.h"

const test_config_t kTestConfig;

const size_t kEntropyDataNumWords = 12;

// This expected value seems to require frequent updating.  If it turns
// out that changes to the overall system start causing race conditions
// we may want to relax the requirement for exact binary equivalence.
//
// Issue #10092 has been opened to discuss this.

const uint32_t kExpectedEntropyData[] = {
    0x7c7baa24, 0x4e3f30f8, 0xcae3aefe, 0x80197526, 0xbb8761a4, 0x18b5b15f,
    0xe8126c66, 0x69efaf13, 0x8cc59ac2, 0xb7f3bcad, 0x06dc9089, 0x1f4c6ef2,
};

bool test_main() {
  dif_entropy_src_t entropy_src;
  CHECK_DIF_OK(dif_entropy_src_init(
      mmio_region_from_addr(TOP_EARLGREY_ENTROPY_SRC_BASE_ADDR), &entropy_src));

  entropy_testutils_entropy_src_disable(entropy_src);

  uint32_t entropy_data[kEntropyDataNumWords];
  uint32_t result = 0;
  for (uint32_t i = 0; i < kEntropyDataNumWords; ++i) {
    // wait for entropy to become available
    while (dif_entropy_src_read(&entropy_src, &entropy_data[i]) != kDifOk)
      ;
    LOG_INFO("received 0x%x, expected 0x%x", entropy_data[i],
             kExpectedEntropyData[i]);
    result |= entropy_data[i] ^ kExpectedEntropyData[i];
  }

  return result == 0;
}
