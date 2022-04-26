#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Architecture ID Register 'marchid'
//

#include "base/riscvcsr.hpp"

#define CSR_marchid_DEFAULT_AT_COMPILE                0


namespace Jasse::CSR {

    class RVCSR_marchid : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_marchid() noexcept;
        RVCSR_marchid(const RVCSR_marchid& obj) noexcept;
        ~RVCSR_marchid() noexcept;

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'marchid' instance allocator
    RVCSR* __allocator_RVCSR_marchid() noexcept
    {
        return new RVCSR_marchid();
    }

    // CSR 'marchid' definition
    csrdef marchid = { CSR_marchid, "marchid", &__allocator_RVCSR_marchid };
}


// Implementation of: class RVCSR_marchid
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_marchid::RVCSR_marchid() noexcept
        : RVCSR (marchid)
        , value (CSR_marchid_DEFAULT_AT_COMPILE)
    { }

    RVCSR_marchid::RVCSR_marchid(const RVCSR_marchid& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_marchid::~RVCSR_marchid() noexcept
    { }

    bool RVCSR_marchid::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_marchid::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_marchid::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_marchid::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        this->value = value;
    }
}