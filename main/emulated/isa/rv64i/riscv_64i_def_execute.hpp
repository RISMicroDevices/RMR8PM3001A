#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Base RV64I, bahaviour function definitions
//

#include "riscv.hpp"

// executors
namespace Jasse {

#define RV64I_EXECUTOR_PARAMS       const RVInstruction& insn, const RVExecContext& ctx

    RVExecStatus RV64IExecutor_ADDI (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SLTI (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SLTIU(RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_ANDI (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_ORI  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_XORI (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_SLLI (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRLI (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRAI (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_ADDIW(RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SLLIW(RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRLIW(RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRAIW(RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_ADD  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SUB  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SLT  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SLTU (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_AND  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_OR   (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_XOR  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SLL  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRL  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRA  (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_ADDW (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SUBW (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SLLW (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRLW (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRAW (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_LUI  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_AUIPC(RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_JAL  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_JALR (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_BEQ  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_BNE  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_BLT  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_BLTU (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_BGE  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_BGEU (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_LD   (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_LW   (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_LH   (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_LB   (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_LWU  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_LHU  (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_LBU  (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_SD   (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SW   (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SH   (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SB   (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_FENCE(RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_ECALL(RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_EBREAK(RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_MRET (RV64I_EXECUTOR_PARAMS);
    RVExecStatus RV64IExecutor_SRET (RV64I_EXECUTOR_PARAMS);

    RVExecStatus RV64IExecutor_WFI  (RV64I_EXECUTOR_PARAMS);

    // TODO ... Add RV64I instructions here ...
}
