/*#################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 *
 * @FILE    config.h
 * @AUTHOR  Brox Chen
 * @DATE    May 28, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * This file defines all settings of the project
 * launchpad
 ##################################################################*/

/*********************************************************************/
/*********************************************************************/
/************      HARDWARE CONFIGURATION          *****************/
/*
 * @Notice
 * DONT CHANGE THE HARDWARE SETTINGS UNLESS YOU KNOW WHAT YOU ARE DOING!
 *
 * @Content -- Search the module name and jump to the part!
 * 1. Kernel Clocking System
 * 2. Timer & Watch Dog
 * 3. Analog
 * 4  EPWM
 * 5. Communication Port(uart,spi,usb)
 */
/*********************************************************************/
/*********************************************************************/

/*####################################################
 *@ Kernel Clocking System
 *@ Notice  Default select external Osc as clock source
            Default select PLL as clock source of the CPU
            Default apply same clock freq. to dual-core of CPU
 *@ Explains
     XTAL_FREQ(MHz)  external Oscillator Freq.
     SYS_CLK(MHz)    Kernel(including CPU) Main Clock Freq.,
                     select from: XTAL_FREQ to 200
     AUX_CLK(MHz)    auxiliary clock for USB device
     LSPCLK_DIV      LSPCLK(low speed Peripheral clock) = SYS_CLK / LSPCLK_DIV,
                     select from: 1,2,4,6,8,10,12,14
     EPWMCLK_DIV     EPWM_CLK = SYS_CLK / EPWMCLK_DIV,
                     select from: 1 or 2
########################################################*/
/*      CPU         */
#define   XTAL_FREQ      10
#define   SYS_CLK        200
#define   AUX_CLK        60
#define   LSPCLK_DIV     4
#define   EPWMCLK_DIV    2
/*      ALU         */


/*####################################################
 *@ Kernel  Timer && Watch Dog
 *@ Notice
    !!!TIMER2 is default enabled
 *@ Explains
    TIMERx_EN           enable timer and the corresponding interrupt
                        select from: 0 diable,
                                     1 enable
    TIMERx_TS(us)       default hardware period of the timer
                        select from: smaller than (0xffff / SYS_CLK)
    TIMERx_ISR_ROUTE    interrupt routing, connect to which kernel, some of the options
                        are not availible, check the user manual when changing
                        select from: 0,  cpu 1
                                     1,  cpu 2
                                     2,  cla 1
                                     3,  cla 2
########################################################*/
/*      Timer         */
#define  TIMER0_EN          1
#define  TIMER0_TS          1000

#define  TIMER1_EN          1
#define  TIMER1_TS          1000

#define  TIMER2_EN          1
#define  TIMER2_TS          1000

#define  IPC_TIMER_EN       0
#define  IPC_TIMER_TS       100

/*       WDT         */
#define  WDT_EN             0
#define  WDT_PERIOD         1

/*####################################################
 *@ Analog
 *@ Notice
 *@ Explains
#####################################################*/
/*      ADC         */
#define ADC_CLK


/*#####################################################
 *@ EPWM
 *  epwm modules enable/disable
 *@ Notice

 *@ Explains
    EPWMx_ISR_ROUTE    interrupt routing, connect to which kernel, some of the options
                        are not availible, check the user manual when changing
                        select from: 0,  cpu 1
                                     1,  cpu 2
                                     2,  cla 1
                                     3,  cla 2
######################################################*/
#define   EPWM1_EN           1
#define   EPWM1_ISR_RT       0

#define   EPWM2_EN           1
#define   EPWM2_ISR_RT       0

#define   EPWM3_EN           0
#define   EPWM3_ISR_RT       0

#define   EPWM4_EN           0
#define   EPWM4_ISR_RT       0

#define   EPWM5_EN           0
#define   EPWM5_ISR_RT       0

#define   EPWM6_EN           0
#define   EPWM6_ISR_RT       0

#define   EPWM7_EN           0
#define   EPWM7_ISR_RT       0

#define   EPWM8_EN           0
#define   EPWM8_ISR_RT       0

#define   EPWM9_EN           0
#define   EPWM9_ISR_RT       0

#define   EPWM10_EN          0
#define   EPWM10_ISR_RT      0

#define   EPWM11_EN          0
#define   EPWM11_ISR_RT      0

#define   EPWM12_EN          0
#define   EPWM12_ISR_RT      0


/*####################################################
 *@ Communication Port
########################################################*/
/*@ uart
 *@ Notice
    UART A is reserved
 *@ Explains
    UART_x_EN                        enable serial port x
    UART_x_BAUD(bps, bit per sec)    set baud rate,
                                     select from: 1 - LPSCLK  (typical value 2400,4800,9600,...,115200,...)
    UART_x_SHRED                     how many bytes triggered receive interrupt
                                     select from  1 - 31
    UART_x_PARITY                    even/odd check,
                                     select from:   0,  disable
                                                    1,  odd check
                                                    2,  even check
    UART_x_ISR_ROUTE                 interrupt routing, connect to which kernel, some of the options
                                     are not availible, check the user manual when changing
                                    select from: 0,  cpu 1
                                                 1,  cpu 2
                                                 2,  cla 1
                                                 3,  cla 2
*/
#define UART_A_EN         0
#define UART_A_BAUD       115200
#define UART_A_SHRED      1
#define UART_A_PARITY     0
#define UART_A_ISR_RT     0

#define UART_B_EN         0
#define UART_B_BAUD       115200
#define UART_B_SHRED      1
#define UART_B_PARITY     0
#define UART_B_ISR_RT     0

#define UART_C_EN         1
#define UART_C_BAUD       1000000
#define UART_C_SHRED      1
#define UART_C_PARITY     0
#define UART_C_ISR_RT     0

#define UART_D_EN         0
#define UART_D_BAUD       115200
#define UART_D_SHRED      1
#define UART_D_PARITY     0
#define UART_D_ISR_RT     0

#define RS485_SEL_PIN     97

/*@ spi
 *@ Explains
    SPI_x_EN                        enable serial peripheral interface x
    SPI_x_BAUD(bps, bit per sec)    set baud rate,
                                     select from: 1 - LPSCLK  (typical value 2400,4800,9600,...,115200,...)
    SPI_x_SHRED                     how many bytes triggered receive interrupt
                                     select from  1 - 31
    SPI_x_ISR_ROUTE                 interrupt routing, connect to which kernel, some of the options
                                     are not available, check the user manual when changing
                                    select from: 0,  cpu 1
                                                 1,  cpu 2
                                                 2,  cla 1
                                                 3,  cla 2
*/
#define SPI_A_EN         1
#define SPI_A_BAUD       1000000
#define SPI_A_SHRED      1
#define SPI_A_ISR_RT     0

#define SPI_B_EN         0
#define SPI_B_BAUD       1000000
#define SPI_B_SHRED      1
#define SPI_B_ISR_RT     0

#define SPI_C_EN         0
#define SPI_C_BAUD       1000000
#define SPI_C_SHRED      1
#define SPI_C_ISR_RT     0

/*********************************************************************/
/*********************************************************************/
/************      SOFTWARE CONFIGURATION          *****************/
/*
 * @Content -- Search the module name and jump to the part!
 * 1. software buffer size
 * 2. system variables
 */
/*********************************************************************/
/*********************************************************************/
/*####################################################
 *@ Operating System
########################################################*/
#define OS_EN                    0           //enable OS
/*####################################################
 *@ Software Buffer Size
 *@ Notice
    *Buffer distribution of disabled device won't be executed
 *@ Explains
    xxx_BUF_SIZE                 buffer size of xxx device
########################################################*/
#define ADC_CHANNEL_BUF_SIZE     100         //ADC Channel Buffer Size
#define UART_IN_BUF_SIZE         256         //UART Receive Buffer Size
#define UART_OUT_BUF_SIZE        400         //UART Send Buffer Size
#define STDOUT_BUF_SIZE          400         //STDOUT Buffer Size
#define STDIN_BUF_SIZE           400         //STDIN Buffer Size
#define SPI_IN_BUF_SIZE          20          //SPI Receive Buffer Size
#define SPI_OUT_BUF_SIZE         20          //SPI Send Buffer Size
/*####################################################
 *@ System Setting
########################################################*/
#define   STDIO                  UART_B
