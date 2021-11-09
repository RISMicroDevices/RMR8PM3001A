// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  PRF free list for 64-entry RAT (Register Alias Table)
//  (2 bank interleaving, 2 write per-clk max, 1 read per-clk max)
//
// @author Kumonda221
//

module issue_rat_freelist (
    input   wire                clk,
    input   wire                reset,

    //
    input   wire [5:0]          i_redeemed_prf,
    input   wire                i_redeemed_valid,
    output  wire                o_redeemed_ready,

    //
    output  wire [5:0]          o_acquire_prf,
    output  wire                o_acquire_valid,
    input   wire [2:0]          i_acquire_fgr,
    input   wire                i_acquire_fgr_speculative,
    input   wire                i_acquire_ready,

    //
    input   wire [2:0]          i_commit_fgr,
    input   wire                i_commit_valid,

    //
    input   wire [2:0]          i_abandon_fgr,
    input   wire                i_abandon_valid
);

    // TODO

endmodule
