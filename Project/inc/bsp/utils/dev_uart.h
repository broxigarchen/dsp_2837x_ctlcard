/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    dev_uart.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef DEV_UART_H_
#define DEV_UART_H_

#include "bsp/utils/dev_inc.h"

#ifdef DEV_UART_C_
#define DEV_UART_EXEC
#else
#define DEV_UART_EXEC extern
#endif

/******************************************************************
 * @ Marco
 *******************************************************************/

/******************************************************************
 * @ public
 *******************************************************************/


/******************************************************************
 * @ interface
 *******************************************************************/
DEV_UART_EXEC void    dev_uart_init(void);
DEV_UART_EXEC size_t  uart_write(int dev_id, char* buf_in,  size_t len);
DEV_UART_EXEC size_t  uart_read(int dev_id , char* buf_out, size_t max_len);
DEV_UART_EXEC int     uart_listen(int dev_id);
DEV_UART_EXEC void    uart_flush(int dev_id);
/******************************************************************
 * @ Private
 *******************************************************************/
#ifdef DEV_UART_C_

void uart_hook_485send_enter(int);
void uart_hook_485send_quit(int);
#endif
#endif
