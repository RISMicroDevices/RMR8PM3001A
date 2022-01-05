
//

module sim_dffcam_1qa (
    input   wire            clk,
    input   wire            resetn,

    input   wire [4:0]      addr,
    input   wire            en,
    input   wire            we,

    input   wire [31:0]     din,
    input   wire            din_valid,

    output  wire [31:0]     dout,
    output  wire            dout_valid,

    input   wire [31:0]     qdata,
    output  wire [4:0]      qaddr,
    output  wire            qvalid
);

    common_dffcam_1a1w1r1qa #(
        .CAM_WIDTH              (32),
        .CAM_DEPTH              (32),

        .CAM_ADDRESS_ONEHOT     (0),
        .QUERY_ADDRESS_ONEHOT   (0)
    ) common_dffcam_1a1w1r1qa_INST_SIM (
        .clk        (clk),
        .reset      (resetn),

        .addr       (addr),
        .en         (en),
        .we         (we),
        .din        (din),
        .din_valid  (din_valid),

        .dout       (dout),
        .dout_valid (dout_valid),

        .qdata      (qdata),
        .qaddr      (qaddr),
        .qvalid     (qvalid)
    );

endmodule
