// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Coupling Content Addressable Memory module
//  (Data coupled with external RAM module)
//  (1 content-address port)
//
// @author Kumonda221
//

module common_tcam_1qa #(
    parameter       CAM_DEPTH                   = 1,
    parameter       CAM_WIDTH                   = 1,

    parameter       QUERY_ADDRESS_ONEHOT        = 0,

    localparam      QUERY_ADDRESS_PORT_WIDTH    = QUERY_ADDRESS_ONEHOT ? CAM_DEPTH : $clog2(CAM_DEPTH)
) (
    //
    input   wire [CAM_DEPTH * CAM_WIDTH - 1:0]      tdata,

    //
    input   wire [CAM_DEPTH - 1:0]                  dvalid,

    //
    input   wire [CAM_WIDTH - 1:0]                  qdata,
    output  wire [QUERY_ADDRESS_PORT_WIDTH - 1:0]   qaddr,
    output  wire                                    qvalid
); 
    //
    wire [(1 << $clog2(CAM_DEPTH)) - 1:0]   qaddr_1h;

    //
    genvar i;
    generate
        for (i = 0; i < CAM_DEPTH; i = i + 1) begin: GENERATED_CAM_COMPARSION
            assign qaddr_1h[i] = dvalid[i] && (qdata == tdata[CAM_WIDTH * i +: CAM_WIDTH]);
        end
    endgenerate

    //
    generate
        
        if (QUERY_ADDRESS_ONEHOT) begin: GENERATED_QUERY_ADDRESS_PORT_ONEHOT
            
            assign qaddr    = qaddr_1h;
            assign qvalid   = 1'b1;
        end
        else begin: GENERATED_QUERY_ADDRESS_PORT_BINARY
            
            macro_encoder_onehot_bin #(
                .OUTPUT_WIDTH   (QUERY_ADDRESS_PORT_WIDTH)
            ) macro_encoder_onehot_bin_INST_qaddr (
                .d      (qaddr_1h),
                .q      (qaddr)
            );

            macro_encoder_onehot_detect #(
                .INPUT_WIDTH    (1 << $clog2(CAM_DEPTH))
            ) macro_encoder_onehot_detect_INST_qvalid (
                .d      (qaddr_1h),
                .valid  (qvalid)
            );
        end
    endgenerate

    //

endmodule
