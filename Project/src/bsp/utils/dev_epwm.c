/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_epwm.c
 * @AUTHOR  Brox Chen
 * @DATE    Aug 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * device manage: dma
##################################################################*/
#define DEV_EPWM_C_
#include "bsp/utils/dev_epwm.h"
#include "bsp/bsp_xbar.h"

/* ##################################################
 *   @device declare
 #################################################*/
/*
 * @ EPWM device Id enumeration
 */
#define  DEV_INFO_EPWM(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, ISR_RT, DESC)\
    DEV_ID,
enum
{
#include "bsp/dev_info.h"
EPWM_MAX,
};
#undef DEV_INFO_EPWM

/*
 * @ EPWM physic pointer Array
 */

#define DEV_INFO_EPWM(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, ISR_RT, DESC)\
    (void*)PHY_PTR,
void* epwm_phy_ptr[] = {
#include "bsp/dev_info.h"
};
#undef DEV_INFO_EPWM

/*
 * @ EPWM device Prototype Declare
 */

#define DEV_INFO_EPWM(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, ISR_RT, DESC)\
        dev_epwm DEV_NAME;
#include "bsp/dev_info.h"
#undef DEV_INFO_EPWM

/*
 * @ EPWM device Prototype pointer
 */
dev_epwm* dev_epwm_array[EPWM_MAX];


static const uint16_t clkdiv_v[] = {1,2,4,8,16,32,64,128};
static const uint16_t hspdiv_v[] = {1,2,4,6,8,10,12,14};
static const uint16_t maxdiv = 128*14;
/* ##################################################
 *   @device check MACRO
 #################################################*/
#define  DEV_CHK(dev_id,ret)\
    if(dev_id >=  EPWM_MAX || dev_id < 0)    return ret;
#define DEV_STATUS_CHK(ptr,ret)\
    if(ptr->ctrl.bit.ready == 0)             return ret;
/* ##################################################
 *   @internal functions
 #################################################*/
enum BSP_EPWM_TB_MODE epwm_carr_getmode(enum PWM_CAR mode)
{
    switch(mode)
    {
    case SAW_RISE:
    default:
        return TB_COUNT_UP;
    case SAW_FALL:
        return TB_COUNT_DOWN;
    case TRIANGLE:
        return TB_COUNT_UPDOWN;
    }
}


uint8_t  epwm_carr_trystep(size_t* try_step,float tar_ratio, uint16_t* hsp, uint16_t* zerocount)
{
    float  div_sum,dif_percent;
    size_t div_temp,zero_count;

    div_sum = (tar_ratio/(*try_step));
    div_temp = div_sum;
    if(div_temp > maxdiv)  //exceed maximum
    {
        *try_step = (*try_step) * div_temp/maxdiv;
        return 0;
    }
    zero_count = 0;
    while(div_temp > 7) {div_temp >>= 1;zero_count++;}
    if(zero_count == 0)    //zero_count must bigger than zero
    {
        *try_step = 2*(*try_step);
        return 0;
    }
    *hsp = div_temp;
    div_temp = (div_temp<<zero_count);
    dif_percent = fabs(div_sum/div_temp - 1); //dif_percent = |tar_ratio/real_ratio - 1|
    if(dif_percent < EPWM_CAR_MAX_ERR)
    {
        *zerocount = zero_count;
        return 1;
    }
    return 0;
}

size_t epwm_carr_set_clk(int dev_id,enum BSP_EPWM_TB_MODE mode, float carr_freq, size_t min_step,size_t max_step)
{
    uint8_t res;
    float epwm_clk = bsp_clk_get_epwmclk() * 1000000;
    size_t try_step,inc_step;
    float tar_ratio = epwm_clk/carr_freq;
    uint16_t div,hdiv,zerocount;
    inc_step = 1;
    if(mode == TB_COUNT_UPDOWN)  //updown, steps should be even
    {
        inc_step = 2;
        min_step += (min_step & 0x1);
        max_step -= (max_step & 0x1);
    }
    if(carr_freq < (epwm_clk / maxdiv / 0x10000))  //smaller than minimum freq, exit
    {
        return 0;
    }
    /*  calc. carrier div and clk  */
    for(try_step=max_step;try_step>=min_step;try_step-=inc_step)
    {
        res = epwm_carr_trystep(&try_step,tar_ratio,&hdiv,&zerocount);
        if(res)
        {
            div = zerocount-1;
            /*      set bsp       */
            bsp_epwm_carrier_mode(epwm_phy_ptr[dev_id],mode);
            bsp_epwm_carrier_clk(epwm_phy_ptr[dev_id],div,hdiv);
            if(mode == TB_COUNT_UPDOWN)
            {
                try_step >>= 1;
                bsp_epwm_carrier_prd(epwm_phy_ptr[dev_id],try_step-1,0);
            }
            else
            {
                bsp_epwm_carrier_prd(epwm_phy_ptr[dev_id],try_step-1,0);
            }
            return try_step;
        }
    }
    return 0;
}


int epwm_deadband_update(int dev_id,size_t us)
{
    dev_epwm* ptr = dev_epwm_array[dev_id];
    switch(ptr->out_mode)
    {
    case A_B:
    case B_A:
    default:
        return -1;
    case A_NOTA:
    case NOTA_A:
    {
        //t_red = t_fed = us, step = t_red / carr_prd * carr_prdsteps
        float step_f = us * ptr->car.tim.prdsteps / ptr->car.tim.period;
        size_t step = step_f;
        if(step > 0x3fff)  //maximum 14bits
        {
            return -1;
        }
        bsp_epwm_db_red(epwm_phy_ptr[dev_id],step,0);
        bsp_epwm_db_fed(epwm_phy_ptr[dev_id],step,0);
    }
        break;
    }
    return 1;
}
/* ##################################################
 *   @hardware handler
 *   shared between devices, needs to be reentrant
 #################################################*/
void epwm_interrupt_handler(int dev_id)
{
    dev_epwm* ptr = dev_epwm_array[dev_id];
    //clear isr flag
    bsp_epwm_event_clr(epwm_phy_ptr[dev_id]);
    if(ptr->event_cb)
    {
        ptr->event_cb();
    }
}

/* ##################################################
 *   @device methods
 #################################################*/
void setAllEpwmStart(void)
{
    bsp_epwm_sys_clk(1);
}
void setAllEpwmStop(void)
{
    bsp_epwm_sys_clk(0);
}

int setEpwmCarrierPhs(int dev_id, float phs)
{
    size_t offset;
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    DEV_STATUS_CHK(ptr,-1);
    ptr->car.tim.phase = phs;
    switch(ptr->car.tim.type)
    {
    case SAW_RISE:
        offset = ptr->car.tim.steps * (phs/(2*PI));
        break;
    case SAW_FALL:
        offset = ptr->car.tim.steps * (1-phs/(2*PI));
        break;
    case TRIANGLE:
        offset = ptr->car.tim.steps * (0.5-fabs(0.5-phs/(2*PI)));
        break;
    default:
        return -1;
    }
    bsp_epwm_carrier_phase(epwm_phy_ptr[dev_id],offset,0);
    return 1;
}

size_t  setEpwmCarrier(int dev_id,enum PWM_CAR mode,float freq,float phase,size_t min_step,size_t max_step)
{
    size_t res;
    size_t real_steps;
    dev_epwm* ptr = dev_epwm_array[dev_id];
    DEV_STATUS_CHK(ptr,0);
    DEV_CHK(dev_id,0);
    if(max_step >= 0x10000)
    {
        return 0;
    }
    if(phase < 0 || phase >= 2*PI)
    {
        return 0;
    }
    if(mode == TB_FREEZE)
    {
        return 0;
    }
    else
    {
        res = epwm_carr_set_clk(dev_id,epwm_carr_getmode(mode),freq,min_step,max_step);
        if(res)
        {
            real_steps = res;
            ptr->car.tim.type = mode;
            ptr->car.tim.freq = freq;
            ptr->car.tim.period = 1000000./freq;
            ptr->car.tim.steps = real_steps;
            if(mode == TB_COUNT_UPDOWN)
                ptr->car.tim.prdsteps = (real_steps << 1);
            else
                ptr->car.tim.prdsteps = real_steps;
            setEpwmCarrierPhs(dev_id,phase);
        }
    }
    return real_steps;
}

enum PWM_CAR_DIR  getEpwmCarrierDir(int dev_id)
{
    DEV_CHK(dev_id,0);
    enum BSP_EPWM_TB dir;
    dir = bsp_epwm_get_carrier_dir(epwm_phy_ptr[dev_id]);
    switch(dir)
    {
    case TB_DOWN:
    default:
        return DOWN_COUNT;
    case TB_UP:
        return UP_COUNT;
    }
}

float getEpwmCarrierPrd(int dev_id)
{
    DEV_CHK(dev_id,0);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    return ptr->car.tim.period;
}

int setEpwmCmp(int dev_id, enum PWM_CMP id, float pu_value)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    DEV_STATUS_CHK(ptr,-1);
    if(pu_value >= 1 || pu_value <= -1)
    {
        return -1;
    }
    size_t cmp_value = pu_value * ptr->car.tim.steps - 1 ;
    switch(id)
    {
    case CMP_A:
        bsp_epwm_cmp_a(epwm_phy_ptr[dev_id], cmp_value, 0);
        break;
    case CMP_B:
        bsp_epwm_cmp_b(epwm_phy_ptr[dev_id], cmp_value, 0);
            break;
    case CMP_C:
        bsp_epwm_cmp_c(epwm_phy_ptr[dev_id], cmp_value);
            break;
    case CMP_D:
        bsp_epwm_cmp_d(epwm_phy_ptr[dev_id], cmp_value);
            break;
    default:
        return -1;
    }
    ptr->cmp.steps[id] = cmp_value;
    return 1;
}

int setEPwmAction(int dev_id,enum PWM_CHAN ch_id,enum PWM_CDT condition, enum PWM_OPR operation)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    DEV_STATUS_CHK(ptr,-1);
    switch(condition)
    {
    case CROSS_A_U:
        if(ch_id == PWMA)
            bsp_epwm_act_a_cmpa(epwm_phy_ptr[dev_id],TB_UP, operation);
        else if(ch_id == PWMB)
            bsp_epwm_act_b_cmpa(epwm_phy_ptr[dev_id],TB_UP, operation);
        break;
    case CROSS_A_D:
        if(ch_id == PWMA)
            bsp_epwm_act_a_cmpa(epwm_phy_ptr[dev_id],TB_DOWN, operation);
        else if(ch_id == PWMB)
            bsp_epwm_act_b_cmpa(epwm_phy_ptr[dev_id],TB_DOWN, operation);
        break;
    case CROSS_B_U:
        if(ch_id == PWMA)
            bsp_epwm_act_a_cmpb(epwm_phy_ptr[dev_id],TB_UP, operation);
        else if(ch_id == PWMB)
            bsp_epwm_act_b_cmpb(epwm_phy_ptr[dev_id],TB_UP, operation);
        break;
    case CROSS_B_D:
        if(ch_id == PWMA)
            bsp_epwm_act_a_cmpb(epwm_phy_ptr[dev_id],TB_DOWN, operation);
        else if(ch_id == PWMB)
            bsp_epwm_act_b_cmpb(epwm_phy_ptr[dev_id],TB_DOWN, operation);
        break;
    case BOTTOM:
        if(ch_id == PWMA)
            bsp_epwm_act_a_bottom(epwm_phy_ptr[dev_id],operation);
        else if(ch_id == PWMB)
            bsp_epwm_act_b_bottom(epwm_phy_ptr[dev_id],operation);
        break;
    case PEAK:
        if(ch_id == PWMA)
            bsp_epwm_act_a_peak(epwm_phy_ptr[dev_id],operation);
        else if(ch_id == PWMB)
            bsp_epwm_act_b_peak(epwm_phy_ptr[dev_id],operation);
         break;
    default:
        return -1;
    }
    return 1;
}

int setEpwmEvent(int dev_id,enum PWM_CDT condition, void (*callback)(void))
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    DEV_STATUS_CHK(ptr,-1);
    bsp_epwm_event(epwm_phy_ptr[dev_id],1, condition+1);   /*  set event */
    if(callback)
    {
        ptr->event_cb = callback;
    }
    return 1;
}

int delEpwmEvent(int dev_id)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    bsp_epwm_event(epwm_phy_ptr[dev_id],0, 0);
    ptr->event_cb = NULL;
    return 1;
}

int setEpwmEventAD(int dev_id,enum PWM_EVENT_AD ad_id,enum PWM_CDT condition, void (*callback)(void))
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    DEV_STATUS_CHK(ptr,-1);
    if(ad_id == SOCA)
    {
        bsp_epwm_event_ad_a(epwm_phy_ptr[dev_id],1, condition + 1);
        ptr->event_ad_a_cb = callback;
    }
    else if(ad_id == SOCB)
    {
        bsp_epwm_event_ad_b(epwm_phy_ptr[dev_id],1, condition + 1);
        ptr->event_ad_b_cb = callback;
    }
    else
    {
        return -1;
    }
    return 1;
}

int delEpwmEventAD(int dev_id,enum PWM_EVENT_AD ad_id)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    if(ad_id == SOCA)
    {
        bsp_epwm_event_ad_a(epwm_phy_ptr[dev_id],0, 0);
        ptr->event_ad_a_cb = NULL;
    }
    else if(ad_id == SOCB)
    {
        bsp_epwm_event_ad_b(epwm_phy_ptr[dev_id],0, 0);
        ptr->event_ad_b_cb = NULL;
    }
    else
    {
        return -1;
    }
    return 1;
}

int  setEpwmDeadband(int dev_id, size_t us)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    DEV_STATUS_CHK(ptr,-1);
    if(us >= ptr->car.tim.period)
    {
        return -1;
    }
    if(epwm_deadband_update(dev_id,us) == -1)
    {
        return -1;
    }
    ptr->deadtime = us;
    return 1;
}

int  setEpwmOutputMode(int dev_id,enum PWM_OUT outmode)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    DEV_STATUS_CHK(ptr,-1);
    switch(outmode)
    {
    case A_B:
        bsp_epwm_db_outmode(epwm_phy_ptr[dev_id],DB_DISABLE);  //A_B mode does not has deadtime
        bsp_epwm_db_swap(epwm_phy_ptr[dev_id],0);
        break;
    case B_A:
        bsp_epwm_db_outmode(epwm_phy_ptr[dev_id],DB_DISABLE);  //B_A mode does not has deadtime
        bsp_epwm_db_swap(epwm_phy_ptr[dev_id],1);
        break;
    case A_NOTA:
        bsp_epwm_db_outmode(epwm_phy_ptr[dev_id],DB_FULL_ENABLE);  //A_NOTA mode has deadtime
        bsp_epwm_db_inmode(epwm_phy_ptr[dev_id],DBA_ALL);
        bsp_epwm_db_pol(epwm_phy_ptr[dev_id],DB_ACTV_HIC);
        bsp_epwm_db_swap(epwm_phy_ptr[dev_id],0);
        epwm_deadband_update(dev_id,ptr->deadtime);
        break;
    case NOTA_A:
        bsp_epwm_db_outmode(epwm_phy_ptr[dev_id],DB_FULL_ENABLE);
        bsp_epwm_db_inmode(epwm_phy_ptr[dev_id],DBA_ALL);
        bsp_epwm_db_pol(epwm_phy_ptr[dev_id],DB_ACTV_HIC);
        bsp_epwm_db_swap(epwm_phy_ptr[dev_id],1);
        epwm_deadband_update(dev_id,ptr->deadtime);
        break;
    default:
        return -1;
    }
    ptr->out_mode = outmode;
    return 1;
}

int  setEpwmOutput(int dev_id,enum PWM_CHAN ch_id,enum PWM_OPR output)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    switch(ch_id)
    {
    case PWMA:
        bsp_epwm_act_a_force(epwm_phy_ptr[dev_id],output);
        break;
    case PWMB:
        bsp_epwm_act_b_force(epwm_phy_ptr[dev_id],output);
        break;
    default:
        return -1;
    }
    return 1;
}


int  setEpwmChainSyncIn(enum PWM_SYN_CHN chain_id,enum PWM_SYN_SRC src)
{
    enum BSP_EPWM_SYNCI_SRC sel = (enum BSP_EPWM_SYNCI_SRC)src;
    switch(chain_id)
    {
    case PWM1CHN:
        //fixed, cannot change
    default:
         return -1;
    case PWM4CHN:
        bsp_epwm_4_synci_sel(sel);
        break;
    case PWM7CHN:
        bsp_epwm_7_synci_sel(sel);
        break;
    case PWM10CHN:
        bsp_epwm_10_synci_sel(sel);
        break;
    }
    return 1;
}

int setEpwmChainSyncInExt(enum PWM_SYN_SRC ext_id, int gpio_id)
{
    switch(ext_id)
    {
    case EXTSYNIN1:
        bsp_xbar_set_input(5, gpio_id);
        break;
    case EXTSYNIN2:
        bsp_xbar_set_input(6, gpio_id);
        break;
    default:
        return -1;
    }
    return 1;
}

int setEpwmSyncBypass(int dev_id)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    bsp_epwm_sync_switch(epwm_phy_ptr[dev_id], 0);
    return 1;
}

int delEpwmSyncBypass(int dev_id)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    bsp_epwm_sync_switch(epwm_phy_ptr[dev_id], 1);
    return 1;
}

int softSyncEpwm(enum PWM_SYN_CHN chain_id)
{
    int dev_id;
    switch(chain_id)
    {
    case PWM1CHN:
        dev_id = EPWM1;
        break;
    case PWM4CHN:
        dev_id = EPWM3;
        break;
    case PWM7CHN:
        dev_id = EPWM7;
        break;
    case PWM10CHN:
        dev_id = EPWM10;
        break;
    default:
        return -1;
    }
    bsp_epwm_soft_sync(epwm_phy_ptr[dev_id]);
    return 1;
}

int resetEpwm(int dev_id)
{
    DEV_CHK(dev_id,-1);
    dev_epwm* ptr = dev_epwm_array[dev_id];
    setEPwmAction(dev_id,PWMA,BOTTOM,NO_ACT);
    setEPwmAction(dev_id,PWMA,PEAK,NO_ACT);
    setEPwmAction(dev_id,PWMA,CROSS_A_U,NO_ACT);
    setEPwmAction(dev_id,PWMA,CROSS_A_D,NO_ACT);
    setEPwmAction(dev_id,PWMA,CROSS_B_U,NO_ACT);
    setEPwmAction(dev_id,PWMA,CROSS_B_D,NO_ACT);
    delEpwmEventAD(dev_id,SOCA);
    delEpwmEventAD(dev_id,SOCB);
    delEpwmEvent(dev_id);
    setEpwmOutputMode(dev_id,A_NOTA);
    return 1;
}

int dev_epwm_init(void)
{
    int res;

    bsp_epwm_sys_clk(0);      //disable local clk
   /*
    * @ epwm device Prototype Initialize
    */
#define DEV_INFO_EPWM(DEV_ID, DEV_EN, DEV_NAME, PHY_PTR, ISR_RT, DESC)\
         dev_epwm_array[DEV_ID] = &DEV_NAME;\
         memset(&DEV_NAME,0,sizeof(dev_epwm));\
         strcpy(DEV_NAME.dev, DESC);\
         strcpy(DEV_NAME.desc,"Device Type: epwm");\
         if(DEV_EN && (ISR_RT == CORE_ID)){\
         res = bsp_epwm_init(PHY_PTR,ISR_RT);\
         if(res == 1) {\
            DEV_NAME.ctrl.bit.ready = 1;\
            resetEpwm(DEV_ID);\
            }\
         }
 #include "bsp/dev_info.h"
 #undef DEV_INFO_EPWM
    return res;
}
/*##################################################
 *   @hardware interrupt
 #################################################*/
#if EPWM1_EN
__interrupt void epwm1_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM1);
}
#endif
#if EPWM2_EN
__interrupt void epwm2_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM2);
}
#endif
#if EPWM3_EN
__interrupt void epwm3_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM3);
}
#endif
#if EPWM4_EN
__interrupt void epwm4_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM4);
}
#endif
#if EPWM5_EN
__interrupt void epwm5_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM5);
}
#endif
#if EPWM6_EN
__interrupt void epwm6_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM6);
}
#endif
#if EPWM7_EN
__interrupt void epwm7_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM7);
}
#endif
#if EPWM8_EN
__interrupt void epwm8_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM8);
}
#endif
#if EPWM9_EN
__interrupt void epwm9_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM9);
}
#endif
#if EPWM10_EN
__interrupt void epwm10_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM10);
}
#endif
#if EPWM11_EN
__interrupt void epwm11_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM11);
}
#endif
#if EPWM12_EN
__interrupt void epwm12_isr_handler(void)
{
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP3;
    epwm_interrupt_handler(EPWM12);
}
#endif
