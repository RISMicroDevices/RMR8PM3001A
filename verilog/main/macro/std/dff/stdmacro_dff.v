// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF STDMACRO
//
// @see macro/conf/confmacro_dff.v
//
// @author Kumonda221
//

`include "macro/conf/confmacro_dff.v"

module stdmacro_dff #(
    parameter   DFF_WIDTH           = 1,
    parameter   DFF_RESET_VALUE     = 'b0
) (
    input   wire                        clk,
    input   wire                        reset,

    input   wire [DFF_WIDTH - 1:0]      d,
    output  wire [DFF_WIDTH - 1:0]      q
);

`ifdef STDMACRO_DFF_RESET_SYNC
`ifdef STDMACRO_DFF_RESET_ON_HIGH
    std_dffr #(
        .DFF_WIDTH(DFF_WIDTH),
        .DFF_RESET_VALUE(DFF_RESET_VALUE)
    ) std_macro_dff_INST (
        .clk(clk),
        .reset(reset),
        .d(d),
        .q(q)
    );
`endif
`ifdef STDMACRO_DFF_RESET_ON_LOW
    std_dffrn #(
        .DFF_WIDTH(DFF_WIDTH),
        .DFF_RESET_VALUE(DFF_RESET_VALUE)
    ) std_macro_dff_INST (
        .clk(clk),
        .resetn(reset),
        .d(d),
        .q(q)
    );
`endif
`endif

`ifdef STDMACRO_DFF_RESET_ASYNC
`ifdef STDMACRO_DFF_RESET_ON_HIGH
    std_dffra #(
        .DFF_WIDTH(DFF_WIDTH),
        .DFF_RESET_VALUE(DFF_RESET_VALUE)
    ) std_macro_dff_INST (
        .clk(clk),
        .areset(reset),
        .d(d),
        .q(q)
    );
`endif
`ifdef STDMACRO_DFF_RESET_ON_LOW
    std_dffran #(
        .DFF_WIDTH(DFF_WIDTH),
        .DFF_RESET_VALUE(DFF_RESET_VALUE)
    ) std_macro_dff_INST (
        .clk(clk),
        .aresetn(reset),
        .d(d),
        .q(q)
    );
`endif
`endif

endmodule
