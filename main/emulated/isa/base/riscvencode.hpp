#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Encoder infrastructure
//

#include "riscvdef.hpp"

namespace Jasse {
    // RISC-V Instruction Encoder
    class RVEncoder {
    private:
        const bool  imm_required;
        const bool  rd_required;
        const bool  rs1_required;
        const bool  rs2_required;

    public:
        RVEncoder() noexcept;
        RVEncoder(bool imm_required, bool rd_required, bool rs1_required, bool rs2_required) noexcept;
        ~RVEncoder() noexcept;

        virtual void        SetImmediate(imm_t imm) noexcept = 0;
        virtual void        SetRD(int rd) noexcept = 0;
        virtual void        SetRS1(int rs1) noexcept = 0;
        virtual void        SetRS2(int rs2) noexcept = 0;

        virtual insnraw_t   Get() const noexcept = 0;

        bool                IsImmediateRequired() const noexcept;
        bool                IsRDRequired() const noexcept;
        bool                IsRS1Required() const noexcept;
        bool                IsRS2Required() const noexcept;

        RVEncoder&          Imm(imm_t imm) noexcept;
        RVEncoder&          RD(int rd) noexcept;
        RVEncoder&          RS1(int rs1) noexcept;
        RVEncoder&          RS2(int rs2) noexcept;
    };

    // RISC-V Instruction Encoder Allocator
    typedef RVEncoder*  (*RVEncoderAllocator)(void);
}



// Implementation of: class RVEncoder
namespace Jasse {
    RVEncoder::RVEncoder() noexcept
        : imm_required  (true)
        , rd_required   (true)
        , rs1_required  (true)
        , rs2_required  (true)
    { }

    RVEncoder::RVEncoder(bool imm_required, bool rd_required, bool rs1_required, bool rs2_required) noexcept
        : imm_required  (imm_required)
        , rd_required   (rd_required)
        , rs1_required  (rs1_required)
        , rs2_required  (rs2_required)
    { }

    RVEncoder::~RVEncoder() noexcept
    { }

    inline RVEncoder& RVEncoder::Imm(imm_t imm) noexcept
    {
        SetImmediate(imm);
        return *this;
    }

    inline RVEncoder& RVEncoder::RD(int rd) noexcept
    {
        SetRD(rd);
        return *this;
    }

    inline RVEncoder& RVEncoder::RS1(int rs1) noexcept
    {
        SetRS1(rs1);
        return *this;
    }

    inline RVEncoder& RVEncoder::RS2(int rs2) noexcept
    {
        SetRS2(rs2);
        return *this;
    }

    inline bool RVEncoder::IsImmediateRequired() const noexcept
    {
        return imm_required;
    }

    inline bool RVEncoder::IsRDRequired() const noexcept
    {
        return rd_required;
    }

    inline bool RVEncoder::IsRS1Required() const noexcept
    {
        return rs1_required;
    }

    inline bool RVEncoder::IsRS2Required() const noexcept
    {
        return rs2_required;
    }
}
