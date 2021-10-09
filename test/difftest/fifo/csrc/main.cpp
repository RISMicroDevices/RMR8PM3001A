#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

using namespace std;

//#define FIFO_RAM_BASE

#define GEN(p, t) (p | (t << 8) | (t << 16) | (t << 24))
#define PRT(t)    (t & 0x00FF) 

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#ifdef FIFO_RAM_BASE
#include "Vsim_fifo_ram_1w1r.h"
static Vsim_fifo_ram_1w1r* dut_ptr;
#else
#include "Vsim_fifo_shift_1w1r.h"
static Vsim_fifo_shift_1w1r* dut_ptr;
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
    clkp_dumpgen(t);

    dut_ptr->resetn = 1;

    printf("[##] \033[1;33mCircuit reset.\033[0m\n");
}

int testbench_0(int& t)
{
    int error = 0;

    // Testbench #0 
    // Pop reset values from FIFO
    printf("[#0] Testbench #0\n");
    printf("[#0] Popping reset values from FIFO ... \n");

    // 
    dut_ptr->din = 0;
    dut_ptr->wen = 0;

    dut_ptr->ren = 0;

    clkn_dumpgen(t);
    //

    // Verify FIFO state
    printf("[#0] FIFO state EMPTY: ");

    if (dut_ptr->fifo_empty)
    {
        printf("TRUE\t, incorrect.\n");
        error++;
    }
    else
    {
        printf("FALSE\t, correct.\n");
    }

    printf("[#0] FIFO state FULL : ");

    if (dut_ptr->fifo_full)
    {
        printf("TRUE\t, correct.\n");
    }
    else
    {
        printf("FALSE\t, incorrect.\n");
        error++;
    }

    // popping reset values
    for (int i = 0; i < 16; i++)
    {
        clkp_dumpgen(t);

        dut_ptr->ren = 1;

        clkn_dumpgen(t);

        if (dut_ptr->fifo_empty)
        {
            printf("[#0] FIFO data underflow.\n");
            error++;
            break;
        }

        if (dut_ptr->dout != (15 - i))
        {
            printf("[#0] FIFO data corruption. At rCLK %d, reading '%x', expecting '%x'.\n", 
                    i, dut_ptr->dout, (15 - i));
            error++;
        }
    }

    printf("[#0] All reset data popped out of FIFO.\n");

    dut_ptr->ren = 0;

    clkp_dumpgen(t);

    // Verify FIFO state
    printf("[#0] FIFO state EMPTY: ");

    if (dut_ptr->fifo_empty)
    {
        printf("TRUE\t, correct.\n");
    }
    else
    {
        printf("FALSE\t, incorrect.\n");
        error++;
    }

    printf("[#0] FIFO state FULL : ");

    if (dut_ptr->fifo_full)
    {
        printf("TRUE\t, incorrect.\n");
        error++;
    }
    else
    {
        printf("FALSE\t, correct.\n");
    }

    if (error)
        printf("[#0] Testbench #0 \033[1;31mFAILED\033[0m !!!\n");
    else
        printf("[#0] Testbench #0 \033[1;32mPASSED\033[0m !!!\n");

    //

    return error;
}

int testbench_1(int& t)
{
    int error = 0;

    // Testbench #1
    // Writing ordered values into FIFO
    printf("[#1] Testbench #1\n");
    printf("[#1] Writing ordered values into FIFO ...\n");

    // push ordered values
    int i;
    for (i = 0; i < 16; i++)
    {
        if (dut_ptr->fifo_full)
        {
            printf("[#1] FIFO data overflow.\n");
            error++;
            break;
        }

        //
        dut_ptr->wen = 1;
        dut_ptr->din = i;

        clkn_dumpgen(t);

        clkp_dumpgen(t);
    }

    //
    printf("[#1] %d of %d data values written.\n", i, 16);

    //
    dut_ptr->wen = 0;
    dut_ptr->din = 0;

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
    // Reading orderer values from FIFO
    printf("[#2] Testbench #2\n");
    printf("[#2] Reading ordered values from FIFO ...\n");

    // read ordered values
    int i;
    for (i = 0; i < 16; i++)
    {
        if (dut_ptr->fifo_empty)
        {
            printf("[#2] FIFO data underflow.\n");
            error++;
            break;
        }
        
        if (dut_ptr->dout != i)
        {
            printf("[#2] FIFO data corruption. At rCLK %d, reading '%x', expecting '%x'.\n",
                    i, dut_ptr->dout, i);
            error++;
        }

        //
        dut_ptr->ren = 1;

        clkn_dumpgen(t);

        clkp_dumpgen(t);
    }

    //
    printf("[#2] %d of %d data values read.\n", i, 16);

    //
    dut_ptr->ren = 0;

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
    // Random read/write test-case on FIFO
    printf("[#3] Testbench #3\n");
    printf("[#3] Random read/write test-case on FIFO.\n");

    // generate output records
    printf("[#3] Generating output records and checkpoints.\n");

    const int checkpoint_lifecycle = 64;
    const int checkpoint_limiter = 16;
    const int checkpoint_count = 32;

    printf("[#3] \033[1;30mConfigured checkpoint.lifecycle         = %4d.\033[0m\n", checkpoint_lifecycle);
    printf("[#3] \033[1;30mConfigured checkpoint.lifecycle.limiter = %4d.\033[0m\n", checkpoint_limiter);
    printf("[#3] \033[1;30mConfigured checkpoint.count             = %4d.\033[0m\n", checkpoint_count);

    srand(time(0));

    int cnt = 0;
    int opi = 0;

    list<int> records;
    for (int i = 0; i < checkpoint_count; i++)
    {
        int checkpoint_start;
        int checkpoint_end;

        int checkpoint_error = 0;

        checkpoint_start = t;

        for (int j = 0; j < checkpoint_lifecycle; j++)
        {
            // write
            opi = rand() % checkpoint_limiter;

            for (int k = 0; k < opi; k++)
            {
                if (records.size() < 16)
                {
                    if (dut_ptr->fifo_full)
                    {
                        printf("[#3] Checkpoint %d escaped, corrupt FIFO state on write (unexpected FULL).\n", i);

                        dut_ptr->wen = 0;
                        dut_ptr->din = 0;
                        checkpoint_error = 1;

                        goto TESTBENCH_3_CHECKPOINT_END;
                    }

                    records.push_back(cnt & 0x3F);
                }
                else if (dut_ptr->fifo_empty)
                {
                    printf("[#3] Checkpoint %d escaped, corrupt FIFO state on write (unexpected EMPTY).\n", i);

                    dut_ptr->wen = 0;
                    dut_ptr->din = 0;
                    checkpoint_error = 1;

                    goto TESTBENCH_3_CHECKPOINT_END;
                }

                dut_ptr->wen = 1;
                dut_ptr->din = cnt & 0x3F;

                clkn_dumpgen(t);
                clkp_dumpgen(t);

                cnt++;
            }

            dut_ptr->wen = 0;
            dut_ptr->din = 0;

            // read
            opi = rand() % checkpoint_limiter;

            for (int k = 0; k < opi; k++)
            {
                if (records.empty())
                {
                    if (!dut_ptr->fifo_empty)
                    {
                        printf("[#3] Checkpoint %d escaped, corrupt FIFO state on read (unexcepted not EMPTY).\n", i);

                        dut_ptr->ren = 0;
                        checkpoint_error = 1;

                        goto TESTBENCH_3_CHECKPOINT_END;
                    }
                    else
                        continue;
                }
                else if (dut_ptr->dout != records.front())
                {
                    printf("[#3] Checkpoint %d escaped, corrupt data read. At CLK %d. Reading: '%x', expecting: '%x'.\n",
                            i, t, dut_ptr->dout, records.front());

                    dut_ptr->ren = 0;
                    checkpoint_error = 1;

                    goto TESTBENCH_3_CHECKPOINT_END;
                }
                else if (!records.empty())
                    records.pop_front();

                //
                dut_ptr->ren = 1;

                clkn_dumpgen(t);
                clkp_dumpgen(t);
            }

            dut_ptr->ren = 0;

            // write & read
            opi = rand() % checkpoint_limiter;

            for (int k = 0; k < opi; k++)
            {
                if (records.empty())
                {
                    if (!dut_ptr->fifo_empty)
                    {
                        printf("[#3] Checkpoint %d escaped, corrupt FIFO state (unexcepted not EMPTY).\n", i);

                        dut_ptr->ren = 0;
                        dut_ptr->wen = 0;
                        dut_ptr->din = 0;
                        checkpoint_error = 1;

                        goto TESTBENCH_3_CHECKPOINT_END;
                    }

                    records.push_back(cnt & 0x3F);
                }
                else
                {
                    if (dut_ptr->fifo_empty)
                    {
                        printf("[#3] Checkpoint %d escaped, corrupt FIFO state (unexcepted EMPTY).\n", i);

                        dut_ptr->ren = 0;
                        dut_ptr->wen = 0;
                        dut_ptr->din = 0;
                        checkpoint_error = 1;

                        goto TESTBENCH_3_CHECKPOINT_END;
                    }

                    if (dut_ptr->dout != records.front())
                    {
                        printf("[#3] Checkpoint %d escaped, corrupt data read. At CLK %d. Reading: '%x', expecting: '%x'.\n",
                                i, t, dut_ptr->dout, records.front());

                        dut_ptr->ren = 0;
                        dut_ptr->wen = 0;
                        dut_ptr->din = 0;
                        checkpoint_error = 1;

                        goto TESTBENCH_3_CHECKPOINT_END;
                    }

                    records.push_back(cnt & 0x3F);
                    records.pop_front();
                }

                //
                dut_ptr->ren = 1;
                dut_ptr->wen = 1;
                dut_ptr->din = cnt & 0x3F;

                clkn_dumpgen(t);
                clkp_dumpgen(t);

                cnt++;
            }

            dut_ptr->ren = 0;
            dut_ptr->wen = 0;
            dut_ptr->din = 0;
        }
    
TESTBENCH_3_CHECKPOINT_END:

        checkpoint_end = t;

        if (checkpoint_error)
        {
            printf("[#3] Checkpoint %d FAILED. At CLK from %d to %d.\n", i, checkpoint_start, checkpoint_end);
            error++;
        }
    }

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

#ifdef FIFO_RAM_BASE
    printf("[##] Testing on module '\033[1;33mcommon_fifo_ram_1w1r\033[0m'\n");
#else
    printf("[##] Testing on module '\033[1;33mcommon_fifo_shift_1w1r\033[0m'\n");
#endif

    printf("[--] ----------------------------------------\n");

    reset(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_0(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_1(t);

    printf("[--] ----------------------------------------\n");

    e += testbench_2(t);

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

#ifdef FIFO_RAM_BASE
    dut_ptr = new Vsim_fifo_ram_1w1r;
    printf("\033[1;33mRAM-base FIFO module 'common_fifo_ram_1w1r' selected.\033[0m\n");
#else
    dut_ptr = new Vsim_fifo_shift_1w1r;
    printf("\033[1;33mBucket Shifter FIFO module 'common_fifo_shift_1w1r' selected.\033[0m\n");
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
