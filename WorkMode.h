#ifndef _WORK_MODE_H_
#define _WORK_MODE_H_
#include "typedefs.h"


class CWorkMode
{
private:
	
	//CWnd * m_ParentWnd;
public:

	CTime					m_CurSubModeBeginTime;
	float					m_SendSv;
	int						m_iTestMode;
	SUB_MODE				m_SubMode;
	//CString *				m_TestMsg;
	float					m_OpenDoor_Temp;
	CTime					m_TopPIDStartTime;
	CTime					m_EndTestTime;
	CTime					m_BeginTestTime;
	CTime					m_OpenDoor_StartTime;

	float					m_CloseDoorTopTemp;
	float					m_CloseDoorStartTemp;
	CTime					m_CloseDoorStartTime;
	SWITCH_CONFIG_PARA		m_SwitchConfigPara;
	CWorkMode(void);
	int						m_DiscID;
	CTempTimeArray			m_PidTempArray;
	double					m_AdjustLow[4][4];			//线路板上的修正值
	double					m_AdjustHigh[4][4];			//线路板上的修正值
	double					m_CalLowPidTemp[4];			//校准时PID恒温值
	double					m_CalHighPidTemp[4];		//校准时PID恒温值
	double					m_MeterLow[4][4];			//校准时仪表读数
	double					m_MeterHigh[4][4];			//校准时仪表读数
	float					m_InputGlassLow[4][4];		//校准时水银温度计的读数
	float					m_InputGlassHigh[4][4];		//校准时水银温度计的读数
	float					m_WriteAdjustLow[4][4];		//校准后写入仪表的修正值
	float					m_WriteAdjustHigh[4][4];	//校准后写入仪表的修正值
	CONTROL_TEMP_RANGE		m_TempRange;
    virtual ~CWorkMode();

	void CWorkMode::SetTestMode(int TestMode);
	void CWorkMode::EnterAutoTest(CWnd * ParentWnd,int BoxNr,float CurTemp[4],SWITCH_CONFIG_PARA	SwitchConfigPara);
	SUB_MODE CWorkMode::AutoTestStep_DirectHeat(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	SUB_MODE CWorkMode::AuotTestStep_NormalPID(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	SUB_MODE CWorkMode::AuotTestStep_NormalPID_2(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	SUB_MODE CWorkMode::AutoTestStep_Ready(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_StepPID(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_StepPID_2(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::OpenDoor_For_OpenFlashTest(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::OpenDoor_For_CloseFlashTest(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::ExitAutoTest(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara,CString barcode);

	bool CWorkMode::IsAllOpened(int BoxNr);
	bool CWorkMode::IsAllClosed(int BoxNr);
	void CWorkMode::ReadSwitchConfigPara(CWnd * ParentWnd,int BoxNr);
	void CWorkMode::Timer(CWnd * ParentWnd,int BoxNr,float CurTemp[4],SWITCH_CONFIG_PARA	SwitchConfigPara);
	static void CWorkMode::Timer_Sorter(void);
	void CWorkMode::Timer_Cal(CWnd * ParentWnd,int BoxNr,float CurTemp[4]);
	bool CWorkMode::IsPT100Caled(int BoxNr);
	
	void CWorkMode::AutoTestStep_DircetCool(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_StepCool(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_PidCool(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_HeatAgain(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_TopPID(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	float CWorkMode::GetHighestOpenTemp(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	int CWorkMode::GetHighestCloseTemp(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	int CWorkMode::GetLowestOpenTemp(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::OnClickStartButton(CWnd * ParentWnd,int BoxNr,float CurTemp[4],SWITCH_CONFIG_PARA	SwitchConfigPara);	
	void CWorkMode::DealStatusNormalClose(CWnd * ParentWnd,int BoxNr,int SwitchNr,SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status,float m_PidTemp,
												bool m_DoorIsOpenned,int TestMode, bool bSwitchMustBeAdjusted);
	void CWorkMode::DealStatusNormalOpen(int BoxNr,int SwitchTestID,SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status,float m_PidTemp,
												bool m_DoorIsOpenned,int TestMode);
	void CWorkMode::DealStatus_DiscOpenTemp(int BoxNr,DISC_RECORD *BiMetalRecord,unsigned char SoundVal,unsigned char MaxSoundVal,int SameCounts,int DiscID,float PidTemp,CONTROL_TEMP_RANGE	TempRange);
	void CWorkMode::DealStatus_DiscCloseTemp(int BoxNr,DISC_RECORD *BiMetalRecord,unsigned char SoundVal,unsigned char MaxSoundVal,int SameCounts,int DiscID,float PidTemp,CONTROL_TEMP_RANGE	TempRange);

	void CWorkMode::DealStatus_Disc(int BoxNr,MODBUS_RS485_DISC_ANSWER *answer_cmd,float m_PidTemp);
	void CWorkMode::SaveTestRecord(int BoxNr,CString barcode);
	int  CWorkMode::StartAirControl(CWnd * ParentWnd,int BoxNr,float CurTemp);
	bool CWorkMode::GetOpenMode(void);
	bool CWorkMode::GetCloseMode(void);
	void CWorkMode::InitSwitchStatus(int BoxNr);

	void CWorkMode::Send_Better_MES(RECORD *record);

	void CWorkMode::AutoTestStep_CoolFirst_Ready(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_CoolFirst_DirectCool(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_CoolFirst_CoolPid(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_CoolFirst_StepCoolPid(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_CoolFirst_HeatPid(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	void CWorkMode::AutoTestStep_CoolFirst_StepHeatPid(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara);
	int CWorkMode::StopAirControl(CWnd * ParentWnd,int BoxNr,float CurTemp);
	static void CWorkMode::DisplaySendCmd(CWnd * ParentWnd,unsigned char cmd,unsigned char *cmd_buf,int len,int BoxNr);

	static void CWorkMode::TempCmd_128(CWnd * ParentWnd,int MeterID);
	static void CWorkMode::TempCmd_Disc(CWnd * ParentWnd,int MeterID);
	static void CWorkMode::TempCmd_300(CWnd * ParentWnd,int MeterID);
	static void CWorkMode::TempCmd_192(CWnd * ParentWnd,int MeterID) ;
	static void CWorkMode::TempCmd_32PT100(CWnd * ParentWnd,int MeterID);
	static void CWorkMode::TempCmd_TX300W_POWER(CWnd * ParentWnd,int MeterID);

	static void CWorkMode::Send_MES(RECORD record,bool bMsg);

	bool CWorkMode::CheckTempStable(CWnd * ParentWnd,int BoxNr,float CurTemp);
	void CWorkMode::AutoTestStep_CalLow(CWnd * ParentWnd,int BoxNr,float CurTemp[4]);
	void CWorkMode::AutoTestStep_CalHigh(CWnd * ParentWnd,int BoxNr,float CurTemp[4]);
	void CWorkMode::InputGlassLowTemp(CWnd * ParentWnd,int BoxNr);
	void CWorkMode::InputGlassHighTemp(CWnd * ParentWnd,int BoxNr);
	void CWorkMode::EnterCalMode(CWnd * ParentWnd,int BoxNr);
	void CWorkMode::CalAdjustTemp(CWnd * ParentWnd,int BoxNr);

	bool CWorkMode::CheckSameSound2Continue(int BoxNr,int DiscID);


	void CWorkMode::DealCmdAnswer(CWnd * ParentWnd,int BoxNr,unsigned char *para, unsigned char *flash,bool IsCmdAscii,int data_len = 0);
};

#endif