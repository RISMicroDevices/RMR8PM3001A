#pragma once
//
// RISC-V Instruction Set Architecture 
//
// Extension RV64M
//

#ifndef __INCLUDE_RV64M
#define __INCLUDE_RV64M
#endif

#include <cstdint>

#include "gmp.h"

#include "riscv.hpp"
#include "riscvdef.hpp"
#include "riscvmisc.hpp"

#include "riscv_32m.hpp"


// Function-3
#define RV64M_FUNCT3_MASK                       0x00007000
#define RV64M_FUNCT3_OFFSET                     12

#define RV64M_FUNCT3_MULW                       0b000
#define RV64M_FUNCT3_DIVW                       0b100
#define RV64M_FUNCT3_DIVUW                      0b101
#define RV64M_FUNCT3_REMW                       0b110
#define RV64M_FUNCT3_REMUW                      0b111


// Function-7
#define RV64M_FUNCT7_MASK                       0xFE000000
#define RV64M_FUNCT7_OFFSET                     25

#define RV64M_FUNCT7_MULW                       0b0000001
#define RV64M_FUNCT7_DIVW                       0b0000001
#define RV64M_FUNCT7_DIVUW                      0b0000001
#define RV64M_FUNCT7_REMW                       0b0000001
#define RV64M_FUNCT7_REMUW                      0b0000001



// executors
namespace Jasse {

#define RV64M_EXECUTOR_PARAMS       const RVInstruction& insn, RVArchitecturalOOC* arch, RVMemoryInterface* MI, RVCSRSpace* CSRs

    // MUL
    RVExecStatus RV64MExecutor_MUL(RV64M_EXECUTOR_PARAMS)
    {
        arch->GR64()->Set(insn.GetRD(),
            arch->GR64()->Get(insn.GetRS1()) * arch->GR64()->Get(insn.GetRS2()));

        return EXEC_SEQUENTIAL;
    }

    // MULH
    RVExecStatus RV64MExecutor_MULH(RV64M_EXECUTOR_PARAMS)
    {
        int64_t multiplicand = (int64_t) arch->GR64()->Get(insn.GetRS1());
        int64_t multiplier   = (int64_t) arch->GR64()->Get(insn.GetRS2());
        
        int64_t result;

        // gmp field
        mpz_t mpz_s, mpz_i, mpz_r;

        mpz_inits(mpz_s, mpz_i, mpz_r, nullptr);

        mpz_set_si(mpz_s, multiplicand);
        mpz_mul_si(mpz_i, mpz_s, multiplier); // mul
        
        mpz_tdiv_q_2exp(mpz_r, mpz_i, 64);    // shift

        ASSERTM(mpz_fits_slong_p(mpz_r), "mpz mulh overflow");

        result = mpz_get_si(mpz_r);

        mpz_clears(mpz_s, mpz_i, mpz_r, nullptr);
        //

        arch->GR64()->Set(insn.GetRD(), (uint64_t) result);

        return EXEC_SEQUENTIAL;
    }

    // MULHU
    RVExecStatus RV64MExecutor_MULHU(RV64M_EXECUTOR_PARAMS)
    {
        uint64_t multiplicand = arch->GR64()->Get(insn.GetRS1());
        uint64_t multiplier   = arch->GR64()->Get(insn.GetRS2());
        
        uint64_t result;

        // gmp field
        mpz_t mpz_s, mpz_i, mpz_r;

        mpz_inits(mpz_s, mpz_i, mpz_r, nullptr);

        mpz_set_ui(mpz_s, multiplicand);
        mpz_mul_ui(mpz_i, mpz_s, multiplier); // mul
        
        mpz_tdiv_q_2exp(mpz_r, mpz_i, 64);    // shift

        ASSERTM(mpz_fits_ulong_p(mpz_r), "mpz mulhu overflow");

        result = mpz_get_ui(mpz_r);

        mpz_clears(mpz_s, mpz_i, mpz_r, nullptr);
        //

        arch->GR64()->Set(insn.GetRD(), result);

        return EXEC_SEQUENTIAL;
    }

    // MULHSU
    RVExecStatus RV64MExecutor_MULHSU(RV64M_EXECUTOR_PARAMS)
    {
        int64_t  multiplicand = arch->GR64()->Get(insn.GetRS1());
        uint64_t multiplier   = arch->GR64()->Get(insn.GetRS2());

        int64_t result;

        // gmp field
        mpz_t mpz_s, mpz_i, mpz_r;

        mpz_inits(mpz_s, mpz_i, mpz_r, nullptr);

        mpz_set_si(mpz_s, multiplicand);
        mpz_mul_ui(mpz_i, mpz_s, multiplier); // mul
        
        mpz_tdiv_q_2exp(mpz_r, mpz_i, 64);    // shift

        ASSERT(mpz_fits_slong_p(mpz_r));

        result = mpz_get_si(mpz_r);

        mpz_clears(mpz_s, mpz_i, mpz_r, nullptr);
        //

        arch->GR64()->Set(insn.GetRD(), (uint64_t) result);

        return EXEC_SEQUENTIAL;
    }

    // MULW
    RVExecStatus RV64MExecutor_MULW(RV64M_EXECUTOR_PARAMS)
    {
        uint32_t result =  
            ((uint32_t) arch->GR64()->Get(insn.GetRS1())) * ((uint32_t) arch->GR64()->Get(insn.GetRS2()));

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }

    // DIV
    RVExecStatus RV64MExecutor_DIV(RV64M_EXECUTOR_PARAMS)
    {
        int64_t dividend = (int64_t) arch->GR64()->Get(insn.GetRS1());
        int64_t divisor  = (int64_t) arch->GR64()->Get(insn.GetRS2());

        int64_t result;

        if (dividend == INT64_MIN && divisor == -1) // overflow
            result = INT64_MIN;
        else if (!divisor) // divide by zero
            result = -1;
        else
            result = dividend / divisor;

        arch->GR64()->Set(insn.GetRD(), (uint64_t) result);

        return EXEC_SEQUENTIAL;
    }

    // REM
    RVExecStatus RV64MExecutor_REM(RV64M_EXECUTOR_PARAMS)
    {
        int64_t dividend = (int64_t) arch->GR64()->Get(insn.GetRS1());
        int64_t divisor  = (int64_t) arch->GR64()->Get(insn.GetRS2());

        int64_t result;

        if (dividend == INT64_MIN && divisor == -1) // overflow
            result = 0;
        else if (!divisor) // divide by zero
            result = dividend;
        else
            result = dividend % divisor;

        arch->GR64()->Set(insn.GetRD(), (uint64_t) result);

        return EXEC_SEQUENTIAL;
    }

    // DIVU
    RVExecStatus RV64MExecutor_DIVU(RV64M_EXECUTOR_PARAMS)
    {
        uint64_t dividend = arch->GR64()->Get(insn.GetRS1());
        uint64_t divisor  = arch->GR64()->Get(insn.GetRS2());

        uint64_t result;

        if (!divisor) // divide by zero
            result = UINT64_MAX;
        else
            result = dividend / divisor;

        arch->GR64()->Set(insn.GetRD(), result);

        return EXEC_SEQUENTIAL;
    }

    // REMU
    RVExecStatus RV64MExecutor_REMU(RV64M_EXECUTOR_PARAMS)
    {
        uint64_t dividend = arch->GR64()->Get(insn.GetRS1());
        uint64_t divisor  = arch->GR64()->Get(insn.GetRS2());

        uint64_t result;

        if (!divisor) // divide by zero
            result = dividend;
        else
            result = dividend % divisor;

        arch->GR64()->Set(insn.GetRD(), result);

        return EXEC_SEQUENTIAL;
    }

    // DIVW
    RVExecStatus RV64MExecutor_DIVW(RV64M_EXECUTOR_PARAMS)
    {
        int32_t dividend = (int32_t) arch->GR64()->Get(insn.GetRS1());
        int32_t divisor  = (int32_t) arch->GR64()->Get(insn.GetRS2());

        int32_t result;

        if (dividend == INT32_MIN && divisor == -1) // overflow
            result = INT32_MIN;
        else if (!divisor) // divide by zero
            result = -1;
        else
            result = dividend / divisor;

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }

    // REMW
    RVExecStatus RV64MExecutor_REMW(RV64M_EXECUTOR_PARAMS)
    {
        int32_t dividend = (int32_t) arch->GR64()->Get(insn.GetRS1());
        int32_t divisor  = (int32_t) arch->GR64()->Get(insn.GetRS2());

        int32_t result;

        if (dividend == INT32_MIN && divisor == -1) // overflow
            result = 0;
        else if (!divisor) // divide by zero
            result = dividend;
        else
            result = dividend % divisor;

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }

    // DIVUW
    RVExecStatus RV64MExecutor_DIVUW(RV64M_EXECUTOR_PARAMS)
    {
        uint32_t dividend = (uint32_t) arch->GR64()->Get(insn.GetRS1());
        uint32_t divisor  = (uint32_t) arch->GR64()->Get(insn.GetRS2());

        uint32_t result;

        if (!divisor) // divide by zero
            result = UINT32_MAX;
        else
            result = dividend / divisor;

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }

    // REMUW
    RVExecStatus RV64MExecutor_REMUW(RV64M_EXECUTOR_PARAMS)
    {
        uint32_t dividend = (uint32_t) arch->GR64()->Get(insn.GetRS1());
        uint32_t divisor  = (uint32_t) arch->GR64()->Get(insn.GetRS2());

        uint32_t result;

        if (!divisor) // divide by zero
            result = dividend;
        else
            result = dividend % divisor;

        arch->GR64()->Set(insn.GetRD(), SEXT_W(result));

        return EXEC_SEQUENTIAL;
    }
}


// RV64M Codepoint Instances
namespace Jasse {

//

    static const RVCodepoint RV64M_MUL      = RVCodepoint("mul"     , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_MUL);
    static const RVCodepoint RV64M_MULH     = RVCodepoint("mulh"    , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_MULH);
    static const RVCodepoint RV64M_MULHSU   = RVCodepoint("mulhsu"  , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_MULHSU);
    static const RVCodepoint RV64M_MULHU    = RVCodepoint("mulhu"   , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_MULHU);

    static const RVCodepoint RV64M_DIV      = RVCodepoint("div"     , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_DIV);
    static const RVCodepoint RV64M_DIVU     = RVCodepoint("divu"    , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_DIVU);
    
    static const RVCodepoint RV64M_REM      = RVCodepoint("rem"     , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_REM);
    static const RVCodepoint RV64M_REMU     = RVCodepoint("remu"    , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_REMU);

    static const RVCodepoint RV64M_MULW     = RVCodepoint("mulw"    , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_MULW);

    static const RVCodepoint RV64M_DIVW     = RVCodepoint("divw"    , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_DIVW);
    static const RVCodepoint RV64M_DIVUW    = RVCodepoint("divuw"   , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_DIVUW);

    static const RVCodepoint RV64M_REMW     = RVCodepoint("remw"    , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_REMW);
    static const RVCodepoint RV64M_REMUW    = RVCodepoint("remuw"   , RVTYPE_R, &TextualizeRVTypeR, &RV64MExecutor_REMUW);
}


#define RV64M_DECINSN(funct7, T, codepoint) \
    (GET_STD_OPERAND(insnraw, RV64M_FUNCT7) == funct7 \
        ? (DecodeNormalRV64Type##T(insnraw, insn), insn.SetTrait(codepoint), true) \
        : false) \

// codepoints - decoder facilities
namespace Jasse {

    // 
    bool RV64MCodePoint_Funct3_MUL(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_MUL, R, RV64M_MUL);
    }

    bool RV64MCodePoint_Funct3_MULH(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_MULH, R, RV64M_MULH);
    }

    bool RV64MCodePoint_Funct3_MULHSU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_MULHSU, R, RV64M_MULHSU);
    }

    bool RV64MCodePoint_Funct3_MULHU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_MULHU, R, RV64M_MULHU);
    }

    bool RV64MCodePoint_Funct3_DIV(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_DIV, R, RV64M_DIV);
    }

    bool RV64MCodePoint_Funct3_DIVU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_DIVU, R, RV64M_DIVU);
    }

    bool RV64MCodePoint_Funct3_REM(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_REM, R, RV64M_REM);
    }

    bool RV64MCodePoint_Funct3_REMU(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV32M_FUNCT7_REMU, R, RV64M_REMU);
    }

    bool RV64MCodePoint_Funct3_MULW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_MULW, R, RV64M_MULW);
    }

    bool RV64MCodePoint_Funct3_DIVW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_DIVW, R, RV64M_DIVW);
    }

    bool RV64MCodePoint_Funct3_DIVUW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_DIVUW, R, RV64M_DIVUW);
    }

    bool RV64MCodePoint_Funct3_REMW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_REMW, R, RV64M_REMW);
    }

    bool RV64MCodePoint_Funct3_REMUW(insnraw_t insnraw, RVInstruction& insn)
    {
        return RV64M_DECINSN(RV64M_FUNCT7_REMUW, R, RV64M_REMUW);
    }
}

namespace Jasse {
    // RV64M codepoint
    typedef bool (*RV64MCodepoint)(insnraw_t insnraw, RVInstruction& insn);

    // RV64M decoder
    class RV64MDecoder : public RVDecoder {
    private:
        RV64MCodepoint*     codepoints64;  // 8
        RV64MCodepoint*     codepoints32;  // 8

    public:
        RV64MDecoder();
        ~RV64MDecoder();

        virtual bool    Decode(insnraw_t insnraw, RVInstruction& insn) const override;
    };

    // RV64M decoder instance
    static const RV64MDecoder* const    RV64M = new RV64MDecoder;
}


// Implementation of: RV64MDecoder
namespace Jasse {
    /*
    RV64MCodepoint*     codepoints64;  // 8
    RV64MCodepoint*     codepoints32;  // 8
    */

    RV64MDecoder::RV64MDecoder()
        : RVDecoder     ("M", "RV64M")
        , codepoints64  (new RV64MCodepoint[8]())
        , codepoints32  (new RV64MCodepoint[8]())
    {
        codepoints64[RV32M_FUNCT3_MUL]      = &RV64MCodePoint_Funct3_MUL;
        codepoints64[RV32M_FUNCT3_MULH]     = &RV64MCodePoint_Funct3_MULH;
        codepoints64[RV32M_FUNCT3_MULHSU]   = &RV64MCodePoint_Funct3_MULHSU;
        codepoints64[RV32M_FUNCT3_MULHU]    = &RV64MCodePoint_Funct3_MULHU;
        codepoints64[RV32M_FUNCT3_DIV]      = &RV64MCodePoint_Funct3_DIV;
        codepoints64[RV32M_FUNCT3_DIVU]     = &RV64MCodePoint_Funct3_DIVU;
        codepoints64[RV32M_FUNCT3_REM]      = &RV64MCodePoint_Funct3_REM;
        codepoints64[RV32M_FUNCT3_REMU]     = &RV64MCodePoint_Funct3_REMU;

        codepoints32[RV64M_FUNCT3_MULW]     = &RV64MCodePoint_Funct3_MULW;
        codepoints32[RV64M_FUNCT3_DIVW]     = &RV64MCodePoint_Funct3_DIVW;
        codepoints32[RV64M_FUNCT3_DIVUW]    = &RV64MCodePoint_Funct3_DIVUW;
        codepoints32[RV64M_FUNCT3_REMW]     = &RV64MCodePoint_Funct3_REMW;
        codepoints32[RV64M_FUNCT3_REMUW]    = &RV64MCodePoint_Funct3_REMUW;
    }

    RV64MDecoder::~RV64MDecoder()
    {
        delete[] codepoints64;
        delete[] codepoints32;
    }

    bool RV64MDecoder::Decode(insnraw_t insnraw, RVInstruction& insn) const
    {
        int opcode = GET_STD_OPERAND(insnraw, RV_OPCODE);

        RV64MCodepoint* codepoints;

        if (opcode == RV_OPCODE_OP)
            codepoints = codepoints64;
        else if (opcode == RV_OPCODE_OP_32)
            codepoints = codepoints32;
        else
            return false;

        RV64MCodepoint codepoint = codepoints[GET_STD_OPERAND(insnraw, RV64M_FUNCT3)];

        if (!codepoint)
            return false;

        return codepoint(insnraw, insn);
    }
}
