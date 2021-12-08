#pragma once
//
// Mixed emulation for Global Core Resources
//
//

#include "bitset"

#include "base.hpp"

#define EMULATED_GC_COUNT               8

using namespace std;

namespace MEMU::Core {
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

        virtual void    Eval() override;
    };
}


// MEMU::Core::GlobalCheckpointTable
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
        , gc_next       (gc_next)
        , gc_eval_state (__MEMU_CORE_GCT_EVAL_STATE_NOP)
        , gc_eval_index (-1)
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

