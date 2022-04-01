#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Miscs and utilities
//

#include "riscv.hpp"


#define IMM64_SEXT_W(imm) \
    (imm.imm64 = SEXT_W(imm.imm32), imm)



namespace Jasse {
    // RISC-V Instruction Immediate Decoder
    typedef imm_t       (*RVImmediateDecoder)(insnraw_t insnraw);

    imm_t DecodeRV32ImmediateTypeR(insnraw_t insnraw) noexcept;
    imm_t DecodeRV64ImmediateTypeR(insnraw_t insnraw) noexcept;
    imm_t DecodeRV32ImmediateTypeI(insnraw_t insnraw) noexcept;
    imm_t DecodeRV64ImmediateTypeI(insnraw_t insnraw) noexcept;
    imm_t DecodeRV32ImmediateTypeS(insnraw_t insnraw) noexcept;
    imm_t DecodeRV64ImmediateTypeS(insnraw_t insnraw) noexcept;
    imm_t DecodeRV32ImmediateTypeB(insnraw_t insnraw) noexcept;
    imm_t DecodeRV64ImmediateTypeB(insnraw_t insnraw) noexcept;
    imm_t DecodeRV32ImmediateTypeU(insnraw_t insnraw) noexcept;
    imm_t DecodeRV64ImmediateTypeU(insnraw_t insnraw) noexcept;
    imm_t DecodeRV32ImmediateTypeJ(insnraw_t insnraw) noexcept;
    imm_t DecodeRV64ImmediateTypeJ(insnraw_t insnraw) noexcept;

    // RISC-V Instruction Normal Textualizer
    std::string TextualizeRVTypeR(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeI(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeS(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeB(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeU(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeJ(const RVInstruction& insn) noexcept;
    std::string TextualizeRVZeroOperand(const RVInstruction& insn) noexcept;

    // RISC-V 64 Normal Instruction Form Decoder
    void DecodeNormalRV64TypeR(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV64TypeI(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV64TypeS(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV64TypeB(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV64TypeU(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV64TypeJ(insnraw_t insnraw, RVInstruction& insn) noexcept;

    // RISC-V 32 Normal Instruction Form Decoder
    void DecodeNormalRV32TypeR(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV32TypeI(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV32TypeS(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV32TypeB(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV32TypeU(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRV32TypeJ(insnraw_t insnraw, RVInstruction& insn) noexcept;
}


// Implementation of Immediate Decoders
namespace Jasse {
    imm_t DecodeRV32ImmediateTypeR(insnraw_t insnraw) noexcept
    {
        imm_t imm;
        imm.imm32 = 0;

        return imm;
    }

    imm_t DecodeRV64ImmediateTypeR(insnraw_t insnraw) noexcept
    {
        imm_t imm;
        imm.imm64 = 0;

        return imm;
    }

    imm_t DecodeRV32ImmediateTypeI(insnraw_t insnraw) noexcept
    {
        imm_t imm;
        imm.imm32 = 0;

        imm.imm32 |= ((insnraw & 0xFFF00000U) >> 20);
        imm.imm32 |= ((insnraw & 0x80000000U) ? 0xFFFFF000U : 0);

        return imm;
    }

    imm_t DecodeRV64ImmediateTypeI(insnraw_t insnraw) noexcept
    {
        imm_t imm = DecodeRV32ImmediateTypeI(insnraw);

        return IMM64_SEXT_W(imm);
    }

    imm_t DecodeRV32ImmediateTypeS(insnraw_t insnraw) noexcept
    {
        imm_t imm;
        imm.imm32 = 0;

        imm.imm32 |= ((insnraw & 0x00000F80U) >> 7);
        imm.imm32 |= ((insnraw & 0xFE000000U) >> 20);
        imm.imm32 |= ((insnraw & 0x80000000U) ? 0xFFFFF000U : 0);

        return imm;
    }

    imm_t DecodeRV64ImmediateTypeS(insnraw_t insnraw) noexcept
    {
        imm_t imm = DecodeRV32ImmediateTypeS(insnraw);

        return IMM64_SEXT_W(imm);
    }

    imm_t DecodeRV32ImmediateTypeB(insnraw_t insnraw) noexcept
    {
        imm_t imm;
        imm.imm32 = 0;

        imm.imm32 |= ((insnraw & 0x00000F00U) >> 7);
        imm.imm32 |= ((insnraw & 0x00000080U) << 4);
        imm.imm32 |= ((insnraw & 0x7E000000U) >> 20);
        imm.imm32 |= ((insnraw & 0x80000000U) ? 0xFFFFF000U : 0);

        return imm;
    }

    imm_t DecodeRV64ImmediateTypeB(insnraw_t insnraw) noexcept
    {
        imm_t imm = DecodeRV32ImmediateTypeB(insnraw);

        return IMM64_SEXT_W(imm);
    }

    imm_t DecodeRV32ImmediateTypeU(insnraw_t insnraw) noexcept
    {
        imm_t imm;
        imm.imm32 = 0;

        imm.imm32 |= ((insnraw & 0xFFFFF000));

        return imm;
    }

    imm_t DecodeRV64ImmediateTypeU(insnraw_t insnraw) noexcept
    {
        imm_t imm = DecodeRV32ImmediateTypeU(insnraw);

        return IMM64_SEXT_W(imm);
    }

    imm_t DecodeRV32ImmediateTypeJ(insnraw_t insnraw) noexcept
    {
        imm_t imm;
        imm.imm32 = 0;

        imm.imm32 |= ((insnraw & 0x7FE00000U) >> 20);
        imm.imm32 |= ((insnraw & 0x00100000U) >> 9);
        imm.imm32 |= ((insnraw & 0x000FF000U));
        imm.imm32 |= ((insnraw & 0x80000000U) ? 0xFFF00000U : 0);

        return imm;
    }

    imm_t DecodeRV64ImmediateTypeJ(insnraw_t insnraw) noexcept
    {
        imm_t imm = DecodeRV32ImmediateTypeJ(insnraw);

        return IMM64_SEXT_W(imm);
    }
}


// Implementation of Instruction Textualizers
namespace Jasse {
    std::string TextualizeRVGR(int GR) noexcept
    {
        std::ostringstream oss;

        oss << "x";
        oss << GR;

        return oss.str();
    }

    std::string TextualizeRVGR_ABI(int GR) noexcept
    {
        // TODO

        return std::string();
    }

    std::string TextualizeRVTypeR(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRD())     << ", ";
        oss << TextualizeRVGR(insn.GetRS1())    << ", ";
        oss << TextualizeRVGR(insn.GetRS2());

        return oss.str();
    }

    std::string TextualizeRVTypeI(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRD())     << ", ";
        oss << TextualizeRVGR(insn.GetRS1())    << ", ";
        oss << "0x" << std::hex << std::setw(3) << std::setfill('0') << (insn.GetImmediate().imm32 & 0xFFF);
        // Note: Only display 12 bits of immediate value here.
        //       Because higher bits are only produced by sign-extension. The same below.

        return oss.str();
    }

    std::string TextualizeRVTypeS(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRS1())    << ", ";
        oss << TextualizeRVGR(insn.GetRS2())    << ", ";
        oss << "0x" << std::hex << std::setw(3) << std::setfill('0') << (insn.GetImmediate().imm32 & 0xFFF);

        return oss.str();
    }

    std::string TextualizeRVTypeB(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRS1())    << ", ";
        oss << TextualizeRVGR(insn.GetRS2())    << ", ";
        oss << "0x" << std::hex << std::setw(4) << std::setfill('0') << (insn.GetImmediate().imm32 & 0xFFFF);

        return oss.str();
    }

    std::string TextualizeRVTypeU(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRD())     << ", ";
        oss << "0x" << std::hex << std::setw(8) << std::setfill('0') << (insn.GetImmediate().imm32);

        return oss.str();
    }

    std::string TextualizeRVTypeJ(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRD())     << ", ";
        oss << "0x" << std::hex << std::setw(6) << std::setfill('0') << (insn.GetImmediate().imm32 & 0x00FFFFFF);

        return oss.str();
    }

    std::string TextualizeRVZeroOperand(const RVInstruction& insn) noexcept
    {
        return insn.GetName();
    }
}


// Implementation of Normal Form Decoders
namespace Jasse {
    // RV64
    inline void DecodeNormalRV64TypeR(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
    }

    inline void DecodeNormalRV64TypeI(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetImmediate(DecodeRV64ImmediateTypeI(insnraw));
    }

    inline void DecodeNormalRV64TypeS(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
        insn.SetImmediate(DecodeRV64ImmediateTypeS(insnraw));
    }

    inline void DecodeNormalRV64TypeB(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
        insn.SetImmediate(DecodeRV64ImmediateTypeB(insnraw));
    }

    inline void DecodeNormalRV64TypeU(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetImmediate(DecodeRV64ImmediateTypeU(insnraw));
    }

    inline void DecodeNormalRV64TypeJ(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetImmediate(DecodeRV64ImmediateTypeJ(insnraw));
    }

    // RV32
    inline void DecodeNormalRV32TypeR(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
    }

    inline void DecodeNormalRV32TypeI(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetImmediate(DecodeRV32ImmediateTypeI(insnraw));
    }

    inline void DecodeNormalRV32TypeS(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
        insn.SetImmediate(DecodeRV32ImmediateTypeS(insnraw));
    }

    inline void DecodeNormalRV32TypeB(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
        insn.SetImmediate(DecodeRV32ImmediateTypeB(insnraw));
    }

    inline void DecodeNormalRV32TypeU(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetImmediate(DecodeRV32ImmediateTypeU(insnraw));
    }

    inline void DecodeNormalRV32TypeJ(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetImmediate(DecodeRV32ImmediateTypeJ(insnraw));
    }
}

