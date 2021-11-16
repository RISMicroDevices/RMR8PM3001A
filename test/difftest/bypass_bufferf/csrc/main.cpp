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

#define GEN(i) ((i) | ((i) << 8) | ((i) << 16) | ((i) << 24))

#include "Vsim_bypass_bufferf.h"
static Vsim_bypass_bufferf* dut_ptr;

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
    // Verify on post-reset state
    printf("[#0] Testbench #0\n");
    printf("[#0] Verify on post-reset state\n");

    //
    if (!dut_ptr->prev_o_ready)
    {
        printf("[#0] Wrong state detected. 'prev_o_ready' not asserted.\n");
        error++;
    }

    if (dut_ptr->next_o_valid)
    {
        printf("[#0] Wrong state detected. 'next_o_valid' asserted.\n");
        error++;
    }

    // zero beat
    dut_ptr->flush          = 0;
    dut_ptr->prev_i_data    = 0;
    dut_ptr->prev_i_valid   = 0;
    dut_ptr->next_i_ready   = 0;

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
    printf("[#1] Mixed emulation random differential test\n");

    //
    int c = 256;

    printf("[#1] \033[1;30mDifferential payload count: %d\033[0m\n", c);

    //
    FlushableBypassBuffer<int> emulated;

    for (int i = 0; i < c; i++)
    {
        int gen = GEN(i & 0xFF);

        int w_valid =   rand() % 2;
        int r_ready =   rand() % 2;
        int flush   = !(rand() % 8);

        dut_ptr->prev_i_data    = gen;
        dut_ptr->prev_i_valid   = w_valid;
        dut_ptr->next_i_ready   = r_ready;
        dut_ptr->flush          = flush;

        if (w_valid)
            emulated.SetInput(gen);

        if (flush)
            emulated.Flush();

        clkn_dumpgen(t);

        int  dut_read_data  = dut_ptr->next_o_data;
        bool dut_read_valid = dut_ptr->next_o_valid;

        int  emulated_read_data;
        bool emulated_read_valid;

        if (r_ready)
            emulated_read_valid = emulated.ReadOutput(&emulated_read_data);
        else
            emulated_read_valid = emulated.GetOutput(&emulated_read_data);

        if (dut_read_valid != emulated_read_valid)
        {
            printf("[#1] DUT 'next_o_valid' differs from Emulation at clk %d (%x, %x).\n",
                t, dut_read_valid, emulated_read_valid);
            error++;
        }
        else if (dut_read_valid)
        {
            if (dut_read_data != emulated_read_data)
            {
                printf("[#1] DUT 'next_o_data' differs from Emulation at clk %d (%x, %x).\n",
                    t, dut_read_data, emulated_read_data);
                error++;
            }
        }

        clkp_dumpgen(t);

        emulated.Eval();
        emulated.ResetInput();
    }

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

    printf("[##] Testing on module '\033[1;33mcommon_bypass_bufferf\033[0m'\n");

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
    dut_ptr = new Vsim_bypass_bufferf;
    printf("\033[1;33mFlushable Bypass Buffer module 'common_bypass_bufferf' selected.\033[0m\n");

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
