// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Between-Pipeline Bypass Buffer Module
//
// @author Kumonda221
//

module common_bypass_buffer #(
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

    stdmacro_dffe  #(
        .DFF_WIDTH(1)
    ) stdmacro_dffe_INST_buffer_valid (
        .clk(clk),
        .reset(reset),

        .en(valid_en),
        .d(valid_d),
        .q(valid_q)
    );

    //
    assign buffer_d  = prev_i_data;

    // Buffer write condition:
    assign buffer_en = prev_i_valid & ~next_i_ready;

    // Valid flag transition:
    assign valid_d   = ~valid_q ? ( prev_i_valid & ~next_i_ready ? 1'b1 : 1'b0)
                                : (                 next_i_ready ? 1'b0 : 1'b1);

    //
    assign valid_en  = 'b1;

    //
    assign prev_o_ready = ~valid_q;

    // Output logic
    assign next_o_data  = valid_q ? buffer_q : prev_i_data;
    assign next_o_valid = valid_q | prev_i_valid;

    //

endmodule
