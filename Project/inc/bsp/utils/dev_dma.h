/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_spi.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef DEV_DMA_H_
#define DEV_DMA_H_

#include "bsp/utils/dev_inc.h"
#include "bsp/bsp_dma.h"

#ifdef DEV_DMA_C_
#define DEV_DMA_EXEC
#else
#define DEV_DMA_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/
#define   DMA_MAX_LEN  0xffff

/******************************************************************
 * @ public
 *******************************************************************/
/*    DMA operator    */
#define     DMA_16      (0<<0)    //DMA 16 bit
#define     DMA_32      (1<<0)    //DMA_32_bit
#define     DMA_BIND    (1<<1)    //DMA bind to channel, continously/looping sending or receiveing
#define     DMA_ATOM    (1<<2)    //DMA transmition in atom mode, continously transmit all data without sliced in pieces
#define     DMA_W       (0<<3)    //DMA device write, only affect in dma_dev_bind
#define     DMA_R       (1<<3)    //DMA device read, only affect in dma_dev_bind

/******************************************************************
 * @ interface
 *******************************************************************/
DEV_DMA_EXEC int   dma_memcpy(uint32_t* dst, uint32_t* src, size_t len,unsigned int opr, void(*cb)(int));
DEV_DMA_EXEC int   dma_memcpy_h(uint32_t* dst, uint32_t* src, size_t len,unsigned int opr,void(*cb)(int));
DEV_DMA_EXEC int   dma_dev_cpy(enum DMA_SRC dev, uint32_t* dst, uint32_t* src, size_t len,unsigned int opr,void(*cb)(int));
DEV_DMA_EXEC int   dma_dev_cpy_h(enum DMA_SRC dev,uint32_t* dst, uint32_t* src, size_t len,unsigned int opr,void(*cb)(int));
DEV_DMA_EXEC void  dma_dev_free(int ch_id);
DEV_DMA_EXEC int   dev_dma_init(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef DEV_DMA_C_


#endif
#endif
