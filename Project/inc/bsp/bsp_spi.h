/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_spi.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_SPI_H_
#define BSP_SPI_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_SPI_C_
#define BSP_SPI_EXEC
#else
#define BSP_SPI_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/
enum BSP_SPI_POLARITY
{
    BSP_SPI_ROFI,     //rise out fall in
    BSP_SPI_RIFO,     //rise in  fall out
};
enum BSP_SPI_MODE
{
    BSP_SPI_SLAVE,
    BSP_SPI_MASTER,
};
enum BSP_SPI_WID
{
    BSP_SPI_1 = 0x0,
    BSP_SPI_2 = 0x1,
    BSP_SPI_8 = 0x7,
    BSP_SPI_16 = 0xf,
};
/******************************************************************
 * @ public
 *******************************************************************/
inline void bsp_spi_isr_switch_tx(volatile struct SPI_REGS* spi_port,uint8_t on_off)
{
    spi_port->SPIFFTX.bit.TXFFIENA = on_off;//Tx FIFO interrupt enable/disable
}
inline void bsp_spi_clrisr_tx(volatile struct SPI_REGS* spi_port)
{
    spi_port->SPIFFTX.bit.TXFFINTCLR = 1;
}
inline void bsp_spi_clrisr_rx(volatile struct SPI_REGS* spi_port)
{
    spi_port->SPIFFRX.bit.RXFFOVFCLR = 1;   //clear Rx overflow interrupt
    spi_port->SPIFFRX.bit.RXFFINTCLR = 1;   //clear Rx receive interrupt
}
inline void bsp_spi_set_mode(volatile struct SPI_REGS* spi_port,enum BSP_SPI_MODE mode)
{
    spi_port->SPICCR.bit.SPISWRESET = 0;    //soft hold reset
    spi_port->SPICTL.bit.MASTER_SLAVE = mode;     //master 1, slave 0
    spi_port->SPICCR.bit.SPISWRESET = 1;    //soft hold release
}
inline void bsp_spi_set_polarity(volatile struct SPI_REGS* spi_port,enum BSP_SPI_POLARITY polarity)
{
    spi_port->SPICCR.bit.SPISWRESET = 0;    //soft hold reset
    spi_port->SPICCR.bit.CLKPOLARITY = polarity;
    spi_port->SPICCR.bit.SPISWRESET = 1;    //soft hold release
}
inline void bsp_spi_set_width(volatile struct SPI_REGS* spi_port,enum BSP_SPI_WID width)
{
    spi_port->SPICCR.bit.SPISWRESET = 0;    //soft hold reset
    spi_port->SPICCR.bit.SPICHAR = width;
    spi_port->SPICCR.bit.SPISWRESET = 1;    //soft hold release
}
inline void bsp_spi_talk(volatile struct SPI_REGS* spi_port,uint8_t on_off)
{
    spi_port->SPICTL.bit.TALK = on_off;
}
inline int bsp_spi_writerdy(volatile struct SPI_REGS* spi_port)
{
    return !spi_port->SPISTS.bit.BUFFULL_FLAG;
}
inline int bsp_spi_writefifordy(volatile struct SPI_REGS* spi_port)
{
    if(spi_port->SPIFFTX.bit.TXFFST < 0x10)
    {
        return 1;
    }
    return -1;
}
inline void bsp_spi_writechar(volatile struct SPI_REGS* spi_port,uint8_t byte,uint8_t wid)
{
    spi_port->SPITXBUF = (byte << (16-wid));
}
inline int bsp_spi_readfifordy(volatile struct SPI_REGS* spi_port)
{
    if(spi_port->SPIFFRX.bit.RXFFST > 0)
    {
        return 1;
    }
    return -1;
}
inline void bsp_spi_readchar(volatile struct SPI_REGS* spi_port,uint8_t* byte,uint8_t wid)
{
   uint32_t mask = ((1<<wid)-1);
   *byte = ((spi_port->SPIRXBUF) & mask);
}
inline uint16_t bsp_spi_tx_empty(volatile struct SPI_REGS* spi_port)
{
    return (spi_port->SPIFFTX.bit.TXFFST == 0);
}
inline addr_t bsp_spi_get_dma_sendaddr(volatile struct SPI_REGS* spi_port)
{
    return (addr_t)spi_port->SPITXBUF;
}
inline addr_t bsp_spi_get_dma_rcvaddr(volatile struct SPI_REGS* spi_port)
{
    return (addr_t)spi_port->SPIRXBUF;
}
/******************************************************************
 * @ interface
 *******************************************************************/
BSP_SPI_EXEC int bsp_spi_init(volatile struct SPI_REGS* spi_port,uint32_t baud,uint16_t rcv_shred, uint16_t isr_route);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_SPI_C_

#endif
#endif
