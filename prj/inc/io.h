/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    io.h
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * io interface
##################################################################*/
#ifndef IO_H_
#define IO_H_
#include "bsp/cpu_info.h"
/*####################################################
 *@ IO device id enumeration
######################################################*/
enum
{
    /*     Class 0:  direct IO      */
    //GPIO,  both I/O
    GPIO_0,GPIO_1,GPIO_2,GPIO_3,GPIO_4,GPIO_5,GPIO_6,GPIO_7,GPIO_8,GPIO_9,
    GPIO_10,GPIO_11,GPIO_12,GPIO_13,GPIO_14,GPIO_15,GPIO_16,GPIO_17,GPIO_18,GPIO_19,
    GPIO_20,GPIO_21,GPIO_22,GPIO_23,GPIO_24,GPIO_25,GPIO_26,GPIO_27,GPIO_28,GPIO_29,
    GPIO_30,GPIO_31,GPIO_32,GPIO_33,GPIO_34,GPIO_35,GPIO_36,GPIO_37,GPIO_38,GPIO_39,
    GPIO_40,GPIO_41,GPIO_42,GPIO_43,GPIO_44,GPIO_45,GPIO_46,GPIO_47,GPIO_48,GPIO_49,
    GPIO_50,GPIO_51,GPIO_52,GPIO_53,GPIO_54,GPIO_55,GPIO_56,GPIO_57,GPIO_58,GPIO_59,
    GPIO_60,GPIO_61,GPIO_62,GPIO_63,GPIO_64,GPIO_65,GPIO_66,GPIO_67,GPIO_68,GPIO_69,
    GPIO_70,GPIO_71,GPIO_72,GPIO_73,GPIO_74,GPIO_75,GPIO_76,GPIO_77,GPIO_78,GPIO_79,
    GPIO_80,GPIO_81,GPIO_82,GPIO_83,GPIO_84,GPIO_85,GPIO_86,GPIO_87,GPIO_88,GPIO_89,
    GPIO_90,GPIO_91,GPIO_92,GPIO_93,GPIO_94,GPIO_95,GPIO_96,GPIO_97,GPIO_98,GPIO_99,
    GPIO_100,GPIO_101,GPIO_102,GPIO_103,GPIO_104,GPIO_105,GPIO_106,GPIO_107,GPIO_108,GPIO_109,
    GPIO_110,GPIO_111,GPIO_112,GPIO_113,GPIO_114,GPIO_115,GPIO_116,GPIO_117,GPIO_118,GPIO_119,
    GPIO_120,GPIO_121,GPIO_122,GPIO_123,GPIO_124,GPIO_125,GPIO_126,GPIO_127,GPIO_128,GPIO_129,
    GPIO_130,GPIO_131,GPIO_132,GPIO_133,GPIO_134,GPIO_135,GPIO_136,GPIO_137,GPIO_138,GPIO_139,
    GPIO_140,GPIO_141,GPIO_142,GPIO_143,GPIO_144,GPIO_145,GPIO_146,GPIO_147,GPIO_148,GPIO_149,
    GPIO_150,GPIO_151,GPIO_152,GPIO_153,GPIO_154,GPIO_155,GPIO_156,GPIO_157,GPIO_158,GPIO_159,
    GPIO_160,GPIO_161,GPIO_162,GPIO_163,GPIO_164,GPIO_165,GPIO_166,GPIO_167,GPIO_168,

    /*     Class 1: buffered IO    */
    //UART/SCI, both I/O
    UART_A,UART_B,UART_C,UART_D,
    //USB, both I/O
    USB,

    /*     Class 2: superior IO     */
    //AD,  Only I
    AD_1_CHAN1,AD_1_CHAN2,AD_1_CHAN3,AD_1_CHAN4,AD_1_CHAN5,AD_1_CHAN6,AD_1_CHAN7,AD_1_CHAN8,AD_1_CHAN9,AD_1_CHAN10,AD_1_CHAN11,AD_1_CHAN12,
    AD_2_CHAN1,AD_2_CHAN2,AD_2_CHAN3,AD_2_CHAN4,AD_2_CHAN5,AD_2_CHAN6,AD_2_CHAN7,AD_2_CHAN8,AD_2_CHAN9,AD_2_CHAN10,AD_2_CHAN11,AD_2_CHAN12,
    AD_3_CHAN1,AD_3_CHAN2,AD_3_CHAN3,AD_3_CHAN4,AD_3_CHAN5,AD_3_CHAN6,AD_3_CHAN7,AD_3_CHAN8,AD_3_CHAN9,AD_3_CHAN10,AD_3_CHAN11,AD_3_CHAN12,
    AD_4_CHAN1,AD_4_CHAN2,AD_4_CHAN3,AD_4_CHAN4,AD_4_CHAN5,AD_4_CHAN6,AD_4_CHAN7,AD_4_CHAN8,AD_4_CHAN9,AD_4_CHAN10,AD_4_CHAN11,AD_4_CHAN12,
    //SPI,  both I/O
    SPI_A,SPI_B,SPI_C,

    IO_MAX,
};

enum DEV_TYPE
{
    DEV_BUF_IO,
    DEV_DIR_IO,
    DEV_SUP_IO,
    DEV_TIMER,
    DEV_TYPE_MAX,
};   /*   device type enum  */
enum IO_CLASS io_get_class(int id);          //get io class
enum DEV_TYPE dev_get_type(int id);          //get device type

/*#################################################
 *  @Standard Write/Read IO interface
 *  @brief.
 *  The available functions:
 *  1. write    //write data to IO
 *  2. read     //read data from IO
 *  3. listen   //listen from IO if there are data to receive
 *  4. flush    //clear all buffers
 *  5. print    //print string to IO
 *  6. getbyte  //read one char from IO
 *###################################################*/

/*****************************************************
 *  @write
 *  @brief
 *  I/O write function
 *
 *  @in parameter
 *   ###depends on the type of the device!!!
 *   1. buffered IO
 *   write(int dev_id, char* buf_in, size_t data_len)
 *   int     dev_id,      device id
 *   char*   buf_in,      transfer buffer
 *   size_t  data_len,    data length
 *
 *   2. direct IO
 *   write(int dev_id, char value)
 *   int      dev_id,      device_id
 *   char     value,       set value
 *
 *   3. superior IO
 *   depends on device, see superior IO part
 *  @return
 *   data_len,             written data length, 0 when fails
 *****************************************************/
extern size_t   write(int dev_id, ...);

/*****************************************************
 *  @read
 *  @brief
 *  I/O read function
 *
 *  @in parameter
 *   ###depends on the type of the device!!!
 *   1. buffered IO
 *   read(int dev_id, char* buf_in, size_t data_len)
 *   int     dev_id,      device id
 *   char*   buf_in,      transfer buffer
 *   size_t  data_len,    data length
 *
 *   2. direct IO
 *   read(int dev_id, char* value)
 *   int      dev_id,      device_id
 *   char*     value,      pointer to read value
 *
 *   3. superior IO
 *   depends on device, see superior IO part
 *  @return
 *   data_len,             read data length, 0 when fails
 *****************************************************/
extern size_t   read(int id, ...);

/*****************************************************
 *  @listen
 *  @brief
 *  I/O listen
 *
 *  @in parameter
 *  ###depends on the type of the device!!!
 *  1. generally
 *   listen(int dev_id)
 *  2. SPI
 *   listen(int dev_id, int node_id)
 *
 *  @return
 *    1,            there are data to receive
 *   -1,            idle
 *****************************************************/
extern int      listen(int id, ...);

/*****************************************************
 *  @ioflush
 *  @brief
 *  I/O flush, clear write/read buffers, only applied to buffered IO
 *
 *  @in parameter
 *   int     dev_id,      device id
 *
 *  @return
 *    none
 *****************************************************/
extern void     ioflush(int id);

/*#################################################
 *  @Superior IO interface
 *  @brief.
 *  Write/Read and Control the I/O depends on type
 *#################################################*/
enum IO_CTL_OP
{
    /*       General        */
    IO_RESET,           //reset I/O to state after initialization
    IO_DMA,             //DMA mode,
    /*       SPI Only       */
    IO_SPI_MODE,        //Master/Slave
    IO_SPI_BUS,         //share/private
    IO_SPI_POLARITY,    //RIFO/ROFI
    IO_SPI_WIDTH,       //data width
    IO_SPI_BIND,        //bind GPIO as the EN signal of SPI device in bus mode
    IO_SPI_BIND_USR,    //use user-defined function to select SPI device in bus mode
}; /* operation enumeration */

/*****************************************************
 *  @ioctl
 *  @brief
 *  I/O control and configuration, see all the supported IO operation below
 *
 *  @in parameter
 *   int                dev_id,      device id
 *   enum IO_CTL_OP     operation,   supported operation, see above
 *
 *   the detail of how to use will be listed below.
 *  @return
 *    1,    success
 *   -1,    fail
 *****************************************************/
extern  int    ioctl(int id,enum IO_CTL_OP operation,...);

/*-------------------------------
 * @SPI interface
 * @Write/Read
 * USAGE: ### write(int dev_id, enum SPI_NODE node, char* buf_in, size_t len) ###
 * USAGE: ### read(int dev_id, enum SPI_NODE node, char* buf_in, size_t len) ###
 *
 * @Supported Operation
 * General Operation
 * 1. IO_RESET
 * 2. IO_DMA
 *    USAGE: ### ioctl(int id, enum IO_CTL_OP operation,enum SPI_NODE node,addr_t send, size_t len, addr_t rcv, size_t len); ###
 *    eg: ioctl(SPI_A, IO_DMA, node_id, sendbuf, 128, rcvbuf, 128);
 *
 * SPI Only
 * 1. IO_SPI_MODE
 *    USAGE: ### ioctl(int id, enum IO_CTL_OP operation, enum SPI_MODE mode); ###
 * 2. IO_SPI_BUS
 *    USAGE: ### ioctl(int id, enum IO_CTL_OP operation, enum SPI_BUS bus); ###
 *    SPI has two mode, bus/private mode.
 *    ＃Bus mode:
 *    master communicate with multiple slaves in the shared SPI channel.The pin, EN signal of the SPI module, needs to be selected as
 *    the normal GPIO instead of SPI EN signal. The node need to be registered using SPI_BIND with the EN GPIO id or with a function
 *    to select the device before sending or receiving data
 *    ＃Private mode:
 *    only one slave, do not need binding, select any node do not affect
 * 3. IO_SPI_POLARITY
 *    USAGE: ### ioctl(int id, enum IO_CTL_OP operation, enum SPI_POLARITY polarity); ###
 * 4. IO_SPI_BIND
 *    only in bus mode
 *    USAGE: ### ioctl(int id, enum IO_CTL_OP operation, enum SPI_NODE node_id, int gpio_id); ###
 *    example: ioctl(SPI_A, IO_SPI_BIND, SPI_NODE_0, GPIO_1);
 * 5. IO_SPI_BIND_USR
 *    only in bus mode
 *    USAGE: ### ioctl(int id, enum IO_CTL_OP operation, void (*user_func)(enum SPI_NODE node)); ###
 -------------------------------*/
enum SPI_MODE
{
   SPI_MASTER,
   SPI_SLAVE,
};/* SPI mODE */

enum SPI_BUS
{
   SPI_PRIVATE,
   SPI_SHARE,
};/* SPI BUs */

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

enum SPI_NODE
{
    SPI_NODE_0,SPI_NODE_1,SPI_NODE_2,SPI_NODE_3,
};/* SPI Node enumeration, maximum 4 nodes */

#endif
