#pragma once
//
// RISC-V Instruction Set Architecture Emulator (Jasse)
//
// Instruction Generator (CodeGen) components
//

#include <random>
#include <set>

#include "riscvdef.hpp"
#include "riscvcodeset.hpp"
#include "riscvencode.hpp"


#define RV_CODEGEN_CONSTRAINT_MASK              0x0F

#define RV_CODEGEN_TYPECODE(code)               (code & RV_CODEGEN_CONSTRAINT_MASK)
#define RV_CODEGEN_ORDINAL(code)                (code >> 4)


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
        constexpr RVCodeGenConstraintTrait(int code) : code(code) { };

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

    // RISC-V Code Generator constraint collection
    class RVCodeGenConstraints {
    private:
        RVCodeGenConstraint*** constraints;

        template<class TTrait>
        RVCodeGenConstraint*& __AddressAndEnsure(const TTrait& trait) noexcept;

        template<class TTrait>
        RVCodeGenConstraint*& __AddressAndReplace(const TTrait& trait) noexcept;

    public:
        RVCodeGenConstraints() noexcept;
        RVCodeGenConstraints(const RVCodeGenConstraints& obj) noexcept;
        ~RVCodeGenConstraints() noexcept;

       
        template<class TConstraint>
        void                Set(
            const RVCodeGenConstraintTrait<TConstraint>&    trait,
            const TConstraint&                              constraint) noexcept;

        template<class TConstraint, class... Args>
        TConstraint*        Emplace(
            const RVCodeGenConstraintTrait<TConstraint>&    trait,
            Args...                                         args) noexcept;

        template<class TConstraint, class... Args>
        TConstraint*        EmplaceIfAbsent(
            const RVCodeGenConstraintTrait<TConstraint>&    trait,
            Args...                                         args) noexcept;


        template<class TConstraint>
        TConstraint*        Get(const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept;

        template<class TConstraint>
        const TConstraint*  Get(const RVCodeGenConstraintTrait<TConstraint>& trait) const noexcept;


        template<class TConstraint>
        bool                Remove(const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept;

        void                Clear() noexcept;
    };


    // RISC-V Code Generator function
    typedef insnraw_t   (*RVCodeGen)(const RVCodeGenConstraints* constraints);
}


// RISC-V Code Generator utilities
namespace Jasse {
    //
    template<class TConstraint>
    TConstraint*        GetConstraint(RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept;

    template<class TConstraint>
    const TConstraint*  GetConstraint(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept;
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


// Implementation of: class RVCodeGenConstraints
namespace Jasse {
    /*
    RVCodeGenConstraint*** constraints;
    */
    
    RVCodeGenConstraints::RVCodeGenConstraints() noexcept
        : constraints   (new RVCodeGenConstraint**[1 << 4]())
    { }

    RVCodeGenConstraints::RVCodeGenConstraints(const RVCodeGenConstraints& obj) noexcept
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

    RVCodeGenConstraints::~RVCodeGenConstraints() noexcept
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

    template<class TTrait>
    inline RVCodeGenConstraint*& RVCodeGenConstraints::__AddressAndEnsure(const TTrait& trait) noexcept
    {
        RVCodeGenConstraint**& l1 = constraints[trait.GetTypeCode()];

        if (!l1)
            l1 = new RVCodeGenConstraint*[1 << 4]();

        return l1[trait.GetOrdinal()];
    }

    template<class TTrait>
    inline RVCodeGenConstraint*& RVCodeGenConstraints::__AddressAndReplace(const TTrait& trait) noexcept
    {
        RVCodeGenConstraint*& l2 = __AddressAndEnsure(trait);

        if (l2)
            delete l2;

        return l2;
    }

    template<class TConstraint>
    void RVCodeGenConstraints::Set(const RVCodeGenConstraintTrait<TConstraint>&    trait,
                                   const TConstraint&                              constraint) noexcept
    {
        __AddressAndReplace(trait) = static_cast<RVCodeGenConstraint*>(new TConstraint(constraint));
    }

    template<class TConstraint, class... Args>
    TConstraint* RVCodeGenConstraints::Emplace(const RVCodeGenConstraintTrait<TConstraint>&    trait,
                                               Args...                                         args) noexcept
    {
        return static_cast<TConstraint*>(
            __AddressAndReplace(trait) = static_cast<RVCodeGenConstraint*>(new TConstraint(trait.GetCode(), args...)));
    }

    template<class TConstraint, class... Args>
    TConstraint* RVCodeGenConstraints::EmplaceIfAbsent(const RVCodeGenConstraintTrait<TConstraint>& trait,
                                                       Args...                                      args) noexcept
    {
        return static_cast<TConstraint*>(
            __AddressAndEnsure(trait) = static_cast<RVCodeGenConstraint*>(new TConstraint(trait.GetCode(), args...)));
    }

    template<class TConstraint>
    TConstraint* RVCodeGenConstraints::Get(const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept
    {
        RVCodeGenConstraint** l1;

        if (!(l1 = constraints[trait.GetTypeCode()]))
            return nullptr;

        return static_cast<TConstraint*>(l1[trait.GetOrdinal()]);
    }

    template<class TConstraint>
    const TConstraint* RVCodeGenConstraints::Get(const RVCodeGenConstraintTrait<TConstraint>& trait) const noexcept
    {
        RVCodeGenConstraint** l1;

        if (!(l1 = constraints[trait.GetTypeCode()]))
            return nullptr;

        return static_cast<const TConstraint*>(l1[trait.GetOrdinal()]);
    }

    template<class TConstraint>
    bool RVCodeGenConstraints::Remove(const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept
    {
        RVCodeGenConstraint** l1 = constraints[trait.GetTypeCode()];

        if (!l1)
            return false;

        RVCodeGenConstraint*& l2 = l1[trait.GetOrdinal()];

        if (!l2)
            return false;

        delete l2;
        l2 = nullptr;

        return true;
    }
    
    void RVCodeGenConstraints::Clear() noexcept
    {
        for (int i = 0; i < (1 << 4); i++)
        {
            RVCodeGenConstraint**& l1 = constraints[i];

            if (l1)
            {
                for (int j = 0; j < (1 << 4); j++)
                {
                    if (l1[j])
                        delete l1[j];
                }

                delete[] l1;
                l1 = nullptr;
            }
        }
    }
}


// Implementation of utilities
namespace Jasse {
    //
    template<class TConstraint>
    TConstraint* GetConstraint(RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept
    {
        return constraints ? constraints->Get(trait) : nullptr;
    }

    template<class TConstraint>
    const TConstraint* GetConstraint(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept
    {
        return constraints ? constraints->Get(trait) : nullptr;
    }
}
