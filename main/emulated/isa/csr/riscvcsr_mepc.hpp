#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Exception Program Counter 'mepc'
//

#include "riscvcsr.hpp"


//
#define CSR_mepc_DEFAULT_AT_COMPILE                 0


namespace Jasse::CSR {

    class RVCSR_mepc : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mepc();
        RVCSR_mepc(const RVCSR_mepc& obj);
        ~RVCSR_mepc();

        virtual RVCSR*          Clone();

        virtual csr_t           GetValue() noexcept;
        virtual void            SetValue(csr_t value) noexcept;
    };

    // CSR 'mepc' instance allocator
    RVCSR* __allocator_RVCSR_mepc()
    {
        return new RVCSR_mepc();
    }

    // CSR 'mepc' definition
    static const RVCSRDefinition mepc = { CSR_mepc, &__allocator_RVCSR_mepc };
}


// Implementation of: class RVCSR_mepc
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mepc::RVCSR_mepc()
        : RVCSR (CSR_mepc, "mepc")
        , value (CSR_mepc_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mepc::RVCSR_mepc(const RVCSR_mepc& obj)
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mepc::~RVCSR_mepc()
    { }

    RVCSR* RVCSR_mepc::Clone()
    {
        return new RVCSR_mepc(*this);
    }

    csr_t RVCSR_mepc::GetValue() noexcept
    {
        return value;
    }

    void RVCSR_mepc::SetValue(csr_t value) noexcept
    {
        this->value = value;
    }
}
