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
        RVCSR_mimpid();
        RVCSR_mimpid(const RVCSR_mimpid& obj);
        ~RVCSR_mimpid();

        virtual RVCSR*          Clone();

        virtual csr_t           GetValue() noexcept;
        virtual void            SetValue(csr_t value) noexcept;
    };

    // CSR 'mimpid' instance allocator
    RVCSR* __allocator_RVCSR_mimpid()
    {
        return new RVCSR_mimpid();
    }

    // CSR 'mimpid' definition
    static const RVCSRDefinition mimpid = { CSR_mimpid, &__allocator_RVCSR_mimpid };
}


// Implementation of: class RVCSR_mimpid
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mimpid::RVCSR_mimpid()
        : RVCSR (mimpid, "mimpid")
        , value (CSR_mimpid_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mimpid::RVCSR_mimpid(const RVCSR_mimpid& obj)
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mimpid::~RVCSR_mimpid()
    { }

    RVCSR* RVCSR_mimpid::Clone()
    {
        return new RVCSR_mimpid(*this);
    }

    csr_t RVCSR_mimpid::GetValue() noexcept
    {
        return value;
    }

    void RVCSR_mimpid::SetValue(csr_t value) noexcept
    {
        this->value = value;
    }
}