#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Base RV32I
//

#include "riscvdef.hpp"

// Function-3
#define RV32I_FUNCT3_MASK                       0x00007000
#define RV32I_FUNCT3_OFFSET                     12

#define RV32I_FUNCT3_ADDI                       0b000
#define RV32I_FUNCT3_SLTI                       0b010
#define RV32I_FUNCT3_SLTIU                      0b011
#define RV32I_FUNCT3_XORI                       0b100
#define RV32I_FUNCT3_ORI                        0b110
#define RV32I_FUNCT3_ANDI                       0b111

#define RV32I_FUNCT3_SLLI                       0b001
#define RV32I_FUNCT3_SRLI                       0b101
#define RV32I_FUNCT3_SRAI                       0b101
#define RV32I_FUNCT3_SRLI__SRAI                 0b101

#define RV32I_FUNCT3_ADD                        0b000
#define RV32I_FUNCT3_SUB                        0b000
#define RV32I_FUNCT3_ADD__SUB                   0b000
#define RV32I_FUNCT3_SLL                        0b001
#define RV32I_FUNCT3_SLT                        0b010
#define RV32I_FUNCT3_SLTU                       0b011
#define RV32I_FUNCT3_XOR                        0b100
#define RV32I_FUNCT3_SRL                        0b101
#define RV32I_FUNCT3_SRA                        0b101
#define RV32I_FUNCT3_SRL__SRA                   0b101
#define RV32I_FUNCT3_OR                         0b110
#define RV32I_FUNCT3_AND                        0b111

#define RV32I_FUNCT3_JALR                       0b000
#define RV32I_FUNCT3_BEQ                        0b000
#define RV32I_FUNCT3_BNE                        0b001
#define RV32I_FUNCT3_BLT                        0b100
#define RV32I_FUNCT3_BGE                        0b101
#define RV32I_FUNCT3_BLTU                       0b110
#define RV32I_FUNCT3_BGEU                       0b111

#define RV32I_FUNCT3_LB                         0b000
#define RV32I_FUNCT3_LH                         0b001
#define RV32I_FUNCT3_LW                         0b010
#define RV32I_FUNCT3_LBU                        0b100
#define RV32I_FUNCT3_LHU                        0b101
#define RV32I_FUNCT3_SB                         0b000
#define RV32I_FUNCT3_SH                         0b001
#define RV32I_FUNCT3_SW                         0b010

#define RV32I_FUNCT3_FENCE                      0b000


//
#ifndef __INCLUDE_RV64I

// ...

#endif
