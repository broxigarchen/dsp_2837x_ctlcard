/*****************************************************************************
 * @file     init.h
 * @brief    initialize task
 * @version  V1.00
 * @date     Sep 14th, 2017
 * @author   Brox Chen
 *
 * @note
 * initialize the hardware and start application task
 *
 * @par
 * None
 *
 *
 Copyright
 ******************************************************************************/
#ifndef INIT_H
#define INIT_H

#ifdef  INIT_C
#define Ex_INIT_C
#else
#define Ex_INIT_C  extern
#endif

/**************************************************************
 * @brief Macro
**************************************************************/

/**************************************************************
 * @brief Data Structure Definition
**************************************************************/

/**************************************************************
 * @brief Public
**************************************************************/
Ex_INIT_C void init(void);
Ex_INIT_C void schd(void);
/**************************************************************
 * @brief Private
**************************************************************/
#ifdef INIT_C


#endif
#endif
