#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Cause Register 'mcause'
//

#include "base/riscvcsr.hpp"

// Interrupt field      - [63]  (MXLEN=64 only)
#define CSR_mcause_FIELD_INTERRUPT_MX64_MASK            0x80000000'00000000UL
#define CSR_mcause_FIELD_INTERRUPT_MX64_OFFSET          63

// Interrupt field      - [31]  (MXLEN=32 only)
#define CSR_mcause_FIELD_INTERRUPT_MX32_MASK            0x80000000U
#define CSR_mcause_FIELD_INTERRUPT_MX32_OFFSET          31

// Exception code field - [0]   (MXLEN=64 only)
#define CSR_mcause_FIELD_EXCEPTION_CODE_MX64_MASK       0x7FFFFFFF'FFFFFFFFUL
#define CSR_mcause_FIELD_EXCEPTION_CODE_MX64_OFFSET     0

// Exception code field - [0]   (MXLEN=32 only)
#define CSR_mcause_FIELD_EXCEPTION_CODE_MX32_MASK       0x7FFFFFFFU
#define CSR_mcause_FIELD_EXCEPTION_CODE_MX32_OFFSET     0


//
#define CSR_mcause_DEFAULT_AT_COMPILE                   0


namespace Jasse::CSR {

    class RVCSR_mcause : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mcause();
        RVCSR_mcause(const RVCSR_mcause& obj);
        ~RVCSR_mcause();

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read() noexcept override;
        virtual void        Write(csr_t value) noexcept override;
    };

    // CSR 'mcause' instance allocator
    RVCSR* __allocator_RVCSR_mcause()
    {
        return new RVCSR_mcause();
    }

    // CSR 'mcause' definition
    static const RVCSRDefinition mcause = { CSR_mcause, &__allocator_RVCSR_mcause };
}


// Implementation of: class RVCSR_mcause
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mcause::RVCSR_mcause()
        : RVCSR (mcause, "mcause")
        , value (CSR_mcause_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mcause::RVCSR_mcause(const RVCSR_mcause& obj)
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mcause::~RVCSR_mcause()
    { }

    bool RVCSR_mcause::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_mcause::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_mcause::Read() noexcept
    {
        return value;
    }

    void RVCSR_mcause::Write(csr_t value) noexcept
    {
        this->value = value;
    }
}
