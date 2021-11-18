
//

module sim_macro_decoder_onehot (
    input   wire [7  :0]            d,
    output  wire [255:0]            q
);

    macro_decoder_onehot_bin #(
        .INPUT_WIDTH(8)
    ) macro_decoder_onehot_bin_INST_SIM (
        .d(d),
        .q(q)
    );

endmodule
