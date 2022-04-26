#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Implementation ID Register 'mimpid'
//

#include "base/riscvcsr.hpp"

#define CSR_mimpid_DEFAULT_AT_COMPILE                   0


namespace Jasse::CSR {

    class RVCSR_mimpid : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mimpid() noexcept;
        RVCSR_mimpid(const RVCSR_mimpid& obj) noexcept;
        ~RVCSR_mimpid() noexcept;

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'mimpid' instance allocator
    RVCSR* __allocator_RVCSR_mimpid() noexcept
    {
        return new RVCSR_mimpid();
    }

    // CSR 'mimpid' definition
    csrdef mimpid = { CSR_mimpid, "mimpid", &__allocator_RVCSR_mimpid };
}


// Implementation of: class RVCSR_mimpid
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mimpid::RVCSR_mimpid() noexcept
        : RVCSR (mimpid)
        , value (CSR_mimpid_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mimpid::RVCSR_mimpid(const RVCSR_mimpid& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mimpid::~RVCSR_mimpid() noexcept
    { }

    bool RVCSR_mimpid::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_mimpid::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_mimpid::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_mimpid::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        this->value = value;
    }
}