// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard Single-port SRAM Module with Bit Write Enable
//  (128 port width, 64 depth, 1 clk delay)
//
// @author Kumonda221
//

module std_sram_64d128x_1a1wb1r (
    //
    input   wire                clk,

    //
    input   wire[5:0]           addr,
    input   wire                en,
    input   wire                we,
    input   wire[127:0]         bwe,

    input   wire[127:0]         din,
    output  wire[127:0]         dout
);

    S011HD1P_X32Y2D128_BW S011HD1P_X32Y2D128_BW_INST_WRAPPED (
        .CLK(clk),
        .CEN(~en),
        .WEN(~we),
        .BWEN(~bwe),
        .A(addr),
        .D(din),
        .Q(dout)
    );

endmodule