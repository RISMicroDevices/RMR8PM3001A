#pragma once
//
// Mixed emulation for Issue Stage
//
//

#include "base.hpp"
#include "core_global.hpp"

using namespace std;


namespace MEMU::Core::Issue {

    class PhysicalRegisterFile final : public MEMU::Emulated
    {
    private:
        uint64_t    prfs[EMULATED_PRF_SIZE] = { 0 };
        int         writing_index;
        uint64_t    writing_value;

    public:
        PhysicalRegisterFile();
        PhysicalRegisterFile(const PhysicalRegisterFile& obj);
        ~PhysicalRegisterFile();

        constexpr int   GetCapacity() const;

        bool            CheckBound(int index) const;

        uint64_t        Get(int index) const;
        void            Set(int index, uint64_t value);

        virtual void    Eval() override;
    };

    class ReorderBuffer final : public MEMU::Emulated
    {
        // TODO
    };
}



// class MEMU::Core::Issue::PhysicalRegisterFile
namespace MEMU::Core::Issue {

    //
    PhysicalRegisterFile::PhysicalRegisterFile()
        : writing_index(-1)
    {  }

    PhysicalRegisterFile::PhysicalRegisterFile(const PhysicalRegisterFile& obj)
        : writing_index(-1)
    {
        memcpy(prfs, obj.prfs, sizeof(int64_t) * EMULATED_PRF_SIZE);
    }

    PhysicalRegisterFile::~PhysicalRegisterFile()
    {  }

    constexpr int PhysicalRegisterFile::GetCapacity() const
    {
        return EMULATED_PRF_SIZE;
    }

    inline bool PhysicalRegisterFile::CheckBound(int index) const
    {
        return (index >= 0) && (index < EMULATED_PRF_SIZE);
    }

    inline uint64_t PhysicalRegisterFile::Get(int index) const
    {
        return prfs[index];
    }

    inline void PhysicalRegisterFile::Set(int index, uint64_t value)
    {
        writing_index = index;
        writing_value = value;
    }

    void PhysicalRegisterFile::Eval()
    {
        if (writing_index < 0)
            return;

        prfs[writing_index] = writing_value;
        
        writing_index = -1;
    }
}

