#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Instruction Generator (CodeGen) Standard utilities
//

#include "riscvgenstd.hpp"


// RISC-V Code Generator utilities
namespace Jasse {
    
    //
    void        Rand32Seed(uint32_t seed);
    uint32_t    Rand32(uint32_t lower_range, uint32_t upper_range);
    uint32_t    Rand32(const RVCodeGenConstraintRangeI* range = nullptr);
    uint32_t    Rand32(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>& trait);

    void        Rand64Seed(uint64_t seed);
    uint64_t    Rand64(uint64_t lower_range, uint64_t upper_range);
    uint64_t    Rand64(const RVCodeGenConstraintRangeI* range = nullptr);
    uint32_t    Rand64(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>& trait);

    //
    const RVCodepoint*  Roll(const RVCodepointCollection& codeset);
}

// RISC-V General Code Generators
namespace Jasse {

    insnraw_t   GenRVCodeGeneral(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;

    insnraw_t   GenRVCodeTypeR(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeI(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeS(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeB(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeU(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeJ(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;

    insnraw_t   GenRVCodeZeroOperand(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;


    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVGeneral(const RVCodeGenConstraints* constraints) noexcept;

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeR(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeI(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeS(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeB(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeU(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeJ(const RVCodeGenConstraints* constraints) noexcept;

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVZeroOperand(const RVCodeGenConstraints* constraints) noexcept;
}



// Implementation of utilities
namespace Jasse {

    //
    static std::mt19937     rand32_engine = std::mt19937(std::random_device()());

    inline uint32_t Rand32(uint32_t lower_range, uint32_t upper_range)
    {
        return std::uniform_int_distribution<uint32_t>(lower_range, upper_range)(rand32_engine);
    }

    inline uint32_t Rand32(const RVCodeGenConstraintRangeI* range)
    {
        if (!range)
            return Rand32(0, UINT32_MAX);
        else
            return Rand32((uint32_t) range->GetLowerRange(), (uint32_t) range->GetUpperRange());
    }

    inline uint32_t Rand32(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>& trait)
    {
        return Rand32(GetConstraint(constraints, trait));
    }

    inline void Rand32Seed(uint32_t seed)
    {
        rand32_engine.seed(seed);
    }

    // 
    static std::mt19937_64  rand64_engine = std::mt19937_64(std::random_device()());

    inline uint64_t Rand64(uint64_t lower_range, uint64_t upper_range)
    {
        return std::uniform_int_distribution<uint64_t>(lower_range, upper_range)(rand64_engine);
    }

    inline uint64_t Rand64(const RVCodeGenConstraintRangeI* range)
    {
        if (!range)
            return Rand64(0, UINT64_MAX);
        else
            return Rand64(range->GetLowerRange(), range->GetUpperRange());
    }

    inline uint32_t Rand64(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>& trait)
    {
        return Rand64(GetConstraint(constraints, trait));
    }

    inline void Rand64Seed(uint64_t seed)
    {
        rand64_engine.seed(seed);
    }

    //
    inline const RVCodepoint* Roll(const RVCodepointCollection& codeset)
    {
        if (!codeset.GetSize())
            return nullptr;

        return codeset.Get(Rand32(0, codeset.GetSize() - 1));
    }
}


// Implementation of General Code Generators
namespace Jasse {

    // *NOTICE: It's recommended to call GenRVCodeType(X)(...) when generating corresponding type of RISC-V 
    //          instruction code (e.g. Calling GenRVCodeTypeR when generating R-type instructions). 
    //          Though the performance gain of calling individual function is not significant at all, it's
    //          designed to make the code more readable, providing more information for code generation, and
    //          applying specialized constraints through calling different functions, regardless of it's duplicating.
    //          In design, generating any type of instruction codes can also be done by calling GenRVCodeGeneral,
    //          and wouldn't cause critical fault, but it's not always elegant depending on implementation.

    insnraw_t GenRVCodeGeneral(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS2Required())
            encoder->SetRS2(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVGeneral(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeGeneral(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeR(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS2Required())
            encoder->SetRS2(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeR(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeR(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeI(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeI(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeI(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeS(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS2Required())
            encoder->SetRS2(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeS(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeS(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeB(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS2Required())
            encoder->SetRS2(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeB(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeB(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeU(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeU(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeU(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeJ(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeJ(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeJ(alloc, constraints);
    }

    insnraw_t GenRVCodeZeroOperand(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVZeroOperand(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeZeroOperand(alloc, constraints);
    }
}
