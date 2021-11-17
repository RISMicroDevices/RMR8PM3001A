// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  AND-reduction tree Macro
//
// @author Kumonda221
//

module macro_reduction_tree_and #(
    parameter   INPUT_WIDTH         = 1,
    parameter   INPUT_COUNT         = 1
) (
    input   wire [INPUT_WIDTH * INPUT_COUNT - 1:0]      d,

    output  wire [INPUT_WIDTH - 1:0]                    q
);
    //
    localparam P_COUNT  = 1 << $clog2(INPUT_COUNT);

    /* verilator lint_off UNOPTFLAT */
    wire [INPUT_WIDTH - 1:0]    d_reduction_tree [(P_COUNT * 2 - 1) - 1:0];

    //
    genvar i, j;

    //
    generate
        for (i = 0; i < P_COUNT; i = i + 1) begin : GENERATED_OR_REDUCTION_TREE_INPUT
            
            if (i < INPUT_COUNT) begin
                assign d_reduction_tree[(P_COUNT * 2 - 1) - 1 - i] = d[INPUT_WIDTH * i +: INPUT_WIDTH];
            end
            else begin
                assign d_reduction_tree[(P_COUNT * 2 - 1) - 1 - i] = 'b0;
            end
        end
    endgenerate

    //
    generate
        for (i = 0; i < $clog2(INPUT_COUNT); i = i + 1) begin : GENERATED_OR_REDUCTION_TREE
            for (j = 0; j < (1 << i); j = j + 1) begin
                
                assign d_reduction_tree[((1 << i) - 1) + j]
                     = d_reduction_tree[((1 << (i + 1)) - 1) + j * 2]
                     & d_reduction_tree[((1 << (i + 1)) - 1) + j * 2 + 1];
            end
        end
    endgenerate

    //
    assign q = d_reduction_tree[0];

    //

endmodule
