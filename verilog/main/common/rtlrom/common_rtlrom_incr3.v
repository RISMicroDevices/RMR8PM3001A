// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  RTL ROM Module for 3-bit unsigned increment
//
// @author Kumonda221
//

module common_rtlrom_incr3 (
    input   wire [2:0]          d,
    output  wire [2:0]          q,
    output  wire                c
);

    reg [3:0]   r;

    assign q = r[2:0];
    assign c = r[3];

    always @(*) begin
        
        // r = $unsigned(d) + 1;

        case (d)

            3'd00:      r = 4'd01;
            3'd01:      r = 4'd02;
            3'd02:      r = 4'd03;
            3'd03:      r = 4'd04;
            3'd04:      r = 4'd05;
            3'd05:      r = 4'd06;
            3'd06:      r = 4'd07;
            3'd07:      r = 4'd08; // { 1'b1, 3'd00 }

            default:    r = 4'd00;
        endcase
    end

endmodule
