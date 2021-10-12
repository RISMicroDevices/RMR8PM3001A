// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  PRF free list checkpoint bank for 64-entry RAT
//  (4 entries per-bank, FGR associated)
//
// @author Kumonda221
//

module issue_rat_freelist_checkpoint_bank #(
    parameter   PRF_WIDTH       = 6,
    parameter   FGR_WIDTH       = 3
) (
    input   wire                        clk,
    input   wire                        reset,

    //
    input   wire                        tag_i_wen,
    input   wire                        tag_i_valid,
    input   wire                        tag_i_abandoned,

    output  wire                        tag_o_valid,
    output  wire                        tag_o_abandoned,

    //
    input   wire                        fgr_i_wen,
    input   wire [FGR_WIDTH - 1:0]      fgr_i,

    output  wire [FGR_WIDTH - 1:0]      fgr_o,

    //
    input   wire                        fifo_i_reset,

    input   wire                        fifo_i_wen,
    input   wire [PRF_WIDTH - 1:0]      fifo_i_prf,

    input   wire                        fifo_i_ren,
    output  wire [PRF_WIDTH - 1:0]      fifo_o_prf,

    output  wire                        fifo_o_full,
    output  wire                        fifo_o_empty
);
    //
    localparam  BANK_DEPTH_LOG2     = 2;

    // Tag: Bank valid
    wire    tag_valid_d;
    wire    tag_valid_en;

    wire    tag_valid_q;

    stdmacro_dffe #(
        .DFF_WIDTH          (1),
        .DFF_RESET_VALUE    (1'b0)
    ) stdmacro_dffe_INST_cp_tag_valid (
        .clk    (clk),
        .reset  (reset),

        .en     (tag_valid_en),
        .d      (tag_valid_d),

        .q      (tag_valid_q)
    );

    assign  tag_valid_d  = tag_i_valid;
    assign  tag_valid_en = tag_i_wen;

    //
    assign  tag_o_valid  = tag_valid_q;


    // Tag: Bank abandoned
    wire    tag_abandoned_d;
    wire    tag_abandoned_en;

    wire    tag_abandoned_q;

    stdmacro_dffe #(
        .DFF_WIDTH          (1),
        .DFF_RESET_VALUE    (1'b0)
    ) stdmacro_dffe_INST_cp_tag_abandoned (
        .clk    (clk),
        .reset  (reset),
        
        .en     (tag_abandoned_en),
        .d      (tag_abandoned_d),

        .q      (tag_abandoned_q)
    );

    assign  tag_abandoned_d  = tag_i_abandoned;
    assign  tag_abandoned_en = tag_i_wen;

    //
    assign  tag_o_abandoned  = tag_abandoned_q;


    // Tag: Bank associated FGR
    wire [FGR_WIDTH - 1:0]  tag_fgr_d;
    wire                    tag_fgr_en;

    wire [FGR_WIDTH - 1:0]  tag_fgr_q;

    stdmacro_dffe #(
        .DFF_WIDTH          (FGR_WIDTH),
        .DFF_RESET_VALUE    ('b0)
    ) stdmacro_dffe_INST_cp_tag_fgr (
        .clk    (clk),
        .reset  (reset),

        .en     (tag_fgr_en),
        .d      (tag_fgr_d),

        .q      (tag_fgr_q)
    );

    assign  tag_fgr_d  = fgr_i;
    assign  tag_fgr_en = fgr_i_wen;

    //
    assign  fgr_o      = tag_fgr_q;


    // FIFO: stored RAT entries
    wire [PRF_WIDTH - 1:0]  fifo_din;
    wire                    fifo_wen;

    wire [PRF_WIDTH - 1:0]  fifo_dout;
    wire                    fifo_ren;

    wire                    fifo_full;
    wire                    fifo_empty;

    common_fifo_ram_1w1r #(
        .FIFO_DEPTH_LOG2    (BANK_DEPTH_LOG2),
        .FIFO_WIDTH         (PRF_WIDTH),
        .FIFO_RESET_STATE   (0)
    ) common_fifo_ram_1w1r_INST_fifo (
        .clk        (clk),
        .reset      (reset & fifo_i_reset),  // IMPORTANT: Reset on low
//      .reset      (reset | fifo_i_reset),

        .din        (fifo_din),
        .wen        (fifo_wen),

        .dout       (fifo_dout),
        .ren        (fifo_ren),

        .fifo_full  (fifo_full),
        .fifo_empty (fifo_empty)
    );

    assign  fifo_din   = fifo_i_prf;
    assign  fifo_wen   = fifo_i_wen;

    assign  fifo_ren   = fifo_i_ren;

    //
    assign  fifo_o_prf   = fifo_dout;

    assign  fifo_o_full  = fifo_full;
    assign  fifo_o_empty = fifo_empty;

    //

endmodule
