#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

#include "common.hpp"

using namespace std;

using namespace MEMU::Common;

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#include "Vsim_dffram_1a.h"
static Vsim_dffram_1a* dut_ptr;

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

//
int testbench_0(int& t)
{
    int error = 0;

    // Testbench #0
    // Verify post-reset state.
    printf("[#0] Testbench #0\n");
    printf("[#0] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#0] Verify on post-reset state.\n");

    // zero beat
    dut_ptr->addr   = 0;
    dut_ptr->en     = 0;
    dut_ptr->we     = 0;
    dut_ptr->din    = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    // scan memory
    for (int i = 0; i < 32; i++)
    {
        dut_ptr->addr = i;

        clkn_dumpgen(t);

        if (dut_ptr->dout)
        {
            printf("[#0] Not zero after reset at address 0x%08x.\n", i);

            error++;
        }

        clkp_dumpgen(t);
    }

    // scan coupled port
    for (int i = 0; i < 32; i++)
    {
        if (dut_ptr->tdout[i])
        {
            printf("[#0] Not zero (data-coupled port) after reset at address 0x%08x.\n", i);

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

int testbench_1(int& t)
{
    int error = 0;

    // Testbench #1
    // Random write emulated differential test.
    printf("[#1] Testbench #1\n");
    printf("[#1] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#1] Random write emulated differential test.\n");

    //
    int c = 65535;

    printf("[#1] \033[1;30mDifferential payload count: %d\033[0m\n", c);

    int size = 32;

    printf("[#1] \033[1;30mConfigurated memory capacity: %d\033[0m\n", size);

    //
    W1RTRandomAccessMemory<int>* emulated
        = new W1RTRandomAccessMemory<int>(size, MemoryReadMode::READ_FIRST);

    for (int i = 0; i < c; i++)
    {
        int wen   = rand() % 2;
        int waddr = rand() % 32;
        int wdata = rand() | (rand() << 16);

        dut_ptr->addr = waddr;
        dut_ptr->en   = 1;
        dut_ptr->we   = wen;
        dut_ptr->din  = wdata;

        if (wen)
            emulated->SetWrite(waddr, wdata);

        clkn_dumpgen(t);

        // scan coupled memory
        for (int j = 0; j < size; j++)
        {
            int emu_read;
            int dut_read;

            emulated->ReadThrough(j, &emu_read);

            dut_read = dut_ptr->tdout[j];

            if (dut_read != emu_read)
            {
                printf("[#1] \033[1;31mMemory content differs\033[0m at address 0x%08x.\n", j);
                printf("[#1] \033[1;31mDumping memory contents\033[0m ...\n");

                // dump to console
                printf("[#1] Memory content of DUT:\n");
                for (int dump_k = 0; dump_k < size; dump_k++)
                {
                    int dut_dump;
                    int emu_dump;

                    emulated->ReadThrough(dump_k, &emu_dump);

                    dut_dump = dut_ptr->tdout[dump_k];

                    if (!(dump_k & 0x0007))
                    {
                        if (dump_k)
                            printf("\n");
                        
                        printf("[#1] 0x%08x ", dump_k);
                    }

                    if (dut_dump != emu_dump)
                        printf("\033[1;31m%08x \033[0m", dut_dump);
                    else
                        printf("%08x ", dut_dump);
                }

                printf("\n");

                printf("[#1] Memory content of Emulation:\n");
                for (int dump_k = 0; dump_k < size; dump_k++)
                {
                    int dut_dump;
                    int emu_dump;

                    emulated->ReadThrough(dump_k, &emu_dump);

                    dut_dump = dut_ptr->tdout[dump_k];

                    if (!(dump_k & 0x0007))
                    {
                        if (dump_k)
                            printf("\n");
                        
                        printf("[#1] 0x%08x ", dump_k);
                    }

                    if (dut_dump != emu_dump)
                        printf("\033[1;31m%08x \033[0m", emu_dump);
                    else
                        printf("%08x ", emu_dump);
                }

                printf("\n");

                error++;
                break;
            }
        }

        clkp_dumpgen(t);

        emulated->Eval();
    }

    //
    delete emulated;

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

    printf("[##] Testing on module '\033[1;33mcommon_dffram_1a1w1r\033[0m'\n");

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
    dut_ptr = new Vsim_dffram_1a;
    printf("\033[1;33mDFF-base RAM module 'common_dffram_1a1w1r' selected.\033[0m\n");

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

