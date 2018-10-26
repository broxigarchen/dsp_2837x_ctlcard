/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    comtest.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef HBRIDGE_H_
#define HBRIDGE_H_

#include "app/app_inc.h"

#ifdef HBRIDGE_C_
#define HBRIDGE_EXEC
#else
#define HBRIDGE_EXEC extern
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
HBRIDGE_EXEC int   hbridge_init(void);
HBRIDGE_EXEC void  hbridge_main(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef HBRIDGE_C_


#endif
#endif
