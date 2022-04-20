#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, exporting fields
//

#include "riscv_64m_impl_decode.hpp"
#include "riscv_64m_impl_encode.hpp"
#include "riscv_64m_impl_execute.hpp"

// RV64M Codepoint Instances
namespace Jasse {

    decdef  RV64M    = new RV64MDecoder;

    codedef RV64M_MUL      {"mul"     , RVTYPE_R, __RV64M_D3CGER_OP(MUL)   , &TextualizeRVTypeR};
    codedef RV64M_MULH     {"mulh"    , RVTYPE_R, __RV64M_D3CGER_OP(MULH)  , &TextualizeRVTypeR};
    codedef RV64M_MULHSU   {"mulhsu"  , RVTYPE_R, __RV64M_D3CGER_OP(MULHSU), &TextualizeRVTypeR};
    codedef RV64M_MULHU    {"mulhu"   , RVTYPE_R, __RV64M_D3CGER_OP(MULHU) , &TextualizeRVTypeR};

    codedef RV64M_DIV      {"div"     , RVTYPE_R, __RV64M_D3CGER_OP(DIV) , &TextualizeRVTypeR};
    codedef RV64M_DIVU     {"divu"    , RVTYPE_R, __RV64M_D3CGER_OP(DIVU), &TextualizeRVTypeR};
    codedef RV64M_REM      {"rem"     , RVTYPE_R, __RV64M_D3CGER_OP(REM) , &TextualizeRVTypeR};
    codedef RV64M_REMU     {"remu"    , RVTYPE_R, __RV64M_D3CGER_OP(REMU), &TextualizeRVTypeR};

    codedef RV64M_MULW     {"mulw"    , RVTYPE_R, __RV64M_D3CGER_OP32(MULW), &TextualizeRVTypeR};

    codedef RV64M_DIVW     {"divw"    , RVTYPE_R, __RV64M_D3CGER_OP32(DIVW) , &TextualizeRVTypeR};
    codedef RV64M_DIVUW    {"divuw"   , RVTYPE_R, __RV64M_D3CGER_OP32(DIVUW), &TextualizeRVTypeR};

    codedef RV64M_REMW     {"remw"    , RVTYPE_R, __RV64M_D3CGER_OP32(REMW) , &TextualizeRVTypeR};
    codedef RV64M_REMUW    {"remuw"   , RVTYPE_R, __RV64M_D3CGER_OP32(REMUW), &TextualizeRVTypeR};


    codesetdef ALL_OF_RV64M = {
        &RV64M_MUL,     &RV64M_MULH,    &RV64M_MULHSU,  &RV64M_MULHU,
        &RV64M_DIV,     &RV64M_DIVU,
        &RV64M_REM,     &RV64M_REMU,
        &RV64M_MULW,
        &RV64M_DIVW,    &RV64M_DIVUW,
        &RV64M_REMW,    &RV64M_REMUW
    };
}
