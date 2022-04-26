#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
// **Jasse CUSTOM**
//
// MIMIC CSR read operation total counter register for debug
// (Self-increase on read 'dmimic0' ... 'dmimic15')
//

#include "base/riscvcsr.hpp"

#define CSR_dmimicrdcnt_DEFAULT_AT_COMPILE              0


namespace Jasse::CSR {

    class RVCSR_dmimicrdcnt : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_dmimicrdcnt() noexcept;
        RVCSR_dmimicrdcnt(const RVCSR_dmimicrdcnt& obj) noexcept;
        ~RVCSR_dmimicrdcnt();

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'dmimicrdcnt' instance allocator
    RVCSR* __allocator_RVCSR_dmimicrdcnt() noexcept
    {
        return new RVCSR_dmimicrdcnt();
    }

    // CSR 'dmimicrdcnt' definition
    csrdef dmimicrdcnt  { CSR_dmimicrdcnt, "dmimicrdcnt", &__allocator_RVCSR_dmimicrdcnt };
}


// Implementation of: class RVCSR_dmimicrdcnt
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_dmimicrdcnt::RVCSR_dmimicrdcnt() noexcept
        : RVCSR (dmimicrdcnt)
        , value (CSR_dmimicrdcnt_DEFAULT_AT_COMPILE)
    { }

    RVCSR_dmimicrdcnt::RVCSR_dmimicrdcnt(const RVCSR_dmimicrdcnt& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_dmimicrdcnt::~RVCSR_dmimicrdcnt()
    { }

    bool RVCSR_dmimicrdcnt::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_dmimicrdcnt::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_dmimicrdcnt::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_dmimicrdcnt::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        this->value = value;
    }
}
