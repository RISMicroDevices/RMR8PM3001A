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


// Function-6
#define RV64I_FUNCT6_MASK                       0xFC000000
#define RV64I_FUNCT6_OFFSET                     26

#define RV64I_FUNCT6_SLLI                       0b000000
#define RV64I_FUNCT6_SRLI                       0b000000
#define RV64I_FUNCT6_SRAI                       0b010000


// Function-7
#define RV64I_FUNCT7_MASK                       0xFE000000
#define RV64I_FUNCT7_OFFSET                     25

#define RV64I_FUNCT7_SLLIW                      0b0000000
#define RV64I_FUNCT7_SRLIW                      0b0000000
#define RV64I_FUNCT7_SRAIW                      0b0100000

#define RV64I_FUNCT7_ADDW                       0b0000000
#define RV64I_FUNCT7_SUBW                       0b0100000

#define RV64I_FUNCT7_SLLW                       0b0000000
#define RV64I_FUNCT7_SRLW                       0b0000000
#define RV64I_FUNCT7_SRAW                       0b0100000



