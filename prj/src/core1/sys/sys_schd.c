/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    sys_schd.c
 * @AUTHOR  Brox Chen
 * @DATE    May 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 *
##################################################################*/
#define SYS_SCHD_C_
#include "sys/sys_schd.h"
#include "sys/sys_tick.h"
#include "io.h"
#include "time.h"
#include "app/root.h"

err_t sys_reset(void)
{
    return 0;
}

err_t sys_init(int err)
{
    /*  lib init */
    init_crc8();
#if OS_EN

#else
    /*   sys tick init */
    sys_tick_init();
#endif
    /*   start task root  */
    root_init();
    return 0;
}

void sys_schd(void)
{
#if OS_EN
    vTaskStartScheduler();
#else
    while(1)
    {
        root_main();
    }
#endif
}
