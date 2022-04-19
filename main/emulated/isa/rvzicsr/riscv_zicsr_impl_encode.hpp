#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR", codepoint infrastructure definitions
//

#include "riscvmisc.hpp"
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
    insnraw_t GenRVZicsrCodeCSRx(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints)
    {
        return GenRVCodeTypeI(encoderAlloc, constraints);

        // TODO
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVZicsrCSRx(const RVCodeGenConstraints* constraints)
    {
        return GenRVZicsrCodeCSRx(alloc, constraints);
    }

    insnraw_t GenRVZicsrCodeCSRxI(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints)
    {
        return GenRVCodeTypeI(encoderAlloc, constraints);

        // TODO
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVZicsrCSRxI(const RVCodeGenConstraints* constraints)
    {
        return GenRVZicsrCodeCSRxI(alloc, constraints);
    }
}