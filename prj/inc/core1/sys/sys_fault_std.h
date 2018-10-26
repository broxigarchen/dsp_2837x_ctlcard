/*#################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 *
 * @FILE    sys_fault_std.h
 * @AUTHOR  Brox Chen
 * @DATE    June 16, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * This file defines all fault of the system
 * This file might be included multiple times into other files
 ##################################################################*/
/*      MACRO DEF STD   */
#ifndef ERR_ID
#define ERR_ID(x)       (x | 0x8000)
#endif

/*
 *  MACRO: ERR_INFO(ID,MOD,DESC)
 *  explain:
 *      ID:     id of the error
 *      MOD:    belongs to which sub module
 *      DESC:   description
 */
SYS_ERR_INIT(ERR_ID(0), 0,  "System Initialize error")

