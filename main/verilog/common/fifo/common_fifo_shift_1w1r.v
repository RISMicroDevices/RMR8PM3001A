// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Synchronous Shifting FIFO with 1 write port, 1 read port
//  (Write Enable and data from 'din' always ignored when FIFO was full)
//
// @author Kumonda221
//

module common_fifo_shift_1w1r #(
    parameter                                   FIFO_DEPTH          = 1,
    parameter                                   FIFO_WIDTH          = 1,
    parameter                                   FIFO_RESET_STATE    = 0,
    parameter [FIFO_DEPTH * FIFO_WIDTH - 1:0]   FIFO_RESET_VALUE    = { (FIFO_DEPTH){ {(FIFO_WIDTH){1'b0}} } }
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

    //
    wire r_pop;   // functionally accepted reading from FIFO, popped
    wire r_push;  // functionally accepted writing into FIFO, pushed

    wire p_hold;  // read and write FIFO simultaneously

    wire p_pop;   // read FIFO only
    wire p_push;  // write FIFO only

    wire p_mov;   // moving P registers

    assign  r_pop  = ren & ~fifo_empty;
    assign  r_push = wen & ~fifo_full;

    assign  p_hold = r_pop & wen;

    assign  p_pop  = ~p_hold & r_pop;
    assign  p_push = ~p_hold & r_push;

    assign  p_mov  = p_push | p_pop;
    
    //
    wire [FIFO_DEPTH:-1]            fifo_p;

    wire [FIFO_WIDTH - 1:0]         content_dff_din  [FIFO_DEPTH:0];
    wire [FIFO_WIDTH - 1:0]         content_dff_dout [FIFO_DEPTH:-1];

    assign  fifo_p[-1]          = 'b0;

    assign  content_dff_dout[-1] = 'b0;

    // empty flag P register
    stdmacro_dffe #(
        .DFF_WIDTH          (1),
        .DFF_RESET_VALUE    (FIFO_RESET_STATE == 0 ? 1'b1 : 1'b0)
    ) stdmacro_dffe_INST_fifo_empty (
        .clk    (clk),
        .reset  (reset),

        .en     (p_mov),
        .d      (p_pop ? fifo_p[FIFO_DEPTH - 1] : 1'b0),

        .q      (fifo_p[FIFO_DEPTH])
    );

    // content DFFs & P registers
    genvar i;
    generate
        for (i = 0; i < FIFO_DEPTH; i = i + 1) begin :GENERATED_FIFO_DFFS
            
            // content DFFs
            wire    content_dff_en;
            wire    content_dff_dw; // direct write

            stdmacro_dffe #(
                .DFF_WIDTH          (FIFO_WIDTH),
                .DFF_RESET_VALUE    (FIFO_RESET_VALUE[FIFO_WIDTH * (FIFO_DEPTH - i - 1) +: FIFO_WIDTH])
            ) stdmacro_dffe_INST_fifo_content_dffs (
                .clk    (clk),
                .reset  (reset),
                
                .en     (content_dff_en),
                .d      (content_dff_din[i]),
                
                .q      (content_dff_dout[i])
            );

            assign  content_dff_dw      = (p_hold & fifo_p[i])       // direct write on Read-Write
                                        | (p_push & fifo_p[i + 1]);  // direct write on Write-Only
            assign  content_dff_en      =  p_hold | p_pop | content_dff_dw;

            assign  content_dff_din[i]  = content_dff_dw ? din : content_dff_dout[i - 1];
            //

            // P registers 
            stdmacro_dffe #(
                .DFF_WIDTH          (1),
                .DFF_RESET_VALUE    (FIFO_RESET_STATE == (FIFO_DEPTH - i) ? 1'b1 : 1'b0)
            ) stdmacro_dffe_INST_fifo_p_reg (
                .clk    (clk),
                .reset  (reset),

                .en     (p_mov),

                .d      (p_pop ? fifo_p[i - 1] : fifo_p[i + 1]),
                .q      (fifo_p[i])
            );
            //
        end
    endgenerate

    //
    assign  fifo_full  = fifo_p[0];
    assign  fifo_empty = fifo_p[FIFO_DEPTH];

    assign  dout = content_dff_dout[FIFO_DEPTH - 1];

    //

endmodule
