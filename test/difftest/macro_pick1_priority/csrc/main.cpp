#include "verilated.h"
#include <cstdio>

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#include "Vsim_macro_pick1_priority.h"
static Vsim_macro_pick1_priority* dut_ptr;

void comb_dumpgen(int& time)
{
    dut_ptr->eval();

#ifdef VM_TRACE
    fp->dump(time++);
#endif
}

int testbench_0(int& t)
{
    int error = 0;

    // Testbench #0
    // Behavioural differential test..
    printf("[#0] Testbench #0\n");
    printf("[#0] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#0] Behavioural differential test.\n");

    //
    int c = 65535;

    printf("[#1] \033[1;30mDifferential payload count: %d\033[0m\n", c);

    //
    for (int i = 0; i < c; i++)
    {
        int val = rand();

        dut_ptr->dvalid = val;

        //
        comb_dumpgen(t);

        //
        int j = val;
        int k = 0;

        for (; j; k++)
        {
            if (j & 0x01)
                break;

            j = (unsigned)j >> 1;
        }

        if (val)
        {
            if (dut_ptr->qaddr != k)
            {
                printf("[#1] DUT differs at clk %d. Wrong 'qaddr' (%x, %x).\n",
                    t, dut_ptr->qaddr, k);
                error++;
            }
        }
        else if (dut_ptr->qvalid)
        {
            printf("[#1] DUT differs at clk %d. 'qvalid' asserted.\n", t);
            error++;
        }
    }

    //
    if (error)
        printf("[#0] Testbench #0 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#0] Testbench #0 \033[1;32mPASSED\033[0m !!!\n");

    return error;
}

void test()
{
    int t = 0;
    int e = 0;

    printf("[--] ----------------------------------------\n");

    printf("[##] Testing on MACRO '\033[1;33mmacro_pick1_priority_bin\033[0m'\n");

    printf("[--] ----------------------------------------\n");

    printf("[##] Combinational circuit test.\n");

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
    dut_ptr = new Vsim_macro_pick1_priority;
    printf("\033[1;33mWire priority one-pick MACRO 'macro_pick1_priority_bin' selected.\033[0m\n");

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
