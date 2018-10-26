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
#define BSP_CLK_C_
#include "bsp/bsp_clk.h"

/*
 *@ Notes for developer
 * sysclk = osc_clk * (pll.imult + pll.fmult) / (pll.div + 1)
   where pll.imult from 1 - 127, step 1,
         pll.fmult from 0 - 0.75, step 0.25
         pll.div   from 0 - 125

 * auxclk = aux_osc_clk * ( auxpll.imult + auxpll.fmult)
 **/


float bsp_clk_cal(uint64_t freq,uint16_t* im,uint16_t* fm,uint16_t* div)
{
    uint16_t i,j,k;
    float diff,op_diff = 0;
    float imult, fmult, plldiv,clk,op_clk;
    uint16_t op_imult, op_fmult, op_plldiv;
    for(i=0;i<127;i++)
    {
        imult = i;
        for(j=0;j<4;j++)
        {
           fmult = j * 0.25;
           for(k=0;k<126;k++)
           {
               plldiv = k + 1;
               clk = XTAL_FREQ * (imult + fmult) / plldiv;
               diff = clk - freq;
               diff = fabs(diff);
               if(diff <= F_ZERO)
               {
                   *im = i; *fm=j;  *div=k;
                   return clk;
               }
               else if(diff < op_diff || op_diff == 0)
               {
                   op_diff = diff;
                   op_imult = i; op_fmult = j; op_plldiv = k;
                   op_clk = clk;
               }
           }
        }
   }
   if(op_diff < 0.1)
   {
       *im = op_imult;
       *fm = op_fmult;
       *div = op_plldiv;
       return op_clk;
   }
   else
   {
       return 0;
   }
}

int bsp_sys_clk_init(void)
{
   uint16_t imult,fmult,plldiv;
   float final_clk;
   if(XTAL_FREQ > SYS_CLK)
   {
       PERROR("pll clk smaller than osc freqency!");
       return -1;
   }
   final_clk = bsp_clk_cal(SYS_CLK,&imult,&fmult,&plldiv);
   if(final_clk == 0)
   {
       PERROR("pll clk set fail, no option matched!");
       return -1;
   }
   PDEBUG("set sys clk freq: %f",final_clk);
   bsp_clk_info.sysclk = final_clk;
   InitSysPll(XTAL_OSC,imult,fmult,plldiv);
   return 1;
}

int bsp_aux_clk_init(void)
{
    uint16_t imult,fmult,plldiv;
    float final_clk;
    final_clk = bsp_clk_cal(AUX_CLK,&imult,&fmult,&plldiv);
    if(final_clk == 0)
    {
       PERROR("aux pll set fail, no option matched!");
       return -1;
    }
    PDEBUG("set aux clk freq: %f",final_clk);
    bsp_clk_info.auxclk = final_clk;
    InitAuxPll(XTAL_OSC,imult,fmult,plldiv);
    return 1;
}

void bsp_pclk_init(void)
{
    /*      pclk select    */

    /* low speed device clk */
    EALLOW;
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV =  (LSPCLK_DIV >> 1);
    bsp_clk_info.lspclk = bsp_clk_info.sysclk / LSPCLK_DIV;
    EDIS;

    PDEBUG("set ls pclk freq: %f",bsp_clk_info.lspclk);

    /* epwm clk */
    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV =  EPWMCLK_DIV - 1;
    bsp_clk_info.epwmclk = bsp_clk_info.sysclk / EPWMCLK_DIV;
    EDIS;

    PDEBUG("set epwm clk freq: %f", bsp_clk_info.epwmclk);
}


int bsp_clk_init(void)
{
    int res;
    res = bsp_sys_clk_init();
    if(res == -1)
    {
        return -1;
    }
    res = bsp_aux_clk_init();
    if(res == -1)
    {
        return -1;
    }
    bsp_pclk_init();
    return 1;
}

float bsp_clk_get_sysclk(void)
{
    return bsp_clk_info.sysclk;
}

float bsp_clk_get_auxclk(void)
{
    return bsp_clk_info.auxclk;
}

float bsp_clk_get_lspclk(void)
{
    return bsp_clk_info.lspclk;
}

float bsp_clk_get_epwmclk(void)
{
    return bsp_clk_info.epwmclk;
}

