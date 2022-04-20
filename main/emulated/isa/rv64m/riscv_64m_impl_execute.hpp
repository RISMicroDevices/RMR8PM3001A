#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, bahaviour function implementations
//

#include "riscv_64m_def_execute.hpp"

#include "gmp.h"


// executors
namespace Jasse {

#define RV64M_EXECUTOR_PARAMS       const RVInstruction& insn, RVArchitecturalOOC* arch, RVMemoryInterface* MI, RVCSRSpace* CSRs

    // MUL
    RVExecStatus RV64MExecutor_MUL(RV64M_EXECUTOR_PARAMS)
    {
        arch->GR64()->Set(insn.GetRD(),
            arch->GR64()->Get(insn.GetRS1()) * arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // MULH
    RVExecStatus RV64MExecutor_MULH(RV64M_EXECUTOR_PARAMS)
    {
        int64_t multiplicand = (int64_t) arch->GR64()->Get(insn.GetRS1());
        int64_t multiplier   = (int64_t) arch->GR64()->Get(insn.GetRS2());
        
        int64_t result;

        // gmp field
        mpz_t mpz_s, mpz_i, mpz_r;

        mpz_inits(mpz_s, mpz_i, mpz_r, nullptr);

        mpz_set_si(mpz_s, multiplicand);
        mpz_mul_si(mpz_i, mpz_s, multiplier); // mul
        
        mpz_tdiv_q_2exp(mpz_r, mpz_i, 64);    // shift

        ASSERTM(mpz_fits_slong_p(mpz_r), "mpz mulh overflow");

        result = mpz_get_si(mpz_r);

        mpz_clears(mpz_s, mpz_i, mpz_r, nullptr);
        //

        arch->GR64()->Set(insn.GetRD(), (uint64_t) result);

        return EXEC_SEQUENTIAL;
    }

    // MULHU
    RVExecStatus RV64MExecutor_MULHU(RV64M_EXECUTOR_PARAMS)
    {
        uint64_t multiplicand = arch->GR64()->Get(insn.GetRS1());
        uint64_t multiplier   = arch->GR64()->Get(insn.GetRS2());
        
        uint64_t result;

        // gmp field
        mpz_t mpz_s, mpz_i, mpz_r;

        mpz_inits(mpz_s, mpz_i, mpz_r, nullptr);

        mpz_set_ui(mpz_s, multiplicand);
        mpz_mul_ui(mpz_i, mpz_s, multiplier); // mul
        
        mpz_tdiv_q_2exp(mpz_r, mpz_i, 64);    // shift

        ASSERTM(mpz_fits_ulong_p(mpz_r), "mpz mulhu overflow");

        result = mpz_get_ui(mpz_r);

        mpz_clears(mpz_s, mpz_i, mpz_r, nullptr);
        //

        arch->GR64()->Set(insn.GetRD(), result);

        return EXEC_SEQUENTIAL;
    }

    // MULHSU
    RVExecStatus RV64MExecutor_MULHSU(RV64M_EXECUTOR_PARAMS)
    {
        int64_t  multiplicand = arch->GR64()->Get(insn.GetRS1());
        uint64_t multiplier   = arch->GR64()->Get(insn.GetRS2());

        int64_t result;

        // gmp field
        mpz_t mpz_s, mpz_i, mpz_r;

        mpz_inits(mpz_s, mpz_i, mpz_r, nullptr);

        mpz_set_si(mpz_s, multiplicand);
        mpz_mul_ui(mpz_i, mpz_s, multiplier); // mul
        
        mpz_tdiv_q_2exp(mpz_r, mpz_i, 64);    // shift

        ASSERT(mpz_fits_slong_p(mpz_r));

        result = mpz_get_si(mpz_r);

        mpz_clears(mpz_s, mpz_i, mpz_r, nullptr);
        //

        arch->GR64()->Set(insn.GetRD(), (uint64_t) result);

        return EXEC_SEQUENTIAL;
    }

    // MULW
    RVExecStatus RV64MExecutor_MULW(RV64M_EXECUTOR_PARAMS)
    {
        uint32_t result =  
            ((uint32_t) arch->GR64()->Get(insn.GetRS1())) * ((uint32_t) arch->GR64()->Get(insn.GetRS2()));

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }

    // DIV
    RVExecStatus RV64MExecutor_DIV(RV64M_EXECUTOR_PARAMS)
    {
        int64_t dividend = (int64_t) arch->GR64()->Get(insn.GetRS1());
        int64_t divisor  = (int64_t) arch->GR64()->Get(insn.GetRS2());

        int64_t result;

        if (dividend == INT64_MIN && divisor == -1) // overflow
            result = INT64_MIN;
        else if (!divisor) // divide by zero
            result = -1;
        else
            result = dividend / divisor;

        arch->GR64()->Set(insn.GetRD(), (uint64_t) result);

        return EXEC_SEQUENTIAL;
    }

    // REM
    RVExecStatus RV64MExecutor_REM(RV64M_EXECUTOR_PARAMS)
    {
        int64_t dividend = (int64_t) arch->GR64()->Get(insn.GetRS1());
        int64_t divisor  = (int64_t) arch->GR64()->Get(insn.GetRS2());

        int64_t result;

        if (dividend == INT64_MIN && divisor == -1) // overflow
            result = 0;
        else if (!divisor) // divide by zero
            result = dividend;
        else
            result = dividend % divisor;

        arch->GR64()->Set(insn.GetRD(), (uint64_t) result);

        return EXEC_SEQUENTIAL;
    }

    // DIVU
    RVExecStatus RV64MExecutor_DIVU(RV64M_EXECUTOR_PARAMS)
    {
        uint64_t dividend = arch->GR64()->Get(insn.GetRS1());
        uint64_t divisor  = arch->GR64()->Get(insn.GetRS2());

        uint64_t result;

        if (!divisor) // divide by zero
            result = UINT64_MAX;
        else
            result = dividend / divisor;

        arch->GR64()->Set(insn.GetRD(), result);

        return EXEC_SEQUENTIAL;
    }

    // REMU
    RVExecStatus RV64MExecutor_REMU(RV64M_EXECUTOR_PARAMS)
    {
        uint64_t dividend = arch->GR64()->Get(insn.GetRS1());
        uint64_t divisor  = arch->GR64()->Get(insn.GetRS2());

        uint64_t result;

        if (!divisor) // divide by zero
            result = dividend;
        else
            result = dividend % divisor;

        arch->GR64()->Set(insn.GetRD(), result);

        return EXEC_SEQUENTIAL;
    }

    // DIVW
    RVExecStatus RV64MExecutor_DIVW(RV64M_EXECUTOR_PARAMS)
    {
        int32_t dividend = (int32_t) arch->GR64()->Get(insn.GetRS1());
        int32_t divisor  = (int32_t) arch->GR64()->Get(insn.GetRS2());

        int32_t result;

        if (dividend == INT32_MIN && divisor == -1) // overflow
            result = INT32_MIN;
        else if (!divisor) // divide by zero
            result = -1;
        else
            result = dividend / divisor;

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }

    // REMW
    RVExecStatus RV64MExecutor_REMW(RV64M_EXECUTOR_PARAMS)
    {
        int32_t dividend = (int32_t) arch->GR64()->Get(insn.GetRS1());
        int32_t divisor  = (int32_t) arch->GR64()->Get(insn.GetRS2());

        int32_t result;

        if (dividend == INT32_MIN && divisor == -1) // overflow
            result = 0;
        else if (!divisor) // divide by zero
            result = dividend;
        else
            result = dividend % divisor;

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }

    // DIVUW
    RVExecStatus RV64MExecutor_DIVUW(RV64M_EXECUTOR_PARAMS)
    {
        uint32_t dividend = (uint32_t) arch->GR64()->Get(insn.GetRS1());
        uint32_t divisor  = (uint32_t) arch->GR64()->Get(insn.GetRS2());

        uint32_t result;

        if (!divisor) // divide by zero
            result = UINT32_MAX;
        else
            result = dividend / divisor;

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }

    // REMUW
    RVExecStatus RV64MExecutor_REMUW(RV64M_EXECUTOR_PARAMS)
    {
        uint32_t dividend = (uint32_t) arch->GR64()->Get(insn.GetRS1());
        uint32_t divisor  = (uint32_t) arch->GR64()->Get(insn.GetRS2());

        uint32_t result;

        if (!divisor) // divide by zero
            result = dividend;
        else
            result = dividend % divisor;

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }
}
