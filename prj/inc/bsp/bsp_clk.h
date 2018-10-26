/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_clk.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_CLK_H_
#define BSP_CLK_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_CLK_C_
#define BSP_CLK_EXEC
#else
#define BSP_CLK_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/
typedef struct _bsp_clk_info_t
{
    float sysclk;
    float auxclk;
    float lspclk;
    float epwmclk;
}bsp_clk_info_t;

/******************************************************************
 * @ interface
 *******************************************************************/
BSP_CLK_EXEC int   bsp_clk_init(void);
BSP_CLK_EXEC float bsp_clk_get_sysclk(void);
BSP_CLK_EXEC float bsp_clk_get_auxclk(void);
BSP_CLK_EXEC float bsp_clk_get_lspclk(void);
BSP_CLK_EXEC float bsp_clk_get_epwmclk(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_CLK_C_

bsp_clk_info_t  bsp_clk_info;

#endif
#endif
