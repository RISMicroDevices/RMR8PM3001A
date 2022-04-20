#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, basic definitions
//

#include "riscv_32m.hpp"


// Function-3
#define RV64M_FUNCT3_MASK                       RV_FUNCT3_MASK
#define RV64M_FUNCT3_OFFSET                     RV_FUNCT3_OFFSET

#define RV64M_FUNCT3_MUL                        RV32M_FUNCT3_MUL   
#define RV64M_FUNCT3_MULH                       RV32M_FUNCT3_MULH  
#define RV64M_FUNCT3_MULHSU                     RV32M_FUNCT3_MULHSU
#define RV64M_FUNCT3_MULHU                      RV32M_FUNCT3_MULHU 

#define RV64M_FUNCT3_DIV                        RV32M_FUNCT3_DIV   
#define RV64M_FUNCT3_DIVU                       RV32M_FUNCT3_DIVU  
#define RV64M_FUNCT3_REM                        RV32M_FUNCT3_REM   
#define RV64M_FUNCT3_REMU                       RV32M_FUNCT3_REMU  

#define RV64M_FUNCT3_MULW                       0b000
#define RV64M_FUNCT3_DIVW                       0b100
#define RV64M_FUNCT3_DIVUW                      0b101
#define RV64M_FUNCT3_REMW                       0b110
#define RV64M_FUNCT3_REMUW                      0b111


// Function-7
#define RV64M_FUNCT7_MASK                       RV_FUNCT7_MASK
#define RV64M_FUNCT7_OFFSET                     RV_FUNCT7_OFFSET

#define RV64M_FUNCT7_MUL                        RV32M_FUNCT7_MUL   
#define RV64M_FUNCT7_MULH                       RV32M_FUNCT7_MULH  
#define RV64M_FUNCT7_MULHSU                     RV32M_FUNCT7_MULHSU
#define RV64M_FUNCT7_MULHU                      RV32M_FUNCT7_MULHU 

#define RV64M_FUNCT7_DIV                        RV32M_FUNCT7_DIV   
#define RV64M_FUNCT7_DIVU                       RV32M_FUNCT7_DIVU  
#define RV64M_FUNCT7_REM                        RV32M_FUNCT7_REM   
#define RV64M_FUNCT7_REMU                       RV32M_FUNCT7_REMU  

#define RV64M_FUNCT7_MULW                       0b0000001
#define RV64M_FUNCT7_DIVW                       0b0000001
#define RV64M_FUNCT7_DIVUW                      0b0000001
#define RV64M_FUNCT7_REMW                       0b0000001
#define RV64M_FUNCT7_REMUW                      0b0000001
