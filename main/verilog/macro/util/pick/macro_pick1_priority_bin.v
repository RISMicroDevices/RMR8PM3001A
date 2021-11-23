// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Wire priority multiple-valid pick-one algorithm with binary output MACRO
//  (Always picking lower index first)
//
// @author Kumonda221
//


`define     INPUT_WIDTH             INPUT_COUNT
`define     OUTPUT_WIDTH            $clog2(INPUT_COUNT)

`define     PRIORITY_LOWER_FIRST    0
`define     PRIORITY_HIGHER_FIRST   1

module macro_pick1_priority_bin #(
    parameter               INPUT_COUNT = 1,
    parameter               PRIORITY    = `PRIORITY_LOWER_FIRST
) (
    input   wire [`INPUT_WIDTH - 1:0]       dvalid,
    output  wire [`OUTPUT_WIDTH - 1:0]      qaddr,
    output  wire                            qvalid
);

    //
    localparam P_COUNT  = 1 << $clog2(`INPUT_WIDTH);

    /* verilator lint_off UNOPTFLAT */
    wire [`OUTPUT_WIDTH - 1:0]      d_pick_tree_addr  [(P_COUNT * 2 - 1) - 1:0];
    wire                            d_pick_tree_valid [(P_COUNT * 2 - 1) - 1:0];

    //
    genvar i, j;

    //
    generate
        for (i = 0; i < P_COUNT; i = i + 1) begin: GENERATED_PICK_TREE_INPUT

            if (i < `INPUT_WIDTH) begin
                
                assign d_pick_tree_addr[(P_COUNT - 1) + i]  = i;
                assign d_pick_tree_valid[(P_COUNT - 1) + i] = dvalid[i];
            end
            else begin
                
                assign d_pick_tree_addr[(P_COUNT - 1) + i]  = 'b0;
                assign d_pick_tree_valid[(P_COUNT - 1) + i] = 'b0;
            end
        end
    endgenerate

    //
    generate
        for (i = 0; i < $clog2(`INPUT_WIDTH); i = i + 1) begin: GENERATED_PICK_TREE
            for (j = 0; j < (1 << i); j = j + 1) begin
                
                //
                wire [`OUTPUT_WIDTH - 1:0]  da;
                wire                        valida;

                wire [`OUTPUT_WIDTH - 1:0]  db;
                wire                        validb;

                wire [`OUTPUT_WIDTH - 1:0]  q;
                wire                        qvalid;

                //
                if (PRIORITY == `PRIORITY_LOWER_FIRST) begin

                    macro_pick1_prioritylf_bin_unit #(
                        .OUTPUT_WIDTH(`OUTPUT_WIDTH)
                    ) macro_pick1_prioritylf_bin_unit_INST (
                        .da     (da),
                        .valida (valida),

                        .db     (db),
                        .validb (validb),

                        .q      (q),
                        .qvalid (qvalid)
                    );
                end
                else if (PRIORITY == `PRIORITY_HIGHER_FIRST) begin
                    
                    macro_pick1_priorityhf_bin_unit #(
                        .OUTPUT_WIDTH(`OUTPUT_WIDTH)
                    ) macro_pick1_priorityhf_bin_unit_INST (
                        .da     (da),
                        .valida (valida),

                        .db     (db),
                        .validb (validb),

                        .q      (q),
                        .qvalid (qvalid)
                    );
                end

                //
                assign da       = d_pick_tree_addr[((1 << (i + 1)) - 1) + j * 2];
                assign db       = d_pick_tree_addr[((1 << (i + 1)) - 1) + j * 2 + 1];

                assign valida   = d_pick_tree_valid[((1 << (i + 1)) - 1) + j * 2];
                assign validb   = d_pick_tree_valid[((1 << (i + 1)) - 1) + j * 2 + 1];

                //
                assign d_pick_tree_addr[((1 << i) - 1) + j]     = q;
                assign d_pick_tree_valid[((1 << i) - 1) + j]    = qvalid;
            end
        end
    endgenerate

    //
    assign qaddr    = d_pick_tree_addr[0];
    assign qvalid   = d_pick_tree_valid[0];

    //

endmodule

//
module macro_pick1_prioritylf_bin_unit #(
    parameter               OUTPUT_WIDTH = 1
) (
    //
    input   wire [OUTPUT_WIDTH - 1:0]   da,
    input   wire                        valida,

    //
    input   wire [OUTPUT_WIDTH - 1:0]   db,
    input   wire                        validb,

    //
    output  wire [OUTPUT_WIDTH - 1:0]   q,
    output  wire                        qvalid
);
    
    assign q = valida ? da : db;

    assign qvalid = valida | validb;

endmodule

//
module macro_pick1_priorityhf_bin_unit #(
    parameter               OUTPUT_WIDTH = 1
) (
    //
    input   wire [OUTPUT_WIDTH - 1:0]   da,
    input   wire                        valida,

    //
    input   wire [OUTPUT_WIDTH - 1:0]   db,
    input   wire                        validb,

    //
    output  wire [OUTPUT_WIDTH - 1:0]   q,
    output  wire                        qvalid
);

    assign q = validb ? db : da;

    assign qvalid = valida | validb;

endmodule
