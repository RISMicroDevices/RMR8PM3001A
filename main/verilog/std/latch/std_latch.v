// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard Latch Module
//
// @author Kumonda221
//

module std_latch #(
    parameter   LATCH_WIDTH         = 1
) (
    input   wire                        en,

    input   wire [LATCH_WIDTH - 1:0]    d,
    output  wire [LATCH_WIDTH - 1:0]    q
);

    assign q = en ? d : q;

endmodule
