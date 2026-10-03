
#ifndef _COMM_H_
#define _COMM_H_
#include "typedefs.h"

#define INVALID_TEMP	999.9f
#define COMM_MSG	WM_USER+1000
#define COMM_MSG_1	WM_USER+1001

 

#define SWITCH_STATUS_ADDR	'A'

#define NEW_192_INTERFACE_PCB	0
#define OLD_192_INTERFACE_PCB	1

extern unsigned char STM32_ID[][12];
extern unsigned char STM32_CopyRight[8];
extern int			G_ProgramStarted;
typedef enum
{
	CMD_NULL					= 0,
	
	CMD_OMRON_READ				= 0x03,
	CMD_OMRON_WRITE				= 0x06,


	CMD_READ_STATUS				= 0x10,
	//CMD_READ_OPEN_FLASH			= 0x11,
	//CMD_READ_CLOSE_FLASH		= 0x12,
	CMD_START_OPEN_FLASH_TEST	= 0x13,
	CMD_START_CLOSE_FLASH_TEST	= 0x14,
	CMD_END_FLASH_TEST			= 0x15,
	CMD_SET_FLASH_TEST_SPEED	= 0x16,

	CMD_STOP_OUT				= '0',
	CMD_FULL_OUT				= '1',
	CMD_NORMAL_PID_OUT			= '2',
	CMD_STEP_PID_OUT			= '3',
	CMD_READ_TEMP				= '4',
	CMD_READ_PID_T				= '5',
	CMD_SET_PID_T				= '6',
	CMD_SET_ADJUST_LOW			= '7',
	CMD_SET_ADJUST_HIGH			= '8',	
	CMD_READ_ADJUST_LOW			= '9',
	CMD_READ_ADJUST_HIGH		= 0x3A, // ':',

//	CMD_READ_FLASH		= 0x3B,	//';',
	CMD_SET_SWITCH_NORMAL_MODE	= 0x3C,
	CMD_AUTO_TEST				= 0x3D,
	CMD_SET_HIGH_VOLT			= 0x3E,
	CMD_SET_RELAY				= 0x3F,
	CMD_SET_BEEP				= 0x40,
	
	CMD_READ_AUTHOR				= 'A',
	CMD_READ_BUILDDATE			= 'B',
	CMD_READ_PRODUCEDATE		= 'C',
	CMD_SAVE_ADJUST_BOARD		= 'D',
	CMD_READ_ADJUST_BOARD_LOW	= 'E',
	CMD_READ_ADJUST_BOARD_HIGH	= 'F',
	CMD_READ_SN					= 'G',
	CMD_SET_DISPLAY_TYPE		= 'H',
	CMD_SET_LOGO_FONT_NR		=	'I',
	CMD_SET_LOGO_FONT_0			=	'J',
	CMD_SET_LOGO_FONT_1			=	'K',
	CMD_SET_LOGO_FONT_2			=	'L',
	CMD_SET_LOGO_FONT_3			=	'M',
	CMD_SET_LOGO_FONT_4			=	'N',
	CMD_SET_LOGO_FONT_5			=	'O',
	CMD_SET_LOGO_FONT_6			=	'P',
	CMD_SET_LOGO_FONT_7			=	'Q',
	CMD_SET_LOGO_FONT_8			=	'R',
	CMD_SET_LOGO_FONT_9			=	'S',
	
	CMD_SET_ADJUST_ALL			= 0x7C,
	CMD_READ_ADJUST_ALL			= 0x7D,
	CMD_CPLD_RESET				= 0x7E,

	CMD_WRITE_STEP_PID_COUNT	= 0x80,
	CMD_READ_STEP_PID_COUNT		= 0x81,
#ifndef _XIN_QI_NENG_
	CMD_AIR_CTL					= 0x82,  //老板子是0x7F,新的是0x82
#endif
	CMD_STEP_PID_COOL			= 0x83,

//	CMD_AIR_VALVE				= 0x84,
	CMD_FAN						= 0x85,

	CMD_PID_COOL				= 0x86,
	CMD_AT						= 0x87,
	CMD_COOL_AT					= 0x88,
	CMD_PERCENT_HEAT_WITH_AIR	= 0x89,
	CMD_AT_END					= 0x8C,
	CMD_WRITE_USEFUL_SENSORS	= 0x8D,
//	CMD_READ_USEFUL_SENSORS		= 0x8E,	

	CMD_READ_TICK_COUNT			= 0x8F,

	CMD_READ_DISC_STATUS		= 0x90,
//	CMD_DISC_START			= 0x91,

	CMD_SET_NTC24_TH2512_RELAY  = 0x92,

	CMD_SET_PCB_TYPE_192_INTERFACE=0xE0,
	CMD_READ_BOARD_TYPE			= 0xE1,
	CMD_WRITE_BOARD_TYPE		= 0xE2,
	CMD_WRITE_TM7705_OSC		= 0xE3,

	CMD_PROGRAM_START			= 0xF1,
	CMD_PROGRAM_DATA			= 0xF2,
	CMD_PROGRAM_END				= 0xF3,

	CMD_STM32_UNIQUE_ID			=	0xF8,

	CMD_SET_ADDR				= 0xFF,

}COMMAND_ID;

typedef struct
{
	COMMAND_ID id;
	TCHAR	   text[2][30];
}CMD_ID_STRING;


//#include "stdAfx.h"
#pragma pack(push)
#pragma pack(1)	

typedef struct
{
	short  Temp[4];	//Temp
	short  Sv;
	unsigned TestMode						:8;
	unsigned OUTPUT_STATUS_DOOR_ON_BIT		:1;
	unsigned OUTPUT_STATUS_AIR_ON_BIT		:1;
	unsigned OUTPUT_STATUS_AIR_VALVE_ON_BIT	:1;
	unsigned OUTPUT_STATUS_BEEP_BIT			:1;
	unsigned OUTPUT_STATUS_HIGH_VOLT_BIT	:1;
	unsigned AutoTune_recs_id				:3;
	unsigned	HeatPercent:					8;
	unsigned	TempDecimalPlaces:				1;
	unsigned	PID_Step:						7;
	unsigned short  iError;
}MODBUS_RS485_TEMP_ANSWER_PARA;


typedef struct
{
	short  Temp[2];	//Temp
	short  Sv[2];
	char TestMode[2];
	char OutputStatus[2];
	signed char HeatPercent[2];
	unsigned short		iError;
}MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT;

typedef struct
{ 
	short Pid_P;
	short Pid_I;
	short Pid_D;
	short Pid_T;
	
	short Pid_P_Cool;
	short Pid_I_Cool;
	short Pid_D_Cool;
	short Pid_T_Cool;
	
//	int Pid_P_AC;
//	int Pid_I_AC;
//	int Pid_D_AC;
//	int Pid_T_AC;
}PID_PARA;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
}MODBUS_RS485_HEADER;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned short page;
	unsigned short para_len;
}MODBUS_RS485_ARM_HEADER;

typedef struct
{
	MODBUS_RS485_HEADER head;

	char para[64*sizeof(short)+2]; //+2 为了不和 MODBUS_RS485_CMD_PROGRAM 冲突
	unsigned short crc;
}MODBUS_RS485_NTC_AD_VAL_ANSWER;

typedef struct
{
	short  Temp[20];	//Temp
	short  Sv;
	char TestMode;
	char OutputStatus;
	signed char HeatPercent;
	unsigned char PcbType;//CoolPercent;
	unsigned short  iError;

}MODBUS_RS485_20TEMP_5SSR_ANSWER_PARA;

typedef struct
{
	short	Sv;
	char	UsedPT100[5];
	char    Tel[4];	//7,8,9,10 == '8598'
	char    dummy2[5];

}MODBUS_RS485_20TEMP_5SSR_CMD_PARA;

typedef struct
{
	MODBUS_RS485_HEADER head;
	MODBUS_RS485_20TEMP_5SSR_ANSWER_PARA para;
	unsigned short crc;
}MODBUS_RS485_20TEMP_5SSR_ANSWER;


typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[128/8];
	unsigned short crc;
}MODBUS_RS485_CMD_128;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[32];
	unsigned short crc;
}MODBUS_RS485_CMD_32PT100;

typedef struct
{
	unsigned short  Temp[32];	//Temp
	unsigned short  Sv;
	unsigned	TestMode:8;
	unsigned	OUTPUT_STATUS_DOOR_ON_BIT:1;
	unsigned	OUTPUT_STATUS_AIR_ON_BIT:1;
	unsigned	OUTPUT_STATUS_AIR_VALVE_ON_BIT:	1;
	unsigned	OUTPUT_STATUS_BEEP_BIT:1;
	unsigned	OUTPUT_STATUS_HIGH_VOLT_BIT:1;
	unsigned	AutoTune_recs_id:3;
	unsigned	HeatPercent:8;
	unsigned	TempDecimalPlaces:8;
	
}MODBUS_RS485_32TEMP_ANSWER_PARA;

typedef struct
{
	MODBUS_RS485_HEADER head;
	MODBUS_RS485_32TEMP_ANSWER_PARA para;
	unsigned short crc;
}MODBUS_RS485_32TEMP_ANSWER;

typedef struct
{
	MODBUS_RS485_HEADER head;
	signed char para[32];
	unsigned short crc;
}MODBUS_RS485_32TEMP_CMD_ADJUST;

typedef struct
{

   short    Temp_Sv;
   short    Temp_Sv_2;
   char     Temp_Control_Mode;
   char     Temp_Control_Mode_2;
   unsigned TestMode:	2;
   unsigned TestMode_2:	2;
   unsigned WorkMode:	4;
   unsigned WorkMode_2:	4;
   unsigned IsClosing:	1;
   unsigned IsClosing_2:1;
   unsigned Beep:		1;


}LoadLifeTestBench_WORK_PARA;


typedef struct
{
	MODBUS_RS485_HEADER head;
	short   Pv[8];
	short	Sv[8];
	signed char	HeatPercent[8];
	unsigned char TestMode[8];
	unsigned short Error;
	unsigned char SSR_Status;
	unsigned short crc;
}MODBUS_RS485_8TEMP_8SSR_ANSWER;


//14:59:15.93收←◆:010204000020D9
//14:59:21.93收←◆:01020860001085
//14:59:24.93收←◆:010310320001B9
//14:59:27.93收←◆:01030E7A00056F
//14:59:30.109收←◆:010310320001B9

//读 X
//3A 30 31 30 32 30 34 30 30 30 30 32 30 44 39 0D 0A
//3A 30 31 30 32 30 34 30 30 30 30 32 30 30 30 44 39 0D 0A 

//读D(0x1032)的值
//3A 30 31 30 33 31 30 33 32 30 30 30 31 42 39 0D 0A
//Answer: 3A 30 31 30 33 30 32 30 30 30 30 46 41 0D 0A

//读C(0x0E7A)的值,5个计数
//3A 30 31 30 33 30 45 37 41 30 30 30 35 36 46 0D 0A 
//Answer 3A, 30 31, 30 33, 30 41 30 46 42 36 30 30 30 30 30 37 38 38 30 30 30 30 30 32 39 44 46 46 0D 0A


//读M(0x0860)的值
//       3A 30 31 30 32 30 38 36 30 30 30 31 30 38 35 0D 0A
//Answer 3A 30 31 30 32 30 32 30 30 30 30 46 42 0D 0A

// 自动标志 0x0866,赋值 0xFF00,手动
// 复位标志 0x0867,赋值 0xFF00
// 启动标志 0x0867,赋值 0x0000
// 停止标志 0x0865,赋值 0xFF00

//'强制地址 0x0867
//:01,05,0867,FF00,8C


typedef enum
{
	X00	=	0,
	X01,
	X02,
	X03,
	X04,
	X05,
	X06,
	X07,

	X10,
	X11,
	X12,
	X13,
	X14,
	X15,
	X16,
	X17,

	X20,
	X21,
	X22,
	X23,
	X24,
	X25,
	X26,
	X27,
	

}X_INPUT;

#define	X_Has_Switch							X00
#define	X_Low_Level_Input_Cylinder				X06

#define X_Low_Level_Test_Cylinder				X10
#define X_Low_Level_Test_Switch					X11

#define X_Low_Level_Output_Cylinder				X12		
#define X_Low_Level_Fail_Cylinder				X14


#define	X_High_Level_Input_Cylinder				X16

#define X_High_Level_Test_Cylinder				X21
#define X_High_Level_Test_Switch				X22

#define X_High_Level_Output_Cylinder			X23
#define X_High_Level_Selected_Pass_Cylinder		X25
#define X_High_Level_Selected_Fail_Cylinder		X26
#define X_High_Volt_Pass						X27

typedef enum
{
	Y00	=	0,
	Y01,
	Y02,
	Y03,
	Y04,
	Y05,
	Y06,
	Y07,

	Y10,
	Y11,
	Y12,
	Y13,
	Y14,
	Y15,
	Y16,
	Y17,

	Y20,
	Y21,
	Y22,
	Y23,
	Y24,
	Y25,
	Y26,
	Y27,
	

}Y_OUTPUT;

//:0105 08C8 FF002B 低温进料电磁阀动作
#define	Y_Low_Level_Input_Cylinder				Y00

//:0105 08CB FF0028 低温检测电磁阀动作
#define Y_Low_Level_Test_Cylinder				Y03

//#define Y_Low_Level_Test_Light					Y04

//:0105 08CC FF0027 低温出料电磁阀动作
#define Y_Low_Level_Output_Cylinder				Y04	

//:0105 08CE FF0025 低温接料电磁阀动作
#define Y_Low_Level_Fail_Cylinder				Y06

//3A 30 31 30 35 30 38 43 46 46 46 30 30 32 34 0D 0A
//:0105 08CF FF0024
#define	Y_High_Level_Input_Cylinder				Y07

//:0105 08D0 FF0023
#define Y_High_Level_Test_Cylinder				Y10

//#define Y_High_Level_Test_Light					Y11

#define Y_High_Level_Output_Cylinder			Y11

#define Y_High_Level_Selected_Cylinder			Y12

#define Y_High_Volt_Pass						Y14

#define Y_Red_Light								Y14
#define Y_Green_Light							Y15
#define Y_Yellow_Light							Y16

#define DELTA_READ_POINT						0x01
#define DELTA_READ_POINT_X						0x02
#define DELTA_READ_POINT_Y						0x02
#define DELTA_READ_M							0x02
#define DELTA_READ_D							0x03
#define DELTA_READ_C							0x03

#define DELTA_SET_POINT							0x05
#define DELTA_SET_POINT_OFF						0x0000
#define DELTA_SET_POINT_ON						0xFF00
#define DELTA_SET_MEMERY						0x06

#define DELTA_X_ADDR							0x0400
#define DELTA_Y_READ_ADDR						0x0500
#define DELTA_Y_ADDR							0x08C8
#define DELTA_M_ADDR							0x0800
#define DELTA_D_ADDR							0x1000
#define DELTA_C_ADDR							0x0E00


typedef struct
{
	unsigned char STX;

// S addr = 0x0000 - - 0x03FF
// X addr = 0x0400 - - 0x04FF
// Y addr = 0x0500 - - 0x05FF
// T addr = 0x0600 - - 0x06FF
// M addr = 0x0700 - - 0xB9FF
// C addr = 0x0E00 - - 0x0EFF
// D addr = 0x1000 - - 0xA70F
	unsigned char Addr[2];
	
	//====
	// 01 读节点状态 S,Y,M，T，C(不可读输入节点状态)
	// 02 读节点状态 S,X，Y,M，T，C(可读输入节点状态)
	// 03 读寄存器的内容值 T，C，D
	// 05 强制单节点状态On.Off	S,Y,M,T,C
	// 06 预设单独寄存器的值 T,C,D
	// 15(0x0FH) 强制多个节点状态On.Off  S,Y,M,T,C
	// 16(0x10H) 预设多个寄存器的值 T,C,D
	// 17（0x11H）报告从站地址
	// 23 (0x17H) PLC LINK 在一个轮询时间内同时执行读写功能
	unsigned char Cmd[2];


}DELTA_PLC_CMD_HEAD;


typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Addr[4];
	

	unsigned char Data_Len[4];

	unsigned char LCR[2];
	
	unsigned char Tail[2];
}DELTA_PLC_READ_CMD;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Addr[4];
	

	unsigned char Data[4];

	unsigned char LCR[2];
	
	unsigned char Tail[2];
}DELTA_PLC_SET_CMD;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];

	unsigned char Data[8];

	unsigned char LCR[2];

	unsigned char Tail[2];

}DELTA_PLC_READ_X_ANSWER;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];

	unsigned char Data[4];

	unsigned char LCR[2];

	unsigned char Tail[2];

}DELTA_PLC_READ_Y_ANSWER;


typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];

	unsigned char Data[4];
	

	unsigned char LCR[2];
	
	unsigned char Tail[2];
}DELTA_PLC_READ_D_ANSWER;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];

	unsigned char Data[4];
	

	unsigned char LCR[2];
	
	unsigned char Tail[2];
}DELTA_PLC_READ_M_ANSWER;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];


	unsigned char Data[20];

	unsigned char LCR[2];

	unsigned char Tail[2];

}DELTA_PLC_READ_C_ANSWER;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[16];
	unsigned short crc;
}MODBUS_RS485_CMD_ARM_128;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[129];
	unsigned short crc;
}MODBUS_RS485_CMD_PROGRAM;


typedef struct
{
	char 		PcbType_192Interface;
	char		UsefulTemps;
	float		AdjustAll;
	float  		AdjustLow[4];
	float  		AdjustHigh[4];
//	signed int	AdjustAll;
//	signed int  AdjustLow[4];
//	signed int  AdjustHigh[4];
//	signed int 	AlarmHigh;
	float 		Sv;
	PID_PARA	PID;			//16 chars

	char	   BoardType; 	
	char	   FanPercent;
	char   	   Pid_Exceed;  
	char       RS485_id;	
	unsigned char NoOsc;	
	unsigned char dummy2;
	unsigned int Error;
	unsigned short    crc;		
}stEEPROM;

typedef struct
{
	MODBUS_RS485_HEADER head;
	MODBUS_RS485_TEMP_ANSWER_PARA	Temp_Answer;
	stEEPROM						EEPROM_Answer;
	unsigned char para[128 - sizeof(MODBUS_RS485_TEMP_ANSWER_PARA) - sizeof(stEEPROM)];
	unsigned short crc;
}MODBUS_RS485_CMD_PARA;

typedef struct
{
	MODBUS_RS485_ARM_HEADER head;
	unsigned char  para[1024];
	unsigned short crc;
}MODBUS_RS485_ARM_CMD_PROGRAM;

typedef struct
{
	MODBUS_RS485_HEADER head;

	unsigned char para[192/8];
	unsigned short crc;
}MODBUS_RS485_CMD_192;

typedef struct
{
	MODBUS_RS485_HEADER head;

	unsigned char para[(300+7)/8];
	unsigned short crc;
}MODBUS_RS485_CMD_300;
typedef struct
{
	MODBUS_RS485_HEADER head;

	unsigned char para[(300+7)/8];
	unsigned short crc;
}MODBUS_RS485_ANSWER_300;


typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[128/8 * 2];
	unsigned short crc;
}MODBUS_RS485_STATUS_FLASH_ANSWER_128;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[192/8 * 2];
	unsigned short crc;
}MODBUS_RS485_STATUS_FLASH_ANSWER_192;



typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[64];
	unsigned short crc;
}MODBUS_RS485_TICK_COUNT_ANSWER;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[20];
	unsigned short crc;
}MODBUS_RS485_DISC_ANSWER;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[24];
	unsigned short crc;
}MODBUS_RS485_DISC_24_ANSWER;


typedef struct
{
	unsigned char RS485Addr;
	unsigned char cmd;
	unsigned short para_addr;
	unsigned short para;
}DELTA_FREQUNCE_CMD_HEX;

typedef struct
{
	unsigned char Head;
	unsigned char RS485Addr[2];
	unsigned char cmd[2];
	unsigned char para_addr[4];
	unsigned char para[4];
	unsigned char check[2];
	unsigned char end[2];
}DELTA_FREQUNCE_CMD_ASC;

typedef struct
{
	unsigned char Head;
	unsigned char RS485Addr[2];
	unsigned char cmd[2];
	unsigned char para_addr[4];
	unsigned char para[4];
	unsigned char check[2];
	unsigned char end[2];
}DELTA_FREQUNCE_ANSWER_ASC;




typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para[4];
	unsigned short crc;
}MODBUS_RS485_CMD_OMRON;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
	unsigned char para[2];
	unsigned short crc;
}MODBUS_RS485_ANSWER_OMRON_READ;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para[4];
	unsigned short crc;
}MODBUS_RS485_ANSWER_OMRON_WRITE;




typedef CArray<MODBUS_RS485_CMD_OMRON ,MODBUS_RS485_CMD_OMRON> CCommCmdArray_OMRON;

#pragma pack(pop)

 
typedef enum
{
	COMM_PROTOCOL_MODBUS_128,
	COMM_PROTOCOL_MODBUS_192,
	COMM_PROTOCOL_MODBUS_300,
	COMM_PROTOCOL_MODBUS_PROGRAM,
	COMM_PROTOCOL_DELTA_FREQUENCE_ASC,
	COMM_PROTOCOL_ASC,
	COMM_PROTOCOL_OMRON,
}COMM_PROTOCOL_TYPE;

#define COMM_MSG				WM_USER+1000
#define START_CHAR				0x02
#define END_CHAR				0x03
#define RS485_PARA_LEN			(16)
#define SWITCH_STATUS_DATA_LEN	(32)
//
#define SWITCH_FLASH_DATA_LEN	(32+32)
typedef struct
{
	char start;
	char des_id;
	char BoxNr;
	char command;
}ASCII_RS485_HEADER;

typedef struct 
{
	ASCII_RS485_HEADER header;
	char chk[2];
	char end;
}ASCII_RS485_READ_COMMAND;

typedef struct 
{
	ASCII_RS485_HEADER header;
	char val[RS485_PARA_LEN];
	char chk[2];
	char end;
}ASCII_RS485_COMMAND;

typedef struct 
{
	ASCII_RS485_HEADER header;
	char val[RS485_PARA_LEN+SWITCH_STATUS_DATA_LEN];
	char chk[2];
	char end;
}ASCII_RS485_COMMAND_READ_TEMP;

typedef struct 
{
	ASCII_RS485_HEADER header;
	char val[SWITCH_FLASH_DATA_LEN];
	char chk[2];
	char end;
}ASCII_RS485_COMMAND_READ_FLASH;


#define SWITCH_STATUS_ADDR	'A'


typedef struct
{
	__time64_t	time;
	MODBUS_RS485_8TEMP_8SSR_ANSWER Answer_8TEMP_8SSR;
}TEMP_RECORD_8TEMP_8SSR;

typedef CArray<TEMP_RECORD_8TEMP_8SSR ,TEMP_RECORD_8TEMP_8SSR> CTempArray_8TEMP_8SSR;


typedef CArray<DELTA_PLC_SET_CMD,DELTA_PLC_SET_CMD>CDelta_PLC_SetCmdArray;
typedef CArray<MODBUS_RS485_CMD_PROGRAM,MODBUS_RS485_CMD_PROGRAM>CCommCmdProgram;
typedef CArray<MODBUS_RS485_ARM_CMD_PROGRAM,MODBUS_RS485_ARM_CMD_PROGRAM>CCommCmdProgram_ARM;
typedef CArray<MODBUS_RS485_CMD_128 ,MODBUS_RS485_CMD_128> CCommCmdArray_128;
typedef CArray<MODBUS_RS485_CMD_32PT100 ,MODBUS_RS485_CMD_32PT100> CCommCmdArray_32PT100;
typedef CArray<MODBUS_RS485_CMD_192 ,MODBUS_RS485_CMD_192> CCommCmdArray_192;
typedef CArray<MODBUS_RS485_CMD_300 ,MODBUS_RS485_CMD_300> CCommCmdArray_300;
typedef CArray<ASCII_RS485_COMMAND ,ASCII_RS485_COMMAND> CCommCmdArray_ASC;
typedef CArray<DELTA_FREQUNCE_CMD_ASC ,DELTA_FREQUNCE_CMD_ASC> CCommCmdArray_DetlaFrequnce_ASC;
typedef CArray<MODBUS_RS485_20TEMP_5SSR_ANSWER ,MODBUS_RS485_20TEMP_5SSR_ANSWER> C20Temp5SsrAnswerArray;


typedef struct
{
	HWND	m_hWnd;
	void	*comm;
	UINT	MessageID;
	CWinThread *ReadThread;

}COMM_THREAD_PARA;

typedef struct
{
	COMMAND_ID id;
	TCHAR		text[100];
}COMMAND_TEXT;

typedef struct
{
	int ID;
	TCHAR str[100];
}COMMAND_STRING;

extern void GetUsefulCommPort(CStringArray *CommStrArray);
extern	COMM_THREAD_PARA G_RS485_CommPara;
extern bool G_bCMD_192_READ_STATUS;
extern bool    G_IsJustCmdAscii;
extern  CCommCmdProgram					G_SendmCmdProgram;
extern 	CCommCmdArray_OMRON				m_SendCmdArray_OMRON;
extern  CCommCmdArray_32PT100			m_CmdArray_32PT100[MAX_HEAT_BOX];
extern 	CCommCmdArray_128				m_CmdArray_128[MAX_HEAT_BOX];
extern 	CCommCmdArray_192				m_CmdArray_192[MAX_HEAT_BOX];
extern  CCommCmdArray_300				m_CmdArray_300[MAX_HEAT_BOX];

extern 	CCommCmdArray_DetlaFrequnce_ASC m_SendCmdDeltaFrequnce;
extern	COMM_PROTOCOL_TYPE				G_CommProtocolType;
extern	int								G_iCmdAscii[MAX_HEAT_BOX];

extern unsigned char CalCheckSum(char *buf,int len);

extern unsigned char Buf2Char(unsigned char * buf);
bool IsValidTemp(float temp);
void WriteLogFile(char *msg, int BoxNr, bool NoTimeLimit = true);
void WriteLogFile(TCHAR *msg, int BoxNr, bool NoTimeLimit = true);
void WriteLogFile(CString msg, int BoxNr, bool NoTimeLimit = true);
unsigned short PcShort2OmronShort(unsigned short v);
unsigned short OmronShort2PcShort(unsigned short v);
LRESULT DealOnComm(WPARAM wParam,LPARAM lParam,CWnd * ParentWnd);
LRESULT DealOnComm_Delta_PLC(WPARAM wParam,LPARAM lParam,CWnd * ParentWnd);
unsigned int CRC16_Ex ( unsigned char *arr_buff, unsigned int len);


//void CRC16(unsigned char *source, int  n,  unsigned char ReturnData[2]);
//bool CheckCRC16(unsigned char *source, int  n, unsigned char source_crc[2]);
/////////////////////////////////////////////////////////////////////////////
class CComm
{
private:
	
	OVERLAPPED ov;

	void AddCommCmd( int equip, COMMAND_ID  cmdId, unsigned char* para,int BoxNr);
	
public:
	void SetCmdString_ASCII(ASCII_RS485_COMMAND *cmd,int equip, int cmdId, short para[4]);
	
	int	 m_OpenCount;
	bool bReadComm;
	HANDLE m_hComm;
	//NORMAL_CONFIG_PARA	G_NormalConfigPara[BoxNr];

	unsigned char OSCCAL;
	CComm();
	int OpenComm(CString commName,COMM_THREAD_PARA *para,DCB dcb);
	int OpenComm(CString commName,COMM_THREAD_PARA *para);
	int CComm::OpenComm_Delta_PLC(CString commName,COMM_THREAD_PARA *para);
	int CloseComm(void);
	int ReadComm(char *buf,int len); 
	int WriteComm(char *buf,int len);
	bool IsOpened(void);
	bool	bXModem;
    virtual ~CComm();
	CTime	m_BeepStartTime[5];

	MODBUS_RS485_CMD_32PT100 m_LastCmd_32PT100[MAX_HEAT_BOX];
	MODBUS_RS485_CMD_128 m_LastCmd_128[MAX_HEAT_BOX];
	MODBUS_RS485_CMD_192 m_LastCmd_192[MAX_HEAT_BOX];
	MODBUS_RS485_CMD_300 m_LastCmd_300[MAX_HEAT_BOX];
	int				repeat_counts[MAX_HEAT_BOX];

	 bool CheckCRC16(unsigned char *source, int  n, unsigned short source_crc);
	static unsigned short CRC16(unsigned char *source, int  n );//, unsigned char ReturnData[2]);
	static unsigned short CRC16_MODBUS(unsigned char *source, int  n ); //, unsigned char ReturnData[2])
	 unsigned short XMODEM_CRC(char *ptr, int count);
	int SetBaudrate(HANDLE hComm,DWORD BaudRate,BYTE ByteSize,BYTE StopBits ,BYTE Parity);
	 static unsigned short CalCheckSum_OIL(unsigned char *buf,unsigned char len);
	 MODBUS_RS485_CMD_128 *  SendReadTemp(int BoxNr);
	 MODBUS_RS485_CMD_32PT100 *  SendReadTemp_32PT100(int BoxNr);

	 MODBUS_RS485_CMD_128 *  CComm::SendReadTemp(int BoxNr,char *para);
	 MODBUS_RS485_CMD_300 *  CComm::SendReadTemp_300(int BoxNr);
	 static bool IsOmronMeter(BOX_TYPE BoxType);
	int SendAirControlCmd(bool AirOn,bool MeterAirOn,int BoxNr,CString * msg);

	 void SendDirectHeatCmd(float temp,int BoxNr);
	 void SendStopHeatCmd(float temp,int BoxNr);
	 void SendStepUpPidCmd_Omron(float *m_LastStepUpPidTemp_Omron,float HeatSpeed,int BoxNr);
	 void SendStepPIDDownCmd_Omron(float *m_LastStepPidDownTemp_Omron,float CoolSpeed,int BoxNr);
	 void SendStepUpPidCmd_Bruce(float *m_LastStepUpPidTemp_Bruce,int BoxNr);
	 void SendStepDownPidCmd_Bruce(float *m_LastStepDownPidTemp_Bruce,int BoxNr);
	 void SendNormalPidCmd(float temp,int BoxNr);
	 void CComm::SendNormalPidCmd(float temp1,float temp2 ,int BoxNr);
	 void CComm::SendNormalPidCmd_8PT100_8SSR(float temp[8] ,int BoxNr);
	 void SendBeepCmd(char  beep_flag,int BoxNr);
	 void SendSetFan(char fan ,int BoxNr);
	 void CComm::SendGetStm32ID(int BoxNr);
	 int SendOpenDoorCmd(char opendoor_flag,int BoxNr);
	 int CComm::Send_NTC_TH2512_Relay(char ID,int BoxNr,unsigned char pass[3]);
	 int CComm::SendSet192InterfacePcbType(char type,int BoxNr);
	 int SendStepPIDCmd(float TopTemp, int minutes,int BoxNr);
	 void SendEPM1270Reset(bool reset,int BoxNr);
	 void SendPidCoolCmd(float temp,int BoxNr,bool UsedFan);
	 void SendStepPidCoolCmd(float temp, short time,int BoxNr,bool UsedFan);
	int CComm::SetPidCount(int BoxNr);
	int CComm::ReadPidCount(int BoxNr);

	 void ReadPidT(int BoxNr);
	 void ReadPidT(int BoxNr,char ID);
	 void  CComm::SendReadTempToCmdAray(int BoxNr);

	 void SetCmdString_DeltaFrequence(int equip, DELTA_FREQUNCE_CMD_HEX cmd_hex);
	 void SetDeltaFrequenceRun(bool run);
	
	 void SendAdjustAll_OMRON(float temp,int BoxNr);
	 float OMRON_Get_AdjustedTemp(float temp,int BoxNr);
	 float OMRON_Get_MeterTemp_By_Sv(float sv,int BoxNr);
	 void ReadAdjustAll_OMRON(int BoxNr);
	 void SendDirectHeatCmd_OMRON(float temp,int BoxNr);
	
	 void SetCmdString_OMRON(int equip, COMMAND_ID cmdId, unsigned char* para);
	
	 void XModem_Start(int BoxNr,int BoardType,int NoOSC);
	static int Hex2Bin(CString HexFile,char *binData,int MaxLen);
	static int Hex2Bin(unsigned char *HexBuf,int HexBufLen ,char *binData,int MaxLen);
	void CComm::SendPcbBoardType(int BoxNr);
	void CComm::SendTM7705_OSC(int BoxNr);
	void CComm::SendSetUsefulSensors(int Sensors,int BoxNr);
	void CComm::SendAT(float temp,int BoxNr);
	void CComm::SendCoolAT(float temp,int BoxNr);
	void CComm::ReadAdjustHigh(int BoxNr);
	void CComm::ReadAdjustLow(int BoxNr);
	void CComm::ReadUsedSensor(int BoxNr);
	int   CComm::Delta_Cal_LCR(unsigned char *cmd,int len);
	void CComm::Delta_Read_X(void);
	void CComm::Delta_Read_Y(void);
	void CComm::Delta_SendReadCmd(unsigned char PlcAddr,unsigned char Cmd,unsigned short DataAddr,unsigned short DataLen);
	void CComm::Delta_SendSetCmd(unsigned char PlcAddr,unsigned char Cmd,unsigned short DataAddr,unsigned char On);

	void CComm::Delta_Read_D32(void);
	//void CComm::Delta_Read_M60(void);
	void CComm::Delta_Read_M(int Addr,int len);
	void CComm::Delta_Read_C_7A(void);
	void CComm::Delta_Set_Y(int ID,char On);
	void CComm::Delta_Set_M(int ID,char On);
	void CComm::Delta_Set_D32(int ID,int val);
	void CComm::Delta_Set_C(int ID,char On);

	void CComm::SetAdjustHigh(float *val,int BoxNr);
	void CComm::SetAdjustLow(float *val,int BoxNr);
	void CComm::SetAdjustAll(float val,int BoxNr);
	void CComm::SetAdjustAll(float* val,int BoxNr);
	void CComm::ReadAdjustAll(int BoxNr);
	void CComm::SendSetHighVolt(int BoxNr,unsigned char bHighVolt);
	void CComm::SetDisplayType(int DisplayType,int BoxNr);
	void CComm::SetAddress(int BoxNr);
	void CComm::SetPID_T(short *val,int BoxNr);
	void CComm::SetPID_T(short *val,int BoxNr,int ID);
	void CComm::ReadPIDT_P_Omron(int BoxNr);
	void CComm::ReadPIDT_I_Omron(int BoxNr);
	void CComm::ReadPIDT_D_Omron(int BoxNr);
	void CComm::ReadPIDT_T_Omron(int BoxNr);
	void CComm::SetPIDT_P_Omron(float v,int BoxNr);
	void CComm::SetPIDT_I_Omron(int v,int BoxNr);
	void CComm::SetPIDT_D_Omron(int v,int BoxNr);
	void CComm::XModem_Start_ByFile(int BoxNr);
	void CComm::ARM_XModem_Start_ByFile(int BoxNr);
	void CComm::SendProgramStart(int BoxNr);
	MODBUS_RS485_CMD_128 * CComm::SendReadStatus(int BoxNr);
	MODBUS_RS485_CMD_32PT100 * CComm::SendReadStatus_32PT100(int BoxNr);
	MODBUS_RS485_CMD_192 * CComm::SendReadStatus_192(int BoxNr);
	MODBUS_RS485_CMD_300 * CComm::SendReadStatus_300(int BoxNr);
	MODBUS_RS485_CMD_128 *  CComm::SendReadBimetalStatus(int BoxNr,unsigned char SoundLevel);
	void CComm::SendAutoTest(bool test,int BoxNr);
	//MODBUS_RS485_CMD_128 * CComm::SendReadTemp(unsigned char *switch_status,int BoxNr);
	MODBUS_RS485_CMD_192 *  CComm::SendReadTemp_192(unsigned char *switch_status,int BoxNr);
	static	CString GetCmdString(unsigned char cmd);

	int CComm::Hex2Bin_ARM(unsigned char *Hex,int HexBufLen ,char *binData,int MaxLen);
};

#define COMM_MSG	WM_USER+1000
#define CMD_STX		 0x02
#define CMD_ETX		 0x03
#ifdef _HEATBOX_RESET_
#define CRC_START_VAL_0		(unsigned char)0xFF
#define CRC_START_VAL_1		(unsigned char)0xFF
#else
#define CRC_START_VAL_0		(unsigned char)0xF2
#define CRC_START_VAL_1		(unsigned char)0xAF
#endif



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WD_STEPDLG_H__8830311D_0F83_4E8F_8DE8_CC092EAFA428__INCLUDED_)


extern COMM_THREAD_PARA G_PrntCommPara_RS232;

extern CComm			G_PrintRS232_Comm;

extern CComm			G_RS485_Comm;

extern CCommCmdArray_OMRON				m_SendCmdArray_OMRON;
extern CCommCmdArray_128				m_CmdArray_128[MAX_HEAT_BOX];
extern CCommCmdArray_192				m_CmdArray_192[MAX_HEAT_BOX];
extern CCommCmdArray_DetlaFrequnce_ASC	m_SendCmdDeltaFrequnce;
extern C20Temp5SsrAnswerArray			G_20Temp5SsrAnswerArray[2];

extern CCommCmdProgram					G_SendmCmdProgram;
extern int								G_ProgramBoxNr;
extern CCommCmdProgram_ARM				G_SendmCmdProgram_ARM;
extern int								G_iLangID ;
extern CDelta_PLC_SetCmdArray			G_Delta_PLC_SetCmdArray;
extern DELTA_PLC_SET_CMD				G_Delta_PLC_SetCmd;
extern DELTA_PLC_READ_CMD				G_Delta_PLC_ReadCmd;

void DealDeltaPLC(unsigned char *rec_cmd, int data_len );
void ReadDeltaPLC(COMM_THREAD_PARA *para,CByteArray     *ReadArray);