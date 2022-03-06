
//

module sim_macro_pick1_priority (
    input   wire [31:0]         dvalid,
    output  wire [4:0]          qaddr,
    output  wire                qvalid
);

    macro_pick1_priority_bin #(
        .INPUT_COUNT(32)
    ) macro_pick1_priority_bin_INST_SIM (
        .dvalid (dvalid),
        .qaddr  (qaddr),
        .qvalid (qvalid)
    );

endmodule
