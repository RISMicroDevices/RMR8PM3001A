// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Between-Pipeline Flushable Bypass Buffer Module
//  ('ready' always asserted when buffer empty)
//
// @author Kumonda221
//

module common_bypass_bufferf #(
    parameter   BUFFER_WIDTH    = 1
) (
    // 
    input   wire                        clk,
    input   wire                        reset,
    input   wire                        flush,

    input   wire [BUFFER_WIDTH - 1:0]   prev_i_data,
    input   wire                        prev_i_valid,
    output  wire                        prev_o_ready,

    output  wire [BUFFER_WIDTH - 1:0]   next_o_data,
    output  wire                        next_o_valid,
    input   wire                        next_i_ready
);

    //
    wire    flush_d;
    wire    flush_q;

    stdmacro_dff #(
        .DFF_WIDTH(1)
    ) stdmacro_dff_INST_flush (
        .clk    (clk),
        .reset  (reset),

        .d      (flush_d),
        .q      (flush_q)
    );

    //
    wire                        buffer_en;
    wire [BUFFER_WIDTH - 1:0]   buffer_d;
    wire [BUFFER_WIDTH - 1:0]   buffer_q;

    stdmacro_dffe #(
        .DFF_WIDTH(BUFFER_WIDTH)
    ) stdmacro_dffe_INST_buffer (
        .clk    (clk),
        .reset  (reset),

        .en     (buffer_en),
        .d      (buffer_d),
        .q      (buffer_q)
    );

    //
    wire    valid_d;
    wire    valid_q;

    stdmacro_dff  #(
        .DFF_WIDTH(1)
    ) stdmacro_dff_INST_buffer_valid (
        .clk    (clk),
        .reset  (reset),

        .d      (valid_d),
        .q      (valid_q)
    );

    //
    assign flush_d   = flush;

    //
    assign buffer_d  = prev_i_data;

    // Buffer write condition:
    assign buffer_en = ~valid_q ? ( prev_i_valid & ~next_i_ready )
                                : 1'b0;

    // Valid flag transition:
    assign valid_d   =  flush   ? 1'b0 
                     : ~valid_q ? ( prev_i_valid & ~next_i_ready )
                                : (                ~next_i_ready );

    //
    assign prev_o_ready = ~valid_q & ~flush_q;

    // Output logic
    assign next_o_data  =  valid_q ? buffer_q : prev_i_data;
    assign next_o_valid = (valid_q | prev_i_valid) & ~flush_q;

    //

endmodule
