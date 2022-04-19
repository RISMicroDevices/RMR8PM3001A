#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR", exporting fields
//

#include "riscv_zicsr_impl_decode.hpp"
#include "riscv_zicsr_impl_encode.hpp"
#include "riscv_zicsr_impl_execute.hpp"

// RV Codepoint instances
namespace Jasse {

    decdef  RVZicsr = new RVZicsrDecoder;

    codedef RVZicsr_CSRRW   {"csrrw"   , RVTYPE_I, __RVZicsr_D3CGI_CSRx(CSRRW), &TextualizeRVTypeI};
    codedef RVZicsr_CSRRS   {"csrrs"   , RVTYPE_I, __RVZicsr_D3CGI_CSRx(CSRRS), &TextualizeRVTypeI};
    codedef RVZicsr_CSRRC   {"csrrc"   , RVTYPE_I, __RVZicsr_D3CGI_CSRx(CSRRC), &TextualizeRVTypeI};

    codedef RVZicsr_CSRRWI  {"csrrwi"  , RVTYPE_I, __RVZicsr_D3CGI_CSRxI(CSRRWI), &TextualizeRVTypeI};
    codedef RVZicsr_CSRRSI  {"csrrsi"  , RVTYPE_I, __RVZicsr_D3CGI_CSRxI(CSRRSI), &TextualizeRVTypeI};
    codedef RVZicsr_CSRRCI  {"csrrci"  , RVTYPE_I, __RVZicsr_D3CGI_CSRxI(CSRRCI), &TextualizeRVTypeI};

    codesetdef ALL_OF_RVZicsr = {
        &RVZicsr_CSRRW,     &RVZicsr_CSRRS,     &RVZicsr_CSRRC,
        &RVZicsr_CSRRWI,    &RVZicsr_CSRRSI,    &RVZicsr_CSRRCI
    };
}
