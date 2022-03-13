#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR"
//

#include "riscvdef.hpp"


// Function-3
#define RVZICSR_FUNCT3_MASK                     0x00007000
#define RVZICSR_FUNCT3_OFFSET                   12

#define RVZICSR_FUNCT3_CSRRW                    0b001
#define RVZICSR_FUNCT3_CSRRS                    0b010
#define RVZICSR_FUNCT3_CSRRC                    0b011
#define RVZICSR_FUNCT3_CSRRWI                   0b101
#define RVZICSR_FUNCT3_CSRRSI                   0b110
#define RVZICSR_FUNCT3_CSRRCI                   0b111

