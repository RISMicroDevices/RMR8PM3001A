
//

module sim_common_pseudo_lru_pick #(
    parameter       SUBJECT_COUNT_LOG2  = 5
) (
    input   wire                                    clk,
    input   wire                                    resetn,

    input   wire [SUBJECT_COUNT_LOG2 - 1:0]         waddr,
    input   wire                                    wen,

    input   wire [(1 << SUBJECT_COUNT_LOG2) - 1:0]  dvalid,

    output  wire [SUBJECT_COUNT_LOG2 - 1:0]         qaddr,
    output  wire                                    qvalid
);

    common_pseudo_lru_pick_binwr #(
        .SUBJECT_COUNT_LOG2(SUBJECT_COUNT_LOG2)
    ) common_pseudo_lru_pick_binwr_INST_SIM (
        .clk    (clk),
        .reset  (resetn),

        .waddr  (waddr),
        .wen    (wen),

        .dvalid (dvalid),

        .qaddr  (qaddr),
        .qvalid (qvalid)
    );

endmodule
