#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR", codepoint infrastructure definitions
//

#include "riscvmisc.hpp"
#include "riscvgenutil.hpp"
#include "riscv_zicsr_def_encode.hpp"


#define __RVZicsr_D3CGI_CSRx(name) \
    &RVZicsrCodePoint_Funct3_##name, \
    &CodeGenRVZicsrCSRx<&AllocRVEncoderTypeI<RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_##name>>, \
    &RVZicsrExecutor_##name

#define __RVZicsr_D3CGI_CSRxI(name) \
    &RVZicsrCodePoint_Funct3_##name, \
    &CodeGenRVZicsrCSRxI<&AllocRVEncoderTypeI<RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_##name>>, \
    &RVZicsrExecutor_##name


// RV Zicsr CodeGen
namespace Jasse {

    inline void __RandCSR(RVEncoder* encoder, const RVCodeGenConstraints* constraints)
    {
        const RVCodeGenConstraintCSRList* candidate 
            = constraints->Get(RV_CODEGEN_CSR_CANDIDATE);

        if (candidate)
            encoder->SetImmediate(Rand32(0, candidate->GetSize() - 1));
        else
            encoder->SetImmediate(Rand32(0, CSR_ADDRESS_MASK));
    }

    insnraw_t GenRVZicsrCodeCSRx(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints)
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            __RandCSR(encoder, constraints);

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVZicsrCSRx(const RVCodeGenConstraints* constraints)
    {
        return GenRVZicsrCodeCSRx(alloc, constraints);
    }

    insnraw_t GenRVZicsrCodeCSRxI(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints)
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            __RandCSR(encoder,constraints);

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS1Required()) // uimm-5 generation
            encoder->SetRS1(Rand32(0, 0x1F));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVZicsrCSRxI(const RVCodeGenConstraints* constraints)
    {
        return GenRVZicsrCodeCSRxI(alloc, constraints);
    }
}