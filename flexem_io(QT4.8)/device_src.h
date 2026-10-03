#ifndef DEVICE_SRC_H
#define DEVICE_SRC_H






//#define x86   //用做调试
#ifdef x86

void Beep(int len,int times)
{

}
int Misc_RunCmd(unsigned int code, void *prm)
{

}
int DevCmd_SetBright(int val)
{

}

#else


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include	<sys/time.h>
#include	<time.h>
#include	<stdbool.h>
#include	<sys/sem.h>
#include	<linux/serial.h>
#include	<sys/ioctl.h>
#include    <unistd.h>
#define CTL_CODE(function, method) (                 \
    (4<< 16) | ((function) << 2) | (method) \
)


#define IOCTL_FE_MISC_GetBootSelect            CTL_CODE(1,0)
#define IOCTL_FE_MISC_GetBrightness            CTL_CODE(2,0)
#define IOCTL_FE_MISC_SetBrightness            CTL_CODE(3,0)
#define IOCTL_FE_MISC_Beep                 CTL_CODE(4,0)

#define IOCTL_FE_MISC_TurnOnBacklight       CTL_CODE(6,0)
#define IOCTL_FE_MISC_TurnOffBacklight      CTL_CODE(7,0)
#define IOCTL_FE_MISC_Set_Com0_232       CTL_CODE(8,0)
#define IOCTL_FE_MISC_Set_Com0_485      CTL_CODE(9,0)
#define IOCTL_FE_MISC_Set_Com0_422      CTL_CODE(10,0)
#define IOCTL_FE_MISC_Set_Com1_232       CTL_CODE(11,0)
#define IOCTL_FE_MISC_Set_Com1_485      CTL_CODE(12,0)
#define IOCTL_FE_MISC_Set_Com1_422      CTL_CODE(13,0)
#define IOCTL_FE_MISC_CpuLed      CTL_CODE(14,0)     // gpa2

#define IOCTL_FE_MISC_GetUniqueId      CTL_CODE(16,0)

#define IOCTL_FE_MISC_GetBacklight      CTL_CODE(17,0)
#define IOCTL_FE_MISC_GetUsbLink      CTL_CODE(18,0)

#define IOCTL_FE_MISC_GetSdCardStatus      CTL_CODE(19,0)
#define IOCTL_FE_MISC_GetEthernetStatus      CTL_CODE(20,0)
#define IOCTL_FE_MISC_GetUsbDiskStatus      CTL_CODE(21,0)
#define IOCTL_FE_MISC_SetBrightness            CTL_CODE(3,0)
#define IOCTL_FE_MISC_SaveBrightness            CTL_CODE(5,0) // 2014-11-25

#define FEMISC "/dev/femisc"//
const char *FE_DEVICE_CTL = "/dev/femisc";
//蜂鸣器
typedef struct
{
   int times;
   int beep_delay;
}BEEPCTRL;

int FE_IOCTL(const char*IOCTL_FILE,int Data,void *Flags)
{
    int fd;
    if( (fd=open(IOCTL_FILE,O_RDWR)) == -1)
    {
        printf("FE_IOCTL failed %s",IOCTL_FILE);
        return -1;
    }
    ioctl(fd, Data, Flags);
    close(fd);
    return 0;
}


void Beep(int len,int times)
{
    int fd;
    BEEPCTRL beepctrl;
    if(times)//确保不为0
    {
        beepctrl.times=times;
        beepctrl.beep_delay=len;
        FE_IOCTL(FE_DEVICE_CTL,IOCTL_FE_MISC_Beep,&beepctrl);
    }
}

//背光
int Misc_RunCmd(unsigned int code, void *prm)
{
    int ret = 0;
    int fd = open("/dev/femisc", O_RDWR);
    if(fd > 0)
        {
            ioctl(fd, code, prm);
        close(fd);		//150205:
        ret = 1;
        }
    return ret;
}

int setcom_ctl()
{
    int ret = Misc_RunCmd(IOCTL_FE_MISC_Set_Com0_485,NULL);

    return ret;	//130829:

}
int DevCmd_SetBright(int val)
{
    int ret = Misc_RunCmd(IOCTL_FE_MISC_SetBrightness, &val);
    Misc_RunCmd(IOCTL_FE_MISC_SaveBrightness, NULL);
    return ret;	//130829:
}
int DevCmd_GetBright(int *val)
{
    int ret = Misc_RunCmd(IOCTL_FE_MISC_GetBrightness, val);

    return ret;	//130829:
}

int trun_backlight(int val)
{
    int ret = Misc_RunCmd(IOCTL_FE_MISC_TurnOffBacklight, NULL);
    sleep(3);
    Misc_RunCmd(IOCTL_FE_MISC_TurnOnBacklight, NULL);
    return ret;	//130829:
}





#endif // DEVICE_SRC_H

#endif // DEVICE_SRC_H
