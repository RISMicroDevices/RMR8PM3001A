#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
// **Jasse CUSTOM**
//
// MIMIC CSR increase-on-read counter register for debug
// (Self-increase on read, optional clear on write zero)
//

#include "base/riscvcsr.hpp"

#define CSR_dmimicrinc_DEFAULT_AT_COMPILE               0
#define CSR_dmimicrinc_CLEAR_ON_WRITE_ZERO              true


namespace Jasse::CSR {

    class RVCSR_dmimicrinc : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_dmimicrinc(const RVCSRDefinition& def) noexcept;
        RVCSR_dmimicrinc(const RVCSR_dmimicrinc& obj) noexcept;
        ~RVCSR_dmimicrinc();

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'dmimicrinc' instance allocator
    template<const RVCSRDefinition* def>
    RVCSR* __allocator_RVCSR_dmimicrinc() noexcept
    {
        return new RVCSR_dmimicrinc(*def);
    }

    // CSR 'dmimicrinc' registers definition
    csrdef dmimicrinc0  { CSR_dmimicrinc0, "dmimicrinc0", &__allocator_RVCSR_dmimicrinc<&dmimicrinc0> };
    csrdef dmimicrinc1  { CSR_dmimicrinc1, "dmimicrinc1", &__allocator_RVCSR_dmimicrinc<&dmimicrinc1> };
    csrdef dmimicrinc2  { CSR_dmimicrinc2, "dmimicrinc2", &__allocator_RVCSR_dmimicrinc<&dmimicrinc2> };
    csrdef dmimicrinc3  { CSR_dmimicrinc3, "dmimicrinc3", &__allocator_RVCSR_dmimicrinc<&dmimicrinc3> };
    csrdef dmimicrinc4  { CSR_dmimicrinc4, "dmimicrinc4", &__allocator_RVCSR_dmimicrinc<&dmimicrinc4> };
    csrdef dmimicrinc5  { CSR_dmimicrinc5, "dmimicrinc5", &__allocator_RVCSR_dmimicrinc<&dmimicrinc5> };
    csrdef dmimicrinc6  { CSR_dmimicrinc6, "dmimicrinc6", &__allocator_RVCSR_dmimicrinc<&dmimicrinc6> };
    csrdef dmimicrinc7  { CSR_dmimicrinc7, "dmimicrinc7", &__allocator_RVCSR_dmimicrinc<&dmimicrinc7> };
}


// Implementation of: class RVCSR_dmimicrinc
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_dmimicrinc::RVCSR_dmimicrinc(const RVCSRDefinition& def) noexcept
        : RVCSR (def)
        , value (CSR_dmimicrinc_DEFAULT_AT_COMPILE)
    { }

    RVCSR_dmimicrinc::RVCSR_dmimicrinc(const RVCSR_dmimicrinc& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_dmimicrinc::~RVCSR_dmimicrinc()
    { }

    bool RVCSR_dmimicrinc::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_dmimicrinc::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_dmimicrinc::Read(RVCSRSpace* CSRs) noexcept
    {
        return value++;
    }

    void RVCSR_dmimicrinc::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
#if CSR_dmimicrinc_CLEAR_ON_WRITE_ZERO
        if (!value)
            this->value = CSR_dmimicrinc_DEFAULT_AT_COMPILE;
#endif
    }
}
