#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
// **Jasse CUSTOM**
//
// MIMIC CSR write operation total counter register for debug
// (Self-increase on write 'dmimic0' ... 'dmimic15')
//

#include "base/riscvcsr.hpp"

#define CSR_dmimicwrcnt_DEFAULT_AT_COMPILE              0


namespace Jasse::CSR {

    class RVCSR_dmimicwrcnt : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_dmimicwrcnt() noexcept;
        RVCSR_dmimicwrcnt(const RVCSR_dmimicwrcnt& obj) noexcept;
        ~RVCSR_dmimicwrcnt();

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'dmimicwrcnt' instance allocator
    RVCSR* __allocator_RVCSR_dmimicwrcnt() noexcept
    {
        return new RVCSR_dmimicwrcnt();
    }

    // CSR 'dmimicwrcnt' definition
    csrdef dmimicwrcnt  { CSR_dmimicwrcnt, "dmimicwrcnt", &__allocator_RVCSR_dmimicwrcnt };
}


// Implementation of: class RVCSR_dmimicwrcnt
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_dmimicwrcnt::RVCSR_dmimicwrcnt() noexcept
        : RVCSR (dmimicwrcnt)
        , value (CSR_dmimicwrcnt_DEFAULT_AT_COMPILE)
    { }

    RVCSR_dmimicwrcnt::RVCSR_dmimicwrcnt(const RVCSR_dmimicwrcnt& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_dmimicwrcnt::~RVCSR_dmimicwrcnt()
    { }

    bool RVCSR_dmimicwrcnt::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_dmimicwrcnt::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_dmimicwrcnt::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_dmimicwrcnt::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        this->value = value;
    }
}
