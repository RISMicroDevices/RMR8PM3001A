#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Trap-Vector Base-Address Register 'mtvec'
//

#include "base/riscvcsr.hpp"

// MODE field   - [1:0]
#define CSR_mtvec_FIELD_MODE_MASK                   0x00000003U
#define CSR_mtvec_FIELD_MODE_OFFSET                 0

// BASE field   - [63:2]    (MXLEN=64 only)
#define CSR_mtvec_FIELD_BASE_MX64_MASK              0xFFFFFFFF'FFFFFFFCUL
#define CSR_mtvec_FIELD_BASE_MX64_OFFSET            0

// BASE field   - [31:2]    (MXLEN=32 only)
#define CSR_mtvec_FIELD_BASE_MX32_MASK              0xFFFFFFFCU
#define CSR_mtvec_FIELD_BASE_MX32_OFFSET            0


//
#define CSR_mtvec_FIELD_MODE_DEF_DIRECT             0
#define CSR_mtvec_FIELD_MODE_DEF_VECTORED           1


//
#define CSR_mtvec_DEFAULT_AT_COMPILE                0


namespace Jasse::CSR {

    class RVCSR_mtvec : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mtvec() noexcept;
        RVCSR_mtvec(const RVCSR_mtvec& obj) noexcept;
        ~RVCSR_mtvec() noexcept;

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'mtvec' instance allocator
    RVCSR* __allocator_RVCSR_mtvec() noexcept
    {
        return new RVCSR_mtvec();
    }

    // CSR 'mtvec' definition
    csrdef mtvec = { CSR_mtvec, "mtvec", &__allocator_RVCSR_mtvec };
}


// Implementation of: class RVCSR_mtvec
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mtvec::RVCSR_mtvec() noexcept
        : RVCSR (mtvec)
        , value (CSR_mtvec_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mtvec::RVCSR_mtvec(const RVCSR_mtvec& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mtvec::~RVCSR_mtvec() noexcept
    { }

    bool RVCSR_mtvec::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_mtvec::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_mtvec::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_mtvec::Write(RVCSRSpace* CSRs, csr_t value) noexcept
    {
        this->value = value;
    }
}
