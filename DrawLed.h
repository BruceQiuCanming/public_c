
#ifndef _DRAW_LED_H_
#define _DRAW_LED_H_

extern unsigned char						G_JiaJuTested[192];
extern bool									G_JiaJuTestedPass[3];
extern bool									G_bJiajuTesting;

void DrawDoubleMetalLED(CWnd *Wnd,CRect LedRect,int BoxNr,unsigned char* m_DoubleMetal);void DrawLED_128_OLD_DISPLAY(CWnd *Wnd,CRect LedRect,int BoxNr);
void DrawDoubleMetalLED(CWnd *Wnd,CRect LedRect,int BoxNr,unsigned char* m_DoubleMetal);
void DrawLED_JiaJu(CWnd *Wnd,CRect LedRect,int BoxNr);
void DrawLED_OilBaths(CWnd *Wnd,CRect LedRect,int BoxNr,int DebugMode);
void DrawLED(CWnd *Wnd,CRect LedRect,int BoxNr,int DebugMode);
void DrawTempCurve(CWnd *Wnd,CRect ChartRect,CTempArray* m_TempArray,CTime m_BeginTestTime);
void DrawTempCurve(CWnd *Wnd,CRect ChartRect,C20Temp5SsrAnswerArray* m_TempArray,CTime m_BeginTestTime);
void DrawTempCurve(CWnd *Wnd,CRect ChartRect,CTempArray_8TEMP_8SSR* m_TempArray,CTime m_BeginTestTime);
#ifdef WINCE
	void DrawLED_BiMetal(CWnd *parentWnd,int BoxNr,SWITCH_CONFIG_PARA  *m_ConfigPara,CRect* m_LED_Rect,BOX_TYPE box_type);
#else
	void DrawLED_BiMetal(CWnd *parentWnd,int BoxNr,CRect AllRect,SWITCH_CONFIG_PARA  *m_ConfigPara,CRect* m_LED_Rect,BOX_TYPE box_type);
#endif
#endif