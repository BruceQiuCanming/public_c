// CurveDlg.cpp : 实现文件
//



#pragma once

#include "stdafx.h"


#include "CurveDlg.h"
#include "DrawLed.h"
#include "HeatBoxDlg.h"
#include "Common.h"

// CCurveDlg 对话框
CCurveDlg								G_CurveDlg;

IMPLEMENT_DYNAMIC(CCurveDlg, CDialog)

CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurveDlg::IDD, pParent)
{
	m_NTC_ID = -1;
}

CCurveDlg::~CCurveDlg()
{
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCurveDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_PAINT()
//	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCurveDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


BOOL CCurveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->ShowWindow(SW_SHOWMAXIMIZED); 
	m_BeginTestTime = CTime::GetCurrentTime();
	return TRUE;
}
// CCurveDlg 消息处理程序
void CCurveDlg::DrawNtcTempCurve(void)
{
	CFile log_file;
	CString m_TestMsg;

#ifdef WINCE
	this->ShowWindow(SW_SHOWMAXIMIZED); 
#endif
	CString start_time;
	start_time = m_BeginTestTime.Format(_T("开始时间 %Y-%m-%d %H:%M:%S")); 

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 
	 CBrush groundbrush(COLORREF(RGB(0,0,0)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush yellowbrush(COLORREF(RGB(0xFF,0xFF,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(192,192,192)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	
	//绘制准备
	dc=this->GetDC();
		

	if(dc == NULL)
	{
		
		return;
	}
	
	CRect ChartRect;
	this->GetWindowRect(ChartRect);

	ScreenToClient(&ChartRect);


	
	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		
		ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			
			ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,ChartRect.Width(),ChartRect.Height()))	//建立与显示设备兼容的位图
	{
		
		ReleaseDC(dc);
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
		float min_temp = 99999999.0f;
		
		float max_res = -999.0f;
		float min_res = 99999999.0f;

		for(int i = 0; i < m_NtcRecordArray.GetSize(); i++)
		{
			NTC_RECORD ntc = m_NtcRecordArray.GetAt(i); 
			if(ntc.temp > max_temp)
			{
				max_temp = ntc.temp;
			}

			if(ntc.temp < min_temp)
			{
				min_temp = ntc.temp; 
			}

			if(ntc.NTC[this->m_NTC_ID] > max_res)
			{
				max_res = ntc.NTC[this->m_NTC_ID];
			}

			if(ntc.NTC[this->m_NTC_ID] < min_res)
			{
				min_res = ntc.NTC[this->m_NTC_ID];
			}
		}
		
		max_res *= 1.2f;
		min_res *= 0.8f;
		

		CString s;
		MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));
		MemDC.SetBkMode(TRANSPARENT);
		
		s.Format(_T(" 最高温度:% 6.2f℃ 最低温度:% 6.2f℃,最大电阻:% 8.2f 最小电阻:% 8.2f"),max_temp,min_temp,max_res,min_res);
		s = start_time + s;
		max_temp += 20.0f;
		min_temp -= 20.0f;

		CFont font,*oldFont;
		CSize FontSize;
		int FontHeight = (ChartRect.Height()*10)/30;
		if(font.CreatePointFont(FontHeight,_T("黑体"),&MemDC))
		{
			oldFont = MemDC.SelectObject(&font);
		}
		else
		{
			return;
		}
		
		while(1)
		{
			s = _T("888888");
			 FontSize = MemDC.GetTextExtent(s);
			 if(FontSize.cx < ChartRect.Width() * 0.1 / 3)
			 {
				 break;
			 }
			 else
			 {
				MemDC.SelectObject(oldFont);
				font.DeleteObject(); 
				FontHeight -= 20;
				if(font.CreatePointFont(FontHeight,_T("黑体"),&MemDC))
				{
					oldFont = MemDC.SelectObject(&font);
				}
				else
				{
					return;
				}
			 }
			 
		}
		 FontSize = MemDC.GetTextExtent(_T("888.8个"));
		float y_gap = (max_res - min_res) / 5;
		
		pen.DeleteObject(); 
		pen.CreatePen(PS_DOT,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);

		//MemDC.DrawText(s,CRect(0,0,ChartRect.Width(),FontSize.cx),DT_CENTER);
		s = _T("电阻温度曲线   ") +s;
		this->SetWindowTextW(s); 

		for(int i = 0; i <= 5; i++)
		{
			s.Format(_T("% 8.2f"),min_res + y_gap * i);
			if(i < 5)
			{
				CRect rect;
				rect.left	= 0;
				rect.right	= FontSize.cx;
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

		
		float x_gap;

		//X-0C-->100C X轴是0到100°
		x_gap = (float)(ChartRect.Width()) / 100.0f; 
	
		y_gap = (float)(ChartRect.Height() / (float)(max_res - min_res));
		


		pen.DeleteObject(); 
		pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0,0));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		for(int i = 0; i < m_NtcRecordArray.GetSize() - 2; i++)
		{
			NTC_RECORD ntc = m_NtcRecordArray.GetAt(i); 
			NTC_RECORD ntc2 = m_NtcRecordArray.GetAt(i+1); 

			MemDC.MoveTo((int)(ChartRect.left + ntc.temp * x_gap),    (int)(ChartRect.Height() - ( ntc.NTC[m_NTC_ID]    - min_res) * y_gap));
		
			MemDC.LineTo((int)(ChartRect.left +ntc2.temp * x_gap),    (int)(ChartRect.Height() - (ntc2.NTC[m_NTC_ID]    - min_res) * y_gap));
			 
		}

		

	
	dc->BitBlt(ChartRect.left,ChartRect.top,ChartRect.Width(),ChartRect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形
	this->UpdateData(false); 


	MemDC.SelectObject(oldbrush);
	
	if(font.GetSafeHandle() != NULL)
	{
		font.DeleteObject();
	}

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
	
	ReleaseDC(dc); 


}

void CCurveDlg::DrawTempCurve(int BoxNr)
{
	CFile log_file;
	CString m_TestMsg;

	if(!this->IsWindowVisible()) 
	{
		return;
	}

	if(this->m_NTC_ID >= 0)
	{
		DrawNtcTempCurve();
		return;
	}

		CRect ChartRect;
#ifdef WINCE
	this->GetWindowRect(ChartRect);

	ScreenToClient(&ChartRect);
#else
	GetClientRect(&ChartRect);
	ChartRect.top += ChartRect.Height() / 20; 
#endif

	//if(G_TempArray[BoxNr].GetSize() > 0)
	{
		::DrawTempCurve(this,ChartRect,&G_TempArray[BoxNr],m_BeginTestTime);
	}

	if(G_TempArray_8TEMP_8SSR[BoxNr].GetSize() > 0)
	{
		::DrawTempCurve(this,ChartRect,&G_TempArray_8TEMP_8SSR[BoxNr],m_BeginTestTime);
	}

}
void CCurveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CCurveDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
/*
	HDC hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);  
	WriteBmp(_T("\\USB HardDisk\\TempCurve.bmp"),hdc);  
	DeleteDC(hdc);  
*/
	
	if(G_TempArray_8TEMP_8SSR[0].GetSize() > 0)
	{
	
		CFile f;
		char buf[200];
		TEMP_RECORD_8TEMP_8SSR data;

		if(f.Open(_T("\\USB HardDisk\\a.csv"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			for(int i = 0; i < G_TempArray_8TEMP_8SSR[0].GetSize(); i++)
			{
				data = G_TempArray_8TEMP_8SSR[0].GetAt(i);
				memset(buf,0,sizeof(buf));
				sprintf(buf,"%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,%d,%X\r\n",
					data.Answer_8TEMP_8SSR.Pv[0] / 100.0f,
					data.Answer_8TEMP_8SSR.Pv[1] / 100.0f,
					data.Answer_8TEMP_8SSR.Pv[2] / 100.0f,
					data.Answer_8TEMP_8SSR.Pv[3] / 100.0f,
					data.Answer_8TEMP_8SSR.Pv[4] / 100.0f,
					data.Answer_8TEMP_8SSR.Pv[5] / 100.0f,
					data.Answer_8TEMP_8SSR.Pv[6] / 100.0f,
					data.Answer_8TEMP_8SSR.Pv[7] / 100.0f,
					data.Answer_8TEMP_8SSR.Sv[3] / 10.0f,
					data.Answer_8TEMP_8SSR.Sv[7] / 10.0f,
					data.Answer_8TEMP_8SSR.TestMode[0],
					data.Answer_8TEMP_8SSR.Error);
				f.Write(buf,strlen(buf));
			}
			f.Close();
			AfxMessageBox(_T("保存曲线数据 成功!" ));
		}
	}
	CDialog::OnClose();
}

void CCurveDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	DrawTempCurve(0);
}

void CCurveDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
}
