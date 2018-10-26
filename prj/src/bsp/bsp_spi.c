/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_spi.c
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * initialize bsp
##################################################################*/
#define BSP_SPI_C_
#include "bsp/bsp_spi.h"
#include "bsp/bsp_clk.h"

#if SPI_A_EN
extern __interrupt void spia_rx_isr_handler();
extern __interrupt void spia_tx_isr_handler();
#endif
#if SPI_B_EN
extern __interrupt void spib_rx_isr_handler();
extern __interrupt void spib_tx_isr_handler();
#endif
#if SPI_C_EN
extern __interrupt void spic_rx_isr_handler();
extern __interrupt void spic_tx_isr_handler();
#endif


int bsp_spi_clk_init(volatile struct SPI_REGS* spi_port)
{
    EALLOW;
    if((spi_port) == (&SpiaRegs))
    {
        CpuSysRegs.PCLKCR8.bit.SPI_A = 1;     //clk
        PieCtrlRegs.PIEIER6.bit.INTx1 = 1;   // PIE Group 6, INT1, RX
        PieCtrlRegs.PIEIER6.bit.INTx2 = 1;   // PIE Group 6, INT2, TX
        IER |= M_INT6;                       // Enable CPU INT
#if SPI_A_EN
        PieVectTable.SPIA_RX_INT =  &spia_rx_isr_handler;
        PieVectTable.SPIA_TX_INT =  &spia_tx_isr_handler;
#endif
    }
    else if((spi_port) == (&SpibRegs))
    {
        CpuSysRegs.PCLKCR8.bit.SPI_B = 1;
        PieCtrlRegs.PIEIER6.bit.INTx3 = 1;   // PIE Group 6, INT3, RX
        PieCtrlRegs.PIEIER6.bit.INTx4 = 1;   // PIE Group 6, INT4, TX
        IER |= M_INT6;                       // Enable CPU INT
#if SPI_B_EN
        PieVectTable.SPIB_RX_INT = &spib_rx_isr_handler;
        PieVectTable.SPIB_TX_INT = &spib_tx_isr_handler;
#endif
    }
    else if((spi_port) == (&SpicRegs))
    {
        CpuSysRegs.PCLKCR8.bit.SPI_C = 1;
        PieCtrlRegs.PIEIER6.bit.INTx9 = 1;   // PIE Group 6, INT9, RX
        PieCtrlRegs.PIEIER6.bit.INTx10 = 1;   // PIE Group 6, INT10, TX
        IER |= M_INT6;                       // Enable CPU INT
#if SPI_C_EN
        PieVectTable.SPIC_RX_INT = &spic_rx_isr_handler;
        PieVectTable.SPIC_TX_INT = &spic_tx_isr_handler;
#endif
    }
    else
    {
        EDIS;
        return -1;
    }
    EDIS;
    return 1;
}

/*
 * non-High speed mode baud
 */
int bsp_spi_set_baud(volatile struct SPI_REGS* spi_port,uint32_t baud)
{
    float lspclk = bsp_clk_get_lspclk() * 1000000;
    uint16_t spibrr;
    uint16_t brr_l,brr_h;
    float brr;
    float percision;
    if(baud == 0 || baud > CPU_GPIO_MAX_TOG_FREQ)    //maximum GPIO toggle frequency
    {
        PERROR("spi baud larger than gpio max speed");
        return -1;
    }
    if((baud << 2) > lspclk)   //maximum SPI speed == lspclk / 4
    {
        PERROR("spi baud too fast");
        return -1;
    }
    brr = lspclk/baud;
    if(brr > 128)
    {
        PERROR("spi baud too slow");
        return -1;
    }
    brr_l = brr;
    brr_h = brr + 1;
    if(fabs(lspclk/brr_l-baud) > fabs(lspclk/brr_h-baud))
    {
        spibrr = brr_h;
    }
    else
    {
        spibrr = brr_l;
    }
    percision = (lspclk / spibrr - baud) / baud;
    if(percision > SPI_CLK_MAX_ERR)
    {
        return -1;
    }
    spi_port->SPIBRR.bit.SPI_BIT_RATE = spibrr-1;
    return 1;
}



void bsp_spi_reset(volatile struct SPI_REGS* spi_port,uint16_t rcv_shred)
{
    spi_port->SPICCR.bit.SPISWRESET = 0;    //soft hold reset
    /*      General        */
    spi_port->SPICCR.bit.HS_MODE = 0;       //non-high speed mode
    spi_port->SPICCR.bit.SPILBK = 0;        //loop back disable
    spi_port->SPICTL.bit.CLK_PHASE = 1;     //clock phase shifted in advance
    spi_port->SPICTL.bit.MASTER_SLAVE = 1;  //default master
    spi_port->SPICTL.bit.OVERRUNINTENA = 1;       //overrun interrupt
    spi_port->SPICTL.bit.SPIINTENA = 1;           //enable interrupt
    /*        FIFO        */
    spi_port->SPIFFTX.bit.SPIRST = 1;       //FIFO normal
    spi_port->SPIFFTX.bit.SPIFFENA = 0;     //FIFO enhancement disable
    spi_port->SPIFFTX.bit.TXFIFO = 0;       //Tx FIFO hold reset
    spi_port->SPIFFTX.bit.TXFFINTCLR = 1;   //clear Tx FIFO interrupt
    spi_port->SPIFFTX.bit.TXFFIENA = 1;     //Tx FIFO interrupt enable
    spi_port->SPIFFTX.bit.TXFFIL = 0;       //Tx FIFO interrupt when no words remained
    spi_port->SPIFFRX.bit.RXFIFORESET = 0;  //Rx FIFO hold reset
    spi_port->SPIFFRX.bit.RXFFOVFCLR = 1;   //clear Rx overflow interrupt
    spi_port->SPIFFRX.bit.RXFFINTCLR = 1;   //clear Rx receive interrupt
    spi_port->SPIFFRX.bit.RXFFIENA = 1;     //Rx FIFO interrupt enable
    spi_port->SPIFFRX.bit.RXFFIL = rcv_shred;//Rx FIFO interrupt when rcv_shred words remained
    spi_port->SPIFFTX.bit.TXFIFO = 1;       //Tx FIFO hold release
    spi_port->SPIFFRX.bit.RXFIFORESET = 1;  //Rx FIFO hold release
    /*      Delay        */
    spi_port->SPIFFCT.bit.TXDLY = 0;        //NO Tx delay
    spi_port->SPICCR.bit.SPISWRESET = 1;    //soft hold release
}


int bsp_spi_init(volatile struct SPI_REGS* spi_port,uint32_t baud,uint16_t rcv_shred, uint16_t isr_route)
{
    int res;
    /*      clk && isr      */
    res = bsp_spi_clk_init(spi_port);
    if(res == -1)
    {
        PERROR("spi clock init error!");
        return -1;
    }
    /*     spi      */
    bsp_spi_reset(spi_port,rcv_shred);
    res = bsp_spi_set_baud(spi_port,baud);
    if(res == -1)
    {
        PERROR("spi baud setting error!");
        return -1;
    }
    bsp_spi_talk(spi_port,1);
    return 1;
}
