// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard Single-Port SRAM Module with Latches on Output
//
// @author Kumonda221
//

module std_sram_singleport_latch #(
    parameter   ADDR_WIDTH          = 1,
    parameter   DATA_WIDTH          = 1
) (
    input   wire                        clk,

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

        .addr(addr),
        .din(din),
        .en(en),
        .we(we),
        .dout(out)
    );

    std_latch #(
        .LATCH_WIDTH(DATA_WIDTH)
    ) std_sram_output_latch_INST (
        .en(en & ~we),

        .d(out),
        .q(dout)
    );

endmodule
