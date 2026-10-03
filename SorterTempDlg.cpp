// SortingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SorterTempDlg.h"
#include "comm.h"
#include "common.h"
#include "inputdlg.h"
#include "math.h"

#ifdef WINCE
#include "CurveDlg.h"
#endif

CSorterTempDlg							G_SorterTempDlg;

static int EDIT_ID[8][10] = 
	{
		{IDC_EDIT_PV_1,IDC_EDIT_SV_1,IDC_EDIT_PID_P_1,IDC_EDIT_PID_I_1,IDC_EDIT_PID_D_1,IDC_EDIT_MSG_1,IDC_EDIT_ADJUST_1,IDC_STATIC_PV1,IDC_STATIC_SV1,IDC_STATIC_ADJUST_1},
		{IDC_EDIT_PV_2,IDC_EDIT_SV_2,IDC_EDIT_PID_P_2,IDC_EDIT_PID_I_2,IDC_EDIT_PID_D_2,IDC_EDIT_MSG_2,IDC_EDIT_ADJUST_2,IDC_STATIC_PV2,IDC_STATIC_SV2,IDC_STATIC_ADJUST_2},
		{IDC_EDIT_PV_3,IDC_EDIT_SV_3,IDC_EDIT_PID_P_3,IDC_EDIT_PID_I_3,IDC_EDIT_PID_D_3,IDC_EDIT_MSG_3,IDC_EDIT_ADJUST_3,IDC_STATIC_PV3,IDC_STATIC_SV3,IDC_STATIC_ADJUST_3},
		{IDC_EDIT_PV_4,IDC_EDIT_SV_4,IDC_EDIT_PID_P_4,IDC_EDIT_PID_I_4,IDC_EDIT_PID_D_4,IDC_EDIT_MSG_4,IDC_EDIT_ADJUST_4,IDC_STATIC_PV4,IDC_STATIC_SV4,IDC_STATIC_ADJUST_4},
		{IDC_EDIT_PV_5,IDC_EDIT_SV_5,IDC_EDIT_PID_P_5,IDC_EDIT_PID_I_5,IDC_EDIT_PID_D_5,IDC_EDIT_MSG_5,IDC_EDIT_ADJUST_5,IDC_STATIC_PV5,IDC_STATIC_SV5,IDC_STATIC_ADJUST_5},
		{IDC_EDIT_PV_6,IDC_EDIT_SV_6,IDC_EDIT_PID_P_6,IDC_EDIT_PID_I_6,IDC_EDIT_PID_D_6,IDC_EDIT_MSG_6,IDC_EDIT_ADJUST_6,IDC_STATIC_PV6,IDC_STATIC_SV6,IDC_STATIC_ADJUST_6},
		{IDC_EDIT_PV_7,IDC_EDIT_SV_7,IDC_EDIT_PID_P_7,IDC_EDIT_PID_I_7,IDC_EDIT_PID_D_7,IDC_EDIT_MSG_7,IDC_EDIT_ADJUST_7,IDC_STATIC_PV7,IDC_STATIC_SV7,IDC_STATIC_ADJUST_7},
		{IDC_EDIT_PV_8,IDC_EDIT_SV_8,IDC_EDIT_PID_P_8,IDC_EDIT_PID_I_8,IDC_EDIT_PID_D_8,IDC_EDIT_MSG_8,IDC_EDIT_ADJUST_8,IDC_STATIC_PV8,IDC_STATIC_SV8,IDC_STATIC_ADJUST_8},

	};

// CSorterTempDlg 对话框

IMPLEMENT_DYNAMIC(CSorterTempDlg, CDialog)

CSorterTempDlg::CSorterTempDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSorterTempDlg::IDD, pParent)
{

}

CSorterTempDlg::~CSorterTempDlg()
{
}

void CSorterTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSorterTempDlg, CDialog)
	ON_BN_CLICKED(IDC_OK, &CSorterTempDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_SV_4, &CSorterTempDlg::OnEnSetfocusEditSv4)
	ON_EN_SETFOCUS(IDC_EDIT_SV_8, &CSorterTempDlg::OnEnSetfocusEditSv8)
	ON_EN_SETFOCUS(IDC_EDIT_PID_P_1, &CSorterTempDlg::OnEnSetfocusEditPidP1)
	ON_EN_SETFOCUS(IDC_EDIT_PID_I_1, &CSorterTempDlg::OnEnSetfocusEditPidI1)
	ON_EN_SETFOCUS(IDC_EDIT_PID_D_1, &CSorterTempDlg::OnEnSetfocusEditPidD1)

	ON_EN_SETFOCUS(IDC_EDIT_PID_P_2, &CSorterTempDlg::OnEnSetfocusEditPidP2)
	ON_EN_SETFOCUS(IDC_EDIT_PID_I_2, &CSorterTempDlg::OnEnSetfocusEditPidI2)
	ON_EN_SETFOCUS(IDC_EDIT_PID_D_2, &CSorterTempDlg::OnEnSetfocusEditPidD2)

	ON_EN_SETFOCUS(IDC_EDIT_PID_P_3, &CSorterTempDlg::OnEnSetfocusEditPidP3)
	ON_EN_SETFOCUS(IDC_EDIT_PID_I_3, &CSorterTempDlg::OnEnSetfocusEditPidI3)
	ON_EN_SETFOCUS(IDC_EDIT_PID_D_3, &CSorterTempDlg::OnEnSetfocusEditPidD3)

	ON_EN_SETFOCUS(IDC_EDIT_PID_P_4, &CSorterTempDlg::OnEnSetfocusEditPidP4)
	ON_EN_SETFOCUS(IDC_EDIT_PID_I_4, &CSorterTempDlg::OnEnSetfocusEditPidI4)
	ON_EN_SETFOCUS(IDC_EDIT_PID_D_4, &CSorterTempDlg::OnEnSetfocusEditPidD4)

	ON_EN_SETFOCUS(IDC_EDIT_PID_P_5, &CSorterTempDlg::OnEnSetfocusEditPidP5)
	ON_EN_SETFOCUS(IDC_EDIT_PID_I_5, &CSorterTempDlg::OnEnSetfocusEditPidI5)
	ON_EN_SETFOCUS(IDC_EDIT_PID_D_5, &CSorterTempDlg::OnEnSetfocusEditPidD5)

	ON_EN_SETFOCUS(IDC_EDIT_PID_P_6, &CSorterTempDlg::OnEnSetfocusEditPidP6)
	ON_EN_SETFOCUS(IDC_EDIT_PID_I_6, &CSorterTempDlg::OnEnSetfocusEditPidI6)
	ON_EN_SETFOCUS(IDC_EDIT_PID_D_6, &CSorterTempDlg::OnEnSetfocusEditPidD6)

	ON_EN_SETFOCUS(IDC_EDIT_PID_P_7, &CSorterTempDlg::OnEnSetfocusEditPidP7)
	ON_EN_SETFOCUS(IDC_EDIT_PID_I_7, &CSorterTempDlg::OnEnSetfocusEditPidI7)
	ON_EN_SETFOCUS(IDC_EDIT_PID_D_7, &CSorterTempDlg::OnEnSetfocusEditPidD7)

	ON_EN_SETFOCUS(IDC_EDIT_PID_P_8, &CSorterTempDlg::OnEnSetfocusEditPidP8)
	ON_EN_SETFOCUS(IDC_EDIT_PID_I_8, &CSorterTempDlg::OnEnSetfocusEditPidI8)
	ON_EN_SETFOCUS(IDC_EDIT_PID_D_8, &CSorterTempDlg::OnEnSetfocusEditPidD8)


	ON_EN_SETFOCUS(IDC_EDIT_PV_4, &CSorterTempDlg::OnEnSetfocusEditPV4)
	ON_EN_SETFOCUS(IDC_EDIT_PV_8, &CSorterTempDlg::OnEnSetfocusEditPV8)

	ON_EN_SETFOCUS(IDC_EDIT_ADJUST_4, &CSorterTempDlg::OnEnSetfocusEditAdjust4)
	ON_EN_SETFOCUS(IDC_EDIT_ADJUST_8, &CSorterTempDlg::OnEnSetfocusEditAdjust8)

END_MESSAGE_MAP()


// CSorterTempDlg 消息处理程序

void CSorterTempDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


BOOL CSorterTempDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->ShowWindow(SW_SHOWMAXIMIZED);
	CRect WndRect;
	this->GetWindowRect(WndRect);
	CWnd *w;
	
	CRect rect;

	int line_h = WndRect.Height()	/  10;

	int	group_w =  WndRect.Width()	/  4;

	int	group_h =  WndRect.Height()	/  2;

	int edit_w = group_w	/  4;
	int edit_h = WndRect.Height()	/  12;

	rect.left	=	0;
	rect.top	=	0;
	rect.right	=	WndRect.right / 2;
	rect.bottom	=	WndRect.bottom	/ 2;
	
	CString s;

	for(int i = 0; i < 8; i++)
	{
		w = this->GetDlgItem(EDIT_ID[i][7]);//PV
		w->MoveWindow(group_w * (i % 4)+edit_w * 0,line_h * 0 + group_h *( i /4),edit_w * 1,edit_h);
		s.Format(_T("PV%d"),i+1);
		w->SetWindowTextW(s);

		w = this->GetDlgItem(EDIT_ID[i][8]);//SV
		w->MoveWindow(group_w * (i % 4)+edit_w * 0,line_h * 1 + group_h *( i /4),edit_w * 1,edit_h);
		s.Format(_T("SV%d"),i+1);
		w->SetWindowTextW(s);

		w = this->GetDlgItem(EDIT_ID[i][0]);
		w->MoveWindow(group_w * (i % 4)+edit_w * 1,line_h * 0 + group_h *( i /4),edit_w * 1.8,edit_h);

		w = this->GetDlgItem(EDIT_ID[i][9]);
		w->MoveWindow(group_w * (i % 4)+edit_w * 2.8,line_h * 0 + group_h *( i /4),edit_w*1.2,edit_h);
		w = this->GetDlgItem(EDIT_ID[i][6]);
		w->MoveWindow(group_w * (i % 4)+edit_w * 2.8,line_h * 1 + group_h *( i /4),edit_w*1.2,edit_h);
		if(i != 3 && i != 7)
		{
			w->EnableWindow(false);
		}
		w = this->GetDlgItem(EDIT_ID[i][1]);
		w->MoveWindow(group_w * (i % 4)+edit_w * 1,line_h * 1 + group_h *( i /4) ,edit_w * 1.8,edit_h);

		w = this->GetDlgItem(EDIT_ID[i][2]);
		w->MoveWindow(group_w * (i % 4)+edit_w * 0,line_h * 2 + group_h *( i /4) ,edit_w,edit_h);

		w = this->GetDlgItem(EDIT_ID[i][3]);
		w->MoveWindow(group_w * (i % 4)+edit_w * 1.3,line_h * 2 + group_h *( i /4) ,edit_w,edit_h);

		w = this->GetDlgItem(EDIT_ID[i][4]);
		w->MoveWindow(group_w * (i % 4)+edit_w * 2.6,line_h * 2 + group_h *( i /4) ,edit_w,edit_h);

		w = this->GetDlgItem(EDIT_ID[i][5]);
		w->MoveWindow(group_w * (i % 4)+edit_w * 0,line_h * 3 + group_h *( i /4) ,group_w,edit_h);
	}


	w = this->GetDlgItem(IDC_EDIT_SV_1);
	w->EnableWindow(false);
	w = this->GetDlgItem(IDC_EDIT_SV_2);
	w->EnableWindow(false);
	w = this->GetDlgItem(IDC_EDIT_SV_3);
	w->EnableWindow(false);
	w = this->GetDlgItem(IDC_EDIT_SV_5);
	w->EnableWindow(false);
	w = this->GetDlgItem(IDC_EDIT_SV_6);
	w->EnableWindow(false);
	w = this->GetDlgItem(IDC_EDIT_SV_7);
	w->EnableWindow(false);

	w = this->GetDlgItem(IDC_OK);
	w->MoveWindow((WndRect.Width()/2 - edit_w),WndRect.Height() - edit_h ,edit_w*2,edit_h);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSorterTempDlg::DealCmdAnswer(unsigned char *para, unsigned char *flash,bool IsCmdAscii,int data_len)
{


	CString s;

	m_MeterAnswerTime = CTime::GetCurrentTime();

	memcpy(&m_Answer_8TEMP_8SSR,para,sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER));

	MODBUS_RS485_CMD_128					*modbus_cmd_128				=	(MODBUS_RS485_CMD_128*) para;
	

	unsigned char cmd,temp;
	
	
	cmd  = modbus_cmd_128->head.cmd;
	
	CWnd * w;
	short *val = (short *)modbus_cmd_128->para;

			



	switch(cmd)
	{
	case CMD_SET_ADJUST_ALL:
		break;
	case CMD_READ_ADJUST_ALL:
		for(int i = 0; i < 8; i++)
		{
			w = this->GetDlgItem(EDIT_ID[i][6]);
			s.Format(_T("%4.1f"),m_Answer_8TEMP_8SSR.Pv[i] / 10.0f);
			w->SetWindowTextW(s);
		}
		break;
		case CMD_READ_PID_T:
			
			if(data_len == sizeof(MODBUS_RS485_CMD_128))
			{
				
				
				short val[8];
				memcpy(val,modbus_cmd_128->para,sizeof(val));
			
				temp  = val[7];
	
				s.Format(_T("%d"),val[0]);
				w = this->GetDlgItem(EDIT_ID[temp][2]);
				w->SetWindowTextW(s);

				s.Format(_T("%d"),val[1]);
				w = this->GetDlgItem(EDIT_ID[temp][3]);
				w->SetWindowTextW(s);

				s.Format(_T("%d"),val[2]);
				w = this->GetDlgItem(EDIT_ID[temp][4]);
				w->SetWindowTextW(s);

				
			}
			break;
	
		case CMD_READ_TEMP:
			
				
			if(data_len == sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER))
			{
				
				for(int i = 0; i < 8; i++)
				{
					if(m_Answer_8TEMP_8SSR.Error & (1 << i))
					{
						w = this->GetDlgItem(EDIT_ID[i][0]);
						w->SetWindowTextW(_T("错误"));
					}
					else
					{
						w = this->GetDlgItem(EDIT_ID[i][0]);
						s.Format(_T("%5.2f"),m_Answer_8TEMP_8SSR.Pv[i] / 100.0f);
						w->SetWindowTextW(s);
					}

					w = this->GetDlgItem(EDIT_ID[i][1]);
					s.Format(_T("%5.1f"),m_Answer_8TEMP_8SSR.Sv[i] / 10.0f);
					w->SetWindowTextW(s);

					
					w = this->GetDlgItem(EDIT_ID[i][5]);
					s.Format(_T("%d%%"),m_Answer_8TEMP_8SSR.HeatPercent[i]);
					w->SetWindowTextW(s);
				}
			
			}
			
				
		
		break;


	}

}

HBRUSH CSorterTempDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	switch(pWnd->GetDlgCtrlID())
	{

		case IDC_EDIT_PV_1:
		case IDC_EDIT_PV_2:
		case IDC_EDIT_PV_3:
		case IDC_EDIT_PV_4:
		case IDC_EDIT_PV_5:
		case IDC_EDIT_PV_6:
		case IDC_EDIT_PV_7:
		case IDC_EDIT_PV_8:
		
			pDC->SetTextColor(RGB(255,0, 0));
	
		break;

		case IDC_EDIT_SV_1:
		case IDC_EDIT_SV_2:
		case IDC_EDIT_SV_3:
		case IDC_EDIT_SV_4:
		case IDC_EDIT_SV_5:
		case IDC_EDIT_SV_6:
		case IDC_EDIT_SV_7:
		case IDC_EDIT_SV_8:
		
			pDC->SetTextColor(RGB(0,255, 0));
	
		break;

	}

	return hbr;
}

void CSorterTempDlg::SendSv(void)
{
		TCHAR	buf[100];

		CWnd *edit;
		float temp[8];

		for(int i = 0; i < 8; i++)
		{
			edit=	(CEdit*)this->GetDlgItem(EDIT_ID[i][1]);
			edit->GetWindowTextW(buf,100); 
	
			temp[i] = _tcstod(buf,NULL);
		}
		G_RS485_Comm.SendNormalPidCmd_8PT100_8SSR(temp,0);

		::G_TempArray_8TEMP_8SSR[0].RemoveAll();

}


void CSorterTempDlg::OnEnSetfocusEditSv4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_PV_1);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入设置温度值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
		CString s;
		TCHAR buf[100];

		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_SV_4);
		edit->SetWindowTextW(dlg.m_Input); 

		edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_4);
		edit->GetWindowTextW(buf,100); 

		float temp = _tcstod(buf,NULL);

		edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_3);
		s.Format(_T("%5.1f"),temp - 2.0f);
		edit->SetWindowTextW(s);

		edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_2);
		s.Format(_T("%5.1f"),temp - 3.0f);
		edit->SetWindowTextW(s);

		edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_1);
		s.Format(_T("%5.1f"),temp - 4.0f);
		edit->SetWindowTextW(s);


		SendSv();

		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[0] = temp - SV_8TEMP_8SSR_DELTA_1;
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[1] = temp - SV_8TEMP_8SSR_DELTA_2;
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[2] = temp - SV_8TEMP_8SSR_DELTA_3;
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[3] = temp - SV_8TEMP_8SSR_DELTA_4;

		::SaveNormalConfigPara();

	}

}



void CSorterTempDlg::OnEnSetfocusEditSv8()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_PV_1);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入设置温度值");

	if(dlg.DoModal() == IDOK)
	{
		
		dlg.m_Input.Trim(); 
		CWnd *edit;
		CString s;
		TCHAR buf[100];

		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_SV_8);
		edit->SetWindowTextW(dlg.m_Input); 

		edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_8);
		edit->GetWindowTextW(buf,100); 

		float temp = _tcstod(buf,NULL);

		edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_7);
		s.Format(_T("%5.1f"),temp - 2.0f);
		edit->SetWindowTextW(s);

		edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_6);
		s.Format(_T("%5.1f"),temp - 3.0f);
		edit->SetWindowTextW(s);

		edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_5);
		s.Format(_T("%5.1f"),temp - 4.0f);
		edit->SetWindowTextW(s);
		SendSv();

		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[4] = temp - SV_8TEMP_8SSR_DELTA_4;
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[5] = temp - SV_8TEMP_8SSR_DELTA_5;
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[6] = temp - SV_8TEMP_8SSR_DELTA_6;
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[7] = temp - SV_8TEMP_8SSR_DELTA_7;

		::SaveNormalConfigPara();
	}

}

void CSorterTempDlg::SendPIDT(int ID)
{
		TCHAR	buf[100];

		CWnd *edit;
		short temp[8];
		memset(temp,0,sizeof(temp));
		for(int i = 0; i < 3; i++)
		{
			edit=	(CEdit*)this->GetDlgItem(EDIT_ID[ID][2+i]);
			edit->GetWindowTextW(buf,100); 
			
			temp[i] = _tcstod(buf,NULL);
			temp[i+4] = _tcstod(buf,NULL);
			
		}
		temp[3] = 1;
		CString s;
			s.Format(_T("%d,%d,%d"),temp[0],temp[1],temp[2]);
			AfxMessageBox(s);
		G_RS485_Comm.SetPID_T(temp,0,ID);
}

void CSorterTempDlg::OnEnSetfocusEditPidP1()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 P 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_P_1);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(0);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidI1()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_I_1);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(0);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidD1()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_D_1);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(0);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidP2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 P 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_P_2);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(1);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidI2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_I_2);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(1);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidD2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_D_2);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(1);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidP3()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 P 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_P_3);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(2);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidI3()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_I_3);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(2);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidD3()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_D_3);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(2);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidP4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 P 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_P_4);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(3);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidI4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_I_4);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(3);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidD4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_D_4);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(3);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidP5()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 P 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_P_5);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(4);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidI5()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_I_5);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(4);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidD5()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_D_5);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(4);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidP6()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 P 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_P_6);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(5);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidI6()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_I_6);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(5);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidD6()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_D_6);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(5);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidP7()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 P 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_P_7);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(6);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidI7()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_I_7);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(6);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidD7()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_D_7);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(6);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidP8()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 P 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_P_8);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(7);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidI8()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 I 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_I_8);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(7);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPidD8()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 D 值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PID_D_8);
		edit->SetWindowTextW(dlg.m_Input); 

		SendPIDT(7);
	}
}

void CSorterTempDlg::OnEnSetfocusEditPV4()
{
#ifdef WINCE
	G_CurveDlg.m_NTC_ID = -1;
	G_CurveDlg.DoModal();  
#endif
}

void CSorterTempDlg::OnEnSetfocusEditPV8()
{
#ifdef WINCE
	G_CurveDlg.m_NTC_ID = -1;
	G_CurveDlg.DoModal();  
#endif
}


void CSorterTempDlg::OnEnSetfocusEditAdjust4()
{
#ifdef WINCE
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 修正值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJUST_1);
		edit->SetWindowTextW(dlg.m_Input); 
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJUST_2);
		edit->SetWindowTextW(dlg.m_Input); 
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJUST_3);
		edit->SetWindowTextW(dlg.m_Input); 
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJUST_4);
		edit->SetWindowTextW(dlg.m_Input); 

		SendAdjust();
	}
#endif
}

void CSorterTempDlg::SendAdjust(void)
{
	TCHAR s[100];
	CWnd *w;
	float val[8];

	for(int i = 0; i < 8; i++)
	{
		w = this->GetDlgItem(EDIT_ID[i][6]);

		w->GetWindowText(s,100);
		val[i]  = _tcstod(s,NULL);
	}

	
	G_RS485_Comm.SetAdjustAll(val ,0);
	G_RS485_Comm.ReadAdjustAll(0);

}
void CSorterTempDlg::OnEnSetfocusEditAdjust8()
{
#ifdef WINCE
	CWnd *w = this->GetDlgItem(IDC_EDIT_MSG_8);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 修正值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJUST_5);
		edit->SetWindowTextW(dlg.m_Input); 
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJUST_6);
		edit->SetWindowTextW(dlg.m_Input); 
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJUST_7);
		edit->SetWindowTextW(dlg.m_Input); 
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJUST_8);
		edit->SetWindowTextW(dlg.m_Input); 

		SendAdjust();
	}
#endif
}

float fabsf_temp[8+8];

bool CSorterTempDlg::IsTempOK(int ID)
{

	CTime cur = CTime::GetCurrentTime();
	CTimeSpan span = cur - m_MeterAnswerTime;
	if(span.GetTotalSeconds() > 60)
	{//没有了通信
		return false;
	}
	int counts;
#ifdef WINCE 
		counts = 100;
#else
		counts = 40;
#endif

	if(G_TempArray_8TEMP_8SSR[0].GetSize() < counts)
	{
	
		return false;
	}

	float avr = 0;
	float max = -999.9f;
	float min =  999.9f;

	TEMP_RECORD_8TEMP_8SSR temp_record;

	for(int i = G_TempArray_8TEMP_8SSR[0].GetSize() - counts; i < G_TempArray_8TEMP_8SSR[0].GetSize(); i++)
	{
			
			temp_record = G_TempArray_8TEMP_8SSR[0].GetAt(i);

			{//最后200个数据的最大最小值
					if(temp_record.Answer_8TEMP_8SSR.Pv[ID]/100.0f   < min)
					{
						min = temp_record.Answer_8TEMP_8SSR.Pv[ID]/100.0f;
					}
					
					if(temp_record.Answer_8TEMP_8SSR.Pv[ID]/100.0f  > max)
					{
						max = temp_record.Answer_8TEMP_8SSR.Pv[ID]/100.0f;
					}
					avr += temp_record.Answer_8TEMP_8SSR.Pv[ID]/100.0f;

				
			}
	}

	avr /= counts;


	switch(ID)
	{
	case 0:

		if(this->GetSafeHwnd() && this->IsWindowVisible())
			{
				CWnd *w;
				CString s;
				s.Format(_T("%5.1f"),avr);
				w = this->GetDlgItem(IDC_EDIT_PID_P_2);
				w->SetWindowTextW(s);
				s.Format(_T("%5.1f"),temp_record.Answer_8TEMP_8SSR.Sv[0] / 10.0f);
				w = this->GetDlgItem(IDC_EDIT_PID_I_2);
				w->SetWindowTextW(s);

			}

		fabsf_temp[0] = /*fabsf*/(avr - temp_record.Answer_8TEMP_8SSR.Sv[0] / 10.0f);

		if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[0] / 10.0f) > SV_8TEMP_8SSR_DELTA_1)
		{
			
			return false;
		}
		break;
		case 1:
			fabsf_temp[1] = /*fabsf*/(avr - temp_record.Answer_8TEMP_8SSR.Sv[1] / 10.0f);
		if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[1] / 10.0f) > SV_8TEMP_8SSR_DELTA_2)
		{
			return false;
		}
		
		break;
		case 2:
			fabsf_temp[2] = /*fabsf*/(avr - temp_record.Answer_8TEMP_8SSR.Sv[2] / 10.0f);
		if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[2] / 10.0f) > SV_8TEMP_8SSR_DELTA_3)
		{
			return false;
		}
		break;
		case 3:
			fabsf_temp[3] = /*fabsf*/(avr - temp_record.Answer_8TEMP_8SSR.Sv[3] / 10.0f);
			fabsf_temp[8] = /*fabsf*/(max - min);
			fabsf_temp[10] = max;
			fabsf_temp[11] = min;

		//if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[3] / 10.0f) > 0.2f)
		if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[3] / 10.0f) > 0.5f)
		{
			return false;
		}
		
		//if(fabsf(max - min) > 0.3)
		if(fabsf(max - min) > 0.8)
		{
			return false;
		}
		break;

		case 4:
			fabsf_temp[4] = /*fabsf*/(avr - temp_record.Answer_8TEMP_8SSR.Sv[4] / 10.0f);
		if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[4] / 10.0f) > SV_8TEMP_8SSR_DELTA_5)
		{
			return false;
		}
		break;
		case 5:
			fabsf_temp[5] = /*fabsf*/(avr - temp_record.Answer_8TEMP_8SSR.Sv[5] / 10.0f);
		if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[5] / 10.0f) > SV_8TEMP_8SSR_DELTA_6)
		{
			return false;
		}
		break;
		case 6:
			fabsf_temp[6] = /*fabsf*/(avr - temp_record.Answer_8TEMP_8SSR.Sv[6] / 10.0f);
		if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[6] / 10.0f) > SV_8TEMP_8SSR_DELTA_7)
		{
			return false;
		}
		break;
		case 7:

			fabsf_temp[7] = /*fabsf*/(avr - temp_record.Answer_8TEMP_8SSR.Sv[7] / 10.0f);

			fabsf_temp[9] = /*fabsf*/(max - min);
			fabsf_temp[12] = max;
			fabsf_temp[13] = min;

			if(this->GetSafeHwnd() && this->IsWindowVisible())
			{
				CWnd *w;
				CString s;
				s.Format(_T("%5.1f"),avr);
				w = this->GetDlgItem(IDC_EDIT_PID_P_8);
				w->SetWindowTextW(s);
				s.Format(_T("%5.1f"),temp_record.Answer_8TEMP_8SSR.Sv[7] / 10.0f);
				w = this->GetDlgItem(IDC_EDIT_PID_I_8);
				w->SetWindowTextW(s);

				s.Format(_T("%5.1f"),max);
				w = this->GetDlgItem(IDC_EDIT_PID_D_8);
				w->SetWindowTextW(s);

				s.Format(_T("%5.1f"),min);
				w = this->GetDlgItem(IDC_EDIT_PID_P_7);
				w->SetWindowTextW(s);

			}

		//if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[7] / 10.0f) > 0.2f)
		if(fabsf(avr - temp_record.Answer_8TEMP_8SSR.Sv[7] / 10.0f) > 0.5f)
		{
			return false;
		}
		//if(fabsf(max - min) > 0.3)
		if(fabsf(max - min) > 0.8)
		{
			return false;
		}
		break;

	}

	return true;
}
void CSorterTempDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	this->IsTempOK(0);
	this->IsTempOK(7);
}
