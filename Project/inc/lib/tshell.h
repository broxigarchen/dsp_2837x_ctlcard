/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    mbash.h
 * @AUTHOR  Brox Chen
 * @DATE    August 7, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef TSHELL_H_
#define TSHELL_H_

#include "bsp/cpu_info.h"
#include <stdint.h>
#include <string.h>

#ifdef  TSHELL_C_
#define TSHELL_EXEC
#else
#define TSHELL_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/
enum TSH_STATUS
{
    TSH_IDLE,
    TSH_RUN,
};

typedef struct
{
    int    id;
    char*  name;
}tsh_file_t;          //mbash instance

enum TSH_SIG          //mbash standard signal
{
    TSH_SIG_TERM,     //terminate, ctrl+C
    TSH_SIG_IGN,      //ignore
    TSH_SIG_CORE,     //dump core
    TSH_SIG_STOP,     //stop
    TSH_SIG_CONT,     //continue
};
/******************************************************************
 * @ interface
 *******************************************************************/
TSHELL_EXEC  int     tshInit(void);
TSHELL_EXEC  void    tshMain(void);
TSHELL_EXEC  int     tshCreateInstance(tsh_file_t* instance, void(*pid)(enum TSH_SIG));
TSHELL_EXEC  int     tshInterrupt(void);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef MBASH_C_

struct
{
    tsh_file_t*             curr;        //current instance
    enum TSH_STATUS         status;      //tsh  status


}tshell;


#endif
#endif
