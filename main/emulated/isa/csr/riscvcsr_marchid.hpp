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
        RVCSR_marchid();
        RVCSR_marchid(const RVCSR_marchid& obj);
        ~RVCSR_marchid();

        virtual RVCSR*          Clone();

        virtual csr_t           GetValue() noexcept;
        virtual void            SetValue(csr_t value) noexcept;
    };

    // CSR 'marchid' instance allocator
    RVCSR* __allocator_RVCSR_marchid()
    {
        return new RVCSR_marchid();
    }

    // CSR 'marchid' definition
    static const RVCSRDefinition marchid = { CSR_marchid, &__allocator_RVCSR_marchid };
}


// Implementation of: class RVCSR_marchid
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_marchid::RVCSR_marchid()
        : RVCSR (CSR_marchid, "marchid")
        , value (CSR_marchid_DEFAULT_AT_COMPILE)
    { }

    RVCSR_marchid::RVCSR_marchid(const RVCSR_marchid& obj)
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_marchid::~RVCSR_marchid()
    { }

    RVCSR* RVCSR_marchid::Clone()
    {
        return new RVCSR_marchid(*this);
    }

    csr_t RVCSR_marchid::GetValue() noexcept
    {
        return value;
    }

    void RVCSR_marchid::SetValue(csr_t value) noexcept
    {
        this->value = value;
    }
}