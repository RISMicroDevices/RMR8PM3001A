
//

module sim_macro_encoder_onehot (
    input   wire [15:0]             d,
    output  wire [3 :0]             q,
    output  wire                    valid
);

    macro_encoder_onehot_bin #(
        .OUTPUT_WIDTH   (4)
    ) macro_encoder_onehot_bin_INST_SIM (
        .d      (d),
        .q      (q)
    );

    macro_encoder_onehot_detect #(
        .INPUT_WIDTH    (1 << 4)
    ) macro_encoder_onehot_detect_INST_SIM (
        .d      (d),
        .valid  (valid)
    );

endmodule
