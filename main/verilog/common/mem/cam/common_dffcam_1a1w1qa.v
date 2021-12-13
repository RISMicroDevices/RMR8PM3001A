// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF-base Content Addressable Memory module
//  (1 binary address port, 1 write port, 1 content-address port)
//
// @author Kumonda221
//

`define     ADDR_WIDTH          (1 << $clog2(CAM_DEPTH))

module common_dffcam_1a1w1qa #(
    parameter       CAM_DEPTH                   = 1,
    parameter       CAM_WIDTH                   = 1,

    parameter       WRITE_ADDRESS_ONEHOT        = 0,
    parameter       QUERY_ADDRESS_ONEHOT        = 0,

    localparam      WRITE_ADDRESS_PORT_WIDTH    = WRITE_ADDRESS_ONEHOT ? CAM_DEPTH : (1 << $clog2(CAM_DEPTH)),
    localparam      QUERY_ADDRESS_PORT_WIDTH    = QUERY_ADDRESS_ONEHOT ? CAM_DEPTH : (1 << $clog2(CAM_DEPTH))
) (
    input   wire                                    clk,
    input   wire                                    reset,

    //
    input   wire [WRITE_ADDRESS_PORT_WIDTH - 1:0]   waddr,
    input   wire                                    wen,
    input   wire [CAM_WIDTH - 1:0]                  wdata,

    //
    input   wire [CAM_DEPTH - 1:0]                  dvalid,

    //
    input   wire [CAM_WIDTH - 1:0]                  qdata,
    output  wire [QUERY_ADDRESS_PORT_WIDTH - 1:0]   qaddr  
);
    //
    wire [(1 << $clog2(CAM_DEPTH)) - 1:0]   qaddr_1h;
    wire [(1 << $clog2(CAM_DEPTH)) - 1:0]   waddr_1h;

    //
    generate

        if (WRITE_ADDRESS_ONEHOT) begin: GENERATED_WRITE_ADDRESS_PORT_ONEHOT
            
            assign waddr_1h = waddr;
        end
        else begin: GENERATED_WRITE_ADDRESS_PORT_BINARY
            
            macro_decoder_onehot_bin #(
                .INPUT_WIDTH(WRITE_ADDRESS_PORT_WIDTH)
            ) macro_decoder_onehot_bin_INST_waddr (
                .d  (waddr),
                .q  (waddr_1h)
            );
        end
    endgenerate

    //
    genvar i;
    generate
        for (i = 0; i < CAM_DEPTH; i = i + 1) begin: GENERATED_CONTENT_DFFS
            
            //
            wire                    dff_wen;
            wire [CAM_WIDTH - 1:0]  dff_d;
            wire [CAM_WIDTH - 1:0]  dff_q;

            //
            stdmacro_dffe #(
                .DFF_WIDTH(CAM_WIDTH)
            ) stdmacro_dffe_INST_content (
                .clk    (clk),
                .reset  (reset),

                .en     (dff_wen),
                .d      (dff_d),
                .q      (dff_q)
            );

            //
            assign  dff_wen = wen & waddr_1h[i];
            assign  dff_d   = wdata;

            //
            assign  qaddr_1h[i] = dff_q == qdata;
        end
    endgenerate

    //
    generate
        
        if (QUERY_ADDRESS_ONEHOT) begin: GENERATED_QUERY_ADDRESS_PORT_ONEHOT
            
            assign qaddr = qaddr_1h;
        end
        else begin: GENERATED_QUERY_ADDRESS_PORT_BINARY
            
            macro_encoder_onehot_bin #(
                .OUTPUT_WIDTH(QUERY_ADDRESS_PORT_WIDTH)
            ) macro_encoder_onehot_bin_INST_qaddr (
                .d  (qaddr_1h),
                .q  (qaddr)
            );
        end
    endgenerate

    //

endmodule
