#pragma once

#include "comm.h"
#include "typedefs.h"

#include "afxwin.h"

#define METER_OUTPUT_STATUS_BIT_DOOR		0
#define METER_OUTPUT_STATUS_BIT_AIR			1
#define METER_OUTPUT_STATUS_BIT_AIR_VALVE	2
#define METER_OUTPUT_STATUS_BIT_FAN			3
#define METER_OUTPUT_STATUS_BIT_BEEP		4

/*
typedef struct
{
	float Temp;
	unsigned char HeatPercent;
	DWORD ticks;
}STRUCT_TEMP_TICKS;

typedef CArray<STRUCT_TEMP_TICKS ,STRUCT_TEMP_TICKS> CTempTickArray;
*/

class CTempMeter
{

public:
	CTempMeter();
	void Init(HWND	WndHandle,CDC *memDC,CRect rect,MODBUS_RS485_TEMP_ANSWER_PARA m_TempAnswerPara);
	void Init(HWND	WndHandle, CDC *memDC,CRect rect,MODBUS_RS485_8TEMP_8SSR_ANSWER Answer_8TEMP_8SSR);
	void Init(HWND	WndHandle, CDC *memDC,CRect rect,MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT answer_para);
	void CTempMeter::Init(HWND	WndHandle, CDC *memDC,CRect rect,MODBUS_RS485_32TEMP_ANSWER Answer_32TEMP);

	~CTempMeter();

public:

	CTime		m_LedAnswerTime[2];
	CTime		m_MeterAnswerTime;

	HWND	m_WndHandle;
	CDC		*m_DC;
	CRect	m_Rect;
	MODBUS_RS485_32TEMP_ANSWER		m_32TEMP_ANSWER;
	MODBUS_RS485_TEMP_ANSWER_PARA	m_TempAnswerPara;
	MODBUS_RS485_8TEMP_8SSR_ANSWER	m_8TEMP_8SSR_Answer;
	MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT	m_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT_answer_para;
	float	m_SendSv;
	float	m_Sv;
	float	m_Pv;
	int		m_BoxNr;

	int		m_PvFontSize;
	
	int		m_TextFontSize;

	void	DrawMeter(int BoxNr);
	void	DrawMeter(int BoxNr,MODBUS_RS485_8TEMP_8SSR_ANSWER m_8TEMP_8SSR_Answer);
	void	DrawMeter(int BoxNr,MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT m_8TEMP_8SSR_Answer);
	void	DrawMeter(MODBUS_RS485_TEMP_ANSWER_PARA m_TempAnswerPara);
	void	DrawMeter(MODBUS_RS485_8TEMP_8SSR_ANSWER m_8TEMP_8SSR_Answer);
	void	DrawMeter(MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT m_8TEMP_8SSR_Answer);
	void	DrawMeter(MODBUS_RS485_32TEMP_ANSWER Answer_32TEMP);
	void	DrawMeter(int BoxNr,MODBUS_RS485_32TEMP_ANSWER Answer_32TEMP);
	static float	GetCurPv(MODBUS_RS485_TEMP_ANSWER_PARA TempAnswerPara);
	float	GetCurSv(void);
	float	GetCurAmp(void);
	float	GetCurAmp2(void);
	float	m_Speed;
	float   m_StartTemp,m_EndTemp;
	int     m_TimeSpan;
	bool	m_IsDoorOpened;
	bool	m_IsAcOn;
	bool	m_IsAcValveOn;
	bool	m_IsFanOn;
	bool	m_IsBeepOn;
	
	PCB_SUB_MODE		m_PcbTestMode;

};

