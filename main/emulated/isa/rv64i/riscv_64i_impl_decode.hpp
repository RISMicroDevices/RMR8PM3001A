#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, decoding infrastructure implementations
//

#include "riscv_64i_def_base.hpp"
#include "riscv_64i_def_decode.hpp"
#include "riscv_64i_def_export.hpp"
#include "riscvmisc.hpp"

//
#define RV64I_DECINSN(T, codepoint) \
    (DecodeNormalRVType##T(insnraw, insn), insn.SetCodepoint(&codepoint), true)

// codepoints - decoder facilities
namespace Jasse {

    //
    bool RV64ICodePoint_Funct3_ADDI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ADDI);
    }

    bool RV64ICodePoint_Funct3_SLTI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_SLTI);
    }

    bool RV64ICodePoint_Funct3_SLTIU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_SLTIU);
    }

    bool RV64ICodePoint_Funct3_ANDI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ANDI);
    }

    bool RV64ICodePoint_Funct3_ORI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ORI);
    }

    bool RV64ICodePoint_Funct3_XORI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_XORI);
    }

    bool RV64ICodePoint_Funct3_SLLI(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct6 = GET_STD_OPERAND(insnraw, RV64I_FUNCT6);

        if (funct6 == RV64I_FUNCT6_SLLI)
            return RV64I_DECINSN(I, RV64I_SLLI);
        else
            return false;
    }

#define  RV64ICodePoint_Funct3_SRLI         RV64ICodePoint_Funct3_SRLI__SRAI
#define  RV64ICodePoint_Funct3_SRAI         RV64ICodePoint_Funct3_SRLI__SRAI
    bool RV64ICodePoint_Funct3_SRLI__SRAI(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct6 = GET_STD_OPERAND(insnraw, RV64I_FUNCT6);

        if (funct6 == RV64I_FUNCT6_SRLI)
            return RV64I_DECINSN(I, RV64I_SRLI);
        else if (funct6 == RV64I_FUNCT6_SRAI)
            return RV64I_DECINSN(I, RV64I_SRAI);
        else
            return false;
    }

    //
    bool RV64ICodePoint_Funct3_ADDIW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ADDIW);
    }

    bool RV64ICodePoint_Funct3_SLLIW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLLIW)
            return RV64I_DECINSN(I, RV64I_SLLIW);
        else
            return false;
    }

#define  RV64ICodePoint_Funct3_SRLIW        RV64ICodePoint_Funct3_SRLIW__SRAIW
#define  RV64ICodePoint_Funct3_SRAIW        RV64ICodePoint_Funct3_SRLIW__SRAIW
    bool RV64ICodePoint_Funct3_SRLIW__SRAIW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SRLIW)
            return RV64I_DECINSN(I, RV64I_SRLIW);
        else if (funct7 == RV64I_FUNCT7_SRAIW)
            return RV64I_DECINSN(I, RV64I_SRAIW);
        else
            return false;
    }

    //
#define  RV64ICodePoint_Funct3_ADD          RV64ICodePoint_Funct3_ADD__SUB
#define  RV64ICodePoint_Funct3_SUB          RV64ICodePoint_Funct3_ADD__SUB
    bool RV64ICodePoint_Funct3_ADD__SUB(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_ADD)
            return RV64I_DECINSN(R, RV64I_ADD);
        else if (funct7 == RV64I_FUNCT7_SUB)
            return RV64I_DECINSN(R, RV64I_SUB);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLT(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLT)
            return RV64I_DECINSN(R, RV64I_SLT);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLTU(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLTU)
            return RV64I_DECINSN(R, RV64I_SLTU);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_AND(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_AND)
            return RV64I_DECINSN(R, RV64I_AND);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_OR(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_OR)
            return RV64I_DECINSN(R, RV64I_OR);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_XOR(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_XOR)
            return RV64I_DECINSN(R, RV64I_XOR);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLL(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLL)
            return RV64I_DECINSN(R, RV64I_SLL);
        else
            return false;
    }

#define  RV64ICodePoint_Funct3_SRL          RV64ICodePoint_Funct3_SRL__SRA
#define  RV64ICodePoint_Funct3_SRA          RV64ICodePoint_Funct3_SRL__SRA
    bool RV64ICodePoint_Funct3_SRL__SRA(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SRL)
            return RV64I_DECINSN(R, RV64I_SRL);
        else if (funct7 == RV64I_FUNCT7_SRA)
            return RV64I_DECINSN(R, RV64I_SRA);
        else
            return false;
    }

    //
#define  RV64ICodePoint_Funct3_ADDW        RV64ICodePoint_Funct3_ADDW__SUBW
#define  RV64ICodePoint_Funct3_SUBW        RV64ICodePoint_Funct3_ADDW__SUBW
    bool RV64ICodePoint_Funct3_ADDW__SUBW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_ADDW)
            return RV64I_DECINSN(R, RV64I_ADDW);
        else if (funct7 == RV64I_FUNCT7_SUBW)
            return RV64I_DECINSN(R, RV64I_SUBW);
        else
            return false;
    }

    bool RV64ICodePoint_Funct3_SLLW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SLLW)
            return RV64I_DECINSN(R, RV64I_SLLW);
        else
            return false;
    }

#define  RV64ICodePoint_Funct3_SRLW         RV64ICodePoint_Funct3_SRLW__SRAW
#define  RV64ICodePoint_Funct3_SRAW         RV64ICodePoint_Funct3_SRLW__SRAW
    bool RV64ICodePoint_Funct3_SRLW__SRAW(insnraw_t insnraw, RVInstruction& insn)
    {
        int funct7 = GET_STD_OPERAND(insnraw, RV64I_FUNCT7);

        if (funct7 == RV64I_FUNCT7_SRLW)
            return RV64I_DECINSN(R, RV64I_SRLW);
        else if (funct7 == RV64I_FUNCT7_SRAW)
            return RV64I_DECINSN(R, RV64I_SRAW);
        else
            return false;
    }


    //
    bool RV64ICodePoint_Funct3_BEQ(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BEQ);
    }

    bool RV64ICodePoint_Funct3_BNE(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BNE);
    }

    bool RV64ICodePoint_Funct3_BLT(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BLT);
    }

    bool RV64ICodePoint_Funct3_BLTU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BLTU);
    }

    bool RV64ICodePoint_Funct3_BGE(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BGE);
    }

    bool RV64ICodePoint_Funct3_BGEU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(B, RV64I_BGEU);
    }

    //
    bool RV64ICodePoint_Funct3_LD(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LD);
    }

    bool RV64ICodePoint_Funct3_LW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LW);
    }

    bool RV64ICodePoint_Funct3_LH(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LH);
    }

    bool RV64ICodePoint_Funct3_LB(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LB);
    }

    bool RV64ICodePoint_Funct3_LWU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LWU);
    }

    bool RV64ICodePoint_Funct3_LHU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LHU);
    }

    bool RV64ICodePoint_Funct3_LBU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_LBU);
    }

    //
    bool RV64ICodePoint_Funct3_SD(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(S, RV64I_SD);
    }

    bool RV64ICodePoint_Funct3_SW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(S, RV64I_SW);
    }

    bool RV64ICodePoint_Funct3_SH(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(S, RV64I_SH);
    }

    bool RV64ICodePoint_Funct3_SB(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(S, RV64I_SB);
    }

    //
    bool RV64ICodePoint_Funct12_ECALL(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_ECALL);
    }

    bool RV64ICodePoint_Funct12_EBREAK(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_EBREAK);
    }

    bool RV64ICodePoint_Funct12_MRET(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_MRET);
    }

    bool RV64ICodePoint_Funct12_SRET(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_SRET);
    }

    bool RV64ICodePoint_Funct12_WFI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64I_DECINSN(I, RV64I_WFI);
    }

    // TODO ... Add RV64I instruction codepoints here ...
}


namespace Jasse {

#define __RV64I_UNIQUE_CODEGROUP(name) \
    RV64IUniqueCodegroup_##name

#define __RV64I_UNIQUE_CODEGROUP_DEFINE(name, function) \
    __RV64I_UNIQUE_CODEGROUP(name)::__RV64I_UNIQUE_CODEGROUP(name)() \
        : RV64IUniqueCodegroup (&function) \
    { } \

#define __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(name, function) \
    bool function(insnraw_t insnraw, RVInstruction& insn); \
    __RV64I_UNIQUE_CODEGROUP(name)::__RV64I_UNIQUE_CODEGROUP(name)() \
        : RV64IUniqueCodegroup (&function) \
    { } \
    bool function(insnraw_t insnraw, RVInstruction& insn) \

#define __RV64I_UNIQUE_CODEGROUP_DECLARE(name) \
    class __RV64I_UNIQUE_CODEGROUP(name) : public RV64IUniqueCodegroup { \
    public: \
        __RV64I_UNIQUE_CODEGROUP(name)(); \
    }; \


#define __RV64I_FUNCT3_CODEGROUP(name) \
    RV64IFunct3Codegroup_##name

#define __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(name) \
    __RV64I_FUNCT3_CODEGROUP(name)::__RV64I_FUNCT3_CODEGROUP(name)()

#define __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(name) \
    __RV64I_FUNCT3_CODEGROUP(name)::~__RV64I_FUNCT3_CODEGROUP(name)()


#define __RV64I_FUNCT3_CODEGROUP_DECLARE(name) \
    class __RV64I_FUNCT3_CODEGROUP(name) : public RV64IFunct3Codegroup { \
    public: \
        __RV64I_FUNCT3_CODEGROUP(name)(); \
        ~__RV64I_FUNCT3_CODEGROUP(name)(); \
    }; \


    // Codepoint declarations
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_32)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_IMM)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(OP_IMM_32)

    __RV64I_FUNCT3_CODEGROUP_DECLARE(BRANCH)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(LOAD)
    __RV64I_FUNCT3_CODEGROUP_DECLARE(STORE)

    __RV64I_UNIQUE_CODEGROUP_DECLARE(MISC_MEM)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(SYSTEM)

    __RV64I_UNIQUE_CODEGROUP_DECLARE(LUI)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(AUIPC)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(JAL)
    __RV64I_UNIQUE_CODEGROUP_DECLARE(JALR)
};


// Implementation of: class RV64IUniqueCodegroup
namespace Jasse {
    RV64IUniqueCodegroup::RV64IUniqueCodegroup(RV64ICodepoint codepoint)
        : codepoint (codepoint)
    { }

    RV64IUniqueCodegroup::~RV64IUniqueCodegroup()
    { }

    bool RV64IUniqueCodegroup::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        return (*codepoint)(insnraw, insn);
    }
}


// Implementation of: class RV64IFunct3Codegroup
namespace Jasse {
    RV64IFunct3Codegroup::RV64IFunct3Codegroup()
        : codepoints(new RV64ICodepoint[8]())
    { }

    RV64IFunct3Codegroup::~RV64IFunct3Codegroup()
    { 
        delete[] codepoints;
    }

    bool RV64IFunct3Codegroup::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        int funct3 = GET_OPERAND(insnraw, RV64I_FUNCT3_MASK, RV64I_FUNCT3_OFFSET);

        if (codepoints[funct3])
            return (*(codepoints[funct3]))(insnraw, insn);
        else
            return false;
    }

    inline void RV64IFunct3Codegroup::Define(int funct3, RV64ICodepoint codepoint)
    {
        codepoints[funct3] = codepoint;
    }
}

// codegroups
namespace Jasse {
    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_IMM)
    {
        Define(RV64I_FUNCT3_ADDI,           &RV64ICodePoint_Funct3_ADDI);
        Define(RV64I_FUNCT3_SLTI,           &RV64ICodePoint_Funct3_SLTI);
        Define(RV64I_FUNCT3_SLTIU,          &RV64ICodePoint_Funct3_SLTIU);
        Define(RV64I_FUNCT3_ANDI,           &RV64ICodePoint_Funct3_ANDI);
        Define(RV64I_FUNCT3_ORI,            &RV64ICodePoint_Funct3_ORI);
        Define(RV64I_FUNCT3_XORI,           &RV64ICodePoint_Funct3_XORI);
        Define(RV64I_FUNCT3_SLLI,           &RV64ICodePoint_Funct3_SLLI);
        Define(RV64I_FUNCT3_SRLI__SRAI,     &RV64ICodePoint_Funct3_SRLI__SRAI);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_IMM)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_IMM_32)
    {
        Define(RV64I_FUNCT3_ADDIW,          &RV64ICodePoint_Funct3_ADDIW);
        Define(RV64I_FUNCT3_SLLIW,          &RV64ICodePoint_Funct3_SLLIW);
        Define(RV64I_FUNCT3_SRLIW__SRAIW,   &RV64ICodePoint_Funct3_SRLIW__SRAIW);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_IMM_32)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP)
    {
        Define(RV64I_FUNCT3_ADD__SUB,       &RV64ICodePoint_Funct3_ADD__SUB);
        Define(RV64I_FUNCT3_SLT,            &RV64ICodePoint_Funct3_SLT);
        Define(RV64I_FUNCT3_SLTU,           &RV64ICodePoint_Funct3_SLTU);
        Define(RV64I_FUNCT3_AND,            &RV64ICodePoint_Funct3_AND);
        Define(RV64I_FUNCT3_OR,             &RV64ICodePoint_Funct3_OR);
        Define(RV64I_FUNCT3_XOR,            &RV64ICodePoint_Funct3_XOR);
        Define(RV64I_FUNCT3_SLL,            &RV64ICodePoint_Funct3_SLL);
        Define(RV64I_FUNCT3_SRL__SRA,       &RV64ICodePoint_Funct3_SRL__SRA);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(OP_32)
    {
        Define(RV64I_FUNCT3_ADDW__SUBW,     &RV64ICodePoint_Funct3_ADDW__SUBW);
        Define(RV64I_FUNCT3_SLLW,           &RV64ICodePoint_Funct3_SLLW);
        Define(RV64I_FUNCT3_SRLW__SRAW,     &RV64ICodePoint_Funct3_SRLW__SRAW);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(OP_32)
    { }

    // 
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(BRANCH)
    {
        Define(RV64I_FUNCT3_BEQ,            &RV64ICodePoint_Funct3_BEQ);
        Define(RV64I_FUNCT3_BNE,            &RV64ICodePoint_Funct3_BNE);
        Define(RV64I_FUNCT3_BLT,            &RV64ICodePoint_Funct3_BLT);
        Define(RV64I_FUNCT3_BLTU,           &RV64ICodePoint_Funct3_BLTU);
        Define(RV64I_FUNCT3_BGE,            &RV64ICodePoint_Funct3_BGE);
        Define(RV64I_FUNCT3_BGEU,           &RV64ICodePoint_Funct3_BGEU);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(BRANCH)
    { }

    //
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(LOAD)
    {
        Define(RV64I_FUNCT3_LD,             &RV64ICodePoint_Funct3_LD);
        Define(RV64I_FUNCT3_LW,             &RV64ICodePoint_Funct3_LW);
        Define(RV64I_FUNCT3_LH,             &RV64ICodePoint_Funct3_LH);
        Define(RV64I_FUNCT3_LB,             &RV64ICodePoint_Funct3_LB);
        Define(RV64I_FUNCT3_LWU,            &RV64ICodePoint_Funct3_LWU);
        Define(RV64I_FUNCT3_LHU,            &RV64ICodePoint_Funct3_LHU);
        Define(RV64I_FUNCT3_LBU,            &RV64ICodePoint_Funct3_LBU);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(LOAD)
    { }

    // 
    __RV64I_FUNCT3_CODEGROUP_CONSTRUCTOR(STORE)
    {
        Define(RV64I_FUNCT3_SD,             &RV64ICodePoint_Funct3_SD);
        Define(RV64I_FUNCT3_SW,             &RV64ICodePoint_Funct3_SW);
        Define(RV64I_FUNCT3_SH,             &RV64ICodePoint_Funct3_SH);
        Define(RV64I_FUNCT3_SB,             &RV64ICodePoint_Funct3_SB);
    }

    __RV64I_FUNCT3_CODEGROUP_DECONSTRUCTOR(STORE)
    { }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(MISC_MEM, RV64ICodeGroup_Unique_MISC_MEM)
    {
        if (GET_STD_OPERAND(insnraw, RV_OPERAND_RD) != 0)
            return false;

        if (GET_STD_OPERAND(insnraw, RV_OPERAND_RS1) != 0)
            return false;

        if (GET_STD_OPERAND(insnraw, RV64I_FUNCT3) != RV64I_FUNCT3_FENCE)
            return false;

        return RV64I_DECINSN(I, RV64I_FENCE);
    }

    // 
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(SYSTEM, RV64ICodeGroup_Unique_SYSTEM)
    {
        if (GET_STD_OPERAND(insnraw, RV_OPERAND_RD) != 0)
            return false;

        if (GET_STD_OPERAND(insnraw, RV_OPERAND_RS1) != 0)
            return false;

        if (GET_STD_OPERAND(insnraw, RV64I_FUNCT3) != 0)
            return false;

        switch (GET_STD_OPERAND(insnraw, RV64I_FUNCT12))
        {
            case RV64I_FUNCT12_ECALL:
                return RV64ICodePoint_Funct12_ECALL(insnraw, insn);

            case RV64I_FUNCT12_EBREAK:
                return RV64ICodePoint_Funct12_EBREAK(insnraw, insn);

            case RV64I_FUNCT12_MRET:
                return RV64ICodePoint_Funct12_MRET(insnraw, insn);

            case RV64I_FUNCT12_SRET:
                return RV64ICodePoint_Funct12_SRET(insnraw, insn);

            case RV64I_FUNCT12_WFI:
                return RV64ICodePoint_Funct12_WFI(insnraw, insn);

            default:
                return false;
        }
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(LUI, RV64ICodeGroup_Unique_LUI)
    {
        return RV64I_DECINSN(U, RV64I_LUI);
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(AUIPC, RV64ICodeGroup_Unique_AUIPC)
    {
        return RV64I_DECINSN(U, RV64I_AUIPC);
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(JAL, RV64ICodeGroup_Unique_JAL)
    {
        return RV64I_DECINSN(J, RV64I_JAL);
    }

    //
    __RV64I_UNIQUE_CODEGROUP_DEFINE_FUNC(JALR, RV64ICodeGroup_Unique_JALR)
    {
        if (GET_STD_OPERAND(insnraw, RV64I_FUNCT3) == RV64I_FUNCT3_JALR)
            return RV64I_DECINSN(I, RV64I_JALR);
        else
            return false;
    }
}

// Implementation of: class RV64IDecoder
namespace Jasse {
    /*
    RV64IFunct3Codegroup**  codegroups; // 32 (2 ^ (7 - 2))
    */

    RV64IDecoder::RV64IDecoder()
        : RVDecoder     ("I", "RV64I")
        , codegroups    (new RV64ICodegroup*[32]())
    { 
        codegroups[RV_OPCODE_OP         >> 2]   = new __RV64I_FUNCT3_CODEGROUP(OP);
        codegroups[RV_OPCODE_OP_32      >> 2]   = new __RV64I_FUNCT3_CODEGROUP(OP_32);
        codegroups[RV_OPCODE_OP_IMM     >> 2]   = new __RV64I_FUNCT3_CODEGROUP(OP_IMM);
        codegroups[RV_OPCODE_OP_IMM_32  >> 2]   = new __RV64I_FUNCT3_CODEGROUP(OP_IMM_32);

        codegroups[RV_OPCODE_BRANCH     >> 2]   = new __RV64I_FUNCT3_CODEGROUP(BRANCH);
        codegroups[RV_OPCODE_LOAD       >> 2]   = new __RV64I_FUNCT3_CODEGROUP(LOAD);
        codegroups[RV_OPCODE_STORE      >> 2]   = new __RV64I_FUNCT3_CODEGROUP(STORE);

        codegroups[RV_OPCODE_MISC_MEM   >> 2]   = new __RV64I_UNIQUE_CODEGROUP(MISC_MEM);
        codegroups[RV_OPCODE_SYSTEM     >> 2]   = new __RV64I_UNIQUE_CODEGROUP(SYSTEM);

        codegroups[RV_OPCODE_LUI        >> 2]   = new __RV64I_UNIQUE_CODEGROUP(LUI);
        codegroups[RV_OPCODE_AUIPC      >> 2]   = new __RV64I_UNIQUE_CODEGROUP(AUIPC);
        codegroups[RV_OPCODE_JAL        >> 2]   = new __RV64I_UNIQUE_CODEGROUP(JAL);
        codegroups[RV_OPCODE_JALR       >> 2]   = new __RV64I_UNIQUE_CODEGROUP(JALR);
    }

    RV64IDecoder::~RV64IDecoder()
    {
        for (int i = 0; i < 32; i++)
            if (codegroups[i])
                delete codegroups[i];

        delete[] codegroups;
    }

    bool RV64IDecoder::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        if ((insnraw & 0x00000003U) != 0x03)
            return false;

        RV64ICodegroup* codegroup 
            = codegroups[GET_STD_OPERAND(insnraw, RV_OPCODE) >> 2];

        if (!codegroup)
            return false;

        return codegroup->Decode(insnraw, insn);
    }

    const RVCodepointCollection& RV64IDecoder::GetAllCodepoints() const
    {
        return ALL_OF_RV64I;
    }
}