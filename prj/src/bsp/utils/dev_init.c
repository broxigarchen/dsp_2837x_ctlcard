/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_init.c
 * @AUTHOR  Brox Chen
 * @DATE    May 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * initialize bsp
##################################################################*/
#define DEV_INIT_C_
#include "bsp/utils/dev_init.h"
#include "bsp/utils/dev_timer.h"
#include "bsp/utils/dev_uart.h"
#include "bsp/utils/dev_spi.h"
#include "bsp/utils/dev_epwm.h"

void dev_init(void)
{
    dev_uart_init();
    dev_timer_init();
    dev_spi_init();
    dev_epwm_init();
}

