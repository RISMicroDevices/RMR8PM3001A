#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Base RV64I
//

#ifndef __INCLUDE_RV64I
#define __INCLUDE_RV64I
#endif

#include "riscvdef.hpp"
#include "riscv_32i.hpp"


// Function-3
#define RV64I_FUNCT3_MASK                       0x00007000
#define RV64I_FUNCT3_OFFSET                     12

#define RV64I_FUNCT3_ADDIW                      0b000
#define RV64I_FUNCT3_SLLIW                      0b001
#define RV64I_FUNCT3_SRLIW                      0b101
#define RV64I_FUNCT3_SRAIW                      0b101
#define RV64I_FUNCT3_SRLIW__SRAIW               0b101

#define RV64I_FUNCT3_ADDW                       0b000
#define RV64I_FUNCT3_SUBW                       0b000
#define RV64I_FUNCT3_ADDW__SUBW                 0b000
#define RV64I_FUNCT3_SLLW                       0b001
#define RV64I_FUNCT3_SRLW                       0b101
#define RV64I_FUNCT3_SRAW                       0b101
#define RV64I_FUNCT3_SRLW__SRAW                 0b101


//
