// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

`include "dma_base_vseq.sv"
`include "dma_common_vseq.sv"
`include "dma_generic_vseq.sv"
`include "dma_generic_smoke_vseq.sv"
`include "dma_memory_vseq.sv"
`include "dma_memory_smoke_vseq.sv"
`include "dma_handshake_vseq.sv"
`include "dma_handshake_smoke_vseq.sv"
`include "dma_memory_region_lock_vseq.sv"
`include "dma_abort_vseq.sv"
`include "dma_generic_stress_vseq.sv"
`include "dma_memory_stress_vseq.sv"
`include "dma_handshake_stress_vseq.sv"
`include "dma_stress_all_vseq.sv"
`include "dma_short_transfer_vseq.sv"
`include "dma_longer_transfer_vseq.sv"
`include "dma_mem_enabled_vseq.sv"
//`include "dma_intr_vseq.sv"
