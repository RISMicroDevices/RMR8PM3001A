// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  One-hot Detector ('valid' is asserted only when the input is one-hot)
//
// @author Kumonda221
//

module macro_encoder_onehot_detect #(
    parameter           INPUT_WIDTH = 1
) (
    input   wire [INPUT_WIDTH - 1:0]    d,
    output  wire                        valid
);
    // 
    localparam P_COUNT  = 1 << $clog2(INPUT_WIDTH);

    /* verilator lint_off UNOPTFLAT */
    wire    d_tree_d[(P_COUNT * 2 - 1) - 1:0];
    wire    d_tree_c[(P_COUNT * 2 - 1) - 1:0];

    //
    genvar i, j;

    //
    generate
        for (i = 0; i < P_COUNT; i = i + 1) begin: GENERATED_TREE_INPUT
            
            if (i < INPUT_WIDTH) begin
                assign d_tree_d[(P_COUNT * 2 - 1) - 1 - i] = d[i];
            end
            else begin
                assign d_tree_d[(P_COUNT * 2 - 1) - 1 - i] = 1'b0;
            end

            assign d_tree_c[(P_COUNT * 2 - 1) - 1 - i] = 1'b0;
        end
    endgenerate

    //
    generate
        for (i = 0; i < $clog2(INPUT_WIDTH); i = i + 1) begin: GENERATED_TREE
            for (j = 0; j < (1 << i); j = j + 1) begin
                
                wire    da;
                wire    cina;

                wire    db;
                wire    cinb;

                wire    q;
                wire    cout;

                macro_encoder_onehot_detect_unit macro_encoder_onehot_detect_unit_INST (
                    .da     (da),
                    .cina   (cina),

                    .db     (db),
                    .cinb   (cinb),

                    .q      (q),
                    .cout   (cout)
                );

                assign d_tree_d[((1 << i) - 1) + j] = q;
                assign d_tree_c[((1 << i) - 1) + j] = cout;

                assign da   = d_tree_d[((1 << (i + 1)) - 1) + j * 2];
                assign cina = d_tree_c[((1 << (i + 1)) - 1) + j * 2];

                assign db   = d_tree_d[((1 << (i + 1)) - 1) + j * 2 + 1];
                assign cinb = d_tree_c[((1 << (i + 1)) - 1) + j * 2 + 1];
            end
        end
    endgenerate

    //
    assign valid = d_tree_d[0] & ~d_tree_c[0];

endmodule

//
module macro_encoder_onehot_detect_unit (
    input   wire                da,
    input   wire                cina,

    input   wire                db,
    input   wire                cinb,

    output  wire                q,
    output  wire                cout
);

    assign q    = da | db;
    assign cout = cina | cinb | (da & db);

endmodule
