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
            void        SetValue(PhysicalRegisterFile& prf, uint64_t val);

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

        EntryModification*      modification    /*[rat_size]*/; 
        int                     rollback;
        int                     snapshot;

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
        constexpr int           GetCheckpointCount() const;

        const Entry&            GetEntry(int index) const;
        void                    SetEntry(int index, const Entry& entry);

        const GlobalCheckpoint& GetCheckpoint(int index) const;
        void                    SetCheckpoint(int index, const GlobalCheckpoint& checkpoint);

        int                     GetAliasPRF(int arf);

        void                    Clear();
        bool                    IsFull() const;

        bool                    Touch(int FID, int ARF, int* PRF = 0);
        void                    Writeback(int PRF);
        void                    Commit(int PRF);
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
        : entries           (new Entry[rat_size]())
        , checkpoints       (new GlobalCheckpoint[rat_gc_count]())
        , modification      (new EntryModification[rat_size]())
        , rollback          (-1)
        , snapshot          (-1)
    { 
        for (int i = 0; i < rat_size; i++)
            entries[i].SetPRF(i);
    }    

    RegisterAliasTable::RegisterAliasTable(const RegisterAliasTable& obj)
        : entries           (new Entry[rat_size])
        , checkpoints       (new GlobalCheckpoint[rat_gc_count])
        , modification      (obj.modification)
        , rollback          (obj.rollback)
        , snapshot          (obj.snapshot)
    {
        memcpy(entries, obj.entries, rat_size * sizeof(Entry));
        memcpy(checkpoints, obj.checkpoints, rat_gc_count * sizeof(GlobalCheckpoint));
    }
    
    RegisterAliasTable::~RegisterAliasTable()
    {
        delete[] entries;
        delete[] checkpoints;
        delete[] modification;
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
        for (int i = 0; i < rat_size; i++)
            modification[i].Reset();

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
        for (int i = 0; i < rat_size; i++)
        {
            if (modification[i].IsModified())
                modification[i].Apply(entries[i]);
        }

        //
        ResetInput();
    }
}
