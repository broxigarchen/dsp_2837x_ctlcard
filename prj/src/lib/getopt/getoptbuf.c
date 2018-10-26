/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    getoptbuf.c
 * @AUTHOR  Brox Chen
 * @DATE    July 31, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
##################################################################*/
#define GETOPTBUF_C_
#include <stdint.h>
#include <stddef.h>

#define GETOPTBUF_MAX 20
#define GETOPTBUF_LEN 20
char getoptbuf[GETOPTBUF_MAX + 1][GETOPTBUF_LEN];
char* argv_buf[GETOPTBUF_MAX + 1];
int argc;
char manual_reinit = 0;
#define ARGV_NEXT     {argv_buf[argc] = getoptbuf[argc]; argc++;}

extern int getopt(int argc, char** argv, char* optstr);

size_t getoptstrwrite(char* buf_in,size_t len)
{
   size_t i,curr_len;
   char byte;
   int curr_state = 0;
	//reset getopt
   argc = 0;
   manual_reinit = 1;
	//write buffer
   for(i=0;i<len;i++)
   {
		if(argc >= GETOPTBUF_MAX) break;
		byte = buf_in[i];
        if(byte != ' ')
        {
           if(curr_state == 0)
           {
               curr_state = 1;
               curr_len = 0;
           }
           if(curr_len < GETOPTBUF_LEN)
               getoptbuf[argc][curr_len++] = byte;
           if(byte == '\0')
           {
              ARGV_NEXT;
           }
        }
		else
		{
            if(curr_state == 1)
            {
                curr_state = 0;
				if(curr_len < GETOPTBUF_LEN)
                	getoptbuf[argc][curr_len++] = 0x0;
				else
					getoptbuf[argc][curr_len] = 0x0;
                ARGV_NEXT;
            }
		}
   }
   if(curr_state && argc < GETOPTBUF_MAX)  //if buf_in does not has '\0'
   {
		 ARGV_NEXT;    
   }
   argv_buf[argc++] = NULL;
   return i;
}

char* getoptdebug(int index)
{
    if(index < argc)
    {
        return getoptbuf[index];
    }
    return NULL;
}

int getoptstream(char* optstr)
{
	return getopt(argc, argv_buf, optstr);
}


