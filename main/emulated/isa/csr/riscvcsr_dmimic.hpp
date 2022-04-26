#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
// **Jasse CUSTOM**
//
// MIMIC CSR general register for debug
//

#include "base/riscvcsr.hpp"

#define CSR_dmimic_DEFAULT_AT_COMPILE               0


namespace Jasse::CSR {

    class RVCSR_dmimic : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_dmimic(const RVCSRDefinition& def) noexcept;
        RVCSR_dmimic(const RVCSR_dmimic& obj) noexcept;
        ~RVCSR_dmimic() noexcept;

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'dmimic' instance allocator
    template<const RVCSRDefinition* def>
    RVCSR* __allocator_RVCSR_dmimic() noexcept
    {
        return new RVCSR_dmimic(*def);
    }

    // CSR 'dmimic' registers definition
    csrdef dmimic0    { CSR_dmimic0 , "dmimic0" , &__allocator_RVCSR_dmimic<&dmimic0>  };
    csrdef dmimic1    { CSR_dmimic1 , "dmimic1" , &__allocator_RVCSR_dmimic<&dmimic1>  };
    csrdef dmimic2    { CSR_dmimic2 , "dmimic2" , &__allocator_RVCSR_dmimic<&dmimic2>  };
    csrdef dmimic3    { CSR_dmimic3 , "dmimic3" , &__allocator_RVCSR_dmimic<&dmimic3>  };
    csrdef dmimic4    { CSR_dmimic4 , "dmimic4" , &__allocator_RVCSR_dmimic<&dmimic4>  };
    csrdef dmimic5    { CSR_dmimic5 , "dmimic5" , &__allocator_RVCSR_dmimic<&dmimic5>  };
    csrdef dmimic6    { CSR_dmimic6 , "dmimic6" , &__allocator_RVCSR_dmimic<&dmimic6>  };
    csrdef dmimic7    { CSR_dmimic7 , "dmimic7" , &__allocator_RVCSR_dmimic<&dmimic7>  };
    csrdef dmimic8    { CSR_dmimic8 , "dmimic8" , &__allocator_RVCSR_dmimic<&dmimic8>  };
    csrdef dmimic9    { CSR_dmimic9 , "dmimic9" , &__allocator_RVCSR_dmimic<&dmimic9>  };
    csrdef dmimic10   { CSR_dmimic10, "dmimic10", &__allocator_RVCSR_dmimic<&dmimic10> };
    csrdef dmimic11   { CSR_dmimic11, "dmimic11", &__allocator_RVCSR_dmimic<&dmimic11> };
    csrdef dmimic12   { CSR_dmimic12, "dmimic12", &__allocator_RVCSR_dmimic<&dmimic12> };
    csrdef dmimic13   { CSR_dmimic13, "dmimic13", &__allocator_RVCSR_dmimic<&dmimic13> };
    csrdef dmimic14   { CSR_dmimic14, "dmimic14", &__allocator_RVCSR_dmimic<&dmimic14> };
    csrdef dmimic15   { CSR_dmimic15, "dmimic15", &__allocator_RVCSR_dmimic<&dmimic15> };
}


// Implementation of: class RVCSR_dmimic
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_dmimic::RVCSR_dmimic(const RVCSRDefinition& def) noexcept
        : RVCSR (def)
        , value (CSR_dmimic_DEFAULT_AT_COMPILE)
    { }

    RVCSR_dmimic::RVCSR_dmimic(const RVCSR_dmimic& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_dmimic::~RVCSR_dmimic() noexcept
    { }

    bool RVCSR_dmimic::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_dmimic::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_dmimic::Read(RVCSRSpace* CSRs) noexcept
    {
        RVCSR* dmimicrdcnt = CSRs->GetCSR(CSR_dmimicrdcnt);
        if (dmimicrdcnt)
            dmimicrdcnt->Write(CSRs, dmimicrdcnt->Read(CSRs) + 1);

        return value;
    }

    void RVCSR_dmimic::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        RVCSR* dmimicwrcnt = CSRs->GetCSR(CSR_dmimicwrcnt);
        if (dmimicwrcnt)
            dmimicwrcnt->Write(CSRs, dmimicwrcnt->Read(CSRs) + 1);

        this->value = value;
    }
}
