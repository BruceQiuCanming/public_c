#ifndef _BMP_H_
#define _BMP_H_
#include "typedefs.h"

BOOL CreateBMPFile(HWND hwnd, LPTSTR pszFile, HBITMAP hBMP, HDC hDC);
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
BOOL WriteBmp(CString strFile,BYTE *vtData,DWORD dataLen,const SIZE &sizeImg);
BOOL WriteBmp(CString strFile,HDC hdc);
BOOL WriteBmp(CString strFile,HDC hdc,CRect rcDC);
void DrawTempFlat(int BoxNr,CDC *dc,SWITCH_TEST_RESULT  *pSwitchTestResult ,SWITCH_CONFIG_PARA *pSwitchConfigPara ,CRect ChartRect,CTime BeginTestTime, bool IsOpenTemp,BOX_TYPE BoxType,BOOL FontDirection);

void PrintReport_bmp(CDC *dc,CRect page_rect,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,BOX_TYPE BoxType,int CurGroupNr);

#endif