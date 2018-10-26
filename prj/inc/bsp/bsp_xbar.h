/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_xbar.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_XBAR_H_
#define BSP_XBAR_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_XBAR_C_
#define BSP_XBAR_EXEC
#else
#define BSP_XBAR_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/

/******************************************************************
 * @ interface
 *******************************************************************/
BSP_XBAR_EXEC void bsp_xbar_set_input(uint8_t ch_id,uint16_t gpio);

/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_XBAR_C_


#endif
#endif
