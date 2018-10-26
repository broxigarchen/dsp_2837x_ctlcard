/*#################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 *
 * @FILE    erron.h
 * @AUTHOR  Brox Chen
 * @DATE    May 28, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * This file defines all settings of the project
 ##################################################################*/
#ifndef ERRON_H_
#define ERRON_H_

#include "config.h"

#ifdef    SYS_STDIO         //if SYS_STDIO defined
#define   PRINT(...)         print(__VA_ARGS__)
#else
#define   PRINT(...)
#endif

#ifdef    DEBUG
#define   PDEBUG(str,...)         PRINT(str,__VA_ARGS__);PRINT("\r\n")
#else
#define   PDEBUG
#endif

#define   PERROR(str)    PRINT("file:%s,line:%d,%s\r\n"__FILE__,__LINE__,str)


#endif
