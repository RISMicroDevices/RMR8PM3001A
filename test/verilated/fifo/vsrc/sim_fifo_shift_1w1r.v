
//

module sim_fifo_shift_1w1r (
    input   wire            clk,
    input   wire            resetn,

    input   wire [5:0]      din,
    input   wire            wen,

    output  wire [5:0]      dout,
    input   wire            ren,

    output  wire            fifo_empty,
    output  wire            fifo_full
);

    common_fifo_shift_1w1r #(
        .FIFO_DEPTH         (16),
        .FIFO_WIDTH         (6),
        .FIFO_RESET_STATE   (5'd16),
        .FIFO_RESET_VALUE   (
            { 6'd00, 6'd01, 6'd02, 6'd03, 6'd04, 6'd05, 6'd06, 6'd07,
              6'd08, 6'd09, 6'd10, 6'd11, 6'd12, 6'd13, 6'd14, 6'd15 } )
    ) common_fifo_shift_1w1r_INST_SIM (
        .clk        (clk),
        .reset      (resetn),
        
        .din        (din),
        .wen        (wen),

        .dout       (dout),
        .ren        (ren),

        .fifo_empty (fifo_empty),
        .fifo_full  (fifo_full)
    );

endmodule
