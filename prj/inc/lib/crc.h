/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    crc.h
 * @AUTHOR  Brox Chen
 * @DATE    August 3, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef CRC_H_
#define CRC_H_

#include "bsp/cpu_info.h"
#include <stdint.h>
#include <string.h>

#ifdef CRC_C_
#define CRC_EXEC
#else
#define CRC_EXEC extern
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
CRC_EXEC void init_crc8(void);
CRC_EXEC unsigned char crc8(unsigned char *pdata, size_t nbytes);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef CRC_C_
/*      CRC8     */
unsigned char crc8_table[256];     /* 8-bit table */
int made_crc8_table=0;


#endif
#endif
