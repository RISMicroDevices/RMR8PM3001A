// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard DFF Module with Enable
//
// @author Kumonda221
//

module std_dffe #(
    parameter   DFF_WIDTH       = 1
) (
    input   wire                        clk,
    input   wire                        en,

    input   wire [DFF_WIDTH - 1:0]      d,
    output  wire [DFF_WIDTH - 1:0]      q
);

    reg  [DFF_WIDTH - 1:0]  q_R;

    always @(posedge clk) begin
        
        if (en) begin
            q_R <= d;
        end
        else begin
            q_R <= q_R;
        end
    end

    assign q = q_R;
    
endmodule
