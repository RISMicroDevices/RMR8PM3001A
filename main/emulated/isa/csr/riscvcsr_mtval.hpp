#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Trap Value Register 'mtval'
//

#include "base/riscvcsr.hpp"


//
#define CSR_mtval_DEFAULT_AT_COMPILE                0


namespace Jasse::CSR {

    class RVCSR_mtval : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mtval() noexcept;
        RVCSR_mtval(const RVCSR_mtval& obj) noexcept;
        ~RVCSR_mtval() noexcept;

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'mtval' instance allocator
    RVCSR* __allocator_RVCSR_mtval() noexcept
    {
        return new RVCSR_mtval();
    }

    // CSR 'mtval' definition
    csrdef mtval = { CSR_mtval, "mtval", &__allocator_RVCSR_mtval };
}


// Implementation of: class RVCSR_mtval
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mtval::RVCSR_mtval() noexcept
        : RVCSR (mtval)
        , value (CSR_mtval_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mtval::RVCSR_mtval(const RVCSR_mtval& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mtval::~RVCSR_mtval() noexcept
    { }

    bool RVCSR_mtval::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_mtval::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_mtval::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_mtval::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        this->value = value;
    }
}
