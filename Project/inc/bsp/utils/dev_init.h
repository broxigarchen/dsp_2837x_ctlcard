/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_init.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef DEV_INIT_H_
#define DEV_INIT_H_

#include "bsp/utils/dev_inc.h"

#ifdef DEV_INIT_C_
#define DEV_INIT_EXEC
#else
#define DEV_INIT_EXEC extern
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
DEV_INIT_EXEC void dev_init(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef DEV_INIT_C_


#endif
#endif
