// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Pseudo-LRU for Swap Algorithm with Encoded Binary input and output
//
// @author Kumonda221
//

module common_pseudo_lru_swap_binwr #(
    parameter       SUBJECT_COUNT_LOG2      = 1
) (
    input   wire                                clk,
    input   wire                                reset,

    //
    input   wire [SUBJECT_COUNT_LOG2 - 1:0]     waddr,
    input   wire                                wen,

    //
    output  wire [SUBJECT_COUNT_LOG2 - 1:0]     qaddr
);

    //
    localparam  P_COUNT     = (1 << (SUBJECT_COUNT_LOG2));

    //
    wire [P_COUNT - 1:0]    onehot_waddr;
    wire                    onehot_wen;

    wire [P_COUNT - 1:0]    onehot_qaddr;

    common_pseudo_lru_swap_onehot #(
        .SUBJECT_COUNT_LOG2(SUBJECT_COUNT_LOG2)
    ) common_pseudo_lru_swap_onehot_INST (
        .clk    (clk),
        .reset  (reset),

        .waddr  (onehot_waddr),
        .wen    (onehot_wen),

        .qaddr  (onehot_qaddr)
    );

    //
    wire [SUBJECT_COUNT_LOG2 - 1:0] decoder_d;
    wire [P_COUNT - 1:0]            decoder_q;

    macro_decoder_onehot_bin #(
        .INPUT_WIDTH(SUBJECT_COUNT_LOG2)
    ) macro_decoder_onehot_bin_INST (
        .d  (decoder_d),
        .q  (decoder_q)
    );

    // 
    wire [P_COUNT - 1:0]            encoder_d;
    wire [SUBJECT_COUNT_LOG2 - 1:0] encoder_q;
    
    macro_encoder_onehot_bin #(
        .OUTPUT_WIDTH(SUBJECT_COUNT_LOG2)
    ) macro_encoder_onehot_bin_INST (
        .d  (encoder_d),
        .q  (encoder_q)
    );

    //
    assign onehot_waddr = decoder_q;
    assign onehot_wen   = wen;

    assign decoder_d    = waddr;
    assign encoder_d    = onehot_qaddr;

    assign qaddr        = encoder_q;

    //

endmodule
