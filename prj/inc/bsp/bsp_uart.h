/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_uart.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_UART_H_
#define BSP_UART_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_UART_C
#define BSP_UART_EXEC
#else
#define BSP_UART_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/
inline void bsp_uart_clrisr_rx(volatile struct SCI_REGS* sci_port)
{
    sci_port->SCIFFRX.bit.RXFFOVRCLR = 1;
    sci_port->SCIFFRX.bit.RXFFINTCLR = 1;
}
inline void bsp_uart_clrisr_tx(volatile struct SCI_REGS* sci_port)
{
    sci_port->SCIFFTX.bit.TXFFINTCLR = 1;
}
inline int bsp_uart_writerdy(volatile struct SCI_REGS* sci_port)
{
    if(sci_port->SCIFFTX.bit.TXFFST < 16)
    {
        return 1;
    }
    return -1;
}
inline void bsp_uart_writebyte(volatile struct SCI_REGS* sci_port,char data)
{
     sci_port->SCITXBUF.bit.TXDT = (data & 0xff);
}
inline int bsp_uart_readrdy(volatile struct SCI_REGS* sci_port)
{
    if(sci_port->SCIFFRX.bit.RXFFST != 0)
    {
        return 1;
    }
    return -1;
}
inline void bsp_uart_readbyte(volatile struct SCI_REGS* sci_port,char* data)
{
    *data = (sci_port->SCIRXBUF.bit.SAR);
}
inline int bsp_uart_busy(volatile struct SCI_REGS* sci_port)
{
    if(sci_port->SCICTL2.bit.TXEMPTY == 1)
    {
        return -1;
    }
    return 1;
}
inline void bsp_uart_isr_switch_tx(volatile struct SCI_REGS* sci_port,uint16_t on_off)
{
    sci_port->SCIFFTX.bit.TXFFIENA = on_off;
}
inline int bsp_uart_isr_status_tx(volatile struct SCI_REGS* sci_port)
{
    if(sci_port->SCIFFTX.bit.TXFFIENA) return 1;
    return -1;
}

/******************************************************************
 * @ interface
 *******************************************************************/
BSP_UART_EXEC int   bsp_uart_init(volatile struct SCI_REGS* sci_port,uint32_t baud,uint16_t parity,uint16_t rcv_shred,uint16_t isr_route);
BSP_UART_EXEC int   bsp_uart_set(volatile struct SCI_REGS* sci_port,uint64_t baud,uint16_t polarity);
BSP_UART_EXEC void  bsp_uart_reset(volatile struct SCI_REGS* sci_port, uint16_t rx_trig_bytes);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_UART_C_


#endif
#endif
