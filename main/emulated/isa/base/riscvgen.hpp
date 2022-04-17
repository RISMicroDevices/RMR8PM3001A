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


#define RV_CODEGEN_RANGE_I                      0x01

#define RV_CODEGEN_RANGE_I_GRx                  0x01
#define RV_CODEGEN_RANGE_I_IMM                  0x11


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


    // RISC-V Code Generator function
    typedef insnraw_t   (*RVCodeGen)(const RVCodeGenConstraints* constraints);
}


// RISC-V Code Generator constraint traits
namespace Jasse {
    
    static constexpr RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>    RV_CODEGEN_GRx_RANGE
        { RV_CODEGEN_RANGE_I_GRx };

    static constexpr RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>    RV_CODEGEN_IMM_RANGE
        { RV_CODEGEN_RANGE_I_IMM };
}


// RISC-V Code Generator utilities
namespace Jasse {
    
    //
    void        Rand32Seed(uint32_t seed);
    uint32_t    Rand32(uint32_t lower_range, uint32_t upper_range);
    uint32_t    Rand32(const RVCodeGenConstraintRangeI* range = nullptr);
    uint32_t    Rand32(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>& trait);

    void        Rand64Seed(uint64_t seed);
    uint64_t    Rand64(uint64_t lower_range, uint64_t upper_range);
    uint64_t    Rand64(const RVCodeGenConstraintRangeI* range = nullptr);
    uint32_t    Rand64(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>& trait);

    //
    template<class TConstraint>
    TConstraint*        GetConstraint(RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept;

    template<class TConstraint>
    const TConstraint*  GetConstraint(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<TConstraint>& trait) noexcept;


    //
    const RVCodepoint*  Roll(const RVCodepointCollection& codeset);
}


// RISC-V General Code Generators
namespace Jasse {

    insnraw_t   GenRVCodeGeneral(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;

    insnraw_t   GenRVCodeTypeR(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeI(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeS(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeB(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeU(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;
    insnraw_t   GenRVCodeTypeJ(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;

    insnraw_t   GenRVCodeZeroOperand(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept;


    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVGeneral(const RVCodeGenConstraints* constraints) noexcept;

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeR(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeI(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeS(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeB(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeU(const RVCodeGenConstraints* constraints) noexcept;
    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeJ(const RVCodeGenConstraints* constraints) noexcept;

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVZeroOperand(const RVCodeGenConstraints* constraints) noexcept;
}

// RISC-V General Code Generator Extensions
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
        RVCodeGenConstraint**& l1 = constraints[trait.GetTypeCode()];

        if (!l1)
            l1 = new RVCodeGenConstraint*[1 << 4]();

        RVCodeGenConstraint*&  l2 = l1[trait.GetOrdinal()];

        if (l2)
            delete l2;

        l2 = static_cast<RVCodeGenConstraint*>(new TConstraint(constraint));
    }

    void RVCodeGenConstraints::SetIntegerRange(const RVCodeGenConstraintRangeI& constraint)
    {
        RVCodeGenConstraint**& l1 = constraints[constraint.GetTypeCode()];

        if (!l1)
            l1 = new RVCodeGenConstraint*[1 << 4]();

        RVCodeGenConstraint*&  l2 = l1[constraint.GetOrdinal()];

        if (l2)
            delete l2;

        l2 = new RVCodeGenConstraintRangeI(constraint);
    }

    inline void RVCodeGenConstraints::SetIntegerRange(const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>&  trait,
                                                      uint64_t                                                    lower_range,
                                                      uint64_t                                                    upper_range)
    {
        SetIntegerRange(RVCodeGenConstraintRangeI(trait.GetTypeCode(), lower_range, upper_range));
    }

    template<class TConstraint>
    TConstraint* RVCodeGenConstraints::Get(const RVCodeGenConstraintTrait<TConstraint>& trait)
    {
        RVCodeGenConstraint** l1;

        if (!(l1 = constraints[trait.GetTypeCode()]))
            return nullptr;

        return static_cast<TConstraint*>(l1[trait.GetOrdinal()]);
    }

    template<class TConstraint>
    const TConstraint* RVCodeGenConstraints::Get(const RVCodeGenConstraintTrait<TConstraint>& trait) const
    {
        RVCodeGenConstraint** l1;

        if (!(l1 = constraints[trait.GetTypeCode()]))
            return nullptr;

        return static_cast<const TConstraint*>(l1[trait.GetOrdinal()]);
    }

    template<class TConstraint>
    bool RVCodeGenConstraints::Remove(const RVCodeGenConstraintTrait<TConstraint>& trait)
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
    
    void RVCodeGenConstraints::Clear()
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
    static std::mt19937     rand32_engine = std::mt19937(std::random_device()());

    inline uint32_t Rand32(uint32_t lower_range, uint32_t upper_range)
    {
        return std::uniform_int_distribution<uint32_t>(lower_range, upper_range)(rand32_engine);
    }

    inline uint32_t Rand32(const RVCodeGenConstraintRangeI* range)
    {
        if (!range)
            return Rand32(0, UINT32_MAX);
        else
            return Rand32((uint32_t) range->GetLowerRange(), (uint32_t) range->GetUpperRange());
    }

    inline uint32_t Rand32(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>& trait)
    {
        return Rand32(GetConstraint(constraints, trait));
    }

    inline void Rand32Seed(uint32_t seed)
    {
        rand32_engine.seed(seed);
    }

    // 
    static std::mt19937_64  rand64_engine = std::mt19937_64(std::random_device()());

    inline uint64_t Rand64(uint64_t lower_range, uint64_t upper_range)
    {
        return std::uniform_int_distribution<uint64_t>(lower_range, upper_range)(rand64_engine);
    }

    inline uint64_t Rand64(const RVCodeGenConstraintRangeI* range)
    {
        if (!range)
            return Rand64(0, UINT64_MAX);
        else
            return Rand64(range->GetLowerRange(), range->GetUpperRange());
    }

    inline uint32_t Rand64(const RVCodeGenConstraints* constraints, const RVCodeGenConstraintTrait<RVCodeGenConstraintRangeI>& trait)
    {
        return Rand64(GetConstraint(constraints, trait));
    }

    inline void Rand64Seed(uint64_t seed)
    {
        rand64_engine.seed(seed);
    }

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

    //
    inline const RVCodepoint* Roll(const RVCodepointCollection& codeset)
    {
        if (!codeset.GetSize())
            return nullptr;

        return codeset.Get(Rand32(0, codeset.GetSize() - 1));
    }
}


// Implementation of General Code Generators
namespace Jasse {

    // *NOTICE: It's recommended to call GenRVCodeType(X)(...) when generating corresponding type of RISC-V 
    //          instruction code (e.g. Calling GenRVCodeTypeR when generating R-type instructions). 
    //          Though the performance gain of calling individual function is not significant at all, it's
    //          designed to make the code more readable, providing more information for code generation, and
    //          applying specialized constraints through calling different functions, regardless of it's duplicating.
    //          In design, generating any type of instruction codes can also be done by calling GenRVCodeGeneral,
    //          and wouldn't cause critical fault, but it's not always elegant depending on implementation.

    insnraw_t GenRVCodeGeneral(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS2Required())
            encoder->SetRS2(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVGeneral(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeGeneral(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeR(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS2Required())
            encoder->SetRS2(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeR(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeR(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeI(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeI(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeI(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeS(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS2Required())
            encoder->SetRS2(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeS(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeS(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeB(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRS1Required())
            encoder->SetRS1(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        if (encoder->IsRS2Required())
            encoder->SetRS2(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeB(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeB(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeU(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeU(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeU(alloc, constraints);
    }

    insnraw_t GenRVCodeTypeJ(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        if (encoder->IsImmediateRequired())
            encoder->SetImmediate(Rand32(constraints, RV_CODEGEN_IMM_RANGE));

        if (encoder->IsRDRequired())
            encoder->SetRD(Rand32(constraints, RV_CODEGEN_GRx_RANGE));

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVTypeJ(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeTypeJ(alloc, constraints);
    }

    insnraw_t GenRVCodeZeroOperand(RVEncoderAllocator encoderAlloc, const RVCodeGenConstraints* constraints) noexcept
    {
        RVEncoder* encoder = encoderAlloc();
        insnraw_t  insn;

        insn = encoder->Get();
        delete encoder;

        return insn;
    }

    template<RVEncoderAllocator alloc> insnraw_t CodeGenRVZeroOperand(const RVCodeGenConstraints* constraints) noexcept
    {
        return GenRVCodeZeroOperand(alloc, constraints);
    }
}
