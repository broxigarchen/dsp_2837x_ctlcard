/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_init.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_INIT_H_
#define BSP_INIT_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_INIT_C
#define BSP_INIT_EXEC
#else
#define BSP_INIT_EXEC extern
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
BSP_INIT_EXEC int bsp_reset(void);
BSP_INIT_EXEC int bsp_init(void);

/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_INIT_C

#endif
#endif
