/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    cstruct.h
 * @AUTHOR  Brox Chen
 * @DATE    June 9, 2018
 * @LICENSE Please see file LICENSE
##################################################################*/
#ifndef CSTRUCT_C_
#define CSTRUCT_C_

#include "bsp/cpu_info.h"
#include <stdint.h>
#include <string.h>

#ifdef CSTRUCT_C_
#define CSTRUCT_EXEC
#else
#define CSTRUCT_EXEC extern
#endif

/******************************************************************
 * @ iobuf_ctrl structure
 *******************************************************************/
typedef struct _iobuf_ctrl
{
    size_t   in;
    size_t   out;
    size_t   overflow;
    size_t   max;
}iobuf_ctrl;   /* circle buffer control */


inline void  buf_ctrl_init(iobuf_ctrl* ptr, size_t max_len)
{
    memset(ptr,0,sizeof(iobuf_ctrl));
    ptr->max = max_len;
}

inline void  buf_ctrl_reset(iobuf_ctrl* ptr)
{
    ptr->in = 0;
    ptr->out = 0;
    ptr->overflow = 0;
}

inline unsigned char buf_ctrl_full(iobuf_ctrl *ptr)
{
    if(ptr->overflow > 1)
    {
       return 1;
    }
    else if(ptr->overflow == 1)
    {
        if((ptr->in + 1) == ptr->out)
               return 1;
    }
    else if(ptr->overflow ==  0)
    {
        if((ptr->in + 1) == ptr->max && (ptr->out == 0))
                return 1;
    }

    return 0;
}

inline unsigned char buf_ctrl_wait(iobuf_ctrl *ptr)
{
    if(ptr-> in != ptr->out || ptr->overflow)
    {
        return 1;
    }
    return 0;
}

inline size_t buf_ctrl_in(iobuf_ctrl* ptr)
{
    size_t ret = ptr->in;
    ptr->in++;
    if(ptr->in >= ptr->max)
    {
        ptr->overflow++;
        ptr->in = 0;
    }
    return ret;
}


inline size_t buf_ctrl_out(iobuf_ctrl* ptr)
{
    size_t ret;
    ret =  ptr->out;
    if(ptr-> in != ptr->out)    //not empty
    {
        ptr->out++;
        if(ptr->out >= ptr->max)
        {
            ptr->overflow--;
            ptr->out = 0;
        }
    }
    return ret;
}


#endif
