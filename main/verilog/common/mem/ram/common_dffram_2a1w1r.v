// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF-base RAM module with Write Enable
//  (Simple dual-port: 2 address ports, 1 read port, 1 write port)
//
// @author Kumonda221
//

module common_dffram_2a1w1r #(
    parameter                                       RAM_WIDTH               = 1,
    parameter                                       RAM_DEPTH               = 1,
    parameter [RAM_DEPTH * RAM_WIDTH - 1:0]         RAM_RESET_VALUE         = { (RAM_DEPTH){ {(RAM_WIDTH){1'b0}} } },

    parameter                                       PORTA_ONEHOT_ADDRESSING = 0,
    parameter                                       PORTA_BIT_WRITE_ENABLE  = 0,

    parameter                                       PORTB_ONEHOT_ADDRESSING = 0,

    parameter                                       DATA_COUPLED            = 0,

    localparam                                      RAM_PORTA_ADDR_WIDTH    = PORTA_ONEHOT_ADDRESSING ? RAM_DEPTH : $clog2(RAM_DEPTH),
    localparam                                      RAM_PORTA_WE_WIDTH      = PORTA_BIT_WRITE_ENABLE ? RAM_WIDTH : 1,

    localparam                                      RAM_PORTB_ADDR_WIDTH    = PORTB_ONEHOT_ADDRESSING ? RAM_DEPTH : $clog2(RAM_DEPTH),

    localparam                                      RAM_PORT_TDATA_WIDTH    = DATA_COUPLED ? RAM_DEPTH * RAM_WIDTH : 1
) (
    input  wire                                 clk,
    input  wire                                 reset,

    // Port A - write only
    input  wire [RAM_PORTA_ADDR_WIDTH - 1:0]    addra,
    input  wire                                 ena,
    input  wire [RAM_PORTA_WE_WIDTH - 1:0]      wea,

    input  wire [RAM_WIDTH - 1:0]               dina,

    // Port B - read only
    input  wire [RAM_PORTB_ADDR_WIDTH - 1:0]    addrb,

    output wire [RAM_WIDTH - 1:0]               doutb,

    // Data coupled output
    output wire [RAM_PORT_TDATA_WIDTH - 1:0]    tdout
);
    //
    wire [RAM_WIDTH * RAM_DEPTH - 1:0]          dff_o_dout;
    wire [(1 << ($clog2(RAM_DEPTH))) - 1:0]     dff_i_addra;
    wire [(1 << ($clog2(RAM_DEPTH))) - 1:0]     dff_i_addrb;

    //
    generate 
        if (PORTA_ONEHOT_ADDRESSING) begin :GENERATED_ADDRESS_INPUT_PORTA_ONEHOT
            
            assign dff_i_addra = addra;
        end
        else begin :GENERATED_ADDRESS_INPUT_PORTA_BINARY
            
            macro_decoder_onehot_bin #(
                .INPUT_WIDTH    (RAM_PORTA_ADDR_WIDTH)
            ) macro_decoder_onehot_bin_INST_addra (
                .d  (addra),
                .q  (dff_i_addra)
            );
        end

        if (PORTB_ONEHOT_ADDRESSING) begin :GENERATED_ADDRESS_INPUT_PORTB_ONEHOT

            assign dff_i_addrb = addrb;
        end
        else begin :GENERATED_ADDRESS_INPUT_PORTB_BINARY
            
            macro_decoder_onehot_bin #(
                .INPUT_WIDTH    (RAM_PORTB_ADDR_WIDTH)
            ) macro_decoder_onehot_bin_INST_addrb (
                .d  (addrb),
                .q  (dff_i_addrb)
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
            wire [RAM_PORTA_WE_WIDTH - 1:0] dff_we;

            //
            if (PORTA_BIT_WRITE_ENABLE) begin

                stdmacro_dffbe #(
                    .DFF_WIDTH          (RAM_WIDTH),
                    .DFF_RESET_VALUE    (RAM_RESET_VALUE[RAM_WIDTH * i +: RAM_WIDTH])
                ) stdmacro_dffbe_INST_dffram_dff (
                    .clk    (clk),
                    .reset  (reset),

                    .en     (dff_we),
                    .d      (dina),

                    .q      (dff_q)
                );

                assign dff_we  = wea & { (RAM_WIDTH){ena} } & { (RAM_WIDTH){ dff_i_addra[i] } };
            end
            else begin

                stdmacro_dffe #(
                    .DFF_WIDTH          (RAM_WIDTH),
                    .DFF_RESET_VALUE    (RAM_RESET_VALUE[RAM_WIDTH * i +: RAM_WIDTH])
                ) stdmacro_dffe_INST_dffram_dff (
                    .clk    (clk),
                    .reset  (reset),

                    .en     (dff_we),
                    .d      (dina),

                    .q      (dff_q)
                );

                assign dff_we  = wea & ena & dff_i_addra[i];
            end

            //
            assign dff_o_dout[RAM_WIDTH * i +: RAM_WIDTH] = dff_q & {(RAM_WIDTH){ dff_i_addrb[i] }};

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
        .q  (doutb)
    );

    //

endmodule
