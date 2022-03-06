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

#include "Vsim_issue_prf.h"
static Vsim_issue_prf* dut_ptr;

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
    if (dut_ptr->doutb != 0)
    {
        printf("[#0] Wrong state detected. 'doutb' not zero.\n");
        error++;
    }

    if (dut_ptr->doutc != 0)
    {
        printf("[#0] Wrong state detected. 'doutc' not zero.\n");
        error++;
    }

    // zero beat
    dut_ptr->addra  = 0;
    dut_ptr->wea    = 0;
    dut_ptr->addrb  = 0;
    dut_ptr->addrc  = 0;

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
    PhysicalRegisterFile emulated;

    for (int i = 0; i < c; i++)
    {
        //
        int     w_en    = rand() % 2;
        int64_t w_value = rand();
        int     w_addr  = rand() & 0x3F;

        int     r_addr0 = rand() & 0x3F;
        int     r_addr1 = rand() & 0x3F;

        dut_ptr->wea    = w_en;
        dut_ptr->dina   = w_value;
        dut_ptr->addra  = w_addr;

        dut_ptr->addrb  = r_addr0;
        dut_ptr->addrc  = r_addr1;

        //
        clkn_dumpgen(t);

        if (w_en)
            emulated.Set(w_addr, w_value);

        //
        if (dut_ptr->doutb != emulated.Get(r_addr0))
        {
            printf("[#1] DUT differs at clk %d. Reading PRF on port B at %d (%lx, %lx).\n",
                t, r_addr0, dut_ptr->doutb, emulated.Get(r_addr0));
            error++;
        }

        if (dut_ptr->doutc != emulated.Get(r_addr1))
        {
            printf("[#1] DUT differs at clk %d. Reading PRF on port C at %d (%lx, %lx).\n",
                t, r_addr1, dut_ptr->doutc, emulated.Get(r_addr1));
            error++;
        }

        //
        clkp_dumpgen(t);

        emulated.Eval();
    }

    //
    dut_ptr->addra  = 0;
    dut_ptr->wea    = 0;
    dut_ptr->addrb  = 0;
    dut_ptr->addrc  = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    if (error)
        printf("[#1] Testbench #1 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#1] Testbench #1 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

//
void test()
{
    srand(time(0));

    int t = 0;
    int e = 0;

    printf("[--] ----------------------------------------\n");

    printf("[##] Testing on module '\033[1;33missue_prf\033[0m'\n");

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
    dut_ptr = new Vsim_issue_prf;
    printf("\033[1;33mPhysical Register File module 'issue_prf' selected.\033[0m\n");

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

