
//

module sim_issue_prf (
    input   wire                clk,
    input   wire                resetn,

    //
    input   wire [5:0]          addra,
    input   wire                wea,
    input   wire [63:0]         dina,

    //
    input   wire [5:0]          addrb,
    output  wire [63:0]         doutb,

    //
    input   wire [5:0]          addrc,
    output  wire [63:0]         doutc
);

    issue_prf issue_prf_INST_SIM (
        .clk    (clk),
        .reset  (resetn),

        .addra  (addra),
        .wea    (wea),
        .dina   (dina),

        .addrb  (addrb),
        .doutb  (doutb),

        .addrc  (addrc),
        .doutc  (doutc)
    ); 

endmodule
