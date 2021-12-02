// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF-base RAM module with Write Enable
//  (Simple dual-port: 2 address ports, 2 read ports, 1 write port)
//
//   *Port A is READ FIRST: Data on write wouldn't be sent through the read port
//
// @author Kumonda221
//


`define     RAM_DEPTH       (1 << RAM_ADDR_WIDTH)

module common_dffram_2a1w2r #(
    parameter                                       RAM_DATA_WIDTH      = 1,
    parameter                                       RAM_ADDR_WIDTH      = 1,
    parameter [`RAM_DEPTH * RAM_DATA_WIDTH - 1:0]   RAM_RESET_VALUE     = { (`RAM_DEPTH){ {(RAM_DATA_WIDTH){1'b0}} } }
) (
    input  wire                             clk,
    input  wire                             reset,

    // Port A - read write
    input  wire [RAM_ADDR_WIDTH - 1:0]      addra,
    input  wire                             ena,
    input  wire                             wea,

    input  wire [RAM_DATA_WIDTH - 1:0]      dina,
    output wire [RAM_DATA_WIDTH - 1:0]      douta,

    // Port B - read only
    input  wire [RAM_ADDR_WIDTH - 1:0]      addrb,

    output wire [RAM_DATA_WIDTH - 1:0]      doutb
);

    //
    localparam  RAM_DEPTH   = `RAM_DEPTH;

    wire [RAM_DATA_WIDTH - 1:0]     dff_dout [RAM_DEPTH - 1:0];

    genvar i;
    generate
        for (i = 0; i < RAM_DEPTH; i = i + 1) begin :GENERATED_RAM_DFFS

            //
            wire    dff_we;

            stdmacro_dffe #(
                .DFF_WIDTH          (RAM_DATA_WIDTH),
                .DFF_RESET_VALUE    (RAM_RESET_VALUE[RAM_DATA_WIDTH * i +: RAM_DATA_WIDTH])
            ) stdmacro_dffe_INST_dffram_dff (
                .clk    (clk),
                .reset  (reset),

                .en     (dff_we),
                .d      (dina),

                .q      (dff_dout[i])
            );

            assign dff_we  = wea & ena & (addra == i);
            //
        end
    endgenerate

    //
    assign  douta = dff_dout[addra];

    assign  doutb = dff_dout[addrb];

    //

endmodule
