#pragma once
//
// RISC-V Instruction Set Architecture Trap Definitions
//
//
//

#include <cstdint>

#include "riscvooc.hpp"
#include "riscvcsr.hpp"

    
namespace Jasse {

    // RISC-V Trap Cause
    typedef     uint32_t            RVTrapCause;

    // RISC-V Trap Types
    typedef enum {
        TRAP_EXCEPTION = 0,
        TRAP_INTERRUPT
    } RVTrapType;


    // RISC-V Trap Procedures
    typedef     void (*RVTrapEnterProcedure)  (RVArchitecturalOOC* arch, RVCSRSpace* CSRs, RVTrapType type, RVTrapCause cause);
    typedef     void (*RVTrapReturnProcedure) (RVArchitecturalOOC* arch, RVCSRSpace* CSRs);

    typedef struct {
        RVTrapEnterProcedure  TrapEnter  = nullptr;
        RVTrapReturnProcedure TrapReturn = nullptr;
    } RVTrapProcedures;
}