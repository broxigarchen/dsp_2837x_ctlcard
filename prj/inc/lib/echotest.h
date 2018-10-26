/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    echotest.h
 * @AUTHOR  Brox Chen
 * @DATE    August 3, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef ECHOTEST_H_
#define ECHOTEST_H_

#include "bsp/cpu_info.h"
#include <stdint.h>
#include <string.h>

#ifdef ECHOTEST_C_
#define ECHOTEST_EXEC
#else
#define ECHOTEST_EXEC extern
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
ECHOTEST_EXEC void init_crc8(void);
ECHOTEST_EXEC unsigned char crc8(unsigned char *pdata, size_t nbytes);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef ECHOTEST_C_


#endif
#endif
