/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_timer.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_TIMER_H_
#define BSP_TIMER_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_TIMER_C_
#define BSP_TIMER_EXEC
#else
#define BSP_TIMER_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/
inline void bsp_timer_isr_switch(volatile struct CPUTIMER_REGS * tim_ptr,uint8_t on_off)
{
    tim_ptr->TCR.bit.TIE = on_off;
}
inline uint32_t bsp_timer_readCTR(volatile struct CPUTIMER_REGS * tim_ptr)
{
    return (0xffffffff - tim_ptr->TIM.all);
}
inline uint16_t bsp_timer_readPRECTR(volatile struct CPUTIMER_REGS * tim_ptr)
{
    return (((0xff - tim_ptr->TPRH.bit.PSCH) << 8) | (0xff - tim_ptr->TPR.bit.PSC));
}
inline void bsp_timer_reload(volatile struct CPUTIMER_REGS * tim_ptr)
{
    tim_ptr->TCR.bit.TRB = 1;
}
inline void bsp_timer_switch(volatile struct CPUTIMER_REGS * tim_ptr,uint8_t on_off)
{
    tim_ptr->TCR.bit.TSS = !on_off;
}
/******************************************************************
 * @ interface
 *******************************************************************/
BSP_TIMER_EXEC void bsp_timer_preinit(void);
BSP_TIMER_EXEC int  bsp_timer_init(volatile struct CPUTIMER_REGS * tim_ptr, uint64_t interval);
BSP_TIMER_EXEC int  bsp_timer_tim_set(volatile struct CPUTIMER_REGS * tim_ptr,uint64_t interval);
BSP_TIMER_EXEC void bsp_timer_ctr_set(volatile struct CPUTIMER_REGS * tim_ptr,float* unit,float* hrunit);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_TIMER_C_

int bsp_timer_clk_init(volatile struct CPUTIMER_REGS* tim_ptr);
void bsp_timer_reset(volatile struct  CPUTIMER_REGS * tim_ptr);

#endif
#endif
