// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Register Alias Table Global Checkpoints
//
// @author Kumonda221
//

module issue_rat_gc #(
    parameter           GC_WIDTH        = 64,
    parameter           GC_COUNT_LOG2   = 3
) (
    input   wire                        clk,
    input   wire                        reset,

    //
    input   wire [GC_COUNT_LOG2 - 1:0]  addr,
    input   wire                        we,

    output  wire [GC_WIDTH - 1:0]       dout,
    input   wire [GC_WIDTH - 1:0]       din
);

    common_dffram_1a1w1r #(
        .RAM_DATA_WIDTH (GC_WIDTH),
        .RAM_ADDR_WIDTH (GC_COUNT_LOG2)
    ) common_dffram_1a1w1r_INST (
        .clk    (clk),
        .reset  (reset),

        .addr   (addr),
        .en     (1'b1),
        .we     (we),
        
        .dout   (dout),
        .din    (din)
    );

endmodule
