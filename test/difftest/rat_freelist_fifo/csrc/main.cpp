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

#include "Vsim_rat_freelist_fifo.h"
static Vsim_rat_freelist_fifo* dut_ptr;

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

    //
    if (!dut_ptr->o_acquire_valid)
    {
        printf("[#0] Wrong state detected. 'o_acquire_valid' port not asserted.\n");
        error++;
    }

    printf("[#0] \033[1;30mPost-reset PRF value: 0x%02x\033[0m\n", dut_ptr->o_acquire_prf);

    // zero beat
    dut_ptr->i_abandoned_prf    = 0;
    dut_ptr->i_abandoned_valid  = 0;

    dut_ptr->i_redeemed_prf     = 0;
    dut_ptr->i_redeemed_valid   = 0;

    dut_ptr->i_acquire_ready    = 0;

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
    // Post-reset saturated pop-off test.
    printf("[#1] Testbench #1\n");
    printf("[#1] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#1] Post-reset saturated pop-off test.\n");

    //
    bool prf_slot[64] = { 0 };
    int  prf_parity   = dut_ptr->o_acquire_prf & 0x01;

    prf_slot[dut_ptr->o_acquire_prf] = true;

    dut_ptr->i_acquire_ready = 1;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    for (int i = 1; i < 64; i++)
    {
        //
        if (!dut_ptr->o_acquire_valid)
        {
            printf("[#1] Wrong state detected. FIFO underflow at slot %d.\n", i);
            error++;
            break;
        }

        prf_parity = !prf_parity;

        if ((dut_ptr->o_acquire_prf & 0x01) != prf_parity)
            printf("[#1] \033[1;30mPRF parity sequence not satisfied at slot %d. LRU not working maybe.\033[0m\n", i);

        prf_slot[dut_ptr->o_acquire_prf] = true;

        //
        dut_ptr->i_acquire_ready = 1;

        clkn_dumpgen(t);
        clkp_dumpgen(t);
    }

    //
    if (dut_ptr->o_acquire_valid)
    {
        printf("[#1] Wrong state detected. FIFO overflow.\n");
        error++;
    }

    for (int i = 0; i < 64; i++)
        if (!prf_slot[i])
        {
            printf("[#1] PRF slot %d NOT covered.\n", i);
            error++;
        }

    //
    dut_ptr->i_acquire_ready = 0;

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
    // Saturated redeem and pop-off test.
    printf("[#2] Testbench #2\n");
    printf("[#2] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#2] Saturated redeem and pop-off test.\n");

    //
    for (int i = 0; i < 64; i++)
    {
        dut_ptr->i_redeemed_prf     = i;
        dut_ptr->i_redeemed_valid   = 1;

        clkn_dumpgen(t);

        if (!dut_ptr->o_redeemed_ready)
        {
            printf("[#2] Wrong state detected. Instant response not asserted.\n");
            error++;
        }

        clkp_dumpgen(t);
    }

    if (dut_ptr->o_redeemed_ready)
    {
        printf("[#2] Wrong state detected. Imcomplete FIFO write.\n");
        error++;
    }

    //
    dut_ptr->i_redeemed_prf     = 0;
    dut_ptr->i_redeemed_valid   = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    bool prf_slot[64] = { 0 };
    int  prf_parity   = dut_ptr->o_acquire_prf & 0x01;

    prf_slot[dut_ptr->o_acquire_prf] = true;

    dut_ptr->i_acquire_ready = 1;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    for (int i = 1; i < 64; i++)
    {
        //
        if (!dut_ptr->o_acquire_valid)
        {
            printf("[#2] Wrong state detected. FIFO underflow at slot %d.\n", i);
            error++;
            break;
        }

        prf_parity = !prf_parity;

        if ((dut_ptr->o_acquire_prf & 0x01) != prf_parity)
            printf("[#2] \033[1;30mPRF parity sequence not satisfied at slot %d. LRU not working maybe.\033[0m\n", i);

        prf_slot[dut_ptr->o_acquire_prf] = true;

        //
        dut_ptr->i_acquire_ready = 1;

        clkn_dumpgen(t);
        clkp_dumpgen(t);
    }

    //
    if (dut_ptr->o_acquire_valid)
    {
        printf("[#2] Wrong state detected. FIFO overflow.\n");
        error++;
    }

    for (int i = 0; i < 64; i++)
        if (!prf_slot[i])
        {
            printf("[#2] PRF slot %d NOT covered.\n", i);
            error++;
        }

    //
    dut_ptr->i_acquire_ready = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

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
    // Random push (redeem & abandon) and pop-off test.
    printf("[#3] Testbench #3\n");
    printf("[#3] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#3] Random push (redeem & abandon) and pop-off test.\n");

    // TODO

    //
    if (error)
        printf("[#3] Testbench #3 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#3] Testbench #3 \033[1;32mPASSED\033[0m !!!\n");

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

    reset(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_3(t);

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
    dut_ptr = new Vsim_rat_freelist_fifo;
    printf("\033[1;33mRAT Freelist FIFO of Issue Stage module 'issue_rat_freelist_fifo' selected.\033[0m\n");

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
