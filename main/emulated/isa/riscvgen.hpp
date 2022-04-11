#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Instruction Generator (CodeGen) components
//

#include "riscvdef.hpp"
#include "riscvmisc.hpp"


#define RV_CODEGEN_CONSTRAINT_MASK          0x0F

#define RV_CODEGEN_RANGE_I                    0x01

#define RV_CODEGEN_RANGE_I_PC               0x01
#define RV_CODEGEN_RANGE_I_GRx              0x11
#define RV_CODEGEN_RANGE_I_MEM              0x21


namespace Jasse {

    // RISC-V Code Generator constraint trait
    template<class TConstraint>
    class RVCodeGenConstraintTrait {
    private:
        const int code;

    public:
        constexpr RVCodeGenConstraintTrait(int code);

        constexpr int     GetCode() const;
    };


    // RISC-V Code Generator constraint base class
    class RVCodeGenConstraint {
    private:
        const int code;

    public:
        RVCodeGenConstraint(int code);
        RVCodeGenConstraint(const RVCodeGenConstraint& obj);
        ~RVCodeGenConstraint();

        int     GetCode() const;
        int     GetTypeCode() const;
        int     GetOrdinal() const;
    };

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

        void        operator=(const RVCodeGenConstraintRangeI& obj);
    };


    // RISC-V Code Generator constraint collection
    class RVCodeGenConstraints {
    private:
        RVCodeGenConstraint*** constraints;

    public:
        RVCodeGenConstraints();
        RVCodeGenConstraints(const RVCodeGenConstraints& obj);
        ~RVCodeGenConstraints();

       
        template<class TConstraint>
        void            Set(
            const RVCodeGenConstraintTrait<TConstraint>&    trait,
            const TConstraint&                              constraint);


        void            SetIntegerRange(
            const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>&  trait,
            const RVCodeGenConstraintRangeI&                            constraint);

        void            SetIntegerRange(
            const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>&  trait,
            uint64_t                                                    lower_range,
            uint64_t                                                    upper_range);


        template<class TConstraint>
        TConstraint*    Get(const RVCodeGenConstraintTrait<TConstraint>& trait) const;


        template<class TConstraint>
        bool            Remove(const RVCodeGenConstraintTrait<TConstraint>& trait);

        void            Clear();
    };
}

// RISC-V Code Generator constraint traits
namespace Jasse {
    
    static constexpr RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>    RV_CODEGEN_PC_RANGE 
        { RV_CODEGEN_RANGE_I_PC };

    static constexpr RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>    RV_CODEGEN_GRx_RANGE
        { RV_CODEGEN_RANGE_I_GRx };

    static constexpr RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>    RV_CODEGEN_MEM_RANGE
        { RV_CODEGEN_RANGE_I_MEM };
}



// Implementation of: class RVCodeGenConstraintTrait<TConstraint>
namespace Jasse {
    /*
    const int code;
    */

    template<class TConstraint>
    constexpr RVCodeGenConstraintTrait<TConstraint>::RVCodeGenConstraintTrait(int code)
        : code  (code)
    { }

    template<class TConstraint>
    constexpr int RVCodeGenConstraintTrait<TConstraint>::GetCode() const
    {
        return code;
    }
}


// Implementation of: class RVCodeGenConstraint
namespace Jasse {
    /*
    const int code;
    */

    RVCodeGenConstraint::RVCodeGenConstraint(int code)
        : code  (code)
    { }

    RVCodeGenConstraint::RVCodeGenConstraint(const RVCodeGenConstraint& obj)
        : code  (obj.code)
    { }

    RVCodeGenConstraint::~RVCodeGenConstraint()
    { }

    inline int RVCodeGenConstraint::GetCode() const
    {
        return code;
    }

    inline int RVCodeGenConstraint::GetTypeCode() const
    {
        return (code & RV_CODEGEN_CONSTRAINT_MASK);
    }

    inline int RVCodeGenConstraint::GetOrdinal() const
    {
        return (code >> 4);
    }
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


// Implementation of: class RVCodeGenConstraints
namespace Jasse {
    /*
    RVCodeGenConstraint*** constraints;
    */
    
    
}
