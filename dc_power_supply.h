#ifndef ANS_DC_POWER_SUPPLY
#define ANS_DC_POWER_SUPPLY

#include "comm.h"

typedef enum
{
    CMD_ANS_NULL_DATA_ADDR                = 0,

    CMD_ANS_READ_VOLT_AMP_DATA_ADDR       =   0x10,
    CMD_ANS_READ_AMP_DATA_ADDR            =   0x11,
    CMD_ANS_SET_VOLT_DATA_ADDR            =   0x30,
    CMD_ANS_SET_AMP_DATA_ADDR             =   0x31,
    CMD_ANS_RUN_STOP_DATA_ADDR            =   0x01,


}ANS_COMMAND_ADDR;


typedef enum
{
    CMD_TX500W_NULL_DATA_ADDR                = 0,

    CMD_TX500W_READ_VOLT_AMP_DATA_ADDR       =   1002,
    CMD_TX500W_READ_AMP_DATA_ADDR            =   1003,
    CMD_TX500W_SET_VOLT_DATA_ADDR            =   1000,
    CMD_TX500W_SET_AMP_DATA_ADDR             =   1001,
    CMD_TX500W_RUN_STOP_DATA_ADDR            =   1006,
}TX500_COMMAND_ID;

#pragma pack(push)
#pragma pack(1)

typedef struct
{
    unsigned char RS485_Addr;
    unsigned char cmd;
    unsigned char DataAddr_H;
    unsigned char DataAddr_L;
    unsigned char Data_H;
    unsigned char Data_L;
    unsigned short crc;
}MODBUS_RS485_CMD_SET_ANSWER_WORD_1;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[2];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_1;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[4];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_2;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[12];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_6;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[16];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_8;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[24];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_12;
typedef struct
{
        unsigned char RS485_Addr;

        unsigned char cmd;

        unsigned char DataAddr_H;
        unsigned char DataAddr_L;
        unsigned char DataLen_H;
        unsigned char DataLen_L;
        unsigned short crc;

}MODBUS_RS485_CMD_READ;
typedef struct
{

        unsigned char RS485_Addr;

        unsigned char cmd;

        unsigned char DataAddr_H;
        unsigned char DataAddr_L;
        unsigned char Data_H;
        unsigned char Data_L;
        unsigned short crc;

}MODBUS_RS485_CMD_SET;

typedef struct
{//FE 04 18
    //00 00 00 00 00
    //00 00 00 00 00
    //00 00 00 00 00
    //00 00 00 00 00
    //00 00 00 00 81 60
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Volt[12];
    unsigned char Amp[12];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_JY_DAMAC0606;


#pragma pack(pop)

//typedef QVector<MODBUS_RS485_CMD_SET > CMODBUS_RS485_SET_CMD_ANS_Array;


typedef CArray<MODBUS_RS485_CMD_SET ,MODBUS_RS485_CMD_SET> CMODBUS_RS485_SET_CMD_ANS_Array; 

extern CMODBUS_RS485_SET_CMD_ANS_Array G_MODBUS_RS485_SET_CMD_ANS_Array[8];
//extern CAns_DC_PowerSupply     G_Ans_DC_PowerSupply[8];
//extern void SetJustSendCmdType(int COMM,MODBUS_RS485_CMD_READ cmd,DEVICE_ID device);
//extern void SetJustSendCmdType(int COMM,MODBUS_RS485_CMD_SET cmd,DEVICE_ID device);
//extern void SetJustSendCmdType(int COMM,DEVICE_ID device,FX3GA_CMD_READ cmd);

#endif // ANS_DC_POWER_SUPPLY

