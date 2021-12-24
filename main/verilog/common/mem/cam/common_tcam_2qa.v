// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Coupling Content Addressable Memory module
//  (Data coupled with external RAM module)
//  (2 content-address port)
//
// @author Kumonda221
//

module common_tcam_2qa #(
    parameter       CAM_DEPTH                   = 1,
    parameter       CAM_WIDTH                   = 1,

    parameter       PORTA_QUERY_ADDRESS_ONEHOT  = 0,
    parameter       PORTB_QUERY_ADDRESS_ONEHOT  = 0,

    localparam      QUERY_ADDRESS_PORTA_WIDTH   = PORTA_QUERY_ADDRESS_ONEHOT ? CAM_DEPTH : $clog2(CAM_DEPTH),
    localparam      QUERY_ADDRESS_PORTB_WIDTH   = PORTB_QUERY_ADDRESS_ONEHOT ? CAM_DEPTH : $clog2(CAM_DEPTH)
) (
    //
    input   wire [CAM_DEPTH * CAM_WIDTH - 1:0]      tdata,

    //
    input   wire [CAM_DEPTH - 1:0]                  dvalid,

    //
    input   wire [CAM_WIDTH - 1:0]                  qdataa,
    output  wire [QUERY_ADDRESS_PORTA_WIDTH - 1:0]  qaddra,
    output  wire                                    qvalida,

    //
    input   wire [CAM_WIDTH - 1:0]                  qdatab,
    output  wire [QUERY_ADDRESS_PORTB_WIDTH - 1:0]  qaddrb,
    output  wire                                    qvalidb
);

    //
    wire [(1 << $clog2(CAM_DEPTH)) - 1:0]   qaddra_1h;
    wire [(1 << $clog2(CAM_DEPTH)) - 1:0]   qaddrb_1h;

    //
    genvar i;
    generate

        for (i = 0; i < CAM_DEPTH; i = i + 1) begin: GENERATED_CAM_COMPARSION_A
            assign qaddra_1h[i] = dvalid[i] && (qdataa == tdata[CAM_WIDTH * i +: CAM_WIDTH]);
        end

        for (i = 0; i < CAM_DEPTH; i = i + 1) begin: GENERATED_CAM_COMPARSION_B
            assign qaddrb_1h[i] = dvalid[i] && (qdatab == tdata[CAM_WIDTH * i +: CAM_WIDTH]);
        end
    endgenerate

    //
    generate
        
        if (PORTA_QUERY_ADDRESS_ONEHOT) begin: GENERATED_QUERY_ADDRESS_PORTA_ONEHOT
            
            assign qaddra   = qaddra_1h;
            assign qvalida  = 1'b1;
        end
        else begin: GENERATED_QUERY_ADDRESS_PORTA_BINARY
            
            macro_encoder_onehot_bin #(
                .OUTPUT_WIDTH   (QUERY_ADDRESS_PORTA_WIDTH)
            ) macro_encoder_onehot_bin_INST_qaddra (
                .d      (qaddra_1h),
                .q      (qaddra)
            );

            macro_encoder_onehot_detect #(
                .INPUT_WIDTH    (1 << $clog2(CAM_DEPTH))
            ) macro_encoder_onehot_detect_INST_qvalida (
                .d      (qaddra_1h),
                .valid  (qvalida)
            );
        end

        if (PORTB_QUERY_ADDRESS_ONEHOT) begin: GENERATED_QUERY_ADDRESS_PORTB_ONEHOT
            
            assign qaddrb   = qaddrb_1h;
            assign qvalidb  = 1'b1;
        end
        else begin: GENERATED_QUERY_ADDRESS_PORTB_BINARY
            
            macro_encoder_onehot_bin #(
                .OUTPUT_WIDTH   (QUERY_ADDRESS_PORTB_WIDTH)
            ) macro_encoder_onehot_bin_INST_qaddrb (
                .d      (qaddrb_1h),
                .q      (qaddrb)
            );

            macro_encoder_onehot_detect #(
                .INPUT_WIDTH    (1 << $clog2(CAM_DEPTH))
            ) macro_encoder_onehot_detect_INST_qvalidb (
                .d      (qaddrb_1h),
                .valid  (qvalidb)
            );
        end
    endgenerate

    //

endmodule
