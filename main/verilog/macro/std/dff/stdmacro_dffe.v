// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF with Enable STDMACRO
//
// @see macro/conf/confmacro_dff.v
//
// @author Kumonda221
//

`include "macro/conf/confmacro_dff.v"

module stdmacro_dffe #(
    parameter                       DFF_WIDTH           = 1,
    parameter [DFF_WIDTH - 1:0]     DFF_RESET_VALUE     = 'b0
) (
    input   wire                        clk,
    input   wire                        reset,
    input   wire                        en,

    input   wire [DFF_WIDTH - 1:0]      d,
    output  wire [DFF_WIDTH - 1:0]      q
);

`ifdef STDMACRO_DFF_RESET_SYNC
`ifdef STDMACRO_DFF_RESET_ON_HIGH
    std_dffer #(
        .DFF_WIDTH(DFF_WIDTH),
        .DFF_RESET_VALUE(DFF_RESET_VALUE)
    ) std_macro_dff_INST (
        .clk(clk),
        .reset(reset),
        .en(en),
        .d(d),
        .q(q)
    );
`endif
`ifdef STDMACRO_DFF_RESET_ON_LOW
    std_dffern #(
        .DFF_WIDTH(DFF_WIDTH),
        .DFF_RESET_VALUE(DFF_RESET_VALUE)
    ) std_macro_dff_INST (
        .clk(clk),
        .resetn(reset),
        .en(en),
        .d(d),
        .q(q)
    );
`endif
`endif

`ifdef STDMACRO_DFF_RESET_ASYNC
`ifdef STDMACRO_DFF_RESET_ON_HIGH
    std_dffera #(
        .DFF_WIDTH(DFF_WIDTH),
        .DFF_RESET_VALUE(DFF_RESET_VALUE)
    ) std_macro_dff_INST (
        .clk(clk),
        .areset(reset),
        .en(en),
        .d(d),
        .q(q)
    );
`endif
`ifdef STDMACRO_DFF_RESET_ON_LOW
    std_dfferan #(
        .DFF_WIDTH(DFF_WIDTH),
        .DFF_RESET_VALUE(DFF_RESET_VALUE)
    ) std_macro_dff_INST (
        .clk(clk),
        .aresetn(reset),
        .en(en),
        .d(d),
        .q(q)
    );
`endif
`endif

endmodule
