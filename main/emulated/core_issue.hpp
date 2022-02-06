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


    class RegisterAliasTable final : public MEMU::Emulated
    {
    public:
        class Entry {
        private:
            int     FID;   // Instruction Fetch ID
            bool    FV;    // Instruction On-flight Valid !!Removal Consideration!!
            bool    NRA;   // Not reallocate-able Flag

            int     PRF;   // PRF (Physical Register File) address
            int     ARF;   // ARF (Architectural Register File) mapping address
            bool    V;     // Mapping entry Valid

        public:
            Entry();
            Entry(const Entry& obj);
            ~Entry();

            void        Clear();

            int         GetFID() const;
            bool        GetFV() const; // !!Removal Consideration!!
            bool        GetNRA() const;
            int         GetPRF() const;
            int         GetARF() const;
            bool        GetValid() const;

            void        SetFID(int val);
            void        SetFV(bool val); // !!Removal Consideration!!
            void        SetNRA(bool val);
            void        SetPRF(int val);
            void        SetARF(int val);
            void        SetValid(bool val);

            uint64_t    GetValue(const PhysicalRegisterFile& prf) const;
            void        SetValue(PhysicalRegisterFile& prf, uint64_t val);

            void        operator=(const Entry& obj);
        };

        // *NOTICE: Indicating only one operation could be applied to a single entry
        //          in a single eval() clock.
        class EntryModification {
        private:
            int     index;
            Entry   entry;

        public:
            EntryModification(int index, const Entry& base);
            EntryModification(const EntryModification& obj);
            ~EntryModification();

            int             GetIndex() const;
            void            SetIndex(int index);

            const Entry&    GetEntry() const;
            Entry&          GetEntry();
            void            SetEntry(const Entry& entry);

            void            Apply(Entry& dst) const;
        };

        class GlobalCheckpoint {
        private:
            constexpr static int    gc_size = EMULATED_RAT_SIZE;

            bitset<gc_size>         V; // Saved Mapping entry Valid

        public:
            GlobalCheckpoint();
            GlobalCheckpoint(const GlobalCheckpoint& obj);
            ~GlobalCheckpoint();

            constexpr int               GetSize() const;

            bool                        GetValid(int index) const;
            void                        SetValid(int index, bool valid);

            void                        SetAll();
            void                        ResetAll();
        };

    private:
        constexpr static int    rat_size     = EMULATED_RAT_SIZE;
        constexpr static int    rat_gc_count = EMULATED_RAT_GC_COUNT;

        Entry*                  entries     /*[rat_size]*/;
        GlobalCheckpoint*       checkpoints /*[rat_gc_count]*/;  

        list<EntryModification> modified; 
        int                     rollback;
        int                     snapshot;

    private:
        int                 GetNextEntry() const;
        void                Invalidate(int ARF);
        void                InvalidateAndRelease(int ARF);
        void                Land(int ARF);
        void                Release(int ARF);
        bool                Touch(bool FV, int FID, int ARF, int* PRF);

    public:
        RegisterAliasTable();
        RegisterAliasTable(const RegisterAliasTable& obj);
        ~RegisterAliasTable();

        constexpr int           GetSize() const;
        constexpr int           GetCheckpointCount() const;

        const Entry&            GetEntry(int index) const;
        void                    SetEntry(int index, const Entry& entry);

        const GlobalCheckpoint& GetCheckpoint(int index) const;
        void                    SetCheckpoint(int index, const GlobalCheckpoint& checkpoint);

        int                     GetAliasPRF(int arf);

        void                    Clear();
        bool                    IsFull() const;

        bool                    Touch(int FID, int ARF, int* PRF = 0);
        bool                    TouchOnFlight(int FID, int ARF, int* PRF = 0);
        void                    Writeback(int FID);
        void                    Commit(int ARF);
        bool                    CommitFID(int FID);
        bool                    TouchAndWriteback(int FID, int ARF, int* PRF = 0);
        bool                    TouchAndCommit(int FID, int ARF, int* PRF = 0);

        void                    WriteCheckpoint(int GC);

        void                    Rollback(int GC);

        void                    ResetInput();

        virtual void            Eval() override;
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

    void RegisterAliasTable::Entry::Clear()
    {
        FID = 0;
        FV  = false;
        NRA = false;
        PRF = 0;
        ARF = 0;
        V   = false;
    }

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

    inline uint64_t RegisterAliasTable::Entry::GetValue(const PhysicalRegisterFile& prf) const
    {
        return prf.Get(PRF);
    }

    inline void RegisterAliasTable::Entry::SetValue(PhysicalRegisterFile& prf, uint64_t val)
    {
        prf.Set(PRF, val);
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

    inline void RegisterAliasTable::GlobalCheckpoint::SetAll()
    {
        V.set();
    }

    inline void RegisterAliasTable::GlobalCheckpoint::ResetAll()
    {
        V.reset();
    }
}


// class MEMU::Core::Issue::RegisterAliasTable::EntryModification
namespace MEMU::Core::Issue {
    /*
    int     index;
    Entry   entry;
    */

    RegisterAliasTable::EntryModification::EntryModification(int index, const Entry& base)
        : index (index)
        , entry (base)
    { }

    RegisterAliasTable::EntryModification::EntryModification(const EntryModification& obj)
        : index (obj.index)
        , entry (obj.entry)
    { }

    RegisterAliasTable::EntryModification::~EntryModification()
    { }

    inline int RegisterAliasTable::EntryModification::GetIndex() const
    {
        return index;
    }

    inline void RegisterAliasTable::EntryModification::SetIndex(int index)
    {
        this->index = index;
    }

    inline const RegisterAliasTable::Entry& RegisterAliasTable::EntryModification::GetEntry() const
    {
        return entry;
    }

    inline RegisterAliasTable::Entry& RegisterAliasTable::EntryModification::GetEntry()
    {
        return entry;
    }

    inline void RegisterAliasTable::EntryModification::SetEntry(const Entry& entry)
    {
        this->entry = entry;
    }

    inline void RegisterAliasTable::EntryModification::Apply(Entry& dst) const
    {
        dst = entry;
    }
}


// class MEMU::Core::Issue::RegisterAliasTable
namespace MEMU::Core::Issue {
    /*
    constexpr static int    rat_size     = EMULATED_RAT_SIZE;
    constexpr static int    rat_gc_count = EMULATED_RAT_GC_COUNT;

    Entry*                  entries     //[rat_size];
    GlobalCheckpoint*       checkpoints //[rat_gc_count];  

    list<EntryModification> modified;
    int                     rollback;
    */

    RegisterAliasTable::RegisterAliasTable()
        : entries       (new Entry[rat_size]())
        , checkpoints   (new GlobalCheckpoint[rat_gc_count]())
        , modified      (list<EntryModification>())
        , rollback      (-1)
        , snapshot      (-1)
    { 
        for (int i = 0; i < rat_size; i++)
            entries[i].SetPRF(i);
    }    

    RegisterAliasTable::RegisterAliasTable(const RegisterAliasTable& obj)
        : entries       (new Entry[rat_size])
        , checkpoints   (new GlobalCheckpoint[rat_gc_count])
        , modified      (obj.modified)
        , rollback      (obj.rollback)
        , snapshot      (obj.snapshot)
    {
        memcpy(entries, obj.entries, rat_size * sizeof(Entry));
        memcpy(checkpoints, obj.checkpoints, rat_gc_count * sizeof(GlobalCheckpoint));
    }
    
    RegisterAliasTable::~RegisterAliasTable()
    {
        delete[] entries;
        delete[] checkpoints;
    }

    constexpr int RegisterAliasTable::GetSize() const
    {
        return rat_size;
    }

    constexpr int RegisterAliasTable::GetCheckpointCount() const
    {
        return rat_gc_count;
    }

    const RegisterAliasTable::Entry& RegisterAliasTable::GetEntry(int index) const
    {
        return entries[index];
    }

    void RegisterAliasTable::SetEntry(int index, const Entry& entry)
    {
        entries[index] = entry;
    }

    const RegisterAliasTable::GlobalCheckpoint& RegisterAliasTable::GetCheckpoint(int index) const
    {
        return checkpoints[index];
    }

    void RegisterAliasTable::SetCheckpoint(int index, const GlobalCheckpoint& checkpoint)
    {
        checkpoints[index] = checkpoint;
    }

    int RegisterAliasTable::GetAliasPRF(int arf)
    {
        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetValid() && (entries[i].GetARF() == arf))
                return entries[i].GetPRF();

        return -1;
    }

    void RegisterAliasTable::Clear()
    {
        for (int i = 0; i < rat_size; i++)
        {
            entries[i].Clear();
            entries[i].SetPRF(i);
        }
    }

    bool RegisterAliasTable::IsFull() const
    {
        for (int i = 0; i < rat_size; i++)
            if (!entries[i].GetNRA())
                return false;

        return true;
    }

    int RegisterAliasTable::GetNextEntry() const
    {
        for (int i = 0; i < rat_size; i++)
            if (!entries[i].GetNRA())
                return i;

        return -1;
    }

    void RegisterAliasTable::Invalidate(int ARF)
    {
        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetARF() == ARF)
            {
                Entry entry = entries[i];
                entry.SetValid(false);

                modified.push_back(EntryModification(i, entry));

                // *NOTICE: Break on invalidation, multiple V is fault, keep it
                break;
            }
    }

    void RegisterAliasTable::Release(int ARF)
    {
        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetARF() == ARF)
            {
                Entry entry = entries[i];
                entry.SetNRA(false);

                modified.push_back(EntryModification(i, entry));

                // *NOTICE: Multiple NRA allowed, shouldn't break
                //break;
            }
    }

    void RegisterAliasTable::InvalidateAndRelease(int ARF)
    {
        bool invalidated;

        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetARF() == ARF)
            {
                Entry entry = entries[i];
                
                if (!invalidated)
                {
                    entry.SetValid(false);
                    invalidated = true;
                }

                entry.SetNRA(false);

                modified.push_back(EntryModification(i, entry));

                //
            }
    }

    void RegisterAliasTable::Land(int FID)
    {
        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetFID() == FID && entries[i].GetFV())
            {
                Entry entry = entries[i];
                entry.SetFV(false);

                modified.push_back(EntryModification(i, entry));

                break;
            }
    }

    bool RegisterAliasTable::Touch(bool FV, int FID, int ARF, int* PRF)
    {
        int index = GetNextEntry();

        if (index < 0)
            return false;

        // Invalidate existed same-ARF entry
        if (FV)
            Invalidate(ARF);
        else
            InvalidateAndRelease(ARF);

        // Pre-touch means instruction on flight
        Entry entry = entries[index];
        entry.SetFID    (FID);
        entry.SetFV     (FV);
        entry.SetNRA    (true);
        entry.SetARF    (ARF);
        entry.SetValid  (true);

        if (PRF)
            *PRF = entry.GetPRF();

        //
        modified.push_back(EntryModification(index, entry));

        return true;
    }

    inline bool RegisterAliasTable::Touch(int FID, int ARF, int* PRF)
    {
        return Touch(false, FID, ARF, PRF);
    }

    inline bool RegisterAliasTable::TouchOnFlight(int FID, int ARF, int* PRF)
    {
        return Touch(true, FID, ARF, PRF);
    }

    void RegisterAliasTable::Writeback(int FID)
    {
        // nothing need to be done in RAT
    }

    void RegisterAliasTable::Commit(int ARF)
    {
        Release(ARF);
    }

    /* DEPRECATED */
    bool RegisterAliasTable::CommitFID(int FID)
    {
        int ARF = -1;

        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetFID() == FID && entries[i].GetNRA())
            {
                ARF = i;
                break;
            }

        if (ARF == -1)
            return false;

        Release(ARF);

        return true;
    }

    bool RegisterAliasTable::TouchAndWriteback(int FID, int ARF, int* PRF)
    {
        if (!Touch(false, FID, ARF, PRF))
            return false;

        return true;
    }

    bool RegisterAliasTable::TouchAndCommit(int FID, int ARF, int* PRF)
    {
        if (!Touch(false, FID, ARF, PRF))
            return false;

        return true;
    }

    void RegisterAliasTable::WriteCheckpoint(int GC)
    {
        snapshot = GC;
    }

    void RegisterAliasTable::Rollback(int GC)
    {
        rollback = GC;
    }

    void RegisterAliasTable::ResetInput()
    {
        modified.clear();
        rollback = -1;
        snapshot = -1;
    }

    void RegisterAliasTable::Eval()
    {
        // Write checkpoint
        if (snapshot >= 0)
        {
            for (int i = 0; i < rat_size; i++)
                checkpoints[snapshot].SetValid(i, entries[i].GetValid());
        }

        // Recovery from checkpoint
        if (rollback >= 0)
        {
            for (int i = 0; i < rat_size; i++)
                entries[i].SetValid(checkpoints[rollback].GetValid(i));
        }
        
        // Write entries
        list<EntryModification>::iterator iter = modified.begin();
        while (iter != modified.end())
        {
            entries[iter->GetIndex()] = iter->GetEntry();
            iter++;
        }

        //
        ResetInput();
    }
}
