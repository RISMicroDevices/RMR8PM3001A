#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, exporting fields declarations
//

#include "riscv.hpp"


// RV64M Codepoint Instances
namespace Jasse {

    decdecl  RV64M;

    codedecl RV64M_MUL;
    codedecl RV64M_MULH;
    codedecl RV64M_MULHSU;
    codedecl RV64M_MULHU;

    codedecl RV64M_DIV;
    codedecl RV64M_DIVU;
    codedecl RV64M_REM;
    codedecl RV64M_REMU;

    codedecl RV64M_MULW;

    codedecl RV64M_DIVW;
    codedecl RV64M_DIVUW;

    codedecl RV64M_REMW;
    codedecl RV64M_REMUW;

    codesetdecl ALL_OF_RV64M;
}
