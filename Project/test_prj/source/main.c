/*****************************************************************************
 * @file     main.c
 * @brief
 * @version  V1.00
 * @date     Sep 14th, 2017
 * @author   Brox Chen
 *
 * @note
 * main entry
 *
 * @par
 * None
 *
 *
 Copyright
 ******************************************************************************/
#include "mstdinc.h"
#include "boot/init.h"
void main(void)
{
    init();
    while(1)
    {
        schd();
    }
}
