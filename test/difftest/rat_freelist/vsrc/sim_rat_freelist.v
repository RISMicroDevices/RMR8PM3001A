
//

module sim_rat_freelist (
    input   wire                clk,
    input   wire                resetn,

    input   wire [5:0]          i_redeemed_prf,
    input   wire                i_redeemed_valid,
    output  wire                o_redeemed_ready,

    output  wire [5:0]          o_acquire_prf,
    output  wire                o_acquire_ready,
    input   wire [2:0]          i_acquire_fgr,
    input   wire                i_acquire_fgr_speculative,
    input   wire                i_acquire_valid,

    input   wire [2:0]          i_commit_fgr,
    input   wire                i_commit_valid,

    input   wire [2:0]          i_abandon_fgr,
    input   wire                i_abandon_valid
);

    issue_rat_freelist #(
    ) issue_rat_freelist_INST_sim (
        .clk                        (clk),
        .reset                      (resetn),

        .i_redeemed_prf             (i_redeemed_prf),
        .i_redeemed_valid           (i_redeemed_valid),
        .o_redeemed_ready           (o_redeemed_ready),

        .o_acquire_prf              (o_acquire_prf),
        .o_acquire_ready            (o_acquire_ready),
        .i_acquire_fgr              (i_acquire_fgr),
        .i_acquire_fgr_speculative  (i_acquire_fgr_speculative),
        .i_acquire_valid            (i_acquire_valid),

        .i_commit_fgr               (i_commit_fgr),
        .i_commit_valid             (i_commit_valid),

        .i_abandon_fgr              (i_abandon_fgr),
        .i_abandon_valid            (i_abandon_valid)
    );

endmodule
