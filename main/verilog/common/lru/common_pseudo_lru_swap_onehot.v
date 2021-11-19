// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Pseudo-LRU for Swap Algorithm with One-Hot input and output
//
// @author Kumonda221
//

module common_pseudo_lru_swap_onehot #(
    parameter       SUBJECT_COUNT_LOG2      = 1
) (
    input   wire                                    clk,
    input   wire                                    reset,

    //
    input   wire [(1 << SUBJECT_COUNT_LOG2) - 1:0]  waddr,
    input   wire                                    wen,

    //
    output  wire [(1 << SUBJECT_COUNT_LOG2) - 1:0]  qaddr
);

    //
    localparam P_COUNT  = (1 << (SUBJECT_COUNT_LOG2));

    /* verilator lint_off UNOPTFLAT */
    wire    d_tree_c  [(P_COUNT * 2 - 1) - 1:0];
    wire    d_tree_sel[(P_COUNT * 2 - 1) - 1:0];

    //
    genvar i, j;

    //
    generate
        for (i = 0; i < P_COUNT; i = i + 1) begin: GENERATED_LRU_TREE_INPUT
            
            assign d_tree_c[(P_COUNT - 1) + i] = waddr[i] & wen;
        end
    endgenerate

    //
    generate
        for (i = 0; i < SUBJECT_COUNT_LOG2; i = i + 1) begin: GENERATED_LRU_TREE
            for (j = 0; j < (1 << i); j = j + 1) begin
                
                //
                wire    cina;
                wire    cinb;
                wire    cout;

                wire    dsel;
                wire    qsela;
                wire    qselb;

                common_pseudo_lru_swap_unit common_pseudo_lru_swap_unit_INST (
                    .clk    (clk),
                    .reset  (reset),

                    .cina   (cina),
                    .cinb   (cinb),
                    .cout   (cout),

                    .dsel   (dsel),
                    .qsela  (qsela),
                    .qselb  (qselb)
                );

                //
                assign cina     = d_tree_c[((1 << (i + 1)) - 1) + j * 2];
                assign cinb     = d_tree_c[((1 << (i + 1)) - 1) + j * 2 + 1];

                assign dsel     = d_tree_sel[((1 << i) - 1) + j];

                //
                assign d_tree_c[((1 << i) - 1) + j]                 = cout;

                assign d_tree_sel[((1 << (i + 1)) - 1) + j * 2]     = qsela;
                assign d_tree_sel[((1 << (i + 1)) - 1) + j * 2 + 1] = qselb;
            end
        end
    endgenerate

    assign d_tree_sel[0]    = 1'b1;

    //
    generate
        for (i = 0; i < P_COUNT; i = i + 1) begin: GENERATED_LRU_TREE_OUTPUT

            assign qaddr[i] = d_tree_sel[(P_COUNT - 1) + i];
        end
    endgenerate


endmodule

//
module common_pseudo_lru_swap_unit (
    input   wire                clk,
    input   wire                reset,

    //
    input   wire                cina,
    input   wire                cinb,
    output  wire                cout,

    //
    input   wire                dsel,
    output  wire                qsela,
    output  wire                qselb
);

    wire    dff_en;
    wire    dff_d;

    wire    dff_q;

    stdmacro_dffe #(
        .DFF_WIDTH          (1),
        .DFF_RESET_VALUE    (1'b0)
    ) stdmacro_dffe_INST_lru (
        .clk    (clk),
        .reset  (reset),

        .en     (dff_en),
        .d      (dff_d),
        
        .q      (dff_q)
    );

    assign dff_en = cina | cinb;
    assign dff_d  = cina;

    assign cout   = dff_en;

    assign qsela  = dsel & ~dff_q;
    assign qselb  = dsel &  dff_q;

endmodule
