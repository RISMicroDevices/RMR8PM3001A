// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  RTL ROM Module for 2-bit unsigned decrement
//
// @author Kumonda221
//

module macro_rom_decr2 (
    input   wire [1:0]          d,
    output  wire [1:0]          q,
    output  wire                c
);

    reg  [2:0]      r;

    assign q = r[1:0];
    assign c = r[2];

    always @(*) begin
        
        // r = $unsigned(d) - 1;

        case (d)

            2'd00:      r = 3'd07; // { 1'b1, 2'd03 }
            2'd01:      r = 3'd00;
            2'd02:      r = 3'd01;
            2'd03:      r = 3'd02;

            default:    r = 3'd00;
        endcase
    end

endmodule
