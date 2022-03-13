#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV32M
//

#include "riscvdef.hpp"


// Function-3
#define RV32M_FUNCT3_MASK                       0x00007000
#define RV32M_FUNCT3_OFFSET                     12

#define RV32M_FUNCT3_MUL                        0b000
#define RV32M_FUNCT3_MULH                       0b001
#define RV32M_FUNCT3_MULHSU                     0b010
#define RV32M_FUNCT3_MULHU                      0b011

#define RV32M_FUNCT3_DIV                        0b100
#define RV32M_FUNCT3_DIVU                       0b101
#define RV32M_FUNCT3_REM                        0b110
#define RV32M_FUNCT3_REMU                       0b111


//
#ifndef __INCLUDE_RV64M

// ...

#endif
