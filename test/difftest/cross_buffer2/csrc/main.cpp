#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

using namespace std;

//#define CROSS_BUFFER_PRIORITY

#define GEN(p, t) (p | (t << 8) | (t << 16) | (t << 24))
#define PRT(t)    (t & 0x00FF) 

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#ifdef CROSS_BUFFER_PRIORITY
#include "Vsim_cross_buffer2_priority.h"
static Vsim_cross_buffer2_priority* dut_ptr;
#else
#include "Vsim_cross_buffer2.h"
static Vsim_cross_buffer2* dut_ptr;
#endif

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

    dut_ptr->resetn = 1;
}

void testbench_0(int& t)
{
    int passed = 1;

    printf("[#0] Testbench #0\n");

    //
    int gen_t_p0 = 0;
    int gen_t_p1 = 0;

    int prev0_nready = 0;
    int prev1_nready = 0;
    int next_nready  = 0;

    int complete_p0 = 0;
    int complete_p1 = 0;

    list<int> records_p0;
    list<int> records_p1;

    // zero beat
    dut_ptr->prev0_i_data  = 0;
    dut_ptr->prev0_i_valid = 0;

    dut_ptr->prev1_i_data  = 0;
    dut_ptr->prev1_i_valid = 0;

    dut_ptr->next_i_ready  = 0;

    clk_dumpgen(t);

    printf("[#0] Generating record sequence ...\n");

    //
    while (1)
    {
        // Next
        if (dut_ptr->next_i_ready && dut_ptr->next_o_valid)
        {
            next_nready = rand() % 4;

            //printf("[%08x]", dut_ptr->next_o_data);

            if (PRT(dut_ptr->next_o_data) == 0)
            {
                if (!complete_p0)
                {
                    records_p0.push_back(dut_ptr->next_o_data);
                            
                    complete_p0 = gen_t_p0 == 0x100;
                }
                else
                    printf("[#0] Dataflow corruption detected. Received new data from PORT 0 after dataflow finished.\n");
            }
            else if (PRT(dut_ptr->next_o_data) == 1)
            {
                    
                if (!complete_p1)
                {
                    records_p1.push_back(dut_ptr->next_o_data);

                    complete_p1 = gen_t_p1 == 0x100;
                }
                else
                    printf("[#0] Dataflow corruption detected. Received new data from PORT 1 after dataflow finished.\n");
            }
            else
                printf("[#0] Testbench fatal failure (Unknown PRT [%d]).\n", PRT(dut_ptr->next_o_data));

            dut_ptr->next_i_ready = 0;
        }

        if (next_nready == 0)
        {
            dut_ptr->next_i_ready = 1;
        }
        else
        {
            dut_ptr->next_i_ready = 0;

            next_nready--;
        }

        clkp_dumpgen(t);

        // Prev P0
        if (gen_t_p0 == 0x100)
        {
            dut_ptr->prev0_i_data  = 0;
            dut_ptr->prev0_i_valid = 0;
        }
        else if (prev0_nready == 0)
        {
            dut_ptr->prev0_i_data  = GEN(0, gen_t_p0);
            dut_ptr->prev0_i_valid = 1;

            if (dut_ptr->prev0_o_ready)
            {
                prev0_nready = rand() % 4;
                gen_t_p0++;
            }
        }
        else
        {
            dut_ptr->prev0_i_data  = 0xDEADBEEF;
            dut_ptr->prev0_i_valid = 0;

            prev0_nready--;
        }

        // Prev P1
        if (gen_t_p1 == 0x100)
        {
            dut_ptr->prev1_i_data  = 0;
            dut_ptr->prev1_i_valid = 0;
        }
        else if (prev1_nready == 0)
        {
            dut_ptr->prev1_i_data  = GEN(1, gen_t_p1);
            dut_ptr->prev1_i_valid = 1;

            if (dut_ptr->prev1_o_ready)
            {
                prev1_nready = rand() % 4;
                gen_t_p1++;
            }
        }
        else
        {
            dut_ptr->prev1_i_data  = 0xDEADBEEF;
            dut_ptr->prev1_i_valid = 0;

            prev1_nready--;
        }

        clkn_dumpgen(t);

        if (complete_p0 && complete_p1)
            break;
    }

    //
    if (records_p0.size() == 0x100)
        printf("[#0] Record sequence from PORT 0 size matched.\n");
    else
    {
        printf("[#0] Record sequence from PORT 0 size unmatched. Result: %ld, Ref: %d.\n", records_p0.size(), 0x100);
        passed = 0;
    }

    //
    if (records_p1.size() == 0x100)
        printf("[#0] Record sequence from PORT 1 size matched.\n");
    else
    {
        printf("[#0] Record sequence from PORT 1 size unmatched. Result: %ld, Ref: %d.\n", records_p1.size(), 0x100);
        passed = 0;
    }

    //
    int pattern_unmatch_p0 = 0;
    int cti_p0 = 0;

    list<int>::iterator iter_p0 = records_p0.begin();
    
    while (iter_p0 != records_p0.end())
    {
        if (*iter_p0 != GEN(0, cti_p0))
        {
            printf("[#0] Pattern of PORT 0 unmatched at %3d. Result: %x, Ref: %x\n", cti_p0, *iter_p0, GEN(0, cti_p0));

            pattern_unmatch_p0++;
        }

        cti_p0++;
        iter_p0++;
    }

    if (!pattern_unmatch_p0)
        printf("[#0] Pattern test of PORT 0 Passed.\n");
    else
    {
        printf("[#0] Pattern test of PORT 0 Failed. %d unmatched record(s) found.\n", pattern_unmatch_p0);
        passed = 0;
    }

    //
    int pattern_unmatch_p1 = 0;
    int cti_p1 = 0;

    list<int>::iterator iter_p1 = records_p1.begin();

    while (iter_p1 != records_p1.end())
    {
        if (*iter_p1 != GEN(1, cti_p1))
        {
            printf("[#0] Pattern of PORT 1 unmatched at %3d. Result: %x, Ref: %x\n", cti_p1, *iter_p1, GEN(1, cti_p1));

            pattern_unmatch_p1++;
        }

        cti_p1++;
        iter_p1++;
    }

    if (!pattern_unmatch_p1)
        printf("[#0] Pattern test of PORT 1 Passed.\n");
    else
    {
        printf("[#0] Pattern test of PORT 1 Failed. %d unmatched record(s) found.\n", pattern_unmatch_p1);
        passed = 0;
    }

    //
    if (passed)
        printf("[#0] [PASSED] Testbench #0 Passed !!!\n");
    else
        printf("[#0] [FAILED] Testbench #0 Failed !!!\n");
}

void test()
{
    int time = 0;

    printf("[--] ----------------------------------------\n");

#ifdef CROSS_BUFFER_PRIORITY
    printf("[##] Testing on module \'common_cross_buffer2_priority\'.\n");
#else
    printf("[##] Testing on module \'common_cross_buffer2\'.\n");
#endif

    printf("[--] ----------------------------------------\n");

    printf("[##] Circuit reset.\n");

    reset(time);

    printf("[--] ----------------------------------------\n");

    testbench_0(time);

    printf("[--] ----------------------------------------\n");
}

int main()
{

#ifdef CROSS_BUFFER_PRIORITY
    dut_ptr = new Vsim_cross_buffer2_priority;
    printf("[##] Priority 2-to-1 Cross Buffer module 'common_cross_buffer2_priority' selected.\n");
#else
    dut_ptr = new Vsim_cross_buffer2;
    printf("[##] Round-Robin 2-to-1 Cross Buffer module 'common_cross_buffer2' selected.\n");
#endif

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
    srand(time(0));
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

