/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    sys_schd.c
 * @AUTHOR  Brox Chen
 * @DATE    May 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 *
##################################################################*/
#define SYS_SCHD_C_
#include "sys/sys_schd.h"
#include "io.h"
#include "time.h"

char data_buf[100];
size_t data_len;

err_t sys_init(int err)
{

    return 0;
}

void debug_echo(void)
{
    char byte;
    while(listen(UART_C) == 1)
    {
        byte = getbyte(UART_C);
        data_buf[data_len++] = byte;
        if(byte == '\r')
        {
            print(UART_C,"echo S:");
            write(UART_C,data_buf,data_len);
            data_len = 0;
        }
    }
}

void debug_forward(void)
{
    char byte;
    size_t len = 1;
    while(listen(UART_B) == 1)
    {
        byte = getbyte(UART_B);
        data_buf[data_len++] = byte;
        if(byte == '\r')
        {
            print(UART_B,"echo M:");
            write(UART_B,data_buf,data_len);
            write(UART_C,data_buf,data_len);
            data_len = 0;
        }
    }
    while(listen(UART_C) == 1)
    {
        byte = getbyte(UART_C);
        write(UART_B,&byte,len);
    }
}




void sys_schd(void)
{
    data_len = 0;
    while(1)
    {
        debug_test();
    }

}
