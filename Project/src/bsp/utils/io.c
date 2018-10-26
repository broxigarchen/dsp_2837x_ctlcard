/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    io.c
 * @AUTHOR  Brox Chen
 * @DATE    July 1, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * io
##################################################################*/
#define IO_C_
#include "bsp/utils/dev_inc.h"
#include "bsp/bsp_gpio.h"
#include "bsp/utils/dev_uart.h"
#include "bsp/utils/dev_spi.h"
#include "lib/cstruct.h"

/* ##################################################
 *   @Device Enum
 #################################################*/
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

/* ##################################################
 *   @Macro
 #################################################*/
#define IO_CHK(ID)\
    if(ID >= IO_MAX) return 0;
#define IO_CHK_VOID(ID)\
    if(ID >= IO_MAX) return;
#define IO_PTR_CHK(ptr)\
     if(ptr == NULL) return 0;
#define IO_IN_RANGE(id,min,max)   ((id>=min && id<=max)?1:0)


enum IO_CLASS
{
    IO_CLASS_NODEF,
    IO_CLASS_GPIO,
    IO_CLASS_UART,
    IO_CLASS_AD,
    IO_CLASS_SPI,
    IO_CLASS_USB,
};

/* ##################################################
 *   @public interface
 #################################################*/
enum IO_CLASS io_get_class(int id)
{
    if(IO_IN_RANGE(id,GPIO_0,GPIO_168))
    {
        return IO_CLASS_GPIO;
    }
    else if(IO_IN_RANGE(id,UART_A,UART_D))
    {
        return IO_CLASS_UART;
    }
    else if(IO_IN_RANGE(id,AD_1_CHAN1,AD_4_CHAN12))
    {
        return IO_CLASS_AD;
    }
    else if(IO_IN_RANGE(id,SPI_A,SPI_C))
    {
        return IO_CLASS_SPI;
    }
    return IO_CLASS_NODEF;
}

enum DEV_TYPE dev_get_type(int id)
{
    if(IO_IN_RANGE(id,GPIO_0,GPIO_168))
    {
        return DEV_DIR_IO;
    }
    else if(IO_IN_RANGE(id,UART_A,USB))
    {
        return DEV_BUF_IO;
    }
    else
    {
        return DEV_SUP_IO;
    }
}

size_t write(int id, ...)
{
    size_t res;
    char* buf_in;
    size_t len;
    char dir_num;

    IO_CHK(id);
    enum IO_CLASS io_class = io_get_class(id);
    va_list arg;
    va_start(arg,id);

    switch(io_class)
    {
    case IO_CLASS_GPIO:
    {
        dir_num = va_arg(arg,char);
        bsp_gpio_set(id - GPIO_0,  (uint8_t)dir_num);
        res = 1;
    }
        break;

    case IO_CLASS_UART:
    {
        buf_in = va_arg(arg,char*);
        len = va_arg(arg,size_t);
        IO_PTR_CHK(buf_in);
        res = uart_write(id - UART_A,  buf_in,  len);
    }
        break;

    case IO_CLASS_AD:
        res = 0; //do not support write
        break;

    case IO_CLASS_SPI:
    {
        int node_id;
        node_id = va_arg(arg,int);
        buf_in = va_arg(arg,char*);
        len = va_arg(arg,size_t);
        IO_PTR_CHK(buf_in);
        res = spi_write(id - SPI_A, node_id, buf_in,  len);
    }
        break;

    case IO_CLASS_USB:
        break;

    default:
        break;
    }
    va_end(arg);
    return res;
}

int listen(int id, ...)
{
    int res;
    IO_CHK(id);
    va_list arg;
    va_start(arg,id);

    enum DEV_TYPE dev_type = dev_get_type(id);
    if(dev_type != DEV_BUF_IO)
    {
        return 0;
    }
    enum IO_CLASS io_class = io_get_class(id);
    switch(io_class)
    {

       case IO_CLASS_UART:
           res = uart_listen(id - UART_A);
           break;

       case IO_CLASS_AD:
           break;

       case IO_CLASS_SPI:
       {
           int node_id = va_arg(arg,int);
           res = spi_listen(id - SPI_A, node_id);
       }
           break;

       case IO_CLASS_USB:
           break;

       default:
           res = 0;
           break;
    }
    va_end(arg);
    return res;
}

size_t read(int id, ...)
{
    char* buf_out;
    size_t res;
    size_t max_len;
    IO_CHK(id);
    enum IO_CLASS io_class = io_get_class(id);
    va_list arg;
    va_start(arg,id);

    switch(io_class)
    {
       case IO_CLASS_GPIO:
       {
           char* dir_read;
           dir_read = va_arg(arg,char*);
           *dir_read = (char)bsp_gpio_read(id - GPIO_0);
           res = 1;
       }
           break;

       case IO_CLASS_UART:
       {
           buf_out = va_arg(arg,char*);
           max_len = va_arg(arg,size_t);
           IO_PTR_CHK(buf_out);
           res = uart_read(id - UART_A ,buf_out, max_len);
       }
           break;

       case IO_CLASS_AD:
           break;

       case IO_CLASS_SPI:
       {
           int node_id;
           node_id = va_arg(arg,int);
           buf_out = va_arg(arg,char*);
           max_len = va_arg(arg,size_t);
           IO_PTR_CHK(buf_out);
           res = spi_read(id - SPI_A, node_id, buf_out,  max_len);
       }
           break;

       case IO_CLASS_USB:
           break;

       default:
           break;
    }
    va_end(arg);
    return res;
}

void ioflush(int id)
{
    IO_CHK_VOID(id);
    enum DEV_TYPE dev_type = dev_get_type(id);
    if(dev_type ==  DEV_DIR_IO)
    {
        return;
    }
    enum IO_CLASS io_class = io_get_class(id);
    switch(io_class)
    {
    case IO_CLASS_UART:
        uart_flush(id);
        break;
    case IO_CLASS_USB:
        break;
    case IO_CLASS_SPI:
        spi_flush(id);
        break;
    default:
        break;
    }
}

int ioctl(int id, enum IO_CTL_OP opr, ...)
{
    IO_CHK(id);
    va_list arg;
    enum DEV_TYPE dev_type = dev_get_type(id);
    enum IO_CLASS dev_class = io_get_class(id);
    if(dev_type == DEV_DIR_IO)
    {
        return -1;
    }

    va_start(arg,opr);

    switch(dev_class)
    {
    case IO_CLASS_UART:

        break;
    case IO_CLASS_SPI:
        spi_vioctl(id - SPI_A,opr,arg);
        break;
    default:
        break;
    }
    va_end(arg);
    return 1;
}


