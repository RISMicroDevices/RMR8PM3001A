#pragma once
//
// RISC-V Instruction Set Architecture CSR (Control and Status Register)
//
// Machine Status Register 'mstatus'
//

#include "base/riscvcsr.hpp"

// SIE field    - [1]
#define CSR_mstatus_FIELD_SIE_MASK              0x00000002U
#define CSR_mstatus_FIELD_SIE_OFFSET            1

// MIE field    - [3]
#define CSR_mstatus_FIELD_MIE_MASK              0x00000008U
#define CSR_mstatus_FIELD_MIE_OFFSET            3

// SPIE field   - [5]
#define CSR_mstatus_FIELD_SPIE_MASK             0x00000020U
#define CSR_mstatus_FIELD_SPIE_OFFSET           5

// UBE field    - [6]
#define CSR_mstatus_FIELD_UBE_MASK              0x00000040U
#define CSR_mstatus_FIELD_UBE_OFFSET            6

// MPIE field   - [7]
#define CSR_mstatus_FIELD_MPIE_MASK             0x00000080U
#define CSR_mstatus_FIELD_MPIE_OFFSET           7

// SPP field    - [8]
#define CSR_mstatus_FIELD_SPP_MASK              0x00000100U
#define CSR_mstatus_FIELD_SPP_OFFSET            8

// VS field     - [10:9]
#define CSR_mstatus_FIELD_VS_MASK               0x00000600U
#define CSR_mstatus_FIELD_VS_OFFSET             9

// MPP field    - [12:11]
#define CSR_mstatus_FIELD_MPP_MASK              0x00001800U
#define CSR_mstatus_FIELD_MPP_OFFSET            11

// FS field     - [14:13]
#define CSR_mstatus_FIELD_FS_MASK               0x00006000U
#define CSR_mstatus_FIELD_FS_OFFSET             13

// XS field     - [16:15]
#define CSR_mstatus_FIELD_XS_MASK               0x00018000U
#define CSR_mstatus_FIELD_XS_OFFSET             15

// MPRV field   - [17]
#define CSR_mstatus_FIELD_MPRV_MASK             0x00020000U
#define CSR_mstatus_FIELD_MPRV_OFFSET           17

// SUM field    - [18]
#define CSR_mstatus_FIELD_SUM_MASK              0x00040000U
#define CSR_mstatus_FIELD_SUM_OFFSET            18

// MXR field    - [19]
#define CSR_mstatus_FIELD_MXR_MASK              0x00080000U
#define CSR_mstatus_FIELD_MXR_OFFSET            19

// TVM field    - [20]
#define CSR_mstatus_FIELD_TVM_MASK              0x00100000U
#define CSR_mstatus_FIELD_TVM_OFFSET            20

// TW field     - [21]
#define CSR_mstatus_FIELD_TW_MASK               0x00200000U
#define CSR_mstatus_FIELD_TW_OFFSET             21

// TSR field    - [22]
#define CSR_mstatus_FIELD_TSR_MASK              0x00400000U
#define CSR_mstatus_FIELD_TSR_OFFSET            22

// UXL field    - [33:32]
#define CSR_mstatus_FIELD_UXL_MASK              0x00000003'00000000UL
#define CSR_mstatus_FIELD_UXL_OFFSET            32

// SXL field    - [35:34]
#define CSR_mstatus_FIELD_SXL_MASK              0x0000000C'00000000UL
#define CSR_mstatus_FIELD_SXL_OFFSET            34

// SBE field    - [36]
#define CSR_mstatus_FIELD_SBE_MASK              0x00000010'00000000UL
#define CSR_mstatus_FIELD_SBE_OFFSET            36

// MBE field    - [37]
#define CSR_mstatus_FIELD_MBE_MASK              0x00000020'00000000UL
#define CSR_mstatus_FIELD_MBE_OFFSET            37

// SD field     - [63]
#define CSR_mstatus_FIELD_SD_MX64_MASK          0x80000000'00000000UL
#define CSR_mstatus_FIELD_SD_MX64_OFFSET        63

// SD field     - [31] (RV32 only)
#define CSR_mstatus_FIELD_SD_MX32_MASK          0x80000000U
#define CSR_mstatus_FIELD_SD_MX32_OFFSET        31


//
#define CSR_mstatus_DEFAULT_AT_COMPILE          0


namespace Jasse::CSR {

    class RVCSR_mstatus : public RVCSR {
    private:
        csr_t   value;

    public:
        RVCSR_mstatus() noexcept;
        RVCSR_mstatus(const RVCSR_mstatus& obj) noexcept;
        ~RVCSR_mstatus() noexcept;

        virtual bool        GetValue(csr_t* dst) const noexcept override;
        virtual bool        SetValue(csr_t value) noexcept override;

        virtual csr_t       Read(RVCSRSpace* CSRs) noexcept override;
        virtual void        Write(RVCSRSpace* CSRs, csr_t value) noexcept override;
    };

    // CSR 'mstatus' instance allocator
    RVCSR* __allocator_RVCSR_mstatus() noexcept
    {
        return new RVCSR_mstatus();
    }

    // CSR 'mstatus' definition
    csrdef mstatus = { CSR_mstatus, "mstatus", &__allocator_RVCSR_mstatus };
}


// Implementation of: class RVCSR_mstatus
namespace Jasse::CSR {
    /*
    csr_t   value;
    */

    RVCSR_mstatus::RVCSR_mstatus() noexcept
        : RVCSR (mstatus)
        , value (CSR_mstatus_DEFAULT_AT_COMPILE)
    { }

    RVCSR_mstatus::RVCSR_mstatus(const RVCSR_mstatus& obj) noexcept
        : RVCSR (obj)
        , value (obj.value)
    { }

    RVCSR_mstatus::~RVCSR_mstatus() noexcept
    { }

    bool RVCSR_mstatus::GetValue(csr_t* dst) const noexcept
    {
        *dst = value;
        return true;
    }

    bool RVCSR_mstatus::SetValue(csr_t value) noexcept
    {
        this->value = value;
        return true;
    }

    csr_t RVCSR_mstatus::Read(RVCSRSpace* CSRs) noexcept
    {
        return value;
    }

    void RVCSR_mstatus::Write(RVCSRSpace* CSRs,csr_t value) noexcept
    {
        this->value = value;
    }
}
