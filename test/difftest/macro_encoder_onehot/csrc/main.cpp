#include "verilated.h"
#include <cstdio>

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#include "Vsim_macro_encoder_onehot.h"
static Vsim_macro_encoder_onehot* dut_ptr;

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
    for (int i = 0; i < 65536; i++)
    {
        int j;
        for (j = 0; j < 16; j++)
            if (i >> j)
                continue;
            else
                break;

        j--;

        bool onehot = i && (i & (i - 1)) == 0;

        dut_ptr->d = i;

        comb_dumpgen(t);

        if (dut_ptr->valid != onehot)
        {
            printf("[#0] DUT 'macro_encoder_onehot_detect' differs at %d.\n", t);

            error++;
        }

        if (onehot)
        {
            if (dut_ptr->q != j)
            {
                printf("[#0] DUT 'macro_encoder_onehot_bin' differs at %d (%d, %d).\n", t, dut_ptr->q, j);

                error++;
            }
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

    printf("[##] Testing on MACRO '\033[1;33mmacro_encoder_onehot_bin\033[0m'\n");
    printf("[##] Testing on MACRO '\033[1;33mmacro_encoder_onehot_detect\033[0m'\n");

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
    dut_ptr = new Vsim_macro_encoder_onehot;
    printf("\033[1;33mOne-hot binary encoder MACRO 'macro_encoder_onehot_bin' selected.\033[0m\n");
    printf("\033[1;33mOne-hot detector MACRO 'macro_encoder_onehot_detect' selected.\033[0m\n");

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
