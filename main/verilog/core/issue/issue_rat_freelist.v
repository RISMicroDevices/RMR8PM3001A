// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  PRF free list for 64-entry RAT (Register Alias Table) with checkpoints
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
    output  wire                o_acquire_ready,
    input   wire [3:0]          i_acquire_fgr,
    input   wire                i_acquire_fgr_speculative,
    input   wire                i_acquire_valid,

    //
    input   wire [3:0]          i_commit_fgr,
    input   wire                i_commit_valid,

    //
    input   wire [3:0]          i_abandon_fgr,
    input   wire                i_abandon_valid
);
    //
    wire [5:0]      o_fifo_abandoned_prf;
    wire            o_fifo_abandoned_valid;
    wire            i_fifo_abandoned_ready;

    wire [5:0]      o_fifo_redeemed_prf;
    wire            o_fifo_redeemed_valid;
    wire            i_fifo_redeemed_ready;

    wire [5:0]      i_fifo_acquire_prf;
    wire            i_fifo_acquire_valid;
    wire            o_fifo_acquire_ready;

    issue_rat_freelist_fifo issue_rat_freelist_fifo_INST (
        .clk                (clk),
        .reset              (reset),

        .i_abandoned_prf    (o_fifo_abandoned_prf),
        .i_abandoned_valid  (o_fifo_abandoned_valid),
        .o_abandoned_ready  (i_fifo_abandoned_ready),

        .i_redeemed_prf     (o_fifo_redeemed_prf),
        .i_redeemed_valid   (o_fifo_redeemed_valid),
        .o_redeemed_ready   (i_fifo_redeemed_ready),

        .o_acquire_prf      (i_fifo_acquire_prf),
        .o_acquire_valid    (i_fifo_acquire_valid),
        .i_acquire_ready    (o_fifo_acquire_ready)
    );

    //
    wire [3:0]      o_cp_abandon_fgr;
    wire            o_cp_abandon_valid;

    wire [3:0]      o_cp_commit_fgr;
    wire            o_cp_commit_valid;

    wire [5:0]      i_cp_abandoned_prf;
    wire            i_cp_abandoned_valid;
    wire            o_cp_abandoned_ready;

    wire [5:0]      o_cp_acquired_prf;
    wire [3:0]      o_cp_acquired_fgr;
    wire            o_cp_acquired_valid;
    wire            i_cp_acquired_ready;

    issue_rat_freelist_checkpoint issue_rat_freelist_checkpoint_INST (
        .clk                (clk),
        .reset              (reset),

        .i_abandon_fgr      (o_cp_abandon_fgr),
        .i_abandon_valid    (o_cp_abandon_valid),

        .i_commit_fgr       (o_cp_commit_fgr),
        .i_commit_valid     (o_cp_commit_valid),

        .o_abandoned_prf    (i_cp_abandoned_prf),
        .o_abandoned_valid  (i_cp_abandoned_valid),
        .i_abandoned_ready  (o_cp_abandoned_ready),

        .i_acquired_prf     (o_cp_acquired_prf),
        .i_acquired_fgr     (o_cp_acquired_fgr),
        .i_acquired_valid   (o_cp_acquired_valid),
        .o_acquired_ready   (i_cp_acquired_ready)
    );

    //
    wire    r_cp_acquired_push;

    assign r_cp_acquired_push   =  i_acquire_valid & i_fifo_acquire_valid;

    assign o_cp_acquired_prf    =  i_fifo_acquire_prf;
    assign o_cp_acquired_fgr    =  i_acquire_fgr;
    assign o_cp_acquired_valid  =  i_acquire_fgr_speculative & r_cp_acquired_push;

    assign o_fifo_acquire_ready =  i_acquire_fgr_speculative ? (r_cp_acquired_push & i_cp_acquired_ready) : i_acquire_valid;

    //
    assign o_fifo_abandoned_prf     =  i_cp_abandoned_prf;
    assign o_fifo_abandoned_valid   =  i_cp_abandoned_valid;

    assign o_cp_abandoned_ready     =  i_fifo_abandoned_ready;

    //
    assign o_fifo_redeemed_prf      = i_redeemed_prf;
    assign o_fifo_redeemed_valid    = i_redeemed_valid;
    assign o_redeemed_ready         = i_fifo_redeemed_ready;

    //
    assign o_cp_commit_fgr      = i_commit_fgr;
    assign o_cp_commit_valid    = i_commit_valid;

    assign o_cp_abandon_fgr     = i_abandon_fgr;
    assign o_cp_abandon_valid   = i_abandon_valid;

    //
    assign o_acquire_prf        = i_fifo_acquire_prf;
    assign o_acquire_ready      = o_fifo_acquire_ready;

    //

endmodule
