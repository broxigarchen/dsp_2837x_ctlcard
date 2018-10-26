/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    spitest.h
 * @AUTHOR  Brox Chen
 * @DATE    Aug 7, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef SPITEST_H_
#define SPITEST_H_

#include "app/app_inc.h"

#ifdef ADTEST_C_
#define ADTEST_EXEC
#else
#define ADTEST_EXEC extern
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
ADTEST_EXEC int   adtest_init(void);
ADTEST_EXEC void  adtest_main(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef ADTEST_C_



#endif
#endif
