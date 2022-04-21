#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, exporting fields declarations
//

#include "riscv.hpp"

// RV Codepoint instance exporting declarations
namespace Jasse {

    decdecl  RV64I;
    
    codedecl RV64I_ADDI  ;
    codedecl RV64I_SLTI  ;
    codedecl RV64I_SLTIU ;
    codedecl RV64I_ANDI  ;
    codedecl RV64I_ORI   ;
    codedecl RV64I_XORI  ;
    
    codedecl RV64I_SLLI  ;
    codedecl RV64I_SRLI  ;
    codedecl RV64I_SRAI  ;

    codedecl RV64I_SLLIW ;
    codedecl RV64I_SRLIW ;
    codedecl RV64I_SRAIW ;

    codedecl RV64I_ADDIW ;

    codedecl RV64I_ADD   ;
    codedecl RV64I_SUB   ;
    codedecl RV64I_SLT   ;
    codedecl RV64I_SLTU  ;
    codedecl RV64I_AND   ;
    codedecl RV64I_OR    ;
    codedecl RV64I_XOR   ;
    codedecl RV64I_SLL   ;
    codedecl RV64I_SRL   ;
    codedecl RV64I_SRA   ;

    codedecl RV64I_ADDW  ;
    codedecl RV64I_SUBW  ;
    codedecl RV64I_SLLW  ;
    codedecl RV64I_SRLW  ;
    codedecl RV64I_SRAW  ;

    codedecl RV64I_BEQ   ;
    codedecl RV64I_BNE   ;
    codedecl RV64I_BLT   ;
    codedecl RV64I_BLTU  ;
    codedecl RV64I_BGE   ;
    codedecl RV64I_BGEU  ;

    codedecl RV64I_LD    ;
    codedecl RV64I_LW    ;
    codedecl RV64I_LH    ;
    codedecl RV64I_LB    ;
    codedecl RV64I_LWU   ;
    codedecl RV64I_LHU   ;
    codedecl RV64I_LBU   ;

    codedecl RV64I_SD    ;
    codedecl RV64I_SW    ;
    codedecl RV64I_SH    ;
    codedecl RV64I_SB    ;

    codedecl RV64I_ECALL ;
    codedecl RV64I_EBREAK;
    codedecl RV64I_MRET  ;
    codedecl RV64I_SRET  ;
    codedecl RV64I_WFI   ;

    codedecl RV64I_FENCE ;

    codedecl RV64I_LUI   ;
    codedecl RV64I_AUIPC ;

    codedecl RV64I_JAL   ;
    codedecl RV64I_JALR  ;

    codesetdecl ALL_OF_RV64I;
}