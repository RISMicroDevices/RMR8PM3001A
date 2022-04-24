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
        RVCodeGenConstraintRangeI(int code, uint64_t lower_range, uint64_t upper_range) noexcept;
        RVCodeGenConstraintRangeI(const RVCodeGenConstraintRangeI& obj) noexcept;
        ~RVCodeGenConstraintRangeI() noexcept;

        void        SetLowerRange(uint64_t lower_range) noexcept;
        void        SetUpperRange(uint64_t upper_range) noexcept;

        uint64_t    GetLowerRange() const noexcept;
        uint64_t    GetUpperRange() const noexcept;
    };

    // RISC-V Code Generator CSR-List constraint
    class RVCodeGenConstraintCSRList : public RVCodeGenConstraint {
    private:
        RVCSRList   list;

    public:
        RVCodeGenConstraintCSRList(int code) noexcept;
        RVCodeGenConstraintCSRList(int code, const RVCSRList& list) noexcept;
        RVCodeGenConstraintCSRList(const RVCodeGenConstraintCSRList& obj) noexcept;
        ~RVCodeGenConstraintCSRList() noexcept;

        int                 GetSize() const noexcept;

        RVCSRList&          GetList() noexcept;
        const RVCSRList&    GetList() const noexcept;

        void                SetList(const RVCSRList& list) noexcept;
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

    RVCodeGenConstraintRangeI::RVCodeGenConstraintRangeI(int code, uint64_t lower_range, uint64_t upper_range) noexcept
        : RVCodeGenConstraint   (code)
        , lower_range           (lower_range)
        , upper_range           (upper_range)
    { }

    RVCodeGenConstraintRangeI::RVCodeGenConstraintRangeI(const RVCodeGenConstraintRangeI& obj) noexcept
        : RVCodeGenConstraint   (obj)
        , lower_range           (obj.lower_range)
        , upper_range           (obj.upper_range)
    { }

    RVCodeGenConstraintRangeI::~RVCodeGenConstraintRangeI() noexcept
    { }

    inline uint64_t RVCodeGenConstraintRangeI::GetUpperRange() const noexcept
    {
        return upper_range;
    }

    inline uint64_t RVCodeGenConstraintRangeI::GetLowerRange() const noexcept
    {
        return lower_range;
    }

    inline void RVCodeGenConstraintRangeI::SetUpperRange(uint64_t upper_range) noexcept
    {
        this->upper_range = upper_range;
    }

    inline void RVCodeGenConstraintRangeI::SetLowerRange(uint64_t lower_range) noexcept
    {
        this->lower_range = lower_range;
    }
}


// Implementation of: class RVCodeGenConstraintCSRList
namespace Jasse {
    /*
    RVCSRList   list;
    */

    RVCodeGenConstraintCSRList::RVCodeGenConstraintCSRList(int code, const RVCSRList& list) noexcept
        : RVCodeGenConstraint   (code)
        , list                  (list)
    { }

    RVCodeGenConstraintCSRList::RVCodeGenConstraintCSRList(const RVCodeGenConstraintCSRList& obj) noexcept
        : RVCodeGenConstraint   (obj)
        , list                  (obj.list)
    { }

    RVCodeGenConstraintCSRList::~RVCodeGenConstraintCSRList() noexcept
    { }

    inline int RVCodeGenConstraintCSRList::GetSize() const noexcept
    {
        return list.GetSize();
    }

    inline RVCSRList& RVCodeGenConstraintCSRList::GetList() noexcept
    {
        return list;
    }

    inline const RVCSRList& RVCodeGenConstraintCSRList::GetList() const noexcept
    {
        return list;
    }

    inline void RVCodeGenConstraintCSRList::SetList(const RVCSRList& list) noexcept
    {
        this->list = list;
    }
}
