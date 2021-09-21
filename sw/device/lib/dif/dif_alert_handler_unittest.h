// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef OPENTITAN_SW_DEVICE_LIB_DIF_DIF_ALERT_HANDLER_UNITTEST_H_
#define OPENTITAN_SW_DEVICE_LIB_DIF_DIF_ALERT_HANDLER_UNITTEST_H_

#include "gtest/gtest.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/base/testing/mock_mmio.h"
#include "sw/device/lib/dif/dif_alert_handler.h"

#include "alert_handler_regs.h"  // Generated.

namespace dif_alert_handler_unittest {
namespace {

using ::mock_mmio::LeInt;
using ::mock_mmio::MmioTest;
using ::mock_mmio::MockDevice;
using ::testing::_;
using ::testing::Return;

constexpr uint32_t kAlerts = ALERT_HANDLER_PARAM_N_ALERTS;
constexpr uint32_t kAllZeros = 0;

class AlertHandlerTest : public testing::Test, public MmioTest {
 protected:
  dif_alert_handler_t alert_handler_ = {.base_addr = dev().region()};

  dif_alert_handler_params_t params_ = {
      .alert_count = kAlerts,
      .escalation_signal_count = 4,
  };
};

}  // namespace
}  // namespace dif_alert_handler_unittest

#endif  // OPENTITAN_SW_DEVICE_LIB_DIF_DIF_ALERT_HANDLER_UNITTEST_H_
