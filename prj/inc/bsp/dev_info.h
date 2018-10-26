/*#################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 *
 * @FILE    dev_info.h
 * @AUTHOR  Brox Chen
 * @DATE    June 25, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * device enumeration
 *
 ##################################################################*/
#include "config.h"
/*####################################################
 *@ device  Timer
 *@ FORMAT
    DEV_INFO_TIMER(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, INTERVAL, ISR_EN, DESC)
######################################################*/
#ifdef DEV_TIMER_C_
DEV_INFO_TIMER(TIMER0, TIMER0_EN, timer0, &CpuTimer0Regs, TIMER0_TS, 1, "cpu timer 0")
DEV_INFO_TIMER(TIMER1, TIMER1_EN, timer1, &CpuTimer1Regs, TIMER1_TS, 1, "cpu timer 1")
DEV_INFO_TIMER(TIMER2, TIMER2_EN, timer2, &CpuTimer2Regs, TIMER2_TS, 1, "cpu timer 2")
#endif /*  DEV_TIMER_C  */
/*####################################################
 *@ device  internal AD
 *@ Notice
######################################################*/
/*
#define MAX_ADC 4
enum _DEV_AD
{
    AD_1,
    AD_2,
    AD_3,
    AD_4,
    AD_MAX,
};
enum _DEV_AD1_CHAN
{
    AD_1_CHAN1,AD_1_CHAN2,AD_1_CHAN3,AD_1_CHAN4,
    AD_1_CHAN5,AD_1_CHAN6,AD_1_CHAN7,AD_1_CHAN8,
    AD_1_CHAN9,AD_1_CHAN10,AD_1_CHAN11,AD_1_CHAN12,
    AD_1_CHAN_MAX,
};
*/
/*####################################################
 *@ device  DMA
 *@ format
    DEV_INFO_DMA(DEV_ID, DEV_NAME, PHY_PTR, ISR_RT, DESC)
######################################################*/
#ifdef DEV_DMA_C_
DEV_INFO_DMA(DMA_CH1, dma_ch1, &DmaRegs.CH1, "dma_ch1" )
DEV_INFO_DMA(DMA_CH2, dma_ch2, &DmaRegs.CH2, "dma_ch2" )
DEV_INFO_DMA(DMA_CH3, dma_ch3, &DmaRegs.CH3, "dma_ch3" )
DEV_INFO_DMA(DMA_CH4, dma_ch4, &DmaRegs.CH4, "dma_ch4" )
DEV_INFO_DMA(DMA_CH5, dma_ch5, &DmaRegs.CH5, "dma_ch5" )
DEV_INFO_DMA(DMA_CH6, dma_ch6, &DmaRegs.CH6, "dma_ch6" )
#endif
/*####################################################
 *@ device  EPWM
 *@ format
    DEV_INFO_EPWM(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, ISR_RT, DESC)
######################################################*/
#ifdef DEV_EPWM_C_
DEV_INFO_EPWM(EPWM1,  EPWM1_EN,  epwm1,  &EPwm1Regs,  EPWM1_ISR_RT, "epwm1")
DEV_INFO_EPWM(EPWM2,  EPWM2_EN,  epwm2,  &EPwm2Regs,  EPWM2_ISR_RT, "epwm2")
DEV_INFO_EPWM(EPWM3,  EPWM3_EN,  epwm3,  &EPwm3Regs,  EPWM3_ISR_RT, "epwm3")
DEV_INFO_EPWM(EPWM4,  EPWM4_EN,  epwm4,  &EPwm4Regs,  EPWM4_ISR_RT, "epwm4")
DEV_INFO_EPWM(EPWM5,  EPWM5_EN,  epwm5,  &EPwm5Regs,  EPWM5_ISR_RT, "epwm5")
DEV_INFO_EPWM(EPWM6,  EPWM6_EN,  epwm6,  &EPwm6Regs,  EPWM6_ISR_RT, "epwm6")
DEV_INFO_EPWM(EPWM7,  EPWM7_EN,  epwm7,  &EPwm7Regs,  EPWM7_ISR_RT, "epwm7")
DEV_INFO_EPWM(EPWM8,  EPWM8_EN,  epwm8,  &EPwm8Regs,  EPWM8_ISR_RT, "epwm8")
DEV_INFO_EPWM(EPWM9,  EPWM9_EN,  epwm9,  &EPwm9Regs,  EPWM9_ISR_RT, "epwm9")
DEV_INFO_EPWM(EPWM10, EPWM10_EN, epwm10, &EPwm10Regs, EPWM10_ISR_RT, "epwm10")
DEV_INFO_EPWM(EPWM11, EPWM11_EN, epwm11, &EPwm11Regs, EPWM11_ISR_RT, "epwm11")
DEV_INFO_EPWM(EPWM12, EPWM12_EN, epwm12, &EPwm12Regs, EPWM12_ISR_RT, "epwm12")
#endif
/*####################################################
 *@ device  UART
 *@ format
    DEV_INFO_UART(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, PARITY, RCV_SHRED, ISR_EN, ISR_RT, DESC)
######################################################*/
#ifdef DEV_UART_C_
DEV_INFO_UART(UART_A, UART_A_EN, uarta, &SciaRegs, UART_A_BAUD, UART_A_PARITY, UART_A_SHRED, 1, UART_A_ISR_RT, "uart A")
DEV_INFO_UART(UART_B, UART_B_EN, uartb, &ScibRegs, UART_B_BAUD, UART_B_PARITY, UART_B_SHRED, 1, UART_B_ISR_RT, "uart B")
DEV_INFO_UART(UART_C, UART_C_EN, uartc, &ScicRegs, UART_C_BAUD, UART_C_PARITY, UART_C_SHRED, 1, UART_C_ISR_RT, "uart C")
DEV_INFO_UART(UART_D, UART_D_EN, uartd, &ScidRegs, UART_D_BAUD, UART_D_PARITY, UART_D_SHRED, 1, UART_D_ISR_RT, "uart D")
#endif
/*####################################################
 *@ device  SPI
 *@ format
     DEV_INFO_SPI(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, RCV_SHRED, ISR_EN, ISR_RT, DESC)
######################################################*/
#ifdef DEV_SPI_C_
DEV_INFO_SPI(SPI_A, SPI_A_EN, spia, &SpiaRegs, SPI_A_BAUD, SPI_A_SHRED, 1, SPI_A_ISR_RT, "spi A")
DEV_INFO_SPI(SPI_B, SPI_B_EN, spib, &SpibRegs, SPI_B_BAUD, SPI_B_SHRED, 1, SPI_B_ISR_RT, "spi B")
DEV_INFO_SPI(SPI_C, SPI_C_EN, spic, &SpicRegs, SPI_C_BAUD, SPI_C_SHRED, 1, SPI_C_ISR_RT, "spi C")
#endif

/*
#define MAX_ECAP 7
#define MAX_EQEP 4
#define MAX_I2C 2
#define MAX_MCBSP 2
#define MAX_SDFM 3
#define MAX_TRIPSEL 15
*/



