#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Base RV32I
//

#include "riscvdef.hpp"
#include "riscvmisc.hpp"

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
#define RV32I_OPERAND_FENCE_FM_MASK             0xF0000000U
#define RV32I_OPERAND_FENCE_FM_OFFSET           28

#define RV32I_OPERAND_FENCE_PI_MASK             0x08000000U
#define RV32I_OPERAND_FENCE_PI_OFFSET           27

#define RV32I_OPERAND_FENCE_PO_MASK             0x04000000U
#define RV32I_OPERAND_FENCE_PO_OFFSET           26

#define RV32I_OPERAND_FENCE_PR_MASK             0x02000000U
#define RV32I_OPERAND_FENCE_PR_OFFSET           25

#define RV32I_OPERAND_FENCE_PW_MASK             0x01000000U
#define RV32I_OPERAND_FENCE_PW_OFFSET           24

#define RV32I_OPERAND_FENCE_SI_MASK             0x00800000U
#define RV32I_OPERAND_FENCE_SI_OFFSET           23

#define RV32I_OPERAND_FENCE_SO_MASK             0x00400000U
#define RV32I_OPERAND_FENCE_SO_OFFSET           22

#define RV32I_OPERAND_FENCE_SR_MASK             0x00200000U
#define RV32I_OPERAND_FENCE_SR_OFFSET           21

#define RV32I_OPERAND_FENCE_SW_MASK             0x00100000U
#define RV32I_OPERAND_FENCE_SW_OFFSET           20



//
#define ENCODE_RV32I_SHx(funct3, funct7, rd, rs1, shamt) \
    ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, funct3, rd, rs1, (funct7 << 5) | shamt)


//
#define ENCODE_RV32I_ADDI(rd, rs1, imm)             ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, RV32I_FUNCT3_ADDI , rd, rs1, imm)
#define ENCODE_RV32I_SLTI(rd, rs1, imm)             ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, RV32I_FUNCT3_SLTI , rd, rs1, imm)
#define ENCODE_RV32I_SLTIU(rd, rs1, imm)            ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, RV32I_FUNCT3_SLTIU, rd, rs1, imm)
#define ENCODE_RV32I_XORI(rd, rs1, imm)             ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, RV32I_FUNCT3_XORI , rd, rs1, imm)
#define ENCODE_RV32I_ORI(rd, rs1, imm)              ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, RV32I_FUNCT3_ORI  , rd, rs1, imm)
#define ENCODE_RV32I_ANDI(rd, rs1, imm)             ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, RV32I_FUNCT3_ANDI , rd, rs1, imm)

#define ENCODE_RV32I_SLLI(rd, rs1, shamt)           ENCODE_RV32I_SHx(RV32I_FUNCT3_SLLI, RV32I_FUNCT7_SLLI, rd, rs1, shamt)
#define ENCODE_RV32I_SRLI(rd, rs1, shamt)           ENCODE_RV32I_SHx(RV32I_FUNCT3_SRLI, RV32I_FUNCT7_SRLI, rd, rs1, shamt)
#define ENCODE_RV32I_SRAI(rd, rs1, shamt)           ENCODE_RV32I_SHx(RV32I_FUNCT3_SRAI, RV32I_FUNCT7_SRAI, rd, rs1, shamt)

#define ENCODE_RV32I_ADD(rd, rs1, rs2)              ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_ADD , rd, rs1, rs2, RV32I_FUNCT7_ADD)
#define ENCODE_RV32I_SUB(rd, rs1, rs2)              ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_SUB , rd, rs1, rs2, RV32I_FUNCT7_SUB)
#define ENCODE_RV32I_SLL(rd, rs1, rs2)              ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_SLL , rd, rs1, rs2, RV32I_FUNCT7_SLL)
#define ENCODE_RV32I_SLT(rd, rs1, rs2)              ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_SLT , rd, rs1, rs2, RV32I_FUNCT7_SLT)
#define ENCODE_RV32I_SLTU(rd, rs1, rs2)             ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_SLTU, rd, rs1, rs2, RV32I_FUNCT7_SLTU)
#define ENCODE_RV32I_XOR(rd, rs1, rs2)              ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_XOR , rd, rs1, rs2, RV32I_FUNCT7_XOR)
#define ENCODE_RV32I_SRL(rd, rs1, rs2)              ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_SRL , rd, rs1, rs2, RV32I_FUNCT7_SRL)
#define ENCODE_RV32I_SRA(rd, rs1, rs2)              ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_SRA , rd, rs1, rs2, RV32I_FUNCT7_SRA)
#define ENCODE_RV32I_OR(rd, rs1, rs2)               ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_OR  , rd, rs1, rs2, RV32I_FUNCT7_OR)
#define ENCODE_RV32I_AND(rd, rs1, rs2)              ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32I_FUNCT3_AND , rd, rs1, rs2, RV32I_FUNCT7_AND)

#define ENCODE_RV32I_LUI(rd, imm)                   ENCODE_RVTYPE_U(RV_OPCODE_LUI  , rd, imm)
#define ENCODE_RV32I_AUIPC(rd, imm)                 ENCODE_RVTYPE_U(RV_OPCODE_AUIPC, rd, imm)

#define ENCODE_RV32I_JALR(rd, rs1, offset)          ENCODE_RVTYPE_I(RV_OPCODE_JALR, RV32I_FUNCT3_JALR, rd, rs1, offset)
#define ENCODE_RV32I_JAL(rd, offset)                ENCODE_RVTYPE_J(RV_OPCODE_JAL , rd, offset)

#define ENCODE_RV32I_BEQ(rs1, rs2, offset)          ENCODE_RVTYPE_B(RV_OPCODE_BRANCH, RV32I_FUNCT3_BEQ , rs1, rs2, offset)
#define ENCODE_RV32I_BNE(rs1, rs2, offset)          ENCODE_RVTYPE_B(RV_OPCODE_BRANCH, RV32I_FUNCT3_BNE , rs1, rs2, offset)
#define ENCODE_RV32I_BLT(rs1, rs2, offset)          ENCODE_RVTYPE_B(RV_OPCODE_BRANCH, RV32I_FUNCT3_BLT , rs1, rs2, offset)
#define ENCODE_RV32I_BGE(rs1, rs2, offset)          ENCODE_RVTYPE_B(RV_OPCODE_BRANCH, RV32I_FUNCT3_BGE , rs1, rs2, offset)
#define ENCODE_RV32I_BLTU(rs1, rs2, offset)         ENCODE_RVTYPE_B(RV_OPCODE_BRANCH, RV32I_FUNCT3_BLTU, rs1, rs2, offset)
#define ENCODE_RV32I_BGEU(rs1, rs2, offset)         ENCODE_RVTYPE_B(RV_OPCODE_BRANCH, RV32I_FUNCT3_BGEU, rs1, rs2, offset)

#define ENCODE_RV32I_LB(rd, rs1, offset)            ENCODE_RVTYPE_I(RV_OPCODE_LOAD, RV32I_FUNCT3_LB , rd, rs1, offset)
#define ENCODE_RV32I_LH(rd, rs1, offset)            ENCODE_RVTYPE_I(RV_OPCODE_LOAD, RV32I_FUNCT3_LH , rd, rs1, offset)
#define ENCODE_RV32I_LW(rd, rs1, offset)            ENCODE_RVTYPE_I(RV_OPCODE_LOAD, RV32I_FUNCT3_LW , rd, rs1, offset)
#define ENCODE_RV32I_LBU(rd, rs1, offset)           ENCODE_RVTYPE_I(RV_OPCODE_LOAD, RV32I_FUNCT3_LBU, rd, rs1, offset)
#define ENCODE_RV32I_LHU(rd, rs1, offset)           ENCODE_RVTYPE_I(RV_OPCODE_LOAD, RV32I_FUNCT3_LHU, rd, rs1, offset)

#define ENCODE_RV32I_SB(rs1, rs2, offset)           ENCODE_RVTYPE_S(RV_OPCODE_STORE, RV32I_FUNCT3_SB, rs1, rs2, offset)
#define ENCODE_RV32I_SH(rs1, rs2, offset)           ENCODE_RVTYPE_S(RV_OPCODE_STORE, RV32I_FUNCT3_SH, rs1, rs2, offset)
#define ENCODE_RV32I_SW(rs1, rs2, offset)           ENCODE_RVTYPE_S(RV_OPCODE_STORE, RV32I_FUNCT3_SW, rs1, rs2, offset)

#define ENCODE_RV32I_FENCE(fm, pre, suc)            ENCODE_RVTYPE_I(RV_OPCODE_MISC_MEM, RV_FUNCT3_FENCE, 0, 0, ((fm << 8) | (pre << 4) | suc))

#define ENCODE_RV32I_ECALL                          ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, 0, 0, 0, RV32I_FUNCT12_ECALL)
#define ENCODE_RV32I_EBREAK                         ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, 0, 0, 0, RV32I_FUNCT12_EBREAK)

#define ENCODE_RV32I_SRET                           ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, 0, 0, 0, RV32I_FUNCT12_SRET)
#define ENCODE_RV32I_MRET                           ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, 0, 0, 0, RV32I_FUNCT12_MRET)

