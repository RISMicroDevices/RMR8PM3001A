// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  PRF free list checkpoints for 64-entry RAT
//  (4 bank, 4 entries per-bank, FGR associated)
//
// @author Kumonda221
//

/*
* *Notice:
*   The behaviour of checkpoint was UNDEFINED, when abandon port and commit port
*   had the same valid input ('i_abandon_valid' and 'i_commit_valid' was both high,
*   while 'i_abandon_fgr' and 'i_commit_fgr' indicated the same FGR).
*/

module issue_rat_freelist_checkpoint #(
    parameter   PRF_WIDTH       = 6,
    parameter   FGR_WIDTH       = 4,
    parameter   BANK_COUNT      = 4
) (
    input   wire                        clk,
    input   wire                        reset,

    //
    input   wire                        i_abandon_valid,
    input   wire [FGR_WIDTH - 1:0]      i_abandon_fgr,

    //
    input   wire                        i_commit_valid,
    input   wire [FGR_WIDTH - 1:0]      i_commit_fgr,

    //
    output  wire                        o_abandoned_valid,
    input   wire                        i_abandoned_ready,

    output  wire [PRF_WIDTH - 1:0]      o_abandoned_prf,

    //
    input   wire                        i_acquired_valid,
    output  wire                        o_acquired_ready,

    input   wire [FGR_WIDTH - 1:0]      i_acquired_fgr,
    input   wire [PRF_WIDTH - 1:0]      i_acquired_prf
);

    // banks of checkpoints
    //
    wire                    tag_w_wen       [BANK_COUNT - 1:0];
    wire                    tag_w_valid     [BANK_COUNT - 1:0];
    wire                    tag_w_abandoned [BANK_COUNT - 1:0];

    wire                    tag_r_valid     [BANK_COUNT - 1:0];
    wire                    tag_r_abandoned [BANK_COUNT - 1:0];

    //
    wire                    fgr_w_wen       [BANK_COUNT - 1:0];
    wire [FGR_WIDTH - 1:0]  fgr_w           [BANK_COUNT - 1:0];

    wire [FGR_WIDTH - 1:0]  fgr_r           [BANK_COUNT - 1:0];

    //
    wire                    fifo_w_reset    [BANK_COUNT - 1:0];
    wire                    fifo_w_wen      [BANK_COUNT - 1:0];
    wire [PRF_WIDTH - 1:0]  fifo_w_prf      [BANK_COUNT - 1:0];

    wire                    fifo_r_ren      [BANK_COUNT - 1:0];
    wire [PRF_WIDTH - 1:0]  fifo_r_prf      [BANK_COUNT - 1:0];

    wire                    fifo_s_full     [BANK_COUNT - 1:0];
    wire                    fifo_s_empty    [BANK_COUNT - 1:0];

    //
    genvar i, j, k;

    //
    generate
        for (i = 0; i < BANK_COUNT; i = i + 1) begin :GENERATED_CHECKPOINT_BANK

            issue_rat_freelist_checkpoint_bank #(
                .PRF_WIDTH  (PRF_WIDTH),
                .FGR_WIDTH  (FGR_WIDTH)
            ) issue_rat_freelist_checkpoint_bank_INST (
                .clk                (clk),
                .reset              (reset),

                .tag_i_wen          (tag_w_wen[i]),
                .tag_i_valid        (tag_w_valid[i]),
                .tag_i_abandoned    (tag_w_abandoned[i]),

                .tag_o_valid        (tag_r_valid[i]),
                .tag_o_abandoned    (tag_r_abandoned[i]),

                .fgr_i_wen          (fgr_w_wen[i]),
                .fgr_i              (fgr_w[i]),

                .fgr_o              (fgr_r[i]),

                .fifo_i_reset       (fifo_w_reset[i]),
                .fifo_i_wen         (fifo_w_wen[i]),
                .fifo_i_prf         (fifo_w_prf[i]),
                
                .fifo_i_ren         (fifo_r_ren[i]),
                .fifo_o_prf         (fifo_r_prf[i]),
                
                .fifo_o_full        (fifo_s_full[i]),
                .fifo_o_empty       (fifo_s_empty[i])
            );
        end
    endgenerate

    // Checkpoint control signals
    wire [BANK_COUNT - 1:-1]    c_wen;
    wire [BANK_COUNT - 1:-1]    c_wb_sel;

    wire [BANK_COUNT - 1:0]     c_wb_done;
    wire [BANK_COUNT - 1:0]     c_abandon;
    wire [BANK_COUNT - 1:0]     c_commit;

    //
    /* verilator lint_off UNOPTFLAT */
    wire    c_wen_carrier       [BANK_COUNT - 1:-1];
    /* verilator lint_off UNOPTFLAT */
    wire    c_wb_sel_carrier    [BANK_COUNT - 1:-1];

    assign c_wen   [-1] = 1'b0;
    assign c_wb_sel[-1] = 1'b0;

    assign c_wen_carrier   [-1] = 1'b0;
    assign c_wb_sel_carrier[-1] = 1'b0;

    generate
        for (i = 0; i < BANK_COUNT; i = i + 1) begin : GENERATED_CHECKPOINT_CONTROL_READ_WRITE

            assign c_wen[i]     = ((~fifo_s_full[i] &  tag_r_valid[i] & (i_acquired_fgr == fgr_r[i]))
                                |  (~tag_r_valid[i] & ~tag_r_abandoned[i]))
                                & ~c_wen_carrier[i]
                                &  i_acquired_valid;

            assign c_wb_sel[i]  =  tag_r_abandoned[i]
                                & ~fifo_s_empty[i]
                                & ~c_wb_sel_carrier[i];

            assign c_wen_carrier[i]     = c_wen_carrier[i - 1]    | c_wen[i - 1];

            assign c_wb_sel_carrier[i]  = c_wb_sel_carrier[i - 1] | c_wb_sel[i - 1];
        end
    endgenerate
    //

    generate
        for (i = 0; i < BANK_COUNT; i = i + 1) begin : GENERATED_CHECKPOINT_CONTROL_STATE_TRANS

            assign c_wb_done[i]  = fifo_s_empty[i];

            assign c_abandon[i]  = i_abandon_valid  & (i_abandon_fgr  == fgr_r[i]);

            assign c_commit[i]   = i_commit_valid   & (i_commit_fgr   == fgr_r[i]);
        end
    endgenerate

    // Checkpoint update (state transition, read, write)
    wire [PRF_WIDTH - 1:0]  r_prf [BANK_COUNT - 1:0];

    generate
        for (i = 0; i < BANK_COUNT; i = i + 1) begin : GENERATED_CHECKPOINT_UPDATE

            // Tag update
            assign tag_w_wen[i] = 1'b1;

            assign tag_w_valid[i]       = ~tag_r_valid[i] 
                                        ?  c_wen[i] 
                                        : ~(c_commit[i] | c_abandon[i]);

            assign tag_w_abandoned[i]   = ~tag_r_abandoned[i] 
                                        ? (tag_r_valid[i] & c_abandon[i])
                                        : ~c_wb_done[i];

            // FGR update
            assign fgr_w_wen[i] = c_wen[i];

            assign fgr_w[i]     = i_acquired_fgr;

            // FIFO write
            assign fifo_w_reset[i]  = ~(tag_r_valid[i] & c_commit[i]);

            assign fifo_w_wen[i]    = c_wen[i];

            assign fifo_w_prf[i]    = i_acquired_prf;

            // FIFO read
            assign fifo_r_ren[i] = c_wb_sel[i] & i_abandoned_ready;

            assign r_prf[i] = { (PRF_WIDTH){c_wb_sel[i]} } & fifo_r_prf[i];
        end
    endgenerate
    //

    // output logic
    wire [PRF_WIDTH * BANK_COUNT - 1:0] r_prf_reduction_tree;
    wire [PRF_WIDTH - 1:0]              r_prf_reducted;

    generate 
        for (i = 0; i < BANK_COUNT; i = i + 1) begin
            
            assign r_prf_reduction_tree[i * PRF_WIDTH +: PRF_WIDTH] = r_prf[i];
        end
    endgenerate

    macro_reduction_tree_or #(
        .INPUT_WIDTH    (PRF_WIDTH),
        .INPUT_COUNT    (BANK_COUNT)
    ) macro_reduction_tree_or_INST_r_prf (
        .d(r_prf_reduction_tree),
        .q(r_prf_reducted)
    );

    //
    assign o_abandoned_prf   = r_prf_reducted;

    assign o_acquired_ready  = |{c_wen    [BANK_COUNT - 1:0]};

    assign o_abandoned_valid = |{c_wb_sel [BANK_COUNT - 1:0]};

    //

endmodule
