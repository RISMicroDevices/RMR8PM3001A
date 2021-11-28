#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

#include "common.hpp"

using namespace std;

using namespace MEMU::Common;

//#define DIFF_DEBUG

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#include "Vsim_common_pseudo_lru_pick.h"
static Vsim_common_pseudo_lru_pick* dut_ptr;

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
    dut_ptr->waddr  = 0;
    dut_ptr->wen    = 0;
    dut_ptr->dvalid = 1;

    //
    clkn_dumpgen(t);

    if (dut_ptr->qaddr != 0)
    {
        printf("[#0] Wrong state detected. First 'qaddr' not zero.\n");
        error++;
    }

    //
    clkp_dumpgen(t);

    //
    dut_ptr->dvalid = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    if (error)
        printf("[#0] Testbench #0 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#0] Testbench #0 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

int testbench_1(int &t)
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
    constexpr int lru_size_log2 = 5;

    PseudoLRUPick<lru_size_log2> emulated;
    
    //
    for (int i = 0; i < c; i++)
    {
        //
        int update = rand() & 0x1F;
        int valid  = rand();

        for (int j = 1; j < 4; j++)
            valid ^= rand() << (j << 3);

        dut_ptr->waddr  = update;
        dut_ptr->wen    = 1;
        dut_ptr->dvalid = valid;

        emulated.Update(update);

        unsigned int j = valid;
        for (int k = 0; j; k++)
        {
            if (j & 0x01)
                emulated.SetValid(k);

            j >>= 1;
        }

        //
        clkn_dumpgen(t);

        //
        int dut_picked      = dut_ptr->qaddr;
        int emulated_picked = emulated.GetPicked();

        if (emulated_picked < 0)
        {
            if (dut_ptr->qvalid)
            {
                printf("[#1] DUT differs at clk %d on 'qvalid' (%x, %x).\n", t, dut_ptr->qvalid, 0);
                error++;
            }
        }
        else if (dut_picked != emulated_picked)
        {
            printf("[#1] DUT differs at clk %d (%x, %x). Valid mask: %08x.\n", t, dut_picked, emulated_picked, valid);
            error++;
        }

        //
        clkp_dumpgen(t);

        //
        emulated.Eval();
    }

    //
    dut_ptr->waddr  = 0;
    dut_ptr->wen    = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    if (error)
        printf("[#1] Testbench #1 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#1] Testbench #1 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

void test()
{
    srand(time(0));

    int t = 0;
    int e = 0;

    printf("[--] ----------------------------------------\n");

    printf("[##] Testing on module '\033[1;33mcommon_pseudo_lru_pick_binwr\033[0m'\n");

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
    dut_ptr = new Vsim_common_pseudo_lru_pick;
    printf("\033[1;33mPseudo LRU pick algorithm module 'common_pseudo_lru_pick_binwr' selected.\033[0m\n");

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


