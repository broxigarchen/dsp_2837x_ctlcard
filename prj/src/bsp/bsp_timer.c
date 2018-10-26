/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_timer.c
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * initialize timer
##################################################################*/
#define BSP_TIMER_C_
#include "bsp/bsp_timer.h"
#include "bsp/bsp_clk.h"

#if TIMER0_EN
extern __interrupt void timer0_isr_handler(void);
#endif
#if TIMER1_EN
extern __interrupt void timer1_isr_handler(void);
#endif
#if TIMER2_EN
extern __interrupt void timer2_isr_handler(void);
#endif

/*
 * @timer clk and interrupt setting
 */
int bsp_timer_clk_init(volatile struct CPUTIMER_REGS * tim_ptr)
{
    EALLOW;
    if(tim_ptr == &CpuTimer0Regs)
    {
        CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;             //enable clk
        IER |= M_INT1;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
#if TIMER0_EN
        PieVectTable.TIMER0_INT = &timer0_isr_handler;
#endif
    }
    else if(tim_ptr == &CpuTimer1Regs)
    {
        CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1;             //enable clk
        IER |= M_INT13;                                   //cpu isr group enable
#if TIMER1_EN
        PieVectTable.TIMER1_INT = &timer1_isr_handler;
#endif
    }
    else if(tim_ptr == &CpuTimer2Regs)
    {
        CpuSysRegs.PCLKCR0.bit.CPUTIMER2 = 1;             //enable clk
        IER |= M_INT14;                                   //cpu isr group enable
#if TIMER2_EN
        PieVectTable.TIMER2_INT = &timer2_isr_handler;
#endif
    }
    else
    {
        EDIS;
        return -1;
    }
    EDIS;
    return 1;
}

void bsp_timer_preinit(void)
{
    bsp_timer_reset(&CpuTimer0Regs);
    bsp_timer_reset(&CpuTimer1Regs);
    bsp_timer_reset(&CpuTimer2Regs);
}

void bsp_timer_reset(volatile struct  CPUTIMER_REGS * tim_ptr)
{
    tim_ptr->PRD.all = 0xFFFFFFFF;
    tim_ptr->TPR.all  = 0;
    tim_ptr->TPRH.all  = 0;
    tim_ptr->TCR.bit.TSS = 1;     // 1 = Stop timer, 0 = Start/Restart
    tim_ptr->TCR.bit.TRB = 1;     // 1 = reload timer
    tim_ptr->TCR.bit.SOFT = 0;
    tim_ptr->TCR.bit.FREE = 0;    // Timer Free Run Disabled
    tim_ptr->TCR.bit.TIE = 1;     // 0 = Disable/ 1 = Enable Timer
}

int bsp_timer_tim_set(volatile struct CPUTIMER_REGS * tim_ptr,uint64_t interval)
{
    float sysclk = bsp_clk_get_sysclk();
    long temp = (long) (sysclk * interval);
    if(temp > 0x100000000)
    {
        return -1;
    }
    bsp_timer_reset(tim_ptr);
    tim_ptr->PRD.all = temp - 1;
    tim_ptr->TCR.bit.TRB = 1;
    tim_ptr->TCR.bit.TSS = 0;
    return 1;
}

void bsp_timer_ctr_set(volatile struct CPUTIMER_REGS * tim_ptr,float* unit,float* hrunit)
{
    bsp_timer_reset(tim_ptr);
    float prd_us = 1 / bsp_clk_get_sysclk();
    tim_ptr->TPR.all  = 0xff;    //maximum prescale to reach higher resolution
    tim_ptr->TPRH.all  = 0xff;
    *hrunit = prd_us;
    *unit = prd_us * 0x10000;
    tim_ptr->PRD.all = 0xffffffff;  //maximum period
    tim_ptr->TCR.bit.TRB = 1;
    tim_ptr->TCR.bit.TSS = 1;    //stop timer
}

int bsp_timer_init(volatile struct CPUTIMER_REGS * tim_ptr, uint64_t interval)
{
    int res;
    res = bsp_timer_clk_init(tim_ptr);
    if(res == -1)
    {
        return -1;
    }
    return bsp_timer_tim_set(tim_ptr,interval);
}



