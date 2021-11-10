#include <list>
#include <vector>

#include "core_issue.hpp"

using namespace std;

//
namespace MELA::Core::Issue {

    //
    RATFreelistCheckpoint::RATFreelistCheckpoint(int bank_count, int bank_depth)
        : bank_count(bank_count), bank_depth(bank_depth)
    {
        elaborated_banks  = new list<int>[bank_count];
        elaborated_fgrs   = new int[bank_count];
        elaborated_states = new int[bank_count];

        elaborated_next_banks  = new int[bank_count];
        elaborated_next_fgrs   = new int[bank_count];
        elaborated_next_states = new int[bank_count];

        for (int i = 0; i < bank_count; i++)
        {
            elaborated_fgrs[i]      = -1;
            elaborated_next_fgrs[i] = -1;

            elaborated_states[i]      = ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;
            elaborated_next_states[i] = ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;

            elaborated_next_banks[i]  = ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_NOP;
        }
    }

    RATFreelistCheckpoint::RATFreelistCheckpoint(const RATFreelistCheckpoint& obj)
        : bank_count(bank_count), bank_depth(bank_depth)
    {
        elaborated_banks  = new list<int>[bank_count];
        elaborated_fgrs   = new int[bank_count];
        elaborated_states = new int[bank_count];

        elaborated_next_banks  = new int[bank_count];
        elaborated_next_fgrs   = new int[bank_count];
        elaborated_next_states = new int[bank_count];

        for (int i = 0; i < bank_count; i++)
        {
            elaborated_fgrs[i]      = obj.elaborated_fgrs[i];
            elaborated_next_fgrs[i] = -1;

            elaborated_states[i]      = obj.elaborated_states[i];
            elaborated_next_states[i] = ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;

            elaborated_banks[i]       = obj.elaborated_banks[i];
            elaborated_next_banks[i]  = ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_NOP;
        }
    }

    RATFreelistCheckpoint::~RATFreelistCheckpoint()
    {
        delete[] elaborated_banks;
        delete[] elaborated_fgrs;
        delete[] elaborated_states;

        delete[] elaborated_next_banks;
        delete[] elaborated_next_fgrs;
        delete[] elaborated_next_states;
    }

    int RATFreelistCheckpoint::GetBankCount()
    {
        return bank_count;
    }

    int RATFreelistCheckpoint::GetBankDepth()
    {
        return bank_depth;
    }

    int RATFreelistCheckpoint::PushAcquired(int fgr, int prf)
    {
        for (int i = 0; i < bank_count; i++)
        {
            if (elaborated_states[i] == ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID)
            {
                elaborated_next_fgrs[i]  = fgr;
                elaborated_next_banks[i] = prf;

                elaborated_next_states[i] = ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_VALID;

                return 1;
             }
            else if (elaborated_states[i] == ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_VALID)
            {
                if ((elaborated_fgrs[i] == fgr)
                    && (elaborated_banks[i].size() < bank_depth))
                {
                    elaborated_next_banks[i] = prf;

                    return 1;
                }
            }
        }

        return 0;
    }

    int RATFreelistCheckpoint::AbandonFGR(int fgr)
    {
        int r = 0;
    
        for (int i = 0; i < bank_count; i++)
        {
            if ((elaborated_states[i] == ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_VALID)
                && (elaborated_fgrs[i] == fgr))
            {
                elaborated_next_states[i] = ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED;

                r++;
            }
        }

        return r;
    }

    int RATFreelistCheckpoint::CommitFGR(int fgr)
    {
        int r = 0;

        for (int i = 0; i < bank_count; i++)
        {
            if ((elaborated_states[i] == ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_VALID)
                && (elaborated_fgrs[i] == fgr))
            {
                elaborated_next_banks[i]  = ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_CLEAR;
                elaborated_next_states[i] = ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;

                r++;
            }
        }

        return r;
    }

    int RATFreelistCheckpoint::PopAbandoned(int& prf)
    {
        for (int i = 0; i < bank_count; i++)
        {
            if (elaborated_states[i] == ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED)
            {
                if (elaborated_banks[i].empty())
                    continue;

                prf = elaborated_banks[i].back();

                elaborated_next_banks[i] = ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_POP;

                return 1;
            }
        }

        return 0;
    }

    bool RATFreelistCheckpoint::IsBankFull(int index)
    {
        return elaborated_banks[index].size() == bank_depth;
    }

    bool RATFreelistCheckpoint::IsAllBankFull()
    {
        for (int i = 0; i < bank_count; i++)
            if (!IsBankFull(i))
                return false;

        return true;
    }

    bool RATFreelistCheckpoint::IsBankAbandoned(int index)
    {
        return elaborated_states[index] == ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED;
    }

    bool RATFreelistCheckpoint::HasBankAbandoned()
    {
        for (int i = 0; i < bank_count; i++)
            if (IsBankAbandoned(i))
                return true;

        return false;
    }

    bool RATFreelistCheckpoint::IsBankValid(int index)
    {
        return elaborated_states[index] == ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_VALID;
    }

    bool RATFreelistCheckpoint::HasBankValid()
    {
        for (int i = 0; i < bank_count; i++)
            if (IsBankValid(i))
                return true;

        return false;
    }

    int RATFreelistCheckpoint::GetBankFGR(int index)
    {
        return elaborated_fgrs[index];
    }

    bool RATFreelistCheckpoint::IsBankFGRValid(int index)
    {
        return IsBankValid(index) || IsBankAbandoned(index);
    }

    void RATFreelistCheckpoint::Eval()
    {
        //
        for (int i = 0; i < 4; i++)
            if (elaborated_states[i] == ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED)
                if (elaborated_banks[i].empty())
                    elaborated_next_states[i] = ELABORATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;

        //
        for (int i = 0; i < 4; i++)
        {
            elaborated_fgrs[i]   = elaborated_next_fgrs[i];
            elaborated_states[i] = elaborated_next_states[i];

            if (elaborated_next_banks[i] >= 0)
                elaborated_banks[i].push_front(elaborated_next_banks[i]);
            else if (elaborated_next_banks[i] == ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_POP)
                elaborated_banks[i].pop_back();
            else if (elaborated_next_banks[i] == ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_CLEAR)
                elaborated_banks[i].clear();
        }

        //
        for (int i = 0; i < 4; i++)
        {
            elaborated_next_fgrs[i]   = elaborated_fgrs[i];
            elaborated_next_states[i] = elaborated_states[i];
            elaborated_next_banks[i]  = ELABORATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_NOP;
        }
    }
}
