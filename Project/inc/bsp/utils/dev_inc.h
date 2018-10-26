/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_inc.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef DEV_INC_H_
#define DEV_INC_H_


/******************************************************************
 * @ inlcude
 *******************************************************************/
/*  std include  */
#include <stdarg.h>
#include <math.h>

/*  config */
#include "config.h"
#include "bsp/utils/dev_cfg.h"

/*  device   */
#include "bsp/utils/dev_prototype.h"
#include "bsp/utils/dev_threadsafty.h"

/*  bsp drv include  */
#include "bsp/bsp_timer.h"
#include "bsp/bsp_uart.h"
#include "bsp/bsp_gpio.h"
#include "bsp/bsp_clk.h"

/*  lib  */
#include "lib/cmacroruntime.h"
#include "lib/cstruct.h"
#include "erron.h"



/******************************************************************
 * @ device Marco definition
 *******************************************************************/
#ifndef ISR_ROUTE_CPU1
#define ISR_ROUTE_CPU1  0
#endif
#ifndef ISR_ROUTE_CLA1
#define ISR_ROUTE_CLA1  1
#endif
#ifndef ISR_ROUTE_CPU2
#define ISR_ROUTE_CPU2  2
#endif
#ifndef ISR_ROUTE_CLA2
#define ISR_ROUTE_CLA2  3
#endif



#endif
