
//

module sim_rtlrom_decinc (
    input   wire [4:0]          d,
    input   wire                dec,

    output  wire [1:0]          q_incr2,
    output  wire                c_incr2,

    output  wire [1:0]          q_decr2,
    output  wire                c_decr2,

    output  wire [1:0]          q_decinc2,
    output  wire                c_decinc2,

    output  wire [2:0]          q_incr3,
    output  wire                c_incr3,

    output  wire [2:0]          q_decr3,
    output  wire                c_decr3,

    output  wire [2:0]          q_decinc3,
    output  wire                c_decinc3,

    output  wire [3:0]          q_incr4,
    output  wire                c_incr4,

    output  wire [3:0]          q_decr4,
    output  wire                c_decr4,

    output  wire [3:0]          q_decinc4,
    output  wire                c_decinc4,

    output  wire [4:0]          q_incr5,
    output  wire                c_incr5,

    output  wire [4:0]          q_decr5,
    output  wire                c_decr5,

    output  wire [4:0]          q_decinc5,
    output  wire                c_decinc5
);

    // INCR-DECR-2
    common_rtlrom_incr2 common_rtlrom_incr2_INST_SIM (
        .d(d[1:0]),
        .q(q_incr2),
        .c(c_incr2)
    );
    
    common_rtlrom_decr2 common_rtlrom_decr2_INST_SIM (
        .d(d[1:0]),
        .q(q_decr2),
        .c(c_decr2)
    );

    common_rtlrom_decinc2 common_rtlrom_decinc2_INST_SIM (
        .d(d[1:0]),
        .dec(dec),
        .q(q_decinc2),
        .c(c_decinc2)
    );

    // INCR-DECR-3
    common_rtlrom_incr3 common_rtlrom_incr3_INST_SIM (
        .d(d[2:0]),
        .q(q_incr3),
        .c(c_incr3)
    );
    
    common_rtlrom_decr3 common_rtlrom_decr3_INST_SIM (
        .d(d[2:0]),
        .q(q_decr3),
        .c(c_decr3)
    );

    common_rtlrom_decinc3 common_rtlrom_decinc3_INST_SIM (
        .d(d[2:0]),
        .dec(dec),
        .q(q_decinc3),
        .c(c_decinc3)
    );

    // INCR-DECR-4
    common_rtlrom_incr4 common_rtlrom_incr4_INST_SIM (
        .d(d[3:0]),
        .q(q_incr4),
        .c(c_incr4)
    );
    
    common_rtlrom_decr4 common_rtlrom_decr4_INST_SIM (
        .d(d[3:0]),
        .q(q_decr4),
        .c(c_decr4)
    );

    common_rtlrom_decinc4 common_rtlrom_decinc4_INST_SIM (
        .d(d[3:0]),
        .dec(dec),
        .q(q_decinc4),
        .c(c_decinc4)
    );

    // INCR-DECR-5
    common_rtlrom_incr5 common_rtlrom_incr5_INST_SIM (
        .d(d),
        .q(q_incr5),
        .c(c_incr5)
    );

    common_rtlrom_decr5 common_rtlrom_decr5_INST_SIM (
        .d(d),
        .q(q_decr5),
        .c(c_decr5)
    );

    common_rtlrom_decinc5 common_rtlrom_decinc5_INST_SIM (
        .d(d),
        .dec(dec),
        .q(q_decinc5),
        .c(c_decinc5)
    );

endmodule
