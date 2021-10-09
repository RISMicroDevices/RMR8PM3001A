// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  RTL ROM Module for 2-bit unsigned increment and decrement
//
// @author Kumonda221
//

module common_rtlrom_decinc2 (
    input   wire [1:0]          d,
    input   wire                dec,
    output  wire [1:0]          q,
    output  wire                c
);

    reg  [2:0]      r;

    assign q = r[1:0];
    assign c = r[2];

    always @(*) begin

        // r = $unsigned(d) + (dec ? 2'b11 : 2'b01);

        case ({ dec, d })

            // INCR-2 region
            { 1'b0, 2'd00 }:    r = 3'd01;
            { 1'b0, 2'd01 }:    r = 3'd02;
            { 1'b0, 2'd02 }:    r = 3'd03;
            { 1'b0, 2'd03 }:    r = 3'd04; // { 1'b1, 2'd00 }

            // DECR-2 region
            { 1'b1, 2'd00 }:    r = 3'd07; // { 1'b1, 2'd03 }
            { 1'b1, 2'd01 }:    r = 3'd00;
            { 1'b1, 2'd02 }:    r = 3'd01;
            { 1'b1, 2'd03 }:    r = 3'd02;

            default:            r = 3'd00;
        endcase
    end

endmodule
