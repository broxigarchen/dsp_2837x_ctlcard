/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    root.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef ROOT_H_
#define ROOT_H_

#include "app/app_inc.h"

#ifdef ROOT_C_
#define ROOT_EXEC
#else
#define ROOT_EXEC extern
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
ROOT_EXEC int   root_init(void);
ROOT_EXEC void  root_main(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef ROOT_C_

#endif
#endif
