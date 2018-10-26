/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    sys_std.h
 * @AUTHOR  Brox Chen
 * @DATE    August 7, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef SYS_STD_H_
#define SYS_STD_H_

#include "sys/sys_inc.h"
#include <stdio.h>

#ifdef  SYS_STD_C_
#define SYS_STD_EXEC
#else
#define SYS_STD_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/
#define  SYS_STDIO    //include sys_std, define Macro SYS_STDIO
/******************************************************************
 * @ interface
 *******************************************************************/
SYS_STD_EXEC  void    flush(void);
SYS_STD_EXEC  int     fprint(int id,const char* fmt, ...);
SYS_STD_EXEC  int     print(const char* fmt, ...);
SYS_STD_EXEC  int     scan(const char* fmt, ...);
SYS_STD_EXEC  char    getbyte(int id);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef SYS_STD_C_


#endif
#endif
