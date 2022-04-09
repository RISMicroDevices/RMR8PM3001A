#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Instruction Generator (CodeGen) components
//

#include "riscvdef.hpp"
#include "riscvmisc.hpp"


#define RV_CODEGEN_CONSTRAINT_MASK          0x00FF

#define RV_CODEGEN_RANGEI                   0x0001

#define RV_CODEGEN_RANGEI_PC                0x0101
#define RV_CODEGEN_RANGEI_GRx               0x0201
#define RV_CODEGEN_RANGEI_MEM               0x0301


namespace Jasse {

    // RISC-V Code Generator constraint base class
    class RVCodeGenConstraint {
    private:
        int code;

    public:
        RVCodeGenConstraint(int code);
        ~RVCodeGenConstraint();

        int     GetConstraintCode() const;
        int     GetConstraintTypeCode() const;    
    };

    // RISC-V Code Generator integer range constraint
    class RVCodeGenConstraintRangeI : public RVCodeGenConstraint {
    private:
        uint64_t    lower_range;
        uint64_t    upper_range;

    public:
        RVCodeGenConstraintRangeI(uint64_t lower_range, uint64_t upper_range);
        ~RVCodeGenConstraintRangeI();

        void        SetLowerRange(uint64_t lower_range);
        void        SetUpperRange(uint64_t upper_range);

        uint64_t    GetLowerRange() const;
        uint64_t    GetUpperRange() const;
    };

    //
    class RVCodeGenConstraints {

    };
}
