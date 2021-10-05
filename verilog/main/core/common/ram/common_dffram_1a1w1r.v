// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF-base RAM module
//
// @author Kumonda221
//

`define     RAM_DEPTH       (1 << RAM_ADDR_WIDTH)

module common_dffram_1a1w1r #(
    parameter                   RAM_DATA_WIDTH      = 1,
    parameter                   RAM_ADDR_WIDTH      = 1,
    parameter [`RAM_DEPTH:0]    RAM_RESET_VALUE     = { (`RAM_DEPTH){ {(RAM_ADDR_WIDTH){1'b0}} } }
) (
    input   wire                            clk,
    input   wire                            reset,

    input   wire [RAM_ADDR_WIDTH - 1:0]     addr,
    input   wire                            en,
    input   wire                            we,

    input   wire [RAM_DATA_WIDTH - 1:0]     din,
    output  wire [RAM_DATA_WIDTH - 1:0]     dout
);

    //
    parameter   RAM_DEPTH   = `RAM_DEPTH;

    wire [RAM_DATA_WIDTH - 1:0]     dff_dout [RAM_DEPTH - 1:0];

    genvar i;
    generate
        for (i = 0; i < RAM_DEPTH; i = i + 1) begin :GENERATED_RAM_DFFS

            //
            wire  dff_we;

            stdmacro_dffe #(
                .DFF_WIDTH          (RAM_DATA_WIDTH),
                .DFF_RESET_VALUE    (RAM_RESET_VALUE[i])
            ) stdmacro_dffe_INST_dffram_dff (
                .clk(clk),
                .reset(reset),

                .en(dff_we),
                .d(din),

                .q(dff_dout[i])
            );

            assign dff_we  = we & en & (addr == i);
            //
        end
    endgenerate

    //
    assign  dout = dff_dout[addr];

    //

endmodule
