/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_timer.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef DEV_TIMER_H_
#define DEV_TIMER_H_

#include "bsp/utils/dev_inc.h"

#ifdef DEV_TIMER_C_
#define DEV_TIMER_EXEC
#else
#define DEV_TIMER_EXEC extern
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
DEV_TIMER_EXEC void dev_timer_init(void);
DEV_TIMER_EXEC int configtimer(int dev_id,...);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef DEV_TIMER_C_


#endif
#endif
