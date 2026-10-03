#ifndef SERIAL_H
#define SERIAL_H

#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/
#include 	 <sys/time.h>
#include	 <time.h>
#include     <stdbool.h>
#include      <sys/sem.h>
#include <linux/serial.h>
#include <sys/ioctl.h>



#define RTU_FRAME_CRC_LENGTH  2
#define MIN_RET_LEN 				6
#define RTU_FRAME_READ_BITS 2000
#define RTU_FRAME_READ_WORDS 125
#define MAX_RECV_BYTE     1800  //一次读命令的帧最大的字节数

// #define _MODBUS_UPS_			// Ares 20180402 注释掉这个 不用于掉电处理





/**
*@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void
*/

int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300,
B150, B134, B110, B75  };

int name_arr[] = {115200, 57600, 38400,  19200,  9600,  4800,  2400,  1200,  300,
150, 134, 110, 75 };

const int adjust_time_bps_arr[]={1000,4000,10000,11000,25000,40000,70000,140000,600000,0,0,0,0};

void set_speed(int fd, int baud)
{
        int   status;
        struct termios   Opt;
        struct serial_struct Serial;

        tcgetattr(fd, &Opt);
        tcflush(fd, TCIOFLUSH);

        cfsetispeed(&Opt, B38400);
        cfsetospeed(&Opt, B38400);
        tcflush(fd,TCIOFLUSH);
        status = tcsetattr(fd, TCSANOW, &Opt);
        if  (status != 0)
        {
                printf("tcsetattr fd1\r\n");
                return;
        }

        if((ioctl(fd,TIOCGSERIAL,&Serial))<0)
        {
                printf("Fail to get Serial!\n");
                return;
        }
        printf("TIOCGSERIAL: OK.\n");

        Serial.flags = ASYNC_SPD_CUST;
        Serial.custom_divisor=(((Serial.baud_base)/baud));
        printf("baud_base=%d,divisor is %d\n",Serial.baud_base,Serial.custom_divisor);
        if((ioctl(fd,TIOCSSERIAL,&Serial))<0)
        {
                printf("Fail to set Serial\n");
                return;
        }

        printf("TIOCSSERIAL: OK.\n");

        ioctl(fd,TIOCGSERIAL,&Serial);
   printf("\r\nBAUD: success set baud to %d,custom_divisor=%d,baud_base=%d\n",baud
,Serial.custom_divisor,Serial.baud_base);
}



/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄
*@param  databits 类型  int 数据位   取值 为 7 或者8
*@param  stopbits 类型  int 停止位   取值为 1 或者2
*@param  parity  类型  int  效验类型 取值为 0,1,2
*/

int set_Parity(int fd,int databits,int stopbits,int parity)
{
    struct termios options;
    if  ( tcgetattr( fd,&options)  !=  0)
    {
        perror("SetupSerial 1");
        return(-1);
    }
    options.c_cflag &= ~CSIZE;
    switch (databits) /*设置数据位数*/
    {
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr,"Unsupported data size\n");
        return (-1);
    }
    switch (parity)
    {
    case 0:
        options.c_cflag &= ~PARENB;   /* Clear parity enable */
        options.c_iflag &= ~INPCK;     /* Enable parity checking */
        break;
    case 1:
        options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
        options.c_iflag |= INPCK;             /* Disnable parity checking */
        break;
    case 2:
        options.c_cflag |= PARENB;     /* Enable parity */
        options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
        options.c_iflag |= INPCK;       /* Disnable parity checking */
        break;
    default:
        printf("Unsupported parity\n");
        return (-1);
    }
    /* 设置停止位*/
    switch (stopbits)
    {
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported stop bits\n");
        return (-1);
    }
    /* Set input parity option */
    if (parity != 0)
        options.c_iflag |= INPCK;

    tcflush(fd,TCIFLUSH);
    options.c_cc[VTIME] = 0; /* 设置超时15 seconds*/
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */

    options.c_line=0;  //added by mxt
    options.c_iflag=IGNBRK|IGNPAR|IXANY;

    options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
    options.c_oflag  &= ~OPOST;   /*Output*/

    options.c_cflag &= ~CRTSCTS;//disable hardware flow control

    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("SetupSerial 3");
        return (-1);
    }
    return (0);
}


//打开串口函数
int OpenDev(char *Dev)
{
    int	fd = open( Dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd==-1)
        perror("Can't Open Serial Port");
    else
        fcntl(fd, F_SETFL,FNDELAY);
    return fd;
}

unsigned int ev_plc_init(int baud, int data_bits,int stop_bits, int parity)
{
    int fd;
    int parity_bit;
    int timeout15;


    fd = OpenDev("/dev/ttyS0");
    set_speed(fd,baud);//设置波特率

    //设置数据位、停止位、校验位
    if (set_Parity(fd,data_bits,stop_bits,parity) == -1)
    {
        printf("Set Parity Error\n");
        //exit (0);
    }
    //计算校验位
    if(parity==0)
        parity_bit=0;//无校验
    else
        parity_bit=1;//奇、偶校验
    //计算1.5字符时间，单位微秒
    //timeout15=1.5*1000000*( 1 + serial_settings->data_bits + serial_settings->parity + serial_settings->stop_bits)/serial_settings->baud;
    //timeout15=90000000/serial_settings->baud;//2013.7.2, 经测试取得一个1200鍉115200都有效的值，下面的偏移可以为0
    //timeout15=settings->timeout1*1000 + timeout15;	/* timeout1 单位从ms 变成us */
    //settings->timeout2=settings->timeout2*1000+timeout15;	/* timeout2 单位是ms , response_timeout 单位是ms */
    //printf("timeout=%d,timeout1=%d,timeout2=%d \r\n",settings->response_timeout,settings->timeout1,settings->timeout2);
    return fd;

}













#endif // SERIAL_H
