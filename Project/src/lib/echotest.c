///*##################################################################
// * MCMASTER UNIVERSITY P.E.R.L. LAB.
// * This file is included in Project of High Power Electronic Controller
// *
// * @FILE    echotest.c
// * @AUTHOR  Brox Chen
// * @DATE    July 31, 2018
// * @LICENSE Please see file LICENSE
// * @DESC.
//s
//##################################################################*/
//#define COMTEST_C_
//#include "lib/echotest.h"
//
///**********************************************
// * @ inner function
// *********************************************/
//inline void comtest_setstatus(enum COMTEST_ST next_st)
//{
//    comtest_ctrl.status = next_st;
//}
//
//int comtest_chkframe(char* sample,char* received, size_t len)
//{
//    char crc_byte;
//    /*  last byte CRC8 */
//    crc_byte = crc8((unsigned char*)received,len-1);
//    if(sample[len-1] == crc_byte)
//    {
//        return 1;
//    }
//    return -1;
//}
//
//void comtest_tick(void)
//{
//    switch(comtest_ctrl.status)
//    {
//    default:
//        break;
//    case COMTEST_DELAY:
//        comtest_setstatus(COMTEST_READY);
//        break;
//    case COMTEST_RCVING:
//    {
//        comtest_ctrl.timout++;
//        if(comtest_ctrl.timout >= 5)
//        {
//            comtest_setstatus(COMTEST_TIMOUT);
//        }
//    }
//        break;
//    }
//}
//
//#define  COMTEST_START (1<<0)
//#define  COMTEST_NCON  (1<<1)
//#define  COMTEST_QUIT  (1<<2)
//#define  COMTEST_DATL  (1<<3)
//void comtest_readcmd(char byte)
//{
//    int para;
//    int flag = 0;
//    int dat_len;
//    int loop_num;
//    if(comtest_bash.rcv_len >= 50)
//    {
//        print("Commest: command too long!\r\n");
//        comtest_bash.rcv_len = 0;
//        return;
//    }
//    comtest_bash.rcv_buf[comtest_bash.rcv_len++] = byte;
//    if(byte == '\r' || byte == '\0')
//    {
//        if(0 != strncmp(comtest_bash.rcv_buf,name,strlen(name)))
//        {
//            comtest_bash.rcv_len = 0;
//            return;
//        }
//        getoptstrwrite(comtest_bash.rcv_buf,comtest_bash.rcv_len);
//        comtest_bash.rcv_len = 0;
//        while(1)
//        {
//            para = getoptstream("-sqhn:l:");
//            if(para == -1) break;
//            switch(para)
//            {
//            case 's':
//                flag |= COMTEST_START;
//                break;
//            case 'n':
//                if(optarg != NULL)
//                {
//                    sscanf(optarg,"%d",&loop_num);
//                    flag |= COMTEST_NCON;
//                }
//                break;
//            case 'l':
//                if(optarg != NULL)
//                {
//                    sscanf(optarg,"%d",&dat_len);
//                    flag |= COMTEST_DATL;
//                }
//                break;
//            case 'h':
//                print("Program:     comtest\r\n"
//                      "Description: uart com port test program\r\n"
//                        "-s           start one testing\r\n"
//                        "-n [%%d]      set loop number to [%%d] and start testing\r\n"
//                        "-h           show this manual\r\n"
//                        "-q           quit continue mode\r\n"
//                        );
//                break;
//            case 'q':
//                flag |= COMTEST_QUIT;
//                break;
//            case '?':
//                print("comtest: unknown command\r\n");
//                break;
//            }
//        }
//        if(flag & COMTEST_START)
//        {
//            comtest_ctrl.total_circle = 1;
//            comtest_ctrl.test_circle = 0;
//            comtest_ctrl.succ_circle = 0;
//            comtest_ctrl.roundtrip_us = 0;
//            comtest_setstatus(COMTEST_READY);
//        }
//        if(flag & COMTEST_NCON)
//        {
//            if(loop_num != 0)
//             {
//                 comtest_ctrl.total_circle = loop_num;
//                 comtest_ctrl.test_circle = 0;
//                 comtest_ctrl.succ_circle = 0;
//                 comtest_ctrl.roundtrip_us = 0;
//                 comtest_setstatus(COMTEST_READY);
//             }
//            else
//            {
//                print("comtest: invalid loop number\r\n");
//            }
//        }
//        if(flag & COMTEST_QUIT)
//        {
//            comtest_setstatus(COMTEST_END);
//        }
//        if(flag & COMTEST_DATL)
//        {
//            if(dat_len >= 3 && dat_len <=254)
//            {
//                 print("comtest: set data length %d\r\n", dat_len);
//                 comtest_ctrl.sample_len = dat_len;
//            }
//            else
//            {
//                print("comtest: invalid data length, should between 3 to 254\r\n");
//            }
//        }
//    }
//}
//
//void comtest_rcvdata(char byte)
//{
//    switch(comtest_ctrl.status)
//    {
//    default:
//        break;
//    case COMTEST_RCVING:
//    {
//        comtest_ctrl.rcv_buf[comtest_ctrl.rcv_len++] = byte;
//        comtest_ctrl.timout = 0;
//        if(comtest_ctrl.rcv_len >= comtest_ctrl.sample_len)
//        {
//            stoptimer(TIMER0);
//            comtest_setstatus(COMTEST_RCVEND);
//        }
//        break;
//    }
//    }
//}
//
//#define FRAME_HEAD     0xff
//void comtest_echo(char byte)    //Slave echo function
//{
//    if(byte == FRAME_HEAD)
//    {
//        comtest_ctrl.rcv_len = 0;
//        comtest_ctrl.frame_start = 1;
//        comtest_ctrl.sample_len = 0;
//        comtest_ctrl.rcv_buf[comtest_ctrl.rcv_len++] = FRAME_HEAD;
//        return;
//    }
//    if(comtest_ctrl.frame_start)
//    {
//        if(comtest_ctrl.rcv_len == 1)
//        {
//            if(byte < 3)      //length smaller than three, dump
//            {
//                comtest_ctrl.frame_start = 0;
//                return;
//            }
//            comtest_ctrl.sample_len = byte;
//            comtest_ctrl.rcv_buf[comtest_ctrl.rcv_len++] = byte;
//            return;
//        }
//        if(comtest_ctrl.sample_len)
//        {
//            comtest_ctrl.rcv_buf[comtest_ctrl.rcv_len++] = byte;
//            if(comtest_ctrl.rcv_len >= comtest_ctrl.sample_len)
//            {
//                write(UART_C,comtest_ctrl.rcv_buf,comtest_ctrl.rcv_len);
//                comtest_ctrl.frame_start = 0;
//            }
//        }
//    }
//}
//
//void comtest_fsm(void)
//{
//    switch(comtest_ctrl.status)
//    {
//    case COMTEST_IDLE:
//        break;
//    case COMTEST_READY:
//    {
//        size_t len;
//        char* addr = (char*)0x0000D000;
//        char data;
//        comtest_ctrl.snd_buf[0] = FRAME_HEAD;
//        comtest_ctrl.snd_buf[1] = comtest_ctrl.sample_len;
//        for(len=2;len<comtest_ctrl.sample_len;len++)
//        {
//            data = ((*addr++) & 0xff);
//            if(data == FRAME_HEAD) data = 0;
//            comtest_ctrl.snd_buf[len] = data;
//        }
//        comtest_ctrl.snd_buf[comtest_ctrl.sample_len-1] = crc8((unsigned char*)comtest_ctrl.snd_buf,comtest_ctrl.sample_len-1);
//        comtest_setstatus(COMTEST_SNDING);
//        //single test init
//        comtest_ctrl.send_len = 0;
//        comtest_ctrl.rcv_len = 0;
//        comtest_ctrl.timout=0;
//    }
//        break;
//    case COMTEST_SNDING:
//    {
//        size_t res;
//        //sending
//        resettimer(TIMER0);
//        runtimer(TIMER0);
//        while(comtest_ctrl.send_len < comtest_ctrl.sample_len)
//        {
//           res = write(UART_C,&comtest_ctrl.snd_buf[comtest_ctrl.send_len],comtest_ctrl.sample_len - comtest_ctrl.send_len);
//           if(res == 0)
//           {
//               print("comtest: single send fail!");
//               comtest_ctrl.last_res = COMTEST_RES_SNDERR;
//               comtest_setstatus(COMTEST_SINEND);
//               break;
//           }
//           comtest_ctrl.send_len += res;
//        }
//        comtest_setstatus(COMTEST_RCVING);
//    }
//        break;
//    case COMTEST_RCVING:
//        //wait for data
//        break;
//    case COMTEST_RCVEND:
//    {
//        comtest_ctrl.last_roundtrip_us = readtimer(TIMER0);
//        if((1 == comtest_chkframe(comtest_ctrl.snd_buf,comtest_ctrl.rcv_buf,comtest_ctrl.rcv_len)) &&
//           comtest_ctrl.rcv_len == comtest_ctrl.sample_len)
//        {
//               comtest_ctrl.last_res = COMTEST_RES_SUCC;
//               comtest_ctrl.succ_circle++;
//               comtest_ctrl.roundtrip_us += comtest_ctrl.last_roundtrip_us;
//        }
//        else
//        {
//            comtest_ctrl.last_res = COMTEST_RES_VERERR;
//        }
//        comtest_setstatus(COMTEST_SINEND);
//    }
//        break;
//    case COMTEST_TIMOUT:
//    {
//        stoptimer(TIMER0);
//        comtest_ctrl.last_roundtrip_us = readtimer(TIMER0);
//        comtest_ctrl.timout=0;
//        comtest_ctrl.last_res = COMTEST_RES_TIMOUT;
//        comtest_setstatus(COMTEST_SINEND);
//    }
//        break;
//    case COMTEST_SINEND:
//    {
//        comtest_ctrl.test_circle++;
//        print("testing %ld, %s ,latency %ld us\r\n", comtest_ctrl.test_circle,
//              comtest_err_test[comtest_ctrl.last_res], comtest_ctrl.last_roundtrip_us);
//        if(comtest_ctrl.test_circle >= comtest_ctrl.total_circle)
//        {
//            comtest_setstatus(COMTEST_END);
//        }
//        else
//        {
//            comtest_setstatus(COMTEST_DELAY);
//        }
//    }
//        break;
//    case COMTEST_DELAY:
//        //wait for timeout
//        break;
//    case COMTEST_END:
//    {
//        comtest_ctrl.percent = 100.0 * comtest_ctrl.succ_circle/comtest_ctrl.test_circle;
//        if(comtest_ctrl.succ_circle)
//        {
//            comtest_ctrl.roundtrip_us = ((float)(comtest_ctrl.roundtrip_us)) / comtest_ctrl.succ_circle;
//        }
//        else
//        {
//            comtest_ctrl.roundtrip_us = 0;
//        }
//        print("\r\n");
//        print("Total sent:%ld, Total received:%ld (%3.1f%%)\r\n",comtest_ctrl.test_circle, comtest_ctrl.succ_circle, comtest_ctrl.percent);
//        print("Average round-trip latency: %ld (us)\r\n",comtest_ctrl.roundtrip_us);
//        comtest_setstatus(COMTEST_IDLE);
//    }
//        break;
//    default:
//        break;
//    }
//}
///**********************************************
// * @ Task Interface
// *********************************************/
//int comtest_init(void)
//{
//    int res;
//
//    strcpy(name,"comtest");
//
//    memset(&comtest_ctrl,0,sizeof(comtest_ctrl));
//    memset(&comtest_bash,0,sizeof(comtest_bash));
//    comtest_ctrl.sample_len = 20;                  // default sample data 20 bytes length
//
//#if BOARD_TYPE == F28379D_EVALBOARD
//    res = settimer(TIMER1,TIM_NROM, 100000,comtest_tick,NULL);   //100ms, check time out
//    if(res == -1)
//    {
//        print("comtest init: tick error!\r\n");
//    }
//    print("comtest init: tick initialized succeed!\r\n");
//
//    res = settimer(TIMER0, TIM_CTR, 0, NULL, NULL);  //counter mode, used to measure round-trip communication latency
//    if(res == -1)
//    {
//        print("comtest init: measure timer init error!\r\n");
//    }
//    print("comtest init: measure timer initialized succeed!\r\n");
//#endif
//
//    return 1;
//}
//
//void comtest_main(void)
//{
//    char byte;
//    while(listen(UART_C))
//    {
//        byte = getbyte(UART_C);
//#if BOARD_TYPE == F28379D_EVALBOARD
//        comtest_rcvdata(byte);
//#elif BOARD_TYPE == F28379D_LAUNCHPAD
//        comtest_echo(byte);
//#endif
//    }
//
//#if BOARD_TYPE == F28379D_EVALBOARD
//    while(listen(UART_B))
//    {
//        byte = getbyte(UART_B);
//        comtest_readcmd(byte);
//    }
//    comtest_fsm();     //finite state machine
//#endif
//
//}
//
