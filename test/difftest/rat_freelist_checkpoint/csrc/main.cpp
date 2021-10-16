#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

using namespace std;

//#define DIFF_DEBUG

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#include "Vsim_rat_freelist_checkpoint.h"
static Vsim_rat_freelist_checkpoint* dut_ptr;

void clkp_dumpgen(int& time)
{
    dut_ptr->clk = 1;

    dut_ptr->eval();

#ifdef VM_TRACE
    fp->dump(time++);
#endif
}

void clkn_dumpgen(int& time)
{
    dut_ptr->clk = 0;

    dut_ptr->eval();

#ifdef VM_TRACE
    fp->dump(time++);
#endif
}

void clk_dumpgen(int &time)
{
    clkp_dumpgen(time);
    clkn_dumpgen(time);
}

void reset(int& t)
{
    dut_ptr->resetn = 0;

    clk_dumpgen(t);
    clkp_dumpgen(t);

    dut_ptr->resetn = 1;

    printf("[##] \033[1;33mCircuit reset.\033[0m\n");
}

//
#define ELABORATED_STATE_INVALID        0x00
#define ELABORATED_STATE_VALID          0x01
#define ELABORATED_STATE_ABANDONED      0x02

#define ELABORATED_BANK_NEXT_NOP        -1
#define ELABORATED_BANK_NEXT_POP        -2
#define ELABORATED_BANK_NEXT_CLEAR      -3

list<int>*  elaborated_banks;
int*        elaborated_fgrs;
int*        elaborated_states;

int*        elaborated_next_banks;
int*        elaborated_next_fgrs;
int*        elaborated_next_states;

void elaborated_initialize()
{
    elaborated_banks  = new list<int>[4];
    elaborated_fgrs   = new int[4];
    elaborated_states = new int[4];

    elaborated_next_banks  = new int[4];
    elaborated_next_fgrs   = new int[4];
    elaborated_next_states = new int[4];

    for (int i = 0; i < 4; i++)
    {
        elaborated_fgrs[i]      = -1;
        elaborated_next_fgrs[i] = -1;

        elaborated_states[i]      = ELABORATED_STATE_INVALID;
        elaborated_next_states[i] = ELABORATED_STATE_INVALID;

        elaborated_next_banks[i] = ELABORATED_BANK_NEXT_NOP;
    }
}

void elaborated_delete()
{
    delete[] elaborated_banks;
    delete[] elaborated_fgrs;
    delete[] elaborated_states;

    delete[] elaborated_next_banks;
    delete[] elaborated_next_fgrs;
    delete[] elaborated_next_states;
}

int elaborated_push(int fgr, int value)
{
    for (int i = 0; i < 4; i++)
    {
        if (elaborated_states[i] == ELABORATED_STATE_INVALID)
        {
            elaborated_next_fgrs[i]  = fgr;
            elaborated_next_banks[i] = value;

            elaborated_next_states[i] = ELABORATED_STATE_VALID;

            return 1;
        }
        else if (elaborated_states[i] == ELABORATED_STATE_VALID)
        {
            if ((elaborated_fgrs[i] == fgr)
                && (elaborated_banks[i].size() < 4))
            {
                elaborated_next_banks[i] = value;

                return 1;
            }
        }
    }

    return 0;
}

int elaborated_abandon(int fgr)
{
    int r = 0;
    
    for (int i = 0; i < 4; i++)
    {
        if ((elaborated_states[i] == ELABORATED_STATE_VALID)
            && (elaborated_fgrs[i] == fgr))
        {
            elaborated_next_states[i] = ELABORATED_STATE_ABANDONED;

            r++;
        }
    }

    return r;
}

int elaborated_commit(int fgr)
{
    int r = 0;

    for (int i = 0; i < 4; i++)
    {
        if ((elaborated_states[i] == ELABORATED_STATE_VALID)
            && (elaborated_fgrs[i] == fgr))
        {
            elaborated_next_banks[i]  = ELABORATED_BANK_NEXT_CLEAR;
            elaborated_next_states[i] = ELABORATED_STATE_INVALID;

            r++;
        }
    }

    return r;
}

int elaborated_pop(int& prf)
{
    for (int i = 0; i < 4; i++)
    {
        if (elaborated_states[i] == ELABORATED_STATE_ABANDONED)
        {
            if (elaborated_banks[i].empty())
                continue;

            prf = elaborated_banks[i].back();

            elaborated_next_banks[i] = ELABORATED_BANK_NEXT_POP;

            return 1;
        }
    }

    return 0;
}

void elaboration_eval()
{
    //
    for (int i = 0; i < 4; i++)
        if (elaborated_states[i] == ELABORATED_STATE_ABANDONED)
            if (elaborated_banks[i].empty())
                elaborated_next_states[i] = ELABORATED_STATE_INVALID;

    //
    for (int i = 0; i < 4; i++)
    {
        elaborated_fgrs[i]   = elaborated_next_fgrs[i];
        elaborated_states[i] = elaborated_next_states[i];

        if (elaborated_next_banks[i] >= 0)
            elaborated_banks[i].push_front(elaborated_next_banks[i]);
        else if (elaborated_next_banks[i] == ELABORATED_BANK_NEXT_POP)
            elaborated_banks[i].pop_back();
        else if (elaborated_next_banks[i] == ELABORATED_BANK_NEXT_CLEAR)
            elaborated_banks[i].clear();
    }

    //
    for (int i = 0; i < 4; i++)
    {
        elaborated_next_fgrs[i]   = elaborated_fgrs[i];
        elaborated_next_states[i] = elaborated_states[i];
        elaborated_next_banks[i]  = ELABORATED_BANK_NEXT_NOP;
    }
}

//

int testbench_0(int& t)
{
    int error = 0;

    // Testbench #0
    // Verify on reset state.
    printf("[#0] Testbench #0\n");
    printf("[#0] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#0] Verify on post-reset state.\n");

    //
    if (dut_ptr->o_abandoned_valid)
    {
        printf("[#0] Incorrect output on 'o_abandoned_valid'. Reading '%x', expected '%x'.\n",
                dut_ptr->o_abandoned_valid, 0);
        error++;
    }

    if (dut_ptr->o_acquired_ready)
    {
        printf("[#0] Incorrect output on 'o_acquired_ready'. Reading '%x', expected '%x'.\n",
                dut_ptr->o_acquired_ready, 0);
        error++;
    }

    // zero beat
    dut_ptr->i_abandon_valid = 0;
    dut_ptr->i_abandon_fgr   = 0;

    dut_ptr->i_commit_valid = 0;
    dut_ptr->i_commit_fgr   = 0;

    dut_ptr->i_abandoned_ready = 0;

    dut_ptr->i_acquired_valid = 0;
    dut_ptr->i_acquired_fgr   = 0;
    dut_ptr->i_acquired_prf   = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    if (error)
        printf("[#0] Testbench #0 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#0] Testbench #0 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

int testbench_1(int& t)
{
    int error = 0;

    // Testbench #1
    // Saturated write and abandon-backtrack test.
    printf("[#1] Testbench #1\n");
    printf("[#1] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#1] Saturated write and abandon-backtrack test.\n");

    //
    bool prf_sim_backtrack[16] = { 0 };
    int  prf_sim = 0;

    int backtrack_error = 0;

    // saturated write
    for (int i = 0; i < 4; i++) // bank fill
    {
        // i -> FGR

        for (int j = 0; j < 4; j++, prf_sim++) // saturation
        {
            //
            dut_ptr->i_acquired_valid = 1;
            dut_ptr->i_acquired_fgr   = i;
            dut_ptr->i_acquired_prf   = prf_sim;

            clkn_dumpgen(t);

            //
            if (!dut_ptr->o_acquired_ready)
            {
                printf("[#1] Incorrect state detected. Early saturation (FGR = %d, index = %d).\n",
                    i, prf_sim);
                error++;

                goto TESTBENCH_1_WRITE_END;
            }

            clkp_dumpgen(t);
            //
        }
    }

    clkn_dumpgen(t);

    if (dut_ptr->o_acquired_ready)
    {
        printf("[#1] Incorrect state detected. Out of saturation.\n");
        error++;
    }
    else
        printf("[#1] Saturation correct.\n");

    clkp_dumpgen(t);

    TESTBENCH_1_WRITE_END:
    dut_ptr->i_acquired_valid = 0;
    dut_ptr->i_acquired_fgr   = 0;
    dut_ptr->i_acquired_prf   = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    // abandon-backtrack
    for (int i = 0; i < 4; i++)
    {
        // i -> FGR

        //
        dut_ptr->i_abandon_valid = 1;
        dut_ptr->i_abandon_fgr   = i;

        clkn_dumpgen(t);
        clkp_dumpgen(t);

        //
        dut_ptr->i_abandon_valid = 0;
        dut_ptr->i_abandon_fgr   = 0;

        clkn_dumpgen(t);
        clkp_dumpgen(t);
        
        //
        if (!dut_ptr->o_abandoned_valid)
        {
            printf("[#1] Abandon-backtrack no response on FGR '%d'.\n", i);
            error++;

            goto TESTBENCH_1_BACKTRACK_END;
        }

        //
        for (int j = 0; j < 4; j++)
        {
            dut_ptr->i_abandoned_ready = 1;

            if (!dut_ptr->o_abandoned_valid)
            {
                printf("[#1] Abandon-backtrack no response on FGR '%d' (offset = %d).\n",
                    i, j);
                error++;

                goto TESTBENCH_1_BACKTRACK_END;
            }

            prf_sim_backtrack[dut_ptr->o_abandoned_prf] = 1;

            clkn_dumpgen(t);
            clkp_dumpgen(t);
        }

        //
        dut_ptr->i_abandoned_ready = 0;

        clkn_dumpgen(t);
        clkp_dumpgen(t);
    }

    if (dut_ptr->o_abandoned_valid)
    {
        printf("[#1] Incorrect state detected. Abandon-backtrack hold-up.\n");
        error++;
    }
    else
    {
        printf("[#1] Abandon-backtrack state correct.\n");
    }

    for (int i = 0; i < 16; i++)
        if (!prf_sim_backtrack[i])
        {
            printf("[#1] Abandon-backtrack pattern incorrect. Missing at '%d'.\n", i);
            backtrack_error++;
            error++;
        }

    if (!backtrack_error)
        printf("[#1] Abandon-backtrack pattern correct.\n");

    TESTBENCH_1_BACKTRACK_END:
    dut_ptr->i_abandoned_ready = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    if (error)
        printf("[#1] Testbench #1 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#1] Testbench #1 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

int testbench_2(int& t)
{
    int error = 0;

    // Testbench #2
    // Incremental un-saturated write and abandon-trackback test
    printf("[#2] Testbench #2\n");
    printf("[#2] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#2] Incremental un-saturated write and abandon-trackback test.\n");

    //
    bool prf_sim_backtrack[16];
    int  prf_sim;

    for (int i = 1; i < 4; i++)
    {
        printf("[#2] %d/4 un-saturated write ...\n", i);

        //
        for (int j = 0; j < 16; j++)
            prf_sim_backtrack[j] = 0;

        prf_sim = 0;

        // unsaturated bank write
        for (int j = 0; j < 4; j++)
        {
            // j -> FGR

            for (int k = 0; k < i; k++, prf_sim++)
            {
                //
                dut_ptr->i_acquired_valid = 1;
                dut_ptr->i_acquired_fgr   = j;
                dut_ptr->i_acquired_prf   = prf_sim;

                clkn_dumpgen(t);
                
                //
                if (!dut_ptr->o_acquired_ready)
                {
                    printf("[#2] Incorrect state detected (%d/4). Early saturation (FGR = %d, index = %d).\n",
                        i, j, prf_sim);
                    error++;

                    goto TESTBENCH_2_WRITE_END;
                }

                clkp_dumpgen(t);
                //
            }
        }

        TESTBENCH_2_WRITE_END:
        dut_ptr->i_acquired_valid = 0;
        dut_ptr->i_acquired_fgr   = 0;
        dut_ptr->i_acquired_prf   = 0;

        clkn_dumpgen(t);
        clkp_dumpgen(t);

        // backtrack read
        for (int j = 0; j < 4; j++)
        {
            // j -> FGR

            //
            dut_ptr->i_abandon_valid = 1;
            dut_ptr->i_abandon_fgr   = j;

            clkn_dumpgen(t);
            clkp_dumpgen(t);

            //
            dut_ptr->i_abandon_valid = 0;
            dut_ptr->i_abandon_fgr   = 0;

            clkn_dumpgen(t);
            clkp_dumpgen(t);

            for (int k = 0; k < i; k++)
            {
                dut_ptr->i_abandoned_ready = 1;

                if (!dut_ptr->o_abandoned_valid)
                {
                    printf("[#2] Abandon-backtrack no response on FGR '%d'.\n", i);
                    error++;

                    goto TESTBENCH_2_BACKTRACK_END;
                }

                prf_sim_backtrack[dut_ptr->o_abandoned_prf] = 1;

                clkn_dumpgen(t);
                clkp_dumpgen(t);
            }

            //
            dut_ptr->i_abandoned_ready = 0;

            clkn_dumpgen(t);
            clkp_dumpgen(t);
        }

        if (dut_ptr->o_abandoned_valid)
        {
            printf("[#2] Incorrect state detected (%d/4). Abandon-backtrack hold-up.\n", i);
            error++;
        }

        TESTBENCH_2_BACKTRACK_END:
        dut_ptr->i_abandoned_ready = 0;

        clkn_dumpgen(t);
        clkp_dumpgen(t);

        for (int j = 0; j < prf_sim; j++)
            if (!prf_sim_backtrack[j])
            {
                printf("[#2] Abandon-backtrack pattern incorrect (%d/4). Missing at '%d'.\n", i, j);
                error++;
            }
    }

    //
    if (error)
        printf("[#2] Testbench #2 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#2] Testbench #2 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

int testbench_3(int& t)
{
    int error = 0;

    // Testbench #3
    // Interleaving saturated write and abandon-backtrack test.
    printf("[#3] Testbench #3\n");
    printf("[#3] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#3] Interleaving saturated write and abandon-backtrack test.\n");

    // TODO
    bool prf_sim_backtrack[16] = { 0 };
    int  prf_sim = 0;

    int  backtrack_error = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++, prf_sim++)
        {
            // j -> FGR

            //
            dut_ptr->i_acquired_valid = 1;
            dut_ptr->i_acquired_fgr   = j;
            dut_ptr->i_acquired_prf   = prf_sim;

            clkn_dumpgen(t);

            //
            if (!dut_ptr->o_acquired_ready)
            {
                printf("[#3] Incorrect state detected. Early saturation (FGR = %d, index = %d).\n",
                    i, prf_sim);
                error++;

                goto TESTBENCH_3_WRITE_END;
            }

            clkp_dumpgen(t);
            //
        }
    }

    clkn_dumpgen(t);

    if (dut_ptr->o_acquired_ready)
    {
        printf("[#3] Incorrect state detected. Out of saturation.\n");
        error++;
    }
    else
        printf("[#3] Saturation correct.\n");

    clkp_dumpgen(t);

    TESTBENCH_3_WRITE_END:
    dut_ptr->i_acquired_valid = 0;
    dut_ptr->i_acquired_fgr   = 0;
    dut_ptr->i_acquired_prf   = 0;

    // abandon-backtrack
    for (int i = 0; i < 4; i++)
    {
        // i -> FGR

        //
        dut_ptr->i_abandon_valid = 1;
        dut_ptr->i_abandon_fgr   = i;

        clkn_dumpgen(t);
        clkp_dumpgen(t);

        //
        dut_ptr->i_abandon_valid = 0;
        dut_ptr->i_abandon_fgr   = 0;

        clkn_dumpgen(t);
        clkp_dumpgen(t);
        
        //
        if (!dut_ptr->o_abandoned_valid)
        {
            printf("[#3] Abandon-backtrack no response on FGR '%d'.\n", i);
            error++;

            goto TESTBENCH_3_BACKTRACK_END;
        }

        //
        for (int j = 0; j < 4; j++)
        {
            dut_ptr->i_abandoned_ready = 1;

            if (!dut_ptr->o_abandoned_valid)
            {
                printf("[#3] Abandon-backtrack no response on FGR '%d' (offset = %d).\n",
                    i, j);
                error++;

                goto TESTBENCH_3_BACKTRACK_END;
            }

            prf_sim_backtrack[dut_ptr->o_abandoned_prf] = 1;

            clkn_dumpgen(t);
            clkp_dumpgen(t);
        }

        //
        dut_ptr->i_abandoned_ready = 0;

        clkn_dumpgen(t);
        clkp_dumpgen(t);
    }

    if (dut_ptr->o_abandoned_valid)
    {
        printf("[#3] Incorrect state detected. Abandon-backtrack hold-up.\n");
        error++;
    }
    else
    {
        printf("[#3] Abandon-backtrack state correct.\n");
    }

    for (int i = 0; i < 16; i++)
        if (!prf_sim_backtrack[i])
        {
            printf("[#3] Abandon-backtrack pattern incorrect. Missing at '%d'.\n", i);
            backtrack_error++;
            error++;
        }

    if (!backtrack_error)
        printf("[#3] Abandon-backtrack pattern correct.\n");

    TESTBENCH_3_BACKTRACK_END:
    dut_ptr->i_abandoned_ready = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    if (error)
        printf("[#3] Testbench #3 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#3] Testbench #3 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

int testbench_4(int& t)
{
    int error = 0;

    // Testbench #4
    // Interleaving incremental un-saturated write and abandon-backtrack test.
    printf("[#4] Testbench #4\n");
    printf("[#4] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#4] Interleaving incremental un-saturated write and abandon-backtrack test.\n");

    //
    bool prf_sim_backtrack[16];
    int  prf_sim;

    for (int i = 1; i < 4; i++)
    {
        printf("[#4] %d/4 un-saturated write ...\n", i);

        //
        for (int j = 0; j < 16; j++)
            prf_sim_backtrack[j] = 0;

        prf_sim = 0;

        // unsaturated bank write
        for (int j = 0; j < i; j++)
        {
            for (int k = 0; k < 4; k++, prf_sim++)
            {
                // k -> FGR

                //
                dut_ptr->i_acquired_valid = 1;
                dut_ptr->i_acquired_fgr   = k;
                dut_ptr->i_acquired_prf   = prf_sim;

                clkn_dumpgen(t);
                
                //
                if (!dut_ptr->o_acquired_ready)
                {
                    printf("[#4] Incorrect state detected (%d/4). Early saturation (FGR = %d, index = %d).\n",
                        j, k, prf_sim);
                    error++;

                    goto TESTBENCH_4_WRITE_END;
                }

                clkp_dumpgen(t);
                //
            }
        }

        TESTBENCH_4_WRITE_END:
        dut_ptr->i_acquired_valid = 0;
        dut_ptr->i_acquired_fgr   = 0;
        dut_ptr->i_acquired_prf   = 0;

        clkn_dumpgen(t);
        clkp_dumpgen(t);

        // backtrack read
        for (int j = 0; j < 4; j++)
        {
            // j -> FGR

            //
            dut_ptr->i_abandon_valid = 1;
            dut_ptr->i_abandon_fgr   = j;

            clkn_dumpgen(t);
            clkp_dumpgen(t);

            //
            dut_ptr->i_abandon_valid = 0;
            dut_ptr->i_abandon_fgr   = 0;

            clkn_dumpgen(t);
            clkp_dumpgen(t);

            for (int k = 0; k < i; k++)
            {
                dut_ptr->i_abandoned_ready = 1;

                if (!dut_ptr->o_abandoned_valid)
                {
                    printf("[#4] Abandon-backtrack no response on FGR '%d'.\n", i);
                    error++;

                    goto TESTBENCH_4_BACKTRACK_END;
                }

                prf_sim_backtrack[dut_ptr->o_abandoned_prf] = 1;

                clkn_dumpgen(t);
                clkp_dumpgen(t);
            }

            //
            dut_ptr->i_abandoned_ready = 0;

            clkn_dumpgen(t);
            clkp_dumpgen(t);
        }

        if (dut_ptr->o_abandoned_valid)
        {
            printf("[#4] Incorrect state detected (%d/4). Abandon-backtrack hold-up.\n", i);
            error++;
        }

        TESTBENCH_4_BACKTRACK_END:
        dut_ptr->i_abandoned_ready = 0;

        clkn_dumpgen(t);
        clkp_dumpgen(t);

        for (int j = 0; j < prf_sim; j++)
            if (!prf_sim_backtrack[j])
            {
                printf("[#4] Abandon-backtrack pattern incorrect (%d/4). Missing at '%d'.\n", i, j);
                error++;
            }
    }

    //
    if (error)
        printf("[#4] Testbench #4 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#4] Testbench #4 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

int testbench_5(int& t)
{
    int error = 0;

    // Testbench #5
    // Elaborated differential random read-write test.
    printf("[#5] Testbench #5\n");
    printf("[#5] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#5] Elaborated differential random read-write test.\n");

    //
    elaborated_initialize();
    printf("[#5] Elaborated variables initialized.\n");

    //
    int c = 65535;

    printf("[#5] \033[1;30mDifferential payload count: %d\033[0m\n", c);

    srand(time(0));

#ifdef DIFF_DEBUG
    vector<int> diff_elaborated_history;
    vector<int> diff_dut_history;

    vector<int> diff_trace_fgr;

    vector<int> diff_trace_push;
    vector<int> diff_trace_pop;
    vector<int> diff_trace_abandon;
    vector<int> diff_trace_commit;
#endif

    int delay_push      = 0;
    int delay_pop       = 0;
    int delay_abandon   = 0;
    int delay_commit    = 0;

    int cnt_push    = 0;
    int cnt_pop     = 0;
    int cnt_abandon = 0;
    int cnt_commit  = 0;

    // payload
    for (int i = 0; i < c; i++)
    {
        int fgr     = rand() % 8;
        int value   = rand() % 64;

        //
        if (!delay_abandon)
        {
            //
            elaborated_abandon(fgr);

            //
            dut_ptr->i_abandon_valid = 1;
            dut_ptr->i_abandon_fgr   = fgr;

            // 
            cnt_abandon++;
            delay_abandon = rand() % 32;
        }
        else
            delay_abandon--;

        // Avoid abandon-commit FGR violation, not elaborated (UNDEFINED)
        fgr = (fgr + 1) % 8;

        //
        if (!delay_commit)
        {
            //
            elaborated_commit(fgr);

            //
            dut_ptr->i_commit_valid = 1;
            dut_ptr->i_commit_fgr   = fgr;

            //
            cnt_commit++;
            delay_commit = rand() % 64;
        }
        else
            delay_commit--;

        //
        if (!delay_pop)
        {
            int diff_elaborated_val;
            int diff_dut_val;

            //
            if (!elaborated_pop(diff_elaborated_val))
                diff_elaborated_val = -1;

            //
            dut_ptr->i_abandoned_ready = 1;

            if (!dut_ptr->o_abandoned_valid)
                diff_dut_val = -1;
            else
                diff_dut_val = dut_ptr->o_abandoned_prf;

            // 
#ifdef DIFF_DEBUG
            diff_elaborated_history.push_back(diff_elaborated_val);

            diff_dut_history.push_back(diff_dut_val);
#endif

            // diff verify
            if (diff_dut_val != diff_elaborated_val)
            {
                printf("[#5] Difference detected at clock edge %d.\n", t);

#ifdef DIFF_DEBUG
                for (int j = 0; j < 4; j++)
                    diff_trace_fgr.push_back(elaborated_fgrs[j]);

                diff_trace_push.push_back(cnt_push);
                diff_trace_pop.push_back(cnt_pop);
                diff_trace_abandon.push_back(cnt_abandon);
                diff_trace_commit.push_back(cnt_commit);
#endif

                error++;
            }

            //
            cnt_pop++;
            delay_pop = rand() % 8;
        }
        else
            delay_pop--;

        //
        fgr = (fgr + 1) % 8;

        //
        if (!delay_push)
        {
            //
            elaborated_push(fgr, value);

            //
            dut_ptr->i_acquired_valid = 1;
            dut_ptr->i_acquired_fgr   = fgr;
            dut_ptr->i_acquired_prf   = value;

            //
            cnt_push++;
            delay_push = rand() % 4;
        }
        else
            delay_push--;

        //
        elaboration_eval();

        //
        clkn_dumpgen(t);
        clkp_dumpgen(t);

        //
        dut_ptr->i_acquired_valid = 0;
        dut_ptr->i_acquired_fgr   = 0;
        dut_ptr->i_acquired_prf   = 0;

        dut_ptr->i_abandoned_ready = 0;

        dut_ptr->i_commit_valid = 0;
        dut_ptr->i_commit_fgr   = 0;

        dut_ptr->i_abandon_valid = 0;
        dut_ptr->i_abandon_fgr   = 0;
    }

    //
    elaborated_delete();
    printf("[#5] Elaborated variables finalized.\n");

#ifdef DIFF_DEBUG
    if (error)
    {
        printf("[#5] Differential test failed.\n");

        printf("[#5] Differential comparsion history: \n");

        int j = 0;

        for (int i = 0; i < diff_dut_history.size(); i++, j++)
        {
            if (diff_elaborated_history[i] != diff_dut_history[i])
                printf("\033[1;31m");

            printf("[%3d, %3d]", diff_elaborated_history[i], diff_dut_history[i]);

            printf("\033[0m");

            if (j == 8)
            {
                printf("\n");
                j = -1;
            }
        }

        printf("\n");

        printf("[#5] Trace: \n");

        for (int i = 0; i < diff_trace_push.size(); i++)
        {
            printf("[#5] ");

            printf("FGR: [%3d, %3d, %3d, %3d], ",
                    diff_trace_fgr[i * 4],
                    diff_trace_fgr[i * 4 + 1],
                    diff_trace_fgr[i * 4 + 2],
                    diff_trace_fgr[i * 4 + 3]);

            printf("push: %5d, pop: %5d, abandon: %5d, commit: %5d.\n",
                diff_trace_push[i],
                diff_trace_pop[i],
                diff_trace_abandon[i],
                diff_trace_commit[i]);
        }

        printf("\n");
    }
#else
    if (error)
    {
        printf("[#5] - Differential test failed.\n");
        printf("[#5] - Use definition symbol \'DIFF_DEBUG\' for further information.\n");
    }
#endif

    //
    if (error)
        printf("[#5] Testbench #5 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#5] Testbench #5 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

void test()
{
    int t = 0;
    int e = 0;

    printf("[--] ----------------------------------------\n");

    printf("[##] Testing on module '\033[1;33missue_rat_freelist_checkpoint\033[0m'\n");

    printf("[--] ----------------------------------------\n");

    reset(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_0(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_1(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_2(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_3(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_4(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_5(t);

    printf("[--] ----------------------------------------\n");

    printf("Test ");

    if (e)
        printf("\033[1;31mFAILED\033[0m");
    else
        printf("\033[1;32mPASSED\033[0m");

    printf(", %d error(s).\n", e);
}

int main()
{

    dut_ptr = new Vsim_rat_freelist_checkpoint;
    printf("\033[1;33mRAT Freelist Checkpoint of Issue Stage module 'issue_rat_freelist_checkpoint' selected.\033[0m\n");

#ifdef VM_TRACE
    ////// !!!  ATTENTION  !!!//////
    //  Call Verilated::traceEverOn(true) first.
    //  Then create a VerilatedVcdC object.    
    Verilated::traceEverOn(true);
    printf("Enabling waves ...\n");
    fp = new VerilatedVcdC;     //instantiating .vcd object   
    dut_ptr->trace(fp, 99);     //trace 99 levels of hierarchy
    fp->open("vlt_dump.vcd");   
    fp->dump(0);
#endif

    // payload
    test();

    // finalize
#ifdef VM_TRACE
    fp->close();
    delete fp;
#endif
    delete dut_ptr;

    printf("\033[1;33mFinalized.\033[0m\n");

    return 0;
}

