#pragma once
//
// RISC-V Instruction Set Architecture
//
// Memory interface basis (EEI defined)
//

#include <cstdint>


namespace Jasse {

    // Type definition of Memory Address of XLEN=32/64 (64 bits unified in emulation, zero extension)
    typedef     uint32_t        addr32_t;
    typedef     uint64_t        addr64_t;

    typedef     addr64_t        addr_t;

    // RISC-V Memory Operation Status
    typedef enum {
        MOP_SUCCESS = 0,
        MOP_ADDRESS_MISALIGNED,
        MOP_ACCESS_FAULT
    } RVMOPStatus;

    // RISC-V Memory Opeation Width
    typedef     uint64_t        RVMOPWidth;

    static constexpr RVMOPWidth MOPW_BYTE           = 0x00000000000000FFUL;
    static constexpr RVMOPWidth MOPW_HALF_WORD      = 0x000000000000FFFFUL;
    static constexpr RVMOPWidth MOPW_WORD           = 0x00000000FFFFFFFFUL;
    static constexpr RVMOPWidth MOPW_DOUBLE_WORD    = 0xFFFFFFFFFFFFFFFFUL;

    // Type definition of RISC-V Memory Data
    typedef union {
        uint8_t     data8;      // byte
        uint16_t    data16;     // half-word
        uint32_t    data32;     // word
        uint64_t    data64;     // double-word  
    } data_t;

    // RISC-V Memory Interface (EEI defined, Proxy in case of RVWMO) 
    class RVMemoryInterface { // *pure virtual*
    public:
        virtual RVMOPStatus     Read (addr_t address, RVMOPWidth width, data_t* dst) = 0;
        virtual RVMOPStatus     Write(addr_t address, RVMOPWidth width, data_t  src) = 0;
    };
}
