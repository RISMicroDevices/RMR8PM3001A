#pragma once
//
// RISC-V Instruction Set Architecture
//
// Memory interface basis (EEI defined)
//

#include <cstdint>
#include <cstring>
#include <algorithm>

#include "intmem.h"

#include "riscvdef.hpp"


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
    typedef struct {
        uint64_t    mask;
        uint32_t    alignment;
        uint32_t    length;
    } RVMOPWidth;

    static constexpr RVMOPWidth MOPW_BYTE           = { 0x00000000000000FFUL, 0x00U, 1 };
    static constexpr RVMOPWidth MOPW_HALF_WORD      = { 0x000000000000FFFFUL, 0x01U, 2 };
    static constexpr RVMOPWidth MOPW_WORD           = { 0x00000000FFFFFFFFUL, 0x03U, 4 };
    static constexpr RVMOPWidth MOPW_DOUBLE_WORD    = { 0xFFFFFFFFFFFFFFFFUL, 0x07U, 8 };

    // Type definition of RISC-V Memory Data
    typedef union {
        uint64_t    data64;     // double-word  
        uint32_t    data32;     // word
        uint16_t    data16;     // half-word
        uint8_t     data8;      // byte
    } data_t;

    // RISC-V Memory Interface (EEI defined, Proxy in case of RVWMO) 
    class RVMemoryInterface { // *pure virtual*
    public:
        virtual RVMOPStatus     ReadInsn (addr_t address, RVMOPWidth width, data_t* dst) = 0;
        virtual RVMOPStatus     ReadData (addr_t address, RVMOPWidth width, data_t* dst) = 0;
        virtual RVMOPStatus     WriteInsn(addr_t address, RVMOPWidth width, data_t  src) = 0;
        virtual RVMOPStatus     WriteData(addr_t address, RVMOPWidth width, data_t  src) = 0;
    };
}
