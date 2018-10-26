/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_uart.c
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * device manage: uart
##################################################################*/
#define DEV_UART_C_
#include "bsp/utils/dev_uart.h"
#include "bsp/bsp_uart.h"
#include "bsp/bsp_gpio.h"

/* ##################################################
 *   @device declare
 #################################################*/
/*
 * @ UART device Id enumeration
 */
#define DEV_INFO_UART(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, PARITY, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
    DEV_ID,
enum
{
#include "bsp/dev_info.h"
UART_MAX,
};
#undef DEV_INFO_UART

/*
 * @ UART physic pointer Array
 */

#define DEV_INFO_UART(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, PARITY, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
    (void*)PHY_PTR,
void* uart_phy_ptr[] = {
#include "bsp/dev_info.h"
};
#undef DEV_INFO_UART

/*
 * @ UART device Prototype Declare
 */

#define DEV_INFO_UART(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, PARITY, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
    dev_io DEV_NAME;
#include "bsp/dev_info.h"
#undef DEV_INFO_UART


/*
 * @ Uart device Prototype pointer
 */
dev_io* dev_uart_array[UART_MAX];

/*
 * @ UART device buffer Declare
 */
#define DEV_INFO_UART(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, PARITY, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
    char DEV_NAME ## _in[DEV_EN * UART_IN_BUF_SIZE];\
    char DEV_NAME ## _out[DEV_EN * UART_OUT_BUF_SIZE];
#include "bsp/dev_info.h"
#undef DEV_INFO_UART

/* ##################################################
 *   @device check MACRO
 #################################################*/
#define  DEV_CHK(dev_id)\
    if(dev_id >= UART_MAX || dev_id < 0)    return 0;
#define  DEV_PTR_CHK(ptr)\
    if(ptr == NULL)    return 0;
#define DEV_STATUS_CHK(ptr)\
    if(ptr->ctrl.bit.ready == 0) return 0;
#define DEV_STATUS_CHK_NORET(ptr)\
    if(ptr->ctrl.bit.ready == 0) return;
/* ##################################################
 *   @hardware handler
 *   shared between devices, needs to be reentrant
 #################################################*/
 /*
  * @Sent by Tx interrupt
  */
inline int  uart_handler_sendbyte(int dev_id)
{
    dev_io* ptr =   dev_uart_array[dev_id];
    if(buf_ctrl_wait(&ptr->out))
    {
        while((bsp_uart_writerdy(uart_phy_ptr[dev_id]) == 1) &&  //FIFO has free space && has data to be sent
                (buf_ctrl_wait(&ptr->out) == 1))
        {
            bsp_uart_writebyte(uart_phy_ptr[dev_id],ptr->buf_out[buf_ctrl_out(&ptr->out)]);
        }
        return 1;   //data sending
    }
    return bsp_uart_busy(uart_phy_ptr[dev_id]);   //depends on busy
}
/*
 * @Sent by DMA interrupt
 */

/*
 * @Sent by user
 */
int   uart_trig_send(int dev_id)
{
    dev_io* ptr =   dev_uart_array[dev_id];
    if(buf_ctrl_wait(&ptr->out))          //has data to send, turn on tx isr
    {
        uart_hook_485send_enter(dev_id);
        bsp_uart_isr_switch_tx(uart_phy_ptr[dev_id],1);
    }
    return 1;
}

/*
 * @Received by Rx interrupt
 */
inline void  uart_handler_readbyte(int dev_id)
{
    dev_io* ptr =   dev_uart_array[dev_id];
    char dummy;
    while(bsp_uart_readrdy(uart_phy_ptr[dev_id]) == 1)
    {
        if(!buf_ctrl_full(&ptr->in))
        {
            bsp_uart_readbyte(uart_phy_ptr[dev_id],&ptr->buf_in[buf_ctrl_in(&ptr->in)]);
        }
        else
        {
            bsp_uart_readbyte(uart_phy_ptr[dev_id],&dummy);
        }
    }
}
/* ##################################################
 *   @device internal
 #################################################*/
void   uart_hook_485send_enter(int dev_id)
{
    switch(dev_id)
    {
    case UART_A:
            break;
    case UART_B:
            break;
    case UART_C:
        bsp_gpio_set(RS485_SEL_PIN,1);
            break;
    case UART_D:
            break;
    }
}

void   uart_hook_485send_quit(int dev_id)
{
    switch(dev_id)
    {
    case UART_A:
            break;
    case UART_B:
            break;
    case UART_C:
        bsp_gpio_set(RS485_SEL_PIN,0);
            break;
    case UART_D:
            break;
    }
}
/* ##################################################
 *   @device methods
 #################################################*/
size_t  uart_write(int dev_id, char* buf_in, size_t len)
{
    DEV_CHK(dev_id);
    DEV_PTR_CHK(buf_in);
    dev_io* ptr =   dev_uart_array[dev_id];
    size_t write_len = 0;
    char* in_ptr = buf_in;
    DEV_STATUS_CHK(ptr);
    while(!buf_ctrl_full(&ptr->out) && write_len < len)
    {
        ptr->buf_out[buf_ctrl_in(&ptr->out)] = *in_ptr++;
        write_len ++;
    }
    uart_trig_send(dev_id);
    return write_len;
}

size_t  uart_read(int dev_id ,  char* buf_out, size_t max_len)
{
    DEV_CHK(dev_id);
    DEV_PTR_CHK(buf_out);
    dev_io* ptr =   dev_uart_array[dev_id];
    size_t read_len = 0;
    char* out_ptr = buf_out;
    DEV_STATUS_CHK(ptr);
    while(buf_ctrl_wait(&ptr->in) && read_len < max_len)
    {
       *out_ptr++ =  ptr->buf_in[buf_ctrl_out(&ptr->in)];
       read_len ++;
    }
    return read_len;
}

int  uart_listen(int dev_id)
{
    int res;
    DEV_CHK(dev_id);
    dev_io* ptr =   dev_uart_array[dev_id];
    DEV_STATUS_CHK(ptr);
    res =  buf_ctrl_wait(&ptr->in);
    if(res == 0) return -1;
    return 1;
}

void uart_flush(int dev_id)
{
    if(dev_id >= UART_MAX || dev_id < 0)    return;
    dev_io* ptr =   dev_uart_array[dev_id];
    DEV_STATUS_CHK_NORET(ptr);
    buf_ctrl_reset(&ptr->in);
    buf_ctrl_reset(&ptr->out);
}

void  dev_uart_init(void)
{
    int res;
    /*
     * @ Uart device Prototype Initialize
     */
    #define DEV_INFO_UART(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, BAUD, PARITY, RCV_SHRED, ISR_EN, ISR_ROUTE, DESC)\
            dev_uart_array[DEV_ID] = &DEV_NAME;\
            memset(&DEV_NAME,0,sizeof(dev_io));\
            strcpy(DEV_NAME.dev, DESC);\
            strcpy(DEV_NAME.desc,"Device Type: uart");\
            DEV_NAME.buf_in = DEV_NAME ## _in;\
            DEV_NAME.buf_out = DEV_NAME ## _out;\
            buf_ctrl_init(&DEV_NAME.in,DEV_EN*UART_IN_BUF_SIZE);\
            buf_ctrl_init(&DEV_NAME.out,DEV_EN*UART_OUT_BUF_SIZE);\
            if(DEV_EN && (ISR_ROUTE == CORE_ID)){\
                res = bsp_uart_init(PHY_PTR,BAUD,PARITY,RCV_SHRED,ISR_ROUTE);\
                if(res == 1) {\
                    DEV_NAME.ctrl.bit.ready = 1;\
                }\
            }
    #include "bsp/dev_info.h"
    #undef DEV_INFO_UART

}

/* ##################################################
 *   @hardware interrupt
 #################################################*/
#if UART_A_EN
__interrupt void uarta_rx_isr_handler(void)
{
    bsp_uart_clrisr_rx(uart_phy_ptr[UART_A]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
    uart_handler_readbyte(UART_A);
}
__interrupt void uarta_tx_isr_handler(void)
{
    bsp_uart_clrisr_tx(uart_phy_ptr[UART_A]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
    if(-1 == uart_handler_sendbyte(UART_A))
    {
      bsp_uart_isr_switch_tx(uart_phy_ptr[UART_A],0);   //no more data to send, turn off tx isr
    }
}
#endif

#if UART_B_EN
__interrupt void uartb_rx_isr_handler(void)
{
    bsp_uart_clrisr_rx(uart_phy_ptr[UART_B]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
    uart_handler_readbyte(UART_B);
}
__interrupt void uartb_tx_isr_handler(void)
{
    bsp_uart_clrisr_tx(uart_phy_ptr[UART_B]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
    if(-1 == uart_handler_sendbyte(UART_B))
    {
        bsp_uart_isr_switch_tx(uart_phy_ptr[UART_B],0);   //no more data to send, turn off tx isr
    }
}
#endif

#if UART_C_EN
__interrupt void uartc_rx_isr_handler(void)
{
    bsp_uart_clrisr_rx(uart_phy_ptr[UART_C]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP8;
    uart_handler_readbyte(UART_C);
}
__interrupt void uartc_tx_isr_handler(void)
{
    bsp_uart_clrisr_tx(uart_phy_ptr[UART_C]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP8;
    if(-1 == uart_handler_sendbyte(UART_C))
    {
        bsp_uart_isr_switch_tx(uart_phy_ptr[UART_C],0);   //no more data to send, turn off tx isr
        uart_hook_485send_quit(UART_C);
    }
}
#endif

#if UART_D_EN
__interrupt void uartd_rx_isr_handler(void)
{
    bsp_uart_clrisr_rx(uart_phy_ptr[UART_D]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP8;
    uart_handler_readbyte(UART_D);
}
__interrupt void uartd_tx_isr_handler(void)
{
    bsp_uart_clrisr_tx(uart_phy_ptr[UART_D]);
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP8;
    uart_handler_sendbyte(UART_D);
    if(-1 == uart_handler_sendbyte(UART_D))
    {
        bsp_uart_isr_switch_tx(uart_phy_ptr[UART_D],0);   //no more data to send, turn off tx isr
    }
}
#endif
