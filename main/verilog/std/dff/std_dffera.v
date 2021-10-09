// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard DFF Module with High-Active Asynchronous Reset & Enable
//
// @author Kumonda221
//

module std_dffera #(
    parameter                       DFF_WIDTH           = 1,
    parameter [DFF_WIDTH - 1:0]     DFF_RESET_VALUE     = 'b0
) (
    input   wire                        clk,
    input   wire                        areset,
    input   wire                        en,

    input   wire [DFF_WIDTH - 1:0]      d,
    output  wire [DFF_WIDTH - 1:0]      q
);

    reg  [DFF_WIDTH - 1:0]  q_R;

    always @(posedge clk or posedge areset) begin
        
        if (areset) begin
            q_R <= DFF_RESET_VALUE;
        end
        else if (en) begin
            q_R <= d;
        end
        else begin
            q_R <= q_R;
        end
    end

    assign q = q_R;

endmodule
