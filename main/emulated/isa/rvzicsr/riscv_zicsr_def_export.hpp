#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR"
//

#include "riscv.hpp"

// RV Codepoint instances
namespace Jasse {

    decdecl  RVZicsr;

    codedecl RVZicsr_CSRRW;
    codedecl RVZicsr_CSRRS;
    codedecl RVZicsr_CSRRC;

    codedecl RVZicsr_CSRRWI;
    codedecl RVZicsr_CSRRSI;
    codedecl RVZicsr_CSRRCI;

    codesetdecl ALL_OF_RVZicsr;
}
