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


#define EMULATED_SCOREBOARD_GC_COUNT                EMULATED_GC_COUNT

#define EMULATED_PRF_SIZE                           64

#define EMULATED_RAT_SIZE                           EMULATED_PRF_SIZE

#define EMULATED_RAT_GC_COUNT                       EMULATED_GC_COUNT      


using namespace std;

namespace MEMU::Core::Issue {

    class Scoreboard final : public MEMU::Emulated
    {
    public:
        class Entry {
        private:
            bool    busy;
            bool    forward;

        public:
            Entry();
            Entry(const Entry& obj);
            ~Entry();

            bool    IsBusy() const;
            bool    IsForward() const;

            void    SetBusy(bool busy);
            void    SetForward(bool forward);

            void    Clear();
        };

        class EntryModification {
        private:
            bool    modified;

            bool    modified_busy;
            bool    modified_forward;
            bool    modified_land;
            bool    modified_takeoff;

            bool    busy;
            bool    forward;

        public:
            EntryModification();
            EntryModification(const EntryModification& obj);
            ~EntryModification();

            bool    IsModified() const;

            bool    IsBusyModified() const;
            bool    IsForwardModified() const;
            bool    IsLandModified() const;
            bool    IsTakeOffModified() const;

            void    SetBusy(bool busy);
            void    SetForward(bool forward);
            void    Land();
            void    TakeOff();

            void    Reset();

            void    Apply(Entry& entry) const;
        };

        class CheckpointEntry {
        private:
            bool    busy;

        public:
            CheckpointEntry();
            CheckpointEntry(const CheckpointEntry& obj);
            ~CheckpointEntry();

            bool    GetBusy() const;
            void    SetBusy(bool busy);

            void    Clear();

            void    Allocate(const Entry& entry);
            void    Restore(Entry& entry) const;
        };

        class Checkpoint {
        private:
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

            void                    Land(int index);

            void                    Clear();

            void                    Allocate(const Entry* entries);
            void                    Restore(Entry* entries) const;

            void                    operator=(const Checkpoint& obj);
        };

    private:
        constexpr static int    prf_size                = EMULATED_PRF_SIZE;

        constexpr static int    gc_count                = EMULATED_SCOREBOARD_GC_COUNT;

        Entry*              entries;        // [prf_size]

        EntryModification*  modification;   // [prf_size]

        Checkpoint*         checkpoints;    // [gc_count]

        int                 checkpoint_allocate;

        int                 checkpoint_restore;

    public:
        Scoreboard();
        Scoreboard(const Scoreboard& obj);
        ~Scoreboard();

        int                 GetSize() const;
        bool                CheckBound(int index) const;

        const Entry&        GetEntry(int index) const;
        Entry&              GetEntry(int index);
        void                SetEntry(int index, const Entry& entry);

        bool                IsBusy(int index) const;
        bool                IsForward(int index) const;

        void                Land(int index);
        void                TakeOff(int index);
        void                SetBusy(int index, bool busy);
        void                SetForward(int index, bool forward);

        int                 GetCheckpointCount() const;
        bool                CheckCheckpointBound(int index) const;

        const Checkpoint&   GetCheckpoint(int index) const;
        Checkpoint&         GetCheckpoint(int index);
        void                SetCheckpoint(int index, const Checkpoint& checkpoint);

        void                AllocateCheckpoint(int index);
        void                RestoreCheckpoint(int index);

        void                ResetInput();
        void                Clear();

        virtual void        Eval() override;
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
            bool    NRA;   // Not reallocate-able Flag
            int     PRF;   // PRF (Physical Register File) address
            int     ARF;   // ARF (Architectural Register File) mapping address
            bool    V;     // Mapping entry Valid

        public:
            Entry();
            Entry(const Entry& obj);
            ~Entry();

            void        Clear();

            bool        GetNRA() const;
            int         GetPRF() const;
            int         GetARF() const;
            bool        GetValid() const;

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

            bool    modified_NRA;
            bool    modified_ARF;
            bool    modified_V;

            bool    NRA;
            int     ARF;
            bool    V;

        public:
            EntryModification();
            EntryModification(const EntryModification& obj);
            ~EntryModification();

            bool    IsModified() const;

            bool    IsNRAModified() const;
            bool    IsARFModified() const;
            bool    IsValidModified() const;

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
            constexpr static int    gc_size      = EMULATED_PRF_SIZE;

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

            void                    operator=(const Checkpoint& obj);
        };

    private:
        constexpr static int    rat_size     = EMULATED_RAT_SIZE;

        constexpr static int    gc_count     = EMULATED_RAT_GC_COUNT;

        Entry*                  entries      /*[rat_size]*/;

        EntryModification*      modification /*[rat_size]*/; 

        Checkpoint*             checkpoints  /*[gc_count]*/;

        const Scoreboard*       scoreboard;

        int                     checkpoint_allocate;
        int                     checkpoint_restore;


    private:
        int                 GetNextEntry() const;
        void                Invalidate(int ARF);
        void                InvalidateAndRelease(int ARF);
        void                LandAndRelease(int ARF);
        bool                Touch(bool FV, int FID, int ARF, int* PRF);

    public:
        RegisterAliasTable(const Scoreboard* scoreboard);
        RegisterAliasTable(const RegisterAliasTable& obj);
        ~RegisterAliasTable();

        constexpr int           GetSize() const;

        const Entry&            GetEntry(int index) const;
        Entry&                  GetEntry(int index);
        void                    SetEntry(int index, const Entry& entry);

        const Scoreboard*       GetScoreboard() const;
        void                    SetScoreboard(const Scoreboard* scoreboard);

        int                     GetAliasPRF(int arf) const;

        void                    Clear();
        bool                    IsFull() const;

        bool                    Touch(int FID, int ARF, int* PRF = 0);
        void                    Writeback(int PRF);
        void                    Commit(int PRF);
        bool                    TouchAndWriteback(int FID, int ARF, int* PRF = 0);
        bool                    TouchAndCommit(int FID, int ARF, int* PRF = 0);

        void                    SyncNRA(const ShadowRegisterAliasTable& sRAT);

        void                    AllocateCheckpoint(int index);
        void                    RestoreCheckpoint(int index);

        void                    ResetInput();

        virtual void            Eval() override;
    };
}



// class MEMU::Core::Issue::Scoreboard::Entry
namespace MEMU::Core::Issue {
    /*
    bool    busy;
    bool    forward;
    */

    Scoreboard::Entry::Entry()
        : busy      (false)
        , forward   (false)
    { }

    Scoreboard::Entry::Entry(const Entry& obj)
        : busy      (obj.busy)
        , forward   (obj.forward)
    { }

    Scoreboard::Entry::~Entry()
    { }

    inline bool Scoreboard::Entry::IsBusy() const
    {
        return busy;
    }

    inline bool Scoreboard::Entry::IsForward() const
    {
        return forward;
    }

    inline void Scoreboard::Entry::SetBusy(bool busy)
    {
        this->busy = busy;
    }

    inline void Scoreboard::Entry::SetForward(bool forward)
    {
        this->forward = forward;
    }

    inline void Scoreboard::Entry::Clear()
    {
        busy    = false;
        forward = false;
    }
}


// class MEMU::Core::Issue::Scoreboard::EntryModification
namespace MEMU::Core::Issue {
    /*
    bool    modified;

    bool    modified_busy;
    bool    modified_forward
    bool    modified_land;
    bool    modified_takeoff;

    bool    busy;
    bool    forward;
    */

    Scoreboard::EntryModification::EntryModification()
        : modified          (false)
        , modified_busy     (false)
        , modified_forward  (false)
        , busy              (false)
        , forward           (false)
    { }

    Scoreboard::EntryModification::EntryModification(const EntryModification& obj)
        : modified          (obj.modified)
        , modified_busy     (obj.modified_busy)
        , modified_forward  (obj.modified_forward)
        , busy              (obj.busy)
        , forward           (obj.forward)
    { }

    Scoreboard::EntryModification::~EntryModification()
    { }

    inline bool Scoreboard::EntryModification::IsModified() const
    {
        return modified;
    }

    inline bool Scoreboard::EntryModification::IsBusyModified() const
    {
        return modified_busy;
    }

    inline bool Scoreboard::EntryModification::IsForwardModified() const
    {
        return modified_forward;
    }

    inline bool Scoreboard::EntryModification::IsLandModified() const
    {
        return modified_land;
    }

    inline bool Scoreboard::EntryModification::IsTakeOffModified() const
    {
        return modified_takeoff;
    }

    inline void Scoreboard::EntryModification::SetBusy(bool busy)
    {
        modified      = true;
        modified_busy = true;

        this->busy = busy;
    }

    inline void Scoreboard::EntryModification::SetForward(bool forward)
    {
        modified         = true;
        modified_forward = true;

        this->forward = forward;
    }

    inline void Scoreboard::EntryModification::Land()
    {
        modified_land = true;
    }

    inline void Scoreboard::EntryModification::TakeOff()
    {
        modified_takeoff = true;
    }

    inline void Scoreboard::EntryModification::Reset()
    {
        modified = false;

        modified_busy    = false;
        modified_forward = false;
        modified_land    = false;
        modified_takeoff = false;
    }

    void Scoreboard::EntryModification::Apply(Entry& entry) const
    {
        if (IsBusyModified())
            entry.SetBusy(busy);

        if (!IsForwardModified())
            entry.SetForward(forward);
    }
}


// class MEMU::Core::Issue::Scoreboard::CheckpointEntry
namespace MEMU::Core::Issue {
    /*
    bool    busy;
    */

    Scoreboard::CheckpointEntry::CheckpointEntry()
        : busy  (false)
    { }

    Scoreboard::CheckpointEntry::CheckpointEntry(const CheckpointEntry& obj)
        : busy  (obj.busy)
    { }

    Scoreboard::CheckpointEntry::~CheckpointEntry()
    { }

    inline bool Scoreboard::CheckpointEntry::GetBusy() const
    {
        return busy;
    }

    inline void Scoreboard::CheckpointEntry::SetBusy(bool busy)
    {
        this->busy = busy;
    }

    inline void Scoreboard::CheckpointEntry::Clear()
    {
        busy = false;
    }

    inline void Scoreboard::CheckpointEntry::Allocate(const Entry& entry)
    {
        busy = entry.IsBusy();
    }

    inline void Scoreboard::CheckpointEntry::Restore(Entry& entry) const
    {
        entry.SetBusy(busy);
    }
}


// class MEMU::Core::Issue::Scoreboard::Checkpoint
namespace MEMU::Core::Issue {
    /*
    CheckpointEntry*    entries;
    */

    Scoreboard::Checkpoint::Checkpoint()
        : entries   (new CheckpointEntry[prf_size]())
    { }

    Scoreboard::Checkpoint::Checkpoint(const Checkpoint& obj)
        : entries   (new CheckpointEntry[prf_size])
    { 
        for (int i = 0; i < prf_size; i++)
            entries[i] = obj.entries[i];
    }

    Scoreboard::Checkpoint::~Checkpoint()
    {
        delete[] entries;
    }

    inline int Scoreboard::Checkpoint::GetSize() const
    {
        return prf_size;
    }

    inline bool Scoreboard::Checkpoint::CheckBound(int index) const
    {
        return index >= 0 && index < GetSize();
    }

    inline const Scoreboard::CheckpointEntry& Scoreboard::Checkpoint::GetEntry(int index) const
    {
        return entries[index];
    }

    inline Scoreboard::CheckpointEntry& Scoreboard::Checkpoint::GetEntry(int index)
    {
        return entries[index];
    }

    inline void Scoreboard::Checkpoint::SetEntry(int index, const CheckpointEntry& entry)
    {
        entries[index] = entry;
    }

    inline void Scoreboard::Checkpoint::Land(int index)
    {
        entries[index].SetBusy(false);
    }

    void Scoreboard::Checkpoint::Clear()
    {
        for (int i = 0; i < GetSize(); i++)
            entries[i].Clear();
    }

    void Scoreboard::Checkpoint::Allocate(const Entry* entries)
    {
        for (int i = 0; i < GetSize(); i++)
            this->entries[i].Allocate(entries[i]);
    }

    void Scoreboard::Checkpoint::Restore(Entry* entries) const
    {
        for (int i = 0; i < GetSize(); i++)
            this->entries[i].Restore(entries[i]);
    }

    void Scoreboard::Checkpoint::operator=(const Checkpoint& obj)
    {
        for (int i = 0; i < GetSize() && i < obj.GetSize(); i++)
            this->entries[i] = obj.entries[i];
    }
}


// class MEMU::Core::Issue::Scoreboard
namespace MEMU::Core::Issue {
    /*
    constexpr static int    prf_size                = EMULATED_PRF_SIZE;

    constexpr static int    gc_count                = EMULATED_SCOREBOARD_GC_COUNT;

    Entry*              entries;

    EntryModification*  modification;
    */

    Scoreboard::Scoreboard()
        : entries               (new Entry[prf_size]())
        , modification          (new EntryModification[prf_size]())
        , checkpoints           (new Checkpoint[gc_count]())
        , checkpoint_allocate   (-1)
        , checkpoint_restore    (-1)
    { }

    Scoreboard::Scoreboard(const Scoreboard& obj)
        : entries               (new Entry[prf_size])
        , modification          (new EntryModification[prf_size])
        , checkpoints           (new Checkpoint[gc_count])
        , checkpoint_allocate   (-1)
        , checkpoint_restore    (-1)
    { 
        for (int i = 0; i < prf_size; i++)
        {
            entries[i]      = obj.entries[i];
            modification[i] = obj.modification[i];
            checkpoints[i]  = obj.checkpoints[i];
        }
    }

    Scoreboard::~Scoreboard()
    { 
        delete[] entries;
        delete[] modification;
        delete[] checkpoints;
    }

    inline int Scoreboard::GetSize() const
    {
        return prf_size;
    }

    inline bool Scoreboard::CheckBound(int index) const
    {
        return index >= 0 && index < GetSize();
    }

    inline const Scoreboard::Entry& Scoreboard::GetEntry(int index) const
    {
        return entries[index];
    }

    inline Scoreboard::Entry& Scoreboard::GetEntry(int index)
    {
        return entries[index];
    }

    inline void Scoreboard::SetEntry(int index, const Entry& entry)
    {
        entries[index] = entry;
    }

    inline bool Scoreboard::IsBusy(int index) const
    {
        return entries[index].IsBusy();
    }

    inline bool Scoreboard::IsForward(int index) const
    {
        return entries[index].IsForward();
    }

    inline void Scoreboard::Land(int index)
    {
        modification[index].SetBusy(false);
        modification[index].Land();
    }

    inline void Scoreboard::TakeOff(int index)
    {
        modification[index].SetBusy(true);
        modification[index].TakeOff();
    }

    inline void Scoreboard::SetBusy(int index, bool busy)
    {
        modification[index].SetBusy(busy);
    }

    inline void Scoreboard::SetForward(int index, bool forward)
    {
        modification[index].SetForward(forward);
    }

    inline int Scoreboard::GetCheckpointCount() const
    {
        return gc_count;
    }

    inline bool Scoreboard::CheckCheckpointBound(int index) const
    {
        return index >= 0 && index < GetCheckpointCount();
    }

    inline const Scoreboard::Checkpoint& Scoreboard::GetCheckpoint(int index) const
    {
        return checkpoints[index];
    }

    inline Scoreboard::Checkpoint& Scoreboard::GetCheckpoint(int index)
    {
        return checkpoints[index];
    }

    inline void Scoreboard::SetCheckpoint(int index, const Checkpoint& checkpoint)
    {
        checkpoints[index] = checkpoint;
    }

    void Scoreboard::AllocateCheckpoint(int index)
    {
        checkpoint_allocate = index;
    }

    void Scoreboard::RestoreCheckpoint(int index)
    {
        checkpoint_restore = index;
    }

    void Scoreboard::ResetInput()
    {
        for (int i = 0; i < GetSize(); i++)
            modification[i].Reset();

        checkpoint_allocate = -1;
        checkpoint_restore  = -1;
    }

    void Scoreboard::Clear()
    {
        for (int i = 0; i < GetSize(); i++)
            entries[i].Clear();
    }

    void Scoreboard::Eval()
    {
        // checkpoint allocate
        if (checkpoint_allocate != -1)
            checkpoints[checkpoint_allocate].Allocate(entries);

        for (int i = 0; i < GetSize(); i++)
        {
            // Clear forward flag at every clock cycle
            // *NOTE: Data in forwarding network only holds for a cycle
            entries[i].SetForward(false);

            //
            if (modification[i].IsModified())
            {
                modification[i].Apply(entries[i]);

                // Checkpoint write-through on PRF land
                if (modification[i].IsLandModified())
                    for (int j = 0; j < GetCheckpointCount(); j++)
                        checkpoints[j].Land(i);
            }
        }

        // checkpoint restore
        if (checkpoint_restore != -1)
            checkpoints[checkpoint_restore].Restore(entries);

        ResetInput();
    }
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
    bool    NRA;
    int     PRF;
    int     ARF;
    bool    V;
    */

    RegisterAliasTable::Entry::Entry()
        : NRA   (false)
        , PRF   (0)
        , ARF   (0)
        , V     (false)
    { }

    RegisterAliasTable::Entry::Entry(const RegisterAliasTable::Entry& obj)
        : NRA   (obj.NRA)
        , PRF   (obj.PRF)
        , ARF   (obj.ARF)
        , V     (obj.V)
    { }

    RegisterAliasTable::Entry::~Entry()
    { }

    void RegisterAliasTable::Entry::Clear()
    {
        NRA = false;
        PRF = 0;
        ARF = 0;
        V   = false;
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

    bool    modified_NRA;
    bool    modified_PRF;
    bool    modified_V;

    bool    NRA;
    int     PRF;
    bool    V;
    */

    RegisterAliasTable::EntryModification::EntryModification()
        : modified      (false)
        , modified_NRA  (false)
        , modified_ARF  (false)
        , modified_V    (false)
        , NRA           (false)
        , ARF           (0)
        , V             (false)
    { }

    RegisterAliasTable::EntryModification::EntryModification(const EntryModification& obj)
        : modified      (obj.modified)
        , modified_NRA  (obj.modified_NRA)
        , modified_ARF  (obj.modified_ARF)
        , modified_V    (obj.modified_V)
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

        modified_NRA = false;
        modified_ARF = false;
        modified_V   = false;
    }

    void RegisterAliasTable::EntryModification::Apply(Entry& dst) const
    {
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
    constexpr static int    gc_size;

    CheckpointEntry*    entries;
    */

    RegisterAliasTable::Checkpoint::Checkpoint()
        : entries   (new CheckpointEntry[gc_size]())
    { }

    RegisterAliasTable::Checkpoint::Checkpoint(const Checkpoint& obj)
        : entries   (new CheckpointEntry[gc_size])
    {
        for (int i = 0; i < gc_size; i++)
            entries[i] = obj.entries[i];
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
        return index >= 0 && index < GetSize();
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
        for (int i = 0; i < GetSize(); i++)
            this->entries[i].Allocate(entries[i]);
    }

    void RegisterAliasTable::Checkpoint::Restore(Entry* entries) const
    {
        for (int i = 0; i < GetSize(); i++)
            this->entries[i].Restore(entries[i]);
    }

    void RegisterAliasTable::Checkpoint::operator=(const Checkpoint& obj)
    {
        for (int i = 0; i < GetSize() && i < obj.GetSize(); i++)
            this->entries[i] = obj.entries[i];
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

    const Scoreboard*       scoreboard;

    int                     checkpoint_allocate;
    int                     checkpoint_restore;
    */

    RegisterAliasTable::RegisterAliasTable(const Scoreboard* scoreboard)
        : entries               (new Entry[rat_size]())
        , modification          (new EntryModification[rat_size]())
        , checkpoints           (new Checkpoint[gc_count]())
        , scoreboard            (scoreboard)
        , checkpoint_allocate   (-1)
        , checkpoint_restore    (-1)
    { 
        for (int i = 0; i < rat_size; i++)
            entries[i].SetPRF(i);
    }    

    RegisterAliasTable::RegisterAliasTable(const RegisterAliasTable& obj)
        : entries               (new Entry[rat_size])
        , modification          (new EntryModification[rat_size])
        , checkpoints           (new Checkpoint[gc_count])
        , scoreboard            (obj.scoreboard)
        , checkpoint_allocate   (obj.checkpoint_allocate)
        , checkpoint_restore    (obj.checkpoint_restore)
    {
        for (int i = 0; i < rat_size; i++)
        {
            entries[i]      = obj.entries[i];
            modification[i] = obj.modification[i];
        }

        for (int i = 0; i < gc_count; i++)
        {
            checkpoints[i] = obj.checkpoints[i];
        }
    }
    
    RegisterAliasTable::~RegisterAliasTable()
    {
        delete[] entries;
        delete[] modification;
        delete[] checkpoints;
    }

    constexpr int RegisterAliasTable::GetSize() const
    {
        return rat_size;
    }

    inline const RegisterAliasTable::Entry& RegisterAliasTable::GetEntry(int index) const
    {
        return entries[index];
    }

    inline RegisterAliasTable::Entry& RegisterAliasTable::GetEntry(int index)
    {
        return entries[index];
    }

    inline void RegisterAliasTable::SetEntry(int index, const Entry& entry)
    {
        entries[index] = entry;
    }

    inline const Scoreboard* RegisterAliasTable::GetScoreboard() const
    {
        return scoreboard;
    }

    inline void RegisterAliasTable::SetScoreboard(const Scoreboard* scoreboard)
    {
        this->scoreboard = scoreboard;
    }

    int RegisterAliasTable::GetAliasPRF(int arf) const
    {
        if (!arf)
            return -1;

        // PRF Query CAM
        for (int i = 0; i < GetSize(); i++)
            if (entries[i].GetValid() && (entries[i].GetARF() == arf))
                return entries[i].GetPRF();

        return -1;
    }

    void RegisterAliasTable::Clear()
    {
        for (int i = 0; i < GetSize(); i++)
        {
            entries[i].Clear();
            entries[i].SetPRF(i);
        }
    }

    bool RegisterAliasTable::IsFull() const
    {
        for (int i = 0; i < GetSize(); i++)
            if (!entries[i].GetNRA())
                return false;

        return true;
    }

    int RegisterAliasTable::GetNextEntry() const
    {
        // Selection tree.
        for (int i = 0; i < GetSize(); i++)
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
        for (int i = 0; i < GetSize(); i++)
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
        
        // Release: Set NRA of all non-FV same-ARF entries to false
        for (int i = 0; i < GetSize(); i++)
            if (!scoreboard->IsBusy(i) && entries[i].GetARF() == ARF)
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

        for (int i = 0; i < GetSize(); i++)
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

        //
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

    void RegisterAliasTable::SyncNRA(const ShadowRegisterAliasTable& sRAT)
    {
        // HIGHEST PRIORITY write
        for (int i = 0; i < GetSize(); i++)
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
        for (int i = 0; i < GetSize(); i++)
            modification[i].Reset();

        checkpoint_allocate = -1;
        checkpoint_restore  = -1;
    }

    void RegisterAliasTable::Eval()
    {
        // Checkpoint allocate
        if (checkpoint_allocate != -1)
            checkpoints[checkpoint_allocate].Allocate(entries);

        // Write entries
        for (int i = 0; i < GetSize(); i++)
        {
            if (modification[i].IsModified())
                modification[i].Apply(entries[i]);
        }

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
        for (int i = 0; i < GetSize(); i++)
            if (entries[i].GetValid() && entries[i].GetARF() == arf)
                return entries[i].GetPRF();

        return -1;
    }

    void ShadowRegisterAliasTable::Clear()
    {
        for (int i = 0; i < GetSize(); i++)
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
        for (int i = 0; i < GetSize(); i++)
            if (entries[i].GetARF() == ARF)
            {
                if (!modification[i].IsNRAModified())
                    modification[i].SetNRA(false);
            }
    }

    void ShadowRegisterAliasTable::ResetInput()
    {
        for (int i = 0; i < GetSize(); i++)
            modification[i].Reset();
    }

    void ShadowRegisterAliasTable::Eval()
    {
        //
        for (int i = 0; i < GetSize(); i++)
        {
            if (modification[i].IsModified())
                modification[i].Apply(entries[i]);
        }

        //
        ResetInput();
    }
}
