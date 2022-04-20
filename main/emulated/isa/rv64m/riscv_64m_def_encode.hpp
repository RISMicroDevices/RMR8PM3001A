#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, encoding definitions
//

#include "riscv_64m_def_base.hpp"

//
#define ENCODE_RV64M_MUL(rd, rs1, rs2)          ENCODE_RV32M_MUL(rd, rs1, rs2)   
#define ENCODE_RV64M_MULH(rd, rs1, rs2)         ENCODE_RV32M_MULH(rd, rs1, rs2)  
#define ENCODE_RV64M_MULHSU(rd, rs1, rs2)       ENCODE_RV32M_MULHSU(rd, rs1, rs2)
#define ENCODE_RV64M_MULHU(rd, rs1, rs2)        ENCODE_RV32M_MULHU(rd, rs1, rs2) 

#define ENCODE_RV64M_DIV(rd, rs1, rs2)          ENCODE_RV32M_DIV(rd, rs1, rs2)   
#define ENCODE_RV64M_DIVU(rd, rs1, rs2)         ENCODE_RV32M_DIVU(rd, rs1, rs2)  
#define ENCODE_RV64M_REM(rd, rs1, rs2)          ENCODE_RV32M_REM(rd, rs1, rs2)   
#define ENCODE_RV64M_REMU(rd, rs1, rs2)         ENCODE_RV32M_REMU(rd, rs1, rs2)  

#define ENCODE_RV64M_MULW(rd, rs1, rs2)         ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64M_FUNCT3_MULW , rd, rs1, rs2, RV64M_FUNCT7_MULW)

#define ENCODE_RV64M_DIVW(rd, rs1, rs2)         ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64M_FUNCT3_DIVW , rd, rs1, rs2, RV64M_FUNCT7_DIVW)
#define ENCODE_RV64M_DIVUW(rd, rs1, rs2)        ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64M_FUNCT3_DIVUW, rd, rs1, rs2, RV64M_FUNCT7_DIVUW)
#define ENCODE_RV64M_REMW(rd, rs1, rs2)         ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64M_FUNCT3_REMW , rd, rs1, rs2, RV64M_FUNCT7_REMW)
#define ENCODE_RV64M_REMUW(rd, rs1, rs2)        ENCODE_RVTYPE_R(RV_OPCODE_OP_32, RV64M_FUNCT3_REMUW, rd, rs1, rs2, RV64M_FUNCT7_REMUW)
