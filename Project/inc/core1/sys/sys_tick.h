/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    sys_tick.h
 * @AUTHOR  Brox Chen
 * @DATE    July 1, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef SYS_TICK_H_
#define SYS_TICK_H_

#include "sys/sys_inc.h"

#ifdef SYS_TICK_C_
#define SYS_TICK_EXEC
#else
#define SYS_TICK_EXEC extern
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
SYS_TICK_EXEC void sys_tick_init(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef SYS_TICK_EXEC
void sys_tick(void);
#endif
#endif
