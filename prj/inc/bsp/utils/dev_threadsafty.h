/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_spi.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef DEV_THREADSAFTY_H_
#define DEV_THREADSAFTY_H_

#include "FreeRTOS.h"


#if OS_EN
#define  OS_ENTER_CIRTICAL()      vPortEnterCritical()
#define  OS_EXIT_CIRTICAL()       vPortExitCritical()
#define  OS_SWI_ENTER_CIRTICAL()  {UBaseType_t uxSavedInterruptStatus; uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();}
#define  OS_SWI_EXIT_CIRTICAL()   taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus)

#else

#define  OS_ENTER_CIRTICAL()
#define  OS_EXIT_CIRTICAL()
#define  OS_SWI_ENTER_CIRTICAL()
#define  OS_SWI_EXIT_CIRTICAL()
#endif

#endif
