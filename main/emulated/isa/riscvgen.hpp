#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Instruction Generator (CodeGen) components
//

#include <set>

#include "riscvdef.hpp"
#include "riscvmisc.hpp"


#define RV_CODEGEN_CONSTRAINT_MASK              0x0F

#define RV_CODEGEN_TYPECODE(code)               (code & RV_CODEGEN_CONSTRAINT_MASK)
#define RV_CODEGEN_ORDINAL(code)                (code >> 4)


#define RV_CODEGEN_RANGE_I                      0x01

#define RV_CODEGEN_RANGE_I_PC                   0x01
#define RV_CODEGEN_RANGE_I_GRx                  0x11
#define RV_CODEGEN_RANGE_I_MEM                  0x21


namespace Jasse {

    // RISC-V Code Generator execluded codepoint collection iterator
    using RVCodeGenExclusionIterator      = std::set<const RVCodepoint*>::iterator;
    using RVCodeGenExclusionConstIterator = std::set<const RVCodepoint*>::const_iterator;

    // RISC-V Code Generator execluded codepoint collection
    class RVCodeGenExclusion {
    private:
        std::set<const RVCodepoint*>    exclusion;

    public:
        RVCodeGenExclusion();
        RVCodeGenExclusion(const RVCodeGenExclusion& obj);
        ~RVCodeGenExclusion();

        void    Add(const RVCodepoint* codepoint);
        void    Remove(const RVCodepoint* codepoint);

        bool    IsExcluded(const RVCodepoint* codepoint) const;

        RVCodeGenExclusionIterator      Begin();
        RVCodeGenExclusionConstIterator Begin() const;
        RVCodeGenExclusionIterator      End();
        RVCodeGenExclusionConstIterator End() const;
    };


    // RISC-V Code Generator constraint trait
    template<class TConstraint>
    class RVCodeGenConstraintTrait {
    private:
        const int code;

    public:
        constexpr RVCodeGenConstraintTrait(int code);

        constexpr int       GetCode() const;
        constexpr int       GetTypeCode() const;
        constexpr int       GetOrdinal() const;
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
        void                Set(
            const RVCodeGenConstraintTrait<TConstraint>&    trait,
            const TConstraint&                              constraint);


        void                SetIntegerRange(
            const RVCodeGenConstraintRangeI&    constraint);

        void                SetIntegerRange(
            const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>&  trait,
            uint64_t                                                    lower_range,
            uint64_t                                                    upper_range);


        template<class TConstraint>
        TConstraint*        Get(const RVCodeGenConstraintTrait<TConstraint>& trait);

        template<class TConstraint>
        const TConstraint*  Get(const RVCodeGenConstraintTrait<TConstraint>& trait) const;


        template<class TConstraint>
        bool                Remove(const RVCodeGenConstraintTrait<TConstraint>& trait);

        void                Clear();
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


// RISC-V Code Generator utilities
namespace Jasse {
    
}



// Implementation of: class RVCodeGenExclusion
namespace Jasse {
    /*
    std::set<const RVCodepoint*>    exclusion
    */

    RVCodeGenExclusion::RVCodeGenExclusion()
        : exclusion ()
    { }

    RVCodeGenExclusion::RVCodeGenExclusion(const RVCodeGenExclusion& obj)
        : exclusion (obj.exclusion)
    { }

    RVCodeGenExclusion::~RVCodeGenExclusion()
    { }

    inline void RVCodeGenExclusion::Add(const RVCodepoint* codepoint)
    {
        exclusion.insert(codepoint);
    }

    inline void RVCodeGenExclusion::Remove(const RVCodepoint* codepoint)
    {
        exclusion.erase(codepoint);
    }

    inline bool RVCodeGenExclusion::IsExcluded(const RVCodepoint* codepoint) const
    {
        return (exclusion.find(codepoint) != exclusion.end());
    }

    inline RVCodeGenExclusionIterator RVCodeGenExclusion::Begin()
    {
        return exclusion.begin();
    }

    inline RVCodeGenExclusionConstIterator RVCodeGenExclusion::Begin() const
    {
        return exclusion.begin();
    }

    inline RVCodeGenExclusionIterator RVCodeGenExclusion::End()
    {
        return exclusion.end();
    }

    inline RVCodeGenExclusionConstIterator RVCodeGenExclusion::End() const
    {
        return exclusion.end();
    }
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

    template<class TConstraint>
    constexpr int RVCodeGenConstraintTrait<TConstraint>::GetTypeCode() const
    {
        return RV_CODEGEN_TYPECODE(code);
    }

    template<class TConstraint>
    constexpr int RVCodeGenConstraintTrait<TConstraint>::GetOrdinal() const
    {
        return RV_CODEGEN_ORDINAL(code);
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
        return RV_CODEGEN_TYPECODE(code);
    }

    inline int RVCodeGenConstraint::GetOrdinal() const
    {
        return RV_CODEGEN_ORDINAL(code);
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
    
    RVCodeGenConstraints::RVCodeGenConstraints()
        : constraints   (new RVCodeGenConstraint**[1 << 4]())
    { }

    RVCodeGenConstraints::RVCodeGenConstraints(const RVCodeGenConstraints& obj)
        : constraints   (new RVCodeGenConstraint**[1 << 4])
    {
        for (int i = 0; i < (1 << 4); i++)
        {
            if (obj.constraints[i])
            {
                constraints[i] = new RVCodeGenConstraint*[1 << 4];

                for (int j = 0; j < (1 << 4); j++)
                {
                    if (obj.constraints[i][j])
                        constraints[i][j] = new RVCodeGenConstraint(*obj.constraints[i][j]);
                    else
                        constraints[i][j] = nullptr;
                }
            }
            else
                constraints[i] = nullptr;
        }
    }

    RVCodeGenConstraints::~RVCodeGenConstraints()
    {
        for (int i = 0; i < (1 << 4); i++)
        {
            if (constraints[i])
            {
                for (int j = 0; j < (1 << 4); j++)
                {
                    if (constraints[i][j])
                        delete constraints[i][j];
                }

                delete[] constraints[i];
            }
        }

        delete[] constraints;
    }

    template<class TConstraint>
    void RVCodeGenConstraints::Set(const RVCodeGenConstraintTrait<TConstraint>&    trait,
                                   const TConstraint&                              constraint)
    {
        if (!constraints[trait.GetCode()])
            constraints[trait.GetCode()] = new RVCodeGenConstraint*[1 << 4];

        constraints[trait.GetCode()][trait.GetOrdinal()] 
            = static_cast<RVCodeGenConstraint*>(new TConstraint(constraint));
    }

    void RVCodeGenConstraints::SetIntegerRange(const RVCodeGenConstraintRangeI& constraint)
    {
        if (!constraints[constraint.GetCode()])
            constraints[constraint.GetCode()] = new RVCodeGenConstraint*[1 << 4];

        constraints[constraint.GetCode()][constraint.GetOrdinal()] 
            = new RVCodeGenConstraintRangeI(constraint);
    }

    inline void RVCodeGenConstraints::SetIntegerRange(const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>&  trait,
                                                      uint64_t                                                    lower_range,
                                                      uint64_t                                                    upper_range)
    {
        SetIntegerRange(RVCodeGenConstraintRangeI(trait.GetCode(), lower_range, upper_range));
    }

    template<class TConstraint>
    TConstraint* RVCodeGenConstraints::Get(const RVCodeGenConstraintTrait<TConstraint>& trait)
    {
        if (!constraints[trait.GetCode()])
            return nullptr;

        return static_cast<TConstraint*>(constraints[trait.GetCode()][trait.GetOrdinal()]);
    }

    template<class TConstraint>
    const TConstraint* RVCodeGenConstraints::Get(const RVCodeGenConstraintTrait<TConstraint>& trait) const
    {
        if (!constraints[trait.GetCode()])
            return nullptr;

        return static_cast<const TConstraint*>(constraints[trait.GetCode()][trait.GetOrdinal()]);
    }

    template<class TConstraint>
    bool RVCodeGenConstraints::Remove(const RVCodeGenConstraintTrait<TConstraint>& trait)
    {
        if (!constraints[trait.GetCode()])
            return false;

        if (!constraints[trait.GetCode()][trait.GetOrdinal()])
            return false;

        delete constraints[trait.GetCode()][trait.GetOrdinal()];
        constraints[trait.GetCode()][trait.GetOrdinal()] = nullptr;

        return true;
    }
    
    void RVCodeGenConstraints::Clear()
    {
        for (int i = 0; i < (1 << 4); i++)
        {
            if (constraints[i])
            {
                for (int j = 0; j < (1 << 4); j++)
                {
                    if (constraints[i][j])
                        delete constraints[i][j];
                }

                delete[] constraints[i];
                constraints[i] = nullptr;
            }
        }
    }
}
