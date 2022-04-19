#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR", encoding definitions
//

#include "riscvmisc.hpp"

//
#define ENCODE_RVZICSR_CSRRW(rd, rs1, csr)      ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRW, rd, rs1, csr)
#define ENCODE_RVZICSR_CSRRS(rd, rs1, csr)      ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRS, rd, rs1, csr)
#define ENCODE_RVZICSR_CSRRC(rd, rs1, csr)      ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRC, rd, rs1, csr)

#define ENCODE_RVZICSR_CSRRWI(rd, uimm, csr)    ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRWI, rd, uimm, csr)
#define ENCODE_RVZICSR_CSRRSI(rd, uimm, csr)    ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRSI, rd, uimm, csr)
#define ENCODE_RVZICSR_CSRRCI(rd, uimm, csr)    ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRCI, rd, uimm, csr)
