/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    epwm.h
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * epwm interface
##################################################################*/
#ifndef EPWM_H_
#define EPWM_H_

/*####################################################
 *@ EPWM device id
 *  12 on chip devices, each devices has two channel PWMA and PWMB
######################################################*/
enum
{
    EPWM1,EPWM2,EPWM3,EPWM4,
    EPWM5,EPWM6,EPWM7,EPWM8,
    EPWM9,EPWM10,EPWM11,EPWM12,
};
/*####################################################
 *@ Function Parameter enumeration
######################################################*/
enum PWM_CHAN
{
    PWMA,      //PWM A
    PWMB,      //PWM B
};  /*   channel, each device has two channel   */
/* IMPORTANT! Two channel share the same carrier, but the actions can be set independently */
enum PWM_OUT
{
    A_NOTA,   //Output A:PWMA, Output B:NOT PWMA
    NOTA_A,   //Output A:NOT PWMA, Output B:PWMA
    A_B,      //Output A:PWMA, Output B:PWMB
    B_A,      //Output A:PWMB, Output B:PWMA
};  /*   Output connection, each device has two output   */
/* IMPORTANT! Two channel can be connected to two output via different patterns */
enum PWM_CAR
{
    SAW_RISE,    //sawtooth rising edge
    SAW_FALL,    //sawtooth falling edge
    TRIANGLE,    //triangle
}; /*   carrier type   */
enum PWM_CAR_DIR
{
   UP_COUNT,   //carrier currently up counting
   DOWN_COUNT, //carrier currently downing counting
}; /*   carrier counting direction   */
enum PWM_CMP
{
    CMP_A,
    CMP_B,
    CMP_C,
    CMP_D,
}; /*   comparator ID, 4 comparators   */
enum PWM_OPR
{
    NO_ACT,    //do nothing
    SET_L,     //output: set low
    SET_H,     //output: set high
    TOGGLE,    //output: toggle
}; /*   output operation   */
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
}; /*   PWM condition   */
enum PWM_EVENT_AD
{
    SOCA,       //internal AD SOCA
    SOCB,       //internal AD SOCB
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

/*#################################################
 *  @function list
 *  1.  setAllEpwmStart             //set all Epwm clock start
 *  2.  setAllEpwmStop              //set all Epwm clock stop
 *  3.  setEpwmCarrier              //set carrier
 *  4.  setEpwmCarrierPhs           //set carrier phase
 *  5.  getEpwmCarrierDir           //get carrier direction
 *  6.  getEpwmCarrierPrd           //get carrier period in sec
 *  7.  setEpwmCmp                  //set comparator value
 *  8.  setEPwmAction               //set output action
 *  9.  setEpwmEvent                //set event trigger
 *  10. delEpwmEvent                //del event trigger
 *  11. setEpwmEventAD              //set event ad trigger
 *  12. delEpwmEventAD              //del event ad trigger
 *  13. setEpwmDeadband             //set dead time
 *  14. setEpwmOutput               //manually set output value
 *  15. setEpwmOutputMode           //set output connection
 *  16. setEpwmChainSyncIn          //set device-chain synchronization input
 *  17. setEpwmChainSyncInExt       //set device-chain external sync gpio
 *  18. setEpwmSynBypass            //set device bypass sync
 *  19. delEpwmSynBypass            //del device bypass sync
 *  20. softSyncEpwm                //software sync
 *  21. resetEpwm                   //reset epwm
 *###################################################*/
/*****************************************************
 *  @setAllEpwmStart
 *  @brief
     start All Epwm clock
 *****************************************************/
extern  void              setAllEpwmStart(void);

/*****************************************************
 *  @setAllEpwmStop
 *  @brief
     Stop All Epwm clock
 *****************************************************/
extern  void              setAllEpwmStop(void);

/*****************************************************
 *  @setEpwmCarrier
 *  @brief
     set epwm carrier
    @in parameter
        dev_id,               device id
        mode,                 carrier mode, see PWM_CAR
        freq,                 carrier frequency
        phase,                carrier phase, range from [0,2pi)
        min_step,             min.   step number, smallest counter value
        max_step,             max.   step number, biggest counter value
        ***step number are used to calculate the appropriate clk freq. input of the epwm module,.
           As the available input clk is discrete number, there is no gurantee that the appropriate clk
           can be found.  Increase the (min,max) range if failed.
    @return
        real_step,            calculated real step numbers
 *****************************************************/
extern  size_t            setEpwmCarrier(int dev_id,enum PWM_CAR mode,float freq,float phase,size_t min_step,size_t max_step);

/*****************************************************
 *  @setEpwmCarrierPhs
 *  @brief
     set  carrier phase
   　@in parameter
     phase              　   carrier phase, range from [0,2pi)
 *****************************************************/
extern  int               setEpwmCarrierPhs(int dev_id, float phase);

/*****************************************************
 *  @setEpwmCarrierPhs
 *  @return
     get carrier direction, see PWM_CAR_DIR
 *****************************************************/
extern  enum PWM_CAR_DIR  getEpwmCarrierDir(int dev_id);

/*****************************************************
 *  @setEpwmCarrierPhs
 *  @return
     get carrier period in us
 *****************************************************/
extern  float             getEpwmCarrierPrd(int dev_id);

/*****************************************************
 *  @setEpwmCmp
 *  @in parameter
     cmp_id,             comparator id, see PWM_CMP
     pu_value,           comparator value in pu, the cross point of carrier, range from (0,1)
     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEpwmCmp(int dev_id, enum PWM_CMP cmp_id, float pu_value);

/*****************************************************
 *  @setEPwmAction
 *  @brief
 *   execute action when condition happened
 *  @in parameter
     ch_id,              channel id, each device has two channel, epwma and epwmb
     condition,          action triggered condition, see  PWM_CDT
                         ***only CMP_A, CMP_B, PEAK and Bottom can be used
     operation,          the operation that will be executed, see PWM_OPR
     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEPwmAction(int dev_id,enum PWM_CHAN ch_id,enum PWM_CDT condition, enum PWM_OPR operation);

/*****************************************************
 *  @setEpwmEvent
 *  @brief
 *   executed user callback when condition happened
 *  @in parameter
     condition,          action triggered condition, see  PWM_CDT
     callback,           the user function that will be executed
                         ****the user function cannot block
     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEpwmEvent(int dev_id,enum PWM_CDT condition, void (*callback)(void));

/*****************************************************
 *  @setEpwmEvent
 *  @brief
 *   delete user callback
 *****************************************************/
extern  int               delEpwmEvent(int dev_id);

/*****************************************************
 *  @setEpwmEventAD
 *  @brief
 *   executed internal AD sampling when condition happened
 *  @in parameter
     condition,          action triggered condition, see  PWM_CDT
     ad_id,              the AD that will be triggered
     callback,           the user function that will be executed
                         ****the user function cannot block
     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEpwmEventAD(int dev_id,enum PWM_EVENT_AD ad_id,enum PWM_CDT condition, void (*callback)(void));

/*****************************************************
 *  @delEpwmEventAD
 *  @brief
 *   delete AD triggering and user callback
 *****************************************************/
extern  int               delEpwmEventAD(int dev_id,enum PWM_EVENT_AD ad_id);

/*****************************************************
 *  @setEpwmDeadband
 *  @brief
 *    set dead time
 *    **** dead time only affects when in A_NOTA or NOTA_A output mode, see PWM_OUT
 *  @in parameter
     us,                  dead time in us

     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEpwmDeadband(int dev_id, size_t us);

/*****************************************************
 *  @setEpwmOutput
 *  @brief
 *    set output voltage manually
 *  @in parameter
     ch_id,              channel id, each device has two channel, epwma and epwmb
     output,             set high or set low, see PWM_OPR

     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEpwmOutput(int dev_id,enum PWM_CHAN ch_id,enum PWM_OPR output);

/*****************************************************
 *  @setEpwmOutputMode
 *  @brief
 *    two output port can be connected to two pwm channel in different mode
 *  @in parameter
     outmode,             see  PWM_OUT

     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEpwmOutputMode(int dev_id,enum PWM_OUT outmode);

/*****************************************************
 *  @setEpwmSyncIn
 *  @brief
 *    set EPWM synchronization input
 *  @in parameter
     chain_id,            PWM1-3, 4-6, 7-9, 10-12 are connected into 3 chains.
                          **** PWM1-3 has default input from EXTSYNIN1, cannot change
     src,                 synchronization source, see PWM_SYN_SRC

     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEpwmChainSyncIn(enum PWM_SYN_CHN chain_id,enum PWM_SYN_SRC src);

/*****************************************************
 *  @setEpwmSyncInExt
 *  @brief
 *    set the gpio of EPWM external synchronization input
 *  @in parameter
     ext_id,           　　 EXTSYNIN1　and EXTSYNIN2
     gpio_id,             the gpio that are connected to the epwm device

     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern  int               setEpwmChainSyncInExt(enum PWM_SYN_SRC ext_id, int gpio_id);


/*****************************************************
 *  @setEpwmSyncBypass
 *  @brief
 *    bypass the sync. input
 *  @in parameter
     dev_id               device id

     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern int                setEpwmSyncBypass(int dev_id);


/*****************************************************
 *  @delEpwmSyncBypass
 *  @brief
 *    del bypass the sync. input
 *  @in parameter
     dev_id               device id

     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern int                delEpwmSyncBypass(int dev_id);


/*****************************************************
 *  @softSyncEpwm
 *  @brief
 *    software sync. the chain of epwm device
 *  @in parameter
     chain_id,           　chain ID

     @return
     1,                   success
     -1,                  failure
 *****************************************************/
extern int                softSyncEpwm(enum PWM_SYN_CHN chain_id);

/*****************************************************
 *  @resetEpwm
 *  @brief
 *   reset epwm device, delete all events and actions
 *****************************************************/
extern  int               resetEpwm(int ch_id);
#endif
