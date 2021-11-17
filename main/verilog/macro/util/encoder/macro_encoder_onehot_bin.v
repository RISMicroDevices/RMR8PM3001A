// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  One-hot Binary Output Encoder
//
// @author Kumonda221
//

module macro_encoder_onehot_bin #(
    parameter           OUTPUT_WIDTH = 1
) (
    input   wire [(1 << OUTPUT_WIDTH) - 1:0]    d,
    output  wire [OUTPUT_WIDTH - 1:0]           q
);
    //
    wire [(1 << (OUTPUT_WIDTH - 1)) * OUTPUT_WIDTH - 1:0]   d_tree_encoder;

    //
    genvar i, j;

    //
    generate
        for (i = 0; i < OUTPUT_WIDTH; i = i + 1) begin: GENERATED_ENCODER
            for (j = 0; j < (1 << (OUTPUT_WIDTH - 1)); j = j + 1) begin
                
                assign d_tree_encoder[i * (1 << (OUTPUT_WIDTH - 1)) + j] 
                    = d[(1 << i) + (j / (1 << i)) * (1 << (i + 1)) + (j % (1 << i))];
            end
        end
    endgenerate

    //
    generate
        for (i = 0; i < OUTPUT_WIDTH; i = i + 1) begin: GENERATED_ENCODER_OUTPUT
            
            assign q[i] = |{ d_tree_encoder[i * (1 << (OUTPUT_WIDTH - 1)) +: (1 << (OUTPUT_WIDTH - 1))] };
        end
    endgenerate

endmodule