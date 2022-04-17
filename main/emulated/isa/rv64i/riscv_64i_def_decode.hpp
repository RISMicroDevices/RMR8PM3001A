#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, decoding infrastructure definitions
//

#include "riscv.hpp"

// codepoints - decoder facilities
namespace Jasse {

    //
    bool RV64ICodePoint_Funct3_ADDI (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SLTI (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SLTIU(insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_ANDI (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_ORI  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_XORI (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SLLI (insnraw_t insnraw, RVInstruction& insn);
#define  RV64ICodePoint_Funct3_SRLI         RV64ICodePoint_Funct3_SRLI__SRAI
#define  RV64ICodePoint_Funct3_SRAI         RV64ICodePoint_Funct3_SRLI__SRAI
    bool RV64ICodePoint_Funct3_SRLI__SRAI(insnraw_t insnraw, RVInstruction& insn);

    //
    bool RV64ICodePoint_Funct3_ADDIW(insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SLLIW(insnraw_t insnraw, RVInstruction& insn);
#define  RV64ICodePoint_Funct3_SRLIW        RV64ICodePoint_Funct3_SRLIW__SRAIW
#define  RV64ICodePoint_Funct3_SRAIW        RV64ICodePoint_Funct3_SRLIW__SRAIW
    bool RV64ICodePoint_Funct3_SRLIW__SRAIW(insnraw_t insnraw, RVInstruction& insn);

    //
#define  RV64ICodePoint_Funct3_ADD          RV64ICodePoint_Funct3_ADD__SUB
#define  RV64ICodePoint_Funct3_SUB          RV64ICodePoint_Funct3_ADD__SUB
    bool RV64ICodePoint_Funct3_ADD__SUB(insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SLT  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SLTU (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_AND  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_OR   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_XOR  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SLL  (insnraw_t insnraw, RVInstruction& insn);
#define  RV64ICodePoint_Funct3_SRL          RV64ICodePoint_Funct3_SRL__SRA
#define  RV64ICodePoint_Funct3_SRA          RV64ICodePoint_Funct3_SRL__SRA
    bool RV64ICodePoint_Funct3_SRL__SRA(insnraw_t insnraw, RVInstruction& insn);

    //
#define  RV64ICodePoint_Funct3_ADDW        RV64ICodePoint_Funct3_ADDW__SUBW
#define  RV64ICodePoint_Funct3_SUBW        RV64ICodePoint_Funct3_ADDW__SUBW
    bool RV64ICodePoint_Funct3_ADDW__SUBW(insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SLLW (insnraw_t insnraw, RVInstruction& insn);
#define  RV64ICodePoint_Funct3_SRLW         RV64ICodePoint_Funct3_SRLW__SRAW
#define  RV64ICodePoint_Funct3_SRAW         RV64ICodePoint_Funct3_SRLW__SRAW
    bool RV64ICodePoint_Funct3_SRLW__SRAW(insnraw_t insnraw, RVInstruction& insn);

    //
    bool RV64ICodePoint_Funct3_BEQ  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_BNE  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_BLT  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_BLTU (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_BGE  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_BGEU (insnraw_t insnraw, RVInstruction& insn);

    //
    bool RV64ICodePoint_Funct3_LD   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_LW   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_LH   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_LB   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_LWU  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_LHU  (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_LBU  (insnraw_t insnraw, RVInstruction& insn);

    //
    bool RV64ICodePoint_Funct3_SD   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SW   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SH   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct3_SB   (insnraw_t insnraw, RVInstruction& insn);

    //
    bool RV64ICodePoint_Funct12_ECALL(insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct12_EBREAK(insnraw_t insnraw, RVInstruction& insn);

    bool RV64ICodePoint_Funct12_MRET(insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodePoint_Funct12_SRET(insnraw_t insnraw, RVInstruction& insn);

    bool RV64ICodePoint_Funct12_WFI (insnraw_t insnraw, RVInstruction& insn);

    //
    bool RV64ICodeGroup_Unique_MISC_MEM (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodeGroup_Unique_SYSTEM   (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodeGroup_Unique_LUI      (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodeGroup_Unique_AUIPC    (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodeGroup_Unique_JAL      (insnraw_t insnraw, RVInstruction& insn);
    bool RV64ICodeGroup_Unique_JALR     (insnraw_t insnraw, RVInstruction& insn);

    // TODO ... Add RV64I instruction codepoints here ...
}

//
namespace Jasse {

    // RV64I codepoint
    typedef bool (*RV64ICodepoint)(insnraw_t insnraw, RVInstruction& insn);

    // RV64I codegroup (OPCODE base)
    class RV64ICodegroup {
    public:
        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const = 0;
    };


    // RV64I unique codegroup
    class RV64IUniqueCodegroup : public RV64ICodegroup {
    private:
        RV64ICodepoint   codepoint;
    
    public:
        RV64IUniqueCodegroup(RV64ICodepoint codepoint);
        ~RV64IUniqueCodegroup();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };

    // RV64I FUNCT3 codegroup
    class RV64IFunct3Codegroup : public RV64ICodegroup {
    private:
        RV64ICodepoint*  codepoints; // 8 (2 ^ 3)

    public:
        RV64IFunct3Codegroup();
        ~RV64IFunct3Codegroup();

        void            Define(int funct3, RV64ICodepoint codepoint);
        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };


    // RV64I Decoder
    class RV64IDecoder : public RVDecoder {
    private:
        RV64ICodegroup**    codegroups; // 32 (2 ^ (7 - 2))

    public:
        RV64IDecoder();
        ~RV64IDecoder();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;

        virtual const RVCodepointCollection&    GetAllCodepoints() const override;
    };
}
