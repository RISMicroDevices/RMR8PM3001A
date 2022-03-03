#pragma once
//
// Mixed emulation for Global Core Resources
//
//

#include "bitset"

#include "base.hpp"

#define EMULATED_GC_COUNT               8

#define EMULATED_ARF_SIZE               32

using namespace std;

namespace MEMU::Core {

    template<class TPayload>
    class CompressingMemory : public MEMU::Emulated
    {
    private:
        class Modification {
        private:
            bool        modified;

            bool        modified_payload;
            bool        modified_valid;

            TPayload    payload;
            bool        valid;

        public:
            Modification();
            Modification(const Modification& obj);
            ~Modification();

            bool    IsModified() const;

            bool    IsPayloadModified() const;
            bool    IsValidModified() const;

            void    SetPayload(const TPayload& payload);
            void    SetValid(bool valid = true);

            void    Reset();
        };

    private:
        const int       size;

        TPayload*       memory;
        bool*           valid;

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


// class MEMU::Core::CompressingMemory::Modification
namespace MEMU::Core {
    /*
    bool        modified;

    bool        modified_payload;
    bool        modified_valid;

    TPayload    payload;
    bool        valid;
    */

    template<class TPayload>
    CompressingMemory<TPayload>::Modification::Modification()
        : modified          (false)
        , modified_payload  (false)
        , modified_valid    (false)
        , payload           ()
        , valid             (false)
    { }

    template<class TPayload>
    CompressingMemory<TPayload>::Modification::Modification(const Modification& obj)
        : modified          (obj.modified)
        , modified_payload  (obj.modified_payload)
        , modified_valid    (obj.modified_valid)
        , payload           (obj.payload)
        , valid             (obj.valid)
    { }

    template<class TPayload>
    CompressingMemory<TPayload>::Modification::~Modification()
    { }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::Modification::IsModified() const
    {
        return modified;
    }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::Modification::IsPayloadModified() const
    {
        return modified_payload;
    }

    template<class TPayload>
    inline bool CompressingMemory<TPayload>::Modification::IsValidModified() const
    {
        return modified_valid;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::Modification::SetPayload(const TPayload& payload)
    {
        modified         = true;
        modified_payload = true;

        this->payload = payload;
    }

    template<class TPayload>
    inline void CompressingMemory<TPayload>::Modification::SetValid(bool valid)
    {
        modified       = true;
        modified_valid = true;

        this->valid = valid;
    }

    template<class TPayload>
    void CompressingMemory<TPayload>::Modification::Reset()
    {
        modified = false;

        modified_payload = false;
        modified_valid   = false;
    }
}


// class MEMU::Core::CompressingMemory
namespace MEMU::Core {

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

