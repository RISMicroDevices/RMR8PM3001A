// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard Single-Port SRAM Module with Asynchronous High-Active Reset DFFs on Output
//
// @author Kumonda221
//

module std_sram_singleport_dffra #(
    parameter   ADDR_WIDTH          = 1,
    parameter   DATA_WIDTH          = 1
) (
    input   wire                        clk,
    input   wire                        aregrst,

    input   wire [ADDR_WIDTH - 1:0]     addr,
    input   wire [DATA_WIDTH - 1:0]     din,
    input   wire                        en,
    input   wire                        we,
    output  wire [DATA_WIDTH - 1:0]     dout
);

    wire [DATA_WIDTH - 1:0]     out;

    std_sram_singleport #(
        .ADDR_WIDTH(ADDR_WIDTH),
        .DATA_WIDTH(DATA_WIDTH)
    ) std_sram_singleport_INST (
        .clk(clk),
        .en(en),

        .addr(addr),
        .din(din),
        .we(we),
        .dout(out)
    );

    std_dffera #(
        .DFF_WIDTH(DATA_WIDTH)
    ) std_sram_output_dff_INST (
        .clk(clk),
        .areset(aregrst),
        .en(en & ~we),

        .d(out),
        .q(dout)
    );

endmodule
