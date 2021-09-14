// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/lib/testing/test_framework/ottf_isrs.h"

#include "sw/device/lib/base/csr.h"
#include "sw/device/lib/base/macros.h"
#include "sw/device/lib/runtime/hart.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/testing/test_framework/FreeRTOSConfig.h"

static uint32_t get_mtval(void) {
  uint32_t mtval;
  CSR_READ(CSR_REG_MTVAL, &mtval);
  return mtval;
}

static noreturn void loop_forever(void) {
  while (true) {
    wait_for_interrupt();
  }
}

OT_ATTR_WEAK void ottf_exception_handler(void) {
  uint32_t mcause;
  CSR_READ(CSR_REG_MCAUSE, &mcause);

  switch ((ottf_exc_id_t)(mcause & kIdMax)) {
    case kInstrMisaligned:
      ottf_instr_misaligned_fault();
      break;
    case kInstrAccessFault:
      ottf_instr_access_fault();
      break;
    case kIllegalInstrFault:
      ottf_illegal_instr_fault();
      break;
    case kBreakpoint:
      ottf_breakpoint();
      break;
    case kLoadAccessFault:
      ottf_load_store_fault();
      break;
    case kStoreAccessFault:
      ottf_load_store_fault();
      break;
    case kMachineECall:
      ottf_machine_ecall();
      break;
    case kUserECall:
      ottf_user_ecall();
      break;
    default:
      LOG_INFO("Unknown exception triggered!");
      loop_forever();
  }
}

OT_ATTR_WEAK void ottf_instr_misaligned_fault(void) {
  LOG_INFO("Misaligned instruction, mtval holds the fault address.");
  LOG_INFO("MTVAL value is 0x%x", get_mtval());
  loop_forever();
}

OT_ATTR_WEAK void ottf_instr_access_fault(void) {
  LOG_INFO("Instruction access fault, mtval holds the fault address.");
  LOG_INFO("MTVAL value is 0x%x", get_mtval());
  loop_forever();
}

OT_ATTR_WEAK void ottf_illegal_instr_fault(void) {
  LOG_INFO("Illegal instruction fault, mtval shows the faulting instruction.");
  LOG_INFO("MTVAL value is 0x%x", get_mtval());
  loop_forever();
}

OT_ATTR_WEAK void ottf_breakpoint(void) {
  LOG_INFO("Breakpoint triggered, mtval holds the breakpoint address.");
  LOG_INFO("MTVAL value is 0x%x", get_mtval());
  loop_forever();
}

OT_ATTR_WEAK void ottf_load_store_fault(void) {
  LOG_INFO("Load/Store fault, mtval holds the fault address.");
  LOG_INFO("MTVAL value is 0x%x", get_mtval());
  loop_forever();
}

OT_ATTR_WEAK void ottf_machine_ecall(void) {
  LOG_INFO("Machine-mode environment call (syscall).");
  loop_forever();
}

OT_ATTR_WEAK void ottf_user_ecall(void) {
  LOG_INFO("User-mode environment call (syscall).");
  loop_forever();
}

OT_ATTR_WEAK void ottf_software_isr(void) {
  LOG_INFO("Software IRQ triggered!");
  loop_forever();
}

OT_ATTR_WEAK void ottf_timer_isr(void) {
  LOG_INFO("Timer IRQ triggered!");
  loop_forever();
}

OT_ATTR_WEAK void ottf_external_isr(void) {
  LOG_INFO("External IRQ triggered!");
  loop_forever();
}
