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

#define EMULATED_RAT_GC_SIZE                        EMULATED_GC_COUNT                            

using namespace std;

namespace MEMU::Core::Issue {

    class Scoreboard final : public MEMU::Emulated
    {
        // TODO
    };

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


     class ShadowRegisterAliasTable final : public MEMU::Emulated
    {
    public:
        class Entry {
        private:
            int     PRF;    // Physical Register File address
            int     ARF;    // Architectural Register File address
            bool    V;      // Entry valid
            bool    NRA;    // Not reallocate-able flag
        
        public:
            Entry();
            Entry(const Entry& obj);
            ~Entry();

            void        Clear();

            int         GetPRF() const;
            int         GetARF() const;
            bool        GetNRA() const;
            bool        GetValid() const;

            void        SetPRF(int val);
            void        SetARF(int val);
            void        SetNRA(bool val);
            void        SetValid(bool val);

            uint64_t    GetValue(const PhysicalRegisterFile& prf) const;
            void        SetValue(PhysicalRegisterFile& prf, uint64_t val) const;

            void        operator=(const Entry& obj);
        };

        class EntryModification {
        private:
            bool    modified;

            bool    modified_ARF;
            bool    modified_V;
            bool    modified_NRA;

            int     ARF;
            bool    V;
            bool    NRA;

        public:
            EntryModification();
            EntryModification(const EntryModification& obj);
            ~EntryModification();

            bool    IsModified() const;

            bool    IsARFModified() const;
            bool    IsNRAModified() const;
            bool    IsValidModified() const;

            void    SetARF(int ARF);
            void    SetNRA(bool NRA);
            void    SetValid(bool V);

            void    Reset();
            void    Apply(Entry& dst) const;
        };

    private:
        constexpr static int    rat_size     = EMULATED_RAT_SIZE;

        Entry*                  entries      /*[rat_size]*/;

        EntryModification*      modification /*[rat_size]*/;

    public:
        ShadowRegisterAliasTable();
        ShadowRegisterAliasTable(const ShadowRegisterAliasTable& obj);
        ~ShadowRegisterAliasTable();

        constexpr int       GetSize() const;

        const Entry&        GetEntry(int index) const;
        void                SetEntry(int index, const Entry& entry);

        int                 GetAliasPRF(int arf) const;

        void                Clear();

        void                Commit(int PRF, int ARF);

        void                ResetInput();

        virtual void        Eval() override;
    };


    class RegisterAliasTable final : public MEMU::Emulated
    {
    public:
        class Entry {
        private:
            int     FID;   // Instruction Fetch ID
            bool    FV;    // Instruction On-flight Valid
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
            bool        GetFV() const; //
            bool        GetNRA() const;
            int         GetPRF() const;
            int         GetARF() const;
            bool        GetValid() const;

            void        SetFID(int val);
            void        SetFV(bool val); //
            void        SetNRA(bool val);
            void        SetPRF(int val);
            void        SetARF(int val);
            void        SetValid(bool val);

            uint64_t    GetValue(const PhysicalRegisterFile& prf) const;
            void        SetValue(PhysicalRegisterFile& prf, uint64_t val) const;

            void        operator=(const Entry& obj);
        };

        class EntryModification {
        private:
            bool    modified;

            bool    modified_FID;
            bool    modified_FV;
            bool    modified_NRA;
            bool    modified_ARF;
            bool    modified_V;

            int     FID;
            bool    FV;
            bool    NRA;

            int     ARF;
            bool    V;

        public:
            EntryModification();
            EntryModification(const EntryModification& obj);
            ~EntryModification();

            bool    IsModified() const;

            bool    IsFIDModified() const;
            bool    IsFVModified() const;
            bool    IsNRAModified() const;
            bool    IsARFModified() const;
            bool    IsValidModified() const;

            void    SetFID(int FID);
            void    SetFV(bool FV);
            void    SetNRA(bool NRA);
            void    SetARF(int ARF);
            void    SetValid(bool V);

            void    Reset();
            void    Apply(Entry& dst) const;
        };

        class CheckpointEntry {
        private:
            bool    V;

        public:
            CheckpointEntry();
            CheckpointEntry(const CheckpointEntry& obj);
            ~CheckpointEntry();

            bool    GetValid() const;
            void    SetValid(bool V);

            void    Allocate(const Entry& entry);
            void    Restore(Entry& entry) const;
        };

        class Checkpoint {
        private:
            constexpr static int    gc_size      = EMULATED_RAT_GC_SIZE;

            CheckpointEntry*    entries;

        public:
            Checkpoint();
            Checkpoint(const Checkpoint& obj);
            ~Checkpoint();

            int                     GetSize() const;
            bool                    CheckBound(int index) const;

            const CheckpointEntry&  GetEntry(int index) const;
            CheckpointEntry&        GetEntry(int index);
            void                    SetEntry(int index, const CheckpointEntry& entry);

            bool                    GetValid(int index) const;
            void                    SetValid(int index, bool V);

            void                    Allocate(const Entry* entries);
            void                    Restore(Entry* entries) const;
        };

    private:
        constexpr static int    rat_size     = EMULATED_RAT_SIZE;

        constexpr static int    gc_size      = EMULATED_RAT_GC_SIZE;

        Entry*                  entries      /*[rat_size]*/;

        EntryModification*      modification /*[rat_size]*/; 

        Checkpoint*             checkpoints   /*[gc_size]*/;

        int                     checkpoint_allocate;
        int                     checkpoint_restore;


    private:
        int                 GetNextEntry() const;
        void                Invalidate(int ARF);
        void                InvalidateAndRelease(int ARF);
        void                LandAndRelease(int ARF);
        bool                Touch(bool FV, int FID, int ARF, int* PRF);

    public:
        RegisterAliasTable();
        RegisterAliasTable(const RegisterAliasTable& obj);
        ~RegisterAliasTable();

        constexpr int           GetSize() const;

        const Entry&            GetEntry(int index) const;
        void                    SetEntry(int index, const Entry& entry);

        int                     GetAliasPRF(int arf) const;

        void                    Clear();
        bool                    IsFull() const;

        bool                    Touch(int FID, int ARF, int* PRF = 0);
        void                    Writeback(int PRF);
        void                    Commit(int PRF);
        bool                    TouchAndWriteback(int FID, int ARF, int* PRF = 0);
        bool                    TouchAndCommit(int FID, int ARF, int* PRF = 0);

        void                    RestoreNRA(const ShadowRegisterAliasTable& sRAT);

        void                    AllocateCheckpoint(int index);
        void                    RestoreCheckpoint(int index);

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
        : FID   (-1)
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
        FID = -1;
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

    inline void RegisterAliasTable::Entry::SetValue(PhysicalRegisterFile& prf, uint64_t val) const
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


// class MEMU::Core::Issue::RegisterAliasTable::EntryModification
namespace MEMU::Core::Issue {
    /*
    bool    modified;

    bool    modified_FID;
    bool    modified_FV;
    bool    modified_NRA;
    bool    modified_PRF;
    bool    modified_V;

    int     FID;
    bool    FV;
    bool    NRA;

    int     PRF;
    bool    V;
    */

    RegisterAliasTable::EntryModification::EntryModification()
        : modified      (false)
        , modified_FID  (false)
        , modified_FV   (false)
        , modified_NRA  (false)
        , modified_ARF  (false)
        , modified_V    (false)
        , FID           (0)
        , FV            (false)
        , NRA           (false)
        , ARF           (0)
        , V             (false)
    { }

    RegisterAliasTable::EntryModification::EntryModification(const EntryModification& obj)
        : modified      (obj.modified)
        , modified_FID  (obj.modified_FID)
        , modified_FV   (obj.modified_FV)
        , modified_NRA  (obj.modified_NRA)
        , modified_ARF  (obj.modified_ARF)
        , modified_V    (obj.modified_V)
        , FID           (obj.FID)
        , FV            (obj.FV)
        , NRA           (obj.NRA)
        , ARF           (obj.ARF)
        , V             (obj.V)
    { }

    RegisterAliasTable::EntryModification::~EntryModification()
    { }

    inline bool RegisterAliasTable::EntryModification::IsModified() const
    {
        return modified;
    }

    inline bool RegisterAliasTable::EntryModification::IsFIDModified() const
    {
        return modified_FID;
    }

    inline bool RegisterAliasTable::EntryModification::IsFVModified() const
    {
        return modified_FV;
    }

    inline bool RegisterAliasTable::EntryModification::IsNRAModified() const
    {
        return modified_NRA;
    }

    inline bool RegisterAliasTable::EntryModification::IsARFModified() const
    {
        return modified_ARF;
    }

    inline bool RegisterAliasTable::EntryModification::IsValidModified() const
    {
        return modified_V;
    }

    inline void RegisterAliasTable::EntryModification::SetFID(int FID)
    {
        modified     = true;
        modified_FID = true;

        this->FID = FID;
    }

    inline void RegisterAliasTable::EntryModification::SetFV(bool FV)
    {
        modified    = true;
        modified_FV = true;

        this->FV = FV;
    }

    inline void RegisterAliasTable::EntryModification::SetNRA(bool NRA)
    {
        modified     = true;
        modified_NRA = true;

        this->NRA = NRA;
    }

    inline void RegisterAliasTable::EntryModification::SetARF(int ARF)
    {
        modified     = true;
        modified_ARF = true;

        this->ARF = ARF;
    }

    inline void RegisterAliasTable::EntryModification::SetValid(bool V)
    {
        modified   = true;
        modified_V = true;

        this->V = V;
    }

    void RegisterAliasTable::EntryModification::Reset()
    {
        modified     = false;

        modified_FID = false;
        modified_FV  = false;
        modified_NRA = false;
        modified_ARF = false;
        modified_V   = false;
    }

    void RegisterAliasTable::EntryModification::Apply(Entry& dst) const
    {
        if (modified_FID)
            dst.SetFID(FID);

        if (modified_FV)
            dst.SetFV(FV);

        if (modified_NRA)
            dst.SetNRA(NRA);

        if (modified_ARF)
            dst.SetARF(ARF);

        if (modified_V)
            dst.SetValid(V);
    }
}


// class MEMU::Core::Issue::RegisterAliasTable::CheckpointEntry
namespace MEMU::Core::Issue {
    /*
    bool    V;
    */

    RegisterAliasTable::CheckpointEntry::CheckpointEntry()
        : V (false)
    { }

    RegisterAliasTable::CheckpointEntry::CheckpointEntry(const CheckpointEntry& obj)
        : V (obj.V)
    { }

    RegisterAliasTable::CheckpointEntry::~CheckpointEntry()
    { }

    inline bool RegisterAliasTable::CheckpointEntry::GetValid() const
    {
        return V;
    }

    inline void RegisterAliasTable::CheckpointEntry::SetValid(bool V)
    {
        this->V = V;
    }

    inline void RegisterAliasTable::CheckpointEntry::Allocate(const Entry& entry)
    {
        V = entry.GetValid();
    }

    inline void RegisterAliasTable::CheckpointEntry::Restore(Entry& entry) const
    {
        entry.SetValid(V);
    }
}


// class MEMU::Core::Issue::RegisterAliasTable::Checkpoint
namespace MEMU::Core::Issue {
    /*
    constexpr static int    gc_size      = EMULATED_RAT_GC_SIZE;

    CheckpointEntry*    entries;
    */

    RegisterAliasTable::Checkpoint::Checkpoint()
        : entries   (new CheckpointEntry[gc_size]())
    { }

    RegisterAliasTable::Checkpoint::Checkpoint(const Checkpoint& obj)
        : entries   (new CheckpointEntry[gc_size])
    {
        for (int i = 0; i < gc_size; i++)
            new (&entries[i]) CheckpointEntry(obj.entries[i]);
    }

    RegisterAliasTable::Checkpoint::~Checkpoint()
    {
        delete[] entries;
    }

    inline int RegisterAliasTable::Checkpoint::GetSize() const
    {
        return gc_size;
    }

    inline bool RegisterAliasTable::Checkpoint::CheckBound(int index) const
    {
        return index >= 0 && index < gc_size;
    }

    inline const RegisterAliasTable::CheckpointEntry& RegisterAliasTable::Checkpoint::GetEntry(int index) const
    {
        return entries[index];
    }

    inline RegisterAliasTable::CheckpointEntry& RegisterAliasTable::Checkpoint::GetEntry(int index)
    {
        return entries[index];
    }

    inline void RegisterAliasTable::Checkpoint::SetEntry(int index, const CheckpointEntry& entry)
    {
        entries[index] = entry;
    }

    inline bool RegisterAliasTable::Checkpoint::GetValid(int index) const
    {
        return entries[index].GetValid();
    }

    inline void RegisterAliasTable::Checkpoint::SetValid(int index, bool V)
    {
        entries[index].SetValid(V);
    }

    void RegisterAliasTable::Checkpoint::Allocate(const Entry* entries)
    {
        for (int i = 0; i < gc_size; i++)
            this->entries[i].Allocate(entries[i]);
    }

    void RegisterAliasTable::Checkpoint::Restore(Entry* entries) const
    {
        for (int i = 0; i < gc_size; i++)
            this->entries[i].Restore(entries[i]);
    }
}


// class MEMU::Core::Issue::RegisterAliasTable
namespace MEMU::Core::Issue {
    /*
    constexpr static int    rat_size     = EMULATED_RAT_SIZE;

    constexpr static int    gc_size      = EMULATED_RAT_GC_SIZE;

    Entry*                  entries;      // [rat_size]

    EntryModification*      modification; // [rat_size]

    Checkpoint*             checkpoints;   // [gc_size]

    int                     checkpoint_allocate;
    int                     checkpoint_restore;
    */

    RegisterAliasTable::RegisterAliasTable()
        : entries               (new Entry[rat_size]())
        , modification          (new EntryModification[rat_size]())
        , checkpoints           (new Checkpoint[gc_size]())
        , checkpoint_allocate   (-1)
        , checkpoint_restore    (-1)
    { 
        for (int i = 0; i < rat_size; i++)
            entries[i].SetPRF(i);
    }    

    RegisterAliasTable::RegisterAliasTable(const RegisterAliasTable& obj)
        : entries               (new Entry[rat_size])
        , modification          (new EntryModification[rat_size])
        , checkpoints           (new Checkpoint[gc_size])
        , checkpoint_allocate   (obj.checkpoint_allocate)
        , checkpoint_restore    (obj.checkpoint_restore)
    {
        for (int i = 0; i < rat_size; i++)
        {
            new (&entries[i])      Entry(obj.entries[i]);
            new (&modification[i]) EntryModification(obj.modification[i]);
        }

        for (int i = 0; i < gc_size; i++)
        {
            new (&checkpoints[i]) Checkpoint(obj.checkpoints[i]);
        }
    }
    
    RegisterAliasTable::~RegisterAliasTable()
    {
        delete[] entries;
        delete[] modification;
    }

    constexpr int RegisterAliasTable::GetSize() const
    {
        return rat_size;
    }

    const RegisterAliasTable::Entry& RegisterAliasTable::GetEntry(int index) const
    {
        return entries[index];
    }

    void RegisterAliasTable::SetEntry(int index, const Entry& entry)
    {
        entries[index] = entry;
    }

    int RegisterAliasTable::GetAliasPRF(int arf) const
    {
        if (!arf)
            return -1;

        // PRF Query CAM
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
        // Selection tree.
        for (int i = 0; i < rat_size; i++)
            if (!entries[i].GetNRA())
                return i;

        return -1;
    }

    void RegisterAliasTable::Invalidate(int ARF)
    {
        // pre-touch
        // no self-existence

        if (!ARF)
            return;

        // Invalidate all other same-ARF entries.
        // Should be implemented with a CAM in actual circuit.
        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetValid() && entries[i].GetARF() == ARF)
            {
                modification[i].SetValid(false);

                // *NOTICE: Break on invalidation, multiple V is fault, keep it
                //          Only in simulation situation.
                break;
            }
    }

    void RegisterAliasTable::LandAndRelease(int PRF)
    {
        // post-touch
        // self-existence

        // In actual circuit design, ARF and PRF info can be passed through the 
        // inter-pipeline datapath. There is no need to implement another ARF
        // query CAM circuit.
        int ARF = entries[PRF].GetARF();
        
        // Land: Set FV to false
        // Reset of FV bit MUST HAVE the LOWEST 
        if (!modification[PRF].IsFVModified())
            modification[PRF].SetFV(false);

        // Release: Set NRA of all non-FV same-ARF entries to false
        for (int i = 0; i < rat_size; i++)
            if (!entries[i].GetFV() && entries[i].GetARF() == ARF)
            {
                // Reset of NRA bit MUST HAVE the LOWEST priority
                if (modification[i].IsNRAModified())
                    continue;

                modification[i].SetNRA(false);

                // *NOTICE: Multiple NRA allowed, shouldn't break
                //break;
            }
    }

    void RegisterAliasTable::InvalidateAndRelease(int ARF)
    {
        // no self-existence, 0 clk on-flight

        // This circumstance only exists in simulation.

        if (!ARF)
            return;

        bool invalidated = false;

        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetARF() == ARF)
            {
                if (!invalidated && entries[i].GetValid())
                {
                    modification[i].SetValid(false);
                    invalidated = true;
                }

                modification[i].SetNRA(false);
                //
            }
    }

    bool RegisterAliasTable::Touch(bool FV, int FID, int ARF, int* PRF)
    {
        if (!ARF)
            return true;

        int index = GetNextEntry();

        if (index < 0)
            return false;

        // Invalidate existed same-ARF entry
        if (FV)
            Invalidate(ARF);
        else
            InvalidateAndRelease(ARF);

        // Pre-touch means instruction on flight
        modification[index].SetFID  (FID);
        modification[index].SetFV   (FV);
        modification[index].SetNRA  (true);
        modification[index].SetARF  (ARF);
        modification[index].SetValid(true);

        if (PRF)
            *PRF = index;

        return true;
    }

    inline bool RegisterAliasTable::Touch(int FID, int ARF, int* PRF)
    {
        return Touch(true, FID, ARF, PRF);
    }

    void RegisterAliasTable::Writeback(int PRF)
    {
        // nothing need to be done in RAT
    }

    void RegisterAliasTable::Commit(int PRF)
    {
        LandAndRelease(PRF);
    }

    bool RegisterAliasTable::TouchAndWriteback(int FID, int ARF, int* PRF)
    {
        return Touch(true, FID, ARF, PRF);
    }

    bool RegisterAliasTable::TouchAndCommit(int FID, int ARF, int* PRF)
    {
        return Touch(false, FID, ARF, PRF);
    }

    void RegisterAliasTable::RestoreNRA(const ShadowRegisterAliasTable& sRAT)
    {
        // HIGHEST PRIORITY write
        for (int i = 0; i < rat_size; i++)
            modification[i].SetNRA(sRAT.GetEntry(i).GetNRA());
    }

    void RegisterAliasTable::AllocateCheckpoint(int index)
    {
        checkpoint_allocate = index;
    }

    void RegisterAliasTable::RestoreCheckpoint(int index)
    {
        checkpoint_restore = index;
    }

    void RegisterAliasTable::ResetInput()
    {
        for (int i = 0; i < rat_size; i++)
            modification[i].Reset();

        checkpoint_allocate = -1;
        checkpoint_restore  = -1;
    }

    void RegisterAliasTable::Eval()
    {
        // Write entries
        for (int i = 0; i < rat_size; i++)
        {
            if (modification[i].IsModified())
                modification[i].Apply(entries[i]);
        }

        // Checkpoint allocate
        if (checkpoint_allocate != -1)
            checkpoints[checkpoint_allocate].Allocate(entries);

        // Checkpoint restore
        if (checkpoint_restore != -1)
            checkpoints[checkpoint_restore].Restore(entries);

        //
        ResetInput();
    }
}


// class MEMU::Core::Issue::ShadowRegisterAliasTable::Entry
namespace MEMU::Core::Issue {
    /*
    int     PRF;    // Physical Register File address
    int     ARF;    // Architectural Register File address
    bool    V;      // Entry valid
    bool    NRA;    // Not reallocate-able flag
    */

    ShadowRegisterAliasTable::Entry::Entry()
        : PRF   (0)
        , ARF   (0)
        , V     (false)
        , NRA   (false)
    { }

    ShadowRegisterAliasTable::Entry::Entry(const Entry& obj)
        : PRF   (obj.PRF)
        , ARF   (obj.ARF)
        , V     (obj.V)
        , NRA   (obj.NRA)
    { }

    ShadowRegisterAliasTable::Entry::~Entry()
    { }

    void ShadowRegisterAliasTable::Entry::Clear()
    {
        PRF = 0;
        ARF = 0;
        V   = false;
        NRA = false;
    }

    inline int ShadowRegisterAliasTable::Entry::GetPRF() const
    {
        return PRF;
    }

    inline int ShadowRegisterAliasTable::Entry::GetARF() const
    {
        return ARF;
    }

    inline bool ShadowRegisterAliasTable::Entry::GetNRA() const
    {
        return NRA;
    }

    inline bool ShadowRegisterAliasTable::Entry::GetValid() const
    {
        return V;
    }

    inline void ShadowRegisterAliasTable::Entry::SetPRF(int val)
    {
        this->PRF = val;
    }

    inline void ShadowRegisterAliasTable::Entry::SetARF(int val)
    {
        this->ARF = val;
    }

    inline void ShadowRegisterAliasTable::Entry::SetNRA(bool val)
    {
        this->NRA = val;
    }

    inline void ShadowRegisterAliasTable::Entry::SetValid(bool val)
    {
        this->V = val;
    }

    inline uint64_t ShadowRegisterAliasTable::Entry::GetValue(const PhysicalRegisterFile& prf) const
    {
        return prf.Get(PRF);
    }

    inline void ShadowRegisterAliasTable::Entry::SetValue(PhysicalRegisterFile& prf, uint64_t val) const
    {
        prf.Set(PRF, val);
    }

    void ShadowRegisterAliasTable::Entry::operator=(const Entry& obj)
    {
        this->PRF = obj.PRF;
        this->ARF = obj.ARF;
        this->V   = obj.V;
        this->NRA = obj.NRA;
    }
}


// class MEMU::Core::Issue::ShadowRegisterAliasTable::EntryModification
namespace MEMU::Core::Issue {
    /*
    bool    modified;

    bool    modified_ARF;
    bool    modified_V;
    bool    modified_NRA;

    int     ARF;
    bool    V;
    bool    NRA;
    */

    ShadowRegisterAliasTable::EntryModification::EntryModification()
        : modified      (false)
        , modified_ARF  (false)
        , modified_V    (false)
        , modified_NRA  (false)
        , ARF           (0)
        , V             (0)
        , NRA           (0)
    { }

    ShadowRegisterAliasTable::EntryModification::EntryModification(const EntryModification& obj)
        : modified      (obj.modified)
        , modified_ARF  (obj.modified_ARF)
        , modified_V    (obj.modified_V)
        , modified_NRA  (obj.modified_NRA)
        , ARF           (obj.ARF)
        , V             (obj.V)
        , NRA           (obj.NRA)
    { }

    ShadowRegisterAliasTable::EntryModification::~EntryModification()
    { }

    inline bool ShadowRegisterAliasTable::EntryModification::IsModified() const
    {
        return modified;
    }

    inline bool ShadowRegisterAliasTable::EntryModification::IsARFModified() const
    {
        return modified_ARF;
    }

    inline bool ShadowRegisterAliasTable::EntryModification::IsNRAModified() const
    {
        return modified_NRA;
    }

    inline bool ShadowRegisterAliasTable::EntryModification::IsValidModified() const
    {
        return modified_V;
    }

    inline void ShadowRegisterAliasTable::EntryModification::SetARF(int ARF)
    {
        modified     = true;
        modified_ARF = true;

        this->ARF = ARF;
    }

    inline void ShadowRegisterAliasTable::EntryModification::SetNRA(bool NRA)
    {
        modified     = true;
        modified_NRA = true;

        this->NRA = NRA;
    }

    inline void ShadowRegisterAliasTable::EntryModification::SetValid(bool V)
    {
        modified   = true;
        modified_V = true;

        this->V = V;
    }

    void ShadowRegisterAliasTable::EntryModification::Reset()
    {
        modified     = false;

        modified_ARF = false;
        modified_V   = false;
        modified_NRA = false;
    }

    void ShadowRegisterAliasTable::EntryModification::Apply(Entry& dst) const
    {
        if (modified_ARF)
            dst.SetARF(ARF);

        if (modified_V)
            dst.SetValid(V);

        if (modified_NRA)
            dst.SetNRA(NRA);
    }
}


// class MEMU::Core::Issue::ShadowRegisterAliasTable
namespace MEMU::Core::Issue {
    /*
    constexpr static int    rat_size     = EMULATED_RAT_SIZE;

    Entry*                  entries;

    EntryModification*      modification;
    */

    ShadowRegisterAliasTable::ShadowRegisterAliasTable()
        : entries       (new Entry[rat_size]())
        , modification  (new EntryModification[rat_size]())
    { 
        for (int i = 0; i < rat_size; i++)
            entries[i].SetPRF(i);
    }

    ShadowRegisterAliasTable::ShadowRegisterAliasTable(const ShadowRegisterAliasTable& obj)
        : entries       (new Entry[rat_size])
        , modification  (new EntryModification[rat_size])
    {
        for (int i = 0; i < rat_size; i++)
        {
            entries[i] = obj.entries[i];
            modification[i] = obj.modification[i];   
        }
    }

    ShadowRegisterAliasTable::~ShadowRegisterAliasTable()
    { 
        delete[] entries;
        delete[] modification;
    }

    constexpr int ShadowRegisterAliasTable::GetSize() const
    {
        return rat_size;
    }

    const ShadowRegisterAliasTable::Entry& ShadowRegisterAliasTable::GetEntry(int index) const
    {
        return entries[index];
    }

    void ShadowRegisterAliasTable::SetEntry(int index, const ShadowRegisterAliasTable::Entry& entry)
    {
        entries[index] = entry;
    }

    int ShadowRegisterAliasTable::GetAliasPRF(int arf) const
    {
        if (!arf)
            return -1;

        // Query in CAM
        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetValid() && entries[i].GetARF() == arf)
                return entries[i].GetPRF();

        return -1;
    }

    void ShadowRegisterAliasTable::Clear()
    {
        for (int i = 0; i < rat_size; i++)
        {
            entries[i].Clear();
            entries[i].SetPRF(i);
        }
    }

    void ShadowRegisterAliasTable::Commit(int PRF, int ARF)
    {
        if (!ARF)
            return;

        // Shadow commit: 
        // Record ARF-PRF relation and assert NRA
        modification[PRF].SetARF(ARF);
        modification[PRF].SetValid(true);
        modification[PRF].SetNRA(true);

        // De-assert all other same-ARF NRAs
        for (int i = 0; i < rat_size; i++)
            if (entries[i].GetARF() == ARF)
            {
                if (!modification[i].IsNRAModified())
                    modification[i].SetNRA(false);
            }
    }

    void ShadowRegisterAliasTable::ResetInput()
    {
        for (int i = 0; i < rat_size; i++)
            modification[i].Reset();
    }

    void ShadowRegisterAliasTable::Eval()
    {
        //
        for (int i = 0; i < rat_size; i++)
        {
            if (modification[i].IsModified())
                modification[i].Apply(entries[i]);
        }

        //
        ResetInput();
    }
}
