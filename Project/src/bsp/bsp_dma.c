/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_dma.c
 * @AUTHOR  Brox Chen
 * @DATE    July 18, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
##################################################################*/
#define BSP_DMA_C_
#include "bsp/bsp_dma.h"

extern __interrupt void dma_ch1_isr_handler(void);
extern __interrupt void dma_ch2_isr_handler(void);
extern __interrupt void dma_ch3_isr_handler(void);
extern __interrupt void dma_ch4_isr_handler(void);
extern __interrupt void dma_ch5_isr_handler(void);
extern __interrupt void dma_ch6_isr_handler(void);

int bsp_dma_clk_init(struct CH_REGS* dma_ch)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.DMA = 1;          //clk
    if((dma_ch) == (&DmaRegs.CH1))
    {
        PieCtrlRegs.PIEIER7.bit.INTx1 = 1;    // PIE Group 7, INT1
        IER |= GetOneBitMask(7 - 1);           // Enable CPU INT
        PieVectTable.DMA_CH1_INT =  &dma_ch1_isr_handler;
    }
    else if((dma_ch) == (&DmaRegs.CH2))
    {
        PieCtrlRegs.PIEIER7.bit.INTx2 = 1;   // PIE Group 7, INT2
        IER |= GetOneBitMask(7 - 1);           // Enable CPU INT
        PieVectTable.DMA_CH2_INT = &dma_ch2_isr_handler;
    }
    else if((dma_ch) == (&DmaRegs.CH3))
    {
        PieCtrlRegs.PIEIER7.bit.INTx3 = 1;  // PIE Group 7, INT3
        IER |= GetOneBitMask(7 - 1);           // Enable CPU INT
        PieVectTable.DMA_CH3_INT = &dma_ch3_isr_handler;
    }
    else if((dma_ch) == (&DmaRegs.CH4))
    {
        PieCtrlRegs.PIEIER7.bit.INTx4 = 1;   // PIE Group 7, INT4
        IER |= GetOneBitMask(7 - 1);           // Enable CPU INT
        PieVectTable.DMA_CH4_INT = &dma_ch4_isr_handler;
    }
    else if((dma_ch) == (&DmaRegs.CH5))
    {
        PieCtrlRegs.PIEIER7.bit.INTx5 = 1;   // PIE Group 7, INT5
        IER |= GetOneBitMask(7 - 1);           // Enable CPU INT
        PieVectTable.DMA_CH5_INT = &dma_ch5_isr_handler;
    }
    else if((dma_ch) == (&DmaRegs.CH6))
    {
        PieCtrlRegs.PIEIER7.bit.INTx6 = 1;   // PIE Group 7, INT6
        IER |= GetOneBitMask(7 - 1);           // Enable CPU INT
        PieVectTable.DMA_CH6_INT = &dma_ch6_isr_handler;
    }
    else
    {
        EDIS;
        return -1;
    }
    EDIS;
    return 1;
}


// DMA channel source select
void bsp_DMACH_TrigSrcSel(struct CH_REGS* ptr, enum DMA_SRC src)
{
    EALLOW;
    if(ptr == &DmaRegs.CH1)
    {
        DmaClaSrcSelRegs.DMACHSRCSEL1.bit.CH1 = src;
        ptr->MODE.bit.PERINTSEL = 1;                        // PERINTSEL - Should be hard coded to channel
    }
    else if(ptr == &DmaRegs.CH2)
    {
        DmaClaSrcSelRegs.DMACHSRCSEL1.bit.CH2 = src;
        ptr->MODE.bit.PERINTSEL = 2;
    }
    else if(ptr == &DmaRegs.CH3)
    {
        DmaClaSrcSelRegs.DMACHSRCSEL1.bit.CH3 = src;
        ptr->MODE.bit.PERINTSEL = 3;
    }
    else if(ptr == &DmaRegs.CH4)
    {
        DmaClaSrcSelRegs.DMACHSRCSEL1.bit.CH4 = src;
        ptr->MODE.bit.PERINTSEL = 4;
    }
    else if(ptr == &DmaRegs.CH5)
    {
        DmaClaSrcSelRegs.DMACHSRCSEL2.bit.CH5 = src;
        ptr->MODE.bit.PERINTSEL = 5;
    }
    else if(ptr == &DmaRegs.CH6)
    {
        DmaClaSrcSelRegs.DMACHSRCSEL2.bit.CH6 = src;
        ptr->MODE.bit.PERINTSEL = 6;
    }
    EDIS;
}


//
//  DMA Channel Mode configuration
//
void bsp_DMACH_ModeConfig(struct CH_REGS* ptr, uint16_t para)
{
    EALLOW;

    // Set up MODE Register:
    // ONESHOT - Oneshot enable
    // CONTINUOUS - Continuous enable
    // DATASIZE - 16-bit/32-bit data size transfers
    ptr->MODE.all &= ~( DMACH_32BIT | DMACH_CONT | DMACH_1SHOT);

    ptr->MODE.all |= para;   //check "DMA config parameter"

    EDIS;
}

//
//  DMA Channel Mode configuration
//
void bsp_DMACH_ISRConfig(struct CH_REGS* ptr, uint16_t para)
{
    EALLOW;

    // Set up MODE Register:
    // PERINTE - Peripheral interrupt enable
    // OVRINTE - Enable/disable the overflow interrupt
    // CHINTMODE - Generate interrupt to CPU at beginning/end of transfer
    // CHINTE - Channel Interrupt to  CPU enable
    ptr->MODE.all &= ~(DMACH_ISR | DMACH_ISR_AT_END | DMACH_TRIG | DMACH_OVR_ISR);   //clear corresponding bits


    ptr->MODE.all |= para;   //check "DMA config parameter"
    //
    // Clear any spurious flags: interrupt and sync error flags
    //
    ptr->CONTROL.bit.PERINTCLR = 1;
    ptr->CONTROL.bit.ERRCLR = 1;

    EDIS;
}

//
// DMA Channel Address Configuration
//
void bsp_DMACH_AddrConfig(struct CH_REGS* ptr,volatile uint32_t *DMA_Dest,volatile uint32_t *DMA_Source)
{
    EALLOW;

    //
    // Set up SOURCE address:
    //
    ptr->SRC_BEG_ADDR_SHADOW = (uint32_t)DMA_Source;   // Point to
                                                            // beginning of
                                                            // source buffer
    ptr->SRC_ADDR_SHADOW =     (uint32_t)DMA_Source;

    //
    // Set up DESTINATION address:
    //
    ptr->DST_BEG_ADDR_SHADOW = (uint32_t)DMA_Dest;  // Point to
                                                         // beginning of
                                                         // destination buffer
    ptr->DST_ADDR_SHADOW =     (uint32_t)DMA_Dest;

    EDIS;
}

//
//  DMA Channel  Burst size configuration
//
void bsp_DMACH_BurstConfig(struct CH_REGS* ptr,uint16_t bsize, int16_t srcbstep, int16_t desbstep)
{
    EALLOW;

    //
    // Set up BURST registers:
    //
    ptr->BURST_SIZE.all = bsize;      // Number of words(X-1)
                                             // x-ferred in a burst.
    ptr->SRC_BURST_STEP = srcbstep;   // Increment source addr between
                                             // each word x-ferred.
    ptr->DST_BURST_STEP = desbstep;   // Increment dest addr between
                                             // each word x-ferred.

    EDIS;
}

//
// DMA Channel Transfer size configuration
//
void bsp_DMACH_TransferConfig(struct CH_REGS* ptr,uint16_t tsize, int16_t srctstep, int16_t deststep)
{
    EALLOW;

    //
    // Set up TRANSFER registers:
    //
    ptr->TRANSFER_SIZE = tsize;        // Number of bursts per transfer,
                                              // DMA interrupt will occur after
                                              // completed transfer.
    ptr->SRC_TRANSFER_STEP = srctstep; // TRANSFER_STEP is ignored
                                              // when WRAP occurs.
    ptr->DST_TRANSFER_STEP = deststep; // TRANSFER_STEP is ignored
                                              // when WRAP occurs.

    EDIS;
}

//
//  DMA ChannelWrap size configuration
//
void bsp_DMACH_WrapConfig(struct CH_REGS* ptr,uint16_t srcwsize, int16_t srcwstep, uint16_t deswsize,
                          int16_t deswstep)
{
    EALLOW;

    //
    // Set up WRAP registers:
    //
    ptr->SRC_WRAP_SIZE = srcwsize; // Wrap source address after N bursts
    ptr->SRC_WRAP_STEP = srcwstep; // Step for source wrap

    ptr->DST_WRAP_SIZE = deswsize; // Wrap destination address after
                                          // N bursts.
    ptr->DST_WRAP_STEP = deswstep; // Step for destination wrap

    EDIS;
}

//
// This function starts DMA Channel
//
void bsp_DMACH_Start(struct CH_REGS* ptr)
{
    EALLOW;
    ptr->CONTROL.bit.RUN = 1;
    EDIS;
}


int bsp_DMACH_Init(struct CH_REGS* ptr)
{
    return bsp_dma_clk_init(ptr);
}


