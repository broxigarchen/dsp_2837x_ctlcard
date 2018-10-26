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
#include "timer.h"


void sys_tick_init(void)
{
    int res;
    res = setTimer(TIMER2, TIM_NROM, 500000, sys_tick, NULL);
    if(res == -1)
    {
        print("sys tick init fail!");
    }
}

void sys_tick(void)
{
    char value;
    read(GPIO_31,&value);
    write(GPIO_31,!value);
}

