#pragma once
//
// RISC-V Instruction Set Architecture Privileged
//
// Privileged exception codes and procedures (Includes ratified standard only)
//

#include "riscv.hpp"
#include "riscvcsr.hpp"

#include "csr/riscvcsr_mcause.hpp"
#include "csr/riscvcsr_mtvec.hpp"


// Interrupt codes
#define     INTERRUPT_S_SOFTWARE                        1       // Supervisor software interrupt
#define     INTERRUPT_M_SOFTWARE                        3       // Machine software interrupt

#define     INTERRUPT_S_TIMER                           5       // Supervisor timer interrupt
#define     INTERRUPT_M_TIMER                           7       // Machine timer interrupt

#define     INTERRUPT_S_EXTERNAL                        9       // Supervisor external interrupt
#define     INTERRUPT_M_EXTERNAL                        11      // Machine external interrupt


// Exception codes
#define     EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED    0       // Instruction address misaligned
#define     EXCEPTION_INSTRUCTION_ACCESS_FAULT          1       // Instruction access fault
#define     EXCEPTION_ILLEGAL_INSTRUCTION               2       // Illegal instruction

#define     EXCEPTION_BREAKPOINT                        3       // Breakpoint

#define     EXCEPTION_LOAD_ADDRESS_MISALIGNED           4       // Load address misaligned
#define     EXCEPTION_LOAD_ACCESS_FAULT                 5       // Load access fault

#define     EXCEPTION_STORE_ADDRESS_MISALIGNED          6       // Store address misaligned
#define     EXCEPTION_STORE_ACCESS_FAULT                7       // Store access fault

#define     EXCEPTION_AMO_ADDRESS_MISALIGNED            6       // AMO address misaligned
#define     EXCEPTION_AMO_ACCESS_FAULT                  7       // AMO access fault

#define     EXCEPTION_ECALL_FROM_U                      8       // Environment call from U-mode
#define     EXCEPTION_ECALL_FROM_S                      9       // Environment call from S-mode
#define     EXCEPTION_ECALL_FROM_M                      11      // Environment call from M-mode

#define     EXCEPTION_PAGE_FAULT_INSTRUCTION            12      // Instruction page fault
#define     EXCEPTION_PAGE_FAULT_LOAD                   13      // Load page fault
#define     EXCEPTION_PAGE_FAULT_STORE                  15      // Store page fault
#define     EXCEPTION_PAGE_FAULT_AMO                    15      // AMO page fault


namespace Jasse {

    // RISC-V Trap Cause
    typedef     uint32_t            RVTrapCause;

    // RISC-V Trap Types
    typedef enum {
        TRAP_EXCEPTION = 0,
        TRAP_INTERRUPT
    } RVTrapType;

    // RISC-V Trap Procedure
    // * Notice: Only PC and necessary CSRs (excluding 'mtval' .etc) are manipulated in this
    //           Trap-Procedure. Other software-defined or EEI-defined CSR/General Registers
    //           actions should be done out of this Trap-Procedure.
    void Trap(RVArchitectural& arch, RVTrapType type, RVTrapCause cause);

}


namespace Jasse {

    void Trap(RVArchitectural& arch, RVTrapType type, RVTrapCause cause)
    {
        // only M-mode supported currently

        // Write 'mepc' CSR
        arch.CSR()->RequireCSR(CSR_mepc, "mepc")
            ->SetValue(arch.PC().pc64); // always zero-extended in XLEN=32, actually doesn't matter

        // Write 'mcause' CSR
        csr_t mcause = 0;

        if (arch.XLEN() == XLEN32) // XLEN=32
        {
            SET_CSR_MXFIELD(mcause, CSR_mcause_FIELD_INTERRUPT, (uint32_t)type, MX32);
            SET_CSR_MXFIELD(mcause, CSR_mcause_FIELD_EXCEPTION_CODE, cause, MX32);
        }
        else // XLEN=64
        {
            SET_CSR_MXFIELD(mcause, CSR_mcause_FIELD_INTERRUPT, (uint64_t)type, MX64);
            SET_CSR_MXFIELD(mcause, CSR_mcause_FIELD_EXCEPTION_CODE, cause, MX64);
        }

        arch.CSR()->RequireCSR(CSR_mcause, "mcause")
            ->SetValue(mcause);

        // Write PC
        csr_t mtvec = arch.CSR()->RequireCSR(CSR_mtvec, "mtvec")->GetValue(); // read 'mtvec'

        int mode = GET_CSR_FIELD(mtvec, CSR_mtvec_FIELD_MODE);
        if (mode == CSR_mtvec_FIELD_MODE_DEF_VECTORED && type == TRAP_INTERRUPT) // vectored interrupt trap
        {
            if (arch.XLEN() == XLEN32) // XLEN=32
                arch.SetPC32(GET_CSR_MXFIELD(mtvec, CSR_mtvec_FIELD_BASE, MX32) + (cause << 2));
            else // XLEN=64
                arch.SetPC64(GET_CSR_MXFIELD(mtvec, CSR_mtvec_FIELD_BASE, MX64) + (cause << 2));
        }
        else
        {
            if (arch.XLEN() == XLEN32) // XLEN=32
                arch.SetPC32(GET_CSR_MXFIELD(mtvec, CSR_mtvec_FIELD_BASE, MX32));
            else // XLEN=64
                arch.SetPC64(GET_CSR_MXFIELD(mtvec, CSR_mtvec_FIELD_BASE, MX64));
        }

    }
}

