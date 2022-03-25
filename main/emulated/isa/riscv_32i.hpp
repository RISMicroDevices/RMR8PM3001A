#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Base RV32I
//

#include "riscvdef.hpp"

// Function-3
#define RV32I_FUNCT3_MASK                       0x00007000U
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


// Function-7
#define RV32I_FUNCT7_MASK                       0xFE000000U
#define RV32I_FUNCT7_OFFSET                     25

#define RV32I_FUNCT7_SLLI                       0b0000000
#define RV32I_FUNCT7_SRLI                       0b0000000
#define RV32I_FUNCT7_SRAI                       0b0100000

#define RV32I_FUNCT7_ADD                        0b0000000
#define RV32I_FUNCT7_SUB                        0b0100000

#define RV32I_FUNCT7_SLL                        0b0000000
#define RV32I_FUNCT7_SLT                        0b0000000
#define RV32I_FUNCT7_SLTU                       0b0000000
#define RV32I_FUNCT7_XOR                        0b0000000
#define RV32I_FUNCT7_OR                         0b0000000
#define RV32I_FUNCT7_AND                        0b0000000

#define RV32I_FUNCT7_SRL                        0b0000000
#define RV32I_FUNCT7_SRA                        0b0100000


// Function-12
#define RV32I_FUNCT12_MASK                      0xFFF00000U
#define RV32I_FUNCT12_OFFSET                    20

#define RV32I_FUNCT12_ECALL                     0b0000000'00000
#define RV32I_FUNCT12_EBREAK                    0b0000000'00001

#define RV32I_FUNCT12_SRET                      0b0001000'00010
#define RV32I_FUNCT12_MRET                      0b0011000'00010

#define RV32I_FUNCT12_WFI                       0b0001000'00101


// Operands
#define RV32I_OPERAND_LOAD_FM_MASK              0xF0000000U
#define RV32I_OPERAND_LOAD_FM_OFFSET            28

#define RV32I_OPERAND_LOAD_PI_MASK              0x08000000U
#define RV32I_OPERAND_LOAD_PI_OFFSET            27

#define RV32I_OPERAND_LOAD_PO_MASK              0x04000000U
#define RV32I_OPERAND_LOAD_PO_OFFSET            26

#define RV32I_OPERAND_LOAD_PR_MASK              0x02000000U
#define RV32I_OPERAND_LOAD_PR_OFFSET            25

#define RV32I_OPERAND_LOAD_PW_MASK              0x01000000U
#define RV32I_OPERAND_LOAD_PW_OFFSET            24

#define RV32I_OPERAND_LOAD_SI_MASK              0x00800000U
#define RV32I_OPERAND_LOAD_SI_OFFSET            23

#define RV32I_OPERAND_LOAD_SO_MASK              0x00400000U
#define RV32I_OPERAND_LOAD_SO_OFFSET            22

#define RV32I_OPERAND_LOAD_SR_MASK              0x00200000U
#define RV32I_OPERAND_LOAD_SR_OFFSET            21

#define RV32I_OPERAND_LOAD_SW_MASK              0x00100000U
#define RV32I_OPERAND_LOAD_SW_OFFSET            20


