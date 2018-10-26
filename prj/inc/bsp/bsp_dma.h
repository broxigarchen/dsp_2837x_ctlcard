/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_dma.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_DMA_H_
#define BSP_DMA_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_DMA_C_
#define BSP_DMA_EXEC
#else
#define BSP_DMA_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/
enum DMA_SRC
{
    DMA_RAM = 0,
    /*      ADC         */
    DMA_ADCAINT1 = 1,
    DMA_ADCAINT2,DMA_ADCAINT3,DMA_ADCAINT4,DMA_ADCAEVT,
    DMA_ADCBINT1,DMA_ADCBINT2,DMA_ADCBINT3,DMA_ADCBINT4,DMA_ADCBEVT,
    DMA_ADCCINT1,DMA_ADCCINT2,DMA_ADCCINT3,DMA_ADCCINT4,DMA_ADCCEVT,
    DMA_ADCDINT1,DMA_ADCDINT2,DMA_ADCDINT3,DMA_ADCDINT4,DMA_ADCDEVT,
    /*      XINT         */
    DMA_XINT1 = 29,
    DMA_XINT2,DMA_XINT3,DMA_XINT4,DMA_XINT5,
    /*      EPWM         */
    DMA_EPWM1A = 36,
    DMA_EPWM1B,DMA_EPWM2A,DMA_EPWM2B,DMA_EPWM3A,DMA_EPWM3B,
    DMA_EPWM4A,DMA_EPWM4B,DMA_EPWM5A,DMA_EPWM5B,DMA_EPWM6A,DMA_EPWM6B,
    DMA_EPWM7A,DMA_EPWM7B,DMA_EPWM8A,DMA_EPWM8B,DMA_EPWM9A,DMA_EPWM9B,
    DMA_EPWM10A,DMA_EPWM10B,DMA_EPWM11A,DMA_EPWM11B,DMA_EPWM12A,DMA_EPWM12B,
    DMA_EPWM13A,DMA_EPWM13B,DMA_EPWM14A,DMA_EPWM14B,DMA_EPWM15A,DMA_EPWM15B,
    DMA_EPWM16A,DMA_EPWM16B,
    /*      TINT         */
    DMA_TINT0 = 68,
    DMA_TINT1,DMA_TINT2,
    /*      MXEVTA         */
    DMA_MXEVTA = 71,
    DMA_MREVTA,DMA_MXEVTB,DMA_MREVTB,
    /*      SD FTL         */
    DMA_SD1FLT1 = 95,
    DMA_SD1FLT2,DMA_SD1FLT3,DMA_SD1FLT4,
    DMA_SD2FLT1,DMA_SD2FLT2,DMA_SD2FLT3,DMA_SD2FLT4,
    /*      SPI         */
    DMA_SPIATX = 109,
    DMA_SPIARX,DMA_SPIBTX,DMA_SPIBRX,DMA_SPICTX,DMA_SPICRX,
    /*      USB         */
    DMA_USBRX1 = 131,
    DMA_USBTX1,DMA_USBRX2,DMA_USBTX2,DMA_USBRX3,DMA_USBTX3,

};  /* DMA Channel Interrupt trigger Source Enumeration */


/*  DMA config parameter   */
#define   DMACH_ISR          (1 << 15)  //channel interrupt enable
#define   DMACH_32BIT        (1 << 14)  //data size 32bit
#define   DMACH_CONT         (1 << 11)  //continue mode
#define   DMACH_1SHOT        (1 << 10)  //one shot mode
#define   DMACH_ISR_AT_BGN   (0 << 9)   //isr generated at 1: beginning, 0: end
#define   DMACH_ISR_AT_END   (1 << 9)
#define   DMACH_TRIG         (1 << 8)   //peripheral trigger enable
#define   DMACH_OVR_ISR      (1 << 7)   //over run interrupt


__inline void bsp_DMACH_halt(struct CH_REGS* ptr)
{
    EALLOW;
    ptr->CONTROL.bit.HALT=1;
    EDIS;
}
/******************************************************************
 * @ interface
 *******************************************************************/
BSP_DMA_EXEC void      bsp_DMACH_TrigSrcSel(struct CH_REGS* ptr, enum DMA_SRC src);
BSP_DMA_EXEC void      bsp_DMACH_ModeConfig(struct CH_REGS* ptr, uint16_t para);
BSP_DMA_EXEC void      bsp_DMACH_ISRConfig(struct CH_REGS* ptr, uint16_t para);
BSP_DMA_EXEC void      bsp_DMACH_AddrConfig(struct CH_REGS* ptr,volatile uint32_t *DMA_Dest,volatile uint32_t *DMA_Source);
BSP_DMA_EXEC void      bsp_DMACH_BurstConfig(struct CH_REGS* ptr,uint16_t bsize, int16_t srcbstep, int16_t desbstep);
BSP_DMA_EXEC void      bsp_DMACH_TransferConfig(struct CH_REGS* ptr,uint16_t tsize, int16_t srctstep, int16_t deststep);
BSP_DMA_EXEC void      bsp_DMACH_WrapConfig(struct CH_REGS* ptr,uint16_t srcwsize, int16_t srcwstep, uint16_t deswsize,int16_t deswstep);
BSP_DMA_EXEC void      bsp_DMACH_Start(struct CH_REGS* ptr);
BSP_DMA_EXEC int       bsp_DMACH_Init(struct CH_REGS* ptr);

/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_DMA_C_



#endif
#endif
