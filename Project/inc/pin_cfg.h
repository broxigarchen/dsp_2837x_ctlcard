/*#################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 *
 * @FILE    pin_cfg.h
 * @AUTHOR  Brox Chen
 * @DATE    May 28, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * This file defines pin map
 * F2839 Evaluation Board
 ##################################################################*/
#include "config.h"

#if (BOARD_TYPE == F28379D_EVALBOARD)
#include "board/evalboard/pin_cfg.h"
#elif (BOARD_TYPE == F28379D_LAUNCHPAD)
#include "board/launchpad/pin_cfg.h"
#endif /* BOARD_TYPE */


