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
        RVCSR_mstatush();
        RVCSR_mstatush(const RVCSR_mstatush& obj);
        ~RVCSR_mstatush();

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read() noexcept override;
        virtual void        Write(csr_t value) noexcept override;
    };

    // CSR 'mstatush' instance allocator
    RVCSR* __allocator_RVCSR_mstatush()
    {
        return new RVCSR_mstatush();
    }

    // CSR 'mstatush' definition
    static const RVCSRDefinition mstatush = { CSR_mstatush, &__allocator_RVCSR_mstatush };
}


// Implementation of: class RVCSR_mstatush
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mstatush::RVCSR_mstatush()
        : RVCSR (mstatush, "mstatush")
        , value (CSR_mstatush_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mstatush::RVCSR_mstatush(const RVCSR_mstatush& obj)
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mstatush::~RVCSR_mstatush()
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

    csr_t RVCSR_mstatush::Read() noexcept
    {
        return value;
    }

    void RVCSR_mstatush::Write(csr_t value) noexcept
    {
        this->value = value;
    }
}
