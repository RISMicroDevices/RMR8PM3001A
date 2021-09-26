// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
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
