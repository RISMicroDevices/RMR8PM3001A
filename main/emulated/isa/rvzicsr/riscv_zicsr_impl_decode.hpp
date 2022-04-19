#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR"
//

#include "riscvmisc.hpp"
#include "riscv_zicsr_def_base.hpp"
#include "riscv_zicsr_def_export.hpp"
#include "riscv_zicsr_def_decode.hpp"

// codepoints - decoder facilities
namespace Jasse {

#define RVZICSR_DECINSN(T, codepoint) \
    (DecodeNormalRVType##T(insnraw, insn), insn.SetCodepoint(&codepoint), true)
    
    //
    bool RVZicsrCodePoint_Funct3_CSRRW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRW);
    }

    bool RVZicsrCodePoint_Funct3_CSRRS(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRS);
    }

    bool RVZicsrCodePoint_Funct3_CSRRC(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRC);
    }

    bool RVZicsrCodePoint_Funct3_CSRRWI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRWI);
    }

    bool RVZicsrCodePoint_Funct3_CSRRSI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRSI);
    }

    bool RVZicsrCodePoint_Funct3_CSRRCI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRCI);
    }
}


// Implementation of: class RVZicsrDecoder
namespace Jasse {
    /*
    RVZicsrCodepoint*   codepoints;
    */

    RVZicsrDecoder::RVZicsrDecoder()
        : RVDecoder ("Zicsr", "RVZicsr")
        , codepoints(new RVZicsrCodepoint[8]())
    {
        codepoints[RVZICSR_FUNCT3_CSRRW]    = &RVZicsrCodePoint_Funct3_CSRRW;
        codepoints[RVZICSR_FUNCT3_CSRRS]    = &RVZicsrCodePoint_Funct3_CSRRS;
        codepoints[RVZICSR_FUNCT3_CSRRC]    = &RVZicsrCodePoint_Funct3_CSRRC;

        codepoints[RVZICSR_FUNCT3_CSRRWI]   = &RVZicsrCodePoint_Funct3_CSRRWI;
        codepoints[RVZICSR_FUNCT3_CSRRSI]   = &RVZicsrCodePoint_Funct3_CSRRSI;
        codepoints[RVZICSR_FUNCT3_CSRRCI]   = &RVZicsrCodePoint_Funct3_CSRRCI;
    }

    RVZicsrDecoder::~RVZicsrDecoder()
    {
        delete[] codepoints;
    }

    bool RVZicsrDecoder::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        if (GET_STD_OPERAND(insnraw, RV_OPCODE) != RV_OPCODE_SYSTEM)
            return false;

        RVZicsrCodepoint codepoint = codepoints[GET_STD_OPERAND(insnraw, RVZICSR_FUNCT3)];

        if (!codepoint)
            return false;

        return codepoint(insnraw, insn);
    }

    const RVCodepointCollection& RVZicsrDecoder::GetAllCodepoints() const
    {
        return ALL_OF_RVZicsr;
    }
}