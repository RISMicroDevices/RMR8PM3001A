// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Between-Pipeline Active Bypass Buffer Module
//  ('ready' always not asserted 1 beat after next level 'ready' un-asserted)
//
// @author Kumonda221
//

module common_bypass_buffer_active #(
    parameter   BUFFER_WIDTH    = 1
) (
    // 
    input   wire                        clk,
    input   wire                        reset,

    input   wire [BUFFER_WIDTH - 1:0]   prev_i_data,
    input   wire                        prev_i_valid,
    output  wire                        prev_o_ready,

    output  wire [BUFFER_WIDTH - 1:0]   next_o_data,
    output  wire                        next_o_valid,
    input   wire                        next_i_ready
);

    //
    wire                        buffer_en;
    wire [BUFFER_WIDTH - 1:0]   buffer_d;
    wire [BUFFER_WIDTH - 1:0]   buffer_q;

    stdmacro_dffe #(
        .DFF_WIDTH(BUFFER_WIDTH)
    ) stdmacro_dffe_INST_buffer (
        .clk(clk),
        .reset(reset),

        .en(buffer_en),
        .d(buffer_d),
        .q(buffer_q)
    );

    //
    wire    valid_en;
    wire    valid_d;
    wire    valid_q;

    wire    valid_t;

    stdmacro_dffe #(
        .DFF_WIDTH(1)
    ) stdmacro_dffe_INST_buffer_valid (
        .clk(clk),
        .reset(reset),

        .en(valid_en),
        .d(valid_d),
        .q(valid_q)
    );

    // 
    wire    ready_d;
    wire    ready_q;

    stdmacro_dff #(
        .DFF_WIDTH(1)
    ) stdmacro_dff_INST_ready (
        .clk(clk),
        .reset(reset),

        .d(ready_d),
        .q(ready_q)
    );

    //
    assign buffer_d  = prev_i_data;

    // Buffer write condition:
    assign buffer_en = ~valid_q ? ( prev_i_valid & ~next_i_ready )
                                : 1'b0;

    // Valid flag transition:
    assign valid_d   = ~valid_q ? ( prev_i_valid & ~next_i_ready )
                                : (                ~next_i_ready );

    //
    assign valid_en  = 'b1;

    //
    assign ready_d = next_i_ready;

    assign prev_o_ready = ready_q;

    // Output logic
    assign next_o_data  = valid_q ? buffer_q : prev_i_data;
    assign next_o_valid = valid_q | prev_i_valid;

    //

endmodule
