/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    bsp_epwm.c
 * @AUTHOR  Brox Chen
 * @DATE    June 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * CPU and perhperial clock init.
##################################################################*/
#define BSP_EPWM_C_
#include "bsp/bsp_epwm.h"
#include "bsp/bsp_clk.h"

#if EPWM1_EN
extern __interrupt void epwm1_isr_handler(void);
#endif
#if EPWM2_EN
extern __interrupt void epwm2_isr_handler(void);
#endif
#if EPWM3_EN
extern __interrupt void epwm3_isr_handler(void);
#endif
#if EPWM4_EN
extern __interrupt void epwm4_isr_handler(void);
#endif
#if EPWM5_EN
extern __interrupt void epwm5_isr_handler(void);
#endif
#if EPWM6_EN
extern __interrupt void epwm6_isr_handler(void);
#endif
#if EPWM7_EN
extern __interrupt void epwm7_isr_handler(void);
#endif
#if EPWM8_EN
extern __interrupt void epwm8_isr_handler(void);
#endif
#if EPWM9_EN
extern __interrupt void epwm9_isr_handler(void);
#endif
#if EPWM10_EN
extern __interrupt void epwm10_isr_handler(void);
#endif
#if EPWM11_EN
extern __interrupt void epwm11_isr_handler(void);
#endif
#if EPWM12_EN
extern __interrupt void epwm12_isr_handler(void);
#endif
/*
 * @epwm clk and interrupt setting
 */
int bsp_epwm_clk_init(struct EPWM_REGS * pmw_ptr,uint16_t isr_route)
{
    EALLOW;
    if(pmw_ptr == &EPwm1Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM1 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
#if EPWM1_EN
        PieVectTable.EPWM1_INT = &epwm1_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm2Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM2 = isr_route;
        IER |= M_INT3;                                   //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
#if EPWM2_EN
        PieVectTable.EPWM2_INT = &epwm2_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm3Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM3 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
#if EPWM3_EN
        PieVectTable.EPWM3_INT = &epwm3_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm4Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM4 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
#if EPWM4_EN
        PieVectTable.EPWM4_INT = &epwm4_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm5Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM5 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx5 = 1;
#if EPWM5_EN
        PieVectTable.EPWM5_INT = &epwm5_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm6Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM6 = isr_route;
        IER |= M_INT3;                                   //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx6 = 1;
#if EPWM6_EN
        PieVectTable.EPWM6_INT = &epwm6_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm7Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM7 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx7 = 1;
#if EPWM7_EN
        PieVectTable.EPWM7_INT = &epwm7_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm8Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM8 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx8 = 1;
#if EPWM8_EN
        PieVectTable.EPWM8_INT = &epwm8_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm9Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM9 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx9 = 1;
#if EPWM9_EN
        PieVectTable.EPWM9_INT = &epwm9_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm10Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM10 = isr_route;
        IER |= M_INT3;                                   //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx10 = 1;
#if EPWM10_EN
        PieVectTable.EPWM10_INT = &epwm10_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm11Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM11 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM11 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx11 = 1;
#if EPWM11_EN
        PieVectTable.EPWM11_INT = &epwm11_isr_handler;
#endif
    }
    else if(pmw_ptr == &EPwm12Regs)
    {
        CpuSysRegs.PCLKCR2.bit.EPWM12 = 1;                 //enable clk
        DevCfgRegs.CPUSEL0.bit.EPWM12 = isr_route;
        IER |= M_INT3;                                    //cpu isr group enable
        PieCtrlRegs.PIEIER3.bit.INTx12 = 1;
#if EPWM12_EN
        PieVectTable.EPWM12_INT = &epwm12_isr_handler;
#endif
    }
    else
    {
        EDIS;
        return -1;
    }
    EDIS;
    return 1;
}


int bsp_epwm_reset(struct EPWM_REGS * pwm_ptr)
{
    /*#########  Time Base module  ###########*/
    /*      counter & synchronization    */
    pwm_ptr->TBCTL.bit.FREE_SOFT = 0;        //stop when emulation event triggered
    pwm_ptr->TBCTL.bit.CTRMODE = 3;          //counter stop
    pwm_ptr->TBCTL.bit.PHSEN = 1;            //allow sync
    pwm_ptr->TBCTL.bit.PRDLD = 0;            //reload from shadow register
    pwm_ptr->TBCTL.bit.PHSDIR = 1;           //count-up after sync
    pwm_ptr->TBCTL.bit.SYNCOSEL = 0;         //sync output from SYNCI/SWFSYNC
    pwm_ptr->TBCTL2.bit.PRDLDSYNC = 1;       //load shadow PRD with sync or counter equ zero
    pwm_ptr->TBCTL2.bit.OSHTSYNC = 1;        //enable one-shot sync to propagate
    pwm_ptr->TBCTL2.bit.OSHTSYNCMODE = 1;    //enable one-shot sync mode
    pwm_ptr->TBPHS.bit.TBPHS = 1;            //sync reload Phase offset into CTR
    pwm_ptr->TBCTR = 0;                      //counter set zero
    /*##########     Comparator    ###########*/
    pwm_ptr->CMPCTL.bit.SHDWAMODE = 0;       //CMPA shadow reg enable
    pwm_ptr->CMPCTL.bit.LOADASYNC = 0;       //shadow-to-active, depends on LOADAMODE
    pwm_ptr->CMPCTL.bit.LOADAMODE = 2;       //shadow-to-active load mode, load on BOTTOM or PEAK
    pwm_ptr->CMPCTL.bit.SHDWBMODE = 0;       //CMPB shadow reg enable
    pwm_ptr->CMPCTL.bit.LOADBSYNC = 0;       //shadow-to-active, depends on LOADAMODE
    pwm_ptr->CMPCTL.bit.LOADBMODE = 2;       //shadow-to-active load mode, load on BOTTOM or PEAK
    pwm_ptr->CMPCTL2.bit.SHDWCMODE = 0;      //CMPC shadow mode enable
    pwm_ptr->CMPCTL2.bit.LOADCSYNC = 0;      //shadow-to-active, depends on LOADAMODE
    pwm_ptr->CMPCTL2.bit.LOADCMODE = 2;      //shadow-to-active load mode, load on BOTTOM or PEAK
    pwm_ptr->CMPCTL2.bit.SHDWDMODE = 0;      //CMPD shadow mode enable
    pwm_ptr->CMPCTL2.bit.LOADDSYNC = 0;      //shadow-to-active, depends on LOADAMODE
    pwm_ptr->CMPCTL2.bit.LOADDMODE = 2;      //shadow-to-active load mode, load on BOTTOM or PEAK
    /*##########     Action Qualifier   ###########*/
    /*          EPWM a, b           */
    pwm_ptr->AQCTL.bit.SHDWAQAMODE = 0;      //AQA shadow reg enable
    pwm_ptr->AQCTL.bit.LDAQASYNC = 0;        //shadow-to-active, depends on LOADQAMODE
    pwm_ptr->AQCTL.bit.LDAQAMODE = 2;        //shadow-to-active load mode, load on BOTTOM or PEAK
    pwm_ptr->AQCTL.bit.SHDWAQBMODE = 0;      //AQB shadow reg enable
    pwm_ptr->AQCTL.bit.LDAQBSYNC = 0;        //shadow-to-active, depends on LOADQBMODE
    pwm_ptr->AQCTL.bit.LDAQBMODE = 2;        //shadow-to-active load mode, load on BOTTOM or PEAK
    pwm_ptr->AQSFRC.bit.RLDCSF = 3;          //action software force reload immediately
    /*          software force event           */
    pwm_ptr->AQSFRC.bit.RLDCSF = 3;          //shadow disable
    pwm_ptr->AQSFRC.bit.ACTSFA = 0;          //no action on pwmA
    pwm_ptr->AQSFRC.bit.ACTSFB = 0;          //no action on pwmB
    pwm_ptr->AQCSFRC.bit.CSFA = 0;           //no continuous action on pwmA
    pwm_ptr->AQCSFRC.bit.CSFB = 0;           //no continuous action on pwmB
    /*##########     Event Trigger   ###########*/
    pwm_ptr->ETCNTINIT.all = 0;              //set initialize value for all event counter
    pwm_ptr->ETCNTINITCTL.all = 0xffff;      //force to initialize all event counter
    pwm_ptr->ETPS.bit.INTPSSEL = 1;          //select TINTPS as isr counter(instead of ETPS)
    pwm_ptr->ETPS.bit.SOCPSSEL = 1;          //select ETSOCP as ad isr counter(instead of ETPS)
    pwm_ptr->ETPS.bit.INTPRD = 1;            //one event trigger isr
    pwm_ptr->ETPS.bit.SOCAPRD = 1;           //one event trigger ad a isr
    pwm_ptr->ETPS.bit.SOCBPRD = 1;           //one event trigger ad b isr
    pwm_ptr->ETINTPS.bit.INTPRD2 = 1;        //one event trigger isr
    pwm_ptr->ETSOCPS.bit.SOCAPRD2 = 1;       //one event trigger ad a isr
    pwm_ptr->ETSOCPS.bit.SOCBPRD2 = 1;       //one event trigger ad b isr
    /*##########     Dead band    ###########*/
    pwm_ptr->DBCTL2.bit.SHDWDBCTLMODE = 0;   //DBCTL R/W immediate affect
    pwm_ptr->DBCTL.bit.HALFCYCLE = 1;        //T_DB = TBCLK * 2
    pwm_ptr->DBCTL.bit.OUTSWAP = 0;          //No swap, PWMA to A, PWMB to B
    pwm_ptr->DBCTL.bit.OUT_MODE = 3;         //Fully enable RED and FED
    pwm_ptr->DBCTL.bit.SHDWDBFEDMODE = 1;    //FED shadow mode
    pwm_ptr->DBCTL.bit.SHDWDBREDMODE = 1;    //RED shadow mode
    pwm_ptr->DBCTL.bit.LOADFEDMODE = 2;      //FED reload condition
    pwm_ptr->DBCTL.bit.LOADREDMODE = 2;      //RED reload condition

    /*##########    Digital Compare  ###########*/
    //bypass
    /*##########    PWM chopper    ###########*/
    //bypass
    /*##########    Trip Zone    ###########*/
    //bypass

    /*##########    Global CTL    ###########*/
    EALLOW;

    EDIS;
    return 1;
}

void bsp_epwm_sys_clk(uint8_t on_off)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = on_off;
    EDIS;
}

void bsp_epwm_sys_glbclk(uint8_t on_off)   //only affects in CPU1
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.GTBCLKSYNC = on_off;
    EDIS;
}

void bsp_epwm_sys_hrclk(uint8_t on_off)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.HRPWM = on_off;
    EDIS;
}


int bsp_epwm_init(struct EPWM_REGS * pwm_ptr,uint16_t isr_route)
{
    int res;
    /*         CLK              */
    res = bsp_epwm_clk_init(pwm_ptr,isr_route);
    if(res == -1)
    {
        PERROR("epwm clk init fail");
        return -1;
    }
    res = bsp_epwm_reset(pwm_ptr);
    if(res == -1)
    {
        PERROR("epwm init fail");
        return -1;
    }
    return 1;
}
