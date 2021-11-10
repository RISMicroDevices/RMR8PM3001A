#pragma once
//
// Mixed emulation for RAT Freelist Checkpoint
//
//  @see 'rat_freelist_checkpoint.v'
//

#include <list>

#define EMULATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID            0x00
#define EMULATED_RAT_FREELIST_CHECKPOINT_STATE_VALID              0x01
#define EMULATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED          0x02

#define EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_NOP            -1
#define EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_POP            -2
#define EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_CLEAR          -3

using namespace std;

namespace MEMU::Core::Issue {

    class RATFreelistCheckpoint final
    {
    private:
        list<int>*  emulated_banks;
        int*        emulated_fgrs;
        int*        emulated_states;

        int*        emulated_next_banks;
        int*        emulated_next_fgrs;
        int*        emulated_next_states;

        const int   bank_count; 
        const int   bank_depth;

    public:
        RATFreelistCheckpoint(int bank_count, int bank_depth);
        RATFreelistCheckpoint(const RATFreelistCheckpoint& obj);
        ~RATFreelistCheckpoint();

        int     GetBankCount() const;
        int     GetBankDepth() const;

        int     PushAcquired(int fgr, int prf);
        int     AbandonFGR(int fgr);
        int     CommitFGR(int fgr);
        int     PopAbandoned(int& prf);

        bool    IsBankFull(int index) const;
        bool    IsAllBankFull() const;
        bool    IsBankAbandoned(int index) const;
        bool    HasBankAbandoned() const;
        bool    IsBankValid(int index) const;
        bool    HasBankValid() const;

        int     GetBankFGR(int index) const;
        bool    IsBankFGRValid(int index) const;

        void    Eval();
    };
}

