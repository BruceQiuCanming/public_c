#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



#ifdef WINCE 
#define MAX_HEAT_BOX	1
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#define FLASH_PARA_ADDR			0x0008

#define MAX_GROUPS				20


#define MAX_LED_NR				256


#ifndef WINCE
	#define MAX_HEAT_BOX		6
#endif
typedef enum 
{
	OMRON_PV	= 0x2000,
	OMRON_SP	= 0x2601,
	OMRON_PID_T = 0x2708,
	OMRON_PID_P = 0x2A00,
	OMRON_PID_I = 0x2A01,
	OMRON_PID_D = 0x2A02,

	OMRON_PV_ADJUST	= 0x2723,

	OMRON_ALARM_1	=	0x2902,
	OMRON_ALARM_1H	=	0x2903,
	OMRON_ALARM_1L	=	0x2904,
	OMRON_ALARM_2	=	0x2905,
	OMRON_ALARM_2H	=	0x2906,
	OMRON_ALARM_2L	=	0x2907,
	OMRON_ALARM_3	=	0x2908,
	OMRON_ALARM_3H	=	0x2909,
	OMRON_ALARM_3L	=	0x290A,
	OMRON_ALARM_4	=	0x290B,
	OMRON_ALARM_4H	=	0x290C,
	OMRON_ALARM_4L	=	0x290D,



}OMRON_CMD_ADDR;

typedef enum 
{
	PCB_SUB_MODE_STOP						=	0,	
	PCB_SUB_MODE_DIRECT_HEAT				=	1,
	PCB_SUB_MODE_NORMAL_PID					=	2,
	PCB_SUB_MODE_INCREASE_PID				=	3,
	PCB_SUB_MODE_DIRECT_COOL				=	4,
	PCB_SUB_MODE_PID_COOL					=	5,
	PCB_SUB_MODE_STEP_PID_COOL				=	6,
	PCB_SUB_MODE_AT							=	7,
	PCB_SUB_MODE_COOL_AT					=	8,

	PCB_SUB_MODE_PID_COOL_BY_FAN			=	9,
	PCB_SUB_MODE_STEP_PID_COOL_BY_FAN		=	10,

//	SUB_MODE_PID_OUTSIDE_AIR_COOL		=	11,
//	SUB_MODE_STEP_PID_OUTSIDE_AIR_COOL	=	12,
}PCB_SUB_MODE;

typedef enum 
{
	SUB_MODE_STOP								=	0,		//空闲
	SUB_MODE_READY								=	1,		//等待...
	SUB_MODE_DIRECT_HEAT						=	2,		//全速升温	
	SUB_MODE_PID								=	3,		//PID恒温
	SUB_MODE_STEP_PID							=	4,		//线性升温
	SUB_MODE_DIRECT_COOL						=	5,		//全速降温
	SUB_MODE_STEP_COOL							=	6,		//线性降温
	SUB_MODE_HEAT_AGAIN							=	7,		//温度降到最后再次升温
	SUB_MODE_PID_COOL							=   8,		//降温时恒温
	SUB_MODE_PID_2								=	9,		//第二次PID恒温
	SUB_MODE_STEP_PID_2							=	10,		//第二次线性升温
	SUB_MODE_STEP_TOP_PID						=	11,		//到了最高温后恒温一段时间
//	SUB_MODE_TOP_PID_TEMP						=	12,		//到了最高温后恒温一段时间
	SUB_MODE_DIRECT_HEAT_AFTER_ADJUST			=	13,		//可调温控器专用


	SUB_MODE_CAL_LOW							=	0x20,
	SUB_MODE_CAL_HIGH							=	0x21,
	SUB_MODE_CAL_INPUT_GLASS_TEMP				=	0x22,


	SUB_COOL_FIRST_MODE_READY					=	0x80,
	SUB_COOL_FIRST_MODE_DIRECT_COOL				=	0x81,
	SUB_COOL_FIRST_MODE_COOL_PID				=	0x82,
	SUB_COOL_FIRST_MODE_STEP_COOL_PID			=	0x83,
	SUB_COOL_FIRST_MODE_STEP_DIRECT_HEAT		=	0x84,
	SUB_COOL_FIRST_MODE_HEAT_PID				=	0x85,
	SUB_COOL_FIRST_MODE_STEP_HEAT_PID			=	0x86,

	

}SUB_MODE; //AUTO_TEST_STEP; //





extern CString strMsgCaption[];

typedef enum 
{
	HEAT_COOL_UNKNOW		=	0,
	HEAT_FIRST_COOL_SECOND	=	1,
	COOL_FIRST_HEAT_SECOND	=	2
	
}HEAT_COOL_ORDER;




typedef struct
{
	float open_temp_min;
	float open_temp_max;
	float close_temp_min;
	float close_temp_max;
}CONTROL_TEMP_RANGE;

/*
typedef enum
{
	COMM_PROTOCOL_MODBUS_128,
	COMM_PROTOCOL_MODBUS_192,
	COMM_PROTOCOL_MODBUS_PROGRAM,
	COMM_PROTOCOL_DELTA_FREQUENCE_ASC,
	COMM_PROTOCOL_ASC,
	COMM_PROTOCOL_OMRON,
}COMM_PROTOCOL_TYPE;
*/

/*
typedef struct
{
	bool	IsUsed;			//产品是否插入
	bool	IsClosing;			//现在是否闭合
	bool	IsOpenned;			//是否曾经打开过
	bool	IsClosed;			//是否曾经复位过
//	bool IsDoubleClosed; // 
	bool	IsOpenFlash;		//是否动作时闪动
	bool	IsCloseFlash;		//是否复位时闪动
	bool	IsOpenFlashTested;
	bool	IsCloseFlashTested;		//用于开关门测闪动
	bool	IsNoPTC;
	double	OpenTemp;			//动作温度
	double	CloseTemp;		//复位温度
//	double OpenFlashTemp;	//动作温度
//	double CloseFlashTemp;	//复位温度
//	char  Barcode[32];
	double  AdjustTemp;
	unsigned char KSD302_TickCount;
}SWITCH_TEST_RESULT;
*/

typedef struct
{
	unsigned	IsUsed	:			1;			//产品是否插入
	unsigned	IsClosing:			1;			//现在是否闭合
	unsigned	IsOpenned:			1;			//是否曾经打开过
	unsigned	IsClosed:			1;			//是否曾经复位过
//	bool IsDoubleClosed; // 
	unsigned	IsOpenFlash:		1;		//是否动作时闪动
	unsigned	IsCloseFlash:		1;		//是否复位时闪动
	unsigned	IsOpenFlashTested:	1;
	unsigned	IsCloseFlashTested:	1;		//用于开关门测闪动
	unsigned	IsNoPTC:			1;
	
	float	OpenTemp;			//动作温度
	float	CloseTemp;		//复位温度
//	double OpenFlashTemp;	//动作温度
//	double CloseFlashTemp;	//复位温度
//	char  Barcode[32];
//	float  AdjustTemp;
	unsigned char	Group;
	unsigned char   OpenSoundStrong;
	unsigned char   CloseSoundStrong;
	unsigned short KSD302_TickCount;
#ifndef WINCE	
	float	OpenFlashTemp;		//动作温度
	float	CloseFlashTemp;		//复位温度
	long	OpenTime;
	long	CloseTime;
#endif

}SWITCH_TEST_RESULT;


typedef struct
{
	int		switch_nr;
	int		level;
	SWITCH_TEST_RESULT test_result;
}SWITCH_TEST_RESULT_AND_LEVEL;

typedef enum 
{
	CLOSE_TEMP_TYPE_RANGE					=	0,
	
	CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP			=	1,

	CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE=	2,

}CLOSE_TEMP_TYPE;

typedef struct
{
	int		CloseTempType;
	TCHAR   HeadName[7];
	TCHAR   Name[21];
	float MainLevel;
	float MainLevelSpanMinus;
	float MainLevelSpanPlus;
	float HighLevel;
	float HighLevelSpanMinus;
	float HighLevelSpanPlus;
	float LowLevel;
	float LowLevelSpanMinus;
	float LowLevelSpanPlus;
	
	float CloseLevel;
	float CloseLevelSpanPlus;
	float CloseLevelSpanMinus;

	float CloseHighLevel;
	float CloseHighLevelSpanPlus;
	float CloseHighLevelSpanMinus;

	float CloseLowLevel;
	float CloseLowLevelSpanPlus;
	float CloseLowLevelSpanMinus;

	unsigned int	OpenDoorResetLevel;
	unsigned char	dummy;		//IsPTC;
	//================
	float			CloseLevelDiffPlus;
	float			CloseLevelDiffMinus;

	float			CloseHighLevelDiffPlus;
	float			CloseHighLevelDiffMinus;

	float			CloseLowLevelDiffPlus;
	float			CloseLowLevelDiffMinus;
}SWITCH_CONFIG_PARA;

typedef struct
{
	char type;
	unsigned int para[4];
}AUTO_TEST_PARA;

typedef union
{
	short v;
	char  buf[2];
}SHORT_BUF;




typedef enum
{
	ERROR_LEVEL						= -1,
	NO_LEVEL						= 0,
	MAIN_LEVEL						= 1,
	TEST_RESULT_HIGH_LEVEL			= 2,
    LOW_LEVEL						= 3,
	OVER_HIGH_LEVEL					= 4,
    OVER_LOW_LEVEL					= 5,
	CLOSE_HIGH_LEVEL				= 6,
	CLOSE_LOW_LEVEL					= 7,
	OPEN_FLASH_LEVEL				= 8,
	CLOSE_FLASH_LEVEL				= 9,
	OPEN_DOOR_RESET_LEVEL			= 10,
	HIGH_HIGH_LEVEL					= 11,
	NOT_USED						= 12,
	NO_PTC_LEVEL					= 13,

	HIGH_LEVEL_CLOSE_HIGH_LEVEL		= 14,
	HIGH_LEVEL_CLOSE_LOW_LEVEL		= 15,
	LOW_LEVEL_CLOSE_HIGH_LEVEL		= 16,
	LOW_LEVEL_CLOSE_LOW_LEVEL		= 17,

}TEST_RESULT_LEVEL;

typedef enum
{
#ifdef WINCE
	BOX_128							=	0,
	BOX_192,
	BOX_KSD302_TIME,
	BOX_YangZhouYingDi,
	BOX_128_HanJie,
	BOX_192_HanJie,
	HEAT_BOX_TYPE_PTC,
	BOX_128_PTC_NOT_TESTED,
	BOX_32_OMRON,
	BOX_45_OMRON,
	BOX_64_OMRON,
	BOX_128_OMRON,
	BOX_48_CHANGSHENG_LOW_TEMP,
	BOX_KSD302x2,
	BOX_32,
	BOX_48,
	HEAT_BOX_TYPE_ANTI_PTC,		//反向PTC
	BOX_KSD302_TEMP,
	BOX_DISC_40,
	BOX_NINGBO_WOLI,
	BOX_NTC,
	BOX_63_7X9,
	BOX_DISC_FLAT,

	

#else
	BOX_128						=	0,
	HEAT_BOX_TYPE_PTC			=	1,
	HEAT_BOX_TYPE_PTC_NO_TEST	=	2,
	HEAT_BOX_TYPE_HANJIE		=	3,
	BOX_DISC_40					=	4,
	HEAT_BOX_TYPE_JIAJU_TEST	=	5,
	HEAT_BOX_TYPE_128_OLD_DISPLAY = 6,
	HEAT_BOX_TYPE_ANTI_PTC		=	7,
	BOX_NTC						=	8,
#endif

	BOX_DISC_16,
	BOX_DISC_24,
	BOX_DESK_OVEN,
	BOX_DOUBLE_SWITCH,
	BOX_JIU_YANG,
	BOX_8PT100_8SSR,
	BOX_DELTA_PLC_8PT100_8SSR,
	BOX_OIL_BATHS,
	BOX_128_EXT_INPUT,
	BOX_128_ONLY_OPENTEMP_TEST,
	BOX_TEMP2_10IN_12OUT,
	BOX_SENSATA_32PT100,
#ifndef WINCE
	BOX_192,
	BOX_300,
	BOX_TX300W_POWER,
	BOX_KSD302_TIME,
	BOX_YangZhouYingDi,
	BOX_128_HanJie,
	BOX_192_HanJie,
	BOX_128_PTC_NOT_TESTED,
	BOX_32_OMRON,
	BOX_45_OMRON,
	BOX_64_OMRON,
	BOX_128_OMRON,
	BOX_48_CHANGSHENG_LOW_TEMP,
	BOX_KSD302x2,
	BOX_32,
	BOX_48,
	BOX_KSD302_TEMP,
//	BOX_DISC_40,
	BOX_NINGBO_WOLI,
	BOX_NTC_RES,
	BOX_63_7X9,
	BOX_DISC_FLAT,
	
#endif
	
	
}BOX_TYPE;

typedef enum
{
	METER_RAINBOW_MODBUS,
	METER_RAINBOW_ASCII,
	METER_OMRON,

}METER_TYPE;

typedef enum
{
	NO_BOARD							=   0,

	HIGH_VOLT_BOARD						=	1,
	PTC_BOARD							=	2,
	KSD302_BOARD						=	3,
	INTERFACE128_V4_0_BOARD				=	4,  //_INTERFACE128_V4_0 不带光耦
	INTERFACE128_V4_2_BOARD				=	5,  //_INTERFACE128_V4_2 带光耦隔离
	INTERFACE128_HEAT_MAIN_BOARD		=	6,  //	标准板
	EPM1270_BOARD						=	7,
	DISC_FLAT_BOARD						=	8,	//  双金属片平板
	DISC_OVEN_BOARD						=	9,	//  _T("双金属片烘箱")},
	DISC_DESK_ROUND_OVEN_BOARD			=	0x0A,	//	_T("桌面双金属片烘箱")},
	DESK_OVEN_BOARD						=	0x0B,	//	_T("桌面小烘箱")},
	TEMP2_10IN_12OUT_BOARD				=	0x0C,
	dummy_0x0D,
	dummy_0x0E,
	dummy_0x0F,
	dummy_0x10,
//	灯板
	INTERFACE128_STANDARD_LED_BOARD_AND_HIGH_VOLT_BOARD				= 	0x11,
	INTERFACE128_STANDARD_LED_BOARD_AND_PTC_BOARD					= 	0x12,
	INTERFACE128_STANDARD_LED_BOARD_AND_KSD302_BOARD				= 	0x13,
	INTERFACE128_STANDARD_LED_BOARD_AND_INTERFACE128_V4_0_BOARD		= 	0x14,
	INTERFACE128_STANDARD_LED_BOARD_AND_INTERFACE128_V4_2_BOARD		= 	0x15,
	dummy_0x16,
	INTERFACE128_STANDARD_LED_BOARD_AND_EPM1270_BOARD				= 	0x17,
	dummy_0x18,
	dummy_0x19,
	dummy_0x1A,
	dummy_0x1B,
	dummy_0x1C,
	dummy_0x1D,
	dummy_0x1E,
	LI_SIR_BOARD,
	dummy_0x20,
	STM32_20NTC_1PT100_3SSR											=	0x21,
	
}INTERFACE_PCB_TYPE;

typedef struct
{
	INTERFACE_PCB_TYPE	type;
	TCHAR				name[100];
	TCHAR				eng_name[100];
}INTERFACE_PCB_TYPE_NAME;

typedef struct
{
	int ID;
	SWITCH_TEST_RESULT result;
}SWITCH_TEST_RESULT_EX;




/*
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
*/

typedef struct
{
//	bool			AutoTestFlag;
	SUB_MODE		SubMode;

}WORK_STATUS;

typedef struct
{
	unsigned int	SelfCheckTimeCount;
	unsigned int	SomeCountsNoZero;		//本测试周期中,有噪音
	unsigned short	MaxLevel[40][2];		//噪音等级
	unsigned short	MaxCounts[40][2];		//噪音被检测到的次数	
	unsigned short	CurCounts[40];		//噪音被检测到的次数	

}NOISE;

typedef struct
{
	float  Sv;	//Temp
	int		Days;
	int Hours;
	int Minutes;
}MULTI_PID_PARA;

typedef struct
{
	short P;
	short I;
	short D;
	short T;
	short COOL_P;
	short COOL_I;
	short COOL_D;
}PID;

typedef enum
{
	PRINTER_TYPE_LPM260 = 0,
	PRINTER_TYPE_PM_24,
}PRINTER_TYPE;
typedef struct
{//新参数加在尾巴上，在程序更新后，老的参数依然有效
#ifdef WINCE
	short				HeatSpeed;
	short				CoolSpeed;
	short				PidTime;
	TCHAR				SelParaName[50];
	short				TestMode;;
	short				OpenDoorReset_HeatDegree;
	short				OpenDoorReset_CoolDegree;
	short				AirControlEquipment;
	short				SwitchType;
	unsigned char		CoolDownAutoOpenDoor;
	unsigned char		NoFlashTest;
	short				StopTestAutoCloseDoor;
	char				SaveCurve;
	//char				bAirCool_then_DirectHeat;
	long				LastAirCloseTime;
	int					BoxNr;
	float				OverHeatGap;
	BOX_TYPE			BoxType;
	unsigned char		G_RS485_CommPara;
	int					RepeatTestCounts;
	 //0=加热丝  1=加热管
	//unsigned char		InterfaceBoardType;
	float				PID_Temp_Delta;		//only for Adjust Swich????????
	//float				Direct_Heat_Temp_Delta;
	//用于保存校温时的加热特性
	//int					LowPid_HeatPercent;
	//int					HighPid_HeatPercent;
	//float				LowPidTemp;
	//float				HighPidTemp;
	//int				MidFullHeat_HeatSpeed;
	short				PidCoolTime;					//降温时的PID恒温时间
	float				OMRON_0C_Adjust;
	float				OMRON_200C_Adjust;		
	unsigned	char	SoundLevel_Heat;					//测试时的声音强度等级，1--8级，1最低，最灵敏，抗干扰最弱
	unsigned	char	SoundLevel_Cool;					//测试时的声音强度等级，1--8级，1最低，最灵敏，抗干扰最弱
	unsigned	char	SoundLevel_Time;
	unsigned	char	SoundOutputType;				//声音的输出方式
	SYSTEMTIME			stAirControlCloseTime;		//空调压缩机上次关闭时间
	int					Language;									//软件使用的语言，0中文（默认），1English
	unsigned int		MaxLedNrType;								//通讯协议
	unsigned char		Group[192];
	bool				bSpeech;
	int					CommTimer;
	int					StepCoolDelta;
	METER_TYPE			MeterType;
	short				TopPidMinutes;
	float				AmpAdjust_1;
	float				AmpAdjust_2;
	int					JiuYangTime_Gap;
#else
	unsigned char		RS485_CommPort;
	unsigned char		PrinterCommPort;
	unsigned int		MaxLedNrType_No; //通讯协议
	TCHAR				ParaName[100];


	float				dummy[382-1];//SwitchTempAdjust[2][192];

	METER_TYPE			MeterType;


	short				HeatSpeed;
	short				CoolSpeed;
	short				PidTime;
	
	short				TestMode;

	float				OpenDoorReset_HeatDegree;
	float				OpenDoorReset_CoolDegree;
	short				AirControlEquipment;
	short				SwitchType;
	bool				CoolDownAutoOpenDoor;
	bool				NoFlashTest;
	short				StopTestAutoCloseDoor;
	char				SaveCurve;
	char				bAirCool_then_DirectHeat;
	long				LastAirCloseTime;
	//int					BoxNr;
	float				OverHeatGap;
	BOX_TYPE			BoxType;
	
	int					RepeatTestCounts;
	
//	unsigned char		InterfaceBoardType;
//	unsigned char		TM7705_NoOSC;
//	float				PID_Temp_Delta;
	float				Direct_Heat_Temp_Delta;
	//用于保存校温时的加热特性
	int					LowPid_HeatPercent;
	int					HighPid_HeatPercent;
	float				LowPidTemp;
	float				HighPidTemp;
	int					MidFullHeat_HeatSpeed;
	short				AirControlValvePercent;
	short				FanPercent;
	short				PidDownTime;
	float				OMRON_0C_Adjust;
	float				OMRON_200C_Adjust;
	unsigned int		CommTimer;
	bool				bSpeech;
	bool				bSpeechControl;

	unsigned char		Group[192];

	int					StepCoolDelta;
//	int					TempSensorDelay;
	int					TopPidMinutes;
	SYSTEMTIME			stAirControlCloseTime;			//空调压缩机上次关闭时间
	int					Language;						//软件使用的语言，0中文（默认），1English
	unsigned	char	SoundLevel_Heat;				//测试时的声音强度等级，1--8级，1最低，最灵敏，抗干扰最弱
	unsigned	char	SoundLevel_Cool;				//测试时的声音强度等级，1--8级，1最低，最灵敏，抗干扰最弱
	unsigned	char	SoundLevel_Time;
	unsigned	char	SoundOutputType;				//声音的输出方式

#endif
	unsigned char		InterfaceBoardType;
	unsigned char		TM7705_NoOSC;
	
	unsigned char		AutoPrintType;				//是不是要打印测试结果 0=不打印，1=打印 ，2= 打印除温度分布图
	unsigned char		AutoPrintForwardType;
	unsigned char		StopTestType;				//是不是所有产品都复位了才结束测试

	float				Cal_LowStandardTemp[4];
	float				Cal_LowMeterTemp[4];
	float				Cal_HighStandardTemp[4];
	float				Cal_HighMeterTemp[4];
	
	MULTI_PID_PARA		m_MultiPidPara[3];
#ifndef _HeatBox
	char				UsedPT100[2][4];
#endif
	unsigned char		ServerIP[4];
	int					EquipmentNr;
	float				Sv[2];
	int					RunCounts;
	float				Sv_8TEMP_8SSR[8];
	int					PrinterType;
	unsigned int		Para_Password;
	PID					pid_t;
	float				AdjustLow[32];
	float				AdjustHigh[32];
	TCHAR				MES_URL[201];
	int					bBarcode;
	TCHAR				TestMemo[81];
	TCHAR				Tester[21];
	int					bONE_BY_ONE_TAKEBOARD;
	int					bDebugMode;;

	unsigned int		Local_TCP_Port;
	BYTE				MES_IP[4];
	unsigned int		MES_TCP_Port;
	TCHAR				MES_Equipment_Code[100];
	TCHAR				MES_WorkStation_Code[100];
	TCHAR				MES_WorkStation_Name[100];

	TCHAR				MES_WorkSheet_Code[100];
	TCHAR				MES_Product_Code[100];
	TCHAR				MES_Product_Name[100];

}NORMAL_CONFIG_PARA;

typedef struct
{
	TCHAR	URL_1[200];
	TCHAR	URL_2[200];
	TCHAR	site[100];
	TCHAR	operation[100];
	TCHAR	resrce[100];
	TCHAR	barcode[100];
	TCHAR	userid[100];
	TCHAR	password[100];
	TCHAR	shop_order[100];
	TCHAR	sfcs[100];
}TOPBAND_VIETNAM_MES_CONFIG;
/*
#pragma pack(push)
#pragma pack(1)
typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;

	unsigned char para[128/8];
	unsigned short crc;

}MODBUS_RS485_CMD_128;
typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;

	unsigned char para[192/8];
	unsigned short crc;
}MODBUS_RS485_CMD_192;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;

	unsigned char para[129];
	unsigned short crc;
}MODBUS_RS485_CMD_PROGRAM;
typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
	unsigned char para[(128+7)/8 * 2];
	unsigned short crc;
}MODBUS_RS485_STATUS_FLASH_ANSWER_128;
typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
	unsigned char para[(192+7)/8 * 2];
	unsigned short crc;
}MODBUS_RS485_STATUS_FLASH_ANSWER_192;

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
	unsigned short para_len;
	unsigned char para[64*sizeof(float)];
	unsigned short crc;
}MODBUS_RS485_NTC_AD_VAL_ANSWER;



typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
	unsigned char para[64];
	unsigned short crc;
}MODBUS_RS485_TICK_COUNT_ANSWER;

typedef struct
{
	char RS485_Addr;
	char cmd;
	char para_len;
	unsigned char para[20];
	unsigned short crc;
}MODBUS_RS485_DISC_ANSWER;


#pragma pack(pop)
*/

typedef enum
{
		TEST_MODE_NORMAL									=	0,
		TEST_MODE_CLOSE_DOOR_REPEAT							=	1,
		TEST_MODE_CLOSE_DOOR_ONCE							=	2,
		TEST_MODE_OPEN_DOOR_RESET							=	3,
		TEST_MODE_TOP_TEMP_PID								=	4,
		TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET		=	5,
		TEST_MODE_NORMAL_PID_CAL_OPEN_TIME					=	6,
}TEST_MODE;

typedef struct
{
	double temp;
	long   TimeTick;
}TEMP_TIME;

typedef CArray<TEMP_TIME ,TEMP_TIME> CTempTimeArray;

/*
typedef struct
{
	COMMAND_ID   CmdID;				//自动测试时已经发送的命令
	CTime        SendCmdTime;		//自动测试时发送命令时间
	char		 Para[4];			//自动测试时发送命令的参数
}SEND_COMMAND;
*/

#define STD_TIME	CTime(2000,1,1,0,0,0)


#define EXE_FILE	_T("HardDisk\\cesvr.exe")

//测试组
#define GROUP_FILE	_T("HardDisk\\GROUP.BIN")

//测试温度
#define NORMAL_CONFIG_PARA_FILE	_T("HardDisk\\NORMAL_CONFIG_PARA.BIN")
//测试记录
#define RECORD_FILE	_T("HardDisk\\RECORDS.BIN")
//测试记录
#define COMPANY_FILE	_T("HardDisk\\COMPANY.BIN")
//各种选项
#define CONFIG_PARA_FILE_DIR	_T("HardDisk\\PARA\\")
#define CONFIG_SWITCH_ADJUST_FILE_DIR	_T("HardDisk\\SwitchAdjust\\")
#define NOISE_FILE_DIR					_T("HardDisk\\Noise\\")
#define NOISE_FILE_NAME					_T("HardDisk\\Noise\\Noise.bin")
#define SELECTED_PARA_LEN	100

#define DIRECT_HEAT_DELTA	(G_PID[BoxNr].P * 1.0f)
#define PID_HEAT_DELTA		(G_PID[BoxNr].P * 0.3f)


#define DIRECT_COOL_DELTA	(G_PID[BoxNr].P)

#define PID_COOL_DELTA		(3.0f)
#ifdef _CHANGSHEN_
	#define OVER_HEAT_DELTA		(4.9f)
#else
	#define OVER_HEAT_DELTA		(0.9f)
#endif



#define AIR_WAIT_MINUTES		2

typedef struct
{
	double MainLevelAvrSpanMinus;
	double MainLevelAvrSpanPlus;
	double MainLevelCloseAvrSpanMinus;
	double MainLevelCloseAvrSpanPlus;


	double HighLevelAvrSpanMinus;
	double HighLevelAvrSpanPlus;
	double HighLevelCloseAvrSpanMinus;
	double HighLevelCloseAvrSpanPlus;

	double LowLevelAvrSpanMinus;
	double LowLevelAvrSpanPlus;
	double LowLevelCloseAvrSpanMinus;
	double LowLevelCloseAvrSpanPlus;

	float	HeatSpeed;

}SWITCH_CONFIG_PARA_EX;


typedef struct
{
#ifdef WINCE
	long					StartTime;
	long					EndTime;
	SWITCH_CONFIG_PARA		ConfigPara;
//	CONTROL_TEMP_RANGE		temp_range;
	int						SwitchType;
//	HEAT_COOL_ORDER			m_HeatCoolOrder;
	BOX_TYPE				BoxType;
	SWITCH_TEST_RESULT		TestResult[128];

#else
	int						size;
	long					StartTime;
	long					EndTime;
	char					SwitchType;
	BOX_TYPE				BoxType;
	TCHAR					ParaName[20];
	TCHAR					Memo[80];
	TCHAR					Tester[20];
	SWITCH_CONFIG_PARA		ConfigPara;
	SWITCH_TEST_RESULT		TestResult[192];
	SWITCH_CONFIG_PARA_EX	ConfigParaEx;
#endif
	float					HeatSpeed;
	float					CoolSpeed;
}RECORD;

typedef struct
{
	int year;
	int month;
	int day;

	int hour;
	int minute;
	int second;

	int filelen;
}EXCEL_RECORD_FILE_SOCKET_HEADER;

/*
typedef struct
{
	int						size;
	long					StartTime;
	long					EndTime;
	char					SwitchType;
	TCHAR					Memo[100];
	SWITCH_CONFIG_PARA		ConfigPara;
	CONTROL_TEMP_RANGE		temp_range;
	int						m_SwitchType;
	HEAT_COOL_ORDER			m_HeatCoolOrder;
	SWITCH_TEST_RESULT		TestResult[192];
	SWITCH_CONFIG_PARA_EX	ConfigParaEx;
	BOX_TYPE				BoxType;		
}RECORD_128;

typedef struct
{
	int					size;
	long				StartTime;
	long				EndTime;
	char				SwitchType;
	TCHAR				Memo[100];
	SWITCH_CONFIG_PARA	ConfigPara;
	CONTROL_TEMP_RANGE	temp_range;
	int					m_SwitchType;
	HEAT_COOL_ORDER		m_HeatCoolOrder;
	SWITCH_TEST_RESULT  TestResult[192];
	SWITCH_CONFIG_PARA_EX	ConfigParaEx;
}RECORD_192;
*/
typedef struct
{
	long				StartTime;
	long				EndTime;
	SWITCH_CONFIG_PARA	ConfigPara;
	CONTROL_TEMP_RANGE	temp_range;
	int					m_SwitchType;
	HEAT_COOL_ORDER		m_HeatCoolOrder;
	SWITCH_TEST_RESULT  TestResult[16];
	char				Barcode[16][32];
}RECORD_YANGZHOU_YINGDI;

typedef struct
{
	TCHAR		file_flag[8];
	int			cur_record_id;
}RECORD_HEADER;




typedef struct
{
	SUB_MODE sub_mode;
	float	 goal_temp;
	int      minutes;
}STRUCT_SUB_MODE;


typedef struct
{
	float temp;
	float NTC[64];
}NTC_RECORD;

typedef struct
{
	unsigned short	id;
	unsigned short	SoundCounts[20];
	SYSTEMTIME		st;
	float			temp;
	int				IsActMode;
	int				SameCounts;
}DISC_RECORD;

typedef CArray<DISC_RECORD ,DISC_RECORD> CBiMetalRecordArray;
typedef CArray<STRUCT_SUB_MODE ,STRUCT_SUB_MODE> CSubModeArray;
typedef CArray<CByteArray ,CByteArray> CCommCmdArray;
typedef CArray<NTC_RECORD ,NTC_RECORD>CNtcRecordArray;



typedef struct
{
	double	open_max_temp;
	double	open_min_temp;
	double	open_avr_temp;
	double	close_max_temp;
	double	close_min_temp;
	double	close_avr_temp;
	int	 	total_product;
	int		total_main_level;

}TONG_JI_INFO;


typedef struct
{
	SWITCH_CONFIG_PARA		para;
	SWITCH_CONFIG_PARA_EX	para_ex;

}SWITCH_CONFIG_PARA_ALL;

typedef struct
{
	TCHAR		NrStr[100];
	TCHAR		OpenTempStr[100];
	TCHAR		CloseTempStr[100];
	TCHAR		DIF[100];
	TCHAR		TestLevelStr[100];
}PRINT_STR;

typedef CArray<PRINT_STR ,PRINT_STR> CPrintStringArray;

/*
typedef CArray<MODBUS_RS485_CMD_PROGRAM ,MODBUS_RS485_CMD_PROGRAM>CCommCmdProgramArray;
typedef CArray<MODBUS_RS485_CMD_OMRON ,MODBUS_RS485_CMD_OMRON> CCommCmdArray_OMRON;
typedef CArray<MODBUS_RS485_CMD_128 ,MODBUS_RS485_CMD_128> CCommCmdArray_128;
typedef CArray<MODBUS_RS485_CMD_192 ,MODBUS_RS485_CMD_192> CCommCmdArray_192;
typedef CArray<DELTA_FREQUNCE_CMD_ASC ,DELTA_FREQUNCE_CMD_ASC>CCommCmdArray_DetlaFrequnce_ASC;


extern COMM_PROTOCOL_TYPE G_CommProtocolType;
*/

typedef struct
{
	__time64_t	time;
	float		temp[5];
	unsigned char HeatPercent;
}TEMP_RECORD;



typedef CArray<TEMP_RECORD ,TEMP_RECORD> CTempArray;


typedef struct
{
	HWND	m_HWnd;
	bool    bContinue;
}RECORD_THREAD_PROC_PARA;

typedef CArray<int ,int> CIntArray;


typedef struct
{
	float		temp;
	float		NTC[64];
}NTC_TEMP_RECORD;
typedef CArray<NTC_TEMP_RECORD ,NTC_TEMP_RECORD> CNtcTempRecordArray;

typedef struct
{
	char buf[100];
}PrintLine;

typedef CArray<PrintLine ,PrintLine> CPrintLineArray;

typedef CArray<float ,float> CFloatArray;


#define CAL_SPEED_PERIOD	20

#endif

