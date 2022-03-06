
//

module sim_common_pseudo_lru_swap (
    input   wire                clk,
    input   wire                resetn,

    input   wire [5:0]          waddr,
    input   wire                wen,

    output  wire [5:0]          qaddr
);

    common_pseudo_lru_swap_binwr #(
        .SUBJECT_COUNT_LOG2(6)
    ) common_pseudo_lru_swap_binwr_INST_SIM (
        .clk    (clk),
        .reset  (resetn),

        .waddr  (waddr),
        .wen    (wen),

        .qaddr  (qaddr)
    );

endmodule
