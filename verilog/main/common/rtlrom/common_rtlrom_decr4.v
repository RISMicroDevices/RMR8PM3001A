// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  RTL ROM Module for 4-bit unsigned decrement
//
// @author Kumonda221
//

module common_rtlrom_decr4 (
    input   wire [3:0]          d,
    output  wire [3:0]          q,
    output  wire                c
);  

    reg  [4:0]      r;

    assign q = r[3:0];
    assign c = r[4];
    
    always @(*) begin
        
        // r = $unsigned(d) - 1;
        
        case (d)

            4'd00:      r = 5'd31; // { 1'b1, 4'd15 }
            4'd01:      r = 5'd00;
            4'd02:      r = 5'd01;
            4'd03:      r = 5'd02;
            4'd04:      r = 5'd03;
            4'd05:      r = 5'd04;
            4'd06:      r = 5'd05;
            4'd07:      r = 5'd06;
            4'd08:      r = 5'd07;
            4'd09:      r = 5'd08;
            4'd10:      r = 5'd09;
            4'd11:      r = 5'd10;
            4'd12:      r = 5'd11;
            4'd13:      r = 5'd12;
            4'd14:      r = 5'd13;
            4'd15:      r = 5'd14;

            default:    r = 5'd00;
        endcase
    end

endmodule
