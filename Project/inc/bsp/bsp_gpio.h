/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_gpio.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_GPIO_H_
#define BSP_GPIO_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_GPIO_C_
#define BSP_GPIO_EXEC
#else
#define BSP_GPIO_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/


/******************************************************************
 * @ interface
 *******************************************************************/
BSP_GPIO_EXEC void      bsp_gpio_init(void);
BSP_GPIO_EXEC void      bsp_gpio_set(uint16_t pin_id,uint16_t value);
BSP_GPIO_EXEC uint16_t  bsp_gpio_read(uint16_t pin_id);
BSP_GPIO_EXEC void      bsp_gpio_toggle(uint16_t pin_id);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_GPIO_C_

typedef struct _bsp_gpio_info
{
    uint16_t id;
    uint16_t func;
    uint16_t dir;
    uint16_t pullup;
}bsp_gpio_info_t;

#endif
#endif
