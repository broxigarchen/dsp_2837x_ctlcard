/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_spi.c
 * @AUTHOR  Brox Chen
 * @DATE    July 18, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * device manage: spi
##################################################################*/
#define DEV_SPI_C_
#include "bsp/utils/dev_spi.h"
#include "bsp/utils/dev_dma.h"
#include "bsp/bsp_gpio.h"

/* ##################################################
 *   @device declare
 #################################################*/
/*
 * @ SPI device Id enumeration
 */
#define DEV_INFO_SPI(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
    DEV_ID,
enum
{
#include "bsp/dev_info.h"
SPI_MAX,
};
#undef DEV_INFO_SPI

/*
 * @ SPI physic pointer Array
 */

#define DEV_INFO_SPI(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
    (void*)PHY_PTR,
void* spi_phy_ptr[] = {
#include "bsp/dev_info.h"
};
#undef DEV_INFO_SPI

/*
 * @ SPI device Prototype Declare
 */

#define DEV_INFO_SPI(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
        dev_sup_io DEV_NAME;
#include "bsp/dev_info.h"
#undef DEV_INFO_SPI

/*
 * @ SPI device Prototype pointer
 */
dev_sup_io* dev_spi_array[SPI_MAX];

/*
 * @ SPI device superior control Declare
 */
#define DEV_INFO_SPI(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
        dev_spi_ctrl_t dev_ctrl_ ##  DEV_NAME;
#include "bsp/dev_info.h"
#undef DEV_INFO_SPI


/*
 * @ SPI device buffer Declare and Pointer Declare
 */
#define DEV_INFO_SPI(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
    char DEV_NAME ## _in[SPI_NODE_NUM * DEV_EN * SPI_IN_BUF_SIZE];\
    char DEV_NAME ## _out[SPI_NODE_NUM * DEV_EN * SPI_OUT_BUF_SIZE];\
    static iobuf_ctrl DEV_NAME ## nodein[SPI_NODE_NUM];\
    static iobuf_ctrl DEV_NAME ## nodeout[SPI_NODE_NUM];
#include "bsp/dev_info.h"
#undef DEV_INFO_SPI

/*
 * @ SPI device Operations
 */
#define SPI_OP_MAX  __Array_Count(spi_operation,sizeof(int))
int spi_operation[]={
    IO_RESET, IO_DMA,
    IO_SPI_MODE,  IO_SPI_BUS, IO_SPI_POLARITY,
    IO_SPI_WIDTH, IO_SPI_BIND,IO_SPI_BIND_USR,
};
/* ##################################################
 *   @device check MACRO
 #################################################*/
#define  DEV_CHK(dev_id)\
    if(dev_id >=  SPI_MAX || dev_id < 0)    return 0;
#define  DEV_CHK_NORET(dev_id)\
    if(dev_id >=  SPI_MAX || dev_id < 0)    return;
#define  DEV_PTR_CHK(ptr)\
    if(ptr == NULL)    return 0;
#define DEV_STATUS_CHK(ptr,ret)\
        if(ptr->ctrl.bit.ready == 0)   return ret;
#define DEV_OPER_CHK(op_id)\
        if(spi_oper_chk(op_id) == 0) return 0;
#define DEV_NODE_CHK(node_id)\
        if(node_id >= SPI_NODE_NUM) return 0;
/* ##################################################
 *   @internal functions
 #################################################*/
uint16_t spi_oper_chk(int op_id)
{
    int i;
    for(i=0;i<SPI_OP_MAX;i++)
    {
        if(op_id == spi_operation[i])
            return 1;
    }
    return 0;
}

void spi_node_fifo_init(int dev_id)
{
    dev_sup_io* dev_ptr =   dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    buf_ctrl_init(&sup_ptr->nodefifosnd.ctl,DEV_SPI_NODEFIFO_LEN);
    memset((void*)&(sup_ptr->nodefifosnd.curr),0,sizeof(dev_spi_nodedat_t));
}

void spi_node_fifo_push(dev_spi_nodefifo_t* ptr,enum SPI_NODE node_id,size_t len)
{
    uint16_t id = buf_ctrl_in(&ptr->ctl);
    ptr->buf[id].id = node_id;
    ptr->buf[id].len = len;
}
void spi_node_fifo_pop(dev_spi_nodefifo_t* ptr,enum SPI_NODE* node_id,size_t* len)
{
    uint16_t id = buf_ctrl_out(&ptr->ctl);
    *node_id = ptr->buf[id].id;
    *len = ptr->buf[id].len;
}

void spi_node_select(int dev_id,enum SPI_NODE node_id,char en_dis)
{
    dev_sup_io* dev_ptr =   dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    if(sup_ptr->bus == SPI_PRIVATE || sup_ptr->mode == SPI_SLAVE)
    {
        return;
    }
    if(sup_ptr->node[node_id])
    {
        if(sup_ptr->user_func == NULL)
        {
            bsp_gpio_set(sup_ptr->pin_en[node_id],!en_dis);
        }
        else
        {
            sup_ptr->user_func(node_id,en_dis);
        }
    }
}

void spi_dma_init(int dev_id)
{
    dev_sup_io* dev_ptr =   dev_spi_array[dev_id];
    dev_ptr->dma_snd.channel = 0;
    dev_ptr->dma_snd.dst = bsp_spi_get_dma_sendaddr(spi_phy_ptr[dev_id]);
    dev_ptr->dma_snd.status = 0;

    dev_ptr->dma_rcv.channel = 0;
    dev_ptr->dma_rcv.src = bsp_spi_get_dma_rcvaddr(spi_phy_ptr[dev_id]);
    dev_ptr->dma_rcv.status = 0;
}

void spi_default(int dev_id)
{
    spi_ioctl(dev_id,IO_SPI_MODE,SPI_MASTER);
    spi_ioctl(dev_id,IO_SPI_BUS,SPI_PRIVATE);
    spi_ioctl(dev_id,IO_SPI_POLARITY,SPI_ROFI);
    spi_ioctl(dev_id,IO_SPI_WIDTH,SPI_8_BIT);
}

void spi_buf_reset(int dev_id)
{
    int i;
    dev_sup_io* ptr =   dev_spi_array[dev_id];
    for(i=0;i<SPI_NODE_NUM;i++)
    {
        buf_ctrl_reset(&ptr->ptr_in[i]);
        buf_ctrl_reset(&ptr->ptr_out[i]);
    }
    spi_node_fifo_init(dev_id);
}

void spi_reset(int dev_id)
{
    spi_buf_reset(dev_id);
    spi_dma_init(dev_id);
    spi_default(dev_id);
}

uint16_t spi_wait2send(int dev_id)
{
    dev_sup_io* dev_ptr =   dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    if(sup_ptr->bus == SPI_BUS)
    {
        return (buf_ctrl_wait(&sup_ptr->nodefifosnd.ctl) || (sup_ptr->nodefifosnd.curr.len > 0)) ;
    }
    else
    {
        return buf_ctrl_wait(&dev_ptr->ptr_out[SPI_NODE_0]);
    }
}

uint16_t spi_get_sendbyte(int dev_id)
{
    dev_sup_io* dev_ptr =   dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    enum SPI_NODE node_id;
    if(sup_ptr->bus == SPI_BUS && sup_ptr->mode == SPI_MASTER)
    {
        if(sup_ptr->nodefifosnd.curr.len == 0)    //current node sending is end, load next one
        {
            spi_node_select(dev_id,sup_ptr->nodefifosnd.curr.id,0);  //disable last node
            spi_node_fifo_pop(&sup_ptr->nodefifosnd,&(sup_ptr->nodefifosnd.curr.id),(size_t*)&(sup_ptr->nodefifosnd.curr.len));
            spi_node_select(dev_id,sup_ptr->nodefifosnd.curr.id,1);  //enable current node
        }
        node_id = sup_ptr->nodefifosnd.curr.id;
        sup_ptr->nodefifosnd.curr.len--;
    }
    else
    {
        node_id = SPI_NODE_0;        //slave, or master with private, default node 0
    }
    return dev_ptr->buf_out[node_id * SPI_IN_BUF_SIZE + buf_ctrl_out(&dev_ptr->ptr_out[node_id])];
}


int spi_dma_getdev(int ch_id)
{
    uint8_t i;
    dev_sup_io* dev_ptr;
    for(i=0;i<SPI_MAX;i++)
    {
        dev_ptr =  dev_spi_array[i];
        if(dev_ptr->dma_snd.channel == ch_id ||
                dev_ptr->dma_rcv.channel == ch_id)
            return i;
    }
    return -1;
}

int spi_dma_send(int dev_id)
{
    int res;
    dev_sup_io* dev_ptr = dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    size_t len;

    dev_ptr->dma_snd.curr_pos += dev_ptr->dma_snd.curr_size;
    len = dev_ptr->dma_snd.len - dev_ptr->dma_snd.curr_pos;
    if(len)
    {
        if(dev_ptr->dma_snd.status == 1)  //if pending, first time enter
        {
            spi_node_select(dev_id,sup_ptr->nodefifosnd.curr.id,0);  //disable last node
            spi_node_select(dev_id,sup_ptr->dma_ctrl.id,1);          //enable current node
        }
        if(len > DMA_MAX_LEN)
        {
            len = DMA_MAX_LEN;
        }
        dev_ptr->dma_snd.curr_size = len;
        res = dma_dev_cpy(DMA_SPIATX + 2*(dev_id - SPI_A),
                           &(((struct SPI_REGS*)spi_phy_ptr[dev_id])->SPITXBUF),
                           &(((uint16_t*)dev_ptr->dma_snd.src)[dev_ptr->dma_snd.curr_pos]),
                           dev_ptr->dma_snd.curr_size,
                           DMA_16 | DMA_W,
                           spi_dma_handler_send);
        if(res != -1)
        {
            dev_ptr->dma_snd.status = 2;
            return 1;
        }
    }
    //fail or end
    dev_ptr->dma_snd.status = 0;
    spi_node_select(dev_id,sup_ptr->dma_ctrl.id,0);          //disable dma  node
    spi_node_select(dev_id,sup_ptr->nodefifosnd.curr.id,1);  //enable previous node
    return -1;
}
int spi_dma_receive(int dev_id)
{
   int res;
   dev_sup_io* dev_ptr = dev_spi_array[dev_id];
   dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
   size_t len;

   dev_ptr->dma_rcv.curr_pos += dev_ptr->dma_rcv.curr_size;
   len = dev_ptr->dma_rcv.len - dev_ptr->dma_rcv.curr_pos;
   if(len)
   {
       if(dev_ptr->dma_rcv.status == 1)  //if pending, first time enter
       {
           spi_node_select(dev_id,sup_ptr->nodefifosnd.curr.id,0);  //disable last node
           spi_node_select(dev_id,sup_ptr->dma_ctrl.id,1);          //enable current node
       }
       if(len > DMA_MAX_LEN)
       {
           len = DMA_MAX_LEN;
       }
       dev_ptr->dma_rcv.curr_size = len;
       res = dma_dev_cpy(DMA_SPIARX + 2*(dev_id - SPI_A),
                         &(((uint16_t*)dev_ptr->dma_rcv.src)[dev_ptr->dma_rcv.curr_pos]),
                         &(((struct SPI_REGS*)spi_phy_ptr[dev_id])->SPIRXBUF),
                          dev_ptr->dma_rcv.curr_size,
                          DMA_16 | DMA_R,
                          spi_dma_handler_receive);
       if(res != -1)
       {
           dev_ptr->dma_rcv.status = 2;
           return 1;
       }
   }
   //fail or end
   dev_ptr->dma_rcv.status = 0;
   spi_node_select(dev_id,sup_ptr->dma_ctrl.id,0);          //disable dma  node
   spi_node_select(dev_id,sup_ptr->nodefifosnd.curr.id,1);  //enable previous node
   return -1;
}


void spi_dma_trig(int dev_id,int node_id)
{
    dev_sup_io* dev_ptr = dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    sup_ptr->dma_ctrl.id = node_id;   //save node_id
    if(bsp_spi_tx_empty(spi_phy_ptr[dev_id]))   //idle, start dma
    {
        dev_ptr->dma_snd.status = 2;         //running
        dev_ptr->dma_rcv.status = 2;
        spi_node_select(dev_id,sup_ptr->nodefifosnd.curr.id,0);  //disable last node
        spi_node_select(dev_id,sup_ptr->dma_ctrl.id,1);          //enable current node
        spi_dma_send(dev_id);
        spi_dma_receive(dev_id);
    }
    else
    {
        dev_ptr->dma_snd.status = 1;  //pending, wait for tx switch to dma
        dev_ptr->dma_rcv.status = 1;
    }
}



/* ##################################################
 *   @hardware handler
 *   shared between devices, needs to be reentrant
 #################################################*/
 /*
  * @Sent by Tx interrupt
  */
inline int  spi_handler_sendbyte(int dev_id)
{
    dev_sup_io* dev_ptr = dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    if(dev_ptr->dma_snd.status)
    {
        if(dev_ptr->dma_snd.status == 1)   //if pending, start dma sending and halt tx, otherwise ignore
        {
            spi_dma_send(dev_id);
        }
        return -1;
    }
    if(spi_wait2send(dev_id))
    {
        bsp_spi_writechar(spi_phy_ptr[dev_id],spi_get_sendbyte(dev_id),sup_ptr->width);
        return 1;
    }
    return -1;
}
/*
 * @Sent by DMA interrupt
 */
void  spi_dma_handler_send(int ch_id)
{
    int dev_id = spi_dma_getdev(ch_id);
    if(dev_id != -1)
    {
        spi_dma_send(dev_id);
    }
}
/*
 * @Sent by user
 */
int spi_trig_send(int dev_id)
{
    if(bsp_spi_writerdy(spi_phy_ptr[dev_id])
    {
       spi_handler_sendbyte(dev_id);
       return 1;
    }
    return -1;
}
/*
 * @Received by Rx interrupt
 */
inline void  spi_handler_readbyte(int dev_id)
{
    uint8_t byte;
    dev_sup_io* dev_ptr = dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    volatile int node_id = SPI_NODE_0;

    bsp_spi_readchar(spi_phy_ptr[dev_id],&byte,sup_ptr->width);
    /*  in bus mode, current node needs to be updated every byte, use volatile variable */
    if(sup_ptr->bus == SPI_BUS && sup_ptr->mode == SPI_MASTER)
    {
        node_id = sup_ptr->nodefifosnd.curr.id;
    }
    if(!buf_ctrl_full(&dev_ptr->ptr_in[node_id]))
    {
        dev_ptr->buf_in[SPI_IN_BUF_SIZE * node_id + buf_ctrl_out(&dev_ptr->ptr_in[node_id])] = byte;
    }
}

/*
 * @Received by dma
 */
void  spi_dma_handler_receive(int ch_id)
{
    int dev_id = spi_dma_getdev(ch_id);
    if(dev_id != -1)
    {
        spi_dma_receive(dev_id);
    }
}

/* ##################################################
 *   @device methods
 #################################################*/
size_t  spi_write(int dev_id, int node_id, char* buf_in, size_t len)
{
    DEV_CHK(dev_id);
    dev_sup_io* dev_ptr =   dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    DEV_STATUS_CHK(dev_ptr,0);
    DEV_NODE_CHK(node_id);
    size_t write_len = 0;
    if(sup_ptr->bus == SPI_BUS && sup_ptr->mode == SPI_MASTER)  //master with bus mode
    {
        if(buf_ctrl_full(&sup_ptr->nodefifosnd.ctl))
        {
            return 0;
        }
    }
    else              //slave or master with private mode, default set node 0
    {
        node_id = SPI_NODE_0;
    }
    while(!buf_ctrl_full(&dev_ptr->ptr_out[node_id]) && write_len < len)         //load data into buffer
    {
        dev_ptr->buf_out[SPI_IN_BUF_SIZE * node_id + buf_ctrl_in(&dev_ptr->ptr_out[node_id])] = *buf_in++;
        write_len ++;
    }
    if(write_len && sup_ptr->bus == SPI_BUS && sup_ptr->mode == SPI_MASTER)   //if bus mode, load node FIFO
    {
        spi_node_fifo_push(&sup_ptr->nodefifosnd,(enum SPI_NODE)node_id,write_len);
    }
    spi_trig_send(dev_id);
    return write_len;
}


size_t  spi_read(int dev_id , int node_id, char* buf_out, size_t max_len)
{
    DEV_CHK(dev_id);
    dev_sup_io* dev_ptr =   dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    DEV_STATUS_CHK(dev_ptr,0);
    DEV_NODE_CHK(node_id);
    size_t read_len = 0;
    char* read_ptr = buf_out;
    switch(sup_ptr->bus)
    {
    case SPI_BUS:   //if bus mode && node FIFO full, quit
    {
        if(!buf_ctrl_wait(&sup_ptr->nodefifosnd.ctl))
        {
            return 0;
        }
    }
        break;
    case SPI_PRIVATE:               //private mode, default set node 0
    {
        node_id = SPI_NODE_0;
    }
        break;
    }
    while(buf_ctrl_wait(&dev_ptr->ptr_in[node_id]) && read_len < max_len)         //load data into buffer
    {
        *read_ptr++ = dev_ptr->buf_in[SPI_IN_BUF_SIZE * node_id + buf_ctrl_out(&dev_ptr->ptr_in[node_id])] ;
        read_len ++;
    }
    return read_len;
}

int  spi_listen(int dev_id, int node_id)
{
    DEV_CHK(dev_id);
    dev_sup_io* dev_ptr =   dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    DEV_STATUS_CHK(dev_ptr,-1);
    DEV_NODE_CHK(node_id);
    switch(sup_ptr->bus)
    {
    case SPI_BUS:   //if bus mode && node FIFO full, quit
    {
        if(!buf_ctrl_wait(&sup_ptr->nodefifosnd.ctl))
        {
            return 0;
        }
    }
        break;
    case SPI_PRIVATE:               //private mode, default set node 0
    {
        node_id = SPI_NODE_0;
    }
        break;
    }

    return 0;
}

void spi_flush(int dev_id)
{
    DEV_CHK_NORET(dev_id);
    spi_buf_reset(dev_id);
}

int spi_vioctl(int dev_id,int opr,va_list arg)
{
    int res = -1;
    DEV_CHK(dev_id);
    DEV_OPER_CHK(opr);
    dev_sup_io* dev_ptr = dev_spi_array[dev_id];
    dev_spi_ctrl_t* sup_ptr = dev_ptr->supctrl;
    void* phy_ptr = spi_phy_ptr[dev_id];
    DEV_STATUS_CHK(dev_ptr,-1);
    switch(opr)
    {
    case IO_RESET:
    {
        spi_reset(dev_id);
        res = 1;
    }
        break;
    case IO_DMA:
    {
        int node_id;
        if(dev_ptr->ctrl.bit.ready == 0)        //device not ready
        {
            break;
        }
        if(dev_ptr->dma_snd.status == 1 || dev_ptr->dma_rcv.status == 1)  //transmitting
        {
            break;
        }
        node_id = va_arg(arg,int);
        dev_ptr->dma_snd.src = va_arg(arg,addr_t);
        dev_ptr->dma_snd.len  = va_arg(arg,size_t);
        dev_ptr->dma_snd.curr_pos = 0;
        dev_ptr->dma_rcv.dst = va_arg(arg,addr_t);
        dev_ptr->dma_rcv.len = va_arg(arg,size_t);
        dev_ptr->dma_rcv.curr_pos = 0;
        if(sup_ptr->mode != SPI_MASTER || sup_ptr->bus != SPI_BUS)
        {
            node_id = 0;
        }
        spi_dma_trig(dev_id,node_id);
    }
        break;
    case IO_SPI_MODE:
    {
        int spi_mode = va_arg(arg,int);
        if(spi_mode <= SPI_MASTER && sup_ptr->mode != spi_mode)
        {
            sup_ptr->mode = (enum SPI_MODE)spi_mode;
            if(spi_mode == SPI_MASTER)
            {
                bsp_spi_set_mode(phy_ptr,BSP_SPI_MASTER);
            }
            else
            {
                bsp_spi_set_mode(phy_ptr,BSP_SPI_SLAVE);
                spi_ioctl(dev_id,IO_SPI_BUS,SPI_PRIVATE);
            }
            res = 1;
        }
    }
        break;
    case IO_SPI_BUS:
    {
        int bus_mode = va_arg(arg,int);
        if(bus_mode <= SPI_BUS && sup_ptr->bus != bus_mode)
        {
            if(bus_mode == SPI_BUS && sup_ptr->mode == SPI_SLAVE)
            {
                //bus mode cannot be used on slave
                break;
            }
            else
            {
                sup_ptr->bus = (enum SPI_BUS_MODE)bus_mode;
                spi_node_fifo_init(dev_id);
                bsp_spi_isr_switch_tx(phy_ptr,0);   //turn off tx interrupt
            }
            res = 1;
        }
    }
        break;
    case IO_SPI_POLARITY:
    {
        int polarity = va_arg(arg,int);
        if(polarity <= SPI_RIFO)
        {
            sup_ptr->polarity = (enum SPI_POLARITY)polarity;
            if(polarity == SPI_ROFI)  bsp_spi_set_polarity(phy_ptr,BSP_SPI_ROFI);
            else bsp_spi_set_polarity(phy_ptr,BSP_SPI_RIFO);
            res = 1;
        }
    }
        break;

    case IO_SPI_WIDTH:
    {
        int width = va_arg(arg,int);
        if(width <= SPI_16_BIT)
        {
            sup_ptr->width = (enum SPI_WIDTH)width;
            if(width == SPI_1_BIT)       bsp_spi_set_width(phy_ptr,BSP_SPI_1);
            else if(width == SPI_2_BIT)  bsp_spi_set_width(phy_ptr,BSP_SPI_2);
            else if(width == SPI_8_BIT)  bsp_spi_set_width(phy_ptr,BSP_SPI_8);
            else if(width == SPI_16_BIT) bsp_spi_set_width(phy_ptr,BSP_SPI_16);
            res = 1;
        }
    }
        break;

    case IO_SPI_BIND:
    {
        int node_id = va_arg(arg,int);
        int gpio_id = va_arg(arg,int);
        if(sup_ptr->bus != SPI_BUS)
        {
            break;
        }
        if(node_id < SPI_NODE_NUM)
        {
            sup_ptr->node[node_id] = 1;
            sup_ptr->pin_en[node_id] = gpio_id;
            res = 1;
        }
    }
        break;

    case IO_SPI_BIND_USR:
    {
        void (*func)(enum SPI_NODE node,char);
        func = va_arg(arg,cb_node_t);
        sup_ptr->user_func = func;
        res = 1;
    }
        break;

    default:
        break;
    }
    return res;
}

int spi_ioctl(int dev_id,int opr,...)
{
    int res;
    va_list arg;
    va_start(arg,opr);
    res = spi_vioctl(dev_id,opr,arg);
    va_end(arg);
    return res;
}


void  dev_spi_init(void)
{
    int res;
    int i;
   /*
    * @ spi device Prototype Initialize
    */
#define DEV_INFO_SPI(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
         dev_spi_array[DEV_ID] = &DEV_NAME;\
         memset(&DEV_NAME,0,sizeof(dev_sup_io));\
         strcpy(DEV_NAME.dev, DESC);\
         strcpy(DEV_NAME.desc,"Device Type: spi");\
         DEV_NAME.buf_in = DEV_NAME ## _in;\
         DEV_NAME.buf_out = DEV_NAME ## _out;\
         DEV_NAME.ptr_in = DEV_NAME ## nodein;\
         DEV_NAME.ptr_out = DEV_NAME ## nodeout;\
         for(i=0;i<SPI_NODE_NUM;i++){\
             buf_ctrl_init(&(DEV_NAME.ptr_in[i]),DEV_EN*SPI_IN_BUF_SIZE);\
             buf_ctrl_init(&(DEV_NAME.ptr_out[i]),DEV_EN*SPI_OUT_BUF_SIZE);\
         }\
         if(DEV_EN && (ISR_ROUTE == CORE_ID)){\
             res = bsp_spi_init(PHY_PTR,BAUD,RCV_SHRED,ISR_ROUTE);\
             if(res == 1) {\
                DEV_NAME.ctrl.bit.ready = 1;\
                dev_spi_array[DEV_ID]->supctrl = &(dev_ctrl_ ##  DEV_NAME);\
                spi_reset(DEV_ID);\
             }\
         }
 #include "bsp/dev_info.h"
 #undef DEV_INFO_SPI
}

/*##################################################
 *   @hardware interrupt
 #################################################*/
#if SPI_A_EN
__interrupt void spia_rx_isr_handler(void)
{
    bsp_spi_clrisr_rx(spi_phy_ptr[SPI_A]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
    spi_handler_readbyte(SPI_A);
    spi_handler_sendbyte(SPI_A);
}
__interrupt void spia_tx_isr_handler(void)   //non fifo mode, not used
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
    if(-1 == spi_handler_sendbyte(SPI_A))
    {
        bsp_spi_isr_switch_tx(spi_phy_ptr[SPI_A],0);   //no more data to send, turn off tx isr
    }
    bsp_spi_clrisr_tx(spi_phy_ptr[SPI_A]);
}
#endif

#if SPI_B_EN
__interrupt void spib_rx_isr_handler(void)
{
    bsp_spi_clrisr_rx(spi_phy_ptr[SPI_B]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
    spi_handler_readbyte(SPI_B);
}
__interrupt void spia_tx_isr_handler(void)   //non fifo mode, not used
{
    bsp_spi_clrisr_tx(spi_phy_ptr[SPI_B]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
    if(-1 == spi_handler_sendbyte(SPI_B))
    {
        bsp_spi_isr_switch_tx(spi_phy_ptr[SPI_B],0);   //no more data to send, turn off tx isr
    }
}
#endif

#if SPI_C_EN
__interrupt void spic_rx_isr_handler(void)
{
    bsp_spi_clrisr_rx(spi_phy_ptr[SPI_C]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
    spi_handler_readbyte(SPI_C);
}
__interrupt void spic_tx_isr_handler(void)  //non fifo mode, not used
{
    bsp_spi_clrisr_tx(spi_phy_ptr[SPI_C]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
    if(-1 == spi_handler_sendbyte(SPI_C))
    {
        bsp_spi_isr_switch_tx(spi_phy_ptr[SPI_C],0);   //no more data to send, turn off tx isr
    }
}
#endif
