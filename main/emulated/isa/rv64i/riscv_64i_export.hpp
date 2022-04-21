#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, exporting fields
//

#include "riscv_64i_impl_execute.hpp"
#include "riscv_64i_impl_decode.hpp"
#include "riscv_64i_impl_encode.hpp"

// RV Codepoint instance exporting
namespace Jasse {

    decdef  RV64I           { new RV64IDecoder }; // RV64I Decoder Instance
    
    codedef RV64I_ADDI      {"addi"    , RVTYPE_I, __RV64I_D3CGEI_OP_IMM(ADDI) , &TextualizeRVTypeI};
    codedef RV64I_SLTI      {"slti"    , RVTYPE_I, __RV64I_D3CGEI_OP_IMM(SLTI) , &TextualizeRVTypeI};
    codedef RV64I_SLTIU     {"sltiu"   , RVTYPE_I, __RV64I_D3CGEI_OP_IMM(SLTIU), &TextualizeRVTypeI};
    codedef RV64I_ANDI      {"andi"    , RVTYPE_I, __RV64I_D3CGEI_OP_IMM(ANDI) , &TextualizeRVTypeI};
    codedef RV64I_ORI       {"ori"     , RVTYPE_I, __RV64I_D3CGEI_OP_IMM(ORI)  , &TextualizeRVTypeI};
    codedef RV64I_XORI      {"xori"    , RVTYPE_I, __RV64I_D3CGEI_OP_IMM(XORI) , &TextualizeRVTypeI};
    
    codedef RV64I_SLLI      {"slli"    , RVTYPE_I, __RV64I_D3CGEI_SHx(SLLI), &TextualizeRVTypeI};
    codedef RV64I_SRLI      {"srli"    , RVTYPE_I, __RV64I_D3CGEI_SHx(SRLI), &TextualizeRVTypeI};
    codedef RV64I_SRAI      {"srai"    , RVTYPE_I, __RV64I_D3CGEI_SHx(SRAI), &TextualizeRVTypeI};

    codedef RV64I_SLLIW     {"slliw"   , RVTYPE_I, __RV64I_D3CGEI_SHx32(SLLIW), &TextualizeRVTypeI};
    codedef RV64I_SRLIW     {"srliw"   , RVTYPE_I, __RV64I_D3CGEI_SHx32(SRLIW), &TextualizeRVTypeI};
    codedef RV64I_SRAIW     {"sraiw"   , RVTYPE_I, __RV64I_D3CGEI_SHx32(SRAIW), &TextualizeRVTypeI};

    codedef RV64I_ADDIW     {"addiw"   , RVTYPE_I, __RV64I_D3CGEI_OP_IMM32(ADDIW), &TextualizeRVTypeI};

    codedef RV64I_ADD       {"add"     , RVTYPE_R, __RV64I_D3CGER_OP(ADD) , &TextualizeRVTypeR};
    codedef RV64I_SUB       {"sub"     , RVTYPE_R, __RV64I_D3CGER_OP(SUB) , &TextualizeRVTypeR};
    codedef RV64I_SLT       {"slt"     , RVTYPE_R, __RV64I_D3CGER_OP(SLT) , &TextualizeRVTypeR};
    codedef RV64I_SLTU      {"sltu"    , RVTYPE_R, __RV64I_D3CGER_OP(SLTU), &TextualizeRVTypeR};
    codedef RV64I_AND       {"and"     , RVTYPE_R, __RV64I_D3CGER_OP(AND) , &TextualizeRVTypeR};
    codedef RV64I_OR        {"or"      , RVTYPE_R, __RV64I_D3CGER_OP(OR)  , &TextualizeRVTypeR};
    codedef RV64I_XOR       {"xor"     , RVTYPE_R, __RV64I_D3CGER_OP(XOR) , &TextualizeRVTypeR};
    codedef RV64I_SLL       {"sll"     , RVTYPE_R, __RV64I_D3CGER_OP(SLL) , &TextualizeRVTypeR};
    codedef RV64I_SRL       {"srl"     , RVTYPE_R, __RV64I_D3CGER_OP(SRL) , &TextualizeRVTypeR};
    codedef RV64I_SRA       {"sra"     , RVTYPE_R, __RV64I_D3CGER_OP(SRA) , &TextualizeRVTypeR};

    codedef RV64I_ADDW      {"addw"    , RVTYPE_R, __RV64I_D3CGER_OP32(ADDW), &TextualizeRVTypeR};
    codedef RV64I_SUBW      {"subw"    , RVTYPE_R, __RV64I_D3CGER_OP32(SUBW), &TextualizeRVTypeR};
    codedef RV64I_SLLW      {"sllw"    , RVTYPE_R, __RV64I_D3CGER_OP32(SLLW), &TextualizeRVTypeR};
    codedef RV64I_SRLW      {"srlw"    , RVTYPE_R, __RV64I_D3CGER_OP32(SRLW), &TextualizeRVTypeR};
    codedef RV64I_SRAW      {"sraw"    , RVTYPE_R, __RV64I_D3CGER_OP32(SRAW), &TextualizeRVTypeR};

    codedef RV64I_BEQ       {"beq"     , RVTYPE_B, __RV64I_D3CGEB_BRCH(BEQ) , &TextualizeRVTypeB};
    codedef RV64I_BNE       {"bne"     , RVTYPE_B, __RV64I_D3CGEB_BRCH(BNE) , &TextualizeRVTypeB};
    codedef RV64I_BLT       {"blt"     , RVTYPE_B, __RV64I_D3CGEB_BRCH(BLT) , &TextualizeRVTypeB};
    codedef RV64I_BLTU      {"bltu"    , RVTYPE_B, __RV64I_D3CGEB_BRCH(BLTU), &TextualizeRVTypeB};
    codedef RV64I_BGE       {"bge"     , RVTYPE_B, __RV64I_D3CGEB_BRCH(BGE) , &TextualizeRVTypeB};
    codedef RV64I_BGEU      {"bgeu"    , RVTYPE_B, __RV64I_D3CGEB_BRCH(BGEU), &TextualizeRVTypeB};

    codedef RV64I_LD        {"ld"      , RVTYPE_I, __RV64I_D3CGEI_LD(LD) , &TextualizeRVTypeI};
    codedef RV64I_LW        {"lw"      , RVTYPE_I, __RV64I_D3CGEI_LD(LW) , &TextualizeRVTypeI};
    codedef RV64I_LH        {"lh"      , RVTYPE_I, __RV64I_D3CGEI_LD(LH) , &TextualizeRVTypeI};
    codedef RV64I_LB        {"lb"      , RVTYPE_I, __RV64I_D3CGEI_LD(LB) , &TextualizeRVTypeI};
    codedef RV64I_LWU       {"lwu"     , RVTYPE_I, __RV64I_D3CGEI_LD(LWU), &TextualizeRVTypeI};
    codedef RV64I_LHU       {"lhu"     , RVTYPE_I, __RV64I_D3CGEI_LD(LHU), &TextualizeRVTypeI};
    codedef RV64I_LBU       {"lbu"     , RVTYPE_I, __RV64I_D3CGEI_LD(LBU), &TextualizeRVTypeI};

    codedef RV64I_SD        {"sd"      , RVTYPE_S, __RV64I_D3CGES_ST(SD), &TextualizeRVTypeS};
    codedef RV64I_SW        {"sw"      , RVTYPE_S, __RV64I_D3CGES_ST(SW), &TextualizeRVTypeS};
    codedef RV64I_SH        {"sh"      , RVTYPE_S, __RV64I_D3CGES_ST(SH), &TextualizeRVTypeS};
    codedef RV64I_SB        {"sb"      , RVTYPE_S, __RV64I_D3CGES_ST(SB), &TextualizeRVTypeS};

    codedef RV64I_ECALL     {"ecall"   , RVTYPE_I, __RV64I_D12CGEI_SYS(ECALL) , &TextualizeRVZeroOperand};
    codedef RV64I_EBREAK    {"ebreak"  , RVTYPE_I, __RV64I_D12CGEI_SYS(EBREAK), &TextualizeRVZeroOperand};
    codedef RV64I_MRET      {"mret"    , RVTYPE_I, __RV64I_D12CGEI_SYS(MRET)  , &TextualizeRVZeroOperand};
    codedef RV64I_SRET      {"sret"    , RVTYPE_I, __RV64I_D12CGEI_SYS(SRET)  , &TextualizeRVZeroOperand};
    codedef RV64I_WFI       {"wfi"     , RVTYPE_I, __RV64I_D12CGEI_SYS(WFI)   , &TextualizeRVZeroOperand};

    codedef RV64I_FENCE     {"fence"   , RVTYPE_I, __RV64I_DUCGEI_FENCE, &TextualizeRVTypeI};

    codedef RV64I_LUI       {"lui"     , RVTYPE_U, __RV64I_DUCGEU(LUI)  , &TextualizeRVTypeU};
    codedef RV64I_AUIPC     {"auipc"   , RVTYPE_U, __RV64I_DUCGEU(AUIPC), &TextualizeRVTypeU};

    codedef RV64I_JAL       {"jal"     , RVTYPE_J, __RV64I_DUCGEJ(JAL) , &TextualizeRVTypeJ};
    codedef RV64I_JALR      {"jalr"    , RVTYPE_I, __RV64I_DUCGEI(JALR), &TextualizeRVTypeI};


    codesetdef ALL_OF_RV64I {
        //
        &RV64I_ADDI,    &RV64I_SLTI,    &RV64I_SLTIU,   &RV64I_ANDI,    &RV64I_ORI,     &RV64I_XORI,
        &RV64I_SLLI,    &RV64I_SRLI,    &RV64I_SRAI,
        &RV64I_ADDW,    &RV64I_SLLIW,   &RV64I_SRLIW,   &RV64I_SRAIW,
        &RV64I_ADD,     &RV64I_SUB,     &RV64I_SLT,     &RV64I_SLTU,    &RV64I_AND,     &RV64I_OR,
        &RV64I_XOR,     &RV64I_SLL,     &RV64I_SRL,     &RV64I_SRA,
        &RV64I_ADDW,    &RV64I_SUBW,    &RV64I_SLLW,    &RV64I_SRLW,    &RV64I_SRAW,
        &RV64I_BEQ,     &RV64I_BNE,     &RV64I_BLT,     &RV64I_BLTU,    &RV64I_BGE,     &RV64I_BGEU,
        &RV64I_LD,      &RV64I_LW,      &RV64I_LH,      &RV64I_LB,      &RV64I_LWU,     &RV64I_LHU,     &RV64I_LBU,
        &RV64I_SD,      &RV64I_SW,      &RV64I_SH,      &RV64I_SB,
        &RV64I_ECALL,   &RV64I_EBREAK,  &RV64I_MRET,    &RV64I_SRET,    &RV64I_WFI,
        &RV64I_FENCE,
        &RV64I_LUI,     &RV64I_AUIPC,
        &RV64I_JAL,     &RV64I_JALR
    };
}