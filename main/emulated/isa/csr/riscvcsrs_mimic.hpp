#pragma once
//
// RISC-V Instruction Set Architecture CSRs (Control and Status Register)
// Jasse MIMIC CUSTOM debugging CSRs
// 
//

#include "riscvcsr_dmimic.hpp"
#include "riscvcsr_dmimicrdcnt.hpp"
#include "riscvcsr_dmimicwrcnt.hpp"
#include "riscvcsr_dmimicrinc.hpp"
#include "riscvcsr_dmimicwinc.hpp"


namespace Jasse::CSR {

    static const RVCSRList DEBUG_CODEGEN_CANDIDATE_OF_MIMIC {
        dmimic0,        dmimic1,        dmimic2,        dmimic3,
        dmimic4,        dmimic5,        dmimic6,        dmimic7,
        dmimic8,        dmimic9,        dmimic10,       dmimic11,
        dmimic12,       dmimic13,       dmimic14,       dmimic15,
        dmimicrinc0,    dmimicrinc1,    dmimicrinc2,    dmimicrinc3,
        dmimicrinc4,    dmimicrinc5,    dmimicrinc6,    dmimicrinc7,
        dmimicwinc0,    dmimicwinc1,    dmimicwinc2,    dmimicwinc3,
        dmimicwinc4,    dmimicwinc5,    dmimicwinc6,    dmimicwinc7,
    };
}
