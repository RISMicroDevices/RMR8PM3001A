#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Miscs and utilities
//

#include "riscv.hpp"


#define ENCODE_STD_OPERAND(val, name) \
    ((val & (name##_MASK >> name##_OFFSET)) << name##_OFFSET)


#define ENCODE_IMM_RVTYPE_I(imm) \
    ( (imm & 0x00000FFFU) << 20)

#define ENCODE_IMM_RVTYPE_S(imm) \
    ( ((imm & 0x0000001FU) << 7) \
    | ((imm & 0x00000FE0U) << 20)) 

#define ENCODE_IMM_RVTYPE_B(imm) \
    ( ((imm & 0x00000800U) >> 4) \
    | ((imm & 0x0000001EU) << 7) \
    | ((imm & 0x000007E0U) << 20) \
    | ((imm & 0x00001000U) << 19))

#define ENCODE_IMM_RVTYPE_U(imm) \
       (imm & 0xFFFFF000U)

#define ENCODE_IMM_RVTYPE_J(imm) \
    (  (imm & 0x000FF000U) \
    | ((imm & 0x00000800U) << 9) \
    | ((imm & 0x000007FEU) << 20) \
    | ((imm & 0x00100000U) << 11))


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
    | ENCODE_IMM_RVTYPE_I(imm))

#define ENCODE_RVTYPE_S(opcode, funct3, rs1, rs2, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(funct3 , RV_FUNCT3) \
    | ENCODE_STD_OPERAND(rs1    , RV_OPERAND_RS1) \
    | ENCODE_STD_OPERAND(rs2    , RV_OPERAND_RS2) \
    | ENCODE_IMM_RVTYPE_S(imm))

#define ENCODE_RVTYPE_B(opcode, funct3, rs1, rs2, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(funct3 , RV_FUNCT3) \
    | ENCODE_STD_OPERAND(rs1    , RV_OPERAND_RS1) \
    | ENCODE_STD_OPERAND(rs2    , RV_OPERAND_RS2) \
    | ENCODE_IMM_RVTYPE_B(imm))

#define ENCODE_RVTYPE_U(opcode, rd, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(rd     , RV_OPERAND_RD) \
    | ENCODE_IMM_RVTYPE_U(imm))

#define ENCODE_RVTYPE_J(opcode, rd, imm) \
    ( ENCODE_STD_OPERAND(opcode , RV_OPCODE) \
    | ENCODE_STD_OPERAND(rd     , RV_OPERAND_RD) \
    | ENCODE_IMM_RVTYPE_J(imm))



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

    // RISC-V Normal Instruction Form Encoder
    class RVEncoderTypeR;
    class RVEncoderTypeI;
    class RVEncoderTypeS;
    class RVEncoderTypeB;
    class RVEncoderTypeU;
    class RVEncoderTypeJ;
    class RVEncoderTypeIZeroOperand;

    // RISC-V Normal Instruction Form Encoder Allocator
    RVEncoder* AllocRVEncoderTypeR() noexcept;
    RVEncoder* AllocRVEncoderTypeI() noexcept;
    RVEncoder* AllocRVEncoderTypeS() noexcept;
    RVEncoder* AllocRVEncoderTypeB() noexcept;
    RVEncoder* AllocRVEncoderTypeU() noexcept;
    RVEncoder* AllocRVEncoderTypeJ() noexcept;
    RVEncoder* AllocRVEncoderTypeIZeroOperand() noexcept;


#define __RV_ENCODER_DECLARATIONS \
        virtual void        SetImmediate(imm_t imm) noexcept override; \
        virtual void        SetRD(int rd) noexcept override; \
        virtual void        SetRS1(int rs1) noexcept override; \
        virtual void        SetRS2(int rs2) noexcept override; \
        virtual insnraw_t   Get() const noexcept override;

    // 
    class RVEncoderTypeR : public RVEncoder {
    protected:
        insnraw_t   insn;

    public:
        RVEncoderTypeR(int opcode, int funct3, int funct7) noexcept;
        RVEncoderTypeR(int opcode, int funct3, int funct7, bool rd_required, bool rs1_required, bool rs2_required) noexcept;

        __RV_ENCODER_DECLARATIONS
    };

    class RVEncoderTypeI : public RVEncoder {
    protected:
        insnraw_t   insn;

    public:
        RVEncoderTypeI(int opcode, int funct3) noexcept;
        RVEncoderTypeI(int opcode, int funct3, bool imm_required, bool rd_required, bool rs1_required) noexcept;

        __RV_ENCODER_DECLARATIONS
    };

    class RVEncoderTypeS : public RVEncoder {
    protected:
        insnraw_t   insn;

    public:
        RVEncoderTypeS(int opcode, int funct3) noexcept;
        RVEncoderTypeS(int opcode, int funct3, bool imm_required, bool rs1_required, bool rs2_required) noexcept;

        __RV_ENCODER_DECLARATIONS
    };

    class RVEncoderTypeB : public RVEncoder {
    protected:
        insnraw_t   insn;

    public:
        RVEncoderTypeB(int opcode, int funct3) noexcept;
        RVEncoderTypeB(int opcode, int funct3, bool imm_required, bool rs1_required, bool rs2_required) noexcept;

        __RV_ENCODER_DECLARATIONS
    };

    class RVEncoderTypeU : public RVEncoder {
    protected:
        insnraw_t   insn;

    public:
        RVEncoderTypeU(int opcode) noexcept;
        RVEncoderTypeU(int opcode, bool imm_required, bool rd_requried) noexcept;

        __RV_ENCODER_DECLARATIONS
    };

    class RVEncoderTypeJ : public RVEncoder {
    protected:
        insnraw_t   insn;

    public:
        RVEncoderTypeJ(int opcode) noexcept;
        RVEncoderTypeJ(int opcode, bool imm_required, bool rd_required) noexcept;
        
        __RV_ENCODER_DECLARATIONS
    };

    class RVEncoderTypeIZeroOperand : public RVEncoder {
    protected:
        insnraw_t   insn;

    public:
        RVEncoderTypeIZeroOperand(int opcode, int funct3, int funct12) noexcept;
    
        __RV_ENCODER_DECLARATIONS
    };
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



// Implementation of: class RVEncoderTypeR
namespace Jasse {
    /*
    insnraw_t   insn;
    */

    RVEncoderTypeR::RVEncoderTypeR(int opcode, int funct3, int funct7) noexcept
        : RVEncoder (false, true, true, true)
        , insn      (ENCODE_RVTYPE_R(opcode, funct3, 0, 0, 0, funct7))
    { }

    RVEncoderTypeR::RVEncoderTypeR(int opcode, int funct3, int funct7, bool rd_required, bool rs1_required, bool rs2_required) noexcept
        : RVEncoder (false, rd_required, rs1_required, rs2_required)
        , insn      (ENCODE_RVTYPE_R(opcode, funct3, 0, 0, 0, funct7))
    { }

    void RVEncoderTypeR::SetImmediate(imm_t imm) noexcept
    { }

    void RVEncoderTypeR::SetRD(int rd) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rd, RV_OPERAND_RD);
    }

    void RVEncoderTypeR::SetRS1(int rs1) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rs1, RV_OPERAND_RS1);
    }

    void RVEncoderTypeR::SetRS2(int rs2) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rs2, RV_OPERAND_RS2);
    }

    insnraw_t RVEncoderTypeR::Get() const noexcept
    {
        return insn;
    }
}

// Implementation of: class RVEncoderTypeI
namespace Jasse {
    /*
    insnraw_t   insn;
    */

    RVEncoderTypeI::RVEncoderTypeI(int opcode, int funct3) noexcept
        : RVEncoder (true, true, true, false)
        , insn      (ENCODE_RVTYPE_I(opcode, funct3, 0, 0, 0))
    { }

    RVEncoderTypeI::RVEncoderTypeI(int opcode, int funct3, bool imm_required, bool rd_required, bool rs1_required) noexcept
        : RVEncoder (imm_required, rd_required, rs1_required, false)
        , insn      (ENCODE_RVTYPE_I(opcode, funct3, 0, 0, 0))
    { }

    void RVEncoderTypeI::SetImmediate(imm_t imm) noexcept
    {
        insn |= ENCODE_IMM_RVTYPE_I(imm);
    }

    void RVEncoderTypeI::SetRD(int rd) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rd, RV_OPERAND_RD);
    }

    void RVEncoderTypeI::SetRS1(int rs1) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rs1, RV_OPERAND_RS1);
    }

    void RVEncoderTypeI::SetRS2(int rs2) noexcept
    { }

    insnraw_t RVEncoderTypeI::Get() const noexcept
    {
        return insn;
    }
}

// Implementation of: class RVEncoderTypeS
namespace Jasse {
    /*
    insnraw_t   insn;
    */

    RVEncoderTypeS::RVEncoderTypeS(int opcode, int funct3) noexcept
        : RVEncoder (true, false, true, true)
        , insn      (ENCODE_RVTYPE_S(opcode, funct3, 0, 0, 0))
    { }

    RVEncoderTypeS::RVEncoderTypeS(int opcode, int funct3, bool imm_required, bool rs1_required, bool rs2_required) noexcept
        : RVEncoder (imm_required, false, rs1_required, rs2_required)
        , insn      (ENCODE_RVTYPE_S(opcode, funct3, 0, 0, 0))
    { }

    void RVEncoderTypeS::SetImmediate(imm_t imm) noexcept
    {
        insn |= ENCODE_IMM_RVTYPE_S(imm);
    }

    void RVEncoderTypeS::SetRD(int rd) noexcept
    { }

    void RVEncoderTypeS::SetRS1(int rs1) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rs1, RV_OPERAND_RS1);
    }

    void RVEncoderTypeS::SetRS2(int rs2) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rs2, RV_OPERAND_RS2);
    }

    insnraw_t RVEncoderTypeS::Get() const noexcept
    {
        return insn;
    }
}

// Implementation of: class RVEncoderTypeB
namespace Jasse {
    /*
    insnraw_t   insn;
    */

    RVEncoderTypeB::RVEncoderTypeB(int opcode, int funct3) noexcept
        : RVEncoder (true, false, true, true)
        , insn      (ENCODE_RVTYPE_B(opcode, funct3, 0, 0, 0))
    { }

    RVEncoderTypeB::RVEncoderTypeB(int opcode, int funct3, bool imm_required, bool rs1_required, bool rs2_required) noexcept
        : RVEncoder (imm_required, false, rs1_required, rs2_required)
        , insn      (ENCODE_RVTYPE_B(opcode, funct3, 0, 0, 0))
    { }

    void RVEncoderTypeB::SetImmediate(imm_t imm) noexcept
    {
        insn |= ENCODE_IMM_RVTYPE_B(imm);
    }

    void RVEncoderTypeB::SetRD(int rd) noexcept
    { }

    void RVEncoderTypeB::SetRS1(int rs1) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rs1, RV_OPERAND_RS1);
    }

    void RVEncoderTypeB::SetRS2(int rs2) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rs2, RV_OPERAND_RS2);
    }

    insnraw_t RVEncoderTypeB::Get() const noexcept
    {
        return insn;
    }
}

// Implementation of: class RVEncoderTypeU
namespace Jasse {
    /*
    insnraw_t   insn;
    */

    RVEncoderTypeU::RVEncoderTypeU(int opcode) noexcept
        : RVEncoder (true, true, false, false)
        , insn      (ENCODE_RVTYPE_U(opcode, 0, 0))
    { }

    RVEncoderTypeU::RVEncoderTypeU(int opcode, bool imm_required, bool rd_required) noexcept
        : RVEncoder (imm_required, rd_required, false, false)
        , insn      (ENCODE_RVTYPE_U(opcode, 0, 0))
    { }

    void RVEncoderTypeU::SetImmediate(imm_t imm) noexcept
    {
        insn |= ENCODE_IMM_RVTYPE_U(imm);
    }

    void RVEncoderTypeU::SetRD(int rd) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rd, RV_OPERAND_RD);
    }

    void RVEncoderTypeU::SetRS1(int rs1) noexcept
    { }

    void RVEncoderTypeU::SetRS2(int rs2) noexcept
    { }

    insnraw_t RVEncoderTypeU::Get() const noexcept
    {
        return insn;
    }
}

// Implementation of: class RVEncoderTypeJ
namespace Jasse {
    /*
    insnraw_t   insn;
    */

    RVEncoderTypeJ::RVEncoderTypeJ(int opcode) noexcept
        : RVEncoder (true, true, false, false)
        , insn      (ENCODE_RVTYPE_J(opcode, 0, 0))
    { }

    RVEncoderTypeJ::RVEncoderTypeJ(int opcode, bool imm_required, bool rd_required) noexcept
        : RVEncoder (imm_required, rd_required, false, false)
        , insn      (ENCODE_RVTYPE_J(opcode, 0, 0))
    { }

    void RVEncoderTypeJ::SetImmediate(imm_t imm) noexcept
    {
        insn |= ENCODE_IMM_RVTYPE_J(imm);
    }

    void RVEncoderTypeJ::SetRD(int rd) noexcept
    {
        insn |= ENCODE_STD_OPERAND(rd, RV_OPERAND_RD);
    }

    void RVEncoderTypeJ::SetRS1(int rs1) noexcept
    { }

    void RVEncoderTypeJ::SetRS2(int rs2) noexcept
    { }

    insnraw_t RVEncoderTypeJ::Get() const noexcept
    {
        return insn;
    }
}

// Implementation of: class RVEncoderTypeIZeroOperand
namespace Jasse {
    /*
    insnraw_t   insn;
    */

    RVEncoderTypeIZeroOperand::RVEncoderTypeIZeroOperand(int opcode, int funct3, int funct12) noexcept
        : RVEncoder (false, false, false, false)
        , insn      (ENCODE_RVTYPE_I(opcode, funct3, 0, 0, funct12))
    { }

    void RVEncoderTypeIZeroOperand::SetImmediate(imm_t imm) noexcept
    { }

    void RVEncoderTypeIZeroOperand::SetRD(int rd) noexcept
    { }

    void RVEncoderTypeIZeroOperand::SetRS1(int rs1) noexcept
    { }

    void RVEncoderTypeIZeroOperand::SetRS2(int rs2) noexcept
    { }

    insnraw_t RVEncoderTypeIZeroOperand::Get() const noexcept
    {
        return insn;
    }
}
