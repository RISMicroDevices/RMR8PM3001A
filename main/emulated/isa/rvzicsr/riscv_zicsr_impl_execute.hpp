#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR", bahaviour function implementations
//

#include "riscvexcept.hpp"
#include "riscv_zicsr_def_execute.hpp"

// executors
namespace Jasse {

#define __RVZICSR_ACQUIRE_CSR(csr, insn) \
    RVCSR* csr = ctx.CSRs->GetCSR(GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_CSR)); \
    if (!csr) \
    { \
        ctx.trap.TrapEnter(ctx.arch, ctx.CSRs, TRAP_EXCEPTION, EXCEPTION_ILLEGAL_INSTRUCTION); \
        return EXEC_TRAP_ENTER; \
    }


    // CSRRW
    RVExecStatus RVZicsrExecutor_CSRRW(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn)

        if (insn.GetRD())
            ctx.arch->SetGRx64(insn.GetRD(), csr->Read(ctx.CSRs));

        csr->Write(ctx.CSRs, ctx.arch->GetGRx64Zext(insn.GetRS1()));

        return EXEC_SEQUENTIAL;
    }

    // CSRRS
    RVExecStatus RVZicsrExecutor_CSRRS(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn)

        csr_t csrval  = csr->Read(ctx.CSRs);
        csr_t csrmask = ctx.arch->GetGRx64Zext(insn.GetRS1());

        ctx.arch->SetGRx64(insn.GetRD(), csrval);

        csrval = SET_CSR_BITS(csrval, csrmask);

        csr->Write(ctx.CSRs, csrval);

        return EXEC_SEQUENTIAL;
    }

    // CSRRC
    RVExecStatus RVZicsrExecutor_CSRRC(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn)

        csr_t csrval  = csr->Read(ctx.CSRs);
        csr_t csrmask = ctx.arch->GetGRx64Zext(insn.GetRS1());

        ctx.arch->SetGRx64(insn.GetRD(), csrval);

        csrval = CLEAR_CSR_BITS(csrval, csrmask);

        csr->Write(ctx.CSRs, csrval);

        return EXEC_SEQUENTIAL;
    }

    // CSRRWI
    RVExecStatus RVZicsrExecutor_CSRRWI(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn)

        if (insn.GetRD())
            ctx.arch->SetGRx64(insn.GetRD(), csr->Read(ctx.CSRs));

        csr->Write(ctx.CSRs, ctx.arch->GetGRx64Zext(insn.GetRS1()));

        return EXEC_SEQUENTIAL;
    }

    // CSRRSI
    RVExecStatus RVZicsrExecutor_CSRRSI(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn);

        csr_t csrval  = csr->Read(ctx.CSRs);
        csr_t csrmask = GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_CSR_UIMM);

        ctx.arch->SetGRx64(insn.GetRD(), csrval);

        csrval = SET_CSR_BITS(csrval, csrmask);

        csr->Write(ctx.CSRs, csrval);

        return EXEC_SEQUENTIAL;   
    }

    // CSRRCI
    RVExecStatus RVZicsrExecutor_CSRRCI(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn);

        csr_t csrval  = csr->Read(ctx.CSRs);
        csr_t csrmask = GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_CSR_UIMM);

        ctx.arch->SetGRx64(insn.GetRD(), csrval);

        csrval = CLEAR_CSR_BITS(csrval, csrmask);

        csr->Write(ctx.CSRs, csrval);

        return EXEC_SEQUENTIAL;
    }
}
