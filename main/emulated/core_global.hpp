#pragma once
//
// Mixed emulation for Global Core Resources
//
//

#include <bitset>

#include "base.hpp"
#include "common.hpp"
#include "core_globaldef.hpp"


using namespace std;


namespace MEMU::Core {

    template<class TPayload>
    class CompressingMemory : public MEMU::Emulated
    {
    private:
        class Entry {
        private:
            TPayload    payload;
            bool        valid;

        public:
            Entry();
            Entry(const Entry& obj);
            ~Entry();

            const TPayload&     GetPayload() const;
            TPayload&           GetPayload();
            bool                GetValid() const;

            void                SetPayload(const TPayload& payload);
            void                SetValid(bool valid);

            void                Clear();
        };

        class EntryModification {
        private:
            bool        modified;

            bool        modified_payload;
            bool        modified_valid;

            TPayload    payload;
            bool        valid;

        public:
            EntryModification();
            EntryModification(const EntryModification& obj);
            ~EntryModification();

            bool    IsModified() const;

            bool    IsPayloadModified() const;
            bool    IsValidModified() const;

            void    SetPayload(const TPayload& payload);
            void    SetValid(bool valid = true);

            void    Reset();

            void    Apply(Entry& entry) const;
        };

    private:
        const int           size;

        Entry*              entries;

        EntryModification*  modification;

    public:
        CompressingMemory(int size);
        CompressingMemory(const CompressingMemory<TPayload>& obj);
        ~CompressingMemory();

        int                 GetSize() const;
        bool                CheckBound(int address) const;

        int                 NextTopAddress() const;

        const TPayload&     GetPayload(int address) const;
        bool                GetValid(int address) const;

        void                SetPayload(int address, const TPayload& payload);
        void                SetValid(int address, bool valid = true);

        void                ResetInput();
        void                Clear();

        virtual void        Eval() override;

        void                operator=(const CompressingMemory<TPayload>& obj) = delete;
    };



    //
    class GlobalCheckpointTableEntry {
    private:
        fgr_t   FGR;
        bool    V;
        int     GCA;

    public:
        GlobalCheckpointTableEntry();
        GlobalCheckpointTableEntry(fgr_t FGR, bool V, int GCA);
        GlobalCheckpointTableEntry(const GlobalCheckpointTableEntry& obj);
        ~GlobalCheckpointTableEntry();

        fgr_t   GetFGR() const;
        bool    GetValid() const;
        int     GetGCA() const;

        void    SetFGR(fgr_t FGR);
        void    SetValid(bool V);
        void    SetGCA(int GCA);

        void    Clear();
    };

    //
    class GlobalCheckpointWALK
        : public MEMU::Common::FIFO<fgr_t>
    {
    private:
        static constexpr int    size = EMULATED_GC_COUNT;

    public:
        GlobalCheckpointWALK();
        GlobalCheckpointWALK(const GlobalCheckpointWALK& obj);
        ~GlobalCheckpointWALK();
    };

    //
    class GlobalCheckpointTable 
        : public MEMU::Common::FIFO<GlobalCheckpointTableEntry>
    {
    private:
        static constexpr int    size = EMULATED_GC_COUNT;

    public:
        GlobalCheckpointTable();
        GlobalCheckpointTable(const GlobalCheckpointTable& obj);
        ~GlobalCheckpointTable();

        bool            PushFGR(fgr_t FGR, int GCA);
        bool            PopFGR();

        bool            RestoreFGR(fgr_t FGR, int* GCA, GlobalCheckpointWALK* WALK = nullptr);
    };
}



// class MEMU::Core::CompressingMemory::Entry
namespace MEMU::Core {
    /*
    TPayload    payload;
    bool        valid;
    */

    template<class TPayload>
    CompressingMemory<TPayload>::Entry::Entry()
        : payload   ()
        , valid     (false)
    { }

    template<class TPayload>
    CompressingMemory<TPayload>::Entry::Entry(const Entry& obj)
        : payload   (obj.payload)
        , valid     (obj.valid)
    { }

    template<class TPayload>
    CompressingMemory<TPayload>::Entry::~Entry()
    { }

    template<class TPayload>
    inline const TPayload& CompressingMemory<TPayload>::Entry::GetPayload() const
    {
        return payload;
    }

    template<class TPayload>
    inline TPayload& CompressingMemory<TPayload>::Entry::GetPayload()
    {
        return payload;
    }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::Entry::GetValid() const
    {
        return valid;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::Entry::SetPayload(const TPayload& payload)
    {
        this->payload = payload;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::Entry::SetValid(bool valid)
    {
        this->valid = valid;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::Entry::Clear()
    {
        payload = TPayload();
        valid   = false;
    }
}


// class MEMU::Core::CompressingMemory::EntryModification
namespace MEMU::Core {
    /*
    bool        modified;

    bool        modified_payload;
    bool        modified_valid;

    TPayload    payload;
    bool        valid;
    */

    template<class TPayload>
    CompressingMemory<TPayload>::EntryModification::EntryModification()
        : modified          (false)
        , modified_payload  (false)
        , modified_valid    (false)
        , payload           ()
        , valid             (false)
    { }

    template<class TPayload>
    CompressingMemory<TPayload>::EntryModification::EntryModification(const EntryModification& obj)
        : modified          (obj.modified)
        , modified_payload  (obj.modified_payload)
        , modified_valid    (obj.modified_valid)
        , payload           (obj.payload)
        , valid             (obj.valid)
    { }

    template<class TPayload>
    CompressingMemory<TPayload>::EntryModification::~EntryModification()
    { }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::EntryModification::IsModified() const
    {
        return modified;
    }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::EntryModification::IsPayloadModified() const
    {
        return modified_payload;
    }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::EntryModification::IsValidModified() const
    {
        return modified_valid;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::EntryModification::SetPayload(const TPayload& payload)
    {
        modified         = true;
        modified_payload = true;

        this->payload = payload;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::EntryModification::SetValid(bool valid)
    {
        modified       = true;
        modified_valid = true;

        this->valid = valid;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::EntryModification::Reset()
    {
        modified = false;

        modified_payload = false;
        modified_valid   = false;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::EntryModification::Apply(Entry& entry) const
    {
        if (IsPayloadModified())
            entry.SetPayload(payload);

        if (IsValidModified())
            entry.SetValid(valid);
    }
}


// class MEMU::Core::CompressingMemory
namespace MEMU::Core {
    /*
    const int       size;

    TPayload*       memory;
    bool*           valid;

    EntryModification*   modification;
    */

    template<class TPayload>
    CompressingMemory<TPayload>::CompressingMemory(int size)
        : size          (size)
        , entries       (new Entry[size]())
        , modification  (new EntryModification[size]())
    { }

    template<class TPayload>
    CompressingMemory<TPayload>::CompressingMemory(const CompressingMemory<TPayload>& obj)
        : size          (obj.size)
        , entries       (new Entry[size]())
        , modification  (new EntryModification[size]())
    {
        for (int i = 0; i < size; i++)
        {
            entries[i]      = obj.entries[i];
            modification[i] = obj.modification[i];
        }
    }

    template<class TPayload>
    CompressingMemory<TPayload>::~CompressingMemory()
    {
        delete[] entries;
        delete[] modification;
    }

    template<class TPayload>
    inline int CompressingMemory<TPayload>::GetSize() const
    {
        return size;
    }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::CheckBound(int address) const
    {
        return address >= 0 && address < GetSize();
    }

    template<class TPayload>
    int CompressingMemory<TPayload>::NextTopAddress() const
    {
        int addr = -1;

        for (int i = 0; i < GetSize(); i++)
        {
            if (entries[i].GetValid())
                break;

            addr = i;
        }

        return addr;
    }

    template<class TPayload>
    inline const TPayload& CompressingMemory<TPayload>::GetPayload(int address) const
    {
        return entries[address].GetPayload();
    }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::GetValid(int address) const
    {
        return entries[address].GetValid();
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::SetPayload(int address, const TPayload& payload)
    {
        modification[address].SetPayload(payload);
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::SetValid(int address, bool valid)
    {
        modification[address].SetValid(valid);
    }

    template<class TPayload>
    void CompressingMemory<TPayload>::ResetInput()
    {
        for (int i = 0; i < GetSize(); i++)
            modification[i].Reset();
    }

    template<class TPayload>
    void CompressingMemory<TPayload>::Clear()
    {
        for (int i = 0; i < GetSize(); i++)
            entries[i].Clear();
    }

    template<class TPayload>
    void CompressingMemory<TPayload>::Eval()
    {
        bool* comp_carrier = new bool[GetSize() + 1]();

        //
        for (int i = GetSize() - 1; i >= 0; i--)
        {
            //
            comp_carrier[i] = comp_carrier[i + 1] || !entries[i].GetValid();

            //
            bool comp_unit = comp_carrier[i + 1];

            if (comp_unit)
            {
                // NOTICE: Overlap would occur asserting 'valid' of non-valid entries 
                //         between valid entries.
                //         The overlap behaviour is UNDEFINED.
                //         

                if (entries[i].GetValid())
                    entries[i + 1] = entries[i];

                if (modification[i].IsModified())
                    modification[i].Apply(entries[i + 1]);

                entries[i].SetValid(false);
            }
            else if (modification[i].IsModified())
                modification[i].Apply(entries[i]);
        }

        //
        ResetInput();

        delete[] comp_carrier;
    }
}



// class MEMU::Core::GlobalCheckpointTableEntry
namespace MEMU::Core {
    /*
    fgr_t   FGR;
    bool    V;
    int     GCA;
    */

    GlobalCheckpointTableEntry::GlobalCheckpointTableEntry()
        : FGR   (0)
        , V     (false)
        , GCA   (0)
    { }

    GlobalCheckpointTableEntry::GlobalCheckpointTableEntry(fgr_t FGR, bool V, int GCA)
        : FGR   (FGR)
        , V     (V)
        , GCA   (GCA)
    { }

    GlobalCheckpointTableEntry::GlobalCheckpointTableEntry(const GlobalCheckpointTableEntry& obj)
        : FGR   (obj.FGR)
        , V     (obj.V)
        , GCA   (obj.GCA)
    { }

    GlobalCheckpointTableEntry::~GlobalCheckpointTableEntry()
    { }

    inline fgr_t GlobalCheckpointTableEntry::GetFGR() const
    {
        return FGR;
    }

    inline bool GlobalCheckpointTableEntry::GetValid() const
    {
        return V;
    }

    inline int GlobalCheckpointTableEntry::GetGCA() const
    {
        return GCA;
    }

    inline void GlobalCheckpointTableEntry::SetFGR(fgr_t FGR)
    {
        this->FGR = FGR;
    }

    inline void GlobalCheckpointTableEntry::SetValid(bool V)
    {
        this->V = V;
    }

    inline void GlobalCheckpointTableEntry::SetGCA(int GCA)
    {
        this->GCA = GCA;
    }

    inline void GlobalCheckpointTableEntry::Clear()
    {
        this->FGR = 0;
        this->V   = false;
        this->GCA = 0;
    }
}


// class MEMU::Core::GlobalCheckpointWALK
namespace MEMU::Core {
    GlobalCheckpointWALK::GlobalCheckpointWALK()
        : FIFO  (size)
    { }

    GlobalCheckpointWALK::GlobalCheckpointWALK(const GlobalCheckpointWALK& obj)
        : FIFO  (obj)
    { }

    GlobalCheckpointWALK::~GlobalCheckpointWALK()
    { }
}


// class MEMU::Core::GlobalCheckpointTable
namespace MEMU::Core {
    GlobalCheckpointTable::GlobalCheckpointTable()
        : FIFO      (size)
    { }

    GlobalCheckpointTable::GlobalCheckpointTable(const GlobalCheckpointTable& obj)
        : FIFO      (obj)
    { }

    GlobalCheckpointTable::~GlobalCheckpointTable()
    { }

    inline bool GlobalCheckpointTable::PushFGR(fgr_t FGR, int GCA)
    {
        return PushPayload(GlobalCheckpointTableEntry(FGR, true, GCA));
    }

    inline bool GlobalCheckpointTable::PopFGR()
    {
        return PopPayload();
    }

    bool GlobalCheckpointTable::RestoreFGR(fgr_t FGR, int* GCA, GlobalCheckpointWALK* WALK)
    {
        // CAM Query of FGR entry
        int restore = -1;
        for (int i = 0; i < GetSize(); i++)
        {
            const GlobalCheckpointTableEntry& entry = GetPayload(i);

            if (entry.GetValid() && entry.GetFGR() == FGR)
            {
                *GCA = entry.GetGCA();
                restore = i;

                break;
            }
        }

        if (restore == -1)
            return false;

        int restore_ptr = restore + 1;

        // Manipulate V bits (left-handed pseudo-loop combinational)
        int j = restore_ptr;
        do 
        {
            GlobalCheckpointTableEntry entry = GetPayload(j);

            if (!entry.GetValid())
                break;

            entry.SetValid(false);

            SetPayload(j, entry);

            if (--j < 0)
                j = size;
        } 
        while (j != restore_ptr);

        // Write into WALK FIFO
        for (int i = 0; i < GetSize(); i++)
            WALK->SetPayload(i, GetPayload(i).GetFGR());

        // Manipulate Wptr&Rptr of WALK FIFO
        WALK->SetReadPointer(GetReadPointer());
        WALK->SetReadPointerB(GetReadPointerB());

        WALK->SetWritePointer(restore_ptr);
        WALK->SetWritePointerB(restore_ptr > GetWritePointer() ? !GetWritePointerB() : GetWritePointerB());

        return true;
    }
}

