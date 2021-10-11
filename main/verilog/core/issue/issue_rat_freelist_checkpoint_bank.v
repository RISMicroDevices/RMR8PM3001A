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

module issue_rat_freelist_checkpoint_bank (
    input   wire                clk,
    input   wire                reset,

    //
    input   wire                tag_i_en,
    input   wire                tag_i_valid,
    input   wire                tag_i_abandoned,

    input   wire                fgr_i_en,
    input   wire [2:0]          fgr_i,

    //
    output  wire                tag_o_valid,
    output  wire                tag_o_abandoned,

    output  wire [2:0]          fgr_o,

    //
    input   wire                fifo_i_wen,
    input   wire [5:0]          fifo_i_prf,

    input   wire                fifo_i_ren,
    output  wire [5:0]          fifo_o_prf,

    output  wire                fifo_o_full,
    output  wire                fifo_o_empty
);

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
    assign  tag_valid_en = tag_i_en;

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
    assign  tag_abandoned_en = tag_i_en;

    //
    assign  tag_o_abandoned  = tag_abandoned_q;


    // Tag: Bank associated FGR
    wire [2:0]  tag_fgr_d;
    wire        tag_fgr_en;

    wire [2:0]  tag_fgr_q;

    stdmacro_dffe #(
        .DFF_WIDTH          (3),
        .DFF_RESET_VALUE    (3'b0)
    ) stdmacro_dffe_INST_cp_tag_fgr (
        .clk    (clk),
        .reset  (reset),

        .en     (tag_fgr_en),
        .d      (tag_fgr_d),

        .q      (tag_fgr_q)
    );

    assign  tag_fgr_d  = fgr_i;
    assign  tag_fgr_en = fgr_i_en;

    //
    assign  fgr_o      = tag_fgr_q;


    // FIFO: stored RAT entries
    wire [5:0]      fifo_din;
    wire            fifo_wen;

    wire [5:0]      fifo_dout;
    wire            fifo_ren;

    wire            fifo_full;
    wire            fifo_empty;

    common_fifo_ram_1w1r #(
        .FIFO_DEPTH_LOG2    (2),
        .FIFO_WIDTH         (6)
    ) common_fifo_ram_1w1r_INST_fifo (
        .clk(clk),
        .reset(reset),

        .din(fifo_din),
        .wen(fifo_wen),

        .dout(fifo_dout),
        .ren(fifo_ren),

        .fifo_full(fifo_full),
        .fifo_empty(fifo_empty)
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
