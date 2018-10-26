/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    crc.c
 * @AUTHOR  Brox Chen
 * @DATE    August 3, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 *
##################################################################*/
#define CRC_C_
#include "lib/crc.h"
/*  
* crc8.c
* 
* Computes a 8-bit CRC 
* 
*/

#define GP  0x107   /* x^8 + x^2 + x + 1 */
#define DI  0x07

void init_crc8(void)
  /*
   * Should be called before any other crc function.  
   */
{
  int i,j;
  unsigned char crc;
  
  if (!made_crc8_table)
  {
    for (i=0; i<256; i++) 
    {
      crc = i;
      for (j=0; j<8; j++)
      {
        crc = (crc << 1) ^ ((crc & 0x80) ? DI : 0);
      }
      crc8_table[i] = crc & 0xFF;
      /* printf("table[%d] = %d (0x%X)\n", i, crc, crc); */
    }
    made_crc8_table=1;
  }
}
 
unsigned char crc8(unsigned char *pdata, size_t nbytes)
      /*
       * For a byte array whose accumulated crc value is stored in *crc, computes
       * resultant crc obtained by appending m to the byte array
      */
 {
    unsigned char crc = 0;

   if (!made_crc8_table)
    init_crc8();

   while (nbytes-- > 0)
   {
        crc = crc8_table[(crc ^ *pdata++) & 0xff];
   }
   return crc;
}

