#include "verilated.h"
#include "Vsim_common_bypass_buffer.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

#include "common.hpp"

using namespace std;

using namespace MEMU::Common;

#define GEN(i) ((i) | ((i) << 8) | ((i) << 16) | ((i) << 24))

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

Vsim_common_bypass_buffer* dut_ptr;

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

// Circuit reset
void reset(int &time)
{
    dut_ptr->resetn = 0;

    clk_dumpgen(time);
    clkp_dumpgen(time);

    dut_ptr->resetn = 1;
}

// Testbench #0
// Next and prev pipelines are always ready, testing bypass
void testbench_0(int &t)
{
    int passed = 1;

    clkn_dumpgen(t);

    printf("[#0] Testbench #0\n");
    printf("[#0] Next and prev pipelines are always ready, testing bypass ...\n");

    for (int i = 0; i < 0x100; i++)
    {
        int gen = GEN(i);

        dut_ptr->prev_i_data  = gen;
        dut_ptr->prev_i_valid = 1;

        dut_ptr->next_i_ready = 1;

        clk_dumpgen(t);

        if (!dut_ptr->prev_o_ready)
            passed = 0;

        if (!dut_ptr->next_o_valid)
            passed = 0;

        if (dut_ptr->next_o_data != gen)
            passed = 0;
    }

    if (passed)
        printf("[#0] [PASSED] Testbench #0 Passed !!!\n");
    else
        printf("[#0] [FAILED] Testbench #0 Failed !!!\n");
}

// Testbench #1
// Next pipeline asserting not ready for random cycles
void testbench_1(int &t)
{
    int passed = 1;

    printf("[#1] Testbench #1\n");
    printf("[#1] Next pipeline asserting not ready for random cycles ...\n");

    list<int> records;
    vector<int> delays;

    srand(time(0));

    int gen = GEN(0);
    int nready = -1;

    printf("[#1] Generating record sequence ...\n");

    dut_ptr->prev_i_data = gen;
    dut_ptr->prev_i_valid = 1;

    dut_ptr->next_i_ready = 1;

    clkp_dumpgen(t);

    for (int i = 1; i < 0x100; )
    {
        if (dut_ptr->next_i_ready && dut_ptr->next_o_valid)
            records.push_back(dut_ptr->next_o_data);

        gen = GEN(i);

        if (nready < 0)
        {
            nready = rand() % 8;

            delays.push_back(nready);
        }

        dut_ptr->prev_i_data = gen;
        dut_ptr->prev_i_valid = 1;

        if (nready-- > 0)
            dut_ptr->next_i_ready = 0;
        else 
        {
            dut_ptr->next_i_ready = 1;
            i++;
        }

        clkn_dumpgen(t);
        clkp_dumpgen(t);
    }

    clk_dumpgen(t);

    if (dut_ptr->next_i_ready && dut_ptr->next_o_valid)
        records.push_back(dut_ptr->next_o_data);

    printf("[#1] Verifying record sequence ...\n");

    if (records.size() == 0x100)
        printf("[#1] Record sequence size matched.\n");
    else
    {
        printf("[#1] Record sequence size unmatched. Result: %ld, Ref: %d\n", records.size(), 0x100);
        passed = 0;
    }

    int pattern_unmatch = 0;
    int cti = 0;
    list<int>::iterator iter = records.begin();

    while (iter != records.end())
    {
        if (*iter != GEN(cti))
        {
            printf("[#1] Pattern unmatched at %3d. Delay: %d, Result: %x, Ref: %x\n", cti, delays[cti - 1], *iter, GEN(cti));

            pattern_unmatch++;
            passed = 0;
        }

        cti++;
        iter++;
    }
    
    if (!pattern_unmatch)
        printf("[#1] Pattern test Passed.\n");
    else
    {
        printf("[#1] Pattern test Failed. %d unmatched record(s) found.\n", pattern_unmatch);
        passed = 0;
    }

    if (passed)
        printf("[#1] [PASSED] Testbench #1 Passed !!!\n");
    else
        printf("[#1] [FAILED] Testbench #1 Failed !!!\n");
}

// Testbench #2
// Mixed emulation random input differential test.
void testbench_2(int& t)
{
    int passed = 1;

    //
    printf("[#2] Testbench #2\n");
    printf("[#2] Mixed emulation random input differential test.\n");

    //
    int c = 65535;
    
    printf("[#2] Configurated differential payload: %d\n", c);

    //
    dut_ptr->prev_i_data  = 0;
    dut_ptr->prev_i_valid = 0;

    dut_ptr->next_i_ready = 0;

    clk_dumpgen(t);
    clkp_dumpgen(t);

    //
    NormalBypassBuffer<int> emulated;

    for (int i = 0; i < c; i++)
    {
        int g = GEN(i & 0xFF);

        int w_valid = rand() % 2;
        int r_ready = rand() % 2;

        dut_ptr->prev_i_data  = g;
        dut_ptr->prev_i_valid = w_valid;
        dut_ptr->next_i_ready = r_ready;

        if (w_valid)
            emulated.SetInput(g);

        clkn_dumpgen(t);

        int  emulated_read;
        bool emulated_read_valid;

        if (r_ready)
            emulated_read_valid = emulated.ReadOutput(&emulated_read);
        else
            emulated_read_valid = emulated.GetOutput(&emulated_read);

        int  dut_read        = dut_ptr->next_o_data;
        bool dut_read_valid  = dut_ptr->next_o_valid;

        if (dut_read_valid != emulated_read_valid)
        {
            printf("[#2] DUT differs from Emulation at clk %d (signal 'next_o_valid', %x, %x).\n",
                t, dut_read_valid, emulated_read_valid);

            passed = 0;
        }
        else if (dut_read_valid)
        {
            if (dut_read != emulated_read)
            {
                printf("[#2] DUT differs from Emulation at clk %d (signal 'next_o_data', %x, %x).\n", 
                    t, dut_read, emulated_read);

                passed = 0;
            }
        }

        clkp_dumpgen(t);

        emulated.Eval();
        emulated.ResetInput();
    }

    //
    if (passed)
        printf("[#2] [PASSED] Testbench #2 Passed !!!\n");
    else
        printf("[#2] [FAILED] Testbench #2 Failed !!!\n");
}

// Overall payload
void test() 
{
    int time = 0;

    printf("[--] ----------------------------------------\n");

    printf("[##] Testing on module \'common_bypass_buffer\'.\n");

    printf("[--] ----------------------------------------\n");

    printf("[##] Circuit reset.\n");

    reset(time);

    printf("[--] ----------------------------------------\n");

    testbench_0(time);

    printf("[--] ----------------------------------------\n");

    testbench_1(time);

    printf("[--] ----------------------------------------\n");

    printf("[##] Circuit reset.\n");

    reset(time);

    printf("[--] ----------------------------------------\n");

    testbench_2(time);

    printf("[--] ----------------------------------------\n");

    printf("[##] End of all testbench.\n");

    printf("[--] ----------------------------------------\n");
}

int main() 
{
    dut_ptr = new Vsim_common_bypass_buffer;

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

    printf("Finalized.\n");

    return 0;
}