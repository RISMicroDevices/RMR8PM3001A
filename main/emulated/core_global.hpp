#pragma once
//
// Mixed emulation for Global Core Resources
//
//

#include <bitset>

#include "base.hpp"


#define EMULATED_GC_COUNT                           8
#define EMULATED_ARF_SIZE                           32
#define EMULATED_PRF_SIZE                           96

#define EMULATED_SCOREBOARD_GC_COUNT                EMULATED_GC_COUNT

#define EMULATED_RAT_SIZE                           EMULATED_PRF_SIZE
#define EMULATED_RAT_GC_COUNT                       EMULATED_GC_COUNT 


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

    class GlobalCheckpointTable : public MEMU::Emulated
    {
    private:
        bitset<EMULATED_GC_COUNT>   gc_valid;
        int                         gc_next;

        int                         gc_eval_state;
        int                         gc_eval_index;

    public:
        GlobalCheckpointTable();
        GlobalCheckpointTable(const GlobalCheckpointTable& obj);
        ~GlobalCheckpointTable();

        constexpr int   GetSize() const;

        bool            IsValid(int index) const;
        
        int             GetNext() const;
        int             ValidateNext();
        void            Invalidate(int index);
        void            ResetInput();

        virtual void    Eval() override;
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


// class MEMU::Core::GlobalCheckpointTable
namespace MEMU::Core {
    /*
    bitset<EMULATED_GC_COUNT>   gc_valid;
    int                         gc_next;

    int                         gc_eval_state;
    int                         gc_eval_index;
    */

#define __MEMU_CORE_GCT_EVAL_STATE_NOP                  0
#define __MEMU_CORE_GCT_EVAL_STATE_VALIDATE             1
#define __MEMU_CORE_GCT_EVAL_STATE_INVALIDATE           2

    GlobalCheckpointTable::GlobalCheckpointTable()
        : gc_next       (0)
        , gc_eval_state (__MEMU_CORE_GCT_EVAL_STATE_NOP)
        , gc_eval_index (-1)
    { }

    GlobalCheckpointTable::GlobalCheckpointTable(const GlobalCheckpointTable& obj)
        : gc_valid      (obj.gc_valid)
        , gc_next       (obj.gc_next)
        , gc_eval_state (obj.gc_eval_state)
        , gc_eval_index (obj.gc_eval_index)
    { }

    GlobalCheckpointTable::~GlobalCheckpointTable()
    { }

    constexpr int GlobalCheckpointTable::GetSize() const
    {
        return EMULATED_GC_COUNT;
    }

    inline bool GlobalCheckpointTable::IsValid(int index) const
    {
        return gc_valid[index];
    }

    inline int GlobalCheckpointTable::GetNext() const
    {
        return gc_next;
    }

    inline int GlobalCheckpointTable::ValidateNext()
    {
        gc_eval_state = __MEMU_CORE_GCT_EVAL_STATE_VALIDATE;
        gc_eval_index = gc_next;

        return gc_next;
    }

    inline void GlobalCheckpointTable::Invalidate(int index)
    {
        gc_eval_state = __MEMU_CORE_GCT_EVAL_STATE_INVALIDATE;
        gc_eval_index = index;
    }

    inline void GlobalCheckpointTable::ResetInput()
    {
        gc_eval_state = __MEMU_CORE_GCT_EVAL_STATE_NOP;
        gc_eval_index = -1;
    }

    void GlobalCheckpointTable::Eval()
    {
        //
        switch (gc_eval_state)
        {
            case __MEMU_CORE_GCT_EVAL_STATE_VALIDATE:
                gc_valid[gc_eval_index] = true;
                break;

            case __MEMU_CORE_GCT_EVAL_STATE_INVALIDATE:
                for (int i = gc_eval_index; i < GetSize(); i++)
                    gc_valid[i] = false;
                break;

            case __MEMU_CORE_GCT_EVAL_STATE_NOP:
            default:
                break;

        }

        //
        gc_next = -1;

        for (int i = 0; i < GetSize(); i++)
            if (!gc_valid[i])
            {
                gc_next = i;
                break;
            }

        //
        gc_eval_state = __MEMU_CORE_GCT_EVAL_STATE_NOP;
        gc_eval_index = -1;
    }
}

