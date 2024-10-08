// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// This sequence ensures that longer transfers are exercised.

class dma_longer_transfer_vseq extends dma_generic_vseq;
  `uvm_object_utils(dma_longer_transfer_vseq)
  `uvm_object_new

  // Longer transfers are apt to take a long time; rely upon the parallelism of multiple seeds.
  constraint iters_c {num_iters == 1;}
  constraint transactions_c {num_txns == 1;}

  // Permit only valid configurations for this test; invalid configurations are generated by stress
  // sequences.
  virtual function bit pick_if_config_valid();
    return 1'b1;
  endfunction

  // Randomization of DMA configuration and transfer properties; here we are restricting the
  // permissible configuration and transfers to just very short transfers, but all source and
  // destination types.
  //
  // We also deliberately leave inline operations such as SHA Digest enabled, but being careful
  // to ensure that the base constraints will enable it only if its requirements are met.
  virtual function void randomize_item(ref dma_seq_item dma_config);
    bit [31:0] n = $urandom;  // Uniform distribution.
    bit [31:0] range_max = 'h1800;
    bit [31:0] range_min = 'h1000;
    // Decide upon the valid range, with inverse logarithmic probability.
    while (n[31] & !range_max[31]) begin
      range_max <<= 1;
      range_min <<= 1;
      n <<= 1;
    end
    // Disable the soft constraint upon the total transfer size.
    dma_config.total_data_short_c.constraint_mode(0);
    // Allow only valid DMA configurations
    dma_config.valid_dma_config = 1;
    `DV_CHECK_RANDOMIZE_WITH_FATAL(
      dma_config,
      total_data_size >= range_min && total_data_size < range_max; // Limit to range
      per_transfer_width == DmaXfer4BperTxn;) // Limit to only 4B transfers; simulation time.
    `uvm_info(`gfn, $sformatf("DMA: Randomized a new transaction:%s",
                              dma_config.convert2string()), UVM_HIGH)
  endfunction

  virtual task body();
    `uvm_info(`gfn, "DMA: Starting longer transfer Sequence", UVM_LOW)
    // Reduce the access and response delays to limit the duration of simulations.
    set_access_delays(0, 0);
    set_response_delays(0, 0);
    super.body();
    `uvm_info(`gfn, "DMA: Completed longer transfer Sequence", UVM_LOW)
  endtask : body
endclass
