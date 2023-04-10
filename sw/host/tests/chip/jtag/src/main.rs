// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

use std::time::Duration;

use anyhow::Result;
use regex::Regex;
use structopt::StructOpt;

use opentitanlib::app::TransportWrapper;
use opentitanlib::chip::boolean::MultiBitBool8;
use opentitanlib::dif::lc_ctrl::{DifLcCtrlState, LcCtrlReg};
use opentitanlib::execute_test;
use opentitanlib::io::jtag::{JtagParams, JtagTap};
use opentitanlib::test_utils::init::InitializeTest;
use opentitanlib::uart::console::UartConsole;

use top_earlgrey::top_earlgrey_memory;

#[derive(Debug, StructOpt)]
struct Opts {
    #[structopt(flatten)]
    init: InitializeTest,
    #[structopt(flatten)]
    jtag: JtagParams,
}

fn test_openocd(opts: &Opts, transport: &TransportWrapper) -> Result<()> {
    // Reset the device and confirm that its lifecycle state is now RMA by
    // reading the "LCV:" line from the UART.
    let uart = transport.uart("console")?;

    transport.reset_target(
        opts.init.bootstrap.options.reset_delay,
        /*clear_uart_rx=*/ true,
    )?;
    const CONSOLE_TIMEOUT: Duration = Duration::from_secs(5);

    let mut console = UartConsole {
        timeout: Some(CONSOLE_TIMEOUT),
        exit_success: Some(Regex::new(r"PASS!")?),
        ..Default::default()
    };
    let result = console.interact(&*uart, None, Some(&mut std::io::stdout()))?;
    log::info!("result: {:?}", result);

    //
    // Test the RISC-V TAP
    //
    transport.reset_target_w_strappings(
        opts.init.bootstrap.options.reset_delay,
        /*clear_uart_rx=*/ true,
        &["PINMUX_TAP_RISCV"],
        /*hold_added_strappings=*/ true,
    )?;

    let jtag = transport.jtag(&opts.jtag)?;
    jtag.connect(JtagTap::RiscvTap)?;
    jtag.halt()?;
    // Definitions for hardware registers
    let lc_ctrl_base_addr = top_earlgrey_memory::TOP_EARLGREY_LC_CTRL_BASE_ADDR as u32;
    let lc_ctrl_transition_if_addr = lc_ctrl_base_addr + LcCtrlReg::ClaimTransitionIf.byte_offset();
    let lc_ctrl_state_addr = lc_ctrl_base_addr + LcCtrlReg::LcState.byte_offset();
    let lc_ctrl_transition_regwen_addr =
        lc_ctrl_base_addr + LcCtrlReg::TransitionRegwen.byte_offset();
    log::info!(
        "LC_CTRL_TRANSITION_IF_ADDR = {:x}",
        lc_ctrl_transition_if_addr
    );
    log::info!("LC_CTRL_STATE_ADDR = {:x}", lc_ctrl_state_addr);
    log::info!(
        "LC_CTRL_TRANSITION_REGWEN_ADDR = {:x}",
        lc_ctrl_transition_regwen_addr
    );
    // Test reads by checking the LC_STATE register
    let mut lc_ctrl_state = [0u32; 1];
    assert_eq!(
        jtag.read_memory32(lc_ctrl_state_addr, &mut lc_ctrl_state)?,
        1
    );
    assert_eq!(
        lc_ctrl_state[0],
        DifLcCtrlState::TestUnlocked0.redundant_encoding()
    );
    // Test writes by trying to claim the transitions mutex
    let mut lc_ctrl_transition_regwen = [0u32; 1];
    assert_eq!(
        jtag.read_memory32(
            lc_ctrl_transition_regwen_addr,
            &mut lc_ctrl_transition_regwen
        )?,
        1
    );
    assert_eq!(lc_ctrl_transition_regwen[0], 0);
    let mut lc_ctrl_transition_if = [u8::from(MultiBitBool8::True) as u32; 1];

    jtag.write_memory32(lc_ctrl_transition_if_addr, &lc_ctrl_transition_if)?;
    assert_eq!(
        jtag.read_memory32(
            lc_ctrl_transition_regwen_addr,
            &mut lc_ctrl_transition_regwen
        )?,
        1
    );
    assert_eq!(lc_ctrl_transition_regwen[0], 1);
    // Release mutex
    lc_ctrl_transition_if[0] = u8::from(MultiBitBool8::False) as u32;
    jtag.write_memory32(lc_ctrl_transition_if_addr, &lc_ctrl_transition_if)?;
    assert_eq!(
        jtag.read_memory32(
            lc_ctrl_transition_regwen_addr,
            &mut lc_ctrl_transition_regwen
        )?,
        1
    );
    assert_eq!(lc_ctrl_transition_regwen[0], 0);

    // Test bulk read/writes by reading the content of the RAM, then overwrite it with
    // known values and try to read-back, we restore the content afterwards
    let test_ram_addr = top_earlgrey_memory::TOP_EARLGREY_RAM_RET_AON_BASE_ADDR as u32;
    const SIZE: usize = 20;
    let mut ram = [0u8; SIZE];
    assert_eq!(jtag.read_memory(test_ram_addr, &mut ram)?, SIZE);
    log::info!("ram: {:?}", ram);
    let test_data = (0..SIZE as u8).collect::<Vec<u8>>();
    jtag.write_memory(test_ram_addr, &test_data)?;
    let mut test_data2 = [0u8; SIZE];
    assert_eq!(jtag.read_memory(test_ram_addr, &mut test_data2)?, SIZE);
    log::info!("ram: {:?}", test_data2);
    assert_eq!(test_data, test_data2);
    // restore RAM
    jtag.write_memory(test_ram_addr, &ram)?;
    jtag.resume()?;

    jtag.disconnect()?;

    //
    // Test the LC TAP
    //
    transport.reset_target_w_strappings(
        opts.init.bootstrap.options.reset_delay,
        /*clear_uart_rx=*/ true,
        &["PINMUX_TAP_LC"],
        /*hold_added_strappings=*/ true,
    )?;

    let jtag = transport.jtag(&opts.jtag)?;
    jtag.connect(JtagTap::LcTap)?;

    // Test reads by checking the LC_STATE register
    assert_eq!(
        jtag.read_lc_ctrl_reg(&LcCtrlReg::LcState)?,
        DifLcCtrlState::TestUnlocked0.redundant_encoding()
    );

    // Test writes by trying to claim the transitions mutex
    assert_eq!(jtag.read_lc_ctrl_reg(&LcCtrlReg::TransitionRegwen)?, 0x00);
    jtag.write_lc_ctrl_reg(
        &LcCtrlReg::ClaimTransitionIf,
        u8::from(MultiBitBool8::True) as u32,
    )?;
    assert_eq!(jtag.read_lc_ctrl_reg(&LcCtrlReg::TransitionRegwen)?, 0x01);
    jtag.write_lc_ctrl_reg(
        &LcCtrlReg::ClaimTransitionIf,
        u8::from(MultiBitBool8::False) as u32,
    )?;
    jtag.disconnect()?;

    Ok(())
}

fn main() -> Result<()> {
    let opts = Opts::from_args();
    opts.init.init_logging();
    let transport = opts.init.init_target()?;

    execute_test!(test_openocd, &opts, &transport);

    Ok(())
}
