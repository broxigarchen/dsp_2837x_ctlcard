/*#################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 *
 * @FILE    config.h
 * @AUTHOR  Brox Chen
 * @DATE    May 28, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * This file select all settings of the project
 ##################################################################*/
#ifndef CONFIG_H_
#define CONFIG_H_

/*      select  core       */
/*  CPU1,CLA1,CPU2,CLA2 should be defined in project setting to
   be compatible with TI's library */
#if   defined(CPU1)
#define  CORE_ID       0
#elif defined(CPU2)
#define  CORE_ID       1
#elif defined(CLA1)
#define  CORE_ID       2
#elif defined(CLA2)
#define  CORE_ID       3
#endif

/*      select board configuration   */
#define F28379D_EVALBOARD    0
#define F28379D_LAUNCHPAD    1
#define BOARD_TYPE           F28379D_LAUNCHPAD

#if (BOARD_TYPE == F28379D_EVALBOARD)
#include "board/evalboard/config.h"
#elif (BOARD_TYPE == F28379D_LAUNCHPAD)
#include "board/launchpad/config.h"
#endif /* BOARD_TYPE */

/*     Peripheral CLK Maximum Tolerable Error    */
#define UART_CLK_MAX_ERR    0.05        //UART clock maximum tolerable error in ratio
#define SPI_CLK_MAX_ERR     0.01        //SPI clock maximum tolerable error in ratio
#define EPWM_CAR_MAX_ERR    0.01        //EPWM Carrier maximum tolerable error in ratio
/*####################################################
 *@ Debug
########################################################*/
#define   DEBUG
/*####################################################
 *@ Constant
########################################################*/
#define   F_ZERO                 0.001       //constant zero, used in float comparison
#define   PI                     3.1415926


#endif /* CONFIG_H_ */
