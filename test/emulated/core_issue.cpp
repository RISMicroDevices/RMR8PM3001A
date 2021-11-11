#include <list>
#include <vector>

#include "core_issue.hpp"

using namespace std;

// class MEMU::Core::Issue::RATFreelistCheckpoint
namespace MEMU::Core::Issue {

    //
    RATFreelistCheckpoint::RATFreelistCheckpoint(int bank_count, int bank_depth)
        : bank_count(bank_count), bank_depth(bank_depth)
    {
        emulated_banks  = new list<int>[bank_count];
        emulated_fgrs   = new int[bank_count];
        emulated_states = new int[bank_count];

        emulated_next_banks  = new int[bank_count];
        emulated_next_fgrs   = new int[bank_count];
        emulated_next_states = new int[bank_count];

        for (int i = 0; i < bank_count; i++)
        {
            emulated_fgrs[i]      = -1;
            emulated_next_fgrs[i] = -1;

            emulated_states[i]      = EMULATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;
            emulated_next_states[i] = EMULATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;

            emulated_next_banks[i]  = EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_NOP;
        }
    }

    RATFreelistCheckpoint::RATFreelistCheckpoint(const RATFreelistCheckpoint& obj)
        : bank_count(bank_count), bank_depth(bank_depth)
    {
        emulated_banks  = new list<int>[bank_count];
        emulated_fgrs   = new int[bank_count];
        emulated_states = new int[bank_count];

        emulated_next_banks  = new int[bank_count];
        emulated_next_fgrs   = new int[bank_count];
        emulated_next_states = new int[bank_count];

        for (int i = 0; i < bank_count; i++)
        {
            emulated_fgrs[i]      = obj.emulated_fgrs[i];
            emulated_next_fgrs[i] = -1;

            emulated_states[i]      = obj.emulated_states[i];
            emulated_next_states[i] = EMULATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;

            emulated_banks[i]       = obj.emulated_banks[i];
            emulated_next_banks[i]  = EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_NOP;
        }
    }

    RATFreelistCheckpoint::~RATFreelistCheckpoint()
    {
        delete[] emulated_banks;
        delete[] emulated_fgrs;
        delete[] emulated_states;

        delete[] emulated_next_banks;
        delete[] emulated_next_fgrs;
        delete[] emulated_next_states;
    }

    int RATFreelistCheckpoint::GetBankCount() const
    {
        return bank_count;
    }

    int RATFreelistCheckpoint::GetBankDepth() const
    {
        return bank_depth;
    }

    int RATFreelistCheckpoint::PushAcquired(int fgr, int prf)
    {
        for (int i = 0; i < bank_count; i++)
        {
            if (emulated_states[i] == EMULATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID)
            {
                emulated_next_fgrs[i]  = fgr;
                emulated_next_banks[i] = prf;

                emulated_next_states[i] = EMULATED_RAT_FREELIST_CHECKPOINT_STATE_VALID;

                return 1;
             }
            else if (emulated_states[i] == EMULATED_RAT_FREELIST_CHECKPOINT_STATE_VALID)
            {
                if ((emulated_fgrs[i] == fgr)
                    && (emulated_banks[i].size() < bank_depth))
                {
                    emulated_next_banks[i] = prf;

                    return 1;
                }
            }
        }

        return 0;
    }

    int RATFreelistCheckpoint::AbandonFGR(int fgr)
    {
        return AbandonFGREx(fgr, nullptr);
    }

    int RATFreelistCheckpoint::CommitFGR(int fgr)
    {
        return CommitFGREx(fgr, nullptr);
    }

    int RATFreelistCheckpoint::PopAbandoned(int& prf)
    {
        for (int i = 0; i < bank_count; i++)
        {
            if (emulated_states[i] == EMULATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED)
            {
                if (emulated_banks[i].empty())
                    continue;

                prf = emulated_banks[i].back();

                emulated_next_banks[i] = EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_POP;

                return 1;
            }
        }

        return 0;
    }

    int RATFreelistCheckpoint::AbandonFGREx(int fgr, int* dst)
    {
        int r = 0;
    
        for (int i = 0; i < bank_count; i++)
        {
            if ((emulated_states[i] == EMULATED_RAT_FREELIST_CHECKPOINT_STATE_VALID)
                && (emulated_fgrs[i] == fgr))
            {
                emulated_next_states[i] = EMULATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED;

                if (dst)
                {
                    list<int>::iterator iter = emulated_banks[i].begin();

                    while (iter != emulated_banks[i].end())
                        dst[r++] = *iter;
                }
                else
                    r += emulated_banks[i].size();
            }
        }

        return r;
    }

    int RATFreelistCheckpoint::CommitFGREx(int fgr, int* dst)
    {
        int r = 0;

        for (int i = 0; i < bank_count; i++)
        {
            if ((emulated_states[i] == EMULATED_RAT_FREELIST_CHECKPOINT_STATE_VALID)
                && (emulated_fgrs[i] == fgr))
            {
                emulated_next_banks[i]  = EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_CLEAR;
                emulated_next_states[i] = EMULATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;

                if (dst)
                {
                    list<int>::iterator iter = emulated_banks[i].begin();

                    while (iter != emulated_banks[i].end())
                        dst[r++] = *iter;
                }
                else
                    r += emulated_banks[i].size();
            }
        }

        return r;
    }


    bool RATFreelistCheckpoint::IsBankFull(int index) const
    {
        return emulated_banks[index].size() == bank_depth;
    }

    bool RATFreelistCheckpoint::IsAllBankFull() const
    {
        for (int i = 0; i < bank_count; i++)
            if (!IsBankFull(i))
                return false;

        return true;
    }

    bool RATFreelistCheckpoint::IsBankAbandoned(int index) const
    {
        return emulated_states[index] == EMULATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED;
    }

    bool RATFreelistCheckpoint::HasBankAbandoned() const
    {
        for (int i = 0; i < bank_count; i++)
            if (IsBankAbandoned(i))
                return true;

        return false;
    }

    bool RATFreelistCheckpoint::IsBankValid(int index) const
    {
        return emulated_states[index] == EMULATED_RAT_FREELIST_CHECKPOINT_STATE_VALID;
    }

    bool RATFreelistCheckpoint::HasBankValid() const
    {
        for (int i = 0; i < bank_count; i++)
            if (IsBankValid(i))
                return true;

        return false;
    }

    int RATFreelistCheckpoint::GetBankFGR(int index) const
    {
        return emulated_fgrs[index];
    }

    bool RATFreelistCheckpoint::IsBankFGRValid(int index) const
    {
        return IsBankValid(index) || IsBankAbandoned(index);
    }

    int RATFreelistCheckpoint::GetBankPRF(int index, int* dst) const
    {
        if (!IsBankPRFValid(index))
            return 0;

        int i = 0;
        list<int>::iterator iter = emulated_banks[index].begin();
        while (iter != emulated_banks[index].end())
            dst[i++] = *iter++;

        return emulated_banks[index].size();
    }

    int RATFreelistCheckpoint::GetBankPRFCount(int index) const
    {
        if (!IsBankPRFValid(index))
            return 0;

        return emulated_banks[index].size();
    }

    bool RATFreelistCheckpoint::IsBankPRFValid(int index) const
    {
        return IsBankValid(index) || IsBankAbandoned(index);
    }

    void RATFreelistCheckpoint::Eval()
    {
        //
        for (int i = 0; i < 4; i++)
            if (emulated_states[i] == EMULATED_RAT_FREELIST_CHECKPOINT_STATE_ABANDONED)
                if (emulated_banks[i].empty())
                    emulated_next_states[i] = EMULATED_RAT_FREELIST_CHECKPOINT_STATE_INVALID;

        //
        for (int i = 0; i < 4; i++)
        {
            emulated_fgrs[i]   = emulated_next_fgrs[i];
            emulated_states[i] = emulated_next_states[i];

            if (emulated_next_banks[i] >= 0)
                emulated_banks[i].push_front(emulated_next_banks[i]);
            else if (emulated_next_banks[i] == EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_POP)
                emulated_banks[i].pop_back();
            else if (emulated_next_banks[i] == EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_CLEAR)
                emulated_banks[i].clear();
        }

        //
        for (int i = 0; i < 4; i++)
        {
            emulated_next_fgrs[i]   = emulated_fgrs[i];
            emulated_next_states[i] = emulated_states[i];
            emulated_next_banks[i]  = EMULATED_RAT_FREELIST_CHECKPOINT_BANK_NEXT_NOP;
        }
    }
}
