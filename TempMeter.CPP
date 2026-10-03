
#include "stdafx.h"


#include "TempMeter.h"
#include "common.h"


#define MSG	msg[::G_NormalConfigPara[0].Language]

CTempMeter::CTempMeter(void)
{
	memset(&m_TempAnswerPara,0,sizeof(m_TempAnswerPara));

	memset(&m_8TEMP_8SSR_Answer,0,sizeof(m_8TEMP_8SSR_Answer));

	memset(&m_32TEMP_ANSWER,0,sizeof(m_32TEMP_ANSWER));

	m_BoxNr		= 0;
	m_IsBeepOn	= false;
	this->m_PvFontSize = 0;
	this->m_TextFontSize = 0;
}

void CTempMeter::Init(HWND	WndHandle, CDC *memDC,CRect rect,MODBUS_RS485_32TEMP_ANSWER Answer_32TEMP)
{
	//TempAnswerPara的值为莫名其妙的全被设置成 0xFF
	//如果是 全0xFF,就用老数据

	this->m_32TEMP_ANSWER	= Answer_32TEMP;
	

	m_WndHandle			= WndHandle;
	m_DC				= memDC;
	m_Rect				= rect;

}


void CTempMeter::Init(HWND	WndHandle, CDC *memDC,CRect rect,MODBUS_RS485_8TEMP_8SSR_ANSWER Answer_8TEMP_8SSR)
{
	//TempAnswerPara的值为莫名其妙的全被设置成 0xFF
	//如果是 全0xFF,就用老数据

	this->m_8TEMP_8SSR_Answer	= Answer_8TEMP_8SSR;
	

	m_WndHandle			= WndHandle;
	m_DC				= memDC;
	m_Rect				= rect;

	


}

void CTempMeter::Init(HWND	WndHandle, CDC *memDC,CRect rect,MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT answer_para)
{
	m_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT_answer_para	= answer_para;
	

	m_WndHandle			= WndHandle;
	m_DC				= memDC;
	m_Rect				= rect;
}

void CTempMeter::Init(HWND	WndHandle, CDC *memDC,CRect rect,MODBUS_RS485_TEMP_ANSWER_PARA TempAnswerPara)
{
	//TempAnswerPara的值为莫名其妙的全被设置成 0xFF
	//如果是 全0xFF,就用老数据

	m_TempAnswerPara	= TempAnswerPara;
	

	m_WndHandle			= WndHandle;
	m_DC				= memDC;
	m_Rect				= rect;

	


}
CTempMeter::~CTempMeter(void)
{

}

void	CTempMeter::DrawMeter(MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT m_Answer)
{
	m_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT_answer_para = m_Answer;
	m_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT_answer_para.iError = 0;  //old board
	DrawMeter(m_BoxNr,m_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT_answer_para);
}
void	CTempMeter::DrawMeter(MODBUS_RS485_8TEMP_8SSR_ANSWER Answer_8TEMP_8SSR)
{
	m_8TEMP_8SSR_Answer = Answer_8TEMP_8SSR;
	m_8TEMP_8SSR_Answer.Error = 0;  //old board
	DrawMeter(m_BoxNr,Answer_8TEMP_8SSR);
}

void	CTempMeter::DrawMeter(MODBUS_RS485_32TEMP_ANSWER Answer_32TEMP)
{
	this->m_32TEMP_ANSWER = Answer_32TEMP;
	
	DrawMeter(m_BoxNr,m_32TEMP_ANSWER);
}


void CTempMeter::DrawMeter(MODBUS_RS485_TEMP_ANSWER_PARA TempAnswerPara)
{
	m_TempAnswerPara = TempAnswerPara;
	m_TempAnswerPara.iError = 0;  //old board
	DrawMeter(m_BoxNr);
}
float CTempMeter::GetCurAmp(void)
{
//#ifdef _JIU_YANG
	if(::G_NormalConfigPara[0].BoxType == BOX_JIU_YANG)
	{
		//1,3是温度，2，4是电流
		//4mA=0A,20mA是25A，
		float ad_4ma = (75 * 4 / 2560.0f) * 256 * 256;
		float ad_20ma = (75 * 20 / 2560.0f) * 256 * 256;
		
		float ad = m_TempAnswerPara.Temp[1]; //4mA,2560mV
		
		float amp =25 * (ad-ad_4ma) / (ad_20ma - ad_4ma); //满量程是25A

		
		return amp;

	}
	
	else
	{
		return (m_TempAnswerPara.Temp[0] + m_TempAnswerPara.Temp[1] +m_TempAnswerPara.Temp[2] +m_TempAnswerPara.Temp[3] ) /400.0f;
	}
}

float CTempMeter::GetCurAmp2(void)
{
	if(::G_NormalConfigPara[0].BoxType == BOX_JIU_YANG)
	{
		//1,3是温度，2，4是电流
		
		
		float ad = (m_TempAnswerPara.Temp[3] - 75 * 4  * 256 * 256/ 2560); //4mA,2560mV
			
		float amp =(ad / (75 * (20- 4) * 256 * 256/ 2560)) * 25; //满量程是25A

		return amp;
	}
	else
	{
		return (m_TempAnswerPara.Temp[0] + m_TempAnswerPara.Temp[1] +m_TempAnswerPara.Temp[2] +m_TempAnswerPara.Temp[3] ) /400.0f;
	}
}

float CTempMeter::GetCurPv(MODBUS_RS485_TEMP_ANSWER_PARA TempAnswerPara)
{
	int i = 0;
	float Pv = 0.0f;
	if(::G_NormalConfigPara[0].BoxType == BOX_JIU_YANG)
	{
		//1,3是温度，2，4是电流
		return (TempAnswerPara.Temp[0] + TempAnswerPara.Temp[2] ) / 200.0f;
	}
	else
	{
		if(TempAnswerPara.TempDecimalPlaces == 1)
		{
			if((TempAnswerPara.iError & 0x0001) == 0)
			{
				i++;
				Pv += TempAnswerPara.Temp[0] / 10.0f;
			}

			if((TempAnswerPara.iError & 0x0002) == 0)
			{
				i++;
				Pv += TempAnswerPara.Temp[1] / 10.0f;
			}
			if((TempAnswerPara.iError & 0x0004) == 0)
			{
				i++;
				Pv += TempAnswerPara.Temp[2] / 10.0f;
			}
			if((TempAnswerPara.iError & 0x0008) == 0)
			{
				i++;
				Pv += TempAnswerPara.Temp[3] / 10.0f;
			}

			if(i > 0)
			{
				Pv /= i;
			}
			return Pv;
		}
		else
		{
			if((TempAnswerPara.iError & 0x0001) == 0)
			{
				i++;
				Pv += TempAnswerPara.Temp[0] / 100.0f;
			}

			if((TempAnswerPara.iError & 0x0002) == 0)
			{
				i++;
				Pv += TempAnswerPara.Temp[1] / 100.0f;
			}
			if((TempAnswerPara.iError & 0x0004) == 0)
			{
				i++;
				Pv += TempAnswerPara.Temp[2] / 100.0f;
			}
			if((TempAnswerPara.iError & 0x0008) == 0)
			{
				i++;
				Pv += TempAnswerPara.Temp[3] / 100.0f;
			}

			if(i > 0)
			{
				Pv /= i;
			}
			return Pv;
		}
	}
}


float CTempMeter::GetCurSv(void)
{
		return m_TempAnswerPara.Sv / 10.0f;
}

CFont G_PvFont;
CFont G_ChineseFont;

extern bool G_cFlag;

void	CTempMeter::DrawMeter(int BoxNr,MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT m_Answer)
{
	CBitmap bitmap;	
	CBitmap* pOldBitmap;
	CDC MemDC,*dc;
	CRect rect;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备

	dc =   m_DC;
		

	if(dc == NULL)
	{
		//m_mainDlg->m_TestMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		//m_mainDlg->UpdateData(false);
		return;
	}
	
	nWidth	= m_Rect.Width() ;
	nHeight = m_Rect.Height()  ;

	CFont *oldfont;
	CString PV;
  
	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		return;
	}
	

	
	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
			return;
		}
	}

	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		bitmap.DeleteObject(); 
		return;
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(255,255,255));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);

	


	//CFont font;
	
	
	if(this->m_PvFontSize == 0)
	{
		m_PvFontSize = nHeight / 3 *10;

//		G_PvFont.DeleteObject();

		if(G_PvFont.GetSafeHandle()  == NULL)
		{
			while(1)
			{
				if(G_PvFont.CreatePointFont(m_PvFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_PvFont);
					CSize size = MemDC.GetTextExtent(_T("123.45℃")); 
					if((size.cx <  (nWidth * 0.65)) && (size.cy < (nHeight *0.4)))
					{
						break;
					}
					m_PvFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_PvFont.DeleteObject(); 
				}
				else
				{
					G_PvFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_PvFont);
		}
	}
	else
	{
		oldfont = MemDC.SelectObject(&G_PvFont);
	}
	//if(font.CreatePointFont(400,_T("黑体"),&MemDC))
	{
		//CFont *oldfont = MemDC.SelectObject(&font); 

		MemDC.SetTextColor(RGB(0xFF,0,0));

		rect.left		= nWidth * 0.2 ;
		rect.right		= nWidth * 0.70;
		rect.top		= 0;
		rect.bottom		= nHeight * 0.4;

		//m_Pv			= (m_TempAnswerPara.Temp[0]/ 100.0f + this->m_TempAnswerPara.Temp[1]/ 100.0f + this->m_TempAnswerPara.Temp[2]/ 100.0f + this->m_TempAnswerPara.Temp[3]/ 100.0f) / 4.0f;
		
		m_Pv = m_Answer.Temp[0]/100.0f;
		
//PV
		if(m_Answer.iError == 0)
		{
			PV.Format(_T("%6.2f℃,%6.2f℃"),m_Answer.Temp[0]/100.0f,m_Answer.Temp[1]/100.0f );
		}
		else
		{
			PV.Format(_T("错误码:%X"), m_Answer.iError);
		}
		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);


//SV
		MemDC.SetTextColor(RGB(0x00,0xFF,0x00));

		rect.left		= nWidth	* 0.2;
		rect.right		= nWidth	* 0.70;
		rect.top		= nHeight	* 0.4;
		rect.bottom		= nHeight	* 0.8;

		
			m_Sv = m_Answer.Sv[0] / 10.0f;
			PV.Format(_T("%6.2f℃,%d%%,%6.2f℃,%d%%,"), m_Answer.Sv[0] / 10.0,m_Answer.HeatPercent[0],m_Answer.Sv[1] / 10.0,m_Answer.HeatPercent[1] );
		

		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
	//	MemDC.SelectObject(oldfont); 
	//	font.DeleteObject();

	}

	if(m_TextFontSize == 0)
	{
		m_TextFontSize = nHeight / 3 *10;

	//	G_ChineseFont.DeleteObject();

		if(G_ChineseFont.GetSafeHandle() == NULL)
		{
			while(1)
			{
				if(G_ChineseFont.CreatePointFont(m_TextFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_ChineseFont);
					CSize size = MemDC.GetTextExtent(_T("空调 开")); 
					if(size.cx <  (nWidth * 0.2) && (size.cy < (nHeight *0.1)) )
					{
						break;
					}
					m_TextFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_ChineseFont.DeleteObject(); 
				}
				else
				{
					G_ChineseFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_ChineseFont);
		}

	}
	else
	{
		oldfont = MemDC.SelectObject(&G_ChineseFont);
	}

	
		//oldfont = MemDC.SelectObject(&G_ChineseFont); 

		MemDC.SetTextColor(RGB(0 , 0xFF,0));
		
		

	
		if(m_Answer.HeatPercent[0] >= 0)
		{
			MemDC.SetTextColor(RGB(0xFF,0,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("加热%3d%%"),
					_T("Heat%3d%%"),
				};
				PV.Format(MSG,m_Answer.HeatPercent[0]);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}
		else
		{
			MemDC.SetTextColor(RGB(0x00,0xFF,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("制冷%3d%%"),
					_T("Cool%3d%%"),
				};
				PV.Format(MSG,m_Answer.HeatPercent[0]);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
		}
		
		MemDC.SetTextColor(RGB(0xFF,0xFF,0x00));
		rect.left		= nWidth*0.44;
		rect.right		= nWidth;
		rect.top		= nHeight * 0.8;
		rect.bottom		= nHeight;
		CString ModeName[][2] =
		{
			{_T("空闲"),		_T("Idle")},			//0
			{_T("全速加热"),	_T("Full Heat")},		//1
			{_T("升温恒温"),	_T("Constant Heat")},	//2
			{_T("线性升温"),	_T("Slope Heat")},		//3
			{_T("全速降温"),	_T("Full Cool")},		//4
			{_T("降温恒温"),	_T("Constant Cool")},	//5
			{_T("线性降温"),	_T("Slope Cool")},		//6
			{_T("自整定"),		_T("Auto Tuning")},		//7
			{_T("冷自整定"),	_T("AT by A.C.")},		//8
		};

		int iCAL_SPEED_PERIOD = 20;
		if(G_RS485_Comm.IsOmronMeter(G_NormalConfigPara[m_BoxNr].BoxType ))
		{
			iCAL_SPEED_PERIOD = 30;
		}
		else
		{
			iCAL_SPEED_PERIOD = 10;
		}

		int size = G_TempArray[BoxNr].GetSize(); 




		
		PV.Format(_T("%s:%s"), ModeName[m_Answer.TestMode[0]][G_NormalConfigPara[m_BoxNr].Language],ModeName[m_Answer.TestMode[1]][G_NormalConfigPara[m_BoxNr].Language]);
		
		

		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
	

	

		dc->BitBlt(m_Rect.left , m_Rect.top ,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形



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
	
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());

}
void CTempMeter::DrawMeter(int BoxNr)
{
	if(::G_NormalConfigPara[0].BoxType == BOX_TEMP2_10IN_12OUT)
	{
		return;
	}

	CBitmap bitmap;	
	CBitmap* pOldBitmap;
	CDC MemDC,*dc;
	CRect rect;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备

	dc =   m_DC;
		

	if(dc == NULL)
	{
		//m_mainDlg->m_TestMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		//m_mainDlg->UpdateData(false);
		return;
	}
	
	nWidth	= m_Rect.Width() ;
	nHeight = m_Rect.Height()  ;

	CFont *oldfont;
	CString PV;
  
	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		return;
	}
	

	
	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
			return;
		}
	}

	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		bitmap.DeleteObject(); 
		return;
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(255,255,255));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);

	


	//CFont font;
	
	
	if(this->m_PvFontSize == 0)
	{
		m_PvFontSize = nHeight / 3 *10;

//		G_PvFont.DeleteObject();

		if(G_PvFont.GetSafeHandle()  == NULL)
		{
			while(1)
			{
				if(G_PvFont.CreatePointFont(m_PvFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_PvFont);
					CSize size = MemDC.GetTextExtent(_T("123.45℃")); 
					if((size.cx <  (nWidth * 0.65)) && (size.cy < (nHeight *0.4)))
					{
						break;
					}
					m_PvFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_PvFont.DeleteObject(); 
				}
				else
				{
					G_PvFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_PvFont);
		}
	}
	else
	{
		oldfont = MemDC.SelectObject(&G_PvFont);
	}
	//if(font.CreatePointFont(400,_T("黑体"),&MemDC))
	{
		//CFont *oldfont = MemDC.SelectObject(&font); 

		MemDC.SetTextColor(RGB(0xFF,0,0));

		rect.left		= nWidth * 0.2 ;
		rect.right		= nWidth * 0.85;
		rect.top		= 0;
		rect.bottom		= nHeight * 0.4;

		//m_Pv			= (m_TempAnswerPara.Temp[0]/ 100.0f + this->m_TempAnswerPara.Temp[1]/ 100.0f + this->m_TempAnswerPara.Temp[2]/ 100.0f + this->m_TempAnswerPara.Temp[3]/ 100.0f) / 4.0f;
		
		m_Pv = GetCurPv(this->m_TempAnswerPara);
		
//PV
		CTime cur = CTime::GetCurrentTime();
		CTimeSpan span = cur - G_Meter[BoxNr].m_MeterAnswerTime;

		if(span.GetTotalSeconds() > 10)
		{
			PV = _T("温度无响应");
		}
		else
		{
			if((m_TempAnswerPara.iError & 0x000F) != 0x000F)
			{
				/*CTimeSpan span = CTime::GetCurrentTime() - G_Meter[BoxNr].m_MeterAnswerTime;
				int secs = span.GetSeconds();
				if(secs > 10)
				{
					PV = _T("无响应");
				}
				else*/
				{
					PV.Format(_T("%6.2f℃"),m_Pv );
				}
			}
			else
			{
				PV.Format(_T("错误码:%X"), m_TempAnswerPara.iError & 0x000F);
			}
		}
		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);


//SV
		MemDC.SetTextColor(RGB(0x00,0xFF,0x00));

		rect.left		= nWidth	* 0.2;
		rect.right		= nWidth	* 0.85;
		rect.top		= nHeight	* 0.4;
		rect.bottom		= nHeight	* 0.8;
		 span = cur - G_Meter[BoxNr].m_LedAnswerTime[0];
		 if(span.GetTotalSeconds() > 10)
		 {
			 PV = _T("灯板无响应");
		 }
		else if(G_RS485_Comm.IsOmronMeter(::G_NormalConfigPara[BoxNr].BoxType))
		{
			PV.Format(_T("%6.2f℃"),m_SendSv );
		}
		else
		{
			if(this->m_PcbTestMode == PCB_SUB_MODE_STOP)
			{
				PV = _T("----.--");
			}
			else
			{
				m_Sv = m_TempAnswerPara.Sv / 10.0f;
				PV.Format(_T("%6.2f℃"), m_Sv );
			}
		}

		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
	//	MemDC.SelectObject(oldfont); 
	//	font.DeleteObject();

	}

	if(m_TextFontSize == 0)
	{
		m_TextFontSize = nHeight / 3 *10;

	//	G_ChineseFont.DeleteObject();

		if(G_ChineseFont.GetSafeHandle() == NULL)
		{
			while(1)
			{
				if(G_ChineseFont.CreatePointFont(m_TextFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_ChineseFont);
					CSize size = MemDC.GetTextExtent(_T("空调 开")); 
					if(size.cx <  (nWidth * 0.2) && (size.cy < (nHeight *0.2)) )
					{
						break;
					}
					m_TextFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_ChineseFont.DeleteObject(); 
				}
				else
				{
					G_ChineseFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_ChineseFont);
		}

	}
	else
	{
		oldfont = MemDC.SelectObject(&G_ChineseFont);
	}

	
	if(::G_NormalConfigPara[0].BoxType != BOX_JIU_YANG)
	{
		oldfont = MemDC.SelectObject(&G_ChineseFont); 

		MemDC.SetTextColor(RGB(0 , 0xFF,0));
		rect.left		= 0;
		rect.right		= nWidth * 0.2;
		rect.top		= 0;
		rect.bottom		= rect.top + nHeight * 0.2;

		if(m_TempAnswerPara.OUTPUT_STATUS_DOOR_ON_BIT == 0)
		{
			this->m_IsDoorOpened = false;
			//MemDC.DrawText(_T("门   关"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("门   关"),
				_T("DOOR  N"),
			};
			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);
		}
		else
		{
			this->m_IsDoorOpened = true;
			//MemDC.DrawText(_T("门   开"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("门   开"),
				_T("DOOR  Y"),
			};
			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);
		}
		rect.left		= 0;
		rect.right		= nWidth * 0.2;
		rect.top		= nHeight * 0.3;
		rect.bottom		= rect.top + nHeight * 0.2;
	


		if(m_TempAnswerPara.OUTPUT_STATUS_AIR_ON_BIT)
		{
			this->m_IsAcOn = true;
			//MemDC.DrawText(_T("空调 关"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("空调 开"),
				_T("A.C.  Y"),
			};

			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);

		}
		else
		{
			this->m_IsAcOn = false ;
			//MemDC.DrawText(_T("空调 开"),rect,DT_LEFT | DT_VCENTER);
			
			CString msg[] = 
			{
				_T("空调 关"),
				_T("A.C.  N"),
			};
			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);
		
		}

		rect.left		= 0;
		rect.right		= nWidth * 0.2;
		rect.top		= nHeight * 0.6;
		rect.bottom		= rect.top + nHeight * 0.2;
		if(m_TempAnswerPara.OUTPUT_STATUS_BEEP_BIT)
		{
			this->m_IsBeepOn = true;
			//MemDC.DrawText(_T("空调 开"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("喇叭 开"),
				_T("BEEP  Y"),
			};

			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);

		}
		else
		{
			

			this->m_IsBeepOn = false;
			CString msg[] = 
			{
				_T("喇叭 关"),
				_T("BEEP  N"),
			};
			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);
		
		}

		}

		for(int ii = 0; ii < 4; ii++)
		{
			rect.left		= nWidth*0.85;
			rect.right		= nWidth;
			rect.top		= nHeight * 0.2 * ii;
			rect.bottom		= rect.top + nHeight * 0.2;
			if(m_TempAnswerPara.iError & (1 << ii))
			{
				CString msg[] = 
				{
					_T("错误!"),
					_T("Error!"),
				};
				PV = MSG;
			}
			else
			{
				if(m_TempAnswerPara.TempDecimalPlaces == 1)
				{
					PV.Format(_T("%.2f"),m_TempAnswerPara.Temp[ii] / 10.0f);
				}
				else
				{
					PV.Format(_T("%.2f"),m_TempAnswerPara.Temp[ii] / 100.0f);
				}
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}

	
		if(m_TempAnswerPara.HeatPercent >= 0)
		{
			MemDC.SetTextColor(RGB(0xFF,0,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("加热%3d%%"),
					_T("Heat%3d%%"),
				};
				PV.Format(MSG,m_TempAnswerPara.HeatPercent);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}
		else
		{
			MemDC.SetTextColor(RGB(0x00,0xFF,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("制冷%3d%%"),
					_T("Cool%3d%%"),
				};
				PV.Format(MSG,m_TempAnswerPara.HeatPercent);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
		}
		
		MemDC.SetTextColor(RGB(0xFF,0xFF,0x00));
		rect.left		= nWidth*0.3;
		rect.right		= nWidth;
		rect.top		= nHeight * 0.8;
		rect.bottom		= nHeight;
		CString ModeName[][2] =
		{
			{_T("空闲"),		_T("Idle")},			//0
			{_T("全速加热"),	_T("Full Heat")},		//1
			{_T("升温恒温"),	_T("Constant Heat")},	//2
			{_T("线性升温"),	_T("Slope Heat")},		//3
			{_T("全速降温"),	_T("Full Cool")},		//4
			{_T("降温恒温"),	_T("Constant Cool")},	//5
			{_T("线性降温(热补偿)"),	_T("Slope Cool")},		//6
			{_T("自整定"),		_T("Auto Tuning")},		//7
			{_T("冷自整定"),	_T("AT by A.C.")},		//8
			{_T("降温风扇恒温"),_T("Constant Fan Cool")},	//SUB_MODE_PID_COOL_BY_FAN			=	9,
			{_T("线性降温(外鼓风)"),_T("Slope Fan Cool")},		//SUB_MODE_STEP_PID_COOL_BY_FAN		=	10,
		};

		int iCAL_SPEED_PERIOD = 20;
		if(G_RS485_Comm.IsOmronMeter(G_NormalConfigPara[m_BoxNr].BoxType ))
		{
			iCAL_SPEED_PERIOD = 30;
		}
		else
		{
			iCAL_SPEED_PERIOD = 20;
		}

		int size = G_TempArray[BoxNr].GetSize(); 



		if(size > iCAL_SPEED_PERIOD)
		{
			
			if(::G_NormalConfigPara[BoxNr].BoxType == ::BOX_JIU_YANG)
			{//保存一天24小时的温度值
				if(size > 24 * 60 * 60)
				{
					G_TempArray[BoxNr].RemoveAt(0);
				}
			}
			else
			{
				TEMP_RECORD TempArray = G_TempArray[BoxNr].GetAt(size - iCAL_SPEED_PERIOD);
				if(TempArray.temp[0]  != 0 && TempArray.temp[1]  != 0 && TempArray.temp[2]  != 0 && TempArray.temp[3]  != 0)
				{
					m_StartTemp =	TempArray.temp[0];
					m_EndTemp	=	G_TempArray[BoxNr].GetAt(size - 1).temp[0];
					m_TimeSpan	=	G_TempArray[BoxNr].GetAt(size - 1).time - TempArray.time;

					double a = (G_TempArray[BoxNr].GetAt(size - 1).temp[0] - TempArray.temp[0]);
					double b = (((double)G_TempArray[BoxNr].GetAt(size - 1).time - (double)TempArray.time) );
					m_Speed = (m_Speed + a * 60.0f / b) / 2;
				}
			
			}
			
		}
		else
		{
			//	m_Speed = 0.0f;
		}

		m_PcbTestMode = (PCB_SUB_MODE)m_TempAnswerPara.TestMode;

		if(G_RS485_Comm.IsOmronMeter(G_NormalConfigPara[m_BoxNr].BoxType ))
		{
			PV.Format(_T("%.1f℃/m"), m_Speed);
		}
		else
		{
			PV.Format(_T("%s:%.1f℃/m"), ModeName[m_TempAnswerPara.TestMode][G_NormalConfigPara[m_BoxNr].Language],m_Speed);
			//PV.Format(_T("%1.f,%.1f,%d,%6.1f℃/m"), this->m_StartTemp,this->m_EndTemp,this->m_TimeSpan,m_Speed);
		}
		

		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
	

	

	dc->BitBlt(m_Rect.left , m_Rect.top ,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形



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
	
	::ReleaseDC(m_WndHandle,dc->GetSafeHdc()); 


}
void CTempMeter::DrawMeter(int BoxNr,MODBUS_RS485_8TEMP_8SSR_ANSWER m_8TEMP_8SSR_Answer)
{
	CBitmap bitmap;	
	CBitmap* pOldBitmap;
	CDC MemDC,*dc;
	CRect rect;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备

	dc =   m_DC;
		

	if(dc == NULL)
	{
		//m_mainDlg->m_TestMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		//m_mainDlg->UpdateData(false);
		return;
	}
	
	nWidth	= m_Rect.Width() ;
	nHeight = m_Rect.Height()  ;

	CFont *oldfont;
	CString PV;
  
	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		return;
	}
	

	
	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
			return;
		}
	}

	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		bitmap.DeleteObject(); 
		return;
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(255,255,255));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);

	


	//CFont font;
	
	
	if(this->m_PvFontSize == 0)
	{
		m_PvFontSize = nHeight / 3 *10;

//		G_PvFont.DeleteObject();

		if(G_PvFont.GetSafeHandle()  == NULL)
		{
			while(1)
			{
				if(G_PvFont.CreatePointFont(m_PvFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_PvFont);
					CSize size = MemDC.GetTextExtent(_T("123.45℃")); 
					if((size.cx <  (nWidth * 0.65)) && (size.cy < (nHeight *0.4)))
					{
						break;
					}
					m_PvFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_PvFont.DeleteObject(); 
				}
				else
				{
					G_PvFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_PvFont);
		}
	}
	else
	{
		oldfont = MemDC.SelectObject(&G_PvFont);
	}
	//if(font.CreatePointFont(400,_T("黑体"),&MemDC))
	{
		//CFont *oldfont = MemDC.SelectObject(&font); 

		MemDC.SetTextColor(RGB(0xFF,0,0));

		rect.left		= nWidth * 0.2 ;
		rect.right		= nWidth * 0.70;
		rect.top		= 0;
		rect.bottom		= nHeight * 0.4;

		//m_Pv			= (m_TempAnswerPara.Temp[0]/ 100.0f + this->m_TempAnswerPara.Temp[1]/ 100.0f + this->m_TempAnswerPara.Temp[2]/ 100.0f + this->m_TempAnswerPara.Temp[3]/ 100.0f) / 4.0f;
		

		
		int i = 0;
		m_Pv = 0;
		for(int ii = 0; ii < 8; ii++)
		{
			
			if(m_8TEMP_8SSR_Answer.Error & (1 << ii))
			{
				
			}
			else
			{
				m_Pv += this->m_8TEMP_8SSR_Answer.Pv[ii]/100.0f;
				i++;
			}
		}

		if( i > 0)
		{
			m_Pv /= i;
			PV.Format(_T("%6.2f℃"),m_Pv );
		}
		else
		{
			PV.Format(_T("错误码:%X"), m_8TEMP_8SSR_Answer.Error);
		}
		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);


//SV
		MemDC.SetTextColor(RGB(0x00,0xFF,0x00));

		rect.left		= nWidth	* 0.2;
		rect.right		= nWidth	* 0.70;
		rect.top		= nHeight	* 0.4;
		rect.bottom		= nHeight	* 0.8;

		if(m_8TEMP_8SSR_Answer.TestMode[0] == PCB_SUB_MODE_STOP)
		{
			PV = _T("---.--");
		}
		else
		{
			if(G_RS485_Comm.IsOmronMeter(::G_NormalConfigPara[BoxNr].BoxType))
			{
				PV.Format(_T("%6.2f℃"),m_SendSv );
			}
			else
			{
				m_Sv = m_8TEMP_8SSR_Answer.Sv[0] / 10.0f;
				PV.Format(_T("%6.2f℃"), m_Sv );
			}
		}
		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
	//	MemDC.SelectObject(oldfont); 
	//	font.DeleteObject();

	}

	if(m_TextFontSize == 0)
	{
		m_TextFontSize = nHeight / 3 *10;

	//	G_ChineseFont.DeleteObject();

		if(G_ChineseFont.GetSafeHandle() == NULL)
		{
			while(1)
			{
				if(G_ChineseFont.CreatePointFont(m_TextFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_ChineseFont);
					CSize size = MemDC.GetTextExtent(_T("空调 开")); 
					if(size.cx <  (nWidth * 0.2) && (size.cy < (nHeight *0.1)) )
					{
						break;
					}
					m_TextFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_ChineseFont.DeleteObject(); 
				}
				else
				{
					G_ChineseFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_ChineseFont);
		}

	}
	else
	{
		oldfont = MemDC.SelectObject(&G_ChineseFont);
	}

	
		oldfont = MemDC.SelectObject(&G_ChineseFont); 

		MemDC.SetTextColor(RGB(0 , 0xFF,0));
		for(int i = 0; i < 8; i++)
		{
			if(i < 4)
			{
				rect.left		= 0;
			}
			else
			{
				rect.left		= nWidth * 0.1;
			}
			rect.right		= rect.left + nWidth * 0.1;
			rect.top		= nHeight *0.21 * (i % 4);
			rect.bottom		= rect.top + nHeight * 0.2;

			if(this->m_8TEMP_8SSR_Answer.SSR_Status & (1 << i))
			{
				CString s;
#ifdef WINCE
				s.Format(_T("S%d 关"),i+1);
#else
				s.Format(_T("SSR%d 关"),i+1);
#endif
				MemDC.DrawText(s,rect,DT_LEFT | DT_VCENTER);
			}
			else
			{
				CString s;

#ifdef WINCE
				s.Format(_T("S%d 开"),i+1);
#else
				s.Format(_T("SSR%d 开"),i+1);
#endif
				MemDC.DrawText(s,rect,DT_LEFT | DT_VCENTER);
				
				

			}
		}
		
		for(int ii = 0; ii < 8; ii++)
		{
			if(ii < 4)
			{
				rect.left		= nWidth*0.70;
			}
			else
			{
				rect.left		= nWidth*0.85;
			}
			rect.right		= rect.left + nWidth * 0.15;
			rect.top		= nHeight * 0.2 * (ii % 4);
			rect.bottom		= rect.top + nHeight * 0.2;
			if(m_8TEMP_8SSR_Answer.Error & (1 << ii))
			{
				PV = _T("错误!");
			}
			else
			{
				PV.Format(_T("%.2f"),m_8TEMP_8SSR_Answer.Pv[ii] / 100.0f);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}

	
		if(m_8TEMP_8SSR_Answer.HeatPercent[0] >= 0)
		{
			MemDC.SetTextColor(RGB(0xFF,0,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("加热%3d%%"),
					_T("Heat%3d%%"),
				};
				PV.Format(MSG,m_8TEMP_8SSR_Answer.HeatPercent[0]);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}
		else
		{
			MemDC.SetTextColor(RGB(0x00,0xFF,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("制冷%3d%%"),
					_T("Cool%3d%%"),
				};
				PV.Format(MSG,m_8TEMP_8SSR_Answer.HeatPercent[0]);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
		}
		
		MemDC.SetTextColor(RGB(0xFF,0xFF,0x00));
		rect.left		= nWidth*0.44;
		rect.right		= nWidth;
		rect.top		= nHeight * 0.8;
		rect.bottom		= nHeight;
		CString ModeName[][2] =
		{
			{_T("空闲"),		_T("Idle")},			//0
			{_T("全速加热"),	_T("Full Heat")},		//1
			{_T("升温恒温"),	_T("Constant Heat")},	//2
			{_T("线性升温"),	_T("Slope Heat")},		//3
			{_T("全速降温"),	_T("Full Cool")},		//4
			{_T("降温恒温"),	_T("Constant Cool")},	//5
			{_T("线性降温"),	_T("Slope Cool")},		//6
			{_T("自整定"),		_T("Auto Tuning")},		//7
			{_T("冷自整定"),	_T("AT by A.C.")},		//8
		};

		int iCAL_SPEED_PERIOD = 20;
		if(G_RS485_Comm.IsOmronMeter(G_NormalConfigPara[m_BoxNr].BoxType ))
		{
			iCAL_SPEED_PERIOD = 30;
		}
		else
		{
			iCAL_SPEED_PERIOD = 10;
		}

		int size = G_TempArray[BoxNr].GetSize(); 



		if(size > iCAL_SPEED_PERIOD)
		{
			
			if(::G_NormalConfigPara[BoxNr].BoxType == ::BOX_JIU_YANG)
			{//保存一天24小时的温度值
				if(size > 24 * 60 * 60)
				{
					G_TempArray[BoxNr].RemoveAt(0);
				}
			}
			else
			{
				TEMP_RECORD TempArray = G_TempArray[BoxNr].GetAt(size - iCAL_SPEED_PERIOD);
				if(TempArray.temp[0]  != 0 && TempArray.temp[1]  != 0 && TempArray.temp[2]  != 0 && TempArray.temp[3]  != 0)
				{
					double a = (G_TempArray[BoxNr].GetAt(size - 1).temp[0] - TempArray.temp[0]);
					double b = (((double)G_TempArray[BoxNr].GetAt(size - 1).time - (double)TempArray.time) );
					m_Speed = (m_Speed  + a * 60.0f / b)/ 2; //滤波一下
				}
			}
			
		}
		else
		{
		//	m_Speed = 0.0f;
		}

		m_PcbTestMode = (PCB_SUB_MODE)m_8TEMP_8SSR_Answer.TestMode[0];

		if(G_RS485_Comm.IsOmronMeter(G_NormalConfigPara[m_BoxNr].BoxType ))
		{
			PV.Format(_T("%.1f℃/m"), m_Speed);
		}
		else
		{
			PV.Format(_T("%s:%.1f℃/m"), ModeName[m_8TEMP_8SSR_Answer.TestMode[0]][G_NormalConfigPara[m_BoxNr].Language],m_Speed);
		}
		

		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
	

	

		dc->BitBlt(m_Rect.left , m_Rect.top ,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形



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
	
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());

}

void CTempMeter::DrawMeter(int BoxNr,MODBUS_RS485_32TEMP_ANSWER m_32TEMP_Answer)
{
	CBitmap bitmap;	
	CBitmap* pOldBitmap;
	CDC MemDC,*dc;
	CRect rect;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备

	dc =   m_DC;
		

	if(dc == NULL)
	{
		//m_mainDlg->m_TestMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		//m_mainDlg->UpdateData(false);
		return;
	}
	
	nWidth	= m_Rect.Width() ;
	nHeight = m_Rect.Height()  ;

	CFont *oldfont;
	CString PV;
  
	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		return;
	}
	

	
	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
			return;
		}
	}

	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		bitmap.DeleteObject(); 
		return;
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(255,255,255));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);

	


	//CFont font;
	
	
	if(this->m_PvFontSize == 0)
	{
		m_PvFontSize = nHeight / 3 *10;

//		G_PvFont.DeleteObject();

		if(G_PvFont.GetSafeHandle()  == NULL)
		{
			while(1)
			{
				if(G_PvFont.CreatePointFont(m_PvFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_PvFont);
					CSize size = MemDC.GetTextExtent(_T("123.45℃")); 
					if((size.cx <  (nWidth * 0.65)) && (size.cy < (nHeight *0.4)))
					{
						break;
					}
					m_PvFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_PvFont.DeleteObject(); 
				}
				else
				{
					G_PvFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_PvFont);
		}
	}
	else
	{
		oldfont = MemDC.SelectObject(&G_PvFont);
	}
	//if(font.CreatePointFont(400,_T("黑体"),&MemDC))
	{
		//CFont *oldfont = MemDC.SelectObject(&font); 

		MemDC.SetTextColor(RGB(0xFF,0,0));

		rect.left		= nWidth * 0.2 ;
		rect.right		= nWidth * 0.70;
		rect.top		= 0;
		rect.bottom		= nHeight * 0.4;

		//m_Pv			= (m_TempAnswerPara.Temp[0]/ 100.0f + this->m_TempAnswerPara.Temp[1]/ 100.0f + this->m_TempAnswerPara.Temp[2]/ 100.0f + this->m_TempAnswerPara.Temp[3]/ 100.0f) / 4.0f;
		

		
		int i = 0;
		m_Pv = 0;
		for(int ii = 0; ii < 32; ii++)
		{
			float temp = m_32TEMP_Answer.para.Temp[ii] / 100.0f;

			if(temp > 0 && temp < 400.0f)
			{
				m_Pv += temp;
				i++;
			}
			
		}

		if( i > 0)
		{
			m_Pv /= i;
			PV.Format(_T("%6.2f℃"),m_Pv );
		}
		else
		{
			PV.Format(_T("所有传感器错误"));
		}
		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);


//SV
		MemDC.SetTextColor(RGB(0x00,0xFF,0x00));

		rect.left		= nWidth	* 0.2;
		rect.right		= nWidth	* 0.70;
		rect.top		= nHeight	* 0.4;
		rect.bottom		= nHeight	* 0.8;

		if(m_32TEMP_Answer.para.TestMode == PCB_SUB_MODE_STOP)
		{
			PV = _T("---.--");
		}
		else
		{
			
			{
				m_Sv = m_32TEMP_Answer.para.Sv / 100.0f;
				PV.Format(_T("%6.2f℃"), m_Sv );
			}
		}
		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
	
	}



	if(m_TextFontSize == 0)
	{
		m_TextFontSize = nHeight / 3 *10;

	//	G_ChineseFont.DeleteObject();

		if(G_ChineseFont.GetSafeHandle() == NULL)
		{
			while(1)
			{
				if(G_ChineseFont.CreatePointFont(m_TextFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_ChineseFont);
					CSize size = MemDC.GetTextExtent(_T("空调 开")); 
					if(size.cx <  (nWidth * 0.2) && (size.cy < (nHeight *0.1)) )
					{
						break;
					}
					m_TextFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_ChineseFont.DeleteObject(); 
				}
				else
				{
					G_ChineseFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_ChineseFont);
		}

	}
	else
	{
		oldfont = MemDC.SelectObject(&G_ChineseFont);
	}

	
		oldfont = MemDC.SelectObject(&G_ChineseFont); 

		MemDC.SetTextColor(RGB(0 , 0xFF,0));
		
			
		for(int ii = 0; ii < 32; ii++)
		{
			
			rect.left		= (nWidth / 16)*((int)(ii / 4));;
			
			rect.right		= rect.left + nWidth * 0.08;
			rect.top		= nHeight * 0.2 * (ii % 4);
			rect.bottom		= rect.top + nHeight * 0.2;
			float temp = m_32TEMP_Answer.para.Temp[ii]/100.0f;
			if(temp == 0.0f || temp > 400.0f)
			{
				PV = _T("错误!");
			}
			else
			{
				PV.Format(_T("%.2f(%4.1f)"),temp,temp - m_Pv);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}
		

	
		if(m_32TEMP_Answer.para.HeatPercent >= 0)
		{
			MemDC.SetTextColor(RGB(0xFF,0,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("加热%3d%%"),
					_T("Heat%3d%%"),
				};
				PV.Format(MSG,m_32TEMP_Answer.para.HeatPercent);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}
		else
		{
			MemDC.SetTextColor(RGB(0x00,0xFF,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("制冷%3d%%"),
					_T("Cool%3d%%"),
				};
				PV.Format(MSG,m_32TEMP_Answer.para.HeatPercent);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
		}
		
		MemDC.SetTextColor(RGB(0xFF,0xFF,0x00));
		rect.left		= nWidth*0.44;
		rect.right		= nWidth;
		rect.top		= nHeight * 0.8;
		rect.bottom		= nHeight;
		CString ModeName[][2] =
		{
			{_T("空闲"),		_T("Idle")},			//0
			{_T("全速加热"),	_T("Full Heat")},		//1
			{_T("升温恒温"),	_T("Constant Heat")},	//2
			{_T("线性升温"),	_T("Slope Heat")},		//3
			{_T("全速降温"),	_T("Full Cool")},		//4
			{_T("降温恒温"),	_T("Constant Cool")},	//5
			{_T("线性降温"),	_T("Slope Cool")},		//6
			{_T("自整定"),		_T("Auto Tuning")},		//7
			{_T("冷自整定"),	_T("AT by A.C.")},		//8
		};

		int iCAL_SPEED_PERIOD = 20;
		
		{
			iCAL_SPEED_PERIOD = 10;
		}

		int size = G_TempArray[BoxNr].GetSize(); 



		if(size > iCAL_SPEED_PERIOD)
		{
			
			
			{
				TEMP_RECORD TempArray = G_TempArray[BoxNr].GetAt(size - iCAL_SPEED_PERIOD);
				if(TempArray.temp[0]  != 0 && TempArray.temp[1]  != 0 && TempArray.temp[2]  != 0 && TempArray.temp[3]  != 0)
				{
					double a = (G_TempArray[BoxNr].GetAt(size - 1).temp[0] - TempArray.temp[0]);
					double b = (((double)G_TempArray[BoxNr].GetAt(size - 1).time - (double)TempArray.time) );
					m_Speed = (m_Speed  + a * 60.0f / b)/ 2; //滤波一下
				}
			}
			
		}
		else
		{
		//	m_Speed = 0.0f;
		}

		m_PcbTestMode = (PCB_SUB_MODE)m_32TEMP_Answer.para.TestMode;

		
		{
			PV.Format(_T("%s:%.1f℃/m"), ModeName[m_32TEMP_Answer.para.TestMode][G_NormalConfigPara[m_BoxNr].Language],m_Speed);
		}
		

		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
	

	

		dc->BitBlt(m_Rect.left , m_Rect.top ,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形



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
	
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());

}

/*
void CTempMeter::DrawMeter(int BoxNr)
{
	CBitmap bitmap;	
	CBitmap* pOldBitmap;
	CDC MemDC,*dc;
	CRect rect;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备

	dc =   m_DC;
		

	if(dc == NULL)
	{
		//m_mainDlg->m_TestMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		//m_mainDlg->UpdateData(false);
		return;
	}
	
	nWidth	= m_Rect.Width() ;
	nHeight = m_Rect.Height()  ;

	CFont *oldfont;
	CString PV;
  
	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		return;
	}
	

	
	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
			return;
		}
	}

	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
	{
		::ReleaseDC(m_WndHandle,dc->GetSafeHdc());
		bitmap.DeleteObject(); 
		return;
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(255,255,255));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);

	


	//CFont font;
	
	
	if(this->m_PvFontSize == 0)
	{
		m_PvFontSize = nHeight / 3 *10;

//		G_PvFont.DeleteObject();

		if(G_PvFont.GetSafeHandle()  == NULL)
		{
			while(1)
			{
				if(G_PvFont.CreatePointFont(m_PvFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_PvFont);
					CSize size = MemDC.GetTextExtent(_T("123.45℃")); 
					if((size.cx <  (nWidth * 0.65)) && (size.cy < (nHeight *0.4)))
					{
						break;
					}
					m_PvFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_PvFont.DeleteObject(); 
				}
				else
				{
					G_PvFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_PvFont);
		}
	}
	else
	{
		oldfont = MemDC.SelectObject(&G_PvFont);
	}
	//if(font.CreatePointFont(400,_T("黑体"),&MemDC))
	{
		//CFont *oldfont = MemDC.SelectObject(&font); 

		MemDC.SetTextColor(RGB(0xFF,0,0));

		rect.left		= nWidth * 0.2 ;
		rect.right		= nWidth * 0.85;
		rect.top		= 0;
		rect.bottom		= nHeight * 0.4;

		//m_Pv			= (m_TempAnswerPara.Temp[0]/ 100.0f + this->m_TempAnswerPara.Temp[1]/ 100.0f + this->m_TempAnswerPara.Temp[2]/ 100.0f + this->m_TempAnswerPara.Temp[3]/ 100.0f) / 4.0f;
		
		m_Pv = GetCurPv();
		
//PV
		if((m_TempAnswerPara.iError & 0x000F) == 0)
		{
			PV.Format(_T("%6.2f℃"),m_Pv );
		}
		else
		{
			PV.Format(_T("错误码:%X"), m_TempAnswerPara.iError & 0x000F);
		}
		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);


//SV
		MemDC.SetTextColor(RGB(0x00,0xFF,0x00));

		rect.left		= nWidth	* 0.2;
		rect.right		= nWidth	* 0.85;
		rect.top		= nHeight	* 0.4;
		rect.bottom		= nHeight	* 0.8;

		if(G_RS485_Comm.IsOmronMeter(::G_NormalConfigPara[BoxNr].BoxType))
		{
			PV.Format(_T("%6.2f℃"),m_SendSv );
		}
		else
		{
			if(this->m_PcbTestMode == PCB_SUB_MODE_STOP)
			{
				PV = _T("----.--");
			}
			else
			{
				m_Sv = m_TempAnswerPara.Sv / 10.0f;
				PV.Format(_T("%6.2f℃"), m_Sv );
			}
		}

		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
	//	MemDC.SelectObject(oldfont); 
	//	font.DeleteObject();

	}

	if(m_TextFontSize == 0)
	{
		m_TextFontSize = nHeight / 3 *10;

	//	G_ChineseFont.DeleteObject();

		if(G_ChineseFont.GetSafeHandle() == NULL)
		{
			while(1)
			{
				if(G_ChineseFont.CreatePointFont(m_TextFontSize,_T("黑体"),&MemDC))
				{
					oldfont = MemDC.SelectObject(&G_ChineseFont);
					CSize size = MemDC.GetTextExtent(_T("空调 开")); 
					if(size.cx <  (nWidth * 0.2) && (size.cy < (nHeight *0.2)) )
					{
						break;
					}
					m_TextFontSize -= 10;
					MemDC.SelectObject(oldfont);
					G_ChineseFont.DeleteObject(); 
				}
				else
				{
					G_ChineseFont.DeleteObject();
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&G_ChineseFont);
		}

	}
	else
	{
		oldfont = MemDC.SelectObject(&G_ChineseFont);
	}

	
	if(::G_NormalConfigPara[0].BoxType != BOX_JIU_YANG)
	{
		oldfont = MemDC.SelectObject(&G_ChineseFont); 

		MemDC.SetTextColor(RGB(0 , 0xFF,0));
		rect.left		= 0;
		rect.right		= nWidth * 0.2;
		rect.top		= 0;
		rect.bottom		= rect.top + nHeight * 0.2;

		if(m_TempAnswerPara.OutputStatus & (1 << METER_OUTPUT_STATUS_BIT_DOOR))
		{
			this->m_IsDoorOpened = false;
			//MemDC.DrawText(_T("门   关"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("门   关"),
				_T("DOOR  N"),
			};
			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);
		}
		else
		{
			this->m_IsDoorOpened = true;
			//MemDC.DrawText(_T("门   开"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("门   开"),
				_T("DOOR  Y"),
			};
			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);
		}
		rect.left		= 0;
		rect.right		= nWidth * 0.2;
		rect.top		= nHeight * 0.3;
		rect.bottom		= rect.top + nHeight * 0.2;
	


		if(m_TempAnswerPara.OutputStatus & (1 << METER_OUTPUT_STATUS_BIT_AIR))
		{
			this->m_IsAcOn = false;
			//MemDC.DrawText(_T("空调 关"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("空调 关"),
				_T("A.C.  N"),
			};
			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);

		}
		else
		{
			this->m_IsAcOn = true;
			//MemDC.DrawText(_T("空调 开"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("空调 开"),
				_T("A.C.  Y"),
			};

			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);
		
		}

		rect.left		= 0;
		rect.right		= nWidth * 0.2;
		rect.top		= nHeight * 0.6;
		rect.bottom		= rect.top + nHeight * 0.2;
		if(m_TempAnswerPara.OutputStatus & (1 << METER_OUTPUT_STATUS_BIT_BEEP))
		{
			this->m_IsBeepOn = true;
			//MemDC.DrawText(_T("空调 开"),rect,DT_LEFT | DT_VCENTER);
			CString msg[] = 
			{
				_T("喇叭 开"),
				_T("BEEP  Y"),
			};

			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);

		}
		else
		{
			

			this->m_IsBeepOn = false;
			CString msg[] = 
			{
				_T("喇叭 关"),
				_T("BEEP  N"),
			};
			MemDC.DrawText(MSG,rect,DT_LEFT | DT_VCENTER);
		
		}

		}

		for(int ii = 0; ii < 4; ii++)
		{
			rect.left		= nWidth*0.85;
			rect.right		= nWidth;
			rect.top		= nHeight * 0.2 * ii;
			rect.bottom		= rect.top + nHeight * 0.2;
			if(m_TempAnswerPara.iError & (1 << ii))
			{
				CString msg[] = 
				{
					_T("错误!"),
					_T("Error!"),
				};
				PV = MSG;
			}
			else
			{
				if(m_TempAnswerPara.TempDecimalPlaces == 1)
				{
					PV.Format(_T("%.2f"),m_TempAnswerPara.Temp[ii] / 10.0f);
				}
				else
				{
					PV.Format(_T("%.2f"),m_TempAnswerPara.Temp[ii] / 100.0f);
				}
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}

	
		if(m_TempAnswerPara.HeatPercent >= 0)
		{
			MemDC.SetTextColor(RGB(0xFF,0,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("加热%3d%%"),
					_T("Heat%3d%%"),
				};
				PV.Format(MSG,m_TempAnswerPara.HeatPercent);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		}
		else
		{
			MemDC.SetTextColor(RGB(0x00,0xFF,0));

			rect.left		= 0;
			rect.right		= nWidth*0.22;
			rect.top		= nHeight * 0.8;
			rect.bottom		= nHeight;
			{
				CString msg[] = 
				{
					_T("制冷%3d%%"),
					_T("Cool%3d%%"),
				};
				PV.Format(MSG,m_TempAnswerPara.HeatPercent);
			}
			MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
		
		}
		
		MemDC.SetTextColor(RGB(0xFF,0xFF,0x00));
		rect.left		= nWidth*0.3;
		rect.right		= nWidth;
		rect.top		= nHeight * 0.8;
		rect.bottom		= nHeight;
		CString ModeName[][2] =
		{
			{_T("空闲"),		_T("Idle")},			//0
			{_T("全速加热"),	_T("Full Heat")},		//1
			{_T("升温恒温"),	_T("Constant Heat")},	//2
			{_T("线性升温"),	_T("Slope Heat")},		//3
			{_T("全速降温"),	_T("Full Cool")},		//4
			{_T("降温恒温"),	_T("Constant Cool")},	//5
			{_T("线性降温(热补偿)"),	_T("Slope Cool")},		//6
			{_T("自整定"),		_T("Auto Tuning")},		//7
			{_T("冷自整定"),	_T("AT by A.C.")},		//8
			{_T("降温风扇恒温"),_T("Constant Fan Cool")},	//SUB_MODE_PID_COOL_BY_FAN			=	9,
			{_T("线性降温(外鼓风)"),_T("Slope Fan Cool")},		//SUB_MODE_STEP_PID_COOL_BY_FAN		=	10,
		};

		int iCAL_SPEED_PERIOD = 20;
		if(G_RS485_Comm.IsOmronMeter(G_NormalConfigPara[m_BoxNr].BoxType ))
		{
			iCAL_SPEED_PERIOD = 30;
		}
		else
		{
			iCAL_SPEED_PERIOD = 20;
		}

		int size = G_TempArray[BoxNr].GetSize(); 



		if(size > iCAL_SPEED_PERIOD)
		{
			
			if(::G_NormalConfigPara[BoxNr].BoxType == ::BOX_JIU_YANG)
			{//保存一天24小时的温度值
				if(size > 24 * 60 * 60)
				{
					G_TempArray[BoxNr].RemoveAt(0);
				}
			}
			else
			{
				TEMP_RECORD TempArray = G_TempArray[BoxNr].GetAt(size - iCAL_SPEED_PERIOD);
				if(TempArray.temp[0]  != 0 && TempArray.temp[1]  != 0 && TempArray.temp[2]  != 0 && TempArray.temp[3]  != 0)
				{
					m_StartTemp =	TempArray.temp[0];
					m_EndTemp	=	G_TempArray[BoxNr].GetAt(size - 1).temp[0];
					m_TimeSpan	=	G_TempArray[BoxNr].GetAt(size - 1).time - TempArray.time;

					double a = (G_TempArray[BoxNr].GetAt(size - 1).temp[0] - TempArray.temp[0]);
					double b = (((double)G_TempArray[BoxNr].GetAt(size - 1).time - (double)TempArray.time) );
					m_Speed = (m_Speed + a * 60.0f / b) / 2;
				}
			
			}
			
		}
		else
		{
			//	m_Speed = 0.0f;
		}

		m_PcbTestMode = (PCB_SUB_MODE)m_TempAnswerPara.TestMode;

		if(G_RS485_Comm.IsOmronMeter(G_NormalConfigPara[m_BoxNr].BoxType ))
		{
			PV.Format(_T("%.1f℃/m"), m_Speed);
		}
		else
		{
			PV.Format(_T("%s:%.1f℃/m"), ModeName[m_TempAnswerPara.TestMode][G_NormalConfigPara[m_BoxNr].Language],m_Speed);
			//PV.Format(_T("%1.f,%.1f,%d,%6.1f℃/m"), this->m_StartTemp,this->m_EndTemp,this->m_TimeSpan,m_Speed);
		}
		

		MemDC.DrawText(PV,rect,DT_RIGHT | DT_VCENTER);
	

	

	dc->BitBlt(m_Rect.left , m_Rect.top ,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形



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
	
	::ReleaseDC(m_WndHandle,dc->GetSafeHdc()); 


}
*/