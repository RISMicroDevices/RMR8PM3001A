#pragma once
//
// Mixed emulation for Issue Stage
//
//

#include <list>
#include <cstring>
#include <bitset>

#include "base.hpp"
#include "core_global.hpp"

#define EMULATED_PRF_SIZE                           64

#define EMULATED_RAT_SIZE                           EMULATED_PRF_SIZE
#define EMULATED_RAT_GC_COUNT                       EMULATED_GC_COUNT

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
            int     FID;   // Instruction Fetch ID
            bool    FV;    // Instruction On-flight Valid
            bool    NRA;   // Not reallocate-able Flag

            int     PRF;   // PRF (Physical Register File) address
            int     ARF;   // ARF (Architectural Register File) mapping address
            bool    V;     // Mapping entry Valid

            Entry();
            Entry(const Entry& obj);
            ~Entry();

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

            void    operator=(const Entry& obj);
        };

        class GlobalCheckpoint {
        private:
            constexpr static int    gc_size = EMULATED_RAT_SIZE;

        public:
            bitset<gc_size>         V; // Saved Mapping entry Valid

            GlobalCheckpoint();
            GlobalCheckpoint(const GlobalCheckpoint& obj);
            ~GlobalCheckpoint();

            constexpr int               GetSize() const;

            bool                        GetValid(int index) const;
            void                        SetValid(int index, bool valid);

            void                        operator=(const GlobalCheckpoint& obj);
            bitset<gc_size>::reference& operator[](const int index);
        };

    private:
        constexpr static int    rat_size     = EMULATED_RAT_SIZE;
        constexpr static int    rat_gc_count = EMULATED_RAT_GC_COUNT;

        Entry                   entries     [rat_size];
        GlobalCheckpoint        checkpoints [rat_gc_count];   

    public:
        RegisterAliasTable();
        RegisterAliasTable(const RegisterAliasTable& obj);
        ~RegisterAliasTable();

        constexpr int       GetSize() const;
        constexpr int       GetCheckpointCount() const;

        Entry*              GetEntryReference(int index);
        Entry               GetEntry(int index) const;
        void                SetEntry(int index, Entry entry);

        GlobalCheckpoint*   GetCheckpointReference(int index);
        GlobalCheckpoint    GetCheckpoint(int index) const;
        void                SetCheckpoint(int index, GlobalCheckpoint checkpoint);


        // TODO

        virtual void        Eval() override;
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

    inline int64_t PhysicalRegisterFile::Get(int index) const
    {
        return prfs[index];
    }

    inline void PhysicalRegisterFile::Set(int index, int64_t value)
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


// class MEMU::Core::Issue::RegisterAliasTable::Entry
namespace MEMU::Core::Issue {
    /*
    int     FID;
    bool    FV;
    bool    NRA;

    int     PRF;
    int     ARF;
    bool    V;
    */

    RegisterAliasTable::Entry::Entry()
        : FID   (0)
        , FV    (false)
        , NRA   (false)
        , PRF   (0)
        , ARF   (0)
        , V     (false)
    { }

    RegisterAliasTable::Entry::Entry(const RegisterAliasTable::Entry& obj)
        : FID   (obj.FID)
        , FV    (obj.FV)
        , NRA   (obj.NRA)
        , PRF   (obj.PRF)
        , ARF   (obj.ARF)
        , V     (obj.V)
    { }

    RegisterAliasTable::Entry::~Entry()
    { }

    inline int RegisterAliasTable::Entry::GetFID() const
    {
        return FID;
    }

    inline bool RegisterAliasTable::Entry::GetFV() const
    {
        return FV;
    }

    inline bool RegisterAliasTable::Entry::GetNRA() const
    {
        return NRA;
    }

    inline int RegisterAliasTable::Entry::GetPRF() const
    {
        return PRF;
    }

    inline int RegisterAliasTable::Entry::GetARF() const
    {
        return ARF;
    }

    inline bool RegisterAliasTable::Entry::GetValid() const
    {
        return V;
    }

    inline void RegisterAliasTable::Entry::SetFID(int val)
    {
        FID = val;
    }

    inline void RegisterAliasTable::Entry::SetFV(bool val)
    {
        FV = val;
    }

    inline void RegisterAliasTable::Entry::SetNRA(bool val)
    {
        NRA = val;
    }

    inline void RegisterAliasTable::Entry::SetPRF(int val)
    {
        PRF = val;
    }

    inline void RegisterAliasTable::Entry::SetARF(int val)
    {
        ARF = val;
    }

    inline void RegisterAliasTable::Entry::SetValid(bool val)
    {
        V = val;
    }

    void RegisterAliasTable::Entry::operator=(const RegisterAliasTable::Entry& obj)
    {
        FID = obj.FID;
        FV = obj.FV;
        NRA = obj.NRA;
        PRF = obj.PRF;
        ARF = obj.ARF;
        V = obj.V;
    }
}


// class MEMU::Core::Issue::RegisterAliasTable::GlobalCheckpoint
namespace MEMU::Core::Issue {
    /*
    constexpr static int    gc_size = EMULATED_RAT_SIZE;

    bitset<gc_size>         V;
    */

    RegisterAliasTable::GlobalCheckpoint::GlobalCheckpoint()
    { }

    RegisterAliasTable::GlobalCheckpoint::GlobalCheckpoint(const RegisterAliasTable::GlobalCheckpoint& obj)
        : V(obj.V)
    { }

    RegisterAliasTable::GlobalCheckpoint::~GlobalCheckpoint()
    { }

    constexpr int RegisterAliasTable::GlobalCheckpoint::GetSize() const
    {
        return gc_size;
    }

    inline bool RegisterAliasTable::GlobalCheckpoint::GetValid(int index) const
    {
        return V[index];
    }

    inline void RegisterAliasTable::GlobalCheckpoint::SetValid(int index, bool valid)
    {
        V[index] = valid;
    }

    void RegisterAliasTable::GlobalCheckpoint::operator=(const RegisterAliasTable::GlobalCheckpoint& obj)
    {
        V = obj.V;
    }

    bitset<EMULATED_RAT_SIZE>::reference& RegisterAliasTable::GlobalCheckpoint::operator[](const int index)
    {
        return V[index];
    }
}

