#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV32M
//

#include "base/riscvdef.hpp"
#include "riscvmisc.hpp"


// Function-3
#define RV32M_FUNCT3_MASK                       RV_FUNCT3_MASK
#define RV32M_FUNCT3_OFFSET                     RV_FUNCT3_OFFSET

#define RV32M_FUNCT3_MUL                        0b000
#define RV32M_FUNCT3_MULH                       0b001
#define RV32M_FUNCT3_MULHSU                     0b010
#define RV32M_FUNCT3_MULHU                      0b011

#define RV32M_FUNCT3_DIV                        0b100
#define RV32M_FUNCT3_DIVU                       0b101
#define RV32M_FUNCT3_REM                        0b110
#define RV32M_FUNCT3_REMU                       0b111


// Function-7
#define RV32M_FUNCT7_MASK                       RV_FUNCT7_MASK
#define RV32M_FUNCT7_OFFSET                     RV_FUNCT7_OFFSET

#define RV32M_FUNCT7_MUL                        0b0000001
#define RV32M_FUNCT7_MULH                       0b0000001
#define RV32M_FUNCT7_MULHSU                     0b0000001
#define RV32M_FUNCT7_MULHU                      0b0000001

#define RV32M_FUNCT7_DIV                        0b0000001
#define RV32M_FUNCT7_DIVU                       0b0000001
#define RV32M_FUNCT7_REM                        0b0000001
#define RV32M_FUNCT7_REMU                       0b0000001



//
#define ENCODE_RV32M_MUL(rd, rs1, rs2)          ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32M_FUNCT3_MUL   , rd, rs1, rs2, RV32M_FUNCT7_MUL)
#define ENCODE_RV32M_MULH(rd, rs1, rs2)         ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32M_FUNCT3_MULH  , rd, rs1, rs2, RV32M_FUNCT7_MULH)
#define ENCODE_RV32M_MULHSU(rd, rs1, rs2)       ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32M_FUNCT3_MULHSU, rd, rs1, rs2, RV32M_FUNCT7_MULHSU)
#define ENCODE_RV32M_MULHU(rd, rs1, rs2)        ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32M_FUNCT3_MULHU , rd, rs1, rs2, RV32M_FUNCT7_MULHU)

#define ENCODE_RV32M_DIV(rd, rs1, rs2)          ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32M_FUNCT3_DIV   , rd, rs1, rs2, RV32M_FUNCT7_DIV)
#define ENCODE_RV32M_DIVU(rd, rs1, rs2)         ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32M_FUNCT3_DIVU  , rd, rs1, rs2, RV32M_FUNCT7_DIVU)
#define ENCODE_RV32M_REM(rd, rs1, rs2)          ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32M_FUNCT3_REM   , rd, rs1, rs2, RV32M_FUNCT7_REM)
#define ENCODE_RV32M_REMU(rd, rs1, rs2)         ENCODE_RVTYPE_R(RV_OPCODE_OP, RV32M_FUNCT3_REMU  , rd, rs1, rs2, RV32M_FUNCT7_REMU)


//
#ifndef __INCLUDE_RV64M

// ...

#endif
