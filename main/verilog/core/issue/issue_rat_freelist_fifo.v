// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  PRF Free List 64-entry FIFO
//  (32 entries per-bank)
//
// @author Kumonda221
//

module issue_rat_freelist_fifo (
    input   wire                clk,
    input   wire                reset,

    // 
    input   wire [5:0]          i_abandoned_prf,
    input   wire                i_abandoned_valid,
    output  wire                o_abandoned_ready,

    //
    input   wire [5:0]          i_redeemed_prf,
    input   wire                i_redeemed_valid,
    output  wire                o_redeemed_ready,

    //
    output  wire [5:0]          o_acquire_prf,
    output  wire                o_acquire_valid,
    input   wire                i_acquire_ready
);
    //
    wire        abandoned_buffer_en;
    wire [5:0]  abandoned_buffer_d;

    wire [5:0]  abandoned_buffer_q;

    stdmacro_dffe #(
        .DFF_WIDTH  (6)
    ) stdmacro_dffe_INST_abandoned_buffer (
        .clk    (clk),
        .reset  (reset),

        .en     (abandoned_buffer_en),
        .d      (abandoned_buffer_d),

        .q      (abandoned_buffer_q)
    );

    //
    wire    abandoned_buffer_valid_en;
    wire    abandoned_buffer_valid_d;

    wire    abandoned_buffer_valid_q;

    stdmacro_dffe #(
        .DFF_WIDTH          (1),
        .DFF_RESET_VALUE    (0)
    ) stdmacro_dffe_INST_abandoned_buffer_valid (
        .clk    (clk),
        .reset  (reset),

        .en     (abandoned_buffer_valid_en),
        .d      (abandoned_buffer_valid_d),

        .q      (abandoned_buffer_valid_q)
    );

    // bank 0
    wire [5:0]  bank_0_din;
    wire        bank_0_wen;

    wire [5:0]  bank_0_dout;
    wire        bank_0_ren;

    wire        bank_0_empty;
    wire        bank_0_full;

    common_fifo_ram_1w1r #(
        .FIFO_DEPTH_LOG2    (5),
        .FIFO_WIDTH         (6),
        .FIFO_RESET_STATE   (6'd32),
        .FIFO_RESET_VALUE   ({
            6'd00, 6'd02, 6'd04, 6'd06, 6'd08, 6'd10, 6'd12, 6'd14,
            6'd16, 6'd18, 6'd20, 6'd22, 6'd24, 6'd26, 6'd28, 6'd30,
            6'd32, 6'd34, 6'd36, 6'd38, 6'd40, 6'd42, 6'd44, 6'd46,
            6'd48, 6'd50, 6'd52, 6'd54, 6'd56, 6'd58, 6'd60, 6'd62
        })
    ) common_fifo_ram_1w1r_INST_bank_0 (
        .clk        (clk),
        .reset      (reset),

        .din        (bank_0_din),
        .wen        (bank_0_wen),

        .dout       (bank_0_dout),
        .ren        (bank_0_ren),

        .fifo_empty (bank_0_empty),
        .fifo_full  (bank_0_full)
    );

    // bank 1
    wire [5:0]  bank_1_din;
    wire        bank_1_wen;

    wire [5:0]  bank_1_dout;
    wire        bank_1_ren;

    wire        bank_1_empty;
    wire        bank_1_full;

    common_fifo_ram_1w1r #(
        .FIFO_DEPTH_LOG2    (5),
        .FIFO_WIDTH         (6),
        .FIFO_RESET_STATE   (6'd32),
        .FIFO_RESET_VALUE   ({
            6'd01, 6'd03, 6'd05, 6'd07, 6'd09, 6'd11, 6'd13, 6'd15,
            6'd17, 6'd19, 6'd21, 6'd23, 6'd25, 6'd27, 6'd29, 6'd31,
            6'd33, 6'd35, 6'd37, 6'd39, 6'd41, 6'd43, 6'd45, 6'd47,
            6'd49, 6'd51, 6'd53, 6'd55, 6'd57, 6'd59, 6'd61, 6'd63
        })
    ) common_fifo_ram_1w1r_INST_bank_1 (
        .clk        (clk),
        .reset      (reset),

        .din        (bank_0_din),
        .wen        (bank_0_wen),

        .dout       (bank_0_dout),
        .ren        (bank_0_ren),

        .fifo_empty (bank_0_empty),
        .fifo_full  (bank_0_full)
    );

    //
    wire    s_abandon_direct;
    wire    s_abandon_buffer;
    wire    s_abandon_congested;
    wire    s_abandon_idle;

    assign s_abandon_direct     =  i_abandoned_valid & ~abandoned_buffer_valid_q;
    assign s_abandon_buffer     = ~i_abandoned_valid &  abandoned_buffer_valid_q;
    assign s_abandon_congested  =  i_abandoned_valid &  abandoned_buffer_valid_q;
    assign s_abandon_idle       = ~i_abandoned_valid & ~abandoned_buffer_valid_q;

    //
    wire [1:0]  redeem_pick;

    assign redeem_pick[0] = i_redeemed_valid & ~i_redeemed_prf[0]; // one-hot or none
    assign redeem_pick[1] = i_redeemed_valid &  i_redeemed_prf[0];

    //
    wire    r_redeem_p0;
    wire    r_redeem_p1;
    wire    r_redeem_none;

    assign r_redeem_p0      =    redeem_pick[0] /*& ~redeem_pick[1]*/;
    assign r_redeem_p1      = /*~redeem_pick[0] &*/  redeem_pick[1];
    assign r_redeem_none    =   ~redeem_pick[0] &   ~redeem_pick[1];

    //
    wire [5:0]  w_abandoned_prf;

    wire [5:0]  w_prf_p0;
    wire [5:0]  w_prf_p1;

    assign w_abandoned_prf = i_abandoned_valid ? i_abandoned_prf : abandoned_buffer_q;

    assign w_prf_p0 = r_redeem_p0 ? i_redeemed_prf : w_abandoned_prf;
    assign w_prf_p1 = r_redeem_p1 ? i_redeemed_prf : w_abandoned_prf;

    //
    wire    r_en_direct_p0;
    wire    r_en_direct_p1;

    wire    r_en_buffer_p0;
    wire    r_en_buffer_p1;

    wire    r_en_direct;
    wire    r_en_buffer;

    wire    r_en_redeem;

    // TODO

    //
    wire [5:0]  w_en_p0;
    wire [5:0]  w_en_p1;

    assign w_en_p0 = (r_redeem_p0 
                   | (s_abandon_direct    & ~i_abandoned_prf[0])
                   | (s_abandon_buffer    & ~abandoned_buffer_q[0])
                   | (s_abandon_congested & (r_redeem_none ? ~i_abandoned_prf[0] : 1'b1))
                /* | (s_abandon_idle      & 1'b0) */ ) 
                   & ~bank_0_full;

    assign w_en_p1 = (r_redeem_p1
                   | (s_abandon_direct    &  i_abandoned_prf[0])
                   | (s_abandon_buffer    &  abandoned_buffer_q[0])
                   | (s_abandon_congested & (r_redeem_none ?  i_abandoned_prf[0] : 1'b1))
                /* | (s_abandon_idle      & 1'b0) */ )
                   & ~bank_1_full;

    //

endmodule
