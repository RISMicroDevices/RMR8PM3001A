// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  In-Pipeline 2-to-1 Priority Cross Buffer Module
//  (2-to-1 buffer with simple arbiter, port 0 always has highest priority)
//
// @author Kumonda221
//

module common_cross_buffer2_priority #(
    parameter   BUFFER_WIDTH    = 1
) (
    //
    input   wire                        clk,
    input   wire                        reset,

    //
    input   wire [BUFFER_WIDTH - 1:0]   prev0_i_data,
    input   wire                        prev0_i_valid,
    output  wire                        prev0_o_ready,

    //
    input   wire [BUFFER_WIDTH - 1:0]   prev1_i_data,
    input   wire                        prev1_i_valid,
    output  wire                        prev1_o_ready,

    //
    output  wire [BUFFER_WIDTH - 1:0]   next_o_data,
    output  wire                        next_o_valid,
    input   wire                        next_i_ready
);

    // bypass buffer on port 0
    wire [BUFFER_WIDTH - 1:0]   next0_o_data;
    wire                        next0_o_valid;
    wire                        next0_i_ready;

    common_bypass_buffer #(
        .BUFFER_WIDTH(BUFFER_WIDTH)
    ) common_bypass_buffer_p0_INST (
        .clk(clk),
        .reset(reset),
        
        .prev_i_data(prev0_i_data),
        .prev_i_valid(prev0_i_valid),
        .prev_o_ready(prev0_o_ready),

        .next_o_data(next0_o_data),
        .next_o_valid(next0_o_valid),
        .next_i_ready(next0_i_ready)
    );

    // bypass buffer on port 1
    wire [BUFFER_WIDTH - 1:0]   next1_o_data;
    wire                        next1_o_valid;
    wire                        next1_i_ready;

    common_bypass_buffer #(
        .BUFFER_WIDTH(BUFFER_WIDTH)
    ) common_bypass_buffer_p1_INST (
        .clk(clk),
        .reset(reset),
        
        .prev_i_data(prev1_i_data),
        .prev_i_valid(prev1_i_valid),
        .prev_o_ready(prev1_o_ready),

        .next_o_data(next1_o_data),
        .next_o_valid(next1_o_valid),
        .next_i_ready(next1_i_ready)
    );

    // priority arbiter
    wire        compete;

    assign compete = next0_o_valid & next1_o_valid;

    assign next0_i_ready = next_i_ready & (compete ? 1'b1 : next0_o_valid);
    assign next1_i_ready = next_i_ready & (compete ? 1'b0 : next1_o_valid);

    assign next_o_data   = next0_i_ready ? next0_o_data : next1_o_data;
    assign next_o_valid  = next0_o_valid | next1_o_valid; 
    //

endmodule
