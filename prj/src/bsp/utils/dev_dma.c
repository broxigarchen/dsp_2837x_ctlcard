/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_dma.c
 * @AUTHOR  Brox Chen
 * @DATE    Aug 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * device manage: dma
##################################################################*/
#define DEV_DMA_C_
#include "bsp/utils/dev_dma.h"

/* ##################################################
 *   @device declare
 #################################################*/
/*
 * @ DMA device Id enumeration
 */

#define DEV_INFO_DMA(DEV_ID, DEV_NAME, PHY_PTR, DESC) \
    DEV_ID,
enum
{
#include "bsp/dev_info.h"
DMA_CH_MAX,
};
#undef DEV_INFO_DMA

/*
 * @ DMA physic pointer Array
 */

#define DEV_INFO_DMA(DEV_ID, DEV_NAME, PHY_PTR, DESC)\
    (void*)(PHY_PTR),
void* dma_phy_ptr[] = {
#include "bsp/dev_info.h"
};
#undef DEV_INFO_DMA

/*
 * @ DMA device Prototype Declare
 */

#define DEV_INFO_DMA(DEV_ID, DEV_NAME, PHY_PTR, DESC)\
        dev_dma_ch DEV_NAME;
#include "bsp/dev_info.h"
#undef DEV_INFO_DMA


/*
 * @ DMA device Prototype pointer
 */
dev_dma_ch* dev_dma_array[DMA_CH_MAX];


struct
{
    uint8_t   status[DMA_CH_MAX];
}dev_dma;


/* ##################################################
 *   @device check MACRO
 #################################################*/
#define  DEV_PTR_CHK(ptr)\
    if(ptr == NULL)    return 0;

/* ##################################################
 *   @internal functions
 #################################################*/
void dmach_default_cfg(int ch_id)
{
    bsp_DMACH_ISRConfig(dma_phy_ptr[ch_id], DMACH_ISR | DMACH_ISR_AT_END | DMACH_TRIG | DMACH_OVR_ISR);
    bsp_DMACH_ModeConfig(dma_phy_ptr[ch_id], DMACH_32BIT);
}

int dma_get_idle_ch(enum DMACH_ST next_st)  //chan 2-6
{
    int i;
    int res = -1;
    OS_ENTER_CIRTICAL();
    for(i=DMA_CH2;i<DMA_CH_MAX;i++)
    {
        if(dev_dma.status[i] == DMACH_IDLE)
        {
            dev_dma.status[i] = next_st;
            res = i;
            break;
        }
    }
    OS_EXIT_CIRTICAL();
    return res;
}

int dma_get_idle_hch(enum DMACH_ST next_st) //chan 1
{
    int res = -1;
    OS_ENTER_CIRTICAL();
    if(dev_dma.status[DMA_CH1] == DMACH_IDLE)
    {
        dev_dma.status[DMA_CH1] = next_st;
        res = 0;
    }
    OS_EXIT_CIRTICAL();
    return res;
}

void dma_free_ch(int ch_id)
{
    OS_ENTER_CIRTICAL();
    if(ch_id < 6)
    {
        if(dev_dma.status[ch_id] == DMACH_BIND)  //only free binded channel
            dev_dma.status[ch_id] = DMACH_IDLE;
    }
    OS_EXIT_CIRTICAL();
}

int dma_dat_cpy_ch(int ch_id,uint32_t* dst, uint32_t* src, size_t len,unsigned int opr)
{
    int mode = 0;
    uint16_t unit_size = 1;
    uint16_t burst_size, transfer_size;
    uint16_t burst_src_step, burst_dst_step;
    uint16_t transfer_src_step, transfer_dst_step;

    bsp_DMACH_TrigSrcSel(dma_phy_ptr[ch_id],DMA_RAM);

    // bit width
    if(opr | DMA_32)
    {
        mode |= DMACH_32BIT;
        unit_size = 2;
    }
    // one shot mode
    if(opr | DMA_ATOM) mode |= DMACH_1SHOT;

    burst_size = unit_size;
    burst_src_step = unit_size;
    burst_dst_step = unit_size;
    transfer_size = len;
    transfer_src_step = unit_size;
    transfer_dst_step = unit_size;

    bsp_DMACH_AddrConfig(dma_phy_ptr[ch_id],dst,src);
    bsp_DMACH_BurstConfig(dma_phy_ptr[ch_id],burst_size,burst_src_step,burst_dst_step);
    bsp_DMACH_TransferConfig(dma_phy_ptr[ch_id],transfer_size,transfer_src_step,transfer_dst_step);
    bsp_DMACH_ModeConfig(dma_phy_ptr[ch_id],mode);
    bsp_DMACH_Start(dma_phy_ptr[ch_id]);
    return 1;
}

int dma_dev_bind_ch(int ch_id,enum DMA_SRC dev,uint32_t* dst, uint32_t* src,  size_t len,  unsigned int opr)
{
    int mode = 0;
    uint16_t unit_size = 1;
    uint16_t burst_size, transfer_size;
    uint16_t burst_src_step, burst_dst_step;
    uint16_t transfer_src_step, transfer_dst_step;

    bsp_DMACH_TrigSrcSel(dma_phy_ptr[ch_id],dev);

    // bit width
    if(opr | DMA_32)
    {
        mode |= DMACH_32BIT;
        unit_size = 2;
    }
    // bind or not
    if(opr | DMA_BIND)
    {
        mode |= DMACH_CONT;
    }
    // R/W decide step size
    if(opr | DMA_R)
    {
        burst_src_step = 0;
        burst_dst_step = unit_size;
        transfer_src_step = 0;
        transfer_dst_step = unit_size;
    }
    else
    {
        burst_src_step = unit_size;
        burst_dst_step = 0;
        transfer_src_step = unit_size;
        transfer_dst_step = 0;
    }
    // one shot mode
    if(opr | DMA_ATOM) mode |= DMACH_1SHOT;
    burst_size = unit_size;
    transfer_size = len;
    bsp_DMACH_AddrConfig(dma_phy_ptr[ch_id],dst,src);
    bsp_DMACH_BurstConfig(dma_phy_ptr[ch_id],burst_size,burst_src_step,burst_dst_step);
    bsp_DMACH_TransferConfig(dma_phy_ptr[ch_id],transfer_size,transfer_src_step,transfer_dst_step);
    bsp_DMACH_ModeConfig(dma_phy_ptr[ch_id],mode);
    bsp_DMACH_Start(dma_phy_ptr[ch_id]);
    return 1;
}
/* ##################################################
 *   @hardware handler
 *   shared between devices, needs to be reentrant
 #################################################*/
void dma_ch_isr_handler(int ch_id)
{
    dev_dma_ch* ptr = dev_dma_array[ch_id];
    if(ptr->status == DMACH_BIND)
    {
        //do nothing
    }
    else if(ptr->status == DMACH_RUN)
    {
        ptr->status = DMACH_IDLE;
        bsp_DMACH_halt(dma_phy_ptr[ch_id]);
    }
    if(ptr->callback)
    {
        ptr->callback(ch_id);
    }
}

/* ##################################################
 *   @device methods
 #################################################*/
/*  memory data copy  */
int dma_memcpy(uint32_t* dst, uint32_t* src, size_t len,unsigned int opr, void(*cb)(int))
{
    DEV_PTR_CHK(src);
    DEV_PTR_CHK(dst);
    int ch_id = dma_get_idle_ch(DMACH_RUN);
    if(ch_id < 0)
    {
        return -1;
    }
    dev_dma_array[ch_id]->callback = cb;
    return dma_dat_cpy_ch(ch_id,dst,src,len,opr);
}

/*  memory data copy high priority */
int dma_memcpy_h(uint32_t* dst, uint32_t* src, size_t len,unsigned int opr,void(*cb)(int))
{
    DEV_PTR_CHK(src);
    DEV_PTR_CHK(dst);
    if(dma_get_idle_hch(DMACH_RUN) < 0)
    {
        return -1;
    }
    dev_dma_array[DMA_CH1]->callback = cb;
    return dma_dat_cpy_ch(DMA_CH1,dst,src,len,opr);
}

/*  device data copy  */
int dma_dev_cpy(enum DMA_SRC dev, uint32_t* dst, uint32_t* src, size_t len,unsigned int opr,void(*cb)(int))
{
    DEV_PTR_CHK(src);
    DEV_PTR_CHK(dst);
    int ch_id;
    enum DMACH_ST next_st = DMACH_RUN;
    if(opr | DMA_BIND)
    {
        next_st = DMACH_BIND;
    }
    ch_id = dma_get_idle_ch(next_st);
    if(ch_id < 0)
    {
        return -1;
    }
    dev_dma_array[ch_id]->callback = cb;
    return dma_dev_bind_ch(ch_id,dev,dst, src,len,opr);
}

/*  dev bind high priority  */
int dma_dev_cpy_h(enum DMA_SRC dev,uint32_t* dst, uint32_t* src, size_t len,unsigned int opr,void(*cb)(int))
{
    DEV_PTR_CHK(src);
    DEV_PTR_CHK(dst);
    enum DMACH_ST next_st = DMACH_RUN;
    if(opr | DMA_BIND)
    {
       next_st = DMACH_BIND;
    }
    if(dma_get_idle_hch(next_st) < 0)
    {
        return -1;
    }
    dev_dma_array[DMA_CH1]->callback = cb;
    return dma_dev_bind_ch(DMA_CH1,dev,dst, src,len,opr);
}

void dma_dev_free(int ch_id)
{
    dma_free_ch(ch_id);
}

int dev_dma_init(void)
{
    int res;
      /*
       * @ spi device Prototype Initialize
       */
   #define  DEV_INFO_DMA(DEV_ID, DEV_NAME, PHY_PTR, DESC)\
            dev_dma_array[DEV_ID] = &DEV_NAME;\
            memset(&DEV_NAME,0,sizeof(dev_dma_ch));\
            strcpy(DEV_NAME.dev, DESC);\
            strcpy(DEV_NAME.desc,"Device Type: dma");\
            res = bsp_DMACH_Init(PHY_PTR);\
            if(res == 1) {\
                dmach_default_cfg(DEV_ID);\
               }
    #include "bsp/dev_info.h"
    #undef DEV_INFO_DMA
    return 1;
}


/*##################################################
 *   @hardware interrupt
 #################################################*/
__interrupt void dma_ch1_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
    dma_ch_isr_handler(DMA_CH1);
}

__interrupt void dma_ch2_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
    dma_ch_isr_handler(DMA_CH2);
}

__interrupt void dma_ch3_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
    dma_ch_isr_handler(DMA_CH3);
}

__interrupt void dma_ch4_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
    dma_ch_isr_handler(DMA_CH4);
}

__interrupt void dma_ch5_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
    dma_ch_isr_handler(DMA_CH5);
}

__interrupt void dma_ch6_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
    dma_ch_isr_handler(DMA_CH6);
}
