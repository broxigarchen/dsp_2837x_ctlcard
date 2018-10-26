/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_uart.c
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * initialize bsp
##################################################################*/
#define BSP_UART_C_
#include "bsp/bsp_uart.h"
#include "bsp/bsp_clk.h"

#if UART_A_EN
extern __interrupt void uarta_rx_isr_handler(void);
extern __interrupt void uarta_tx_isr_handler(void);
#endif
#if UART_B_EN
extern __interrupt void uartb_rx_isr_handler(void);
extern __interrupt void uartb_tx_isr_handler(void);
#endif
#if UART_C_EN
extern __interrupt void uartc_rx_isr_handler(void);
extern __interrupt void uartc_tx_isr_handler(void);
#endif
#if UART_D_EN
extern __interrupt void uartd_rx_isr_handler(void);
extern __interrupt void uartd_tx_isr_handler(void);
#endif

int bsp_uart_clk_init(volatile struct SCI_REGS* sci_port,uint16_t isr_route)
{
    EALLOW;
    if((sci_port) == (&SciaRegs))
    {
        CpuSysRegs.PCLKCR7.bit.SCI_A = 1;    //clk
        DevCfgRegs.CPUSEL5.bit.SCI_A = isr_route; //cpu sel
        PieCtrlRegs.PIEIER9.bit.INTx1 = 1;   // PIE Group 9, INT1, RX
        PieCtrlRegs.PIEIER9.bit.INTx2 = 1;   // PIE Group 9, INT2, TX
        IER |= GetOneBitMask(9 - 1);           // Enable CPU INT
#if UART_A_EN
        PieVectTable.SCIA_RX_INT =  &uarta_rx_isr_handler;
        PieVectTable.SCIA_TX_INT =  &uarta_tx_isr_handler;
#endif
    }
    else if((sci_port) == (&ScibRegs))
    {
        CpuSysRegs.PCLKCR7.bit.SCI_B = 1;
        DevCfgRegs.CPUSEL5.bit.SCI_B = isr_route;
        PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   // PIE Group 9, INT3, RX
        PieCtrlRegs.PIEIER9.bit.INTx4 = 1;   // PIE Group 9, INT4, TX
        IER |= GetOneBitMask(9 - 1);           // Enable CPU INT
#if UART_B_EN
        PieVectTable.SCIB_RX_INT = &uartb_rx_isr_handler;
        PieVectTable.SCIB_TX_INT = &uartb_tx_isr_handler;
#endif
    }
    else if((sci_port) == (&ScicRegs))
    {
        CpuSysRegs.PCLKCR7.bit.SCI_C = 1;
        DevCfgRegs.CPUSEL5.bit.SCI_C = isr_route;
        PieCtrlRegs.PIEIER8.bit.INTx5 = 1;   // PIE Group 8, INT5, RX
        PieCtrlRegs.PIEIER8.bit.INTx6 = 1;   // PIE Group 8, INT6, TX
        IER |= GetOneBitMask(8 - 1);           // Enable CPU INT
#if UART_C_EN
        PieVectTable.SCIC_RX_INT = &uartc_rx_isr_handler;
        PieVectTable.SCIC_TX_INT = &uartc_tx_isr_handler;
#endif
    }
    else if((sci_port) == (&ScidRegs))
    {
        CpuSysRegs.PCLKCR7.bit.SCI_D = 1;
        DevCfgRegs.CPUSEL5.bit.SCI_D = isr_route;
        PieCtrlRegs.PIEIER8.bit.INTx7 = 1;   // PIE Group 8, INT7, RX
        PieCtrlRegs.PIEIER8.bit.INTx8 = 1;   // PIE Group 8, INT8, TX
        IER |= GetOneBitMask(8 - 1);           // Enable CPU INT
#if UART_D_EN
        PieVectTable.SCID_RX_INT = &uartd_rx_isr_handler;
        PieVectTable.SCID_TX_INT = &uartd_tx_isr_handler;
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

int bsp_uart_init(volatile struct SCI_REGS* sci_port,uint32_t baud,uint16_t parity,uint16_t rcv_shred, uint16_t isr_route)
{
    int res;
    /*      clk && isr      */
    res = bsp_uart_clk_init(sci_port,isr_route);
    if(res == -1)
    {
        PERROR("uart clock init error!");
        return -1;
    }
    /*     uart      */
    bsp_uart_reset(sci_port,rcv_shred);
    res = bsp_uart_set(sci_port,baud,parity);
    if(res == -1)
    {
        PERROR("uart a baud setting error!");
        return -1;
    }
    return 1;
}

/*
 * Baud rate = LSPCLK / ((BRR + 1) * 8), when 0 < BRR <= 65535
 * Baud rate = LSPCLK / 16, when BRR == 0
 */
int bsp_uart_set(volatile struct SCI_REGS* sci_port,uint64_t baud,uint16_t polarity)
{
    uint16_t  brr,brr_h,brr_l;
    float brr_approx,precision,fin_b,fin_b_l,fin_b_h;
    float lspclk = bsp_clk_get_lspclk() * 1000000;

    /*      baud        */
    if(baud == 0 || baud > CPU_GPIO_MAX_TOG_FREQ)  //maximum GPIO toggle frequency)
    {
        return -1;
    }
    brr_approx = lspclk / (8*baud);
    if(brr_approx > 1)
    {
        brr_approx  = brr_approx - 1;
        brr_l = (uint16_t)brr_approx;
    }
    else
    {
        brr_l = 0;
    }
    brr_h = brr_l + 1;
    fin_b_l = lspclk / ((brr_l + 1)*8);
    fin_b_h = lspclk / ((brr_h + 1)*8);
    if(fabs(fin_b_l - baud) < fabs(fin_b_h - baud))
    {
        fin_b = fin_b_l;
        brr = brr_l;
    }
    else
    {
        fin_b = fin_b_h;
        brr = brr_h;
    }
    precision = fabs(fin_b - baud)/baud;
    if(precision > UART_CLK_MAX_ERR)
    {
        return -1;
    }
    sci_port->SCIHBAUD.bit.BAUD = GetDataMoveR(brr & 0xff00,8);
    sci_port->SCILBAUD.bit.BAUD = GetDataMoveR(brr & 0x00ff,0);

    /*      polarity        */
    if(polarity == 1)
    {
        sci_port->SCICCR.bit.PARITYENA = 1;               //parity enable
        sci_port->SCICCR.bit.PARITY = 0;                  //parity odd
    }
    else if(polarity == 2)
    {
        sci_port->SCICCR.bit.PARITYENA = 1;               //parity enable
        sci_port->SCICCR.bit.PARITY = 1;                  //parity even
    }
    else
    {
        sci_port->SCICCR.bit.PARITYENA = 0;               //parity disable
    }
    return 1;
}


void bsp_uart_reset(volatile struct SCI_REGS* sci_port, uint16_t rx_trig_bytes)
{
    sci_port->SCICTL1.bit.SWRESET = 0;              //software reset
    /*      Frame Format & Control       */
    sci_port->SCICCR.bit.STOPBITS = 0;               //one stop bit
    sci_port->SCICCR.bit.ADDRIDLE_MODE = 0;          //general mode
    sci_port->SCICCR.bit.SCICHAR = 7;                //RX BUF 8-bits length
    sci_port->SCICTL1.bit.TXENA = 1;                 //TX enable
    sci_port->SCICTL1.bit.RXENA = 1;                 //RX enable
    sci_port->SCICTL2.bit.RXBKINTENA = 1;            //RX BUF interrupt enable
    sci_port->SCICTL2.bit.TXINTENA = 0;              //TX BUF interrupt disable

    /*      FIFO enable       */
    sci_port->SCIFFTX.bit.SCIRST = 1;                //TX FIFO continuous working
    sci_port->SCIFFTX.bit.SCIFFENA = 1;              //TX FIFO enhancement enable
    sci_port->SCIFFTX.bit.TXFIFORESET = 0;           //TX FIFO reset pointer and hold
    sci_port->SCIFFTX.bit.TXFFINTCLR = 1;            //TX FIFO clear interrupt
    sci_port->SCIFFTX.bit.TXFFIENA = 1;              //TX FIFO interrupt enable
    sci_port->SCIFFCT.bit.FFTXDLY = 0;               //TX FIFO transfer delay
    sci_port->SCIFFTX.bit.TXFFIL = 0;                //TX FIFO interrupt trigger bytes

    sci_port->SCIFFRX.bit.RXFFOVRCLR = 1;            //RX FIFO clear overflow
    sci_port->SCIFFRX.bit.RXFIFORESET = 0;           //RX FIFO reset pointer and hold
    sci_port->SCIFFRX.bit.RXFFINTCLR = 1;            //RX FIFO clear interrupt
    sci_port->SCIFFRX.bit.RXFFIENA = 1;              //RX FIFO interrupt enable
    sci_port->SCIFFRX.bit.RXFFIL = rx_trig_bytes;    //RX FIFO interrupt trigger bytes


    sci_port->SCIFFTX.bit.TXFIFORESET = 1;           //TX FIFO re-enable
    sci_port->SCIFFRX.bit.RXFIFORESET = 1;           //RX FIFO re-enable
    sci_port->SCICTL1.bit.SWRESET = 1;               //software reset release
}

