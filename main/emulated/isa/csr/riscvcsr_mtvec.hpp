#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Trap-Vector Base-Address Register 'mtvec'
//

#include "riscvcsr.hpp"

// MODE field   - [1:0]
#define CSR_mtvec_FIELD_MODE_MASK                   0x00000003U
#define CSR_mtvec_FIELD_MODE_OFFSET                 0

// BASE field   - [63:2]    (MXLEN=64 only)
#define CSR_mtvec_FIELD_BASE64_MASK                 0xFFFFFFFF_FFFFFFFCUL
#define CSR_mtvec_FIELD_BASE64_OFFSET               0

// BASE field   - [31:2]    (MXLEN=32 only)
#define CSR_mtvec_FIELD_BASE32_MASK                 0xFFFFFFFCU
#define CSR_mtvec_FIELD_BASE32_OFFSET               0


//
#define CSR_mtvec_DEFAULT_AT_COMPILE                0


namespace Jasse::CSR {

    class RVCSR_mtvec : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mtvec();
        RVCSR_mtvec(const RVCSR_mtvec& obj);
        ~RVCSR_mtvec();

        virtual RVCSR*          Clone();

        virtual csr_t           GetValue() noexcept;
        virtual void            SetValue(csr_t value) noexcept;
    };

    // CSR 'mtvec' instance allocator
    RVCSR* __allocator_RVCSR_mtvec()
    {
        return new RVCSR_mtvec();
    }

    // CSR 'mtvec' definition
    static const RVCSRDefinition mtvec = { CSR_mtvec, &__allocator_RVCSR_mtvec };
}


// Implementation of: class RVCSR_mtvec
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mtvec::RVCSR_mtvec()
        : RVCSR (CSR_mtvec, "mtvec")
        , value (CSR_mtvec_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mtvec::RVCSR_mtvec(const RVCSR_mtvec& obj)
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mtvec::~RVCSR_mtvec()
    { }

    RVCSR* RVCSR_mtvec::Clone()
    {
        return new RVCSR_mtvec(*this);
    }

    csr_t RVCSR_mtvec::GetValue() noexcept
    {
        return value;
    }

    void RVCSR_mtvec::SetValue(csr_t value) noexcept
    {
        this->value = value;
    }
}