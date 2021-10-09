
//

module sim_cross_buffer2 (
    //
    input   wire                    clk,
    input   wire                    resetn,

    //
    input   wire [31:0]             prev0_i_data,
    input   wire                    prev0_i_valid,
    output  wire                    prev0_o_ready,

    //
    input   wire [31:0]             prev1_i_data,
    input   wire                    prev1_i_valid,
    output  wire                    prev1_o_ready,

    //
    output  wire [31:0]             next_o_data,
    output  wire                    next_o_valid,
    input   wire                    next_i_ready
);

    common_cross_buffer2 #(
        .BUFFER_WIDTH(32)
    ) common_cross_buffer2_SIM_INST (
        .clk(clk),
        .reset(resetn),

        .prev0_i_data(prev0_i_data),
        .prev0_i_valid(prev0_i_valid),
        .prev0_o_ready(prev0_o_ready),

        .prev1_i_data(prev1_i_data),
        .prev1_i_valid(prev1_i_valid),
        .prev1_o_ready(prev1_o_ready),

        .next_o_data(next_o_data),
        .next_o_valid(next_o_valid),
        .next_i_ready(next_i_ready)
    );

endmodule
