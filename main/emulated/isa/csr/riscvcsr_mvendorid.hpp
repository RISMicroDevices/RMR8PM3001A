#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Vendor ID Register 'mvendorid'
//

#include "base/riscvcsr.hpp"

#define CSR_mvendorid_DEFAULT_AT_COMPILE                0


namespace Jasse::CSR {

    class RVCSR_mvendorid : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mvendorid() noexcept;
        RVCSR_mvendorid(const RVCSR_mvendorid& obj) noexcept;
        ~RVCSR_mvendorid() noexcept;

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'mvendorid' instance allocator
    RVCSR* __allocator_RVCSR_mvendorid()
    {
        return new RVCSR_mvendorid();
    }

    // CSR 'mvendorid' definition
    csrdef mvendorid = { CSR_mvendorid, "mvendorid", &__allocator_RVCSR_mvendorid };
}


// Implementation of: class RVCSR_mvendorid
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mvendorid::RVCSR_mvendorid() noexcept
        : RVCSR (mvendorid)
        , value (CSR_mvendorid_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mvendorid::RVCSR_mvendorid(const RVCSR_mvendorid& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mvendorid::~RVCSR_mvendorid() noexcept
    { }

    bool RVCSR_mvendorid::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_mvendorid::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_mvendorid::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_mvendorid::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        this->value = value;
    }
}
