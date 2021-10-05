// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Synchronous RAM-base FIFO with 1 write port, 1 read port
//  (Write Enable and data from 'din' always ignored when FIFO was full)
//
// @author Kumonda221
//

module common_fifo_ram_1w1r #(
    parameter                       FIFO_DEPTH_LOG2     = 1,
    parameter                       FIFO_WIDTH          = 1,
    parameter                       FIFO_RESET_STATE    = 0,
    parameter [FIFO_DEPTH - 1:0]    FIFO_RESET_VALUE    = { (FIFO_DEPTH){ {(FIFO_WIDTH){1'b0}} } }
) (
    input   wire                        clk,
    input   wire                        reset,

    input   wire [FIFO_WIDTH - 1:0]     din,
    input   wire                        wen,

    output  wire [FIFO_WIDTH - 1:0]     dout,
    input   wire                        ren,

    output  wire                        fifo_empty,
    output  wire                        fifo_full
);

endmodule
