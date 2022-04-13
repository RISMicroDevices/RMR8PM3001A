#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Miscs and utilities
//

#include "riscv.hpp"


#define ENCODE_STD_OPERAND(val, name) \
    ((val & (name##_MASK >> name##_OFFSET)) << name##_OFFSET)


#define ENCODE_RVTYPE_R(opcode, funct3, rd, rs1, rs2, funct7) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(funct3 , RV_FUNCT3) \
    | ENCODE_STD_OPERAND(rd     , RV_OPERAND_RD) \
    | ENCODE_STD_OPERAND(rs1    , RV_OPERAND_RS1) \
    | ENCODE_STD_OPERAND(rs2    , RV_OPERAND_RS2) \
    | ENCODE_STD_OPERAND(funct7 , RV_FUNCT7))

#define ENCODE_RVTYPE_I(opcode, funct3, rd, rs1, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(funct3 , RV_FUNCT3) \
    | ENCODE_STD_OPERAND(rd     , RV_OPERAND_RD) \
    | ENCODE_STD_OPERAND(rs1    , RV_OPERAND_RS1) \
    | ((imm & 0x00000FFFU) << 20))

#define ENCODE_RVTYPE_S(opcode, funct3, rs1, rs2, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(funct3 , RV_FUNCT3) \
    | ENCODE_STD_OPERAND(rs1    , RV_OPERAND_RS1) \
    | ENCODE_STD_OPERAND(rs2    , RV_OPERAND_RS2) \
    | ((imm & 0x0000001FU) << 7) \
    | ((imm & 0x00000FE0U) << 20))

#define ENCODE_RVTYPE_B(opcode, funct3, rs1, rs2, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(funct3 , RV_FUNCT3) \
    | ENCODE_STD_OPERAND(rs1    , RV_OPERAND_RS1) \
    | ENCODE_STD_OPERAND(rs2    , RV_OPERAND_RS2) \
    | ((imm & 0x00000800U) >> 4) \
    | ((imm & 0x0000001EU) << 7) \
    | ((imm & 0x000007E0U) << 20) \
    | ((imm & 0x00001000U) << 19))

#define ENCODE_RVTYPE_U(opcode, rd, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(rd     , RV_OPERAND_RD) \
    | (imm & 0xFFFFF000U))

#define ENCODE_RVTYPE_J(opcode, rd, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(rd     , RV_OPERAND_RD) \
    |  (imm & 0x000FF000U) \
    | ((imm & 0x00000800U) << 9) \
    | ((imm & 0x000007FEU) << 20) \
    | ((imm & 0x00100000U) << 11))



namespace Jasse {
    // RISC-V Instruction Immediate Decoder
    typedef imm_t       (*RVImmediateDecoder)(insnraw_t insnraw);

    imm_t DecodeRVImmediateTypeR(insnraw_t insnraw) noexcept;
    imm_t DecodeRVImmediateTypeI(insnraw_t insnraw) noexcept;
    imm_t DecodeRVImmediateTypeS(insnraw_t insnraw) noexcept;
    imm_t DecodeRVImmediateTypeB(insnraw_t insnraw) noexcept;
    imm_t DecodeRVImmediateTypeU(insnraw_t insnraw) noexcept;
    imm_t DecodeRVImmediateTypeJ(insnraw_t insnraw) noexcept;

    // RISC-V Instruction Normal Textualizer
    std::string TextualizeRVTypeR(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeI(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeS(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeB(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeU(const RVInstruction& insn) noexcept;
    std::string TextualizeRVTypeJ(const RVInstruction& insn) noexcept;
    std::string TextualizeRVZeroOperand(const RVInstruction& insn) noexcept;

    // RISC-V Normal Instruction Form Decoder
    void DecodeNormalRVTypeR(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRVTypeI(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRVTypeS(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRVTypeB(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRVTypeU(insnraw_t insnraw, RVInstruction& insn) noexcept;
    void DecodeNormalRVTypeJ(insnraw_t insnraw, RVInstruction& insn) noexcept;
}


// Implementation of Immediate Decoders
namespace Jasse {
    imm_t DecodeRVImmediateTypeR(insnraw_t insnraw) noexcept
    {
        return 0;
    }

    imm_t DecodeRVImmediateTypeI(insnraw_t insnraw) noexcept
    {
        imm_t imm;

        imm  = ((insnraw & 0xFFF00000U) >> 20);
        imm |= ((insnraw & 0x80000000U) ? 0xFFFFF000U : 0);

        return imm;
    }

    imm_t DecodeRVImmediateTypeS(insnraw_t insnraw) noexcept
    {
        imm_t imm;

        imm  = ((insnraw & 0x00000F80U) >> 7);
        imm |= ((insnraw & 0xFE000000U) >> 20);
        imm |= ((insnraw & 0x80000000U) ? 0xFFFFF000U : 0);

        return imm;
    }

    imm_t DecodeRVImmediateTypeB(insnraw_t insnraw) noexcept
    {
        imm_t imm;

        imm  = ((insnraw & 0x00000F00U) >> 7);
        imm |= ((insnraw & 0x00000080U) << 4);
        imm |= ((insnraw & 0x7E000000U) >> 20);
        imm |= ((insnraw & 0x80000000U) ? 0xFFFFF000U : 0);

        return imm;
    }

    imm_t DecodeRVImmediateTypeU(insnraw_t insnraw) noexcept
    {
        imm_t imm = 0;

        imm = ((insnraw & 0xFFFFF000));

        return imm;
    }

    imm_t DecodeRVImmediateTypeJ(insnraw_t insnraw) noexcept
    {
        imm_t imm;

        imm  = ((insnraw & 0x7FE00000U) >> 20);
        imm |= ((insnraw & 0x00100000U) >> 9);
        imm |= ((insnraw & 0x000FF000U));
        imm |= ((insnraw & 0x80000000U) ? 0xFFF00000U : 0);

        return imm;
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
        oss << "0x" << std::hex << std::setw(3) << std::setfill('0') << (insn.GetImmediate() & 0xFFF);
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
        oss << "0x" << std::hex << std::setw(3) << std::setfill('0') << (insn.GetImmediate() & 0xFFF);

        return oss.str();
    }

    std::string TextualizeRVTypeB(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRS1())    << ", ";
        oss << TextualizeRVGR(insn.GetRS2())    << ", ";
        oss << "0x" << std::hex << std::setw(4) << std::setfill('0') << (insn.GetImmediate() & 0xFFFF);

        return oss.str();
    }

    std::string TextualizeRVTypeU(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRD())     << ", ";
        oss << "0x" << std::hex << std::setw(8) << std::setfill('0') << (insn.GetImmediate());

        return oss.str();
    }

    std::string TextualizeRVTypeJ(const RVInstruction& insn) noexcept
    {
        std::ostringstream oss;

        oss << insn.GetName()                   << " \t";
        oss << TextualizeRVGR(insn.GetRD())     << ", ";
        oss << "0x" << std::hex << std::setw(6) << std::setfill('0') << (insn.GetImmediate() & 0x00FFFFFF);

        return oss.str();
    }

    std::string TextualizeRVZeroOperand(const RVInstruction& insn) noexcept
    {
        return insn.GetName();
    }
}


// Implementation of Normal Form Decoders
namespace Jasse {

    inline void DecodeNormalRVTypeR(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
    }

    inline void DecodeNormalRVTypeI(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetImmediate(DecodeRVImmediateTypeI(insnraw));
    }

    inline void DecodeNormalRVTypeS(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
        insn.SetImmediate(DecodeRVImmediateTypeS(insnraw));
    }

    inline void DecodeNormalRVTypeB(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRS1(GET_STD_OPERAND(insnraw, RV_OPERAND_RS1));
        insn.SetRS2(GET_STD_OPERAND(insnraw, RV_OPERAND_RS2));
        insn.SetImmediate(DecodeRVImmediateTypeB(insnraw));
    }

    inline void DecodeNormalRVTypeU(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetImmediate(DecodeRVImmediateTypeU(insnraw));
    }

    inline void DecodeNormalRVTypeJ(insnraw_t insnraw, RVInstruction& insn) noexcept
    {
        insn.SetRD (GET_STD_OPERAND(insnraw, RV_OPERAND_RD));
        insn.SetImmediate(DecodeRVImmediateTypeJ(insnraw));
    }
}

