// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  NAND-reduction Macro
//
// @author Kumonda221
//

module macro_reduction_nand #(
    parameter           INPUT_WIDTH = 1,
    parameter           INPUT_COUNT = 1
) (
    input   wire [INPUT_WIDTH * INPUT_COUNT - 1:0]      d,

    output  wire [INPUT_WIDTH - 1:0]                    q
);

    //
    wire [INPUT_COUNT - 1:0]    a [INPUT_WIDTH - 1:0];

    //
    genvar i, j;

    //
    generate
        for (i = 0; i < INPUT_COUNT; i = i + 1) begin: GENERATED_REDUCTION_INPUT
            for (j = 0; j < INPUT_WIDTH; j = j + 1) begin
                
                assign a[j][i] = d[i * INPUT_WIDTH + j];
            end
        end
    endgenerate

    //
    generate
        for (i = 0; i < INPUT_WIDTH; i = i + 1) begin: GENERATED_REDUCTION_OUTPUT

            assign q[i] = ~&{ a[i] };
        end
    endgenerate

endmodule
