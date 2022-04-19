#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR", decoding infrastructure definitions
//

#include "riscv.hpp"

// codepoints - decoder facilities
namespace Jasse {
    
    //
    bool RVZicsrCodePoint_Funct3_CSRRW(insnraw_t insnraw, RVInstruction& insn);
    bool RVZicsrCodePoint_Funct3_CSRRS(insnraw_t insnraw, RVInstruction& insn);
    bool RVZicsrCodePoint_Funct3_CSRRC(insnraw_t insnraw, RVInstruction& insn);

    bool RVZicsrCodePoint_Funct3_CSRRWI(insnraw_t insnraw, RVInstruction& insn);
    bool RVZicsrCodePoint_Funct3_CSRRSI(insnraw_t insnraw, RVInstruction& insn);
    bool RVZicsrCodePoint_Funct3_CSRRCI(insnraw_t insnraw, RVInstruction& insn);
}

namespace Jasse {
    // RVZicsr codepoint
    typedef bool (*RVZicsrCodepoint)(insnraw_t insnraw, RVInstruction& insn);

    // RVZicsr Decoder
    class RVZicsrDecoder : public RVDecoder {
    private:
        RVZicsrCodepoint*   codepoints; // 8 (2 ^ 3)

    public:
        RVZicsrDecoder();
        ~RVZicsrDecoder();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;

        virtual const RVCodepointCollection&    GetAllCodepoints() const override;
    };
}