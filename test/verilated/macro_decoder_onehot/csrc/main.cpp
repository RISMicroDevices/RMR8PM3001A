#include "verilated.h"
#include <cstdio>

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#include "Vsim_macro_decoder_onehot.h"
static Vsim_macro_decoder_onehot* dut_ptr;

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
    for (int i = 0; i < 256; i++)
    {
        dut_ptr->d = i;

        comb_dumpgen(t);

        int q = -1;
        for (int j = 0; j < 256; j++)
        {
            if ((dut_ptr->q[j / 32] >> (j % 32)) & 0x01)
            {
                if (q != -1)
                {
                    printf("[#0] Multiple bit asserted on clk %d\n", t);

                    error++;
                }
                else
                    q = j;
            }
        }
        
        if (q != i)
        {
            printf("[#0] Wrong output detected on clk %d (Input: %d, Output: q[%d])\n", t, i, q);

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

    printf("[##] Testing on MACRO '\033[1;33mmacro_decoder_onehot_bin\033[0m'\n");

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
    dut_ptr = new Vsim_macro_decoder_onehot;
    printf("\033[1;33mOne-hot binary decoder MACRO 'macro_decoder_onehot_bin' selected.\033[0m\n");

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
