#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

#include "core_issue.hpp"

using namespace std;

using namespace MEMU::Core::Issue;

//#define DIFF_DEBUG

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#include "Vsim_rat_freelist.h"
static Vsim_rat_freelist* dut_ptr;

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

int testbench_0(int& t)
{
    int error = 0;

    // Testbench #0
    // Verify post-reset state.
    printf("[#0] Testbench #0\n");
    printf("[#0] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#0] Verify on post-reset state.\n");

    // zero beat
    dut_ptr->i_redeemed_prf             = 0;
    dut_ptr->i_redeemed_valid           = 0;

    dut_ptr->i_acquire_fgr              = 0;
    dut_ptr->i_acquire_fgr_speculative  = 0;
    dut_ptr->i_acquire_valid            = 0;

    dut_ptr->i_commit_fgr               = 0;
    dut_ptr->i_commit_valid             = 0;

    dut_ptr->i_abandon_fgr              = 0;
    dut_ptr->i_abandon_valid            = 0;

    clkn_dumpgen(t);

    if (dut_ptr->o_redeemed_ready)
    {
        printf("[#0] Wrong state detected. 'o_redeemed_ready' asserted.\n");
        error++;
    }

    if (dut_ptr->o_acquire_ready)
    {
        printf("[#0] Wrong state detected. 'o_acquire_ready' asserted.\n");
        error++;
    }

    clkp_dumpgen(t);

    //
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
    // Mixed emulation random differential test.
    printf("[#1] Testbench #1\n");
    printf("[#1] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#1] Mixed emulation random differential test.\n");

    //
    int c = 65535;

    printf("[#1] \033[1;30mDifferential payload count: %d\033[0m\n", c);

    //
    list<int> on_flight_prfs; // On-flight non-speculative PRFs
    RATFreelistCheckpoint elaborated(4, 4);

    list<int> on_flight_fgrs; // On-flight speculative FGRs to be commited/abandoned

    int fgr = 0;

    int fgr_lifecycle       = 0;
    int fgr_speculative     = 0;

    int acquire_count       = 0;
    int acquire_interval    = 0;

    int fgr_op_count        = 0;
    int fgr_op_interval     = 0;

    int redeem_count        = 0;
    int redeem_interval     = 0;

    int* prf_buffer      = new int[elaborated.GetBankCount() * elaborated.GetBankDepth()];


    for (int i = 0; i < c; i++)
    {
        //
        if (!fgr_lifecycle)
        {
            if (fgr_speculative)
                on_flight_fgrs.push_back(fgr);

            fgr = (fgr + 1) & 0x0F;

            fgr_lifecycle   = rand() % 17;
            fgr_speculative = rand() % 2;
        }

        dut_ptr->i_acquire_fgr              = fgr;
        dut_ptr->i_acquire_fgr_speculative  = fgr_speculative;

        //
        if (!acquire_count)
        {
            acquire_count       = (rand() % 8) + 1;
            acquire_interval    = (rand() % 8) + 1;

            dut_ptr->i_acquire_valid = 0;
        }
        else if (acquire_interval)
        {
            acquire_interval--;

            dut_ptr->i_acquire_valid = 0;
        }
        else
        {
            dut_ptr->i_acquire_valid = 1;
        }

        //
        if (!redeem_count)
        {
            if (!on_flight_prfs.empty())
            {
                redeem_count    = (rand() % 8) + 1;
                redeem_interval = (rand() % 8) + 1;
            }

            dut_ptr->i_redeemed_prf     = 0;
            dut_ptr->i_redeemed_valid   = 0;
        }
        else if (redeem_interval)
        {
            redeem_interval--;

            dut_ptr->i_redeemed_prf     = 0;
            dut_ptr->i_redeemed_valid   = 0;
        }
        else if (!on_flight_prfs.empty())
        {
            dut_ptr->i_redeemed_prf     = on_flight_prfs.back();
            dut_ptr->i_redeemed_valid   = 1;
        }
        else
        {
            dut_ptr->i_redeemed_prf     = 0;
            dut_ptr->i_redeemed_valid   = 0;
        }

        //
        if (!fgr_op_count)
        {
            if (!on_flight_fgrs.empty())
            {
                fgr_op_count    = (rand() % 4) + 1;
                fgr_op_interval = (rand() % 4) + 1;
            }

            dut_ptr->i_abandon_fgr      = 0;
            dut_ptr->i_abandon_valid    = 0;

            dut_ptr->i_commit_fgr       = 0;
            dut_ptr->i_commit_valid     = 0;
        }
        else if (fgr_op_interval)
        {
            fgr_op_interval--;

            dut_ptr->i_abandon_fgr      = 0;
            dut_ptr->i_abandon_valid    = 0;

            dut_ptr->i_commit_fgr       = 0;
            dut_ptr->i_commit_valid     = 0;
        }
        else if (!on_flight_fgrs.empty())
        {
            int sel_op  = rand() % 2;
            int sel_fgr = rand() % 2;

            dut_ptr->i_abandon_fgr      =  sel_fgr ? on_flight_fgrs.front() : on_flight_fgrs.back();
            dut_ptr->i_abandon_valid    =  sel_op;

            dut_ptr->i_commit_fgr       =  sel_fgr ? on_flight_fgrs.front() : on_flight_fgrs.back();
            dut_ptr->i_commit_valid     = !sel_op;

            if (sel_fgr)
                on_flight_fgrs.pop_front();
            else
                on_flight_fgrs.pop_back();

            fgr_op_count--;
        }
        else
        {
            dut_ptr->i_abandon_fgr      = 0;
            dut_ptr->i_abandon_valid    = 0;

            dut_ptr->i_commit_fgr       = 0;
            dut_ptr->i_commit_valid     = 0;
        }

        clkn_dumpgen(t);

        if (on_flight_prfs.size() > 64)
        {
            printf("[#1] Wrong state detected at clock edge %d. FIFO data overflow.\n", t);
            error++;

            break;
        }

        if (dut_ptr->o_acquire_ready)
        {
            if (!dut_ptr->i_acquire_fgr_speculative)
            {
                on_flight_prfs.push_front(dut_ptr->o_acquire_prf);
            }
            else if (!elaborated.PushAcquired(dut_ptr->i_acquire_fgr, dut_ptr->o_acquire_prf))
            {
                printf("[#1] Wrong state detected at clock edge %d. Push operation accepted ahead of emulation.\n", t);
                error++;

                break;
            }

            acquire_count--;
            fgr_lifecycle--;
        }

        if (dut_ptr->o_redeemed_ready)
        {
            on_flight_prfs.pop_back();

            redeem_count--;
        }

        // not precisely emulated on Abandon-Popback
        int abandoned_prf;
        if (elaborated.PopAbandoned(abandoned_prf))
        {
            // back into the FIFO, nothing needed to be done for emulation here
        }

        if (dut_ptr->i_abandon_valid)
        {
            elaborated.AbandonFGR(dut_ptr->i_abandon_fgr);
        }

        if (dut_ptr->i_commit_valid)
        {
            int commit_count = elaborated.CommitFGREx(dut_ptr->i_commit_fgr, prf_buffer);

            for (int j = 0; j < commit_count; j++)
                on_flight_prfs.push_back(prf_buffer[j]);
        }

        elaborated.Eval();

        clkp_dumpgen(t);
    }

    //
    dut_ptr->i_redeemed_prf             = 0;
    dut_ptr->i_redeemed_valid           = 0;

    dut_ptr->i_acquire_fgr              = 0;
    dut_ptr->i_acquire_fgr_speculative  = 0;
    dut_ptr->i_acquire_valid            = 0;

    dut_ptr->i_commit_fgr               = 0;
    dut_ptr->i_commit_valid             = 0;

    dut_ptr->i_abandon_fgr              = 0;
    dut_ptr->i_abandon_valid            = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    printf("[#1] \033[1;30mRemaining on-flight PRFs: %lu\033[0m\n", on_flight_prfs.size());

    //
    printf("[#1] \033[1;30mFinalize interval starting at clock edge %d.\033[0m\n", t);

    for (int i = 0; i < 256; i++)
    {
        clkn_dumpgen(t);
        clkp_dumpgen(t);
    }

    printf("[#1] \033[1;30mFinalize interval ending at clock edge %d.\033[0m\n", t);

    //
    bool* prf_slots = new bool[64];

    for (int i = 0; i < 64; i++)
        prf_slots[i] = false;

    list<int>::iterator iter = on_flight_prfs.begin();
    while (iter != on_flight_prfs.end())
        if (prf_slots[*iter])
        {
            printf("[#1] Duplicated PRF detected in on-flight PRF sequence.\n");
            error++;
        }
        else
            prf_slots[*iter++] = true;

    int prf_buffer_count;
    for (int i = 0; i < 16; i++)
    {
        if ((prf_buffer_count = elaborated.CommitFGREx(i, prf_buffer)) != 0)
            for (int j = 0; j < prf_buffer_count; j++)
                if (prf_slots[prf_buffer[j]])
                {
                    printf("[#1] Duplicated PRF detected in checkpoint.\n");
                    error++;
                }
                else
                    prf_slots[prf_buffer[j]] = true;
        else
            continue;
    }

    while (true)
    {
        dut_ptr->i_acquire_fgr              = 0;
        dut_ptr->i_acquire_fgr_speculative  = 0;
        dut_ptr->i_acquire_valid            = 1;

        clkn_dumpgen(t);

        if (!dut_ptr->o_acquire_ready)
            break;

        if (prf_slots[dut_ptr->o_acquire_prf])
        {
            printf("[#1] Duplicated PRF detected in FIFO acquire sequence.\n");
            error++;
        }
        else
            prf_slots[dut_ptr->o_acquire_prf] = true;

        clkp_dumpgen(t);
    }

    clkp_dumpgen(t);

    for (int i = 0; i < 64; i++)
        if (!prf_slots[i])
        {
            printf("[#1] PRF slot %d not covered !!!\n", i);
            error++;
        }

    //
    dut_ptr->i_redeemed_prf             = 0;
    dut_ptr->i_redeemed_valid           = 0;

    dut_ptr->i_acquire_fgr              = 0;
    dut_ptr->i_acquire_fgr_speculative  = 0;
    dut_ptr->i_acquire_valid            = 0;

    dut_ptr->i_commit_fgr               = 0;
    dut_ptr->i_commit_valid             = 0;

    dut_ptr->i_abandon_fgr              = 0;
    dut_ptr->i_abandon_valid            = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    delete[] prf_buffer;
    delete[] prf_slots;

    //
    if (error)
        printf("[#1] Testbench #1 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#1] Testbench #1 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

void test()
{
#ifndef RANDOM_SEQUENCE_SEED
    unsigned long int seed = time(0);
#else
    unsigned long int seed = RANDOM_SEQUENCE_SEED;
#endif

    srand(seed);

    int t = 0;
    int e = 0;

    printf("[--] ----------------------------------------\n");

    printf("[##] Testing on module '\033[1;33missue_rat_freelist\033[0m'\n");
    printf("[##] \033[1;30mRandom sequence seed: %lu\033[0m\n", seed);

    printf("[--] ----------------------------------------\n");

    reset(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_0(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_1(t);

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
    dut_ptr = new Vsim_rat_freelist;
    printf("\033[1;33mRAT Freelist of Issue Stage module 'issue_rat_freelist' selected.\033[0m\n");

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
