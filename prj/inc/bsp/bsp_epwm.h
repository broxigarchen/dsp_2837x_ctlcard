/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_epwm.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef BSP_EPWM_H_
#define BSP_EPWM_H_

#include "bsp/bsp_inc.h"

#ifdef BSP_EPWM_C_
#define BSP_EPWM_EXEC
#else
#define BSP_EPWM_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/
enum BSP_EPWM_TB_MODE
{
    TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN, TB_FREEZE,
};
enum BSP_EPWM_TB
{
    TB_DOWN, TB_UP,
};
enum BSP_EPWM_AQ_SRC
{
    DCAEVT1,DCAEVT2,DCBEVT1,DCBEVT2,TZ1,TZ2,TZ3,
};
enum BSP_EPWM_AQ_ACT
{
   AQ_NO_ACTION, AQ_CLEAR, AQ_SET, AQ_TOGGLE,
};
enum BSP_EPWM_EVT_TRIG
{
   ET_CTR_NAN,   ET_CTR_ZERO, ET_CTR_PRD, ET_CTR_PRDZERO,
  ET_CTRU_CMPA, ET_CTRD_CMPA, ET_CTRU_CMPB, ET_CTRD_CMPB,
  ET_CTRU_CMPC, ET_CTRD_CMPC, ET_CTRU_CMPD, ET_CTRD_CMPD,
};
enum BSP_EPWM_SYNCI_SRC
{
    EPWM1SYNCO,EPWM4SYNCO,EPWM7SYNCO,EPWM10SYNCO,
    EXTSYNCIN1,EXTSYNCIN2,
};
enum BSP_EPWM_SYNCO_SRC
{
    EPWM1SYNCOUT,EPWM4SYNCOUT,EPWM7SYNCOUT,EPWM10SYNCOUT,
};

enum BSP_EPWM_DB_OUT
{
    DB_DISABLE,DBB_ENABLE,DBA_ENABLE,DB_FULL_ENABLE,
};
enum BSP_EPWM_DB_IN
{
    DBA_ALL,DBB_RED_DBA_FED,DBA_RED_DBB_FED,DBB_ALL,
};
enum BSP_EPWM_DB_POL
{
    DB_ACTV_HI,  //default
    DB_ACTV_LOC, //A inverted
    DB_ACTV_HIC, //B inverted
    DB_ACTV_LO,  //both inverted
};
/******************************************************************
 * @ public
 *******************************************************************/
/*######################################################
 *            Clock && Carrier
 *  ##################################################*/
inline void bsp_epwm_carrier_mode(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_TB_MODE car_mode)
{
    pwm_ptr->TBCTR = 0;
    pwm_ptr->TBCTL.bit.CTRMODE = car_mode;
}
/*  TBCLK = EPWMCLK / (HDIV * DIV)  */
inline void bsp_epwm_carrier_clk(struct EPWM_REGS *pwm_ptr,uint16_t div, uint16_t hdiv)
{
    pwm_ptr->TBCTL.bit.CLKDIV = div;
    pwm_ptr->TBCTL.bit.HSPCLKDIV = hdiv;
}
inline void bsp_epwm_carrier_prd(struct EPWM_REGS *pwm_ptr,uint16_t prd, uint16_t hr_prd)
{
    pwm_ptr->TBPRDHR = hr_prd;
    pwm_ptr->TBPRD = prd;
}
inline void bsp_epwm_carrier_phase(struct EPWM_REGS *pwm_ptr,uint16_t offset, uint16_t hr_offset)
{
    pwm_ptr->TBPHS.bit.TBPHS = offset;
    pwm_ptr->TBPHS.bit.TBPHSHR = hr_offset;
}
inline void bsp_epwm_soft_sync(struct EPWM_REGS *pwm_ptr)
{
    pwm_ptr->TBCTL.bit.SWFSYNC = 1;
}
inline enum BSP_EPWM_TB bsp_epwm_get_carrier_dir(struct EPWM_REGS * pwm_ptr)
{
    if(pwm_ptr->TBSTS.bit.CTRDIR)//0:down, 1: up
        return TB_UP;
    else
        return TB_DOWN;
}

inline void bsp_epwm_sync_switch(struct EPWM_REGS * pwm_ptr, uint8_t on_off)
{
    pwm_ptr->TBCTL.bit.PHSEN = on_off;
}
/*######################################################
 *            Counter Comparator
 *  ##################################################*/
inline void bsp_epwm_cmp_a(struct EPWM_REGS * pmw_ptr, uint16_t val, uint16_t hr_val)
{
    uint32_t value = ((((uint32_t)val) << 16) | hr_val);
    pmw_ptr->CMPA.all = value;
}
inline void bsp_epwm_cmp_b(struct EPWM_REGS * pmw_ptr, uint16_t val, uint16_t hr_val)
{
    uint32_t value = ((((uint32_t)val) << 16) | hr_val);
    pmw_ptr->CMPB.all = value;
}
inline void bsp_epwm_cmp_c(struct EPWM_REGS * pmw_ptr, uint16_t val)
{
    pmw_ptr->CMPC = val;
}
inline void bsp_epwm_cmp_d(struct EPWM_REGS * pmw_ptr, uint16_t val)
{
    pmw_ptr->CMPD = val;
}
/*######################################################
 *           EPWM Actions
 *  ##################################################*/
inline void bsp_epwm_act_T2(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_AQ_SRC source)
{
    pwm_ptr->AQTSRCSEL.bit.T2SEL = source;
}
inline void bsp_epwm_act_T1(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_AQ_SRC source)
{
    pwm_ptr->AQTSRCSEL.bit.T1SEL = source;
}
inline void bsp_epwm_act_a_cmpa(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_TB count_dir, enum BSP_EPWM_AQ_ACT action)
{
    if(count_dir)
        pwm_ptr->AQCTLA.bit.CAU = action;
    else
        pwm_ptr->AQCTLA.bit.CAD = action;
}
inline void bsp_epwm_act_a_cmpb(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_TB count_dir, enum BSP_EPWM_AQ_ACT action)
{
    if(count_dir)
        pwm_ptr->AQCTLA.bit.CBU = action;
    else
        pwm_ptr->AQCTLA.bit.CBD = action;
}
inline void bsp_epwm_act_a_peak(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_AQ_ACT action)
{
        pwm_ptr->AQCTLA.bit.PRD = action;
}
inline void bsp_epwm_act_a_bottom(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_AQ_ACT action)
{
        pwm_ptr->AQCTLA.bit.ZRO = action;
}
inline void bsp_epwm_act_a_T2(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_TB count_dir, enum BSP_EPWM_AQ_ACT action)
{
    if(count_dir)
        pwm_ptr->AQCTLA2.bit.T2U = action;
    else
        pwm_ptr->AQCTLA2.bit.T2D = action;
}
inline void bsp_epwm_act_a_T1(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_TB count_dir, enum BSP_EPWM_AQ_ACT action)
{
    if(count_dir)
        pwm_ptr->AQCTLA2.bit.T1U = action;
    else
        pwm_ptr->AQCTLA2.bit.T1D = action;
}

inline void bsp_epwm_act_b_cmpa(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_TB count_dir, enum BSP_EPWM_AQ_ACT action)
{
    if(count_dir)
        pwm_ptr->AQCTLB.bit.CAU = action;
    else
        pwm_ptr->AQCTLB.bit.CAD = action;
}
inline void bsp_epwm_act_b_cmpb(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_TB count_dir, enum BSP_EPWM_AQ_ACT action)
{
    if(count_dir)
        pwm_ptr->AQCTLB.bit.CBU = action;
    else
        pwm_ptr->AQCTLB.bit.CBD = action;
}
inline void bsp_epwm_act_b_peak(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_AQ_ACT action)
{
        pwm_ptr->AQCTLB.bit.PRD = action;
}
inline void bsp_epwm_act_b_bottom(struct EPWM_REGS * pwm_ptr,enum BSP_EPWM_AQ_ACT action)
{
        pwm_ptr->AQCTLB.bit.ZRO = action;
}
inline void bsp_epwm_act_b_T2(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_TB count_dir, enum BSP_EPWM_AQ_ACT action)
{
    if(count_dir)
        pwm_ptr->AQCTLB2.bit.T2U = action;
    else
        pwm_ptr->AQCTLB2.bit.T2D = action;
}
inline void bsp_epwm_act_b_T1(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_TB count_dir, enum BSP_EPWM_AQ_ACT action)
{
    if(count_dir)
        pwm_ptr->AQCTLB2.bit.T1U = action;
    else
        pwm_ptr->AQCTLB2.bit.T1D = action;
}
inline void bsp_epwm_act_a_force(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_AQ_ACT action)
{
    pwm_ptr->AQSFRC.bit.ACTSFA = action;
    pwm_ptr->AQSFRC.bit.OTSFA = 1;
}
inline void bsp_epwm_act_b_force(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_AQ_ACT action)
{
    pwm_ptr->AQSFRC.bit.ACTSFB = action;
    pwm_ptr->AQSFRC.bit.OTSFB = 1;
}
/*######################################################
 *           Event trigger
 *  ##################################################*/
inline void bsp_epwm_event_ad_a(struct EPWM_REGS * pwm_ptr,uint8_t on_off, enum BSP_EPWM_EVT_TRIG trig)
{
    pwm_ptr->ETSEL.bit.SOCAEN = on_off;
    if(trig > ET_CTRD_CMPB)
    {
        pwm_ptr->ETSEL.bit.SOCASEL = trig - 4;
        pwm_ptr->ETSEL.bit.SOCASELCMP = 1;
    }
    else
    {
        pwm_ptr->ETSEL.bit.SOCASEL = trig;
        pwm_ptr->ETSEL.bit.SOCASELCMP = 0;
    }
}

inline void bsp_epwm_event_ad_b(struct EPWM_REGS * pwm_ptr,uint8_t on_off, enum BSP_EPWM_EVT_TRIG trig)
{
    pwm_ptr->ETSEL.bit.SOCBEN = on_off;
    if(trig > ET_CTRD_CMPB)
    {
        pwm_ptr->ETSEL.bit.SOCBSEL = trig - 4;
        pwm_ptr->ETSEL.bit.SOCBSELCMP = 1;
    }
    else
    {
        pwm_ptr->ETSEL.bit.SOCBSEL = trig;
        pwm_ptr->ETSEL.bit.SOCBSELCMP = 0;
    }
}

inline void bsp_epwm_event(struct EPWM_REGS * pwm_ptr,uint8_t on_off, enum BSP_EPWM_EVT_TRIG trig)
{
    pwm_ptr->ETSEL.bit.INTEN = on_off;
    if(trig > ET_CTRD_CMPB)
   {
       pwm_ptr->ETSEL.bit.INTSEL = trig - 4;
       pwm_ptr->ETSEL.bit.INTSELCMP = 1;
   }
   else
   {
       pwm_ptr->ETSEL.bit.INTSEL = trig;
       pwm_ptr->ETSEL.bit.INTSELCMP = 0;
   }
}

inline void bsp_epwm_event_clr_ad_a(struct EPWM_REGS * pwm_ptr)
{
    pwm_ptr->ETCLR.bit.SOCA = 1;
}
inline void bsp_epwm_event_clr_ad_b(struct EPWM_REGS * pwm_ptr)
{
    pwm_ptr->ETCLR.bit.SOCB = 1;
}
inline void bsp_epwm_event_clr(struct EPWM_REGS * pwm_ptr)
{
    pwm_ptr->ETCLR.bit.INT = 1;
}
/*######################################################
 *           Dead band & output
 *  ##################################################*/
inline void bsp_epwm_db_red(struct EPWM_REGS * pwm_ptr,  uint16_t value, uint16_t hr_value)
{
    pwm_ptr->DBREDHR.bit.DBREDHR = hr_value;
    pwm_ptr->DBRED.bit.DBRED = value;
}
inline void bsp_epwm_db_fed(struct EPWM_REGS * pwm_ptr, uint16_t value, uint16_t hr_value)
{
    pwm_ptr->DBFEDHR.bit.DBFEDHR = hr_value;
    pwm_ptr->DBFED.bit.DBFED = value;
}
inline void bsp_epwm_db_out(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_DB_OUT mode)
{
    pwm_ptr->DBCTL.bit.OUT_MODE = mode;
}
inline void bsp_epwm_db_swap(struct EPWM_REGS * pwm_ptr, uint8_t swap)
{
    if(swap)
        pwm_ptr->DBCTL.bit.OUTSWAP = 3;
    else
        pwm_ptr->DBCTL.bit.OUTSWAP = 0;
}
inline void bsp_epwm_db_inmode(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_DB_IN mode)
{
    pwm_ptr->DBCTL.bit.IN_MODE = mode;
}
inline void bsp_epwm_db_pol(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_DB_POL mode)
{
    pwm_ptr->DBCTL.bit.POLSEL = mode;
}
inline void bsp_epwm_db_outmode(struct EPWM_REGS * pwm_ptr, enum BSP_EPWM_DB_OUT mode)
{
    pwm_ptr->DBCTL.bit.OUT_MODE = mode;
}

/*######################################################
 *           trip-zone
 *  ##################################################*/
/*######################################################
 *           PWM chopper
 *  ##################################################*/
/*######################################################
 *           digital compare
 *  ##################################################*/
/*######################################################
 *           global CTRL
 *  ##################################################*/
/*######################################################
 *           Sync. SEL
 *           epwm1-3 default synci select EXTSYNCIN1
 *  ##################################################*/
#define BSP_EPWM_SYNC_REG    (*((volatile unsigned long*) 0x00007940))
inline void bsp_epwm_4_synci_sel(enum BSP_EPWM_SYNCI_SRC src)
{
    EALLOW;
    BSP_EPWM_SYNC_REG &= (~(0x3<<0));
    BSP_EPWM_SYNC_REG |= (src << 0);
    EDIS;
}
inline void bsp_epwm_7_synci_sel(enum BSP_EPWM_SYNCI_SRC src)
{
    EALLOW;
    BSP_EPWM_SYNC_REG &= (~(0x3<<3));
    BSP_EPWM_SYNC_REG |= (src << 3);
    EDIS;
}
inline void bsp_epwm_10_synci_sel(enum BSP_EPWM_SYNCI_SRC src)
{
    EALLOW;
    BSP_EPWM_SYNC_REG &= (~(0x3<<6));
    BSP_EPWM_SYNC_REG |= (src << 6);
    EDIS;
}
inline void bsp_epwm_synco_sel(enum BSP_EPWM_SYNCO_SRC src)
{
    EALLOW;
    BSP_EPWM_SYNC_REG &= (~(((uint32_t)0x3)<<27));
    BSP_EPWM_SYNC_REG |= (((uint32_t)src)<< 27);
    EDIS;
}
/******************************************************************
 * @ interface
 *******************************************************************/
BSP_EPWM_EXEC int      bsp_epwm_init(struct EPWM_REGS * pwm_ptr,uint16_t isr_route);
BSP_EPWM_EXEC int      bsp_epwm_reset(struct EPWM_REGS * pwm_ptr);
BSP_EPWM_EXEC void     bsp_epwm_sys_clk(uint8_t on_off);
BSP_EPWM_EXEC void     bsp_epwm_sys_glbclk(uint8_t on_off);
BSP_EPWM_EXEC void     bsp_epwm_sys_hrclk(uint8_t on_off);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef BSP_EPWM_C_

#endif
#endif
