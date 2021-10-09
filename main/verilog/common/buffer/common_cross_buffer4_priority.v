// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  In-Pipeline 4-to-1 Priority Cross Buffer Module
//  (4-to-1 buffer with simple arbiter, priority)
//
//    Priority: Port 0 > Port 1 > Port 2 > Port 3
//
// @author Kumonda221
//

module common_cross_buffer4_priority #(
    parameter   BUFFER_WIDTH            = 1
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
    input   wire [BUFFER_WIDTH - 1:0]   prev2_i_data,
    input   wire                        prev2_i_valid,
    output  wire                        prev2_o_ready,

    //
    input   wire [BUFFER_WIDTH - 1:0]   prev3_i_data,
    input   wire                        prev3_i_valid,
    output  wire                        prev3_o_ready,

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
        .clk            (clk),
        .reset          (reset),
        
        .prev_i_data    (prev0_i_data),
        .prev_i_valid   (prev0_i_valid),
        .prev_o_ready   (prev0_o_ready),

        .next_o_data    (next0_o_data),
        .next_o_valid   (next0_o_valid),
        .next_i_ready   (next0_i_ready)
    );

    // bypass buffer on port 1
    wire [BUFFER_WIDTH - 1:0]   next1_o_data;
    wire                        next1_o_valid;
    wire                        next1_i_ready;

    common_bypass_buffer #(
        .BUFFER_WIDTH(BUFFER_WIDTH)
    ) common_bypass_buffer_p1_INST (
        .clk            (clk),
        .reset          (reset),
        
        .prev_i_data    (prev1_i_data),
        .prev_i_valid   (prev1_i_valid),
        .prev_o_ready   (prev1_o_ready),

        .next_o_data    (next1_o_data),
        .next_o_valid   (next1_o_valid),
        .next_i_ready   (next1_i_ready)
    );

    // bypass buffer on port 2
    wire [BUFFER_WIDTH - 1:0]   next2_o_data;
    wire                        next2_o_valid;
    wire                        next2_i_ready;

    common_bypass_buffer #(
        .BUFFER_WIDTH(BUFFER_WIDTH)
    ) common_bypass_buffer_p2_INST (
        .clk            (clk),
        .reset          (reset),
        
        .prev_i_data    (prev2_i_data),
        .prev_i_valid   (prev2_i_valid),
        .prev_o_ready   (prev2_o_ready),

        .next_o_data    (next2_o_data),
        .next_o_valid   (next2_o_valid),
        .next_i_ready   (next2_i_ready)
    );

    // bypass buffer on port 3
    wire [BUFFER_WIDTH - 1:0]   next3_o_data;
    wire                        next3_o_valid;
    wire                        next3_i_ready;

    common_bypass_buffer #(
        .BUFFER_WIDTH(BUFFER_WIDTH)
    ) common_bypass_buffer_p3_INST (
        .clk            (clk),
        .reset          (reset),
        
        .prev_i_data    (prev3_i_data),
        .prev_i_valid   (prev3_i_valid),
        .prev_o_ready   (prev3_o_ready),

        .next_o_data    (next3_o_data),
        .next_o_valid   (next3_o_valid),
        .next_i_ready   (next3_i_ready)
    );

    //
    wire        p0;
    wire        p1;
    wire        p2;
    wire        p3;

    wire        pn;

    assign p0 = next0_o_valid;
    assign p1 = next1_o_valid;
    assign p2 = next2_o_valid;
    assign p3 = next3_o_valid;

    assign pn = next_i_ready;

    // priority selector
    wire        pick0;
    wire        pick1;
    wire        pick2;
    wire        pick3;

    assign pick0 = ( p0                  ) & pn;
    assign pick1 = (~p0 &  p1            ) & pn;
    assign pick2 = (~p0 & ~p1 &  p2      ) & pn;
    assign pick3 = (~p0 & ~p1 & ~p2 &  p3) & pn;

    //
    assign next0_i_ready = pick0;
    assign next1_i_ready = pick1;
    assign next2_i_ready = pick2;
    assign next3_i_ready = pick3;

    //
    assign next_o_data  = { {(BUFFER_WIDTH){pick0}} & next0_o_data }
                        | { {(BUFFER_WIDTH){pick1}} & next1_o_data }
                        | { {(BUFFER_WIDTH){pick2}} & next2_o_data }
                        | { {(BUFFER_WIDTH){pick3}} & next3_o_data };

    assign next_o_valid = next0_o_valid | next1_o_valid | next2_o_valid | next3_o_valid;


endmodule
