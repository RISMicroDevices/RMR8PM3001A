#pragma once
//
// RISC-V Instruction Set Architecture Privileged
//
// Privileged exception codes and procedures (Includes ratified standard only)
//

#include "riscv.hpp"
#include "base/riscvcsr.hpp"

#include "csr/riscvcsr_mstatus.hpp"
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

    // RISC-V Trap Procedure
    // * Notice: Only PC and necessary CSRs (excluding 'mtval' .etc) are manipulated in this
    //           Trap-Procedure. Other software-defined or EEI-defined CSR/General Registers
    //           actions should be done out of this Trap-Procedure.

    //
    void TrapEnterM  (RVArchitecturalOOC* arch, RVCSRSpace* CSRs, RVTrapType type, RVTrapCause cause) noexcept(false);
    void TrapReturnM (RVArchitecturalOOC* arch, RVCSRSpace* CSRs) noexcept(false);

    // M-mode Trap Procedures
    static const RVTrapProcedures TRAP_PROCEDURES_M_MODE = { &TrapEnterM, &TrapReturnM };
}


namespace Jasse {

    // M-mode Trap Enter Procedure
    void TrapEnterM(RVArchitecturalOOC* arch, RVCSRSpace* CSRs, RVTrapType type, RVTrapCause cause) noexcept(false)
    {
        // Write 'mepc' CSR
        CSRs->RequireCSR(CSR_mepc, "mepc")
            ->Write(CSRs, arch->PC().pc64); // always zero-extended in XLEN=32, actually doesn't matter
        

        // Write 'mcause' CSR
        csr_t mcause = 0;

        if (arch->XLEN() == XLEN32) // XLEN=32
        {
            SET_CSR_MXFIELD(mcause, CSR_mcause_FIELD_INTERRUPT, (uint32_t)type, MX32);
            SET_CSR_MXFIELD(mcause, CSR_mcause_FIELD_EXCEPTION_CODE, cause, MX32);
        }
        else // XLEN=64
        {
            SET_CSR_MXFIELD(mcause, CSR_mcause_FIELD_INTERRUPT, (uint64_t)type, MX64);
            SET_CSR_MXFIELD(mcause, CSR_mcause_FIELD_EXCEPTION_CODE, cause, MX64);
        }

        CSRs->RequireCSR(CSR_mcause, "mcause")
            ->Write(CSRs, mcause);
        

        // Write 'mstatus' CSR
        RVCSR* p_mstatus = CSRs->RequireCSR(CSR_mstatus, "mstatus");

        csr_t mstatus = p_mstatus->Read(CSRs); // read 'mstatus'

        SET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MPIE,
            GET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MIE)); // xPIE is set to the value of xIE

        SET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MIE, 0); // xIE is set to 0

        // *NOTICE: MPP writing now hardwired to PRIV_MACHINE, this is not a standard implementation.
        //          Rewrite this part after other privileged levels supported.
        SET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MPP, /*NOTICE*/ PRIV_MACHINE);

        p_mstatus->Write(CSRs, mstatus); // write 'mstatus'


        // Write PC
        csr_t mtvec = CSRs->RequireCSR(CSR_mtvec, "mtvec")->Read(CSRs); // read 'mtvec'

        int mode = GET_CSR_FIELD(mtvec, CSR_mtvec_FIELD_MODE);
        if (mode == CSR_mtvec_FIELD_MODE_DEF_VECTORED && type == TRAP_INTERRUPT) // vectored interrupt trap
        {
            if (arch->XLEN() == XLEN32) // XLEN=32
                arch->SetPC32(GET_CSR_MXFIELD(mtvec, CSR_mtvec_FIELD_BASE, MX32) + (cause << 2));
            else // XLEN=64
                arch->SetPC64(GET_CSR_MXFIELD(mtvec, CSR_mtvec_FIELD_BASE, MX64) + (cause << 2));
        }
        else
        {
            if (arch->XLEN() == XLEN32) // XLEN=32
                arch->SetPC32(GET_CSR_MXFIELD(mtvec, CSR_mtvec_FIELD_BASE, MX32));
            else // XLEN=64
                arch->SetPC64(GET_CSR_MXFIELD(mtvec, CSR_mtvec_FIELD_BASE, MX64));
        }
    }

    // M-mode Trap Return Procedure
    void TrapReturnM(RVArchitecturalOOC* arch, RVCSRSpace* CSRs) noexcept(false)
    {
        // Write 'mstatus' CSR
        RVCSR* p_mstatus  = CSRs->RequireCSR(CSR_mstatus, "mstatus");

        csr_t mstatus = p_mstatus->Read(CSRs); // read 'mstatus'

        SET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MIE,
            GET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MPIE)); // xIE is set to xPIE

        SET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MPIE, 1); // xPIE is set to 1

        // xPP is set to the least-privileged supported mode.
        // *NOTICE: For M-mode supported only, the least-privileged level is M-mode.
        //          Rewrite this part after other privileged levels supported.
        SET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MPP, PRIV_MACHINE); // 

        // Privileged Level should be set to MPP (or xPP), not implemented now.
        // GET_CSR_FIELD(mstatus, CSR_mstatus_FIELD_MPP);

        p_mstatus->Write(CSRs, mstatus); // write 'mstatus'


        // Write PC
        csr_t mepc = CSRs->RequireCSR(CSR_mepc, "mepc")->Read(CSRs);

        if (arch->XLEN() == XLEN32) // XLEN=32
            arch->SetPC32((uint32_t)mepc);
        else
            arch->SetPC64(mepc);
    }
}

