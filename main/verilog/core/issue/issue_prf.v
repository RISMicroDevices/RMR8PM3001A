// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Physical Register Files
//  (64 bits width, 64 slots, 3 address ports, 2 read ports, 1 write port)
//
// @author Kumonda221
//

module issue_prf (
    input   wire                clk,
    input   wire                reset,

    // Port A - write port
    input   wire [5:0]          addra,
    input   wire                wea,

    input   wire [63:0]         dina,

    // Port B - read port
    input   wire [5:0]          addrb,

    output  wire [63:0]         doutb,

    // Port C - read port
    input   wire [5:0]          addrc,

    output  wire [63:0]         doutc
);

    common_dffram_3a1w2r #(
        .RAM_WIDTH  (64),
        .RAM_DEPTH  (1 << 6)
    ) common_dffram_3a1w2r_INST_prfs (
        .clk    (clk),
        .reset  (reset),

        .addra  (addra),
        .ena    (1'b1),
        .wea    (wea),
        .dina   (dina),

        .addrb  (addrb),
        .doutb  (doutb),

        .addrc  (addrc),
        .doutc  (doutc),

        .tdout  ()
    );

endmodule
