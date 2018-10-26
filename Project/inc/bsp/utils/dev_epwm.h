/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_epwm.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef DEV_EPWM_H_
#define DEV_EPWM_H_

#include "bsp/utils/dev_inc.h"
#include "bsp/bsp_epwm.h"

#ifdef DEV_EPWM_C_
#define DEV_EPWM_EXEC
#else
#define DEV_EPWM_EXEC extern
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
DEV_EPWM_EXEC int dev_epwm_init(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef DEV_EPWM_C_


#endif
#endif
