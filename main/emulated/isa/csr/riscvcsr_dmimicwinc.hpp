#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
// **Jasse CUSTOM**
//
// MIMIC CSR increase-on-write counter register for debug
// (Self-increase on write, optional clear on write zero)
//

#include "base/riscvcsr.hpp"

#define CSR_dmimicwinc_DEFAULT_AT_COMPILE               0
#define CSR_dmimicwinc_CLEAR_ON_WRITE_ZERO              true


namespace Jasse::CSR {

    class RVCSR_dmimicwinc : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_dmimicwinc(const RVCSRDefinition& def) noexcept;
        RVCSR_dmimicwinc(const RVCSR_dmimicwinc& obj) noexcept;
        ~RVCSR_dmimicwinc();

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'dmimicwinc' instance allocator
    template<const RVCSRDefinition* def>
    RVCSR* __allocator_RVCSR_dmimicwinc() noexcept
    {
        return new RVCSR_dmimicwinc(*def);
    }

    // CSR 'dmimicwinc' registers definition
    csrdef dmimicwinc0  { CSR_dmimicwinc0, "dmimicwinc0", &__allocator_RVCSR_dmimicwinc<&dmimicwinc0> };
    csrdef dmimicwinc1  { CSR_dmimicwinc1, "dmimicwinc1", &__allocator_RVCSR_dmimicwinc<&dmimicwinc1> };
    csrdef dmimicwinc2  { CSR_dmimicwinc2, "dmimicwinc2", &__allocator_RVCSR_dmimicwinc<&dmimicwinc2> };
    csrdef dmimicwinc3  { CSR_dmimicwinc3, "dmimicwinc3", &__allocator_RVCSR_dmimicwinc<&dmimicwinc3> };
    csrdef dmimicwinc4  { CSR_dmimicwinc4, "dmimicwinc4", &__allocator_RVCSR_dmimicwinc<&dmimicwinc4> };
    csrdef dmimicwinc5  { CSR_dmimicwinc5, "dmimicwinc5", &__allocator_RVCSR_dmimicwinc<&dmimicwinc5> };
    csrdef dmimicwinc6  { CSR_dmimicwinc6, "dmimicwinc6", &__allocator_RVCSR_dmimicwinc<&dmimicwinc6> };
    csrdef dmimicwinc7  { CSR_dmimicwinc7, "dmimicwinc7", &__allocator_RVCSR_dmimicwinc<&dmimicwinc7> };
}


// Implementation of: class RVCSR_dmimicwinc
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_dmimicwinc::RVCSR_dmimicwinc(const RVCSRDefinition& def) noexcept
        : RVCSR (def)
        , value (CSR_dmimicwinc_DEFAULT_AT_COMPILE)
    { }

    RVCSR_dmimicwinc::RVCSR_dmimicwinc(const RVCSR_dmimicwinc& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_dmimicwinc::~RVCSR_dmimicwinc()
    { }

    bool RVCSR_dmimicwinc::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_dmimicwinc::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_dmimicwinc::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_dmimicwinc::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
#if CSR_dmimicwinc_CLEAR_ON_WRITE_ZERO
        if (!value)
            this->value = CSR_dmimicwinc_DEFAULT_AT_COMPILE;
#endif

        value++;
    }
}
