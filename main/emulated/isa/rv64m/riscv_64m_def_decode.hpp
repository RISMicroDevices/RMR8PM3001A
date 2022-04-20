#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, decoding infrastructure definitions
//

#include "riscv.hpp"


// codepoints - decoder facilities
namespace Jasse {

    // 
    bool RV64MCodePoint_Funct3_MUL   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64MCodePoint_Funct3_MULH  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64MCodePoint_Funct3_MULHSU(insnraw_t insnraw, RVInstruction& insn);
    bool RV64MCodePoint_Funct3_MULHU (insnraw_t insnraw, RVInstruction& insn);

    bool RV64MCodePoint_Funct3_DIV (insnraw_t insnraw, RVInstruction& insn);
    bool RV64MCodePoint_Funct3_DIVU(insnraw_t insnraw, RVInstruction& insn);

    bool RV64MCodePoint_Funct3_REM (insnraw_t insnraw, RVInstruction& insn);
    bool RV64MCodePoint_Funct3_REMU(insnraw_t insnraw, RVInstruction& insn);

    bool RV64MCodePoint_Funct3_MULW(insnraw_t insnraw, RVInstruction& insn);

    bool RV64MCodePoint_Funct3_DIVW (insnraw_t insnraw, RVInstruction& insn);
    bool RV64MCodePoint_Funct3_DIVUW(insnraw_t insnraw, RVInstruction& insn);
    bool RV64MCodePoint_Funct3_REMW (insnraw_t insnraw, RVInstruction& insn);
    bool RV64MCodePoint_Funct3_REMUW(insnraw_t insnraw, RVInstruction& insn);
}

namespace Jasse {
    // RV64M codepoint
    typedef bool (*RV64MCodepoint)(insnraw_t insnraw, RVInstruction& insn);

    // RV64M decoder
    class RV64MDecoder : public RVDecoder {
    private:
        RV64MCodepoint*     codepoints64;  // 8
        RV64MCodepoint*     codepoints32;  // 8

    public:
        RV64MDecoder();
        ~RV64MDecoder();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;

        virtual const RVCodepointCollection&    GetAllCodepoints() const override;
    };
}
