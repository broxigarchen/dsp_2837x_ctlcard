/*#################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 *
 * @FILE    chk_config.h
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * This file defines all settings of the project
 *
 ##################################################################*/
#ifndef CHK_CONFIG_H_
#define CHK_CONFIG_H_

#include "config.h"


#define  CHK_CFG_OUT_OF_RANGE(tar,min,max)   (tar < (min) && tar > (max))
/*####################################################
 *@ Kernel Clocking System
########################################################*/
#ifndef   XTAL_FREQ
#error    "XTAL_FREQ missing, Please define the frequency of the external Oscillator!"
#elif      CHK_CFG_OUT_OF_RANGE(XTAL_FREQ,0,60)
#error    "Frequency of the external Oscillator out of range!"
#endif

#ifndef   SYS_CLK
#error    "SYS_CLK missing, Please define the frequency of the CPU clock!"
#elif      CHK_CFG_OUT_OF_RANGE(SYS_CLK,0,60)
#error    "Frequency of the CPU clock out of range!"
#endif

#endif
