// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard DFF Module with Low-Active Asynchronous Reset & Bit Enable
//
// @author Kumonda221
//

module std_dffberan #(
    parameter                       DFF_WIDTH           = 8,
    parameter [DFF_WIDTH - 1:0]     DFF_RESET_VALUE     = 'b10101010
) (
    input   wire                        clk,
    input   wire                        aresetn,
    input   wire [DFF_WIDTH - 1:0]      en,

    input   wire [DFF_WIDTH - 1:0]      d,
    output  wire [DFF_WIDTH - 1:0]      q
);

    reg  [DFF_WIDTH - 1:0]  q_R;

    genvar i;
    generate
        for (i = 0; i < DFF_WIDTH; i = i + 1) begin :GENERATED_ASYNC_REG

            always @(posedge clk or negedge aresetn) begin
                
                if (~aresetn) begin
                    q_R[i] <= DFF_RESET_VALUE[i];
                end
                else if (en[i]) begin
                    q_R[i] <= d[i];
                end
                else begin
                    q_R[i] <= q_R[i];
                end
            end
        end
    endgenerate

    assign q = q_R;

endmodule
