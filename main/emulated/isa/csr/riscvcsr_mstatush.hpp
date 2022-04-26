#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Status Register upper 32 bits 'mstatush', RV32 only
//

#include "base/riscvcsr.hpp"

// SBE field    - [4]
#define CSR_mstatush_FIELD_SBE_MASK             0x00000010U
#define CSR_mstatush_FIELD_SBE_OFFSET           4

// MBE field    - [5]
#define CSR_mstatush_FIELD_MBE_MASK             0x00000020U
#define CSR_mstatush_FIELD_MBE_OFFSET           5


//
#define CSR_mstatush_DEFAULT_AT_COMPILE          0


namespace Jasse::CSR {

    class RVCSR_mstatush : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mstatush() noexcept;
        RVCSR_mstatush(const RVCSR_mstatush& obj) noexcept;
        ~RVCSR_mstatush() noexcept;

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'mstatush' instance allocator
    RVCSR* __allocator_RVCSR_mstatush() noexcept
    {
        return new RVCSR_mstatush();
    }

    // CSR 'mstatush' definition
    csrdef mstatush = { CSR_mstatush, "mstatush", &__allocator_RVCSR_mstatush };
}


// Implementation of: class RVCSR_mstatush
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mstatush::RVCSR_mstatush() noexcept
        : RVCSR (mstatush)
        , value (CSR_mstatush_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mstatush::RVCSR_mstatush(const RVCSR_mstatush& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mstatush::~RVCSR_mstatush() noexcept
    { }

    bool RVCSR_mstatush::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_mstatush::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_mstatush::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_mstatush::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        this->value = value;
    }
}
