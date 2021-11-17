// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  One-hot Binary Input Decoder
//
// @author Kumonda221
//

module macro_decoder_onehot_bin #(
    parameter           INPUT_WIDTH = 1
) (
    input   wire [INPUT_WIDTH - 1:0]            d,
    output  wire [(1 << INPUT_WIDTH) - 1:0]     q
);

    genvar i;
    generate
        for (i = 0; i < (1 << INPUT_WIDTH); i = i + 1) begin: GENERATED_ENCODER
            
            assign q[i] = d == i;
        end
    endgenerate

endmodule

