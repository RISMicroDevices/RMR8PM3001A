// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  PRF free list checkpoints for 64-entry RAT
//  (4 bank, 4 entries per-bank, FGR associated)
//
// @author Kumonda221
//

module issue_rat_freelist_checkpoint (
    input   wire                clk,
    input   wire                reset,

    //
    output  wire                o_abandoned_valid,
    input   wire                i_abandoned_ready,

    output  wire [5:0]          o_abandoned_prf,

    //
    input   wire                i_acquired_valid,
    output  wire                o_acquired_ready,

    input   wire [2:0]          i_acquired_fgr,
    input   wire [5:0]          i_acquired_prf
);

    //

endmodule
