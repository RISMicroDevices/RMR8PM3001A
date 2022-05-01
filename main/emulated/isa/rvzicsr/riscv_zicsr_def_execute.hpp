#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR", bahaviour function definitions
//

#include "riscv.hpp"


// executors
namespace Jasse {

#define RVZICSR_EXECUTOR_PARAMS     const RVInstruction& insn, const RVExecContext& ctx

    RVExecStatus RVZicsrExecutor_CSRRW(RVZICSR_EXECUTOR_PARAMS);
    RVExecStatus RVZicsrExecutor_CSRRS(RVZICSR_EXECUTOR_PARAMS);
    RVExecStatus RVZicsrExecutor_CSRRC(RVZICSR_EXECUTOR_PARAMS);

    RVExecStatus RVZicsrExecutor_CSRRWI(RVZICSR_EXECUTOR_PARAMS);
    RVExecStatus RVZicsrExecutor_CSRRSI(RVZICSR_EXECUTOR_PARAMS);
    RVExecStatus RVZicsrExecutor_CSRRCI(RVZICSR_EXECUTOR_PARAMS);
}