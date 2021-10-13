// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  RTL ROM Module for 3-bit unsigned increment and decrement
//
// ** Performs better in FPGA **
//
// @author Kumonda221
//

module macro_rom_decinc3 (
    input   wire [2:0]          d,
    input   wire                dec,
    output  wire [2:0]          q,
    output  wire                c
);

    reg  [3:0]      r;

    assign q = r[2:0];
    assign c = r[3]; 

    always @(*) begin

        //r = $unsigned(d) + (dec ? 3'b111 : 3'b001);

        case ({ dec, d })

            // INCR-3 region
            { 1'b0, 3'd00 }:    r = 4'd01;
            { 1'b0, 3'd01 }:    r = 4'd02;
            { 1'b0, 3'd02 }:    r = 4'd03;
            { 1'b0, 3'd03 }:    r = 4'd04;
            { 1'b0, 3'd04 }:    r = 4'd05;
            { 1'b0, 3'd05 }:    r = 4'd06;
            { 1'b0, 3'd06 }:    r = 4'd07;
            { 1'b0, 3'd07 }:    r = 4'd08; // { 1'b1, 3'd00 }

            // DECR-3 region
            { 1'b1, 3'd00 }:    r = 4'd15; // { 1'b1, 3'd07 }
            { 1'b1, 3'd01 }:    r = 4'd00;
            { 1'b1, 3'd02 }:    r = 4'd01;
            { 1'b1, 3'd03 }:    r = 4'd02;
            { 1'b1, 3'd04 }:    r = 4'd03;
            { 1'b1, 3'd05 }:    r = 4'd04;
            { 1'b1, 3'd06 }:    r = 4'd05;
            { 1'b1, 3'd07 }:    r = 4'd06;

            default:            r = 4'd00;
        endcase
    end

endmodule
