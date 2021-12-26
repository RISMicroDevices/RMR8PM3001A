
//

module sim_dffram_1a #(
    parameter       RAM_WIDTH   = 32,
    parameter       RAM_DEPTH   = 32
) (
    input   wire                clk,
    input   wire                resetn,

    input   wire [4:0]          addr,
    input   wire                en,
    input   wire                we,

    input   wire [31:0]         din,
    output  wire [31:0]         dout,

    output  wire [1023:0]       tdout
);

    common_dffram_1a1w1r #(
        .RAM_WIDTH          (RAM_WIDTH),
        .RAM_DEPTH          (RAM_DEPTH),

        .ONEHOT_ADDRESSING  (0),
        .BIT_WRITE_ENABLE   (0),
        .DATA_COUPLED       (1)
    ) common_dffram_1a1w1r_INST_SIM (
        .clk    (clk),
        .reset  (resetn),

        .addr   (addr),
        .en     (en),
        .we     (we),

        .din    (din),
        .dout   (dout),

        .tdout  (tdout)
    );

endmodule
