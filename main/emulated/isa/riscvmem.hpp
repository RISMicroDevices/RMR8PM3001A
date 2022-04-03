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
        virtual RVMOPStatus     Read (addr_t address, RVMOPWidth width, data_t* dst) = 0;
        virtual RVMOPStatus     Write(addr_t address, RVMOPWidth width, data_t  src) = 0;
    };



    // Simple Linear Memory Instance
    class SimpleLinearMemory : public RVMemoryInterface {
    private:
        const size_t    size;
        arch64_t*       heap;

    public:
        SimpleLinearMemory(size_t size);
        SimpleLinearMemory(const SimpleLinearMemory& obj);
        ~SimpleLinearMemory();

        arch64_t*           GetHeap();
        const arch64_t*     GetHeap() const;
        size_t              GetSize() const;
        size_t              GetCapacity() const;

        virtual RVMOPStatus Read(addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus Write(addr_t address, RVMOPWidth width, data_t src) override;
    };
}



// Implementation of: class SimpleLinearMemory
namespace Jasse {
    /*
    arch64_t*       heap;
    const size_t    size;
    */

    SimpleLinearMemory::SimpleLinearMemory(size_t size)
        : size  (size)
        , heap  (new arch64_t[size]())
    { }

    SimpleLinearMemory::SimpleLinearMemory(const SimpleLinearMemory& obj)
        : size  (obj.size)
        , heap  (new arch64_t[obj.size])
    { 
        std::copy(obj.heap, obj.heap + obj.size, heap);
    }

    SimpleLinearMemory::~SimpleLinearMemory()
    {
        delete[] heap;
    }

    inline arch64_t* SimpleLinearMemory::GetHeap()
    {
        return heap;
    }

    inline const arch64_t* SimpleLinearMemory::GetHeap() const
    {
        return heap;
    }

    inline size_t SimpleLinearMemory::GetSize() const
    {
        return size;
    }

    inline size_t SimpleLinearMemory::GetCapacity() const
    {
        return size << 3;
    }

    RVMOPStatus SimpleLinearMemory::Read(addr_t address, RVMOPWidth width, data_t* dst)
    {
        // !! little-endian system only !!

        if (address + width.length > GetCapacity()) // address out of range
            return MOP_ACCESS_FAULT;

        if (width.length > 8) // unsupported access length
            return MOP_ACCESS_FAULT;

        memcpy(dst, ((uint8_t*) heap) + address, width.length);

        return MOP_SUCCESS;
    }

    RVMOPStatus SimpleLinearMemory::Write(addr_t address, RVMOPWidth width, data_t src)
    {
        // !! little-endian system only !!

        if (address + width.length > GetCapacity()) // address out of range
            return MOP_ACCESS_FAULT;

        if (width.length > 8) // unsupported access length
            return MOP_ACCESS_FAULT;

        memcpy(((uint8_t*) heap) + address, &src, width.length);

        return MOP_SUCCESS;
    }
}
