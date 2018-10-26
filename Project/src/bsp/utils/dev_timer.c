/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_timer.c
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * device manage: timer
##################################################################*/
#define DEV_TIMER_C_
#include "bsp/utils/dev_timer.h"
#include "bsp/bsp_timer.h"

/*
 * @ Timer device Id enumeration
 */

#define DEV_INFO_TIMER(DEV_ID,  DEV_EN, DEV_NAME, PHY_PTR, INTERVAL, ISR_EN, DESC)\
    DEV_ID,
enum
{
#include "bsp/dev_info.h"
TIMER_MAX,
};
#undef DEV_INFO_TIMER

/*
 * @ Timer physic pointer Array
 */

#define DEV_INFO_TIMER(DEV_ID,  DEV_EN, DEV_NAME, PHY_PTR, INTERVAL, ISR_EN, DESC)\
    (void*)PHY_PTR,
void* timer_phy_ptr[] = {
#include "bsp/dev_info.h"
};
#undef DEV_INFO_TIMER

/*
 * @ Timer device Prototype Declare
 */

#define DEV_INFO_TIMER(DEV_ID,  DEV_EN, DEV_NAME, PHY_PTR, INTERVAL, ISR_EN, DESC)\
    dev_timer DEV_NAME;\
    dev_timer* DEV_NAME ## _ptr = &DEV_NAME;
#include "bsp/dev_info.h"
#undef DEV_INFO_TIMER
/*
 * @ Timer device Prototype pointer
 */
dev_timer* dev_timer_array[TIMER_MAX];

/* ##################################################
 *   @device check MACRO
 #################################################*/
#define  DEV_CHK(dev_id)\
    if(dev_id >= TIMER_MAX || dev_id < 0)    return -1;
#define  DEV_CHK_U(dev_id)\
    if(dev_id >= TIMER_MAX || dev_id < 0)    return 0;
#define  DEV_CHK_N(dev_id)\
    if(dev_id >= TIMER_MAX || dev_id < 0)    return;
/* ##################################################
 *   @device internal
 #################################################*/
void feed_rtc(int dev_id)
{
    dev_timer* ptr = dev_timer_array[dev_id];
    ptr->rtc.rtc_counter++;
    if(ptr->rtc.rtc_counter >= ptr->tim.freq)
    {
        ptr->rtc.rtc_counter = 0;
        ptr->rtc.sec++;
        if(ptr->rtc.sec >= 60)
        {
            ptr->rtc.sec = 0;
            ptr->rtc.min++;
            if(ptr->rtc.min >= 60)
            {
                ptr->rtc.min = 0;
                ptr->rtc.hour++;
                if(ptr->rtc.hour >=24)
                {
                    ptr->rtc.hour = 0;
                }
            }
        }
    }
}

/* ##################################################
 *   @hardware handler
 *   shared between devices, needs to be reentrant
 #################################################*/
void feed(uint16_t dev_id)
{
    dev_timer* ptr = dev_timer_array[dev_id];
    if(ptr->ctrl.bit.occupied && ptr->ctrl.bit.status)
    {
        ptr->tim.watch++;
        if(ptr->ctrl.bit.mode == TIM_NROM)
        {
            if(ptr->tim.counter < ptr->tim.threshold)
            {
                ptr->tim.counter++;
            }
            else
            {
                ptr->tim.counter = 0;
                if(ptr->callback != NULL)
                {
                    ptr->callback();
                }
            }
        }
        if(ptr->ctrl.bit.rtc_en)     //rtc
        {
           feed_rtc(dev_id);
        }
    }
}
/* ##################################################
 *   @device methods
 #################################################*/
int setTimer(int dev_id, enum TIM_MODE mode, size_t interval,void (*callback)(void), err_handler_t default_err)
{
    size_t threshold;
    DEV_CHK(dev_id);
    if(dev_id >= TIMER_MAX || dev_id < 0)
    {
        return -1;
    }
    dev_timer* ptr = dev_timer_array[dev_id];
    if(ptr->ctrl.bit.init == 0)
    {
        PERROR("timer init fail, cannot be used");
        return -1;
    }
    if(ptr->ctrl.bit.occupied)
    {
        PERROR("timer occupied, delete first");
        return -1;
    }
    ptr->ctrl.bit.status = 0;    //stop timer
    if(mode == TIM_CTR)       //counter mode
    {
        ptr->ctrl.bit.mode = TIM_CTR;
        ptr->callback = NULL;
        bsp_timer_ctr_set(timer_phy_ptr[dev_id],&ptr->tim.unit,&ptr->tim.hrunit);
    }
    else                      //normal timer
    {
        ptr->ctrl.bit.mode = TIM_NROM;
        threshold = interval / ptr->tim.step;
        if(threshold <= 0)
        {
            PERROR("timer interval too small or smaller than 0");
            return -1;
        }
        ptr->tim.threshold = threshold;
        ptr->callback = callback;
    }
    ptr->tim.counter = 0;
    ptr->tim.watch = 0;
    ptr->default_err = default_err;

    ptr->ctrl.bit.occupied = 1;
    ptr->ctrl.bit.rtc_en = 0;
    if(mode == TIM_NROM)       //normal mode, auto run
    {
        ptr->ctrl.bit.status = 1;
    }
    return 1;
}

int delTimer(int dev_id)
{
   DEV_CHK(dev_id);
   dev_timer* ptr = dev_timer_array[dev_id];
   ptr->ctrl.bit.occupied = 0;
   ptr->ctrl.bit.status = 0;
   return 1;
}

int runTimer(int dev_id)
{
    DEV_CHK(dev_id);
    dev_timer* ptr = dev_timer_array[dev_id];
    if(ptr->ctrl.bit.occupied == 0)
    {
        return -1;
    }
    ptr->ctrl.bit.status = 1;
    if(ptr->ctrl.bit.mode == TIM_CTR)
    {
       bsp_timer_switch(timer_phy_ptr[dev_id],1);
    }
    return 1;
}

int stopTimer(int dev_id)
{
   DEV_CHK(dev_id);
   dev_timer* ptr = dev_timer_array[dev_id];
   if(ptr->ctrl.bit.occupied == 0)
   {
       return -1;
   }
   ptr->ctrl.bit.status = 0;
   if(ptr->ctrl.bit.mode == TIM_CTR)
   {
       bsp_timer_switch(timer_phy_ptr[dev_id],0);
   }
   return 1;
}

int reloadTimer(int dev_id)
{
    DEV_CHK(dev_id);
    dev_timer* ptr = dev_timer_array[dev_id];
    if(ptr->ctrl.bit.occupied == 0)
    {
       return -1;
    }
    ptr->tim.counter = 0;
    ptr->tim.watch = 0;
    bsp_timer_reload(timer_phy_ptr[dev_id]);
    return 1;
}

size_t readTimer(int dev_id)
{
    DEV_CHK_U(dev_id);
    dev_timer* ptr = dev_timer_array[dev_id];
    size_t us_num;
    if(ptr->ctrl.bit.occupied == 0)
    {
       return 0;
    }
    us_num = ptr->tim.step * ptr->tim.watch;
    if(ptr->ctrl.bit.mode == TIM_CTR)
    {
        us_num += (bsp_timer_readCTR(timer_phy_ptr[dev_id]) * ptr->tim.unit +
                bsp_timer_readPRECTR(timer_phy_ptr[dev_id]) * ptr->tim.hrunit);
    }
    return us_num;
}

void readRtc(int dev_id,int* hour,int* min,int* sec)
{
    DEV_CHK_N(dev_id);
    dev_timer* ptr = dev_timer_array[dev_id];
    *hour = ptr->rtc.hour;
    *min = ptr->rtc.min;
    *sec = ptr->rtc.sec;
}

int configTimer(int dev_id,...)
{
    int res=-1;
    DEV_CHK(dev_id);
    va_list arg;
    va_start(arg,dev_id);
    enum TIM_CFG cfg;
    dev_timer* ptr = dev_timer_array[dev_id];
    cfg = va_arg(arg,enum TIM_CFG);
    switch(cfg)
    {
    case TIM_CFG_HPRD:
    {
        uint32_t period = va_arg(arg,uint32_t);
        res = bsp_timer_tim_set(timer_phy_ptr[dev_id],period);
        if(res == 1)
        {
            ptr->tim.step = period;
            ptr->tim.freq = (1000000/period);
        }
    }
        break;
    case TIM_CFG_RTC:
    {
        int sw = va_arg(arg,int);
        if(ptr->tim.freq < 5)     //not enough accuracy for rtc
        {
            res = -1;
        }
        else
        {
            ptr->ctrl.bit.rtc_en = 1;
            res = 1;
        }
    }
        break;
    }
    return res;
}

int help(int io)
{
    return 1;
}


void dev_timer_init(void)
{
    int res;
    bsp_timer_preinit();
    /*
     * @ Timer device Prototype Initialize
     */
    #define DEV_INFO_TIMER(DEV_ID,  DEV_EN, DEV_NAME, PHY_PTR, INTERVAL, ISR_EN, DESC)\
            dev_timer_array[DEV_ID] = DEV_NAME ## _ptr;\
            memset(&DEV_NAME,0,sizeof(dev_timer));\
            strcpy(DEV_NAME.dev, DESC);\
            DEV_NAME.tim.step = INTERVAL;\
            strcpy(DEV_NAME.desc,"Device Type: timer");\
            if(DEV_EN)\
            {\
                res = bsp_timer_init(PHY_PTR,INTERVAL);\
                if(res == 1) {\
                    DEV_NAME.ctrl.bit.init = 1;\
                    DEV_NAME.tim.step = INTERVAL;\
                    DEV_NAME.tim.freq = (1000000/INTERVAL);\
                }\
            }
    #include "bsp/dev_info.h"
    #undef DEV_INFO_TIMER

}

/* ##################################################
 *   @hardware interrupt
 #################################################*/
#if TIMER0_EN
__interrupt void timer0_isr_handler(void)
{
    OS_SWI_ENTER_CIRTICAL();
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
    feed(TIMER0);
    OS_SWI_EXIT_CIRTICAL();
}
#endif

#if TIMER1_EN
__interrupt void timer1_isr_handler(void)
{
    feed(TIMER1);
}
#endif

#if OS_EN == 0
#if TIMER2_EN
__interrupt void timer2_isr_handler(void)
{
    feed(TIMER2);
}
#endif
#endif

