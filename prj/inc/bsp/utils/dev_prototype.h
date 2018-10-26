/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_prototype.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
 * This file define the device prototype and hardware driver interface
##################################################################*/
#ifndef DEV_PROTOTYPE_H_
#define DEV_PROTOTYPE_H_

#include "bsp/cpu_info.h"
#include "lib/cmacroruntime.h"
#include "lib/cstruct.h"

/*********************************************************************/
/*********************************************************************/
/************              DEVICE PROTOTYPE           *****************/
  /*
   *@Device
   * 1.IO
   * 2.Timer
   * 3.EPWM
   * 4.DMA
   */
/*********************************************************************/
/*********************************************************************/

/*####################################################
 *@ device prototype constant
 *@
######################################################*/
#define  DEV_NAME_LEN    10
#define  DEV_DESC_LEN    50


/*********************************************************************/
/*********************************************************************/
/************                   IO                   *****************/
  /*
   *@Brief
   * 1.Buffered IO
   * 2.Directed IO
   * 3.Superior IO
   */
/*********************************************************************/
/*********************************************************************/
enum DEV_TYPE
{
    DEV_BUF_IO,
    DEV_DIR_IO,
    DEV_SUP_IO,
    DEV_TIMER,
    DEV_TYPE_MAX,
};   /*   device type enum  */
enum DEV_WID
{
    WID_BYTE,         //byte
    WID_DBYTE,        //double byte
    WID_WORD,         //word
    WID_DWORD,        //double word
    WID_BIT,          //bit
    WID_MAX,
};  /*   Bit Width enum */

/*####################################################
 *@ IO typedef
######################################################*/
typedef size_t (*stream_in_t)(void* dat,  size_t length);          //prototype of method: write
typedef size_t (*stream_out_t)(void* dat, size_t max_len);         //prototype of method: read
typedef void   (*err_handler_t)(void);                             //prototype of method: default_err


typedef union _io_cfg
{
    uint16_t dbyte;
    struct
    {
        uint16_t dma:1;                       //DMA do/not support
        uint16_t reserved:15;
    }bit;
}io_cfg_t;   /* io cfg */
/*                  io cfg Macro                 */
#define DEV_IO_WID(x)           GetDataMoveL(x,0)
#define DEV_IO_DMA(x)           GetDataMoveL(x,4)

typedef struct _io_dma
{
    uint8_t  channel;       //dma channel
    addr_t  src;            //src physical address
    addr_t  dst;            //dst physical address
    size_t  len;            //send physical size
    size_t  curr_pos;       //current position
    size_t  curr_size;      //current size
    uint8_t status;         //0: idle, 1, pending, 2: processing
}io_dma_t;   /* io dma */

/*####################################################
 *@ BUffered IO
 *@
######################################################*/
typedef struct _dev_io
{
    /*       member         */
    char dev[DEV_NAME_LEN];
    char desc[DEV_DESC_LEN];
    char*      buf_in;                         //write buffer
    char*      buf_out;                        //read buffer
    iobuf_ctrl in;                             //write buffer control
    iobuf_ctrl out;                            //read buffer control
    io_cfg_t   cfg;                            //io cfg
    io_dma_t   dma;                            //dma info
    union
    {
        uint16_t dbyte;
        struct
        {
            uint16_t ready:1;                  //device ready
            uint16_t reserved:15;
        }bit;
    }ctrl;
    err_handler_t     default_err;

}dev_io;

/*####################################################
 *@ Direct IO
 *@
######################################################*/
typedef struct _dev_dir_io
{
    /*       member         */
    char dev[DEV_NAME_LEN];
    char desc[DEV_DESC_LEN];
    err_handler_t     default_err;
    io_cfg_t          cfg;                     //io cfg
}dev_dir_io;

/*####################################################
 *@ Superior IO
 *@
######################################################*/
enum IO_TYPE
{
    IO_BUF,
    IO_DIR,
};
typedef struct _dev_sup_io
{
    /*       member         */
    char dev[DEV_NAME_LEN];
    char desc[DEV_DESC_LEN];
    char*         buf_in;                      //write buffer
    char*         buf_out;                     //read buffer
    iobuf_ctrl*   ptr_in;                      //write buffer control pointer
    iobuf_ctrl*   ptr_out;                     //read buffer control pointer
    io_cfg_t      cfg;                         //io configure
    io_dma_t      dma_snd;                     //dma send channel
    io_dma_t      dma_rcv;                     //dma rcv channel
    union
    {
        uint16_t dbyte;
        struct
        {
            uint16_t ready:1;                  //device ready
            uint16_t reserved:15;
        }bit;
    }ctrl;
    void*             supctrl;                 //superior io ctrl
    err_handler_t     default_err;
}dev_sup_io;

/*####################################################
 *@ IO operation enumeration
 *@
######################################################*/
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

/*********************************************************************/
/*********************************************************************/
/************                 TIMER                  *****************/
  /*
   *@Brief
   *  Timer device Prototype
   */
/*********************************************************************/
/*********************************************************************/
enum TIM_CFG
{
    TIM_CFG_HPRD,     //set hardware period
    TIM_CFG_RTC,      //enable real time clock, format hh:mm:ss
};
enum TIM_MODE
{
    TIM_NROM,         //timer mode
    TIM_CTR,          //counter mode
};
typedef struct _dev_timer
{
    /*       member         */
    char dev[DEV_NAME_LEN];
    char desc[DEV_DESC_LEN];
    void (*callback)(void);
    err_handler_t  default_err;
    struct
    {
        uint32_t  step;               //hardware: step in each interrupt, us, used in timer mode
        uint32_t  freq;               //hardware: interrupt frequency, 1/step
        float     unit;               //hardware: corresponding unit in each hardware counter step, us, used in counter mode
        float     hrunit;             //hardware: corresponding unit in each hardware prescaled-counter step, us, used in counter mode
        uint64_t  threshold;          //triggered value of counter
        uint64_t  counter;            //counter of the soft timer
        uint64_t  watch;              //current value of the soft timer
    }tim;
    struct
    {
        uint64_t rtc_counter;         //rtc clock counter
        uint16_t sec;                 //rtc clock sec
        uint16_t min;                 //rtc clock min
        uint16_t hour;                //rtc clock hour
    }rtc;
    union
    {
        uint16_t dbyte;
        struct
        {
            uint16_t init:1;          //1: properly init, 0: init fail
            uint16_t occupied:1;      //1: occupied, 0: unused
            uint16_t status:1;        //1: running, 0: stopped
            uint16_t rtc_en:1;        //1: enable, 0: disable
            uint16_t mode:2;          //mode, see enum TIM_MODE
            uint16_t reserved:7;
        }bit;
    }ctrl;
}dev_timer;

/*********************************************************************/
/*********************************************************************/
/************                 EPWM                  *****************/
  /*
   *@Brief
   *  epwm device Prototype
   */
/*********************************************************************/
/*********************************************************************/
enum PWM_CHAN
{
    PWMA,      //outputA
    PWMB,      //outputB
};   /*   channel, each device has two output channel   */
enum PWM_OUT
{
    A_NOTA,   //Output A:PWMA, Output B:NOT PWMA
    NOTA_A,   //Output A:NOT PWMA, Output B:PWMA
    A_B,      //Output A:PWMA, Output B:PWMB
    B_A,      //Output A:PWMB, Output B:PWMA
};  /*   Output connection, each device has two output   */
enum PWM_OPR
{
    NO_ACT,    //do nothing
    SET_L,     //output: set low
    SET_H,     //output: set high
    TOGGLE,    //output: toggle
}; /*   output operation   */
enum PWM_CAR
{
    SAW_RISE,SAW_FALL,TRIANGLE,
};
enum PWM_CAR_DIR
{
   UP_COUNT,DOWN_COUNT,
};
enum PWM_REF
{
    SIN,CONST,USRDEF,
};
enum PWM_CMP
{
    CMP_A,
    CMP_B,
    CMP_C,
    CMP_D,
}; /*   comparator ID, 4 CMP   */
enum PWM_CDT
{
    BOTTOM,      //carrier reach bottom
    PEAK,        //carrier reach peak
    BTM_PK,      //carrier reach bottom or peak
    CROSS_A_U,   //carrier across comparator A when count up
    CROSS_A_D,   //carrier across comparator A when count down
    CROSS_B_U,   //carrier across comparator B when count up
    CROSS_B_D,   //carrier across comparator B when count down
    CROSS_C_U,   //carrier across comparator C when count up
    CROSS_C_D,   //carrier across comparator C when count down
    CROSS_D_U,   //carrier across comparator D when count up
    CROSS_D_D,   //carrier across comparator D when count down
}; /*   event type   */
enum PWM_EVENT_AD
{
    SOCA,      //internal AD SOCA
    SOCB,      //internal AD SOCB
}; /*   event AD enumeration   */
enum PWM_SYN_CHN
{
    PWM1CHN,    //the sync input of pwm1 to 3 are connected together in a chain
    PWM4CHN,    //the sync input of pwm4 to 6 are connected together in a chain
    PWM7CHN,    //the sync input of pwm7 to 9 are connected together in a chain
    PWM10CHN,   //the sync input of pwm10 to 12 are connected together in a chain
}; /*   PWM SYNC Group  */
enum PWM_SYN_SRC
{
    PWM1SYNOUT,  //the output sync of group PWM1
    PWM4SYNOUT,  //the output sync of group PWM4
    PWM7SYNOUT,  //the output sync of group PWM7
    PWM10SYNOUT, //the output sync of group PWM10
    EXTSYNIN1,   //the external sync input 1
    EXTSYNIN2,   //the external sync input 2
}; /*   PWM SYNC source   */
typedef void (*epwm_carr_cb)(void);  //carrier callback
typedef struct _dev_epwm
{
    /*       member         */
    char dev[DEV_NAME_LEN];
    char desc[DEV_DESC_LEN];
    err_handler_t  default_err;
    /***********************
     * @carrier
     *********************/
    struct
    {
        struct
        {
            float  freq;                //carrier frequency
            size_t prdsteps;            //carrier period steps
            size_t steps;               //carrier counter steps
            float  period;              //carrier period in us
            float  phase;               //start phase, locate in [0,2*pi)
            enum  PWM_CAR  type;        //waveform type, see PWM_C_TYPE
        }tim;
        struct
        {

        }ctrl;
    }car;
    /***********************
     * @reference
     *********************/
//    struct
//    {
//        struct
//        {
//            uint32_t  freq;
//            float period;
//            float phase;
//            float ma;
//            void (*ref_update)(enum PWM_CAR_DIR dir,float* pos,enum PWM_EVENT* opr);
//        }info;
//        struct
//        {
//            uint32_t counter;
//        }ctrl;
//    }ref;
    /***********************
     * @events && comparator
     *********************/
    struct
    {
        size_t  steps[4];   //cmp a,b,c,d
    }cmp;
    void (*event_cb)(void);          //event callback functions
    void (*event_ad_a_cb)(void);     //event callback functions
    void (*event_ad_b_cb)(void);     //event callback functions
    /***********************
     * @dead band
     *********************/
    size_t deadtime;   //us
    /***********************
     * @output connection
     *********************/
    enum PWM_OUT out_mode;
    /***********************
    * @ configure && state machine
    *********************/
    struct
    {
        uint16_t dbyte;
        struct
        {
            uint16_t ready:1;          //1: properly init, 0: init fail
            uint16_t status:1;        //1: running, 0: stopped
            uint16_t reserved:14;
        }bit;
    }ctrl;
}dev_epwm;

/*********************************************************************/
/*********************************************************************/
/************                  DMA                   *****************/
  /*
   *@Brief
   *  DMA device Prototype
   */
/*********************************************************************/
/*********************************************************************/
enum DMACH_ST
{
    DMACH_IDLE,   //idle
    DMACH_RUN,    //running
    DMACH_BIND,   //binded to devices
};
typedef struct
{
    char dev[DEV_NAME_LEN];
    char desc[DEV_DESC_LEN];
    enum DMACH_ST  status;
    void (*callback)(int);
}dev_dma_ch;






#endif
