// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  DFF-base Content Addressable Memory module
//  (1 address port, 1 write port, 1 read port, 1 content-address port)
//
// @author Kumonda221
//

`define     ADDR_WIDTH          (1 << $clog2(CAM_DEPTH))

module common_dffcam_1a1w1r1qa #(
    parameter                               CAM_DEPTH                   = 1,
    parameter                               CAM_WIDTH                   = 1,
    parameter [CAM_DEPTH * CAM_WIDTH - 1:0] CAM_CONTENT_RESET_VALUE     = { (CAM_DEPTH){ {(CAM_WIDTH){1'b0}} } },
    parameter [CAM_DEPTH - 1:0]             CAM_VALID_RESET_VALUE       = { (CAM_DEPTH){ 1'b0 } },

    parameter                               CAM_ADDRESS_ONEHOT          = 0,
    parameter                               QUERY_ADDRESS_ONEHOT        = 0,
    parameter                               BIT_WRITE_ENABLE            = 0,

    localparam                              CAM_ADDRESS_PORT_WIDTH      = CAM_ADDRESS_ONEHOT ? CAM_DEPTH : $clog2(CAM_DEPTH),
    localparam                              QUERY_ADDRESS_PORT_WIDTH    = QUERY_ADDRESS_ONEHOT ? CAM_DEPTH : $clog2(CAM_DEPTH),
    localparam                              WE_PORT_WIDTH               = BIT_WRITE_ENABLE ? CAM_WIDTH : 1
) (
    input   wire                                    clk,
    input   wire                                    reset,

    //
    input   wire [CAM_ADDRESS_PORT_WIDTH - 1:0]     addr,
    input   wire                                    en,
    input   wire [WE_PORT_WIDTH - 1:0]              we,

    input   wire [CAM_WIDTH - 1:0]                  din,
    input   wire                                    din_valid,

    output  wire [CAM_WIDTH - 1:0]                  dout,
    output  wire                                    dout_valid,

    //
    input   wire [CAM_WIDTH - 1:0]                  qdata,
    output  wire [QUERY_ADDRESS_PORT_WIDTH - 1:0]   qaddr,
    output  wire                                    qvalid
);
    //
    wire [CAM_DEPTH * CAM_WIDTH - 1:0]  tdata;
    wire [CAM_DEPTH - 1:0]              tvalid;

    wire [(1 << $clog2(CAM_DEPTH)) - 1 : 0] addr_1h;

    //
    generate
        if (CAM_ADDRESS_ONEHOT) begin: GENERATED_CAM_ADDRESS_ONEHOT_INPUT

            assign addr_1h = addr;
        end
        else begin: GENERATED_CAM_ADDRESS_BINARY_INPUT
            
            macro_decoder_onehot_bin #(
                .INPUT_WIDTH    (CAM_ADDRESS_PORT_WIDTH)
            ) macro_decoder_onehot_bin_INST_addr (
                .d  (addr),
                .q  (addr_1h)
            );
        end
    endgenerate

    //
    common_dffram_1a1w1r #(
        .RAM_DEPTH          (CAM_DEPTH),
        .RAM_WIDTH          (CAM_WIDTH),
        .RAM_RESET_VALUE    (CAM_CONTENT_RESET_VALUE),

        .ONEHOT_ADDRESSING  (1),
        .BIT_WRITE_ENABLE   (BIT_WRITE_ENABLE),
        .DATA_COUPLED       (1)
    ) common_dffram_1a1w1r_INST_content (
        .clk    (clk),
        .reset  (reset),

        .addr   (addr_1h),
        .en     (en),
        .we     (we),

        .din    (din),
        .dout   (dout),

        .tdout  (tdata)
    );

    common_dffram_1a1w1r #(
        .RAM_DEPTH          (CAM_DEPTH),
        .RAM_WIDTH          (1),
        .RAM_RESET_VALUE    (CAM_VALID_RESET_VALUE),

        .ONEHOT_ADDRESSING  (1),
        .BIT_WRITE_ENABLE   (0),
        .DATA_COUPLED       (1)
    ) common_dffram_1a1w1r_INST_valid (
        .clk    (clk),
        .reset  (reset),
        
        .addr   (addr_1h),
        .en     (en),
        .we     (we),

        .din    (din_valid),
        .dout   (dout_valid),
        
        .tdout  (tvalid)
    );

    //
    common_tcam_1qa #(
        .CAM_DEPTH              (CAM_DEPTH),
        .CAM_WIDTH              (CAM_WIDTH),
        .QUERY_ADDRESS_ONEHOT   (QUERY_ADDRESS_ONEHOT)
    ) common_tcam_1qa_INST (
        .tdata  (tdata),
        .dvalid (tvalid),

        .qdata  (qdata),
        .qaddr  (qaddr),
        .qvalid (qvalid)
    );

    //

endmodule
