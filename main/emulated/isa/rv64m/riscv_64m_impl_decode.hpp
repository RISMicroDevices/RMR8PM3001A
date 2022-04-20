#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, decoding infrastructure implementations
//

#include "riscv_64m_def_base.hpp"
#include "riscv_64m_def_export.hpp"
#include "riscv_64m_def_decode.hpp"
#include "riscvmisc.hpp"


//
#define RV64M_DECINSN(funct7, T, codepoint) \
    (GET_STD_OPERAND(insnraw, RV64M_FUNCT7) == funct7 \
        ? (DecodeNormalRVType##T(insnraw, insn), insn.SetCodepoint(&codepoint), true) \
        : false) \

// codepoints - decoder facilities
namespace Jasse {

    // 
    bool RV64MCodePoint_Funct3_MUL(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_MUL, R, RV64M_MUL);
    }

    bool RV64MCodePoint_Funct3_MULH(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_MULH, R, RV64M_MULH);
    }

    bool RV64MCodePoint_Funct3_MULHSU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_MULHSU, R, RV64M_MULHSU);
    }

    bool RV64MCodePoint_Funct3_MULHU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_MULHU, R, RV64M_MULHU);
    }

    bool RV64MCodePoint_Funct3_DIV(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_DIV, R, RV64M_DIV);
    }

    bool RV64MCodePoint_Funct3_DIVU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_DIVU, R, RV64M_DIVU);
    }

    bool RV64MCodePoint_Funct3_REM(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_REM, R, RV64M_REM);
    }

    bool RV64MCodePoint_Funct3_REMU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_REMU, R, RV64M_REMU);
    }

    bool RV64MCodePoint_Funct3_MULW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_MULW, R, RV64M_MULW);
    }

    bool RV64MCodePoint_Funct3_DIVW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_DIVW, R, RV64M_DIVW);
    }

    bool RV64MCodePoint_Funct3_DIVUW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_DIVUW, R, RV64M_DIVUW);
    }

    bool RV64MCodePoint_Funct3_REMW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_REMW, R, RV64M_REMW);
    }

    bool RV64MCodePoint_Funct3_REMUW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_REMUW, R, RV64M_REMUW);
    }
}


// Implementation of: RV64MDecoder
namespace Jasse {
    /*
    RV64MCodepoint*     codepoints64;  // 8
    RV64MCodepoint*     codepoints32;  // 8
    */

    RV64MDecoder::RV64MDecoder()
        : RVDecoder     ("M", "RV64M")
        , codepoints64  (new RV64MCodepoint[8]())
        , codepoints32  (new RV64MCodepoint[8]())
    {
        codepoints64[RV32M_FUNCT3_MUL]      = &RV64MCodePoint_Funct3_MUL;
        codepoints64[RV32M_FUNCT3_MULH]     = &RV64MCodePoint_Funct3_MULH;
        codepoints64[RV32M_FUNCT3_MULHSU]   = &RV64MCodePoint_Funct3_MULHSU;
        codepoints64[RV32M_FUNCT3_MULHU]    = &RV64MCodePoint_Funct3_MULHU;
        codepoints64[RV32M_FUNCT3_DIV]      = &RV64MCodePoint_Funct3_DIV;
        codepoints64[RV32M_FUNCT3_DIVU]     = &RV64MCodePoint_Funct3_DIVU;
        codepoints64[RV32M_FUNCT3_REM]      = &RV64MCodePoint_Funct3_REM;
        codepoints64[RV32M_FUNCT3_REMU]     = &RV64MCodePoint_Funct3_REMU;

        codepoints32[RV64M_FUNCT3_MULW]     = &RV64MCodePoint_Funct3_MULW;
        codepoints32[RV64M_FUNCT3_DIVW]     = &RV64MCodePoint_Funct3_DIVW;
        codepoints32[RV64M_FUNCT3_DIVUW]    = &RV64MCodePoint_Funct3_DIVUW;
        codepoints32[RV64M_FUNCT3_REMW]     = &RV64MCodePoint_Funct3_REMW;
        codepoints32[RV64M_FUNCT3_REMUW]    = &RV64MCodePoint_Funct3_REMUW;
    }

    RV64MDecoder::~RV64MDecoder()
    {
        delete[] codepoints64;
        delete[] codepoints32;
    }

    bool RV64MDecoder::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        int opcode = GET_STD_OPERAND(insnraw, RV_OPCODE);

        RV64MCodepoint* codepoints;

        if (opcode == RV_OPCODE_OP)
            codepoints = codepoints64;
        else if (opcode == RV_OPCODE_OP_32)
            codepoints = codepoints32;
        else
            return false;

        RV64MCodepoint codepoint = codepoints[GET_STD_OPERAND(insnraw, RV64M_FUNCT3)];

        if (!codepoint)
            return false;

        return codepoint(insnraw, insn);
    }

    const RVCodepointCollection& RV64MDecoder::GetAllCodepoints() const
    {
        return ALL_OF_RV64M;
    }
}
