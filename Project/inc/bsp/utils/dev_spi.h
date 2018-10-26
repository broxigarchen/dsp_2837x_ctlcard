/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_spi.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef DEV_SPI_H_
#define DEV_SPI_H_

#include "bsp/utils/dev_inc.h"
#include "bsp/bsp_spi.h"

#ifdef DEV_SPI_C_
#define DEV_SPI_EXEC
#else
#define DEV_SPI_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/
enum SPI_MODE
{
   SPI_MASTER,
   SPI_SLAVE,
};/* SPI mODE */

enum SPI_BUS_MODE
{
   SPI_PRIVATE,
   SPI_BUS,
};/* SPI bUS */
enum SPI_NODE
{
    SPI_NODE_0,SPI_NODE_1,SPI_NODE_2,SPI_NODE_3,
    SPI_NODE_NUM,
};/* SPI Node enumeration, maximum 4 nodes */
enum SPI_POLARITY
{
    SPI_ROFI,    //rise out fall in
    SPI_RIFO,    //rise in fall out
};/* SPI Polarity */
enum SPI_WIDTH
{
    SPI_1_BIT = 0x1,
    SPI_2_BIT = 0x2,
    SPI_8_BIT = 0x8,
    SPI_16_BIT = 0x10,
};/* SPI Width */

/******************************************************************
 * @ public
 *******************************************************************/
typedef struct
{
    enum SPI_NODE id;
    size_t len;
}dev_spi_nodedat_t;

#define DEV_SPI_NODEFIFO_LEN   15
typedef struct
{
    dev_spi_nodedat_t               buf[DEV_SPI_NODEFIFO_LEN];   //node data FIFO
    iobuf_ctrl                      ctl;
    volatile   dev_spi_nodedat_t    curr;             //current processing
}dev_spi_nodefifo_t;

typedef struct
{
    enum SPI_NODE id;
}dev_dma_ctrl_t;

typedef void(*cb_node_t)(enum SPI_NODE,char);
typedef struct
{
    enum SPI_MODE           mode;          //master/slave
    enum SPI_BUS_MODE       bus;           //share/private
    enum SPI_POLARITY       polarity;      //ROFI/RIFO
    enum SPI_WIDTH          width;         //data width
    uint16_t                node[SPI_NODE_NUM];       //node register
    uint16_t                pin_en[SPI_NODE_NUM];     //node enable pins
    cb_node_t               user_func;     //node enable function
    dev_spi_nodefifo_t      nodefifosnd;   //node fifo ctl, send
    dev_dma_ctrl_t          dma_ctrl;      //dma info
}dev_spi_ctrl_t;

/******************************************************************
 * @ interface
 *******************************************************************/
DEV_SPI_EXEC void    dev_spi_init(void);
DEV_SPI_EXEC size_t  spi_write(int dev_id, int node_id, char* buf_in,  size_t len);
DEV_SPI_EXEC size_t  spi_read(int dev_id , int node_id, char* buf_out, size_t max_len);
DEV_SPI_EXEC int     spi_listen(int dev_id, int node_id);
DEV_SPI_EXEC void    spi_flush(int dev_id);
DEV_SPI_EXEC int     spi_ioctl(int dev_id,int opr,...);
DEV_SPI_EXEC int     spi_vioctl(int dev_id,int opr,va_list arg);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef DEV_SPI_C_

void  spi_dma_handler_send(int ch_id);
void  spi_dma_handler_receive(int ch_id);

#endif
#endif
