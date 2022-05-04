#pragma once
//
// RISC-V Instruction Set Architecture
//
// Memory interface utilities
//

#include "base/riscvmem.hpp"


namespace Jasse {

    // Harvard Architecture Memory Interface
    class HarvardMemoryInterface : public RVMemoryInterface {
    private:
        RVMemoryInterface*  insnMemory;
        RVMemoryInterface*  dataMemory;

    public:
        HarvardMemoryInterface(RVMemoryInterface* insnMemory, RVMemoryInterface* dataMemory);
        HarvardMemoryInterface(const HarvardMemoryInterface& obj);
        ~HarvardMemoryInterface();

        RVMemoryInterface*  GetInsnMemory();
        RVMemoryInterface*  GetDataMemory();

        void                SetInsnMemory(RVMemoryInterface* insnMemory);
        void                SetDataMemory(RVMemoryInterface* dataMemory);

        virtual RVMOPStatus ReadInsn (addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus ReadData (addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus WriteInsn(addr_t address, RVMOPWidth width, data_t  src) override;
        virtual RVMOPStatus WriteData(addr_t address, RVMOPWidth width, data_t  src) override;
    };

    // Princeton Architecture Memory Interface
    class PrincetonMemoryInterface : public RVMemoryInterface {
    private:
        RVMemoryInterface*  memory;

    public:
        PrincetonMemoryInterface(RVMemoryInterface* memory);
        PrincetonMemoryInterface(const PrincetonMemoryInterface& obj);
        ~PrincetonMemoryInterface();

        RVMemoryInterface*  GetMemory();

        void                SetMemory(RVMemoryInterface* memory);

        virtual RVMOPStatus ReadInsn (addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus ReadData (addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus WriteInsn(addr_t address, RVMOPWidth width, data_t  src) override;
        virtual RVMOPStatus WriteData(addr_t address, RVMOPWidth width, data_t  src) override;
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

        virtual RVMOPStatus ReadInsn (addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus ReadData (addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus WriteInsn(addr_t address, RVMOPWidth width, data_t  src) override;
        virtual RVMOPStatus WriteData(addr_t address, RVMOPWidth width, data_t  src) override;
    };

    // Simple Circular Memory Interface
    class SimpleCircularMemoryInterface : public RVMemoryInterface {
    private:
        addr_t              cycle;
        RVMemoryInterface*  memory;

    public:
        SimpleCircularMemoryInterface(addr_t cycle, RVMemoryInterface* memory);
        SimpleCircularMemoryInterface(const SimpleCircularMemoryInterface& obj);
        ~SimpleCircularMemoryInterface();

        addr_t              GetCycle() const;
        RVMemoryInterface*  GetMemory();

        void                SetCycle(addr_t cycle);
        void                SetMemory(RVMemoryInterface* memory);

        virtual RVMOPStatus ReadInsn (addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus ReadData (addr_t address, RVMOPWidth width, data_t* dst) override;
        virtual RVMOPStatus WriteInsn(addr_t address, RVMOPWidth width, data_t  src) override;
        virtual RVMOPStatus WriteData(addr_t address, RVMOPWidth width, data_t  src) override;
    };
}



// Implementation of: class HarvardMemoryInterface
namespace Jasse {
    /*
    RVMemoryInterface*  insnMemory;
    RVMemoryInterface*  dataMemory;
    */

    HarvardMemoryInterface::HarvardMemoryInterface(RVMemoryInterface* insnMemory, RVMemoryInterface* dataMemory)
        : insnMemory(insnMemory)
        , dataMemory(dataMemory)
    { }

    HarvardMemoryInterface::HarvardMemoryInterface(const HarvardMemoryInterface& obj)
        : insnMemory(obj.insnMemory)
        , dataMemory(obj.dataMemory)
    { }

    HarvardMemoryInterface::~HarvardMemoryInterface() 
    { }

    inline RVMemoryInterface* HarvardMemoryInterface::GetInsnMemory()
    {
        return insnMemory;
    }

    inline RVMemoryInterface* HarvardMemoryInterface::GetDataMemory()
    {
        return dataMemory;
    }

    inline void HarvardMemoryInterface::SetInsnMemory(RVMemoryInterface* insnMemory)
    {
        this->insnMemory = insnMemory;
    }

    inline void HarvardMemoryInterface::SetDataMemory(RVMemoryInterface* dataMemory)
    {
        this->dataMemory = dataMemory;
    }

    RVMOPStatus HarvardMemoryInterface::ReadInsn(addr_t address, RVMOPWidth width, data_t* dst)
    {
        return insnMemory->ReadInsn(address, width, dst);
    }

    RVMOPStatus HarvardMemoryInterface::ReadData(addr_t address, RVMOPWidth width, data_t* dst)
    {
        return dataMemory->ReadData(address, width, dst);
    }

    RVMOPStatus HarvardMemoryInterface::WriteInsn(addr_t address, RVMOPWidth width, data_t  src)
    {
        return insnMemory->WriteInsn(address, width, src);
    }

    RVMOPStatus HarvardMemoryInterface::WriteData(addr_t address, RVMOPWidth width, data_t  src)
    {
        return dataMemory->WriteData(address, width, src);
    }
}


// Implementation of: class PrincetonMemoryInterface
namespace Jasse {
    /*
    RVMemoryInterface*  memory;
    */

    PrincetonMemoryInterface::PrincetonMemoryInterface(RVMemoryInterface* memory)
        : memory(memory)
    { }

    PrincetonMemoryInterface::PrincetonMemoryInterface(const PrincetonMemoryInterface& obj)
        : memory(obj.memory)
    { }

    PrincetonMemoryInterface::~PrincetonMemoryInterface() 
    { }

    inline RVMemoryInterface* PrincetonMemoryInterface::GetMemory()
    {
        return memory;
    }

    inline void PrincetonMemoryInterface::SetMemory(RVMemoryInterface* memory)
    {
        this->memory = memory;
    }

    RVMOPStatus PrincetonMemoryInterface::ReadInsn(addr_t address, RVMOPWidth width, data_t* dst)
    {
        return memory->ReadInsn(address, width, dst);
    }

    RVMOPStatus PrincetonMemoryInterface::ReadData(addr_t address, RVMOPWidth width, data_t* dst)
    {
        return memory->ReadData(address, width, dst);
    }

    RVMOPStatus PrincetonMemoryInterface::WriteInsn(addr_t address, RVMOPWidth width, data_t  src)
    {
        return memory->WriteInsn(address, width, src);
    }

    RVMOPStatus PrincetonMemoryInterface::WriteData(addr_t address, RVMOPWidth width, data_t  src)
    {
        return memory->WriteData(address, width, src);
    }
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

    inline RVMOPStatus SimpleLinearMemory::ReadInsn(addr_t address, RVMOPWidth width, data_t* dst)
    {
        return ReadData(address, width, dst);
    }

    RVMOPStatus SimpleLinearMemory::ReadData(addr_t address, RVMOPWidth width, data_t* dst)
    {
        // !! little-endian system only !!

        if (address + width.length > GetCapacity()) // address out of range
            return MOP_ACCESS_FAULT;

        if (width.length > 8) // unsupported access length
            return MOP_ACCESS_FAULT;

        memcpy(dst, ((uint8_t*) heap) + address, width.length);

        return MOP_SUCCESS;
    }

    inline RVMOPStatus SimpleLinearMemory::WriteInsn(addr_t address, RVMOPWidth width, data_t src)
    {
        return WriteData(address, width, src);
    }

    RVMOPStatus SimpleLinearMemory::WriteData(addr_t address, RVMOPWidth width, data_t src)
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


// Implementation of: class SimpleCircularMemory
namespace Jasse {
    /*
    addr_t              cycle;
    RVMemoryInterface*  memory;
    */

    SimpleCircularMemoryInterface::SimpleCircularMemoryInterface(addr_t cycle, RVMemoryInterface* memory)
        : cycle     (cycle)
        , memory    (memory)
    { }

    SimpleCircularMemoryInterface::SimpleCircularMemoryInterface(const SimpleCircularMemoryInterface& obj)
        : cycle     (obj.cycle)
        , memory    (obj.memory)
    { }

    SimpleCircularMemoryInterface::~SimpleCircularMemoryInterface() 
    { }

    inline addr_t SimpleCircularMemoryInterface::GetCycle() const
    {
        return cycle;
    }

    inline RVMemoryInterface* SimpleCircularMemoryInterface::GetMemory()
    {
        return memory;
    }

    inline void SimpleCircularMemoryInterface::SetCycle(addr_t cycle)
    {
        this->cycle = cycle;
    }

    inline void SimpleCircularMemoryInterface::SetMemory(RVMemoryInterface* memory)
    {
        this->memory = memory;
    }

    RVMOPStatus SimpleCircularMemoryInterface::ReadInsn(addr_t address, RVMOPWidth width, data_t* dst)
    {
        return memory->ReadInsn(address % cycle, width, dst);
    }

    RVMOPStatus SimpleCircularMemoryInterface::ReadData(addr_t address, RVMOPWidth width, data_t* dst)
    {
        return memory->ReadData(address % cycle, width, dst);
    }

    RVMOPStatus SimpleCircularMemoryInterface::WriteInsn(addr_t address, RVMOPWidth width, data_t  src)
    {
        return memory->WriteInsn(address % cycle, width, src);
    }

    RVMOPStatus SimpleCircularMemoryInterface::WriteData(addr_t address, RVMOPWidth width, data_t  src)
    {
        return memory->WriteData(address % cycle, width, src);
    }
}
