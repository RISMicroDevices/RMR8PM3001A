// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  RTL ROM Module for 5-bit unsigned decrement
//
// @author Kumonda221
//

module common_rtlrom_decr5 (
    input   wire [4:0]          d,
    output  wire [4:0]          q,
    output  wire                c
);

    reg  [5:0]      r;

    assign q = r[4:0];
    assign c = r[5];

    always @(*) begin
        
        //r = $unsigned(d) - 1;

        case (d)

            5'd00:      r = 6'd63; // { 1'b1, 5'd31 }
            5'd01:      r = 6'd00;
            5'd02:      r = 6'd01;
            5'd03:      r = 6'd02;
            5'd04:      r = 6'd03;
            5'd05:      r = 6'd04;
            5'd06:      r = 6'd05;
            5'd07:      r = 6'd06;
            5'd08:      r = 6'd07;
            5'd09:      r = 6'd08;
            5'd10:      r = 6'd09;
            5'd11:      r = 6'd10;
            5'd12:      r = 6'd11;
            5'd13:      r = 6'd12;
            5'd14:      r = 6'd13;
            5'd15:      r = 6'd14;
            5'd16:      r = 6'd15;
            5'd17:      r = 6'd16;
            5'd18:      r = 6'd17;
            5'd19:      r = 6'd18;
            5'd20:      r = 6'd19;
            5'd21:      r = 6'd20;
            5'd22:      r = 6'd21;
            5'd23:      r = 6'd22;
            5'd24:      r = 6'd23;
            5'd25:      r = 6'd24;
            5'd26:      r = 6'd25;
            5'd27:      r = 6'd26;
            5'd28:      r = 6'd27;
            5'd29:      r = 6'd28;
            5'd30:      r = 6'd29;
            5'd31:      r = 6'd30;

            default:    r = 6'd00;
        endcase
    end

endmodule
