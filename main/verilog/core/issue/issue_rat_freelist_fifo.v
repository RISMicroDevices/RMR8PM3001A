// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  PRF Free List 64-entry FIFO
//  (32 entries per-bank)
//
// @author Kumonda221
//

module issue_rat_freelist_fifo (
    input   wire                clk,
    input   wire                reset,

    // 
    input   wire [5:0]          i_abandoned_prf,
    input   wire                i_abandoned_valid,
    output  wire                o_abandoned_ready,

    //
    input   wire [5:0]          i_redeemed_prf,
    input   wire                i_redeemed_valid,
    output  wire                o_redeemed_ready,

    //
    output  wire [5:0]          o_acquire_prf,
    output  wire                o_acquire_valid,
    input   wire                i_acquire_ready
);

endmodule
