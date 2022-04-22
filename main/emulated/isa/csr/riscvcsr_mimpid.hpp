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

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read() noexcept override;
        virtual void        Write(csr_t value) noexcept override;
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

    csr_t RVCSR_mimpid::Read() noexcept
    {
        return value;
    }

    void RVCSR_mimpid::Write(csr_t value) noexcept
    {
        this->value = value;
    }
}