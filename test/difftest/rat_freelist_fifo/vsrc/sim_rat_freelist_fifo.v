
//

module sim_rat_freelist_fifo (
    input   wire                clk,
    input   wire                resetn,

    input   wire [5:0]          i_abandoned_prf,
    input   wire                i_abandoned_valid,
    output  wire                o_abandoned_ready,

    input   wire [5:0]          i_redeemed_prf,
    input   wire                i_redeemed_valid,
    output  wire                o_redeemed_ready,

    output  wire [5:0]          o_acquire_prf,
    output  wire                o_acquire_valid,
    input   wire                i_acquire_ready
);

    issue_rat_freelist_fifo #(
    ) issue_rat_freelist_fifo_INST_sim (
        .clk                (clk),
        .reset              (resetn),

        .i_abandoned_prf    (i_abandoned_prf),
        .i_abandoned_valid  (i_abandoned_valid),
        .o_abandoned_ready  (o_abandoned_ready),
        
        .i_redeemed_prf     (i_redeemed_prf),
        .i_redeemed_valid   (i_redeemed_valid),
        .o_redeemed_ready   (o_redeemed_ready),

        .o_acquire_prf      (o_acquire_prf),
        .o_acquire_valid    (o_acquire_valid),
        .i_acquire_ready    (i_acquire_ready)
    );

endmodule
