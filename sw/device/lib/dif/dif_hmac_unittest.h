// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef OPENTITAN_SW_DEVICE_LIB_DIF_DIF_HMAC_UNITTEST_H_
#define OPENTITAN_SW_DEVICE_LIB_DIF_DIF_HMAC_UNITTEST_H_

#include "gtest/gtest.h"
#include "sw/device/lib/base/testing/mock_mmio.h"
#include "sw/device/lib/dif/dif_hmac.h"

#include "hmac_regs.h"  // Generated.

namespace dif_hmac_unittest {
namespace {

using ::mock_mmio::MmioTest;
using ::mock_mmio::MockDevice;
using ::testing::Test;

class HmacTest : public Test, public MmioTest {
 protected:
  dif_hmac_t hmac_ = {.base_addr = dev().region()};
};

}  // namespace
}  // namespace dif_hmac_unittest

#endif  // OPENTITAN_SW_DEVICE_LIB_DIF_DIF_HMAC_UNITTEST_H_
