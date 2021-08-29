// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  SRAM with DFFs on output STDMACRO
//
// @see macro/conf/confmacro_dff.v
//
// @author Kumonda221
//

module stdmacro_sram_singleport_dff #(
    parameter   ADDR_WIDTH          = 1,
    parameter   DATA_WIDTH          = 1
) (
    input   wire                        clk,
    input   wire                        regrst,

    input   wire [ADDR_WIDTH - 1:0]     addr,
    input   wire [DATA_WIDTH - 1:0]     din,
    input   wire                        en,
    input   wire                        we,
    output  wire [DATA_WIDTH - 1:0]     dout
);

`ifdef STDMACRO_DFF_RESET_SYNC
`ifdef STDMACRO_DFF_RESET_ON_HIGH
    std_sram_singleport_dffr #(
        .ADDR_WIDTH(ADDR_WIDTH),
        .DATA_WIDTH(DATA_WIDTH)
    ) std_macro_sram_singleport_dff_INST (
        .clk(clk),
        .regrst(regrst),
        .addr(addr),
        .din(din),
        .en(en),
        .we(we),
        .dout(dout)
    );
`endif
`ifdef STDMACRO_DFF_RESET_ON_LOW
    std_sram_singleport_dffrn #(
        .ADDR_WIDTH(ADDR_WIDTH),
        .DATA_WIDTH(DATA_WIDTH)
    ) std_macro_sram_singleport_dff_INST (
        .clk(clk),
        .regrstn(regrst),
        .addr(addr),
        .din(din),
        .en(en),
        .we(we),
        .dout(dout)
    );
`endif
`endif

`ifdef STDMACRO_DFF_RESET_ASYNC
`ifdef STDMACRO_DFF_RESET_ON_HIGH
    std_sram_singleport_dffra #(
        .ADDR_WIDTH(ADDR_WIDTH),
        .DATA_WIDTH(DATA_WIDTH)
    ) std_macro_sram_singleport_dff_INST (
        .clk(clk),
        .aregrst(regrst),
        .addr(addr),
        .din(din),
        .en(en),
        .we(we),
        .dout(dout)
    );
`endif
`ifdef STDMACRO_DFF_RESET_ON_LOW
    std_sram_singleport_dffran #(
        .ADDR_WIDTH(ADDR_WIDTH),
        .DATA_WIDTH(DATA_WIDTH)
    ) std_macro_sram_singleport_dff_INST (
        .clk(clk),
        .aregrstn(regrst),
        .addr(addr),
        .din(din),
        .en(en),
        .we(we),
        .dout(dout)
    );
`endif
`endif

endmodule
