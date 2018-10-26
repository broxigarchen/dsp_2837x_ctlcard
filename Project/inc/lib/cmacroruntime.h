/*#################################################################
 * @FILE    cmacrorumtime.h
 * @AUTHOR  Brox Chen
 * @DATE    August 1, 2016
 * @DESC.
 * useful C Macro
 ##################################################################*/
#ifndef CMACRORUNTIME_H_
#define CMACRORUNTIME_H_
#include <stdarg.h>
#include <stdint.h>
 
/*******************************************************************
 * @brief   cpu config
*******************************************************************/
#define CPU_ENDIAN   0       //SMALL 0, BIG 1
#define CPU_BITWID   16
 /*******************************************************************
 * @brief    Base Data Used
*******************************************************************/
#if        CPU_ENDIAN    //big
#define    GetOneBitMask(N)    (1<<(CPU_BITWID - 1 - N))
#define    GetDataMoveL(x,N)   ((x)<<(CPU_BITWID - 1 - N))
#define    GetDataMoveR(x,N)   ((x)<<(CPU_BITWID - 1 - N))
#define    GetNBitsMask(x)     (~((1<<(CPU_BITWID - x))-1))     //maximum get (CPU_BITWID - 1) mask
#else //small
#define    GetOneBitMask(N)    (1<<(N))
#define    GetDataMoveL(x,N)   ((x)<<(N))
#define    GetDataMoveR(x,N)   ((x)>>(N))
#define    GetNBitsMask(x)     ((1<<(x))-1)     //maximum get (CPU_BITWID - 1) mask
#endif
 
 
 /*******************************************************************
 * @brief    Bit Used
*******************************************************************/
 #define   RegBitOperation(FlagBit,RegPtr,RegBit)    \
 {if(FlagBit){ (RegPtr)|=(GetOneBitMask(RegBit)); }      \
 else { (RegPtr)&=~(GetOneBitMask(RegBit));}}
 
 #define   RegBitInverseOperation(FlagBit,RegPtr,RegBit)    \
 {if(FlagBit){ (RegPtr)&=~GetOneBitMask(RegBit); }      \
 else { (RegPtr)|=(GetOneBitMask(RegBit));}}
 
 #define   RegNBitsOperation(Value,RegPtr,RegBit,N)    \
 {(RegPtr)&=~(GetNBitsMask(N)<<(RegBit));           \
 (RegPtr)|=(((Value)&GetNBitsMask(N))<<(RegBit));}
 
 /*******************************************************************
 * @brief   Count Used
*******************************************************************/
 #define  __Array_Count(ArrayName,DataSize)     \
          (sizeof(ArrayName)/(DataSize))
 
 #define  __ASCII_Count(StrName)                \
          (strlen(ArrayName)/sizeof(uint8_t))
 
 #define  __Unicode_Count(StrName)                 \
          __ASCII_Count(StrName)/2
 
 /*******************************************************************
 * @brief   va list
*******************************************************************/
#ifndef va_copy
#define va_copy(dst,src)  \
        memcpy((unsigned char*)dst,(unsigned char*)src,sizeof(va_list))
#endif


#endif
