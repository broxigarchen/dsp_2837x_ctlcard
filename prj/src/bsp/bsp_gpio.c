/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_gpio.c
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * initialize bsp
##################################################################*/
#define BSP_GPIO_C_
#include "bsp/bsp_gpio.h"


#define PIN_INFO(ID, FUNC, DIR, PULLUP)\
        {ID,FUNC,DIR,PULLUP},
bsp_gpio_info_t bsp_gpio_info[]={
#include "pin_cfg.h"
};
#define PIN_INFO_SIZE __Array_Count(bsp_gpio_info,sizeof(bsp_gpio_info_t))

void bsp_gpio_init(void)
{
    uint16_t i;
    uint16_t flag;
    InitGpio();

    for(i=0;i<PIN_INFO_SIZE;i++)
    {
        GPIO_SetupPinMux(bsp_gpio_info[i].id, GPIO_MUX_CPU1, bsp_gpio_info[i].func); //set function
        flag = 0;
        if(bsp_gpio_info[i].func == 0)       //if gpio, set input/output and conditions
        {
            if(bsp_gpio_info[i].dir == 1)    //output
            {
                if(bsp_gpio_info[i].pullup == 1)
                {
                    flag |= GPIO_PULLUP;
                }
                else
                {
                    flag |= GPIO_OPENDRAIN;
                }
            }
            else   //input
            {
                if(bsp_gpio_info[i].pullup == 1)
                {
                    flag |= GPIO_PULLUP;
                }
            }
            GPIO_SetupPinOptions(bsp_gpio_info[i].id, bsp_gpio_info[i].dir, flag);
        }
    }
}

void bsp_gpio_set(uint16_t pin_id,uint16_t value)
{
    GPIO_WritePin(pin_id,value);
}

uint16_t bsp_gpio_read(uint16_t pin_id)
{
    return GPIO_ReadPin(pin_id);
}

void bsp_gpio_toggle(uint16_t pin_id)
{
    uint16_t curr_value = bsp_gpio_read(pin_id);
    GPIO_WritePin(pin_id,~curr_value);
}

