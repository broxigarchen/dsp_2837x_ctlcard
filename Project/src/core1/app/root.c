/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    root.c
 * @AUTHOR  Brox Chen
 * @DATE    July 31, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * root task, System run this task first
##################################################################*/
#define ROOT_C_
#include "app/root.h"
#include "app/comtest.h"
#include "app/hbridge.h"

int root_init(void)
{
    //comtest_init();
    adtest_init();
    //hbridge_init();
    return 1;
}


void root_main(void)
{
    //comtest_main();
    adtest_main();
    //hbridge_main();
}
