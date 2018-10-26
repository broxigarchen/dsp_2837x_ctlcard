/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    hbridge.c
 * @AUTHOR  Brox Chen
 * @DATE    Sept 2, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * H bridge application
##################################################################*/
#define HBRIDGE_C
#include "app/hbridge.h"
#include "io.h"
#include "epwm.h"
#include "timer.h"
#include "sys/sys_std.h"


#define  PWNLEG1   EPWM1
#define  PWNLEG2   EPWM2
/**********************************************
 * @ spwm_1phase function
 *********************************************/
typedef struct
{
    struct
    {
        float phs;      //[0-2pi)
        float freq;
        float ma;
    }sina;
    struct
    {
        size_t fcar2fsin;    // freq_carr / freq_sin
        int    dir;          //direction of carrier, 1 up counting, -1 down counting
        float  radhalfcicle; //radius of half circle of carrier
    }carr;
    size_t  resolsteps;      //resolution steps, how many steps tried in finding intersections
    float   resolution;      //smallest step of carrier
    float   radstep;         //corresponding radius of each resolution
    size_t  counter;         //counter
    float   lastcp_legA;     //pu value of cross point of legA, range from [0,1]
    float   lastcp_legB;     //pu value of cross point of legB, range from [0,1]
}spwm_1phase_t;

spwm_1phase_t hbridge_demo;

void spwm_1phase_init(spwm_1phase_t* ptr, float ac_freq, float ac_phs,float ac_ma, size_t ratio, size_t resol)
{
    memset(ptr, 0 ,sizeof(spwm_1phase_t));
    ptr->sina.phs = ac_phs;
    ptr->sina.freq = ac_freq;
    ptr->sina.ma = ac_ma;
    ptr->carr.fcar2fsin = ratio;
    ptr->carr.dir = 1;
    ptr->carr.radhalfcicle = PI/ratio;
    ptr->counter = 0;
    ptr->resolsteps = resol;
    ptr->resolution = 2./resol;    //from [-1,1]
    ptr->radstep = ptr->carr.radhalfcicle/resol;
}

void spwm_1phase_set_ma(spwm_1phase_t* ptr, float ac_ma)
{
    ptr->sina.ma = ac_ma;
}

void spwm_1phase_set_phs(spwm_1phase_t* ptr, float ac_phs)
{
    ptr->sina.phs = ac_phs;
}

void spwm_1phase_calc(spwm_1phase_t* ptr)   //called when carrier reach peak or bottom, prepare the next cross point
{
    size_t i;
    float sina,sinb,carr;
    float rad;
    unsigned int geta = 0,getb = 0;
    //find the cross point of leg A, carr from [-1,1], sin from [-1,1]
    rad = ptr->counter * ptr->carr.radhalfcicle;
    if(ptr->carr.dir == 1)     //count up, start from -1
        carr = -1;
    else                       //count down, start from 1
        carr = 1;
    for(i=0; (i< ptr->resolsteps) && (~(geta && getb)); i++)
    {
        sina = ptr->sina.ma * sin(rad + ptr->sina.phs);
        sinb = -sina;
        if((ptr->carr.dir * (sina - carr)) <= 0 && geta==0)      //cross
        {
            geta = 1;
            ptr->lastcp_legA = (carr+1) / 2;                  //move carr to [0,2] and calc pu value
        }
        if((ptr->carr.dir * (sinb - carr)) <= 0 && getb==0)      //cross
        {
            getb = 1;
            ptr->lastcp_legB = (carr+1) / 2;                  //move carr to [0,2] and calc pu value
        }
        rad += ptr->radstep;
        carr += ptr->carr.dir * ptr->resolution;
    }
    //next
    ptr->counter++;
    ptr->carr.dir = -1 * ptr->carr.dir;                //invert carrier direction
    if(ptr->counter >= (ptr->carr.fcar2fsin << 1))
    {
        ptr->counter = 0;
    }
}

float spwm_1phase_getcp_lega(spwm_1phase_t* ptr)
{
    return ptr->lastcp_legA;
}

float spwm_1phase_getcp_legb(spwm_1phase_t* ptr)
{
    return ptr->lastcp_legB;
}
/**********************************************
 * @ inner function
 *********************************************/
void hbridge_epwm1_callback(void)
{
    //load cmp value
    setEpwmCmp(PWNLEG1, CMP_A, spwm_1phase_getcp_lega(&hbridge_demo));
    setEpwmCmp(PWNLEG2, CMP_A, spwm_1phase_getcp_legb(&hbridge_demo));
    //set output action, unipolar
    if(getEpwmCarrierDir(PWNLEG1) == UP_COUNT)
    {
        setEPwmAction(PWNLEG1, PWMA, CROSS_A_U, SET_L);
        setEPwmAction(PWNLEG2, PWMA, CROSS_A_U, SET_L);
    }
    else
    {
        setEPwmAction(PWNLEG1, PWMA, CROSS_A_D, SET_H);
        setEPwmAction(PWNLEG2, PWMA, CROSS_A_D, SET_H);
    }
    //calc next cmp value
    spwm_1phase_calc(&hbridge_demo);
}

/**********************************************
 * @ Task Interface
 *********************************************/
int hbridge_init(void)
{
    int res;
    size_t step;
    /*      Init hardware      */
    /*      stop all EPWM      */
    setAllEpwmStop();
    /*   set epwm  1  */
    /*
     * 1. set carrier
     * 2. initialize output to zero
     * 3. set event triggered when carrier reach peak or bottom
     * 4. set dead time
     * 5. set output mode to complementary
     */
    step = setEpwmCarrier(PWNLEG1,TRIANGLE, 60*15, 0, 500, 1000); //triangle carrier, fs 60*15, phase 0, steps from 500 - 1000
    if(step == 0)
    {
       PDEBUG("hbridge: pwm1 carrier set failure");
       return -1;
    }
    setEpwmOutput(PWNLEG1, PWMA, SET_L);
    setEpwmOutput(PWNLEG1, PWMB, SET_L);
    setEpwmEvent(PWNLEG1, BTM_PK, hbridge_epwm1_callback);
    res = setEpwmDeadband(PWNLEG1, 4);  //4 us
    if(res == -1)
    {
        PDEBUG("hbridge: pwm1 dead time set failure");
        return -1;
    }
    setEpwmOutputMode(PWNLEG1,A_NOTA);
    /*   set epwm  2  */
    /*
     * 1. set carrier
     * 2. initialize output to zero
     * 3. as use same carrier, we can use epwm1's event to set epwm2
     * 4. set dead time
     * 5. set output mode to complementary
     */
    step = setEpwmCarrier(PWNLEG2,TRIANGLE, 60*15, 0, 500, 1000); //triangle carrier, fs 60*15, phase 0, steps from 500 - 1000
    if(step == 0)
    {
       PDEBUG("hbridge: pwm2 carrier set failure");
       return -1;
    }
    setEpwmOutput(PWNLEG2, PWMA, SET_L);
    setEpwmOutput(PWNLEG2, PWMB, SET_L);
    setEpwmDeadband(PWNLEG2, 4);  //4 us
    setEpwmOutputMode(PWNLEG2,A_NOTA);
    /*      sync set    */
    setEpwmChainSyncIn(PWM1CHN, EXTSYNIN1);  //set PWM1-3 sync from ext input 1
    setEpwmChainSyncIn(PWM4CHN, EXTSYNIN1);  //set PWM4-6 sync from ext input 1
    setEpwmChainSyncIn(PWM7CHN, EXTSYNIN1);  //set PWM7-9 sync from ext input 1
    setEpwmChainSyncIn(PWM10CHN, EXTSYNIN1);  //set PWM10-12 sync from ext input 1
    setEpwmChainSyncInExt(EXTSYNIN1, 100);   //set GPIO100 as the sync ext input 1
    setEpwmSyncBypass(PWNLEG1);
    setEpwmSyncBypass(PWNLEG2);
    /*      Init software SPWM       */
    // f0=60, ac_phs=0, ac_ma=0.9, fs=15*f0, resolution = 50
    spwm_1phase_init(&hbridge_demo, 60, 0, 0.9, 15, 50);
    spwm_1phase_calc(&hbridge_demo);    //prepared the first cross point
    /*      start all EPWM    */
    setAllEpwmStart();

    return 1;
}

void hbridge_main(void)
{
    while(1);    //do nothing
}
