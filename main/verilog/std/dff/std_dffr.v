// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard DFF Module with High-Active Synchronous Reset
//
// @author Kumonda221
//

module std_dffr #(
    parameter                       DFF_WIDTH           = 1,
    parameter [DFF_WIDTH - 1:0]     DFF_RESET_VALUE     = 'b0
) (
    input   wire                        clk,
    input   wire                        reset,

    input   wire [DFF_WIDTH - 1:0]      d,
    output  wire [DFF_WIDTH - 1:0]      q
);
    
    reg  [DFF_WIDTH - 1:0]  q_R;

    always @(posedge clk) begin
        
        if (reset) begin
            q_R <= DFF_RESET_VALUE;
        end
        else begin
            q_R <= d;
        end
    end

    assign q = q_R;

endmodule
