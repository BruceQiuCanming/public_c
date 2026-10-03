#ifndef  _COMMON_H_
#define _COMMON_H_
#include "typedefs.h"
#include "comm.h"
#include "TempMeter.h"

extern unsigned short UNICODE_FILE_HEAR;// = 0xFEFF;
extern unsigned short CSV_COMMMA;//		 = 0x0009;

extern CTempArray								G_TempArray[MAX_HEAT_BOX];
extern CTempArray_8TEMP_8SSR					G_TempArray_8TEMP_8SSR[MAX_HEAT_BOX];
extern CTempMeter								G_Meter[MAX_HEAT_BOX];
extern int										G_iMaxLedNr[MAX_HEAT_BOX+1];
extern int										G_iLedLines[MAX_HEAT_BOX+1];
extern int										G_iLedNrPerLine[MAX_HEAT_BOX+1];
extern CIntArray								G_ErrJiaJu;
extern unsigned int								group[48];
extern NORMAL_CONFIG_PARA						G_NormalConfigPara[MAX_HEAT_BOX];
extern SWITCH_TEST_RESULT						G_SwitchTestResult[MAX_HEAT_BOX][MAX_LED_NR];
extern float									G_AdjustTemp[MAX_HEAT_BOX][2][192];
extern CString									theAppDirectory;
extern CStringArray								G_SpeechArray;
extern INTERFACE_PCB_TYPE_NAME					G_InterfacePcbTypeName[34];
extern int										G_CurMaxHeatBox;
extern PID										G_PID[MAX_HEAT_BOX];
extern WORK_STATUS								G_WorkStatus[MAX_HEAT_BOX];
extern NOISE									G_Noise;
extern int										G_iHighHighLevelDelta;
extern int										G_Group[MAX_HEAT_BOX][MAX_LED_NR];
extern CBiMetalRecordArray						G_BiMetalRecordArray[MAX_HEAT_BOX][40];
extern CString									G_sCompany ;
extern TOPBAND_VIETNAM_MES_CONFIG				G_Topband_Vietnam_Config;

extern bool CheckJiaJuPass(CWnd *wnd,unsigned char ID,int BoxNr);
extern void DrawLED_JiaJu(CWnd *Wnd,CRect LedRect,int BoxNr);

extern void TongJi(TONG_JI_INFO * TongJiData,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,BOX_TYPE BoxType,int BoxNr,int CurGroupNr);

extern HEAT_COOL_ORDER	CheckHeatOrCoolMode(int BoxNr,SWITCH_CONFIG_PARA  ConfigPara,CONTROL_TEMP_RANGE *temp_range,BOX_TYPE BoxType/*,int BoxNr*/);

extern TEST_RESULT_LEVEL CheckTestLevel(SWITCH_CONFIG_PARA SwitchConfigPara,
			SWITCH_TEST_RESULT SwitchTestResult,
			HEAT_COOL_ORDER HeatCoolType,
			CONTROL_TEMP_RANGE temp_range,
			SWITCH_TEST_RESULT SwitchTestResult_Ex,
			BOX_TYPE BoxType);
extern TEST_RESULT_LEVEL CheckTestLevel_BOX_128_ONLY_OPENTEMP_TEST(SWITCH_CONFIG_PARA SwitchConfigPara,
			SWITCH_TEST_RESULT SwitchTestResult,HEAT_COOL_ORDER HeatCoolType,CONTROL_TEMP_RANGE temp_range,SWITCH_TEST_RESULT SwitchTestResult_Ex,BOX_TYPE BoxType);

extern float GetControlGoalTemp(SUB_MODE sub_mode,HEAT_COOL_ORDER m_HeatCoolOrder,bool m_bSwitchMustBeAdjusted,CONTROL_TEMP_RANGE m_TempRange,int BoxNr);

extern 	float GetMaxOpenTempAdjust(int BoxNr);

extern float GetLowestCloseTemp(int BoxNr);

extern CString GetLevelName(TEST_RESULT_LEVEL level);

extern void PrintReport_Ticket(CDC *dc,CRect rect,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,BOX_TYPE BoxType,int CurGroupNr);

extern void PrintReport(CDC *dc,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,BOX_TYPE BoxType,int CurGroupNr);

extern void GetZhengTaiFengBuString(int BoxNr,SWITCH_CONFIG_PARA_ALL ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,int group_id,BOX_TYPE BoxType);

extern void PrintReport_ByGroup_Ticket(CDC *dc,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA_ALL ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,BOX_TYPE BoxType,int GroupNr);

extern void PrintReport_ByGroup(CDC *dc,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA_ALL ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,BOX_TYPE BoxType,int GroupNr);
extern void PrintReport_ByGroup_2(CDC *dc,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,BOX_TYPE BoxType);

extern void GetPrintStringByGroup(int BoxNr,SWITCH_CONFIG_PARA_ALL ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOX_TYPE BoxType);

extern int  CopyAndCreatShortCut(void);
extern BOOL CreateFileShortcut(TCHAR* lpszFileName, TCHAR* lpszLnkFileDir, TCHAR* lpszLnkFileName,       TCHAR* lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd);

extern BOOL GetDesktopPath(TCHAR *pszDesktopPath);

extern BOOL GetProgramFilePath(TCHAR *pszDesktopPath);

extern BOOL GetIEQuickLaunchPath(TCHAR *pszIEQueickLaunchPath);

extern BOOL GetProgramsPath(TCHAR *pszProgramsPath);

//extern CString CreateCurDateDirection(int BoxNr);
extern CString CreateCurDateDirection(int BoxNr,int year = 0,int month = 0,int day = 0,int hour = 0,int minute = 0,int second = 0);
extern CString GetDateDirection(int m_BoxNr,CTime t);

extern void DealStatusNormalOpen(unsigned char SubMode,int SwitchTestID,SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status,float m_PidTemp,float m_CloseDoorStartTemp,
												bool m_DoorIsOpenned,int m_SelectedWorkMode,int BoxNr,CTime CurModeBeginTime);

extern void DealStatusNormalClose(unsigned char SubMode,int SwitchNr,SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status,float m_PidTemp,float m_CloseDoorStartTemp,
												bool m_DoorIsOpenned,int TestMode,int BoxNr,CTime CurModeBeginTime, bool bSwitchMustBeAdjusted = false);

extern CTime  GetRecordTime(long seconds);

extern long  GetSpanSeconds(CTime t);

extern void SaveNormalConfigPara(void);

extern void ReadNormalConfigPara(void);

extern void SaveBmpFile(CWnd* wnd,CString file_name,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA_ALL ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,BOX_TYPE BoxType,int CurGroupNr);

extern void Group2Char(unsigned char *source, unsigned char *dest);

extern void SaveRecord2ExcelCSV(RECORD record,int BoxNr,CString file_name);
extern void SaveRecord2ExcelCSV_CE(RECORD record,int BoxNr,CString file_name);
extern void SaveRecord2ExcelCSV_KSD302_CE(RECORD record,int BoxNr,CString file_name);
extern void SendExcelCSV_2_PC(CString file_name,int year,int month,int day,int hour,int minute,int second);

extern long GetTotalMinutesFromSTD_TIME(CTime cur);

extern  CTime GetTimeFromTotalMinues(long total_minutes);
extern BOOL FreeMyData(LPCTSTR ResName, LPCTSTR ResType, /*LPCTSTR FileName,*/ int Type,unsigned char *HexBuf,int *HexBufLen);
extern unsigned int MyAfxMessageBox(CString Msg,unsigned int type);
extern unsigned int MyAfxMessageBox(CString Msg);
#endif