#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

using namespace std;

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

    // TODO

    //
    if (error)
        printf("[#4] Testbench #4 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#4] Testbench #4 \033[1;32mPASSED\033[0m !!!\n");

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

