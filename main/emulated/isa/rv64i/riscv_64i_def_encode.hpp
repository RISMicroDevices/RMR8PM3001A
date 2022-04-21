#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, encoding definitions
//


#include "riscv_64i_def_base.hpp"

//
#define ENCODE_RV64I_SHx(funct3, funct6, rd, rs1, shamt) \
    ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, funct3, rd, rs1, (funct6 << 6) | shamt)

#define ENCODE_RV64I_SHxW(funct3, funct7, rd, rs1, shamt) \
    ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM, funct3, rd, rs1, (funct7 << 5) | shamt)

//
#define ENCODE_RV64I_ADDI(rd, rs1, imm)             ENCODE_RV32I_ADDI(rd, rs1, imm)
#define ENCODE_RV64I_SLTI(rd, rs1, imm)             ENCODE_RV32I_SLTI(rd, rs1, imm)
#define ENCODE_RV64I_SLTIU(rd, rs1, imm)            ENCODE_RV32I_SLTIU(rd, rs1, imm)
#define ENCODE_RV64I_XORI(rd, rs1, imm)             ENCODE_RV32I_XORI(rd, rs1, imm)
#define ENCODE_RV64I_ORI(rd, rs1, imm)              ENCODE_RV32I_ORI(rd, rs1, imm)
#define ENCODE_RV64I_ANDI(rd, rs1, imm)             ENCODE_RV32I_ANDI(rd, rs1, imm)

#define ENCODE_RV64I_SLLI(rd, rs1, shamt)           ENCODE_RV64I_SHx(RV64I_FUNCT3_SLLI, RV64I_FUNCT6_SLLI, rd, rs1, shamt)
#define ENCODE_RV64I_SRLI(rd, rs1, shamt)           ENCODE_RV64I_SHx(RV64I_FUNCT3_SRLI, RV64I_FUNCT6_SRLI, rd, rs1, shamt)
#define ENCODE_RV64I_SRAI(rd, rs1, shamt)           ENCODE_RV64I_SHx(RV64I_FUNCT3_SRAI, RV64I_FUNCT6_SRAI, rd, rs1, shamt)

#define ENCODE_RV64I_ADDIW(rd, rs1, imm)            ENCODE_RVTYPE_I(RV_OPCODE_OP_IMM_32, RV64I_FUNCT3_ADDIW, rd, rs1, imm)

#define ENCODE_RV64I_SLLIW(rd, rs1, shamt)          ENCODE_RV64I_SHxW(RV64I_FUNCT3_SLLIW, RV64I_FUNCT7_SLLIW, rd, rs1, shamt)
#define ENCODE_RV64I_SRLIW(rd, rs1, shamt)          ENCODE_RV64I_SHxW(RV64I_FUNCT3_SRLIW, RV64I_FUNCT7_SRLIW, rd, rs1, shamt)
#define ENCODE_RV64I_SRAIW(rd, rs1, shamt)          ENCODE_RV64I_SHxW(RV64I_FUNCT3_SRAIW, RV64I_FUNCT7_SRAIW, rd, rs1, shamt)

#define ENCODE_RV64I_ADD(rd, rs1, rs2)              ENCODE_RV32I_ADD(rd, rs1, rs2)
#define ENCODE_RV64I_SUB(rd, rs1, rs2)              ENCODE_RV32I_SUB(rd, rs1, rs2)
#define ENCODE_RV64I_SLL(rd, rs1, rs2)              ENCODE_RV32I_SLL(rd, rs1, rs2)
#define ENCODE_RV64I_SLT(rd, rs1, rs2)              ENCODE_RV32I_SLT(rd, rs1, rs2)
#define ENCODE_RV64I_SLTU(rd, rs1, rs2)             ENCODE_RV32I_SLTU(rd, rs1, rs2)
#define ENCODE_RV64I_XOR(rd, rs1, rs2)              ENCODE_RV32I_XOR(rd, rs1, rs2)
#define ENCODE_RV64I_SRL(rd, rs1, rs2)              ENCODE_RV32I_SRL(rd, rs1, rs2)
#define ENCODE_RV64I_SRA(rd, rs1, rs2)              ENCODE_RV32I_SRA(rd, rs1, rs2)
#define ENCODE_RV64I_OR(rd, rs1, rs2)               ENCODE_RV32I_OR(rd, rs1, rs2)
#define ENCODE_RV64I_AND(rd, rs1, rs2)              ENCODE_RV32I_AND(rd, rs1, rs2)

#define ENCODE_RV64I_ADDW(rd, rs1, rs2)             ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64I_FUNCT3_ADDW, rd, rs1, rs2, RV64I_FUNCT7_ADDW)
#define ENCODE_RV64I_SUBW(rd, rs1, rs2)             ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64I_FUNCT3_SUBW, rd, rs1, rs2, RV64I_FUNCT7_SUBW)
#define ENCODE_RV64I_SLLW(rd, rs1, rs2)             ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64I_FUNCT3_SLLW, rd, rs1, rs2, RV64I_FUNCT7_SLLW)
#define ENCODE_RV64I_SRLW(rd, rs1, rs2)             ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64I_FUNCT3_SRLW, rd, rs1, rs2, RV64I_FUNCT7_SRLW)
#define ENCODE_RV64I_SRAW(rd, rs1, rs2)             ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64I_FUNCT3_SRAW, rd, rs1, rs2, RV64I_FUNCT7_SRAW)

#define ENCODE_RV64I_LUI(rd, imm)                   ENCODE_RV32I_LUI(rd, imm)
#define ENCODE_RV64I_AUIPC(rd, imm)                 ENCODE_RV32I_AUIPC(rd, imm)

#define ENCODE_RV64I_JALR(rd, rs1, offset)          ENCODE_RV32I_JALR(rd, rs1, offset)
#define ENCODE_RV64I_JAL(rd, offset)                ENCODE_RV32I_JAL(rd, offset)     

#define ENCODE_RV64I_BEQ(rs1, rs2, offset)          ENCODE_RV32I_BEQ(rs1, rs2, offset)
#define ENCODE_RV64I_BNE(rs1, rs2, offset)          ENCODE_RV32I_BNE(rs1, rs2, offset)
#define ENCODE_RV64I_BLT(rs1, rs2, offset)          ENCODE_RV32I_BLT(rs1, rs2, offset)
#define ENCODE_RV64I_BGE(rs1, rs2, offset)          ENCODE_RV32I_BGE (rs1, rs2, offset)
#define ENCODE_RV64I_BLTU(rs1, rs2, offset)         ENCODE_RV32I_BLTU(rs1, rs2, offset)
#define ENCODE_RV64I_BGEU(rs1, rs2, offset)         ENCODE_RV32I_BGEU(rs1, rs2, offset)

#define ENCODE_RV64I_LB(rd, rs1, offset)            ENCODE_RV32I_LB(rd, rs1, offset)
#define ENCODE_RV64I_LH(rd, rs1, offset)            ENCODE_RV32I_LH(rd, rs1, offset)
#define ENCODE_RV64I_LW(rd, rs1, offset)            ENCODE_RV32I_LW(rd, rs1, offset)
#define ENCODE_RV64I_LBU(rd, rs1, offset)           ENCODE_RV32I_LBU(rd, rs1, offset)
#define ENCODE_RV64I_LHU(rd, rs1, offset)           ENCODE_RV32I_LHU(rd, rs1, offset)

#define ENCODE_RV64I_LD(rd, rs1, offset)            ENCODE_RVTYPE_I(RV_OPCODE_LOAD, RV64I_FUNCT3_LD , rd, rs1, offset)
#define ENCODE_RV64I_LWU(rd, rs1, offset)           ENCODE_RVTYPE_I(RV_OPCODE_LOAD, RV64I_FUNCT3_LWU, rd, rs1, offset)

#define ENCODE_RV64I_SB(rs1, rs2, offset)           ENCODE_RV32I_SB(rs1, rs2, offset)
#define ENCODE_RV64I_SH(rs1, rs2, offset)           ENCODE_RV32I_SH(rs1, rs2, offset)
#define ENCODE_RV64I_SW(rs1, rs2, offset)           ENCODE_RV32I_SW(rs1, rs2, offset)

#define ENCODE_RV64I_SD(rs1, rs2, offset)           ENCODE_RVTYPE_S(RV_OPCODE_STORE, RV64I_FUNCT3_SD, rs1, rs2, offset)

#define ENCODE_RV64I_FENCE(fm, pre, suc)            ENCODE_RV32I_FENCE(fm, pre, suc)

#define ENCODE_RV64I_ECALL                          ENCODE_RV32I_ECALL              
#define ENCODE_RV64I_EBREAK                         ENCODE_RV32I_EBREAK             

#define ENCODE_RV64I_SRET                           ENCODE_RV32I_SRET               
#define ENCODE_RV64I_MRET                           ENCODE_RV32I_MRET  

