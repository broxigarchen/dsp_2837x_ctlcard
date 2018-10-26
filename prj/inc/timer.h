/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    time.h
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * timer interface
##################################################################*/
#ifndef TIME_H_
#define TIME_H_
/*####################################################
 *@ timer timer_id
 *@ Available Device:
         TIMER0,TIMER1,TIMER2
######################################################*/
enum
{
    TIMER0,
    TIMER1,
    TIMER2,
};

/*####################################################
 *@ timer interface
######################################################*/
enum TIM_MODE
{
    TIM_NROM,         //normal mode
    TIM_CTR,          //counter mode
};
/*********************************
 * @settimer
 *  1. cannot be set if occupied
 *  2. after set, timer default running
 * @input
 *  timer_id,     select timer
 *  timer_mode,   timer mode
 *  period,       set period in us, cannot smaller than hardware period
 *  callback,     callback function, be called once when timer complete a period
 *  default_err,  err callback function, be called when error occur
 * @timer mode
 *  1. TIM_NROM:
 *      normal timer, call the callback function each period
 *      automatically run after called.
 *  2. TIM_CTR:
 *      counter mode, do not call any function, period is ignored. use readtimer to read\
 *      the counter number.
 *      need to be activated using runtimer after rcalled
 * @return
 *   1,           succeed
 *  -1,           fail
 **********************************/
extern int setTimer(int timer_id, enum TIM_MODE mode, size_t period,void (*callback)(void), void *(default_err)(void));
extern int delTimer(void);
extern int runTimer(int timer_id);
extern int stopTimer(int timer_id);
extern int reloadTimer(int timer_id);
extern size_t readTimer(int timer_id);
extern void readRtc(int timer_id,int* hour,int* min,int* sec);

/*********************************
 * @config
 *  1. TIM_CFG_HPRD
 *  @usage
 *    configtimer(int timer_id, TIM_CFG_HPRD, uint32_t hard_prd);
 *  @brief
 *     reset timer after called
 *  @input
 *  timer_id,       select timer
 *  hardware_prd,   set hardware period in us, cannot exceed than the maximum period supported
 *  @return
 *    1,           succeed
 *   -1,           fail
 *
 *  2. TIM_CFG_RTC
 *  @usage
 *    configtimer(int timer_id, TIM_CFG_RTC, int switch);
 *  @brief
 *     enable real time clock
 *  @input
 *  timer_id,       select timer
 *  switch,         turn on/off rtc
 **********************************/
enum TIM_CFG
{
    TIM_CFG_HPRD,     //set hardware period
    TIM_CFG_RTC,      //enable real time clock, format hh:mm:ss
};
extern int configtimer(int timer_id, ...);


extern int help(int io);


#endif
