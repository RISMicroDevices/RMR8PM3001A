// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Standard Single-Port SRAM Module with Direct Output
//
// @author Kumonda221
//

module srd_sram_singleport #(
    parameter   ADDR_WIDTH          = 1,
    parameter   DATA_WIDTH          = 1
) (
    input   wire                        clk,
    input   wire                        en,

    input   wire [ADDR_WIDTH - 1:0]     addr,
    input   wire [DATA_WIDTH - 1:0]     din,
    input   wire                        we,
    output  wire [DATA_WIDTH - 1:0]     dout
);

    localparam SRAM_DEPTH = 1 << ADDR_WIDTH;

    reg  [DATA_WIDTH - 1:0]     mem [SRAM_DPETH - 1:0];

    always @(posedge clk) begin
        
        if (en & we) begin
            mem[addr] <= din;
        end
    end

    assign dout = mem[addr];

endmodule

