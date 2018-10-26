/*#################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 *
 * @FILE    cpu_info.h
 * @AUTHOR  Brox Chen
 * @DATE    May 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * The file defines the specification of CPU
 * CPU:     TMS320F28379D
 *
 *
 ##################################################################*/
#ifndef CPU_INFO_H_
#define CPU_INFO_H_

/*      C28x data type def     */
/**
 * Referring to TMS320C28x Optimizing C/C++ Compiler v18.1.0.LTS, Chapter 6.4
 * The smallest data type of C28x Compiler is 16bits
 *
 * Notice: enum type can goes maximum to 64bits
 */
#include <stdint.h>

typedef char                int8_t;
typedef int                 int16_t;
typedef long                int32_t;
typedef long long           int64_t;
typedef unsigned char       uint8_t;
typedef unsigned int        uint16_t;
typedef unsigned long       uint32_t;
typedef unsigned long long  uint64_t;


typedef uint32_t            addr_t;

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif

#ifndef NULL
#define NULL  0
#endif

/*      C28x ASM Commands   */
#define  EINT   __asm(" clrc INTM")
#define  DINT   __asm(" setc INTM")
#define  ERTM   __asm(" clrc DBGM")
#define  DRTM   __asm(" setc DBGM")
#ifndef  EALLOW
#define  EALLOW __asm(" EALLOW")
#endif
#ifndef  EDIS
#define  EDIS   __asm(" EDIS")
#endif
#define  ESTOP0 __asm(" ESTOP0")

/*      C28x CPU register    */
extern __cregister volatile unsigned int IFR;
extern __cregister volatile unsigned int IER;


#define CPU_GPIO_MAX_TOG_FREQ  25000000     //gpio maximum toggle frequency


#endif /* CPU_INFO_H_ */
