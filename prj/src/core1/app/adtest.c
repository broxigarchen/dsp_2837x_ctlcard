/*##################################################################
 * MCMASTER UNIVERSITY P.E.R.L. LAB.
 * This file is included in Project of High Power Electronic Controller
 *
 * @FILE    adtest.c
 * @AUTHOR  Brox Chen
 * @DATE    Aug 7, 2018
 * @LICENSE Please see file LICENSE
 * @DESC.
 * AD7327 testing task
##################################################################*/
#define SPITEST_C_
#include "app/adtest.h"
#include "io.h"
#include "timer.h"

/**********************************************
 * @ AD7327 Register Macro
 *********************************************/
/*      AD7327  adressing bit       */
#define AD7327_REG_CTL       (0x0)      //control register
#define AD7327_REG_RNG1      (0x1)      //range 1 register
#define AD7327_REG_RNG2      (0x2)      //range 2 register
#define AD7327_REG_SQ        (0x3)      //sequence register
/*      AD7327  control reg         */
enum AD7327_MD
{
    AD7327_MD_SIG,     //single channel mode
    AD7327_MD_P_DIF,   //4 Pseudo differential mode
    AD7327_MD_F_DIF,   //4 full differential mode
    AD7327_MD_TEMP,    //temperature indicator
};  /*  ad mode  */
enum AD7327_PM
{
    AD7327_PM_NORM,   //power normal
    AD7327_PM_IDLE,   //power idle, auto shutdown circuit except internal reference
    AD7327_PM_SAVE,   //power save, auto shutdown circuit including internal reference
    AD7327_PM_CLOSE,  //power close, close everything
};  /*  ad power mode  */
enum AD7327_RES
{
    AD7327_RES_INT,   //result 2's complement
    AD7327_RES_BIN,   //result binary
};  /*  ad result coding  */
enum AD7327_SQ
{
    AD7327_SQ_IDLE,   //do not use sequence
    AD7327_SQ_SEQ,    //sample according to sequence register
    AD7327_SQ_CONJ,   /*sample consecutive sequence of channels from 0 up to and
                      including the final channel selected by channel address in control register*/
};  /*  ad sequencer  mode  */

#define AD7327_CTL_CHN(x)   (((uint16_t)(x))<<10)   //channel 0-7
#define AD7327_CTL_MD(x)    (((uint16_t)(x))<<8)    //mode 0-3
#define AD7327_CTL_PM(x)    (((uint16_t)(x))<<6)    //power mag 0-3
#define AD7327_CTL_RES(x)   (((uint16_t)(x))<<5)    //result coding
#define AD7327_CTL_REF(x)   (((uint16_t)(x))<<4)    //1:internal reference, 0:external reference
#define AD7327_CTL_SQ(x)    (((uint16_t)(x))<<2)    //sequence 0-3
/*      AD7327  sequence  reg         */
#define AD7327_SQ_VAL(seq)      (((uint16_t)(seq)) << 6)
/*      AD7327  range  reg         */
enum AD7327_RNG
{
    AD7327_RNG_PM_10v,  //+-10v
    AD7327_RNG_PM_5v,   //+-5v
    AD7327_RNG_PM_2_5v, //+- 2.5v
    AD7327_RNG_P_10v,   //0-10v
};  /*  ad ranger  */
#define AD7327_RNG_CHN_1(rng)   (((uint16_t)(rng)) << 12)
#define AD7327_RNG_CHN_2(rng)   (((uint16_t)(rng)) << 10)
#define AD7327_RNG_CHN_3(rng)   (((uint16_t)(rng)) << 8)
#define AD7327_RNG_CHN_4(rng)   (((uint16_t)(rng)) << 6)
#define AD7327_RNG_CHN_5(rng)   (((uint16_t)(rng)) << 12)
#define AD7327_RNG_CHN_6(rng)   (((uint16_t)(rng)) << 10)
#define AD7327_RNG_CHN_7(rng)   (((uint16_t)(rng)) << 8)
#define AD7327_RNG_CHN_8(rng)   (((uint16_t)(rng)) << 6)
/*        AD7327  Program  Utils          */
#define AD7327_GET_DATA(adr, data)     (0x8000 | (((uint16_t)(adr)) << 13) | (data))
#define AD7327_RNG1_VAL         (AD7327_RNG_CHN_1(AD7327_RNG_PM_10v) | \
                                 AD7327_RNG_CHN_2(AD7327_RNG_PM_10v) | \
                                 AD7327_RNG_CHN_3(AD7327_RNG_PM_10v) | \
                                 AD7327_RNG_CHN_4(AD7327_RNG_PM_10v))

#define AD7327_RNG2_VAL         (AD7327_RNG_CHN_5(AD7327_RNG_PM_10v) | \
                                 AD7327_RNG_CHN_6(AD7327_RNG_PM_10v) | \
                                 AD7327_RNG_CHN_7(AD7327_RNG_PM_10v) | \
                                 AD7327_RNG_CHN_8(AD7327_RNG_PM_10v))

/*  single channel mode, result 2's complement, power mgr normal, internal reference  */
#define AD7327_CTL_NORM         (AD7327_CTL_MD(AD7327_MD_SIG) | \
                                 AD7327_CTL_RES(AD7327_RES_INT) | \
                                 AD7327_CTL_PM(AD7327_PM_NORM) | \
                                 AD7327_CTL_REF(1))

/*  differential input mode, result 2's complement, power mgr normal, internal reference  */
#define AD7327_CTL_DIFF         (AD7327_CTL_MD(AD7327_MD_F_DIF) | \
                                 AD7327_CTL_RES(AD7327_RES_INT) | \
                                 AD7327_CTL_PM(AD7327_PM_NORM) | \
                                 AD7327_CTL_REF(1))

/*  temperature indicator mode, result 2's complement, power mgr normal, internal reference  */
#define AD7327_CTL_TEMP         (AD7327_CTL_MD(AD7327_MD_TEMP) | \
                                 AD7327_CTL_RES(AD7327_RES_INT) | \
                                 AD7327_CTL_PM(AD7327_PM_NORM) | \
                                 AD7327_CTL_REF(1))

/*  pick  AD7327_CTL_NORM or AD7327_CTL_DIFF or AD7327_CTL_TEMP depends on your application  */
#define AD7327_RUN_CHN(x)       (AD7327_CTL_NORM | AD7327_CTL_SQ(AD7327_SQ_IDLE) | AD7327_CTL_CHN(x))
#define AD7327_RUN_SQ           (AD7327_CTL_NORM | AD7327_CTL_SQ(AD7327_SQ_SEQ))

/*           AD7327  Program  data            */
#define AD7327_RNG1_INIT                    AD7327_GET_DATA(AD7327_REG_RNG1, AD7327_RNG1_VAL)             //set RNG1 init.
#define AD7327_RNG2_INIT                    AD7327_GET_DATA(AD7327_REG_RNG2, AD7327_RNG2_VAL)             //set RNG2 init.
#define AD7327_SQ(val)                      AD7327_GET_DATA(AD7327_REG_SQ,   AD7327_SQ_VAL(val))          //set sequence register to val
#define AD7327_CONVERT_CHN(chan_id)         AD7327_GET_DATA(AD7327_REG_CTL,  AD7327_RUN_CHN(chan_id))     //convert ad channel-chan_id
#define AD7327_CONVERT_SQ                   AD7327_GET_DATA(AD7327_REG_CTL,  AD7327_RUN_SQ)               //convert ad according to sequence register
#define AD7327_CHK_TEMP                     AD7327_GET_DATA(AD7327_REG_CTL,  AD7327_RUN_CHN(chan_id))     //check temperature
/**********************************************
 * @ buffer declare
 *********************************************/
#define MAX_AD_BUF  20
char ad_sampled[8][MAX_AD_BUF];
char ad_sampled_ptr[8];
/**********************************************
 * @ inner function
 *********************************************/
uint16_t ad7327_WRChar(int port, enum SPI_NODE node,uint16_t dat)
{
    size_t len = sizeof(uint16_t);
    write(port, node, &dat, len);
    while(!listen(port, node));                   //wait for rcv
    read(port, node, &dat, len);
    return dat;
}

void ad7327_init(int port, enum SPI_NODE node)
{
    ad7327_WRChar(port, node, AD7327_RNG1_INIT);
    ad7327_WRChar(port, node, AD7327_RNG2_INIT);
    ad7327_WRChar(port, node, AD7327_SQ(0xff));   //1-8 channel sequence
}

void ad_sample_seq(int port,enum SPI_NODE node)
{
    size_t len;
    uint16_t  dat;
    size_t i;
    len = 1;
    for(i=0;i<8;i++)
    {
        if(i == 0)
        {
            dat = ad7327_WRChar(port, node, AD7327_CONVERT_SQ);
        }
        else
        {
            dat = ad7327_WRChar(port, node, 0);
        }
        ad_sampled[i][ad_sampled_ptr[i]] = dat;
        ad_sampled_ptr[i]++;
        if(ad_sampled_ptr[i] >= MAX_AD_BUF)
        {
            ad_sampled_ptr[i] = 0;
        }
    }
}

static const uint16_t seq_dma_send[8] = {AD7327_CONVERT_SQ, 0,0,0,0,0,0,0};
static uint16_t seq_dma_rcv[8];
void ad_sample_seq_dma(int port,enum SPI_NODE node)
{
    ioctl(port, )
}

void ad7327_spi_init(int port)
{
    ioctl(port, IO_RESET);
    ioctl(port, IO_SPI_MODE, SPI_MASTER);
    ioctl(port, IO_SPI_BUS, SPI_PRIVATE);
    ioctl(port, IO_SPI_POLARITY, SPI_ROFI);
    ioctl(port, IO_SPI_WIDTH, SPI_16_BIT);
}

void adtest_tick(void)
{
    ad_sample_seq(SPI_A, SPI_NODE_0);
}
/**********************************************
 * @ Task Interface
 *********************************************/
int adtest_init(void)
{
    int res;
    /*      spi module init         */
    ad7327_spi_init(SPI_A);
    /*      AD ic init         */
    ad7327_init(SPI_A, SPI_NODE_0);
    /*      AD buffered clear        */
    memset(ad_sampled, 0, sizeof(ad_sampled));
    memset(ad_sampled_ptr, 0, sizeof(ad_sampled_ptr));

    /*      Timer init         */
    res = setTimer(TIMER1, TIM_NROM, 100000, adtest_tick, NULL);   //100ms, check time out
    if(res == -1)
    {
        print("adtest init: tick error!\r\n");
        return -1;
    }
    print("adtest init: tick initialized succeed!\r\n");

    return 1;
}

void adtest_main(void)
{
    while(1);
}

