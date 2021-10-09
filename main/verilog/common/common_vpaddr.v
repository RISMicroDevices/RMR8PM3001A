// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Direct-Mapping of Virtual to Physical Address 
//
//  Note: 0x80000000 ~ 0xBFFFFFFF: External Memory Cached
//        0xC0000000 ~ 0xFFFFFFFF: External Memory Uncached
// 
//  ** UNCACHED for all non-ext-memory region **
//  ** 32-bit addressing MAX supported **
//  (Might be modified in future)
//
// @author Kumonda221
//

module common_vpaddr (
    input   wire[63:0]          vaddr,

    output  wire[31:0]          paddr,

    output  wire                attribute_mmio,
    output  wire                attribute_uncached,

    output  wire                exception_no_access
);

    wire seg0, seg1, seg2;
 
    assign seg0 = ~vaddr[31];
    assign seg1 =  vaddr[31] & ~vaddr[30];
    assign seg2 =  vaddr[31] &  vaddr[30];
    
    //
    assign paddr[29:0]  = vaddr[29:0];
    assign paddr[31:30] = (seg1 | seg2) ? 2'b10 : vaddr[31:30];

    //
    assign attribute_mmio     = seg0;
    assign attribute_uncached = seg0 | seg1;

    //
    assign exception_no_access = | {vaddr[63:32]};

endmodule
