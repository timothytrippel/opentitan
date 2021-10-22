// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef OPENTITAN_SW_DEVICE_LIB_DIF_AUTOGEN_DIF_SYSRST_CTRL_AUTOGEN_H_
#define OPENTITAN_SW_DEVICE_LIB_DIF_AUTOGEN_DIF_SYSRST_CTRL_AUTOGEN_H_

// This file is auto-generated.

/**
 * @file
 * @brief <a href="/hw/ip/sysrst_ctrl/doc/">SYSRST_CTRL</a> Device Interface
 * Functions
 */

#include <stdbool.h>
#include <stdint.h>

#include "sw/device/lib/base/macros.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/dif/dif_base.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * A handle to sysrst_ctrl.
 *
 * This type should be treated as opaque by users.
 */
typedef struct dif_sysrst_ctrl {
  /**
   * The base address for the sysrst_ctrl hardware registers.
   */
  mmio_region_t base_addr;
} dif_sysrst_ctrl_t;

/**
 * Creates a new handle for a(n) sysrst_ctrl peripheral.
 *
 * This function does not actuate the hardware.
 *
 * @param base_addr The MMIO base address of the sysrst_ctrl peripheral.
 * @param[out] sysrst_ctrl Out param for the initialized handle.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_init(mmio_region_t base_addr,
                                  dif_sysrst_ctrl_t *sysrst_ctrl);

/**
 * A sysrst_ctrl interrupt request type.
 */
typedef enum dif_sysrst_ctrl_irq {
  /**
   * Common interrupt triggered by combo or keyboard actions
   */
  kDifSysrstCtrlIrqSysrstCtrl = 0,
} dif_sysrst_ctrl_irq_t;

/**
 * A snapshot of the state of the interrupts for this IP.
 *
 * This is an opaque type, to be used with the `dif_sysrst_ctrl_irq_get_state()`
 * function.
 */
typedef uint32_t dif_sysrst_ctrl_irq_state_snapshot_t;

/**
 * Returns whether a particular interrupt is currently pending.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @param[out] snapshot Out-param for interrupt state snapshot.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_get_state(
    const dif_sysrst_ctrl_t *sysrst_ctrl,
    dif_sysrst_ctrl_irq_state_snapshot_t *snapshot);

/**
 * Returns whether a particular interrupt is currently pending.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @param irq An interrupt request.
 * @param[out] is_pending Out-param for whether the interrupt is pending.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_is_pending(
    const dif_sysrst_ctrl_t *sysrst_ctrl, dif_sysrst_ctrl_irq_t irq,
    bool *is_pending);

/**
 * Acknowledges all interrupts, indicating to the hardware that all
 * interrupts have been successfully serviced.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_acknowledge_all(
    const dif_sysrst_ctrl_t *sysrst_ctrl);

/**
 * Acknowledges a particular interrupt, indicating to the hardware that it has
 * been successfully serviced.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @param irq An interrupt request.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_acknowledge(
    const dif_sysrst_ctrl_t *sysrst_ctrl, dif_sysrst_ctrl_irq_t irq);

/**
 * Forces a particular interrupt, causing it to be serviced as if hardware had
 * asserted it.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @param irq An interrupt request.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_force(const dif_sysrst_ctrl_t *sysrst_ctrl,
                                       dif_sysrst_ctrl_irq_t irq);

/**
 * A snapshot of the enablement state of the interrupts for this IP.
 *
 * This is an opaque type, to be used with the
 * `dif_sysrst_ctrl_irq_disable_all()` and `dif_sysrst_ctrl_irq_restore_all()`
 * functions.
 */
typedef uint32_t dif_sysrst_ctrl_irq_enable_snapshot_t;

/**
 * Checks whether a particular interrupt is currently enabled or disabled.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @param irq An interrupt request.
 * @param[out] state Out-param toggle state of the interrupt.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_get_enabled(
    const dif_sysrst_ctrl_t *sysrst_ctrl, dif_sysrst_ctrl_irq_t irq,
    dif_toggle_t *state);

/**
 * Sets whether a particular interrupt is currently enabled or disabled.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @param irq An interrupt request.
 * @param state The new toggle state for the interrupt.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_set_enabled(
    const dif_sysrst_ctrl_t *sysrst_ctrl, dif_sysrst_ctrl_irq_t irq,
    dif_toggle_t state);

/**
 * Disables all interrupts, optionally snapshotting all enable states for later
 * restoration.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @param[out] snapshot Out-param for the snapshot; may be `NULL`.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_disable_all(
    const dif_sysrst_ctrl_t *sysrst_ctrl,
    dif_sysrst_ctrl_irq_enable_snapshot_t *snapshot);

/**
 * Restores interrupts from the given (enable) snapshot.
 *
 * @param sysrst_ctrl A sysrst_ctrl handle.
 * @param snapshot A snapshot to restore from.
 * @return The result of the operation.
 */
OT_WARN_UNUSED_RESULT
dif_result_t dif_sysrst_ctrl_irq_restore_all(
    const dif_sysrst_ctrl_t *sysrst_ctrl,
    const dif_sysrst_ctrl_irq_enable_snapshot_t *snapshot);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // OPENTITAN_SW_DEVICE_LIB_DIF_AUTOGEN_DIF_SYSRST_CTRL_AUTOGEN_H_
