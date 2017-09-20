/*****************************************************************************
 * @file     DataTypeDef.h
 * @brief    data type definition
 * @version  V1.00
 * @date     Sep 14th, 2017
 * @author   Brox Chen
 *
 * @note
 * This h file is used for data type definition for TI TM320 series DSP utilization
 *
 * @par
 * None
 *
 *
 Copyright
 ******************************************************************************/

#ifndef DATATYPEDEF_H
#define DATATYPEDEF_H

#ifndef _STDINT_H
#define _STDINT_H
typedef char                  int16_t;
typedef unsigned char         uint16_t;
typedef long                  int32_t;
typedef unsigned long         uint32_t;
typedef long long             int64_t;
typedef unsigned long long    uint64_t;
#endif

#ifndef _STDFLOAT_H
#define _STDFLOAT_H
typedef float                 float32_t;
typedef long double           float64_t;
#endif

#endif
