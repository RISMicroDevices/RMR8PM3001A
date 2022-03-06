#include "verilated.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

#include "common.hpp"

using namespace std;

using namespace MEMU::Common;

#define CAM_DEPTH           32

#define DGEN8(i)            ((i) | ((i) << 8) | ((i) << 16) | ((i) << 24))

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

#include "Vsim_dffcam_1qa.h"
static Vsim_dffcam_1qa* dut_ptr;

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
    dut_ptr->en     = 1;
    dut_ptr->we     = 0;

    dut_ptr->din        = 0;
    dut_ptr->din_valid  = 0;

    dut_ptr->qdata      = 0;

    clkn_dumpgen(t);
    clkp_dumpgen(t);

    //
    for (int i = 0; i < CAM_DEPTH; i++)
    {
        dut_ptr->addr   = i;
        dut_ptr->we     = 0;

        clkn_dumpgen(t);

        if (dut_ptr->dout_valid)
        {
            printf("[#0] Incorrect post-reset state. 'dout_valid' asserted.\n");

            error++;
        }

        clkp_dumpgen(t);
    }

    //
    dut_ptr->addr   = 0;
    dut_ptr->we     = 0;

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
    // Random write emulated differential test.
    printf("[#1] Testbench #1\n");
    printf("[#1] \033[1;33mStarting at clock edge %d (ps)\033[0m\n", t);
    printf("[#1] Random write emulated differential test.\n");

    //
    int c = 65535;

    printf("[#1] \033[1;30mDifferential payload count: %d\033[0m\n", c);

    int size = CAM_DEPTH;

    printf("[#1] \033[1;30mConfigurated memory capacity: %d\033[0m\n", size);

    //
    W1RTRandomAccessMemory<int>* emulated_memory
        = new W1RTRandomAccessMemory<int>(CAM_DEPTH, MemoryReadMode::READ_FIRST);

    W1RTRandomAccessMemory<bool>* emulated_validf
        = new W1RTRandomAccessMemory<bool>(CAM_DEPTH, MemoryReadMode::READ_FIRST);

    ContentAddressableMemory<int>* emulated_cam 
        = new ContentAddressableMemory<int>(emulated_memory, 0, emulated_validf, 0, CAM_DEPTH);

    //
    for (int i = 0; i < c; i++)
    {
        int qdata  = DGEN8(rand() & 0x00FF);

        int wdata  = DGEN8(rand() & 0x00FF);
        int waddr  =   rand() % CAM_DEPTH;
        int wvalid = !(rand() % 8);
        int wen    = !(rand() % 4);

        //
        dut_ptr->addr       = waddr;
        dut_ptr->en         = wen;
        dut_ptr->din        = wdata;
        dut_ptr->din_valid  = wvalid;

        dut_ptr->qdata      = qdata;

        if (wen)
        {
            emulated_memory->SetWrite(waddr, wdata);
            emulated_validf->SetWrite(waddr, wvalid);
        }
        
        clkn_dumpgen(t);

        //
        int emulated_q = emulated_cam->QueryAddress(qdata);

        int  emulated_qaddr  = emulated_q;
        bool emulated_qvalid = emulated_q >= 0;

        int  dut_qaddr  = dut_ptr->qdata;
        int  dut_qvalid = dut_ptr->qvalid;

        if ((dut_qvalid != emulated_qvalid)
            || (dut_qvalid && (dut_qaddr != emulated_qaddr)))
        {
            printf("[#1] CAM query differs (at clk %d).\n", t);
            printf("[#1] - (qaddr ) DUT: %08x, Emulation: %08x.\n", dut_qaddr,  emulated_qaddr);
            printf("[#1] - (qvalid) DUT: %08x, Emulation: %08x.\n", dut_qvalid, emulated_qvalid);

            error++;
        }

        //
        clkp_dumpgen(t);

        emulated_memory->Eval();
        emulated_validf->Eval();

        //
    }

    //
    delete emulated_memory;
    delete emulated_validf;
    delete emulated_cam;

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

    printf("[##] Testing on module '\033[1;33mcommon_dffcam_1a1w1r1qa\033[0m'\n");

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
    dut_ptr = new Vsim_dffcam_1qa;
    printf("\033[1;33mDFF-base RAM module 'common_dffcam_1a1w1r1qa' selected.\033[0m\n");

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

