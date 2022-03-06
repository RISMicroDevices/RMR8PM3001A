
//

module sim_rat_freelist_checkpoint (
    input   wire                clk,
    input   wire                resetn,

    input   wire                i_abandon_valid,
    input   wire [2:0]          i_abandon_fgr,

    input   wire                i_commit_valid,
    input   wire [2:0]          i_commit_fgr,

    output  wire                o_abandoned_valid,
    input   wire                i_abandoned_ready,
    output  wire [5:0]          o_abandoned_prf,

    input   wire                i_acquired_valid,
    output  wire                o_acquired_ready,
    input   wire [2:0]          i_acquired_fgr,
    input   wire [5:0]          i_acquired_prf
);

    issue_rat_freelist_checkpoint #(
        .PRF_WIDTH  (6),
        .FGR_WIDTH  (3),
        .BANK_COUNT (4)
    ) issue_rat_freelist_checkpoint_INST_SIM (
        .clk                (clk),
        .reset              (resetn),

        .i_abandon_valid    (i_abandon_valid),
        .i_abandon_fgr      (i_abandon_fgr),

        .i_commit_valid     (i_commit_valid),
        .i_commit_fgr       (i_commit_fgr),

        .o_abandoned_valid  (o_abandoned_valid),
        .i_abandoned_ready  (i_abandoned_ready),
        .o_abandoned_prf    (o_abandoned_prf),

        .i_acquired_valid   (i_acquired_valid),
        .o_acquired_ready   (o_acquired_ready),
        
        .i_acquired_fgr     (i_acquired_fgr),
        .i_acquired_prf     (i_acquired_prf)
    );

endmodule
