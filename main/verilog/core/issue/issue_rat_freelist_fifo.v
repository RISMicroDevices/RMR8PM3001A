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

        .din        (bank_1_din),
        .wen        (bank_1_wen),

        .dout       (bank_1_dout),
        .ren        (bank_1_ren),

        .fifo_empty (bank_1_empty),
        .fifo_full  (bank_1_full)
    );

    // bank read LRU register
    wire    bank_lru_en;
    wire    bank_lru_d;

    wire    bank_lru_q;

    stdmacro_dffe #(
        .DFF_WIDTH          (1),
        .DFF_RESET_VALUE    (0)
    ) stdmacro_dffe_INST_bank_lru (
        .clk    (clk),
        .reset  (reset),

        .en     (bank_lru_en),
        .d      (bank_lru_d),

        .q      (bank_lru_q)
    );


    //
    wire [1:0]  redeem_prepick;
    wire [1:0]  abandon_direct_prepick;
    wire [1:0]  abandon_buffer_prepick;

    assign redeem_prepick[0] = i_redeemed_valid & ~i_redeemed_prf[0]; // one-hot or none
    assign redeem_prepick[1] = i_redeemed_valid &  i_redeemed_prf[0];

    assign abandon_direct_prepick[0] = i_abandoned_valid & ~i_abandoned_prf[0];
    assign abandon_direct_prepick[1] = i_abandoned_valid &  i_abandoned_prf[0];

    assign abandon_buffer_prepick[0] = abandoned_buffer_valid_q & ~abandoned_buffer_q[0];
    assign abandon_buffer_prepick[1] = abandoned_buffer_valid_q &  abandoned_buffer_q[0];

    //
    wire    congested_abandon;

    wire    accepted_L0_redeem              , accepted_L1_redeem;
    wire    accepted_L0_abandon_buffer      , accepted_L1_abandon_buffer;
    wire    accepted_L0_abandon_direct      , accepted_L1_abandon_direct;

    wire    accepted_L0_p0                  , accepted_L1_p0;
    wire    accepted_L0_p1                  , accepted_L1_p1;

    wire    accepted_L0_p0_redeem           , accepted_L1_p0_redeem;
    wire    accepted_L0_p0_abandon_direct   , accepted_L1_p0_abandon_direct;
    wire    accepted_L0_p0_abandon_buffer   , accepted_L1_p0_abandon_buffer;
    wire    accepted_L0_p1_redeem           , accepted_L1_p1_redeem;
    wire    accepted_L0_p1_abandon_direct   , accepted_L1_p1_abandon_direct;
    wire    accepted_L0_p1_abandon_buffer   , accepted_L1_p1_abandon_buffer;

    wire [5:0]  accepted_prf_L0_p0          , accepted_prf_L1_p0;
    wire [5:0]  accepted_prf_L0_p1          , accepted_prf_L1_p1;

    wire [9:0]  picked_redeem;
    wire [9:0]  picked_abandon_direct;
    wire [9:0]  picked_abandon_buffer;
    wire [9:0]  picked_none;

    assign congested_abandon  = (~accepted_L0_abandon_direct & i_abandoned_valid)
                              & (~accepted_L0_abandon_buffer & abandoned_buffer_valid_q);

    //                               WEN , redeem, direct, buffer, PRF
    assign picked_redeem         = { 1'b1, 1'b1  , 1'b0  , 1'b0  , i_redeemed_prf     };
    assign picked_abandon_direct = { 1'b1, 1'b0  , 1'b1  , 1'b0  , i_abandoned_prf    };
    assign picked_abandon_buffer = { 1'b1, 1'b0  , 1'b0  , 1'b1  , abandoned_buffer_q };
    assign picked_none           = { 1'b0, 1'b0  , 1'b0  , 1'b0  , 6'b0               };

    // L0 FIFO MUXs
    assign { accepted_L0_p0, accepted_L0_p0_redeem, accepted_L0_p0_abandon_direct, accepted_L0_p0_abandon_buffer, accepted_prf_L0_p0 }
        //
        = bank_0_full ?                 picked_none
        : bank_1_full ? (  // Forced write MUX enabled when another FIFO was full
            i_redeemed_valid         ?  picked_redeem
          : i_abandoned_valid        ?  picked_abandon_direct
          : abandoned_buffer_valid_q ?  picked_abandon_buffer
          :                             picked_none
        )
        : redeem_prepick[0]         ?   picked_redeem
        : abandon_direct_prepick[0] ?   picked_abandon_direct
        : abandon_buffer_prepick[0] ?   picked_abandon_buffer
        :                               picked_none;

    assign { accepted_L0_p1, accepted_L0_p1_redeem, accepted_L0_p1_abandon_direct, accepted_L0_p1_abandon_buffer, accepted_prf_L0_p1 }
        //
        = bank_1_full ?                 picked_none
        : bank_0_full ? (
            i_redeemed_valid         ?  picked_redeem
          : i_abandoned_valid        ?  picked_abandon_direct
          : abandoned_buffer_valid_q ?  picked_abandon_buffer
          :                             picked_none
        )
        : redeem_prepick[1]         ?   picked_redeem
        : abandon_direct_prepick[1] ?   picked_abandon_direct
        : abandon_buffer_prepick[1] ?   picked_abandon_buffer
        :                               picked_none;

    // L1 FIFO MUXs for Congestion Redirection (abandon direct first)
    assign { accepted_L1_p0, accepted_L1_p0_redeem, accepted_L1_p0_abandon_direct, accepted_L1_p0_abandon_buffer, accepted_prf_L1_p0 }
        //
        = (~accepted_L0_p0 & ~bank_0_full & congested_abandon) ? picked_abandon_direct
        : { accepted_L0_p0, accepted_L0_p0_redeem, accepted_L0_p0_abandon_direct, accepted_L0_p0_abandon_buffer, accepted_prf_L0_p0 };

    assign { accepted_L1_p1, accepted_L1_p1_redeem, accepted_L1_p1_abandon_direct, accepted_L1_p1_abandon_buffer, accepted_prf_L1_p1 }
        //
        = (~accepted_L0_p1 & ~bank_1_full & congested_abandon) ? picked_abandon_direct
        : { accepted_L0_p1, accepted_L0_p1_redeem, accepted_L0_p1_abandon_direct, accepted_L0_p1_abandon_buffer, accepted_prf_L0_p1 };

    //
    assign accepted_L0_redeem         = accepted_L0_p0_redeem         | accepted_L0_p1_redeem;
    assign accepted_L0_abandon_direct = accepted_L0_p0_abandon_direct | accepted_L0_p1_abandon_direct;
    assign accepted_L0_abandon_buffer = accepted_L0_p0_abandon_buffer | accepted_L0_p1_abandon_buffer;

    assign accepted_L1_redeem         = accepted_L1_p0_redeem         | accepted_L1_p1_redeem;
    assign accepted_L1_abandon_direct = accepted_L1_p0_abandon_direct | accepted_L1_p1_abandon_direct;
    assign accepted_L1_abandon_buffer = accepted_L1_p0_abandon_buffer | accepted_L1_p1_abandon_buffer;


    // FIFO write control
    assign bank_0_din = accepted_prf_L1_p0;
    assign bank_0_wen = accepted_L1_p0;

    assign bank_1_din = accepted_prf_L1_p1;
    assign bank_1_wen = accepted_L1_p1;

    // abandon buffer/direct control
    wire    abandoned_buffer_writable;

    assign abandoned_buffer_writable = (~abandoned_buffer_valid_q | accepted_L1_abandon_buffer) & ~(bank_0_full & bank_1_full);

    assign abandoned_buffer_d   = i_abandoned_prf;
    assign abandoned_buffer_en  = i_abandoned_valid & ~accepted_L1_abandon_direct & abandoned_buffer_writable;

    assign abandoned_buffer_valid_d     = abandoned_buffer_en;
    assign abandoned_buffer_valid_en    = abandoned_buffer_en | accepted_L1_abandon_buffer;

    // Input control
    assign o_redeemed_ready     = accepted_L1_redeem;
    assign o_abandoned_ready    = accepted_L1_abandon_direct | abandoned_buffer_en;

    // FIFO read MUX
    wire        acquire_L0_valid     ,      acquire_L1_valid;
    wire        acquire_L0_p0_picked ,      acquire_L1_p0_picked;
    wire        acquire_L0_p1_picked ,      acquire_L1_p1_picked;

    wire [5:0]  acquire_prf_L0       ,      acquire_prf_L1;

    wire [8:0]  acquire_picked_p0;
    wire [8:0]  acquire_picked_p1;
    wire [8:0]  acquire_picked_none;

    assign acquire_picked_p0    = { 1'b1, 1'b0, ~bank_0_empty, bank_0_dout };
    assign acquire_picked_p1    = { 1'b0, 1'b1, ~bank_1_empty, bank_1_dout };
    assign acquire_picked_none  = { 1'b0, 1'b0,  1'b0        , 6'b0        };

    assign { acquire_L0_p0_picked, acquire_L0_p1_picked, acquire_L0_valid, acquire_prf_L0 }
        //
        = ~bank_lru_q ? acquire_picked_p0
        :               acquire_picked_p1;

    assign { acquire_L1_p0_picked, acquire_L1_p1_picked, acquire_L1_valid, acquire_prf_L1 }
        //
        =  acquire_L0_valid ? { acquire_L0_p0_picked, acquire_L0_p1_picked, acquire_L0_valid, acquire_prf_L0 }
        : ~bank_0_empty     ? acquire_picked_p0
        : ~bank_1_empty     ? acquire_picked_p1
        :                     acquire_picked_none;

    // FIFO read control
    assign bank_lru_en  = i_acquire_ready & (acquire_L1_p0_picked | acquire_L1_p1_picked);
    assign bank_lru_d   = i_acquire_ready & (acquire_L1_p0_picked);

    assign bank_0_ren   = i_acquire_ready & acquire_L1_p0_picked;
    assign bank_1_ren   = i_acquire_ready & acquire_L1_p1_picked;

    assign o_acquire_prf    = acquire_prf_L1;
    assign o_acquire_valid  = acquire_L1_valid;
    //

endmodule
