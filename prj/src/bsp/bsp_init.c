/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_init.c
 * @AUTHOR  Brox Chen
 * @DATE    May 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * initialize bsp
##################################################################*/
#define BSP_INIT_C_
#include "bsp/bsp_init.h"
#include "bsp/bsp_clk.h"
#include "bsp/bsp_gpio.h"
#include "bsp/utils/dev_init.h"

int bsp_reset(void)
{
    DisableDog();
    DINT;
    InitPieCtrl();     //reset ISR vector

    bsp_clk_init();    //set clock
    bsp_gpio_init();   //set gpio pin function
    dev_init();        //set utils device

    EnableInterrupts();
    return 0;
}

int bsp_init(void)
{
    DINT;

    InitPieVectTable();   //Initialize ISR vector
    return bsp_reset();
}

