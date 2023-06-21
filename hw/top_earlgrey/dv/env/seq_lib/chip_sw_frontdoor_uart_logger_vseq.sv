// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

class chip_sw_frontdoor_uart_logger_vseq extends
  chip_sw_base_vseq;
  `uvm_object_utils(chip_sw_frontdoor_uart_logger_vseq)
  `uvm_object_new

  virtual task body();
    super.body();

    // Wait until we are done executing in ROM to connect the UART agent, as
    // we want to avoid multiple drivers when we are sampling the DFT TAP
    // straps.
    `DV_WAIT(cfg.sw_test_status_vif.sw_test_status == SwTestStatusInTest);
    configure_uart_agent(.uart_idx(ROM_CONSOLE_UART), .enable(1), .enable_tx_monitor(1), .enable_rx_monitor(1));

    // Spawn off a thread to retrieve UART TX items.
    fork get_uart_tx_items(ROM_CONSOLE_UART); join_none
  endtask

  virtual task post_start();
    super.post_start();
    // Disconnect UART agent.
    cfg.m_uart_agent_cfgs[ROM_CONSOLE_UART].en_tx_monitor = 0;
    cfg.m_uart_agent_cfgs[ROM_CONSOLE_UART].en_rx_monitor = 0;
  endtask

endclass : chip_sw_frontdoor_uart_logger_vseq
