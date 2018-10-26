/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_clk.c
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * CPU and perhperial clock init.
##################################################################*/
#define SYS_TICK_C_
#include "sys/sys_tick.h"
#include "io.h"

void sys_tick(void)
{
    int value;
    static int i = 0;
    if(i < 500)
    {
        i++;
    }
    else
    {
        i = 0;
        read(GPIO_31,&value);
        write(GPIO_31,!value);
    }
}


__interrupt void timer2_isr_handler(void)
{
    sys_tick();
}

