#include "stdafx.h"
#include "typedefs.h"
#include "common.h"
#include "comm.h"
#include "LPM260_Printer.h"
#include <shlwapi.h>
#include "HeatBoxDlg.h"
#include "DrawLed.h"
#include <math.h>

unsigned char							G_JiaJuTested[192];
bool									G_JiaJuTestedPass[3];
bool									G_bJiajuTesting;
CIntArray G_ErrJiaJu;

static CFont G_NrFont;
static CFont G_TempFont;
extern NOISE				G_Noise;

unsigned int group[48] = 
		{
			0 , 1, 2, 3, 4, 5, 6, 7,
			16,17,18,19,20,21,22,23,
			32,33,34,35,36,37,38,39,
			48,49,50,51,52,53,54,55,
			64,65,66,67,68,69,70,71,
			80,81,82,83,84,85,86,87
		};

bool CheckJiaJuPass(CWnd *wnd,unsigned char ID,int BoxNr)
{

	

	if(G_iMaxLedNr[BoxNr] == 128)
	{
		if(ID < 16)
		{
			for(int i = 0; i < 64; i++)
			{
				if((i % 16) == ID)
				{
					if(G_SwitchTestResult[BoxNr][i].IsClosing == false)
					{
						if(i < 16)
						{
							if(G_SwitchTestResult[BoxNr][i+16].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+32].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+48].IsClosing == true)
							{
								
								
								G_ErrJiaJu.Add(i); 
#ifndef WINCE							
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									
									::G_SpeechArray.Add (s);
								}
#endif
							}
						}
						else if(i < 32)
						{
							if(G_SwitchTestResult[BoxNr][i-16].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+16].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+32].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
							}
						}
						else if(i < 48)
						{
							if(G_SwitchTestResult[BoxNr][i-32].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i-16].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+16].IsClosing == true)
							{
								G_ErrJiaJu.Add(i);
#ifndef WINCE
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
#endif
							}
						}
						else
						{// < 64
							if(G_SwitchTestResult[BoxNr][i-48].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i-32].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i-16].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
#ifndef WINCE
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
#endif
							}
						}
						return false;
					}
				}
				else
				{
					if(G_SwitchTestResult[BoxNr][i].IsClosing == true)
					{//常亮
						/*if(G_SpeechArray.GetSize() < 10)
						{
							CString s;
							s.Format(_T("%d常亮"),i+1); 
							::G_SpeechArray.Add (s);
						}*/
						return false;
					}
				}
			}
		}
		else
		{
			for(int i = 64; i < 128; i++)
			{
				if((i % 16) == (ID - 16))
				{
					if(G_SwitchTestResult[BoxNr][i].IsClosing == false)
					{
						if(i < 80)
						{
							if(G_SwitchTestResult[BoxNr][i+16].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+32].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+48].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
#ifndef WINCE
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
#endif
							}
						}
						else if(i < 96)
						{
							if(G_SwitchTestResult[BoxNr][i-16].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+16].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+32].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
#ifndef WINCE 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
#endif
							}
						}
						else if(i < 112)
						{
							if(G_SwitchTestResult[BoxNr][i-32].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i-16].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i+16].IsClosing == true)
							{
								G_ErrJiaJu.Add(i);
#ifndef WINCE
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
#endif
							}
						}
						else
						{// < 64
							if(G_SwitchTestResult[BoxNr][i-48].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i-32].IsClosing == true
								&& G_SwitchTestResult[BoxNr][i-16].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
#ifndef WINCE
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
#endif
							}
						}
						return false;
					}
				}
				else
				{
					if(G_SwitchTestResult[BoxNr][i].IsClosing == true)
					{
						/*if(G_SpeechArray.GetSize() < 10)
						{
							CString s;
							s.Format(_T("%d常亮"),i+1); 
							::G_SpeechArray.Add (s);
						}*/
						return false;
					}
				}
			}
		}
		G_JiaJuTested[ID] = 1;

		if(G_JiaJuTestedPass[0] == false)
		{
			bool pass = true;
			for(int i = 0; i < 16; i++)
			{
				if(G_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (G_JiaJuTestedPass[1] == false))
			{
				G_JiaJuTestedPass[0] = true;
#ifndef WINCE
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("1到64夹具测试成功!!!"));
				}
#endif
			}
		}
		if(G_JiaJuTestedPass[1] == false) 
		{
			bool pass = true;
			for(int i = 16; i < 32; i++)
			{
				if(G_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (G_JiaJuTestedPass[0] == false))
			{
				G_JiaJuTestedPass[1] = true;
#ifndef WINCE
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("65到128夹具测试成功!!!"));
				}
#endif
			}
		}

		for(int i = 0; i < 32; i++)
		{
			if(G_JiaJuTested[i] != true)
			{
				return false;
			}
		}
#ifndef WINCE
		if(G_SpeechArray.GetSize() < 10)
		{
			::G_SpeechArray.Add (_T("1到128夹具测试成功!!!"));
		}
#endif

		if(G_NormalConfigPara[BoxNr].bSpeech == false)
		{
			AfxMessageBox(_T("1到128夹具测试成功!!!"));
		}

		G_bJiajuTesting = false;

	}
	else if(G_iMaxLedNr[BoxNr] == 192)
	{
		

			if(G_SwitchTestResult[BoxNr][group[ID]].IsClosing == false)
			{
				return false;
			}
			if(G_SwitchTestResult[BoxNr][group[ID]+8].IsClosing == false)
			{
				return false;
			}
			if(G_SwitchTestResult[BoxNr][group[ID]+96].IsClosing == false)
			{
				return false;
			}
			if(G_SwitchTestResult[BoxNr][group[ID]+104].IsClosing == false)
			{
				return false;
			}
		
		int all_closed = 0;
		for(int i = 0; i < G_iMaxLedNr[BoxNr]; i++)
		{
			if(G_SwitchTestResult[BoxNr][i].IsClosing == true)
			{
				all_closed++;
			}
		}
		if(all_closed > 4)
		{
#ifndef WINCE
			CString s;
			s.Format(_T("%d, %d, %d, %d错误"),group[ID],group[ID]+8,group[ID]+96,group[ID]+104); 
			if(G_SpeechArray.GetSize() < 10)
			{
				::G_SpeechArray.Add (s);
			}
#endif
			return false;
		}
		
		G_JiaJuTested[ID] = 1;

		if(G_JiaJuTestedPass[0] == false)
		{
			bool pass = true;
			for(int i = 0; i < 16; i++)
			{
				if(G_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (G_JiaJuTestedPass[1] == false || G_JiaJuTestedPass[2] == false))
			{
				G_JiaJuTestedPass[0] = true;
#ifndef WINCE
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("1到64夹具测试成功!!!"));
				}
#endif
			}
		}
		if(G_JiaJuTestedPass[1] == false) 
		{
			bool pass = true;
			for(int i = 16; i < 32; i++)
			{
				if(G_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (G_JiaJuTestedPass[0] == false || G_JiaJuTestedPass[2] == false) )
			{
				G_JiaJuTestedPass[1] = true;
#ifndef WINCE
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("65到128夹具测试成功!!!"));
				}
#endif
			}
		}

		if(G_JiaJuTestedPass[2] == false) 
		{
			bool pass = true;
			for(int i = 32; i < 48; i++)
			{
				if(G_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (G_JiaJuTestedPass[0] == false || G_JiaJuTestedPass[1] == false))
			{
				G_JiaJuTestedPass[2] = true;
#ifndef WINCE
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("129到196夹具测试成功!!!"));
				}
#endif
			}
		}
		if(::G_iMaxLedNr[BoxNr] == 128)
		{
			for(int i = 0; i < 32; i++)
			{
				if(G_JiaJuTested[i] != 1)
				{
					return false;
				}
			}
#ifndef WINCE
			if(G_SpeechArray.GetSize() < 10)
			{
				::G_SpeechArray.Add (_T("1到128夹具测试成功!!!"));
			}
#endif
			if(G_NormalConfigPara[BoxNr].bSpeech == false)
			{
				AfxMessageBox(_T("1到128夹具测试成功!!!"));
			}
			G_bJiajuTesting = false;
		}
		else
		{
			for(int i = 0; i < 48; i++)
			{
				if(G_JiaJuTested[i] != 1)
				{
					return false;
				}
			}
#ifndef WINCE
			if(G_SpeechArray.GetSize() < 10)
			{
				::G_SpeechArray.Add (_T("1到192夹具测试成功!!!"));
			}
#endif
			if(G_NormalConfigPara[BoxNr].bSpeech == false)
			{
				AfxMessageBox(_T("1到192夹具测试成功!!!"));
			}
			G_bJiajuTesting = false;
		}
		
	}
	return true;
}


void DrawLED(CWnd *Wnd,CRect LedRect,int BoxNr,int DebugMode)
{

	int i,j,k;
	CFile log_file;


	if(!Wnd->IsWindowVisible()) 
	{
		return;
	}

	if(::G_NormalConfigPara[0].BoxType == BOX_OIL_BATHS)
	{
		DrawLED_OilBaths(Wnd,LedRect, BoxNr, DebugMode);

		return;
	}

#ifndef WINCE	
	if(G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_JIAJU_TEST)
	{
		G_iMaxLedNr[BoxNr]		= 128;
		G_iLedLines[BoxNr]		= 8;
		G_iLedNrPerLine[BoxNr] = 16;
	}
#endif	

	if(G_NormalConfigPara[BoxNr].BoxType == BOX_NTC)
	{
		G_iMaxLedNr[BoxNr]		= 64;
		G_iLedLines[BoxNr]		= 8;
		G_iLedNrPerLine[BoxNr]  = 8;
	}


	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 
	 CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(0,0,0)));
	  CBrush yellowbrush(COLORREF(RGB(0xFF,0xFF,0)));

	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc=Wnd->GetWindowDC();
		

	if(dc == NULL)
	{
		
		return;
	}
	
	
	CRect wndRect = LedRect;
	nWidth	= wndRect.Width();
	nHeight = wndRect.Height();//wndRect.top -TEMP_STRING_HEIGHT;

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		
		Wnd->ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			
			Wnd->ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth * MAX_HEAT_BOX,nHeight))	//建立与显示设备兼容的位图
	{
		Wnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int Ellipse_W = (nWidth - PEN_WEIGHT *2) / G_iLedNrPerLine[BoxNr] - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT *2) / G_iLedLines[BoxNr] - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	if(Ellipse_W < Ellipse_H)
	{
		Ellipse_H = Ellipse_W;
	}
	else
	{
		Ellipse_W = Ellipse_H;
	}

	int Ellipse_H_Space = nHeight / G_iLedLines[BoxNr] - Ellipse_H - 1;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	
	MemDC.RoundRect(PEN_WEIGHT,PEN_WEIGHT,nWidth-PEN_WEIGHT*3,nHeight-PEN_WEIGHT*5 ,10,10);
	

	MemDC.SelectObject(&graybrush);				//选择画刷
	j = 0 ;
#ifndef _HANJIE_192
	//for(j=0;j<MAX_HEAT_BOX;j++)
	{
		for(i=0; i < G_iMaxLedNr[BoxNr] /* MAX_HEAT_BOX*/; i++)
		{//默认的全部是灰色
					rect.left = nWidth * j + (i % G_iLedNrPerLine[BoxNr])*(Ellipse_W_GAP+Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
					rect.top = 5+(i / G_iLedNrPerLine[BoxNr])*(Ellipse_H + Ellipse_H_Space)+Ellipse_H_Space / 2;
					rect.right = rect.left + Ellipse_W;
					rect.bottom = rect.top + Ellipse_H;
					if(G_SwitchTestResult[BoxNr][i].IsClosing == false)
					{
						MemDC.Ellipse(rect);		//填充背景
					}
		
		}
	}	
#endif

	


	MemDC.SelectObject(&redbrush);				//选择画刷
	k = 0;
	{
		for(i=0; i< G_iLedNrPerLine[BoxNr]; i++)
		{
			for(j=0; j < G_iLedLines[BoxNr]; j++)
			{
				if((j * G_iLedNrPerLine[BoxNr] + i) >= G_iMaxLedNr[BoxNr])
				{
				}
				else
				{
#ifdef _HANJIE_192
					if(G_SwitchTestResult[BoxNr][11 - i + j * G_iLedNrPerLine[BoxNr] ].IsClosing)
#else
					if(G_SwitchTestResult[BoxNr][     i + j * G_iLedNrPerLine[BoxNr] ].IsClosing)
#endif
					
					{//闭合就是红色
							/*if(G_SwitchTestResult[BoxNr][     i + j * G_iLedNrPerLine[BoxNr] ].IsNoPTC == true 
								)
							{
								MemDC.SelectObject(&yellowbrush);
								rect.left = nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
								rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
								rect.right = rect.left + Ellipse_W;
								rect.bottom = rect.top + Ellipse_H;
								MemDC.Ellipse(rect);
								MemDC.SelectObject(&redbrush);
							}
							else*/
							{
								rect.left = nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
								rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
								rect.right = rect.left + Ellipse_W;
								rect.bottom = rect.top + Ellipse_H;
								MemDC.Ellipse(rect);		//填充背景
							}

					}
					else
					{
						if((::G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_PTC) && (DebugMode != 0))
						{
							if((G_SwitchTestResult[BoxNr][     i + j * G_iLedNrPerLine[BoxNr] ].IsNoPTC == true )
								/*&& (m_LedFlash == true)*/)
							{
								MemDC.SelectObject(&graybrush );
								rect.left = nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
								rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
								rect.right = rect.left + Ellipse_W;
								rect.bottom = rect.top + Ellipse_H;
								MemDC.Ellipse(rect);
								MemDC.SelectObject(&redbrush);
							}
							else
							{
								MemDC.SelectObject(&yellowbrush);
								rect.left = nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
								rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
								rect.right = rect.left + Ellipse_W;
								rect.bottom = rect.top + Ellipse_H;
								MemDC.Ellipse(rect);
								MemDC.SelectObject(&redbrush);
							}
						}
						
						
					}
				}
			}
		}
	}

#ifndef WINCE
	if(G_iMaxLedNr[BoxNr] == 192 && (::G_NormalConfigPara[BoxNr].BoxType != HEAT_BOX_TYPE_128_OLD_DISPLAY)  )
	{
						
			k = 0;
			{
				for(i=0; i < G_iMaxLedNr[BoxNr]; i++)
				{
						CString nr;

						
						if(((i + 1) % G_iLedNrPerLine[BoxNr]) == 0)
						{
							nr.Format(_T("%dB"),(i + 1) / G_iLedNrPerLine[BoxNr] );
						}
						else if(((i + 1) % G_iLedNrPerLine[BoxNr]) < 10)
						{
							nr.Format(_T("%d%d"),(i+1)/G_iLedNrPerLine[BoxNr],(i+1)%G_iLedNrPerLine[BoxNr]);
						}
						else if(((i + 1) % G_iLedNrPerLine[BoxNr]) == 10)
						{
							nr.Format(_T("%d0"),(i+1)/G_iLedNrPerLine[BoxNr]+1);
						}
						else if(((i + 1) % G_iLedNrPerLine[BoxNr]) == 11)
						{
							nr.Format(_T("%dA"),(i + 1) / G_iLedNrPerLine[BoxNr] + 1);
						}
						

						
						//nr.Format("%02d%c",((i+1) / G_iLedNrPerLine[BoxNr]), nr_low[(i % G_iLedNrPerLine[BoxNr])]);
						CSize size = MemDC.GetTextExtent(nr);
						MemDC.TextOut(5 + nWidth * k + (i % G_iLedNrPerLine[BoxNr]) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine[BoxNr])*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
				}
	
		}
	}
	else
#endif
	{
		
		k = 0;

		CFont font;
		int font_size = Ellipse_H * 10 * 0.4;
		if(font.CreatePointFont(font_size,_T("黑体"),&MemDC))
		{
		
			CFont *oldfont = MemDC.SelectObject(&font); 
			for(i=0; i < G_iMaxLedNr[BoxNr]; i++)
			{
					CString nr;
					nr.Format(_T("%02d"),i+1);
					CSize size = MemDC.GetTextExtent(nr);
#ifdef WINCE

					MemDC.DrawText(nr,CRect(5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,size.cx,size.cy),DT_RIGHT | DT_VCENTER);
#else
					if(G_NormalConfigPara[BoxNr].BoxType == BOX_NTC)
					{
						MemDC.TextOut(5 + nWidth * k + (i % G_iLedNrPerLine[BoxNr]) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine[BoxNr])*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 4 +(Ellipse_H-size.cy)/2,nr);
						
						nr.Format(_T("%.3f"),G_SwitchTestResult[BoxNr][i].OpenTemp);
						MemDC.TextOut(5 + nWidth * k + (i % G_iLedNrPerLine[BoxNr]) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 /*+(Ellipse_W-size.cx)/2*/,	5 + (i / G_iLedNrPerLine[BoxNr])*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space *3 / 4 +(Ellipse_H-size.cy)/2,nr);
						
					}
					else
					{
						MemDC.TextOut(5 + nWidth * k + (i % G_iLedNrPerLine[BoxNr]) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine[BoxNr])*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
					}
#endif					
					
			}

			MemDC.SelectObject(oldfont); 
		
			font.DeleteObject();
		}	
	}


	dc->BitBlt(LedRect.left ,LedRect.top + PEN_WEIGHT * 5 ,LedRect.Width() ,LedRect.Height() ,&MemDC,0,0,SRCCOPY); 	//显示原图形


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	Wnd->ReleaseDC(dc); 
}

void DrawLED_OilBaths(CWnd *Wnd,CRect LedRect,int BoxNr,int DebugMode)
{

	int i,j,k;
	CFile log_file;


	if(!Wnd->IsWindowVisible()) 
	{
		return;
	}

	//G_iMaxLedNr[BoxNr]		= 60;
	

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 
	 CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(0,0,0)));
	  CBrush yellowbrush(COLORREF(RGB(0xFF,0xFF,0)));

	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc=Wnd->GetWindowDC();
		

	if(dc == NULL)
	{
		
		return;
	}
	
	
	CRect wndRect = LedRect;
	nWidth	= wndRect.Width();
	nHeight = wndRect.Height();//wndRect.top -TEMP_STRING_HEIGHT;

	CPoint Circle_center(nWidth/2,nHeight/3);



	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		
		Wnd->ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			
			Wnd->ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth * MAX_HEAT_BOX,nHeight))	//建立与显示设备兼容的位图
	{
		Wnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int Ellipse_W = (nWidth - PEN_WEIGHT *2) / G_iLedNrPerLine[BoxNr] - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT *2) / G_iLedLines[BoxNr] - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	if(Ellipse_W < Ellipse_H)
	{
		Ellipse_H = Ellipse_W;
	}
	else
	{
		Ellipse_W = Ellipse_H;
	}

	int Ellipse_H_Space = nHeight / G_iLedLines[BoxNr] - Ellipse_H - 1;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	
	MemDC.RoundRect(PEN_WEIGHT,PEN_WEIGHT,nWidth-PEN_WEIGHT*3,nHeight-PEN_WEIGHT*5 ,10,10);
	

	MemDC.SelectObject(&graybrush);				//选择画刷

	

	int x,y;
	int r = nHeight / 5;
	
	//MemDC.Ellipse(Circle_center.x-10,Circle_center.y-10,Circle_center.x+10,Circle_center.y+10);

	for(int i = 0; i < 10; i++)
	{
		x = Circle_center.x + r * cos((-15.0f+(9-i)*210.0f/9)/180*3.1415926);
		y = Circle_center.y + r * sin((-15.0f+(9-i)*210.0f/9)/180*3.1415926);

		if(G_SwitchTestResult[BoxNr][i].IsClosing)
		{

		}
		else
		{
			MemDC.Ellipse(x-20,y-20,x+20,y+20);
		}
	}

	r = nHeight / 5 * 2;
	for(int i = 0; i < 20; i++)
	{
		if(G_SwitchTestResult[BoxNr][i+10].IsClosing)
		{

		}
		else
		{
			x = Circle_center.x + r * cos((-15.0f+(19-i)*210.0f/19)/180*3.1415926);
			y = Circle_center.y + r * sin((-15.0f+(19-i)*210.0f/19)/180*3.1415926);

			MemDC.Ellipse(x-20,y-20,x+20,y+20);
		}
	}

	r = nHeight / 5 * 3;
	for(int i = 0; i < 30; i++)
	{
		if(G_SwitchTestResult[BoxNr][i+30].IsClosing)
		{

		}
		else
		{
			x = Circle_center.x + r * cos((-15.0f+(29-i)*210.0f/29)/180*3.1415926);
			y = Circle_center.y + r * sin((-15.0f+(29-i)*210.0f/29)/180*3.1415926);

			MemDC.Ellipse(x-20,y-20,x+20,y+20);
		}
	}

	MemDC.SelectObject(&redbrush);				//选择画刷

	r = nHeight / 5 * 1;
	for(int i = 0; i < 10; i++)
	{
		if(G_SwitchTestResult[BoxNr][i].IsClosing)
		{
			x = Circle_center.x + r * cos((-15.0f+(9-i)*210.0f/9)/180*3.1415926);
			y = Circle_center.y + r * sin((-15.0f+(9-i)*210.0f/9)/180*3.1415926);
			MemDC.Ellipse(x-20,y-20,x+20,y+20);
		}
	}

	r = nHeight / 5 * 2;
	for(int i = 0; i < 20; i++)
	{
		if(G_SwitchTestResult[BoxNr][i+10].IsClosing)
		{
			x = Circle_center.x + r * cos((-15.0f+(19-i)*210.0f/19)/180*3.1415926);
			y = Circle_center.y + r * sin((-15.0f+(19-i)*210.0f/19)/180*3.1415926);
			MemDC.Ellipse(x-20,y-20,x+20,y+20);
		}
	}

	r = nHeight / 5 * 3;
	for(int i = 0; i < 30; i++)
	{
		if(G_SwitchTestResult[BoxNr][i+30].IsClosing)
		{
			x = Circle_center.x + r * cos((-15.0f+(29-i)*210.0f/29)/180*3.1415926);
			y = Circle_center.y + r * sin((-15.0f+(29-i)*210.0f/29)/180*3.1415926);
			MemDC.Ellipse(x-20,y-20,x+20,y+20);
		}
	}

	MemDC.SetTextColor(RGB(255,255,255));
	CString nr;
	r = nHeight / 5 * 1;
	for(int i = 0; i < 10; i++)
	{
		//if(G_SwitchTestResult[BoxNr][i].IsClosing == false)
		{
			
			x = Circle_center.x + r * cos((-15.0f+(9-i)*210.0f/9)/180*3.1415926);
			y = Circle_center.y + r * sin((-15.0f+(9-i)*210.0f/9)/180*3.1415926);
			nr.Format(_T("%d"),i+1);
			MemDC.DrawText(nr,CRect(x-20,y-10,x+20,y+10),DT_CENTER | DT_BOTTOM);
		}
	}

	r = nHeight / 5 * 2;
	for(int i = 0; i < 20; i++)
	{
		//if(G_SwitchTestResult[BoxNr][i+10].IsClosing == false)
		{
			
			x = Circle_center.x + r * cos((-15.0f+(19-i)*210.0f/19)/180*3.1415926);
			y = Circle_center.y + r * sin((-15.0f+(19-i)*210.0f/19)/180*3.1415926);
			nr.Format(_T("%d"),i+11);
			MemDC.DrawText(nr,CRect(x-20,y-10,x+20,y+10),DT_CENTER | DT_BOTTOM);
		}
	}

	r = nHeight / 5 * 3;
	for(int i = 0; i < 30; i++)
	{
		//if(G_SwitchTestResult[BoxNr][i+10].IsClosing == false)
		{
			
			x = Circle_center.x + r * cos((-15.0f+(29-i)*210.0f/29)/180*3.1415926);
			y = Circle_center.y + r * sin((-15.0f+(29-i)*210.0f/29)/180*3.1415926);
			nr.Format(_T("%d"),i+31);
			MemDC.DrawText(nr,CRect(x-20,y-10,x+20,y+10),DT_CENTER | DT_BOTTOM);
		}
	}

	dc->BitBlt(LedRect.left ,LedRect.top + PEN_WEIGHT * 5 ,LedRect.Width() ,LedRect.Height() ,&MemDC,0,0,SRCCOPY); 	//显示原图形


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	Wnd->ReleaseDC(dc); 
}

void DrawLED_JiaJu(CWnd *Wnd,CRect LedRect,int BoxNr)
{
	int i,j,k;
	CFile log_file;
	
	for(i = 0; i < G_iMaxLedNr[BoxNr] / 4; i++)
	{
		CheckJiaJuPass(Wnd,i,BoxNr);
	}

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 
	 CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(0,0,0)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc =  Wnd->GetWindowDC();
		

	if(dc == NULL)
	{
		return;
	}
	
	CRect wndRect = LedRect;
	
	nWidth	= wndRect.Width() ;
	nHeight = wndRect.Height();

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		
		Wnd->ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			Wnd->ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth * MAX_HEAT_BOX,nHeight))	//建立与显示设备兼容的位图
	{
		Wnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int Ellipse_W = (nWidth - PEN_WEIGHT *2) / 16 - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT *2) / 8 - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	
	if(Ellipse_W < Ellipse_H)
	{
		Ellipse_H = Ellipse_W;
	}
	else
	{
		Ellipse_W = Ellipse_H;
	}

	int Ellipse_H_Space = nHeight / G_iLedLines[BoxNr] - Ellipse_H - 1;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	//for(k=0;k<G_CurMaxHeatBox;k++)
	k = 0;
	{
		MemDC.RoundRect(PEN_WEIGHT + nWidth * k,PEN_WEIGHT,nWidth-PEN_WEIGHT*3 + nWidth * k + 4,nHeight-PEN_WEIGHT*5 ,10,10);
	}

	MemDC.SelectObject(&graybrush);				//选择画刷
	j = 0 ;
//#ifndef _HANJIE_192
	//for(j=0;j<MAX_HEAT_BOX;j++)
	{
		for(i=0; i < G_iMaxLedNr[BoxNr] ; i++)
		{//默认的全部是灰色
					rect.left =  (i % 16)*(Ellipse_W_GAP+Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
					rect.top = 5+(i / 16)*(Ellipse_H + Ellipse_H_Space)+Ellipse_H_Space / 2;
					rect.right = rect.left + Ellipse_W;
					rect.bottom = rect.top + Ellipse_H;
					if(G_SwitchTestResult[BoxNr][i].IsClosing == false)
					{
						MemDC.Ellipse(rect);		//填充背景
					}
		
		}
	}	
//#endif
	MemDC.SelectObject(&redbrush);				//选择画刷
	

	k = 0;
	{
		for(i=0; i< 16; i++)
		{
			for(j=0; j < G_iMaxLedNr[BoxNr] / 8; j++)
			{
				if((j * 16 + i) >= G_iMaxLedNr[BoxNr])
				{
				}
				else
				{
					if(G_SwitchTestResult[BoxNr][     i + j * 16 ].IsClosing)

					{//闭合就是红色
						rect.left =  nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
						rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
						rect.right = rect.left + Ellipse_W;
						rect.bottom = rect.top + Ellipse_H;
						MemDC.Ellipse(rect);		//填充背景
					}
				}
			}
		}
	}


	

	if(G_iMaxLedNr[BoxNr] == 128)
	{
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr[BoxNr]; i++)
			{
				int id;
				if(i < 64)
				{
					id = (i % 16);
				}
				else
				{
					id = 16 + (i % 16);
				}
				if(G_JiaJuTested[id] ==  0)
				{
					int same = -1;
					
					for(int m = 0; m < G_ErrJiaJu.GetSize(); m++)
					{
						if(G_ErrJiaJu.GetAt(m) == i)
						{
							same = i;
						}
					}
					CString nr;
					if(same == i)
					{
						nr.Format(_T("X%02d"),i+1);
					}
					else
					{
						
						nr.Format(_T("%02d"),i+1);
					}
					CSize size = MemDC.GetTextExtent(nr);
#ifdef WINCE
					MemDC.DrawText(nr,CRect(5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,size.cx,size.cy),DT_RIGHT | DT_VCENTER);
#else
					MemDC.TextOut( 5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
#endif				
				}
				else
				{
					CString nr = _T("√");
					CSize size = MemDC.GetTextExtent(nr);
#ifdef WINCE
					MemDC.DrawText(nr,CRect(5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,size.cx,size.cy),DT_RIGHT | DT_VCENTER);
#else
					MemDC.TextOut( 5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
#endif				
				}
					
			}
		}	
	}
	else //G_iMaxLedNr[BoxNr] == 192
	{
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr[BoxNr]; i++)
			{

				int id = -1;
				for(int j = 0; j < G_iMaxLedNr[BoxNr] / 4; j++)
				{
					if((group[j] == i)
						|| (group[j] == i - 8)
						|| (group[j] == i - 96)
						|| (group[j] == i - 104))
					{
						id = j;
						break;
					}

				}
				
				if(G_JiaJuTested[id] ==  0)
				{
					CString nr;
					nr.Format(_T("%02d"),i+1);
					CSize size = MemDC.GetTextExtent(nr);
#ifdef WINCE
					MemDC.DrawText(nr,CRect(5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,size.cx,size.cy),DT_RIGHT | DT_VCENTER);
#else
					MemDC.TextOut( 5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
#endif				
				}
				else
				{
					CString nr = _T("√");
					CSize size = MemDC.GetTextExtent(nr);
#ifdef WINCE
					MemDC.DrawText(nr,CRect(5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,size.cx,size.cy),DT_RIGHT | DT_VCENTER);
#else
					MemDC.TextOut( 5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
#endif
				}
					
			}
		}	
	}


	dc->BitBlt(LedRect.left ,LedRect.top + PEN_WEIGHT * 5 ,LedRect.Width() ,LedRect.Height() ,&MemDC,0,0,SRCCOPY); 	//显示原图形


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	Wnd->ReleaseDC(dc); 
		return;
  
}
#ifndef WINCE
void DrawLED_128_OLD_DISPLAY(CWnd *Wnd,CRect LedRect,int BoxNr)
{


	//AfxMessageBox(_T("DrawLED_128_OLD_DISPLAY"));
	int AdjustSwitchNr[128] =
	{ 0, 1,  2, 3, 4, 5, 6, 7,104,105,
	  8, 9, 10,11,12,13,14,15,106,107,
	  16,17,18,19,20,21,22,23,108,109,
	  24,25,26,27,28,29,30,31,110,111,
	  32,33,34,35,36,37,38,39,112,113,
	  40,41,42,43,44,45,46,47,114,115,
	  48,49,50,51,52,53,54,55,116,117,
	  56,57,58,59,60,61,62,63,118,119,
	  64,65,66,67,68,69,70,71,120,121,
	  72,73,74,75,76,77,78,79,122,123,
	  80,81,82,83,84,85,86,87,124,125,
	  88,89,90,91,92,93,94,95,126,127,
	  96,97,98,99,100,101,102,103,

	};


	int i,j,k;
	CFile log_file;


	if(!Wnd->IsWindowVisible()) 
	{
		return;
	}

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 
	 CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(0,0,0)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc=Wnd->GetWindowDC();
		

	if(dc == NULL)
	{
		
		return;
	}
	
	CRect wndRect = LedRect;
	nWidth	= wndRect.Width();
	nHeight = wndRect.Height();//wndRect.top -TEMP_STRING_HEIGHT;


	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		
		Wnd->ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			Wnd->ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth * MAX_HEAT_BOX,nHeight))	//建立与显示设备兼容的位图
	{
		Wnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int Ellipse_W = (nWidth - PEN_WEIGHT *2) / G_iLedNrPerLine[BoxNr] - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT *2) / G_iLedLines[BoxNr] - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	if(Ellipse_W < Ellipse_H)
	{
		Ellipse_H = Ellipse_W;
	}
	else
	{
		Ellipse_W = Ellipse_H;
	}

	int Ellipse_H_Space = nHeight / G_iLedLines[BoxNr] - Ellipse_H - 1;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	
	MemDC.RoundRect(PEN_WEIGHT,PEN_WEIGHT,nWidth-PEN_WEIGHT*3 + 4,nHeight-PEN_WEIGHT*5 ,10,10);
	
	MemDC.SelectObject(&graybrush);				//选择画刷
	j = 0 ;
#ifndef _HANJIE_192
	//for(j=0;j<MAX_HEAT_BOX;j++)
	{
		for(i=0; i < G_iMaxLedNr[BoxNr] /* MAX_HEAT_BOX*/; i++)
		{//默认的全部是灰色
					rect.left = nWidth * j + (i % G_iLedNrPerLine[BoxNr])*(Ellipse_W_GAP+Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
					rect.top = 5+(i / G_iLedNrPerLine[BoxNr])*(Ellipse_H + Ellipse_H_Space)+Ellipse_H_Space / 2;
					rect.right = rect.left + Ellipse_W;
					rect.bottom = rect.top + Ellipse_H;
					if(G_SwitchTestResult[BoxNr][AdjustSwitchNr[i]].IsClosing == false)
					{
						MemDC.Ellipse(rect);		//填充背景
					}
		
		}
	}	
#endif



	MemDC.SelectObject(&redbrush);				//选择画刷
	k = 0;
	{
		for(i=0; i< G_iLedNrPerLine[BoxNr]; i++)
		{
			for(j=0; j < G_iLedLines[BoxNr]; j++)
			{
				if((j * G_iLedNrPerLine[BoxNr] + i) >= G_iMaxLedNr[BoxNr])
				{
				}
				else
				{

					if(G_SwitchTestResult[BoxNr][   AdjustSwitchNr[  i + j * G_iLedNrPerLine[BoxNr] ]].IsClosing)
				
					{//闭合就是红色
						if(G_SwitchTestResult[BoxNr][  AdjustSwitchNr[   i + j * G_iLedNrPerLine[BoxNr]] ].IsNoPTC == true 
							/*&& (m_LedFlash == true)*/)
						{
						}
						else
						{
							rect.left = nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
							rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
							rect.right = rect.left + Ellipse_W;
							rect.bottom = rect.top + Ellipse_H;
							MemDC.Ellipse(rect);		//填充背景
						}
					}
				}
			}
		}
	}


	
	
	{
		
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr[BoxNr]; i++)
			{
					CString nr;
					nr.Format(_T("%02d"),i+1);
					CSize size = MemDC.GetTextExtent(nr);
#ifdef	WINCE
					MemDC.DrawText(nr,CRect(5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,size.cx,size.cy),DT_RIGHT | DT_VCENTER);
#else
					MemDC.TextOut(5 + nWidth * k + (i % G_iLedNrPerLine[BoxNr]) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine[BoxNr])*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
#endif
					/*CRect rect;
					rect.left =5+(i % G_iLedNrPerLine[BoxNr]) *(Ellipse_W + Ellipse_W_Space);
					rect.top  =5+(i / G_iLedNrPerLine[BoxNr])*((Ellipse_H + Ellipse_H_Space));
					rect.right = rect.left + Ellipse_W;
					rect.bottom= rect.top + Ellipse_H;
					
					MemDC.DrawText(nr,rect,DT_CENTER | DT_VCENTER);
					*/
					
			}
		}	
	}


	dc->BitBlt(LedRect.left ,LedRect.top + PEN_WEIGHT * 5 ,LedRect.Width() ,LedRect.Height() ,&MemDC,0,0,SRCCOPY); 	//显示原图形


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	Wnd->ReleaseDC(dc); 
}

#endif


COLORREF TEMP_COLOR[8] = 
{
	RGB(0xFF,0x00,0x00),
	RGB(0x00,0xFF,0x00),
	RGB(0x00,0x00,0xFF),
	RGB(0xFF,0xFF,0x00),
	RGB(0xFF,0x00,0xFF),
	RGB(0x00,0xFF,0xFF),
	RGB(0x80,0x80,0x80),
	RGB(0xFF,0xFF,0xFF),
};

CBrush groundbrush(COLORREF(RGB(0,0,0)));
CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
CBrush graybrush(COLORREF(RGB(192,192,192)));

void DrawTempCurve(CWnd *Wnd,CRect ChartRect,CTempArray_8TEMP_8SSR* m_TempArray,CTime m_BeginTestTime)
{
		
	CString start_time;
	start_time = m_BeginTestTime.Format(_T("开始时间 %Y-%m-%d %H:%M:%S")); 

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 
	 
	 CBrush* oldbrush;
	
//

	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	
	//绘制准备
	dc=Wnd->GetDC();
		

	if(dc == NULL)
	{

		return;
	}
	


	
	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{

		Wnd->ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		Wnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
		return;
	
	}
	if(!bitmap.CreateCompatibleBitmap(dc,ChartRect.Width(),ChartRect.Height()))	//建立与显示设备兼容的位图
	{

		Wnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
		return;
	}

	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境


#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	


				

	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
	MemDC.Rectangle(0,0, ChartRect.Width(),ChartRect.Height());


		float max_temp = -999.0f;
		float min_temp = 999.0f;
		float avr_max_temp = -999.0f;
		float avr_min_temp = 999.0f;
		float max_temp_gap = -999.0f;
		float min_temp_gap = 999.0f;

		

		int less200_counts = 0;

		for(int i = 0; i < m_TempArray->GetSize(); i++)
		{
			TEMP_RECORD_8TEMP_8SSR temp_record;
			temp_record = m_TempArray->GetAt(i);
			for(int j = 0; j < 8; j++)
			{
				if(temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f   < min_temp)
				{
					min_temp = temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
				}
				
				if(temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f  > max_temp)
				{
					max_temp = temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
				}

			
			}
			
			if( i > (m_TempArray->GetSize() - 200))
			{//最后200个数据的最大最小值
				
				float avr = 0;
				float max = -999.9f;
				float min =  999.9f;

				
				
				for(int j = 0; j < 8; j++)
				{
					if(temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f   < min)
					{
						min = temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
					}
					
					if(temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f  > max)
					{
						max = temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
					}
					avr += temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;

					
				}
			
				less200_counts ++;
				
				avr /= 8.0f;

				float gap = max - min;
				if(gap > max_temp_gap)
				{
					max_temp_gap = gap;
				}

				if(gap < min_temp_gap)
				{
					min_temp_gap = gap;
				}

				if(avr  < avr_min_temp)
				{
					avr_min_temp = avr ;
				}
				
				if(avr  > avr_max_temp)
				{
					avr_max_temp = avr ;
				}
			}
			
		}
		
		float avr_up = 0;
		float max_up = -999.9f;
		float min_up =  999.9f;

		float avr_down = 0;
		float max_down = -999.9f;
		float min_down =  999.9f;

		TEMP_RECORD_8TEMP_8SSR temp_record;
		temp_record = m_TempArray->GetAt(m_TempArray->GetSize() - 1);

		for(int j = 0; j < 8; j++)
		{

					if( j < 4)
					{
						if(temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f   < min_up)
						{
							min_up = temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
						}
						
						if(temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f  > max_up)
						{
							max_up = temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
						}
						avr_up += temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
					}
					else
					{
						if(temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f   < min_down)
						{
							min_down = temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
						}
						
						if(temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f  > max_down)
						{
							max_down = temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
						}
						avr_down += temp_record.Answer_8TEMP_8SSR.Pv[j]/100.0f;
					}
		}
		avr_up /= 4.0f;
		avr_down /= 4.0f;

		CString s;


		MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));
		MemDC.SetBkMode(TRANSPARENT);
#ifdef WINCE		
		//s.Format(_T(" %5.1f--%5.1f, 最后200秒:%5.1f--%5.1f℃,温场%5.1f--%5.1f"),max_temp,min_temp,avr_max_temp,avr_min_temp,max_temp_gap,min_temp_gap);

		



		s.Format(_T(" %5.1f--%5.1f, 最后200秒:%6.2f--%6.2f℃,温场%5.1f--%5.1f  最新(上 %5.1f--%5.1f)(下 %5.1f--%5.1f)(上下层平均%5.1f-%5.1f=%5.1f)"),
			max_temp,min_temp,
			avr_max_temp,avr_min_temp,
			max_temp_gap,min_temp_gap,
			max_up,min_up,
			max_down,min_down,
			avr_up,avr_down,
			avr_up-avr_down);
			
#else
		//s.Format(_T(" %5.1f--%5.1f, 最后200秒:%5.1f--%5.1f℃,温场%5.1f--%5.1f"),max_temp,min_temp,avr_max_temp,avr_min_temp,max_temp_gap,min_temp_gap);
		
		s.Format(_T(" %5.1f--%5.1f, \r\n最后200秒:%5.1f--%5.1f℃,\r\n温场%5.1f--%5.1f  \r\n最新(上 %5.1f--%5.1f)(下 %5.1f--%5.1f)(上下层平均%5.1f-%5.1f=%5.1f)"),
			max_temp,min_temp,
			avr_max_temp,avr_min_temp,
			max_temp_gap,min_temp_gap,
			max_up,min_up,
			max_down,min_down,
			avr_up,avr_down,
			avr_up-avr_down);
			

	
#endif
	//	s = start_time + s;
		max_temp += 20.0f;
		min_temp -= 20.0f;

		CFont font,*oldFont;
		CSize FontSize;
#ifdef WINCE
		int FontHeight = (ChartRect.Height()*10)/30;
#else
		int FontHeight = (ChartRect.Height()*10) / 80;
#endif
		if(font.CreatePointFont(FontHeight,_T("黑体"),&MemDC))
		{
			oldFont = MemDC.SelectObject(&font);
		}
		else
		{
			return;
		}
	

		 FontSize = MemDC.GetTextExtent(_T("888.8个"));
		float y_gap = (max_temp - min_temp) / 5;
		
		pen.DeleteObject(); 
		pen.CreatePen(PS_DOT,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);

		//MemDC.DrawText(s,CRect(0,0,ChartRect.Width(),FontSize.cx),DT_CENTER);
		s = _T("温度曲线   ") +s;
#ifdef WINCE
		Wnd->SetWindowTextW(s); 
#else
		MemDC.TextOutW(100,20,s);
#endif
		for(int i = 0; i <= 5; i++)
		{
			s.Format(_T("% 5.1f℃"),min_temp + y_gap * i);
			if(i < 5)
			{
				CRect rect;
				rect.left = 0;
				rect.right = FontSize.cx;
				rect.top    =  ChartRect.Height() - ChartRect.Height() * i/ 5 - FontSize.cy;
				rect.bottom =  rect.top + FontSize.cy;
				MemDC.DrawTextW(s,rect,DT_LEFT);
			}
			else
			{
				CRect rect;
				rect.left = 0;
				rect.right = FontSize.cx;
				rect.top =  ChartRect.Height() - ChartRect.Height() * i/ 5;
				rect.bottom =  rect.top + FontSize.cy;
				MemDC.DrawTextW(s,rect,DT_LEFT);
			}

			MemDC.MoveTo( 0, ChartRect.Height()  - ChartRect.Height() * i/ 5);
			MemDC.LineTo(ChartRect.right, ChartRect.Height() - ChartRect.Height() * i/ 5);
		}

		for(int i = 1; i < 10; i++)
		{
			MemDC.MoveTo(ChartRect.Width() * i / 10, 0);
			MemDC.LineTo(ChartRect.Width() * i / 10, ChartRect.Height());
		}

	//	MemDC.TextOutW(100,100,_T("asfs")); 
		float x_gap;


		if(m_TempArray->GetSize() > 0)
		{
			x_gap = (float)(ChartRect.Width()) / (float)(m_TempArray->GetSize()); 
		}
		else
		{
			x_gap = ChartRect.Width() ; 
		}
	
		y_gap = (float)(ChartRect.Height() / (float)(max_temp - min_temp));
		
	
		for(int line = 0; line < 8; line++)
		{

			pen.DeleteObject(); 
			pen.CreatePen(PS_SOLID,PEN_WEIGHT, TEMP_COLOR[line]);  		//建立画笔
			oldpen=MemDC.SelectObject(&pen);
			for(int i = 0; i < m_TempArray->GetSize()-1; i++)
			{
				MemDC.MoveTo((int)(i*x_gap),    (int)(ChartRect.Height() - (m_TempArray->GetAt(i).Answer_8TEMP_8SSR.Pv[line]/100.0f     - min_temp) * y_gap));
			
				MemDC.LineTo((int)((i+1)*x_gap),(int)(ChartRect.Height() - (m_TempArray->GetAt(i+1).Answer_8TEMP_8SSR.Pv[line]/100.0f  - min_temp) *  y_gap));
				 
			}
		}
		
		for(int i = 0; i < 8; i++)
		{
			pen.DeleteObject(); 
			pen.CreatePen(PS_SOLID,PEN_WEIGHT*5, TEMP_COLOR[i]);//RGB(0xFF,0xFF,0));  		//建立画笔
			oldpen=MemDC.SelectObject(&pen);
			MemDC.MoveTo(  0, i * 10 + 20);
			MemDC.LineTo( 30, i * 10 + 20); 
		}
	
	dc->BitBlt(ChartRect.left,ChartRect.top,ChartRect.Width(),ChartRect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形
 


	MemDC.SelectObject(oldbrush);
	
	MemDC.SelectObject(oldFont);
	if(font.GetSafeHandle() != NULL)
	{
		font.DeleteObject();
	}

	
	
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}

	
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}

	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	Wnd->ReleaseDC(dc); 

}

void DrawTempCurve(CWnd *Wnd,CRect ChartRect,CTempArray* m_TempArray,CTime m_BeginTestTime)
{
		
	CString start_time;
	start_time = m_BeginTestTime.Format(_T("开始时间 %Y-%m-%d %H:%M:%S")); 

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 
	 
	 CBrush* oldbrush;
	
//

	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	
	//绘制准备
	dc=Wnd->GetDC();
		

	if(dc == NULL)
	{

		return;
	}
	

	int TempSize = m_TempArray->GetSize();

	
	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{

		Wnd->ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		Wnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
		return;
	
	}
	if(!bitmap.CreateCompatibleBitmap(dc,ChartRect.Width(),ChartRect.Height()))	//建立与显示设备兼容的位图
	{

		Wnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
		return;
	}

	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境


#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	


				

	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
	MemDC.Rectangle(0,0, ChartRect.Width(),ChartRect.Height());


		float max_temp = -999.0f;
		float min_temp = 999.0f;
		float avr_max_temp = -999.0f;
		float avr_min_temp = 999.0f;
		float max_temp_gap = -999.0f;
		float min_temp_gap = 999.0f;
		for(int i = 1; i < m_TempArray->GetSize(); i++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(m_TempArray->GetAt(i).temp[j]   < min_temp)
				{
					min_temp = m_TempArray->GetAt(i).temp[j];
				}
				
				if(m_TempArray->GetAt(i).temp[j]  > max_temp)
				{
					max_temp = m_TempArray->GetAt(i).temp[j];
				}

			
			}
			
			if( i > (m_TempArray->GetSize() - 180))
			{//最后200个数据的最大最小值
				float avr = 0;
				float max = -999.9f;
				float min =  999.9f;
				for(int j = 0; j < 4; j++)
				{
					if(m_TempArray->GetAt(i).temp[j]   < min)
					{
						min = m_TempArray->GetAt(i).temp[j];
					}
					
					if(m_TempArray->GetAt(i).temp[j]  > max)
					{
						max = m_TempArray->GetAt(i).temp[j];
					}
					avr += m_TempArray->GetAt(i).temp[j];
				}

				avr /= 4.0f;

				float gap = max - min;
				if(gap > max_temp_gap)
				{
					max_temp_gap = gap;
				}

				if(gap < min_temp_gap)
				{
					min_temp_gap = gap;
				}

				if(avr  < avr_min_temp)
				{
					avr_min_temp = avr ;
				}
				
				if(avr  > avr_max_temp)
				{
					avr_max_temp = avr ;
				}
			}
			
		}
		
		

		CString s;


		MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));
		MemDC.SetBkMode(TRANSPARENT);
		
		s.Format(_T(" %5.1f--%5.1f"),max_temp,min_temp);

		s = start_time + s;
		max_temp += 20.0f;
		min_temp -= 20.0f;

		CFont font,*oldFont;
		CSize FontSize;
#ifdef WINCE
		int FontHeight = (ChartRect.Height()*10)/30;
#else
		int FontHeight = (ChartRect.Height()*10) / 80;
#endif
		if(font.CreatePointFont(FontHeight,_T("黑体"),&MemDC))
		{
			oldFont = MemDC.SelectObject(&font);
		}
		else
		{
			return;
		}
	

		 FontSize = MemDC.GetTextExtent(_T("888.8个"));
		float y_gap = (max_temp - min_temp) / 5;
		
		pen.DeleteObject(); 
		pen.CreatePen(PS_DOT,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);

		//MemDC.DrawText(s,CRect(0,0,ChartRect.Width(),FontSize.cx),DT_CENTER);
		s = _T("温度曲线 ") +s;
#ifdef WINCE
		Wnd->SetWindowTextW(s); 
#else
		{
			float max_t = -999.9f,min_t = 999.9f;
			int size = m_TempArray->GetSize();
			float cur_temp[4];
			if(size > 0)
			{
				for(int j = 0; j < 4; j++)
				{
					cur_temp[j] = m_TempArray->GetAt(size-1).temp[j];
					if(cur_temp[j]   < min_t)
					{
						min_t = cur_temp[j];
					}
					
					if(cur_temp[j]  > max_t)
					{
						max_t = cur_temp[j];
					}
					
				}
			}
			
			MemDC.TextOutW(ChartRect.right/10,20,s);
			s.Format(_T("最后180秒:%6.2f--%6.2f℃"),avr_max_temp,avr_min_temp);
			MemDC.TextOutW(ChartRect.right/10,40,s);
			s.Format(_T("温场%5.1f--%5.1f（%5.1f,）"),max_temp_gap,min_temp_gap,max_t - min_t);
			MemDC.TextOutW(ChartRect.right/10,60,s);
			s.Format(_T("1#%5.1f,     3#%5.1f"),cur_temp[0],cur_temp[2]);
			MemDC.TextOutW(ChartRect.right/10,80,s);
			s.Format(_T("2#%5.1f,     4#%5.1f"),cur_temp[1],cur_temp[3]);
			MemDC.TextOutW(ChartRect.right/10,100,s);
		}
#endif
		for(int i = 0; i <= 5; i++)
		{
			s.Format(_T("% 5.1f℃"),min_temp + y_gap * i);
			if(i < 5)
			{
				CRect rect;
				rect.left = 0;
				rect.right = FontSize.cx;
				rect.top    =  ChartRect.Height() - ChartRect.Height() * i/ 5 - FontSize.cy;
				rect.bottom =  rect.top + FontSize.cy;
				MemDC.DrawTextW(s,rect,DT_LEFT);
			}
			else
			{
				CRect rect;
				rect.left = 0;
				rect.right = FontSize.cx;
				rect.top =  ChartRect.Height() - ChartRect.Height() * i/ 5;
				rect.bottom =  rect.top + FontSize.cy;
				MemDC.DrawTextW(s,rect,DT_LEFT);
			}

			MemDC.MoveTo( 0, ChartRect.Height()  - ChartRect.Height() * i/ 5);
			MemDC.LineTo(ChartRect.right, ChartRect.Height() - ChartRect.Height() * i/ 5);
		}

		for(int i = 1; i < 10; i++)
		{
			MemDC.MoveTo(ChartRect.Width() * i / 10, 0);
			MemDC.LineTo(ChartRect.Width() * i / 10, ChartRect.Height());
			if(m_TempArray->GetSize() > 0)
			{
				CTime cur = m_TempArray->GetAt(TempSize / 10 * i).time;
				s = cur.Format(_T("%H:%M:%S"));
				MemDC.TextOutW(ChartRect.Width() * i / 10, ChartRect.Height() - 20,s);
			}
			
		}

	//	MemDC.TextOutW(100,100,_T("asfs")); 
		float x_gap;


		if(m_TempArray->GetSize() > 0)
		{
			x_gap = (float)(ChartRect.Width()) / (float)(m_TempArray->GetSize()); 
		}
		else
		{
			x_gap = ChartRect.Width() ; 
		}
	
		y_gap = (float)(ChartRect.Height() / (float)(max_temp - min_temp));
		
	
		for(int line = 0; line < 5; line++)
		{

			pen.DeleteObject(); 
			pen.CreatePen(PS_SOLID,PEN_WEIGHT, TEMP_COLOR[line]);  		//建立画笔
			oldpen=MemDC.SelectObject(&pen);
			for(int i = 0; i < m_TempArray->GetSize()-1; i++)
			{
				MemDC.MoveTo((int)(i*x_gap),    (int)(ChartRect.Height() - (m_TempArray->GetAt(i).temp[line]     - min_temp) * y_gap));
			
				MemDC.LineTo((int)((i+1)*x_gap),(int)(ChartRect.Height() - (m_TempArray->GetAt(i+1).temp[line]  - min_temp) *  y_gap));
				 
			}
		}

		if(m_TempArray->GetSize() > 180)
		{
			MemDC.MoveTo((int)((m_TempArray->GetSize() - 180)*x_gap),20);
			MemDC.LineTo((int)((m_TempArray->GetSize() - 1)*x_gap),20);
				
		}
		for(int i = 0; i < 5; i++)
		{
			pen.DeleteObject(); 
			pen.CreatePen(PS_SOLID,PEN_WEIGHT*5, TEMP_COLOR[i]);//RGB(0xFF,0xFF,0));  		//建立画笔
			oldpen=MemDC.SelectObject(&pen);
			MemDC.MoveTo(  0, i * 10 + 20);
			MemDC.LineTo( 30, i * 10 + 20); 
		}
	
	dc->BitBlt(ChartRect.left,ChartRect.top,ChartRect.Width(),ChartRect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形
 


	MemDC.SelectObject(oldbrush);

		
	//if(font.GetSafeHandle() != NULL)
	{
	//	font.DeleteObject();
	}

	
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}

	
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}

	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	Wnd->ReleaseDC(dc); 

}

#ifdef WINCE
void DrawLED_BiMetal(CWnd *parentWnd,int BoxNr,SWITCH_CONFIG_PARA  *m_ConfigPara,CRect* m_LED_Rect,BOX_TYPE box_type)
#else
void DrawLED_BiMetal(CWnd *parentWnd,int BoxNr,CRect AllRect,SWITCH_CONFIG_PARA  *m_ConfigPara,CRect* m_LED_Rect,BOX_TYPE box_type)
#endif
{
	if(!parentWnd->IsWindowVisible()) 
	{
		return;
	}

	

	CBitmap bitmap;	
	CBitmap* pOldBitmap;
	CDC MemDC,*dc;
	CRect rect(0,00,70,52);
	CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	CBrush graybrush(COLORREF(RGB(192,192,192)));
	 
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc = parentWnd->GetWindowDC();
		

	if(dc == NULL)
	{
		//m_TestMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		parentWnd->UpdateData(false);
		return;
	}
	
	CRect wndRect;
#ifdef WINCE
	
	parentWnd->GetWindowRect(wndRect);

	nWidth	= (wndRect.right - wndRect.left)*2/3;
	nHeight = wndRect.bottom;
#else
	
	wndRect = AllRect;

	nWidth	= wndRect.right - wndRect.left;
	nHeight = wndRect.Height();

#endif
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		//m_TestMsg.Format(_T("!MemDC.CreateCompatibleDC(dc)%s %d\r\n"),__FILE__,__LINE__);
		parentWnd->UpdateData(false);
		parentWnd->ReleaseDC(dc);
		return;
	}
	

	
	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			//m_TestMsg.Format(_T("!bitmap.DeleteObject()%s %d\r\n"),__FILE__,__LINE__);
			parentWnd->UpdateData(false);
			parentWnd->ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
	{
		//m_TestMsg.Format(_T("!bitmap.CreateCompatibleBitmap%s %d\r\n"),__FILE__,__LINE__);
		parentWnd->UpdateData(false);
		parentWnd->ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(255,255,255));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(0,0,0));
	
	int Ellipse_W_Space	= 1;
	
	int Ellipse_W = (nWidth  - PEN_WEIGHT *2) / G_iLedNrPerLine[BoxNr] - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT *2) / G_iLedLines[BoxNr]       - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	

	int Ellipse_H_Space = nHeight / G_iLedLines[BoxNr] - Ellipse_H - 1;
  




	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
	
	MemDC.RoundRect(PEN_WEIGHT, PEN_WEIGHT, nWidth-PEN_WEIGHT*3 + 4,nHeight - PEN_WEIGHT*5 ,10,10);


	CString nr;

	MemDC.SelectObject(&redbrush);				//选择画刷

	
	for(int line=0; line < G_iLedLines[BoxNr] ; line++)
	{//闭合是红色
		for(int i=0; i < G_iLedNrPerLine[BoxNr] ; i++)
		{
			int id = i + line * G_iLedNrPerLine[BoxNr];

			rect.left	= (i % G_iLedNrPerLine[BoxNr])*(Ellipse_W_GAP+Ellipse_W_Space)  + (int)(Ellipse_W_Space / 2);
			
			rect.top	= line*(Ellipse_H + Ellipse_H_Space)   + (int)(Ellipse_H_Space / 2) + 2;
				
			rect.right	= rect.left + Ellipse_W - 2;
			rect.bottom = rect.top	+ Ellipse_H / 2 - 2;
			
			m_LED_Rect[id] = rect;

/*
			if(G_SwitchTestResult[0][id].IsClosing == true)
			{
				MemDC.Rectangle(rect);		//填充背景
			}
	
			rect.top = rect.bottom - 1;
			rect.bottom = rect.top	+ (int)(Ellipse_H /4);
			if(G_SwitchTestResult[0][id].IsClosing == true)
			{
				MemDC.Rectangle(rect);		//填充背景
			}

			rect.top = rect.bottom - 1;
			rect.bottom = rect.top	+ (int)(Ellipse_H /4);
			if(G_SwitchTestResult[0][id].IsClosing == true)
			{
				MemDC.Rectangle(rect);		//填充背景
			}
*/			
		}
	}

	


	MemDC.SelectObject(&graybrush);				//选择画刷

	for(int line=0; line < G_iLedLines[BoxNr] ; line++)
	{//断开是灰色
		for(int i=0; i < G_iLedNrPerLine[BoxNr] ; i++)
		{
			int id = i + line * G_iLedNrPerLine[BoxNr];

			rect.left	= (i % G_iLedNrPerLine[BoxNr])*(Ellipse_W_GAP+Ellipse_W_Space) + (int)(Ellipse_W_Space / 2);
			
			rect.top	= line*(Ellipse_H + Ellipse_H_Space)  + (int)(Ellipse_H_Space / 2) + 2;
			
			rect.right	= rect.left + Ellipse_W - 2;
			rect.bottom = rect.top	+ Ellipse_H / 4 - 2;
			MemDC.Rectangle(rect);		//填充背景

			rect.top = rect.bottom - 1;
			rect.bottom = rect.top	+ (int)(Ellipse_H /4);
			
			//if(G_SwitchTestResult[0][id ].IsClosing == false)
			{
				MemDC.Rectangle(rect);		//填充背景
			}

			rect.top = rect.bottom - 1;
			rect.bottom = rect.top	+ (int)(Ellipse_H /4);
			//if(G_SwitchTestResult[0][id].IsClosing == false)
			{
				MemDC.Rectangle(rect);		//填充背景
			}

			rect.top = rect.bottom - 1;
			rect.bottom = rect.top	+ (int)(Ellipse_H /4);
			//if(G_SwitchTestResult[0][id].IsClosing == false)
			{
				MemDC.Rectangle(rect);		//填充背景
			}

		}
	}



	
	CFont font;
	MemDC.SetTextColor(RGB(0x00,0x00,0xFF)); 
	if(font.CreatePointFont(200,_T("黑体"),&MemDC))
	{
		CFont *oldfont = MemDC.SelectObject(&font); 


		for(int line=0; line < G_iLedLines[BoxNr] ; line++)
		{//默认的全部是灰色
			for(int i=0; i < G_iLedNrPerLine[BoxNr] ; i++)
			{
				rect.left	= (i % G_iLedNrPerLine[BoxNr])*(Ellipse_W_GAP+Ellipse_W_Space) + (int)(Ellipse_W_Space / 2);
				
				rect.top	= line*(Ellipse_H + Ellipse_H_Space)  + (int)(Ellipse_H_Space / 2) + 2;
					
				rect.right	= rect.left + Ellipse_W - 2;
				rect.bottom = rect.top	+ Ellipse_H / 4 - 2;

				nr.Format(_T("%d"),line * G_iLedNrPerLine[BoxNr] + i + 1);
				MemDC.DrawText(nr,rect,DT_CENTER | DT_VCENTER);
			}
		}
		MemDC.SelectObject(oldfont); 
		
		font.DeleteObject();
	}

	
	MemDC.SetTextColor(RGB(0x00,0x00,0x00));
	if(G_TempFont.GetSafeHandle() == NULL)
	{
		int fontsize = 120;
		while(1)
		{	
			fontsize -= 2;

			if(G_TempFont.CreatePointFont(fontsize,_T("黑体"),&MemDC))
			{
				CFont *oldfont = MemDC.SelectObject(&G_TempFont);
				CSize size;
				if(::G_NormalConfigPara[0].Language == 0)
				{
					size = MemDC.GetTextExtent(_T("动作 188.8"));
				}
				else
				{
					size = MemDC.GetTextExtent(_T("ACT 188.8"));
				}
				if(size.cx <= Ellipse_W)
				{
					MemDC.SelectObject(&oldfont); 
					break;
				}
				else
				{
					//CString s;
					//s.Format(_T("%d,%d,%d"), fontsize,size.cx,Ellipse_W);
					//AfxMessageBox(s);
					G_TempFont.DeleteObject(); 
				}
			}
			else
			{
				break;
			}
			
		}
	}

	CONTROL_TEMP_RANGE		TempRange;
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,*m_ConfigPara  , &TempRange,G_NormalConfigPara[BoxNr].BoxType);


	if(G_TempFont.GetSafeHandle())
	{
		CFont *oldfont = MemDC.SelectObject(&G_TempFont); 


		for(int line=0; line < G_iLedLines[BoxNr] ; line++)
		{//动作复位温度
			for(int i=0; i < G_iLedNrPerLine[BoxNr] ; i++)
			{
				int id = i  + line * G_iLedNrPerLine[BoxNr];

				rect.left	= (i % G_iLedNrPerLine[BoxNr])*(Ellipse_W_GAP+Ellipse_W_Space) + (int)(Ellipse_W_Space / 2);
				rect.top	= line*(Ellipse_H + Ellipse_H_Space)   + Ellipse_H / 4 + 2;
				rect.right	= rect.left + Ellipse_W - 2;
				rect.bottom = rect.top	+ Ellipse_H / 4 - 2;
				if(::G_NormalConfigPara[BoxNr].SoundOutputType == 0)
				{
					if(G_SwitchTestResult[BoxNr][id].IsOpenned)
					{
					
						TEST_RESULT_LEVEL level = CheckTestLevel(*m_ConfigPara,G_SwitchTestResult[0][id],HeatCool,TempRange,G_SwitchTestResult[0][id],box_type);
						
						if(level == MAIN_LEVEL	|| level ==  TEST_RESULT_HIGH_LEVEL	|| level == LOW_LEVEL)
						{
							MemDC.SetTextColor(RGB(0,0,0xFF)); 
						}
						else
						{
							MemDC.SetTextColor(RGB(0xFF,0,0));
						}
						if(::G_NormalConfigPara[0].Language == 0)
						{
							nr.Format(_T("动作 %5.1f"),G_SwitchTestResult[0][id].OpenTemp);
						}
						else
						{
							nr.Format(_T("ACT %5.1f"),G_SwitchTestResult[0][id].OpenTemp);
						}
						MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
					}
					else
					{
						MemDC.SetTextColor(RGB(0x00,0xFF,0x00));
						
						if(::G_NormalConfigPara[0].Language == 0)
						{
							nr=_T("动作");
						}
						else
						{
							nr=_T("ACT %5.1f");
						}
						MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
					}
				}
				else if((G_NormalConfigPara[0].SoundOutputType == 3) || (G_NormalConfigPara[0].SoundOutputType == 2) || (G_NormalConfigPara[0].SoundOutputType == 1))
				{
					
					if(G_Noise.CurCounts[id] == 0)
					{
						MemDC.SetTextColor(RGB(0,0,0xFF)); 
					}
					else
					{
						MemDC.SetTextColor(RGB(0xFF,0,0));
					}
					if(::G_NormalConfigPara[0].Language == 0)
					{
						nr.Format(_T("噪音 %5d"),G_Noise.CurCounts[id]);
					}
					else
					{
						nr.Format(_T("CNT %5d"),G_Noise.CurCounts[id]);
					}
					
					MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
				}

				rect.top	= rect.bottom;
				rect.bottom = rect.top	+ Ellipse_H / 4 - 2;
				
				if(::G_NormalConfigPara[BoxNr].SoundOutputType == 0)
				{
					if(G_SwitchTestResult[BoxNr][id].IsClosed)
					{
						TEST_RESULT_LEVEL level = CheckTestLevel(*m_ConfigPara,G_SwitchTestResult[BoxNr][id],HeatCool,TempRange,G_SwitchTestResult[BoxNr][id],box_type);
						if(level == MAIN_LEVEL	|| level ==  TEST_RESULT_HIGH_LEVEL	|| level == LOW_LEVEL)
						{
							MemDC.SetTextColor(RGB(0,0,0xFF)); 
						}
						else
						{
							MemDC.SetTextColor(RGB(0xFF,0,0));
						}
						if(::G_NormalConfigPara[0].Language == 0)
						{
							nr.Format(_T("复位 %5.1f"),G_SwitchTestResult[0][id].CloseTemp);
						}
						else
						{
							nr.Format(_T("RET %5.1f"),G_SwitchTestResult[0][id].CloseTemp);
						}
						
						MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
					}
					else
					{
						MemDC.SetTextColor(RGB(0x00,0xFF,0x00));
						
						if(::G_NormalConfigPara[0].Language == 0)
						{
							nr=_T("复位");
						}
						else
						{
							nr=_T("RET");
						}
						
						MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
					}
				}
				
				else if(::G_NormalConfigPara[0].SoundOutputType == 2)
				{
					MemDC.SetTextColor(RGB(0,0,0xFF)); 
					if(::G_NormalConfigPara[0].Language == 0)
					{
						nr.Format(_T("等级 %5d"),::G_Noise.MaxLevel[id][0]);
					}
					else
					{
						nr.Format(_T("Lvl %5d"),::G_Noise.MaxLevel[id][0]);
					}
					MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
				}
				else if(::G_NormalConfigPara[0].SoundOutputType == 3 || ::G_NormalConfigPara[0].SoundOutputType == 1)
				{
					MemDC.SetTextColor(RGB(0,0,0xFF)); 
					if(::G_NormalConfigPara[0].Language == 0)
					{
						nr.Format(_T("等级 %5d"),::G_Noise.MaxLevel[id][1]);
					}
					else
					{
						nr.Format(_T("Lvl %5d"),::G_Noise.MaxLevel[id][1]);
					}
					MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
				}



				rect.top	= rect.bottom;
				rect.bottom = rect.top	+ Ellipse_H / 4 - 2;
				
				if(::G_NormalConfigPara[0].SoundOutputType == 0)
				{
					
					float gap;
					if(G_SwitchTestResult[0][id].IsOpenned && G_SwitchTestResult[0][id].IsClosed)
					{
						gap = G_SwitchTestResult[0][id].OpenTemp - G_SwitchTestResult[0][id].CloseTemp;
						if(::G_NormalConfigPara[0].Language == 0)
						{
							nr.Format(_T("差值%5.1f"),gap);
						}
						else
						{
							nr.Format(_T("DIF %5.1f"),gap);
						}

						TEST_RESULT_LEVEL level = CheckTestLevel(*m_ConfigPara,G_SwitchTestResult[0][id],HeatCool,TempRange,G_SwitchTestResult[0][id],box_type);
						
						
						if(level == MAIN_LEVEL	|| level ==  TEST_RESULT_HIGH_LEVEL	|| level == LOW_LEVEL)
						{
							MemDC.SetTextColor(RGB(0,0,0xFF)); 
						}
						else
						{
							MemDC.SetTextColor(RGB(0xFF,0,0));
						}
						MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
						
					}
					else
					{

						if(::G_NormalConfigPara[0].Language == 0)
						{
							nr = _T("差值");
						}
						else
						{
							nr = _T("DIF");
						}
						MemDC.SetTextColor(RGB(0x00,0xFF,0x00));
						
						MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
					}
					
				}
				
				else if(::G_NormalConfigPara[0].SoundOutputType == 2 || G_NormalConfigPara[0].SoundOutputType == 1)
				{
					MemDC.SetTextColor(RGB(0xFF,0,0xFF)); 
					if(::G_NormalConfigPara[0].Language == 0)
					{
						nr.Format(_T("最大 %5d"),::G_Noise.MaxCounts[id][0]);
					}
					else
					{
						nr.Format(_T("Max %5d"),::G_Noise.MaxCounts[id][0]);
					}
					MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
				}
				else if(::G_NormalConfigPara[0].SoundOutputType == 3 )
				{
					MemDC.SetTextColor(RGB(0xFF,0,0xFF)); 
					if(::G_NormalConfigPara[0].Language == 0)
					{
						nr.Format(_T("最大 %5d"),::G_Noise.MaxCounts[id][1]);
					}
					else
					{
						nr.Format(_T("Max %5d"),::G_Noise.MaxCounts[id][1]);
					}
					MemDC.DrawText(nr,rect,DT_LEFT | DT_VCENTER);
				}
				
				

			}
		}
		MemDC.SelectObject(oldfont); 

	//	font.DeleteObject();

	}

#ifdef WINCE
	dc->BitBlt(0, PEN_WEIGHT,nWidth + 30 ,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
#else
	dc->BitBlt(AllRect.left ,AllRect.top + PEN_WEIGHT * 5 ,AllRect.Width() ,AllRect.Height() ,&MemDC,0,0,SRCCOPY); 	//显示原图形

#endif
	parentWnd->UpdateData(false); 


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}

	if(groundbrush.GetSafeHandle() != NULL)
	{
		groundbrush.DeleteObject();
	}
	if(greenbrush.GetSafeHandle() != NULL)
	{
		greenbrush.DeleteObject(); 
	}

	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}

	parentWnd->ReleaseDC(dc); 
}