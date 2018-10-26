/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    comtest.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef COMTEST_H_
#define COMTEST_H_

#include "app/app_inc.h"

#ifdef COMTEST_C_
#define COMTEST_EXEC
#else
#define COMTEST_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/

enum COMTEST_ST
{
    COMTEST_IDLE,
    COMTEST_READY,
    COMTEST_SNDING,
    COMTEST_RCVING,
    COMTEST_RCVEND,
    COMTEST_TIMOUT,
    COMTEST_SINEND,
    COMTEST_END,
    COMTEST_DELAY,
};
enum COMTEST_RES
{
    COMTEST_RES_SUCC,   //SUCCESS
    COMTEST_RES_VERERR, //verification error
    COMTEST_RES_TIMOUT, //timout
    COMTEST_RES_SNDERR, //send error
};
typedef struct
{
    enum COMTEST_ST status;
    //total testing var
    size_t      total_circle;
    size_t      test_circle;
    size_t      succ_circle;
    size_t      sample_len;
    //single testing var
    size_t      send_len;
    uint8_t     frame_start;
    size_t      rcv_len;
    char        snd_buf[256];
    char        rcv_buf[256];
    size_t      timout;
    enum COMTEST_RES   last_res;
    size_t      last_roundtrip_us;
    //final result
    float       percent;
    size_t      roundtrip_us;
}comtest_ctrl_t;

typedef struct
{
    char      rcv_buf[50];
    size_t    rcv_len;
}comtest_bash_t;
/******************************************************************
 * @ interface
 *******************************************************************/
COMTEST_EXEC int   comtest_init(void);
COMTEST_EXEC void  comtest_main(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef COMTEST_C_

static char  name[20];
comtest_bash_t comtest_bash;
comtest_ctrl_t comtest_ctrl;

const char* comtest_err_test[]={
"success",
"verify error",
"time out",
"send error",
};

#endif
#endif
