// @description
//  ** PART OF **
//  RMM4NC3001X - Gemini 3001
//  (MIPS32 Processor for NSCSCC2021)
//
//  Standard DFF Module
//
// @author Kumonda221
//

module std_dff #(
    parameter   DFF_WIDTH       = 1   
) (
    input   wire                        clk,

    input   wire [DFF_WIDTH - 1:0]      d,
    output  wire [DFF_WIDTH - 1:0]      q
);

    reg  [DFF_WIDTH - 1:0]  q_R;

    always @(posedge clk) begin
        q_R <= d;
    end

    assign q = q_R;

endmodule
