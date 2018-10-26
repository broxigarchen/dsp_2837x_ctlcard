/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_xbar.c
 * @AUTHOR  Brox Chen
 * @DATE    Aug 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * device manage: dma
##################################################################*/
#define BSP_XBAR_C_
#include "bsp/bsp_xbar.h"


void bsp_xbar_set_input(uint8_t ch_id,uint16_t gpio)
{
    uint16_t* regptr = (uint16_t*)&InputXbarRegs;
    EALLOW;
    if(ch_id < 15)
    {
        *(regptr + ch_id) = gpio;
    }
    EDIS;

}



void bsp_xbar_set_output()
{

}
