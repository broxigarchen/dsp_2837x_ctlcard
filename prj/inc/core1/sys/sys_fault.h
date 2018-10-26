/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    sys_fault.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef SYS_FAULT_H_
#define SYS_FAULT_H_

#include "sys/sys_inc.h"

#ifdef SYS_FAULT_C
#define SYS_FAULT_EXEC
#else
#define SYS_FAULT_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/
typedef int err_t;
/******************************************************************
 * @ interface
 *******************************************************************/
SYS_FAULT_EXEC void sys_fault(int err);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef SYS_FAULT_C

#endif
#endif
