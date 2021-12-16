// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF-base RAM module
//  (Single-port: 1 address port, 1 read port, 1 write port)
//
// @author Kumonda221
//

module common_dffram_1a1w1r #(
    parameter                                       RAM_WIDTH               = 1,
    parameter                                       RAM_DEPTH               = 1,
    parameter [RAM_DEPTH * RAM_WIDTH - 1:0]         RAM_RESET_VALUE         = { (RAM_DEPTH){ {(RAM_WIDTH){1'b0}} } },

    parameter                                       ONEHOT_ADDRESSING       = 0,
    parameter                                       BIT_WRITE_ENABLE        = 0,

    parameter                                       DATA_COUPLED            = 0,

    //
    localparam                                      RAM_PORT_ADDR_WIDTH     = ONEHOT_ADDRESSING ? RAM_DEPTH : $clog2(RAM_DEPTH),
    localparam                                      RAM_PORT_WE_WIDTH       = BIT_WRITE_ENABLE ? RAM_WIDTH : 1,
    localparam                                      RAM_PORT_TDATA_WIDTH    = DATA_COUPLED ? RAM_DEPTH * RAM_WIDTH : 1
) (
    input   wire                                clk,
    input   wire                                reset,

    //
    input   wire [RAM_PORT_ADDR_WIDTH - 1:0]    addr,
    input   wire                                en,
    input   wire [RAM_PORT_WE_WIDTH - 1:0]      we,

    //
    input   wire [RAM_WIDTH - 1:0]              din,
    output  wire [RAM_WIDTH - 1:0]              dout,

    //
    output  wire [RAM_PORT_TDATA_WIDTH - 1:0]   tdout
);
    //
    wire [RAM_WIDTH * RAM_DEPTH - 1:0]          dff_o_dout;
    wire [(1 << ($clog2(RAM_DEPTH))) - 1:0]     dff_i_addr;

    //
    generate
        if (ONEHOT_ADDRESSING) begin :GENERATED_ADDRESS_INPUT_ONEHOT

            assign dff_i_addr = addr;
        end
        else begin :GENERATED_ADDRESS_INPUT_BINARY
            
            macro_decoder_onehot_bin #(
                .INPUT_WIDTH    (RAM_PORT_ADDR_WIDTH)
            ) macro_decoder_onehot_bin_INST_addr (
                .d  (addr),
                .q  (dff_i_addr)
            );
        end
    endgenerate

    //
    generate 
        if (!DATA_COUPLED) begin :GENERATED_NOT_DATA_COUPLED
            assign tdout = 'b0;
        end
    endgenerate

    //
    genvar i;
    generate
        for (i = 0; i < RAM_DEPTH; i = i + 1) begin :GENERATED_RAM_DFFS

            //
            wire [RAM_WIDTH - 1:0]          dff_q;
            wire [RAM_PORT_WE_WIDTH - 1:0]  dff_we;

            //
            if (BIT_WRITE_ENABLE) begin

                stdmacro_dffbe #(
                    .DFF_WIDTH          (RAM_WIDTH),
                    .DFF_RESET_VALUE    (RAM_RESET_VALUE[RAM_WIDTH * i +: RAM_WIDTH])
                ) stdmacro_dffbe_INST_dffram_dff (
                    .clk    (clk),
                    .reset  (reset),

                    .en     (dff_we),
                    .d      (din),

                    .q      (dff_q)
                );

                assign dff_we  = we & { (RAM_WIDTH){en} } & { (RAM_WIDTH){ dff_i_addr[i] } };
            end
            else begin

                stdmacro_dffe #(
                    .DFF_WIDTH          (RAM_WIDTH),
                    .DFF_RESET_VALUE    (RAM_RESET_VALUE[RAM_WIDTH * i +: RAM_WIDTH])
                ) stdmacro_dffe_INST_dffram_dff (
                    .clk    (clk),
                    .reset  (reset),

                    .en     (dff_we),
                    .d      (din),

                    .q      (dff_q)
                );

                assign dff_we   = we & en & dff_i_addr[i];
            end

            //
            assign dff_o_dout[RAM_WIDTH * i +: RAM_WIDTH] = dff_q & {(RAM_WIDTH){ dff_i_addr[i] }};

            //
            if (DATA_COUPLED) begin :GENERATED_DATA_COUPLED
                assign tdout[RAM_WIDTH * i +: RAM_WIDTH]  = dff_q;
            end

            //
        end
    endgenerate

    //
    macro_reduction_or #(
        .INPUT_WIDTH    (RAM_WIDTH),
        .INPUT_COUNT    (RAM_DEPTH)
    ) macro_reduction_or_INST_output (
        .d  (dff_o_dout),
        .q  (dout)
    );

    //

endmodule
