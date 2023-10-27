// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// Stress 'memory-to-memory' DMA transfers
class dma_memory_stress_vseq extends dma_memory_vseq;
  `uvm_object_utils(dma_memory_stress_vseq)
  `uvm_object_new

  // Constrain number of iterations and transactions in each iteration
  constraint transactions_c {num_txns inside {[10:40]};}
  constraint num_iters_c {num_iters inside {[5:20]};}

  // The functionality of this vseq is implemented in `dma_generic_vseq` and restricted
  // to 'memory-to-memory' transfers in `dma_memory_vseq`
  virtual task body();
    `uvm_info(`gfn, "DMA: Starting memory stress Sequence", UVM_LOW)
    super.body();
    `uvm_info(`gfn, "DMA: Completed memory stress Sequence", UVM_LOW)
  endtask : body
endclass
