/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    sys_std.c
 * @AUTHOR  Brox Chen
 * @DATE    May 17, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 *
##################################################################*/
#define SYS_STD_C_
#include "sys/sys_std.h"
#include "io.h"

/* ##################################################
 *   @std io
 #################################################*/
char std_out[STDOUT_BUF_SIZE];  //standard output buffer
char std_in[STDIN_BUF_SIZE];
int  stdio_dev = STDIO;         //device id

void flush(void)
{
    ioflush(stdio_dev);
}

int scan(const char* fmt, ...)
{
    size_t rcv_len = 0;
    char byte;
    int ret = 0;
    va_list arg;
    va_start(arg,fmt);
    while(1)
    {
        if(listen(stdio_dev))
        {
            byte = getbyte(stdio_dev);
            if(rcv_len >= STDIN_BUF_SIZE)
            {
                return 0;
            }
            std_in[rcv_len++] = byte;
            if(byte == '\r' || byte == '\n' || byte == '\0')  //end frame
            {
                std_in[rcv_len-1] = '\0';
                ret = vsscanf(std_in,fmt,arg);
                break;
            }
        }
    }
    while(listen(stdio_dev)) byte = getbyte(stdio_dev);    //dump
    va_end(arg);
    return ret;
}

int vfprint(int id,const char* fmt,va_list arg)
{
    int res;
    size_t len;
    size_t sent_len = 0;
    size_t curr_len;
    res = vsnprintf(std_out,STDOUT_BUF_SIZE,fmt,arg);
    va_end(arg);
    if(res)
    {
        len = res;
        while(sent_len < len)
        {
            curr_len = write(id, &std_out[sent_len], len - sent_len);
            if(curr_len == 0)
            {
                break;
            }
            sent_len += curr_len;
        }
    }
    else
    {
        return -1;
    }
    return sent_len;
}

int fprint(int id,const char* fmt, ...)
{
    va_list arg;
    enum DEV_TYPE dev_type = dev_get_type(id);
    if(dev_type !=  DEV_BUF_IO)
    {
        return -1;
    }
    va_start(arg,fmt);
    return vfprint(id,fmt,arg);
}

int print(const char* fmt, ...)
{
    va_list arg;
    va_start(arg,fmt);
    return vfprint(stdio_dev,fmt,arg);
}

char getbyte(int id)
{
    char data = 0;
    enum DEV_TYPE dev_type = dev_get_type(id);
    if(dev_type ==  DEV_BUF_IO)
    {
        read(id, &data, (size_t)1);
    }
    return data;
}

