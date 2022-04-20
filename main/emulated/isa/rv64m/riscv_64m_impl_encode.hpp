#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M, codepoint infrastructure definitions
//

#include "riscvmisc.hpp"
#include "riscv_64m_def_encode.hpp"


#define __RV64M_D3CGER_OP(name) \
    &RV64MCodePoint_Funct3_##name, \
    &CodeGenRVTypeR<&AllocRVEncoderTypeR<RV_OPCODE_OP, RV64M_FUNCT3_##name, RV64M_FUNCT7_##name>>, \
    &RV64MExecutor_##name

#define __RV64M_D3CGER_OP32(name) \
    &RV64MCodePoint_Funct3_##name, \
    &CodeGenRVTypeR<&AllocRVEncoderTypeR<RV_OPCODE_OP_32, RV64M_FUNCT3_##name, RV64M_FUNCT7_##name>>, \
    &RV64MExecutor_##name
