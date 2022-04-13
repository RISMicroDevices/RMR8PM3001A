#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension "ZiCSR"
//

#include "riscv.hpp"
#include "riscvmisc.hpp"

#include "riscvcsr.hpp"
#include "riscvexcept.hpp"


// Function-3
#define RVZICSR_FUNCT3_MASK                     RV_FUNCT3_MASK
#define RVZICSR_FUNCT3_OFFSET                   RV_FUNCT3_OFFSET

#define RVZICSR_FUNCT3_CSRRW                    0b001
#define RVZICSR_FUNCT3_CSRRS                    0b010
#define RVZICSR_FUNCT3_CSRRC                    0b011
#define RVZICSR_FUNCT3_CSRRWI                   0b101
#define RVZICSR_FUNCT3_CSRRSI                   0b110
#define RVZICSR_FUNCT3_CSRRCI                   0b111



// executors
namespace Jasse {

#define RVZICSR_EXECUTOR_PARAMS     const RVInstruction& insn, RVArchitecturalOOC* arch, RVMemoryInterface* MI, RVCSRSpace* CSRs    

#define __RVZICSR_ACQUIRE_CSR(csr, insn) \
    RVCSR* csr = CSRs->GetCSR(GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_CSR)); \
    if (!csr) \
    { \
        TrapEnter(arch, CSRs, TRAP_EXCEPTION, EXCEPTION_ILLEGAL_INSTRUCTION); \
        return EXEC_TRAP_ENTER; \
    }


    // CSRRW
    RVExecStatus RVZicsrExecutor_CSRRW(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn)

        if (insn.GetRD())
            arch->SetGRx64(insn.GetRD(), csr->GetValue());

        csr->SetValue(arch->GetGRx64Zext(insn.GetRS1()));

        return EXEC_SEQUENTIAL;
    }

    // CSRRS
    RVExecStatus RVZicsrExecutor_CSRRS(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn)

        csr_t csrval  = csr->GetValue();
        csr_t csrmask = arch->GetGRx64Zext(insn.GetRS1());

        arch->SetGRx64(insn.GetRD(), csrval);

        csrval = SET_CSR_BITS(csrval, csrmask);

        csr->SetValue(csrval);

        return EXEC_SEQUENTIAL;
    }

    // CSRRC
    RVExecStatus RVZicsrExecutor_CSRRC(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn)

        csr_t csrval  = csr->GetValue();
        csr_t csrmask = arch->GetGRx64Zext(insn.GetRS1());

        arch->SetGRx64(insn.GetRD(), csrval);

        csrval = CLEAR_CSR_BITS(csrval, csrmask);

        csr->SetValue(csrval);

        return EXEC_SEQUENTIAL;
    }

    // CSRRWI
    RVExecStatus RVZicsrExecutor_CSRRWI(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn)

        if (insn.GetRD())
            arch->SetGRx64(insn.GetRD(), csr->GetValue());

        csr->SetValue(arch->GetGRx64Zext(insn.GetRS1()));

        return EXEC_SEQUENTIAL;
    }

    // CSRRSI
    RVExecStatus RVZicsrExecutor_CSRRSI(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn);

        csr_t csrval  = csr->GetValue();
        csr_t csrmask = GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_CSR_UIMM);

        arch->SetGRx64(insn.GetRD(), csrval);

        csrval = SET_CSR_BITS(csrval, csrmask);

        csr->SetValue(csrval);

        return EXEC_SEQUENTIAL;   
    }

    // CSRRCI
    RVExecStatus RVZicsrExecutor_CSRRCI(RVZICSR_EXECUTOR_PARAMS)
    {
        __RVZICSR_ACQUIRE_CSR(csr, insn);

        csr_t csrval  = csr->GetValue();
        csr_t csrmask = GET_STD_OPERAND(insn.GetRaw(), RV_OPERAND_CSR_UIMM);

        arch->SetGRx64(insn.GetRD(), csrval);

        csrval = CLEAR_CSR_BITS(csrval, csrmask);

        csr->SetValue(csrval);

        return EXEC_SEQUENTIAL;
    }
}


// RV Codepoint instances
namespace Jasse {

//  static const RVZicsrDecoder* const  RVZicsr = new RVZicsrDecoder;

    static const RVCodepoint RVZicsr_CSRRW  = RVCodepoint("csrrw"   , RVTYPE_I, &TextualizeRVTypeI, &RVZicsrExecutor_CSRRW);
    static const RVCodepoint RVZicsr_CSRRS  = RVCodepoint("csrrs"   , RVTYPE_I, &TextualizeRVTypeI, &RVZicsrExecutor_CSRRS);
    static const RVCodepoint RVZicsr_CSRRC  = RVCodepoint("csrrc"   , RVTYPE_I, &TextualizeRVTypeI, &RVZicsrExecutor_CSRRC);

    static const RVCodepoint RVZicsr_CSRRWI = RVCodepoint("csrrwi"  , RVTYPE_I, &TextualizeRVTypeI, &RVZicsrExecutor_CSRRWI);
    static const RVCodepoint RVZicsr_CSRRSI = RVCodepoint("csrrsi"  , RVTYPE_I, &TextualizeRVTypeI, &RVZicsrExecutor_CSRRSI);
    static const RVCodepoint RVZicsr_CSRRCI = RVCodepoint("csrrci"  , RVTYPE_I, &TextualizeRVTypeI, &RVZicsrExecutor_CSRRCI);


    static const RVCodepointCollection __RVZicsr_ALL = {
        &RVZicsr_CSRRW,     &RVZicsr_CSRRS,     &RVZicsr_CSRRC,
        &RVZicsr_CSRRWI,    &RVZicsr_CSRRSI,    &RVZicsr_CSRRCI
    };
}


//
#define ENCODE_RVZICSR_CSRRW(rd, rs1, csr)      ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRW, rd, rs1, csr)
#define ENCODE_RVZICSR_CSRRS(rd, rs1, csr)      ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRS, rd, rs1, csr)
#define ENCODE_RVZICSR_CSRRC(rd, rs1, csr)      ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRC, rd, rs1, csr)

#define ENCODE_RVZICSR_CSRRWI(rd, uimm, csr)    ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRWI, rd, uimm, csr)
#define ENCODE_RVZICSR_CSRRSI(rd, uimm, csr)    ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRSI, rd, uimm, csr)
#define ENCODE_RVZICSR_CSRRCI(rd, uimm, csr)    ENCODE_RVTYPE_I(RV_OPCODE_SYSTEM, RVZICSR_FUNCT3_CSRRCI, rd, uimm, csr)



//
#define RVZICSR_DECINSN(T, codepoint) \
    (DecodeNormalRV64Type##T(insnraw, insn), insn.SetTrait(codepoint), true)

// codepoints - decoder facilities
namespace Jasse {
    
    //
    bool RVZicsrCodePoint_Funct3_CSRRW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRW);
    }

    bool RVZicsrCodePoint_Funct3_CSRRS(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRS);
    }

    bool RVZicsrCodePoint_Funct3_CSRRC(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRC);
    }

    bool RVZicsrCodePoint_Funct3_CSRRWI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRWI);
    }

    bool RVZicsrCodePoint_Funct3_CSRRSI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRSI);
    }

    bool RVZicsrCodePoint_Funct3_CSRRCI(insnraw_t insnraw, RVInstruction& insn)
    {
        return RVZICSR_DECINSN(I, RVZicsr_CSRRCI);
    }
}

namespace Jasse {
    // RVZicsr codepoint
    typedef bool (*RVZicsrCodepoint)(insnraw_t insnraw, RVInstruction& insn);

    // RVZicsr Decoder
    class RVZicsrDecoder : public RVDecoder {
    private:
        RVZicsrCodepoint*   codepoints; // 8 (2 ^ 3)

    public:
        RVZicsrDecoder();
        ~RVZicsrDecoder();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;

        virtual const RVCodepointCollection&    GetAllCodepoints() const override;
    };

    static const RVZicsrDecoder* const  RVZicsr = new RVZicsrDecoder;
}


// Implementation of: class RVZicsrDecoder
namespace Jasse {
    /*
    RVZicsrCodepoint*   codepoints;
    */

    RVZicsrDecoder::RVZicsrDecoder()
        : RVDecoder ("Zicsr", "Zicsr")
        , codepoints(new RVZicsrCodepoint[8]())
    {
        codepoints[RVZICSR_FUNCT3_CSRRW]    = &RVZicsrCodePoint_Funct3_CSRRW;
        codepoints[RVZICSR_FUNCT3_CSRRS]    = &RVZicsrCodePoint_Funct3_CSRRS;
        codepoints[RVZICSR_FUNCT3_CSRRC]    = &RVZicsrCodePoint_Funct3_CSRRC;

        codepoints[RVZICSR_FUNCT3_CSRRWI]   = &RVZicsrCodePoint_Funct3_CSRRWI;
        codepoints[RVZICSR_FUNCT3_CSRRSI]   = &RVZicsrCodePoint_Funct3_CSRRSI;
        codepoints[RVZICSR_FUNCT3_CSRRCI]   = &RVZicsrCodePoint_Funct3_CSRRCI;
    }

    RVZicsrDecoder::~RVZicsrDecoder()
    {
        delete[] codepoints;
    }

    bool RVZicsrDecoder::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        if (GET_STD_OPERAND(insnraw, RV_OPCODE) != RV_OPCODE_SYSTEM)
            return false;

        RVZicsrCodepoint codepoint = codepoints[GET_STD_OPERAND(insnraw, RVZICSR_FUNCT3)];

        if (!codepoint)
            return false;

        return codepoint(insnraw, insn);
    }

    const RVCodepointCollection& RVZicsrDecoder::GetAllCodepoints() const
    {
        return __RVZicsr_ALL;
    }
}
