// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Synchronous RAM-base FIFO with 1 write port, 1 read port
//  (Write Enable and data from 'din' always ignored when FIFO was full)
//
// @author Kumonda221
//

`define     FIFO_DEPTH          (1 << FIFO_DEPTH_LOG2)

module common_fifo_ram_1w1r #(
    parameter                                   FIFO_DEPTH_LOG2     = 1,
    parameter                                   FIFO_WIDTH          = 1,
    parameter                                   FIFO_RESET_STATE    = 0,
    parameter [`FIFO_DEPTH * FIFO_WIDTH - 1:0]  FIFO_RESET_VALUE    = { (`FIFO_DEPTH){ {(FIFO_WIDTH){1'b0}} } }
) (
    input   wire                        clk,
    input   wire                        reset,

    input   wire [FIFO_WIDTH - 1:0]     din,
    input   wire                        wen,

    output  wire [FIFO_WIDTH - 1:0]     dout,
    input   wire                        ren,

    output  wire                        fifo_empty,
    output  wire                        fifo_full
);

    // FIFO state signals 
    wire    s_overlap;

    wire    s_empty;
    wire    s_full;
    //    

    // 
    wire    r_pop;   // functionally accepted reading from FIFO
    wire    r_push;  // functionally accepted writing into FIFO

    wire    p_hold; // read and write FIFO simultaneously

    wire    p_pop;  // read FIFO only
    wire    p_push; // write FIFO only

    assign r_pop  = ren & ~s_empty;
    assign r_push = wen & ~s_full;

    assign p_hold = r_pop & wen;

    assign p_pop  = ~p_hold & r_pop;
    assign p_push = ~p_hold & r_push;
    //


    // FIFO write pointer
    wire [FIFO_DEPTH_LOG2:0]    wptr_q;
    wire [FIFO_DEPTH_LOG2:0]    wptr_d;

    wire                        wptr_en;

    stdmacro_dffe #(
        .DFF_WIDTH          (FIFO_DEPTH_LOG2 + 1),
        .DFF_RESET_VALUE    (FIFO_RESET_STATE)
    ) stdmacro_dffe_INST_fifo_wptr (
        .clk    (clk),
        .reset  (reset),
        
        .en     (wptr_en),
        .d      (wptr_d),
        
        .q      (wptr_q)
    );

    assign wptr_d  = $unsigned(wptr_q) + 1;

    assign wptr_en = p_push;
    //

    // FIFO read pointer
    wire [FIFO_DEPTH_LOG2:0]    rptr_q;
    wire [FIFO_DEPTH_LOG2:0]    rptr_d;

    wire                        rptr_en;
    
    stdmacro_dffe #(
        .DFF_WIDTH          (FIFO_DEPTH_LOG2 + 1),
        .DFF_RESET_VALUE    ('b0)
    ) stdmacro_dffe_INST_fifo_rptr (
        .clk    (clk),
        .reset  (reset),
        
        .en     (rptr_en),
        .d      (rptr_d),

        .q      (rptr_q)
    );

    assign rptr_d  = $unsigned(rptr_q) + 1;

    assign rptr_en = p_pop;
    //

    // FIFO state connections to W/R pointers
    assign s_overlap = wptr_q[FIFO_DEPTH_LOG2 - 1:0] == rptr_q[FIFO_DEPTH_LOG2 - 1:0];

    assign s_empty   = s_overlap && (wptr_q[FIFO_DEPTH_LOG2] == rptr_q[FIFO_DEPTH_LOG2]);
    assign s_full    = s_overlap && (wptr_q[FIFO_DEPTH_LOG2] != rptr_q[FIFO_DEPTH_LOG2]);
    //

    // FIFO RAM
    wire    ram_wen;
    wire    ram_ren;

    wire    ram_ena;
    wire    ram_wea;

    wire    ram_enb;

    common_dffram_2a1w1r #(
        .RAM_DATA_WIDTH     (FIFO_WIDTH),
        .RAM_ADDR_WIDTH     (FIFO_DEPTH_LOG2),
        .RAM_RESET_VALUE    (FIFO_RESET_VALUE)
    ) common_dffram_2a1w1r_INST_fifo_ram (
        .clk    (clk),
        .reset  (reset),

        .addra  (wptr_q[FIFO_DEPTH_LOG2 - 1:0]),
        .ena    (ram_ena),
        .wea    (ram_wea),
        .dina   (din),

        .addrb  (rptr_q[FIFO_DEPTH_LOG2 - 1:0]),
        .enb    (ram_enb),
        .doutb  (dout)
    );

    assign ram_wen = p_hold | p_push; //
    assign ram_ren = ren;

    assign ram_ena = ram_wen;
    assign ram_wea = ram_wen;

    assign ram_enb = ram_ren;
    //

    //
    assign fifo_empty = s_empty;
    assign fifo_full  = s_full;

    //

endmodule
