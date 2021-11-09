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

    // TODO

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

void test()
{
    srand(time(0));

    int t = 0;
    int e = 0;

    printf("[--] ----------------------------------------\n");

    printf("[##] Testing on module '\033[1;33missue_rat_freelist\033[0m'\n");

    printf("[--] ----------------------------------------\n");

    reset(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_0(t);

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
