// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  RTL ROM Module for 4-bit unsigned increment and decrement
//
// @author Kumonda221
//

module macro_rom_decinc4 (
    input   wire [3:0]          d,
    input   wire                dec,
    output  wire [3:0]          q,
    output  wire                c
);

    reg  [4:0]      r;

    assign q = r[3:0];
    assign c = r[4];

    always @(*) begin

        // r = $unsigned(d) + (dec ? 4'b1111 : 4'b0001);

        case ({ dec, d })

            // INCR-4 region
            { 1'b0, 4'd00 }:    r = 5'd01;
            { 1'b0, 4'd01 }:    r = 5'd02;
            { 1'b0, 4'd02 }:    r = 5'd03;
            { 1'b0, 4'd03 }:    r = 5'd04;
            { 1'b0, 4'd04 }:    r = 5'd05;
            { 1'b0, 4'd05 }:    r = 5'd06;
            { 1'b0, 4'd06 }:    r = 5'd07;
            { 1'b0, 4'd07 }:    r = 5'd08;
            { 1'b0, 4'd08 }:    r = 5'd09;
            { 1'b0, 4'd09 }:    r = 5'd10;
            { 1'b0, 4'd10 }:    r = 5'd11;
            { 1'b0, 4'd11 }:    r = 5'd12;
            { 1'b0, 4'd12 }:    r = 5'd13;
            { 1'b0, 4'd13 }:    r = 5'd14;
            { 1'b0, 4'd14 }:    r = 5'd15;
            { 1'b0, 4'd15 }:    r = 5'd16; // { 1'b1, 4'd00 }

            // DECR-4 region
            { 1'b1, 4'd00 }:    r = 5'd31; // { 1'b1, 4'd15 }
            { 1'b1, 4'd01 }:    r = 5'd00;
            { 1'b1, 4'd02 }:    r = 5'd01;
            { 1'b1, 4'd03 }:    r = 5'd02;
            { 1'b1, 4'd04 }:    r = 5'd03;
            { 1'b1, 4'd05 }:    r = 5'd04;
            { 1'b1, 4'd06 }:    r = 5'd05;
            { 1'b1, 4'd07 }:    r = 5'd06;
            { 1'b1, 4'd08 }:    r = 5'd07;
            { 1'b1, 4'd09 }:    r = 5'd08;
            { 1'b1, 4'd10 }:    r = 5'd09;
            { 1'b1, 4'd11 }:    r = 5'd10;
            { 1'b1, 4'd12 }:    r = 5'd11;
            { 1'b1, 4'd13 }:    r = 5'd12;
            { 1'b1, 4'd14 }:    r = 5'd13;
            { 1'b1, 4'd15 }:    r = 5'd14;

            default:            r = 5'd00;
        endcase
    end

endmodule
