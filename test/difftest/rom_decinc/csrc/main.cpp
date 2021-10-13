#include "verilated.h"
#include "Vsim_rom_decinc.h"
#include <cstdio>

#define TRUE                1
#define FALSE               0

#ifdef VM_TRACE         // --trace
#include <verilated_vcd_c.h>
static VerilatedVcdC* fp;      //to form *.vcd file
#endif

Vsim_rom_decinc* dut_ptr;

int test(int time, int t, bool dec) 
{
    dut_ptr->d = t;
    dut_ptr->dec = dec;

    dut_ptr->eval();

    int incorrect = 0;

    int ref_q_2, ref_c_2;
    int ref_q_3, ref_c_3;
    int ref_q_4, ref_c_4;
    int ref_q_5, ref_c_5;

    if (dec)
    {
        ref_c_2 = (t      & 0x0003) == 0;
        ref_q_2 = (t - 1) & 0x0003;

        ref_c_3 = (t      & 0x0007) == 0;
        ref_q_3 = (t - 1) & 0x0007;

        ref_c_4 = (t      & 0x000F) == 0;
        ref_q_4 = (t - 1) & 0x000F;

        ref_c_5 = (t      & 0x001F) == 0;
        ref_q_5 = (t - 1) & 0x001F;
    }
    else
    {
        ref_c_2 = (t      & 0x0003) == 3;
        ref_q_2 = (t + 1) & 0x0003;

        ref_c_3 = (t      & 0x0007) == 7;
        ref_q_3 = (t + 1) & 0x0007;

        ref_c_4 = (t      & 0x000F) == 15;
        ref_q_4 = (t + 1) & 0x000F;

        ref_c_5 = (t      & 0x001F) == 31;
        ref_q_5 = (t + 1) & 0x001F;
    }

    if (dut_ptr->q_decinc2 != ref_q_2)
    {
        printf("[INCORRECT] At 'q' in module 'common_rtlrom_decinc2'. Input: %2d, Result: %2d, Ref: %2d\n",
            dut_ptr->d, dut_ptr->q_decinc2, ref_q_2);
        incorrect++;
    }

    if (dut_ptr->c_decinc2 != ref_c_2)
    {
        printf("[INCORRECT] At 'c' in module 'common_rtlrom_decinc2'. Input: %2d, Result: %2d, Ref: %2d\n",
            dut_ptr->d, dut_ptr->c_decinc2, ref_c_2);
        incorrect++;
    }

    if (dut_ptr->q_decinc3 != ref_q_3)
    {
        printf("[INCORRECT] At 'q' in module 'common_rtlrom_decinc3'. Input: %2d, Result: %2d, Ref: %2d\n",
            dut_ptr->d, dut_ptr->q_decinc3, ref_q_3);
        incorrect++;
    }

    if (dut_ptr->c_decinc3 != ref_c_3)
    {
        printf("[INCORRECT] At 'c' in module 'common_rtlrom_decinc3'. Input: %2d, Result: %2d, Ref: %2d\n",
            dut_ptr->d, dut_ptr->c_decinc3, ref_c_3);
        incorrect++;
    }

    if (dut_ptr->q_decinc4 != ref_q_4)
    {
        printf("[INCORRECT] At 'q' in module 'common_rtlrom_decinc4'. Input: %2d, Result: %2d, Ref: %2d\n",
            dut_ptr->d, dut_ptr->q_decinc4, ref_q_4);
        incorrect++;
    }

    if (dut_ptr->c_decinc4 != ref_c_4)
    {
        printf("[INCORRECT] At 'c' in module 'common_rtlrom_decinc4'. Input: %2d, Result: %2d, Ref: %2d\n",
            dut_ptr->d, dut_ptr->c_decinc4, ref_c_4);
        incorrect++;
    }

    if (dut_ptr->q_decinc5 != ref_q_5)
    {
        printf("[INCORRECT] At 'q' in module 'common_rtlrom_decinc5'. Input: %2d, Result: %2d, Ref: %2d\n",
            dut_ptr->d, dut_ptr->q_decinc5, ref_q_5);
        incorrect++;
    }

    if (dut_ptr->c_decinc5 != ref_c_5)
    {
        printf("[INCORRECT] At 'c' in module 'common_rtlrom_decinc5'. Input: %2d, Result: %2d, Ref: %2d\n",
            dut_ptr->d, dut_ptr->c_decinc5, ref_c_5);
        incorrect++;
    }

    if (dec)
    {
        if (dut_ptr->q_decr2 != ref_q_2)
        {
            printf("[INCORRECT] At 'q' in module 'common_rtlrom_decr2'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->q_decr2, ref_q_2);
            incorrect++;
        }

        if (dut_ptr->c_decr2 != ref_c_2)
        {
            printf("[INCORRECT] At 'c' in module 'common_rtlrom_decr2'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->c_decr2, ref_c_2);
            incorrect++;
        }

        if (dut_ptr->q_decr3 != ref_q_3)
        {
            printf("[INCORRECT] At 'q' in module 'common_rtlrom_decr3'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->q_decr3, ref_q_3);
            incorrect++;
        }

        if (dut_ptr->c_decr3 != ref_c_3)
        {
            printf("[INCORRECT] At 'c' in module 'common_rtlrom_decr3'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->c_decr3, ref_c_3);
            incorrect++;
        }

        if (dut_ptr->q_decr4 != ref_q_4)
        {
            printf("[INCORRECT] At 'q' in module 'common_rtlrom_decr4'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->q_decr4, ref_q_4);
            incorrect++;
        }

        if (dut_ptr->c_decr4 != ref_c_4)
        {
            printf("[INCORRECT] At 'c' in module 'common_rtlrom_decr4'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->c_decr4, ref_c_4);
            incorrect++;
        }

        if (dut_ptr->q_decr5 != ref_q_5)
        {
            printf("[INCORRECT] At 'q' in module 'common_rtlrom_decr5'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->q_decr5, ref_q_5);
            incorrect++;
        }

        if (dut_ptr->c_decr5 != ref_c_5)
        {
            printf("[INCORRECT] At 'c' in module 'common_rtlrom_decr5'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->c_decr5, ref_c_5);
            incorrect++;
        }
    }
    else
    {
        if (dut_ptr->q_incr2 != ref_q_2)
        {
            printf("[INCORRECT] At 'q' in module 'common_rtlrom_incr2'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->q_incr2, ref_q_2);
            incorrect++;
        }

        if (dut_ptr->c_incr2 != ref_c_2)
        {
            printf("[INCORRECT] At 'c' in module 'common_rtlrom_incr2'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->c_incr2, ref_c_2);
            incorrect++;
        }

        if (dut_ptr->q_incr3 != ref_q_3)
        {
            printf("[INCORRECT] At 'q' in module 'common_rtlrom_incr3'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->q_incr3, ref_q_3);
            incorrect++;
        }

        if (dut_ptr->c_incr3 != ref_c_3)
        {
            printf("[INCORRECT] At 'c' in module 'common_rtlrom_incr3'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->c_incr3, ref_c_3);
            incorrect++;
        }

        if (dut_ptr->q_incr4 != ref_q_4)
        {
            printf("[INCORRECT] At 'q' in module 'common_rtlrom_incr4'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->q_incr4, ref_q_4);
            incorrect++;
        }

        if (dut_ptr->c_incr4 != ref_c_4)
        {
            printf("[INCORRECT] At 'c' in module 'common_rtlrom_incr4'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->c_incr4, ref_c_4);
            incorrect++;
        }

        if (dut_ptr->q_incr5 != ref_q_5)
        {
            printf("[INCORRECT] At 'q' in module 'common_rtlrom_incr5'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->q_incr5, ref_q_5);
            incorrect++;
        }

        if (dut_ptr->c_incr5 != ref_c_5)
        {
            printf("[INCORRECT] At 'c' in module 'common_rtlrom_incr5'.   Input: %2d, Result: %2d, Ref: %2d\n",
                dut_ptr->d, dut_ptr->c_incr5, ref_c_5);
            incorrect++;
        }
    }


#ifdef VM_TRACE
    fp->dump(time);
#endif

    return incorrect;
}

int main() 
{
    dut_ptr = new Vsim_rom_decinc;

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
    int time = 0;
    int incorrect = 0;

    printf("Simulating RTL ROM increment elements ...\n");
    for (int i = 0; i < 32; i++, time++) 
        incorrect += test(time, i, FALSE);

    printf("Simulating RTL ROM decrement elements ...\n");
    for (int i = 0; i < 32; i++, time++)
        incorrect += test(time, i, TRUE);

    if (!incorrect)
        printf("Passed. No Incorrects.\n");
    else
        printf("Failed. %d incorrect(s) found.\n", incorrect);

    // finalize
#ifdef VM_TRACE
    fp->close();
    delete fp;
#endif
    delete dut_ptr;

    return 0;
}