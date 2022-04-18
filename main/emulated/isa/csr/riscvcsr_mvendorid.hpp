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
        RVCSR_mvendorid();
        RVCSR_mvendorid(const RVCSR_mvendorid& obj);
        ~RVCSR_mvendorid();

        virtual RVCSR*          Clone();

        virtual csr_t           GetValue() noexcept;
        virtual void            SetValue(csr_t value) noexcept;
    };

    // CSR 'mvendorid' instance allocator
    RVCSR* __allocator_RVCSR_mvendorid()
    {
        return new RVCSR_mvendorid();
    }

    // CSR 'mvendorid' definition
    static const RVCSRDefinition mvendorid = { CSR_mvendorid, &__allocator_RVCSR_mvendorid };
}


// Implementation of: class RVCSR_mvendorid
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mvendorid::RVCSR_mvendorid()
        : RVCSR (CSR_mvendorid, "mvendorid")
        , value (CSR_mvendorid_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mvendorid::RVCSR_mvendorid(const RVCSR_mvendorid& obj)
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mvendorid::~RVCSR_mvendorid()
    { }

    RVCSR* RVCSR_mvendorid::Clone()
    {
        return new RVCSR_mvendorid(*this);
    }

    csr_t RVCSR_mvendorid::GetValue() noexcept
    {
        return value;
    }

    void RVCSR_mvendorid::SetValue(csr_t value) noexcept
    {
        this->value = value;
    }
}
