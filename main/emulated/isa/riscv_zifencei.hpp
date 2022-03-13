#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "Zifencei"
//

#include "riscvdef.hpp"


// Function-3
#define RVZIFENCEI_FUNCT3_MASK                  0x00007000
#define RVZIFENCEI_FUNCT3_OFFSET                12

#define RVZIFENCEI_FUNCT3_FENCE_I               0b001

