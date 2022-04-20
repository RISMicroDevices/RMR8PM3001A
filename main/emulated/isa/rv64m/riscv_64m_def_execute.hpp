#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, bahaviour function definitions
//

#include <cstdint>

#include "gmp.h"

#include "riscv.hpp"


// executors
namespace Jasse {

#define RV64M_EXECUTOR_PARAMS       const RVInstruction& insn, RVArchitecturalOOC* arch, RVMemoryInterface* MI, RVCSRSpace* CSRs

    RVExecStatus RV64MExecutor_MUL   (RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_MULH  (RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_MULHU (RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_MULHSU(RV64M_EXECUTOR_PARAMS);
    
    RVExecStatus RV64MExecutor_MULW(RV64M_EXECUTOR_PARAMS);

    RVExecStatus RV64MExecutor_DIV (RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_REM (RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_DIVU(RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_REMU(RV64M_EXECUTOR_PARAMS);

    RVExecStatus RV64MExecutor_DIVW (RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_REMW (RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_DIVUW(RV64M_EXECUTOR_PARAMS);
    RVExecStatus RV64MExecutor_REMUW(RV64M_EXECUTOR_PARAMS);
}
