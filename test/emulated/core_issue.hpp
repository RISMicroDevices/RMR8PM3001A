#pragma once
//
// Mixed emulation for RAT Freelist Checkpoint
//
//  @see 'rat_freelist_checkpoint.v'
//

#include <list>
#include <cstring>

#include "base.hpp"

#define EMULATED_PRF_SIZE                                           64

#define EMULATED_RAT_SIZE                                           EMULATED_PRF_SIZE
#define EMULATED_RAT_GC_COUNT                                       8

using namespace std;

namespace MEMU::Core::Issue {

    class PhysicalRegisterFile final : public MEMU::Emulated
    {
    private:
        int64_t prfs[EMULATED_PRF_SIZE] = { 0 };
        int     writing_index;
        int64_t writing_value;

    public:
        PhysicalRegisterFile();
        PhysicalRegisterFile(const PhysicalRegisterFile& obj);
        ~PhysicalRegisterFile();

        constexpr int   GetCapacity() const;

        bool            CheckBound(int index) const;

        int64_t         Get(int index) const;
        void            Set(int index, int64_t value);

        virtual void    Eval() override;
    };


    class RegisterAliasTable final : public MEMU::Emulated
    {
    public:
        class Entry {
        public:
            int         FID;   // Instruction Fetch ID
            bool        FV;    // Instruction On-flight Valid
            bool        NRA;   // Not reallocate-able Flag

            const int   PRF;   // PRF (Physical Register File) address
            int         ARF;   // ARF (Architectural Register File) mapping address
            bool        V;     // Mapping entry Valid

            int     GetFID() const;
            bool    GetFV() const;
            bool    GetNRA() const;
            int     GetPRF() const;
            int     GetARF() const;
            bool    GetValid() const;

            void    SetFID(int val);
            void    SetFV(bool val);
            void    SetNRA(bool val);
            void    SetPRF(int val);
            void    SetARF(int val);
            void    SetValid(bool val);
        };

        class GlobalCheckpoint {
        public:
            bool*       V; // saved Mapping entry Valid

            // TODO
        };

    private:

        // TODO

    public:
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

    bool PhysicalRegisterFile::CheckBound(int index) const
    {
        return (index >= 0) && (index < EMULATED_PRF_SIZE);
    }

    int64_t PhysicalRegisterFile::Get(int index) const
    {
        return prfs[index];
    }

    void PhysicalRegisterFile::Set(int index, int64_t value)
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
