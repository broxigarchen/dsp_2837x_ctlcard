/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    main.c
 * @AUTHOR  Brox Chen
 * @DATE    May 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * entrance of the program
##################################################################*/
#ifdef CPU1
#include "bsp/bsp_init.h"
#endif

#include "sys/sys_schd.h"
void main(void)
{
    int ret;
    err_t err;
#ifdef CPU1
    ret = bsp_init();
#endif
    /* sys run */
    err = sys_init(ret);
    if(err < 0)
    {
        sys_fault(err);
        while(1);
    }
    sys_schd();
    while(1);
}
