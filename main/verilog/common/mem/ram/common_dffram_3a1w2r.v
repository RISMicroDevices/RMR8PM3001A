// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF-base RAM module with Write Enable
//  (Simple dual-port: 3 address ports, 2 read ports, 1 write port)
//
// @author Kumonda221
//


`define     RAM_DEPTH       (1 << RAM_ADDR_WIDTH)

module common_dffram_3a1w2r #(
    parameter                                       RAM_DATA_WIDTH      = 1,
    parameter                                       RAM_DEPTH           = 1,
    parameter [RAM_DEPTH * RAM_DATA_WIDTH - 1:0]    RAM_RESET_VALUE     = { (RAM_DEPTH){ {(RAM_DATA_WIDTH){1'b0}} } },

    parameter                                       PORTA_ONEHOT_ADDRESSING = 0,
    parameter                                       PORTA_BIT_WRITE_ENABLE  = 0,

    parameter                                       PORTB_ONEHOT_ADDRESSING = 0,

    parameter                                       PORTC_ONEHOT_ADDRESSING = 0,

    localparam                                      RAM_PORTA_ADDR_WIDTH    = PORTA_ONEHOT_ADDRESSING ? RAM_DEPTH : $clog2(RAM_DEPTH),
    localparam                                      RAM_PORTA_WE_WIDTH      = PORTA_BIT_WRITE_ENABLE ? RAM_DATA_WIDTH : 1,

    localparam                                      RAM_PORTB_ADDR_WIDTH    = PORTB_ONEHOT_ADDRESSING ? RAM_DEPTH : $clog2(RAM_DEPTH),

    localparam                                      RAM_PORTC_ADDR_WIDTH    = PORTC_ONEHOT_ADDRESSING ? RAM_DEPTH : $clog2(RAM_DEPTH)
) (
    input  wire                             clk,
    input  wire                             reset,

    // Port A - write only
    input  wire [RAM_PORTA_ADDR_WIDTH - 1:0]    addra,
    input  wire                                 ena,
    input  wire                                 wea,

    input  wire [RAM_DATA_WIDTH - 1:0]          dina,

    // Port B - read only
    input  wire [RAM_PORTB_ADDR_WIDTH - 1:0]    addrb,

    output wire [RAM_DATA_WIDTH - 1:0]          doutb,

    // Port C - read only
    input  wire [RAM_PORTC_ADDR_WIDTH - 1:0]    addrc,
    
    output wire [RAM_DATA_WIDTH - 1:0]          doutc
);

    //
    wire [RAM_DATA_WIDTH * RAM_DEPTH - 1:0]     dff_o_doutb;
    wire [RAM_DATA_WIDTH * RAM_DEPTH - 1:0]     dff_o_doutc;
    wire [(1 << ($clog2(RAM_DEPTH))) - 1:0]     dff_i_addra;
    wire [(1 << ($clog2(RAM_DEPTH))) - 1:0]     dff_i_addrb;
    wire [(1 << ($clog2(RAM_DEPTH))) - 1:0]     dff_i_addrc;

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

        if (PORTC_ONEHOT_ADDRESSING) begin :GENERATED_ADDRESS_INPUT_PORTC_ONEHOT

            assign dff_i_addrc = addrc;
        end
        else begin :GENERATED_ADDRESS_INPUT_PORTC_BINARY

            macro_decoder_onehot_bin #(
                .INPUT_WIDTH    (RAM_PORTC_ADDR_WIDTH)
            ) macro_decoder_onehot_bin_INST_addrc (
                .d  (addrc),
                .q  (dff_i_addrc)
            );
        end
    endgenerate

    //
    genvar i;
    generate
        for (i = 0; i < RAM_DEPTH; i = i + 1) begin :GENERATED_RAM_DFFS

            //
            wire [RAM_DATA_WIDTH - 1:0]     dff_q;
            wire [RAM_PORTA_WE_WIDTH - 1:0] dff_we;

            //
            if (PORTA_BIT_WRITE_ENABLE) begin

                stdmacro_dffbe #(
                    .DFF_WIDTH          (RAM_DATA_WIDTH),
                    .DFF_RESET_VALUE    (RAM_RESET_VALUE[RAM_DATA_WIDTH * i +: RAM_DATA_WIDTH])
                ) stdmacro_dffbe_INST_dffram_dff (
                    .clk    (clk),
                    .reset  (reset),

                    .en     (dff_we),
                    .d      (dina),

                    .q      (dff_q)
                );

                assign dff_we  = wea & { (RAM_DATA_WIDTH){ena} } & { (RAM_DATA_WIDTH){ dff_i_addra[i] } };
            end
            else begin

                stdmacro_dffe #(
                    .DFF_WIDTH          (RAM_DATA_WIDTH),
                    .DFF_RESET_VALUE    (RAM_RESET_VALUE[RAM_DATA_WIDTH * i +: RAM_DATA_WIDTH])
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
            assign dff_o_doutb[RAM_DATA_WIDTH * i +: RAM_DATA_WIDTH]    = dff_q & {(RAM_DATA_WIDTH){ dff_i_addrb[i] }};

            assign dff_o_doutc[RAM_DATA_WIDTH * i +: RAM_DATA_WIDTH]    = dff_q & {(RAM_DATA_WIDTH){ dff_i_addrc[i] }};

            //
        end
    endgenerate

    //
    macro_reduction_or #(
        .INPUT_WIDTH    (RAM_DATA_WIDTH),
        .INPUT_COUNT    (RAM_DEPTH)
    ) macro_reduction_or_INST_doutb (
        .d  (dff_o_doutb),
        .q  (doutb)
    );

    macro_reduction_or #(
        .INPUT_WIDTH    (RAM_DATA_WIDTH),
        .INPUT_COUNT    (RAM_DEPTH)
    ) macro_reduction_or_INST_doutc (
        .d  (dff_o_doutc),
        .q  (doutc)
    );

    //

endmodule

