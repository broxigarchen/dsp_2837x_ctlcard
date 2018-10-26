/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    sys_ent.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef SYS_SCHD_H_
#define SYS_SCHD_H_

#include "sys/sys_inc.h"

#ifdef SYS_SCHD_C_
#define SYS_SCHD_EXEC
#else
#define SYS_SCHD_EXEC extern
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
SYS_SCHD_EXEC err_t sys_init(int err);
SYS_SCHD_EXEC void sys_schd(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef SYS_SCHD_C_

#endif
#endif
