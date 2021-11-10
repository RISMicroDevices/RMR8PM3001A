#pragma once
//
// Mixed elaboration for RAT Freelist Checkpoint
//
//  @see 'rat_freelist_checkpoint.v'
//

#include <list>

#define ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID            0x00
#define ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_VALID              0x01
#define ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED          0x02

#define ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_NOP            -1
#define ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_POP            -2
#define ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_CLEAR          -3

using namespace std;

namespace MELA::Core::Issue {

    class RATFreelistCheckpoint
    {
    private:
        list<int>*  elaborated_banks;
        int*        elaborated_fgrs;
        int*        elaborated_states;

        int*        elaborated_next_banks;
        int*        elaborated_next_fgrs;
        int*        elaborated_next_states;

        const int   bank_count; 
        const int   bank_depth;

    public:
        RATFreelistCheckpoint(int bank_count, int bank_depth);
        RATFreelistCheckpoint(const RATFreelistCheckpoint& obj);
        ~RATFreelistCheckpoint();

        int     GetBankCount();
        int     GetBankDepth();

        int     PushAcquired(int fgr, int prf);
        int     AbandonFGR(int fgr);
        int     CommitFGR(int fgr);
        int     PopAbandoned(int& prf);

        bool    IsBankFull(int index);
        bool    IsAllBankFull();
        bool    IsBankAbandoned(int index);
        bool    HasBankAbandoned();
        bool    IsBankValid(int index);
        bool    HasBankValid();

        int     GetBankFGR(int index);
        bool    IsBankFGRValid(int index);

        void    Eval();
    };
}

