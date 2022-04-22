#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Instruction Generator (CodeGen) Standard components
//

#include "riscv.hpp"


//
#define RV_CODEGEN_CODE_RANGE_I                     0x00

#define RV_CODEGEN_CODE_RANGE_I_GRx                 0x00
#define RV_CODEGEN_CODE_RANGE_I_IMM                 0x10

//
#define RV_CODEGEN_CODE_CSR                         0x08

#define RV_CODEGEN_CODE_CSR_CANDIDATE               0x08


namespace Jasse {
    // RISC-V Code Generator integer range constraint
    class RVCodeGenConstraintRangeI : public RVCodeGenConstraint {
    private:
        uint64_t    lower_range;
        uint64_t    upper_range;

    public:
        RVCodeGenConstraintRangeI(int code, uint64_t lower_range, uint64_t upper_range);
        RVCodeGenConstraintRangeI(const RVCodeGenConstraintRangeI& obj);
        ~RVCodeGenConstraintRangeI();

        void        SetLowerRange(uint64_t lower_range);
        void        SetUpperRange(uint64_t upper_range);

        uint64_t    GetLowerRange() const;
        uint64_t    GetUpperRange() const;
    };

    // RISC-V Code Generator CSR-List constraint
    class RVCodeGenConstraintCSRList : public RVCodeGenConstraint {
    private:
        RVCSRList   list;

    public:
        

        // TODO
    };
}


// RISC-V Code Generator constraint traits
namespace Jasse {
    
    static constexpr RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>    RV_CODEGEN_GRx_RANGE
        { RV_CODEGEN_CODE_RANGE_I_GRx };

    static constexpr RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>    RV_CODEGEN_IMM_RANGE
        { RV_CODEGEN_CODE_RANGE_I_IMM };

    
    static constexpr RVCodeGenConstraintTrait<RVCodeGenConstraintCSRList>   RV_CODEGEN_CSR_CANDIDATE
        { RV_CODEGEN_CODE_CSR_CANDIDATE };
}



// Implementation of: class RVCodeGenConstraintRangeI
namespace Jasse {
    /*
    uint64_t    lower_range;

    uint64_t    upper_range;
    */

    RVCodeGenConstraintRangeI::RVCodeGenConstraintRangeI(int code, uint64_t lower_range, uint64_t upper_range)
        : RVCodeGenConstraint   (code)
        , lower_range           (lower_range)
        , upper_range           (upper_range)
    { }

    RVCodeGenConstraintRangeI::RVCodeGenConstraintRangeI(const RVCodeGenConstraintRangeI& obj)
        : RVCodeGenConstraint   (obj)
        , lower_range           (obj.lower_range)
        , upper_range           (obj.upper_range)
    { }

    RVCodeGenConstraintRangeI::~RVCodeGenConstraintRangeI()
    { }

    inline uint64_t RVCodeGenConstraintRangeI::GetUpperRange() const
    {
        return upper_range;
    }

    inline uint64_t RVCodeGenConstraintRangeI::GetLowerRange() const
    {
        return lower_range;
    }

    inline void RVCodeGenConstraintRangeI::SetUpperRange(uint64_t upper_range)
    {
        this->upper_range = upper_range;
    }

    inline void RVCodeGenConstraintRangeI::SetLowerRange(uint64_t lower_range)
    {
        this->lower_range = lower_range;
    }
}
