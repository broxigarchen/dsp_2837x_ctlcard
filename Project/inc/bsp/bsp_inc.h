/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_inc.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_INC_H_
#define BSP_INC_H_

/*  std include  */
#include <stdarg.h>
#include <math.h>

/*  chk config  */
#include "config.h"
#include "bsp/chk_config.h"

/*  board info    */
#include "bsp/cpu_info.h"
#include "bsp/cla_info.h"

/* board driver    */
#include "bsp/bsp_drv.h"

/*  lib  */
#include "lib/cmacroruntime.h"
#include "lib/cstruct.h"
#include "lib/crc.h"

#include "erron.h"

#endif
