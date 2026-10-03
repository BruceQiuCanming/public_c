// SorterPlcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SorterPlcDlg.h"
#include "comm.h"
#include "atlimage.h"
#include "SorterTempDlg.h"
#include "InputDlg.h"
#include "common.h"


CSorterPlcDlg							G_SorterPlcDlg;

// CSorterPlcDlg 对话框
extern	CBrush		G_bkBrush;
#define BK_COLOR	RGB(0, 160, 128)

IMPLEMENT_DYNAMIC(CSorterPlcDlg, CDialog)

CSorterPlcDlg::CSorterPlcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSorterPlcDlg::IDD, pParent)
{
	m_AlarmMsg =_T("");
	m_X_Status = 0;
	m_ManualMode	= 0;
}

CSorterPlcDlg::~CSorterPlcDlg()
{
}

void CSorterPlcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSorterPlcDlg, CDialog)
		ON_BN_CLICKED(IDC_RUN, &CSorterPlcDlg::OnBnClickedButtonRun)
		ON_STN_CLICKED(IDC_STATIC_LOW_LEVEL_FAIL_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticLowLevelFailCyclinde)
		ON_WM_CTLCOLOR()
		ON_WM_PAINT()
		ON_STN_CLICKED(IDC_STATIC_LOW_LEVEL_TEST_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticLowLevelTestCyclinder)
		ON_STN_CLICKED(IDC_STATIC_LOW_LEVEL_INPUT_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticLowLevelInputCyclinder)
		ON_STN_CLICKED(IDC_STATIC_LOW_LEVEL_OUTPUT_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticLowLevelOutputCyclinder)
		ON_STN_CLICKED(IDC_STATIC_HIGH_LEVEL_OUTPUT_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticHighLevelOutputCyclinder)
		ON_STN_CLICKED(IDC_STATIC_HIGH_LEVEL_FAIL_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticHighLevelFailCyclinder)
		ON_STN_CLICKED(IDC_STATIC_HIGH_LEVEL_PASS_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticHighLevelPassCyclinder)
		ON_STN_CLICKED(IDC_STATIC_HIGH_LEVEL_TEST_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticHighLevelTestCyclinder)
		ON_BN_CLICKED(IDC_STOP, &CSorterPlcDlg::OnBnClickedStop)
		ON_BN_CLICKED(IDC_RECOVER, &CSorterPlcDlg::OnBnClickedRecover)
		ON_BN_CLICKED(IDC_AUTO, &CSorterPlcDlg::OnBnClickedAuto)
		ON_BN_CLICKED(IDOK, &CSorterPlcDlg::OnBnClickedOk)
		ON_BN_CLICKED(IDC_SET_TEMP, &CSorterPlcDlg::OnBnClickedSetTemp)
		ON_WM_TIMER()
		ON_STN_CLICKED(IDC_STATIC_HIGH_LEVEL_INPUT_CYCLINDER, &CSorterPlcDlg::OnStnClickedStaticHighLevelInputCyclinder)
		ON_EN_SETFOCUS(IDC_EDIT_SV_LOW, &CSorterPlcDlg::OnEnSetfocusEditSvLow)
		ON_EN_SETFOCUS(IDC_EDIT_SV_HIGH, &CSorterPlcDlg::OnEnSetfocusEditSvHigh)
		ON_EN_SETFOCUS(IDC_EDIT_ADJ_LOW, &CSorterPlcDlg::OnEnSetfocusEditAdjLow)
		ON_EN_SETFOCUS(IDC_EDIT_ADJ_HIGH, &CSorterPlcDlg::OnEnSetfocusEditAdjHigh)
		ON_EN_KILLFOCUS(IDC_EDIT_LOW_LEVEL, &CSorterPlcDlg::OnEnKillfocusEditLowLevel)
		ON_EN_SETFOCUS(IDC_EDIT_HIGH_LEVEL, &CSorterPlcDlg::OnEnSetfocusEditHighLevel)
		ON_EN_SETFOCUS(IDC_EDIT_MAIN_LEVEL, &CSorterPlcDlg::OnEnSetfocusEditMainLevel)
		ON_EN_CHANGE(IDC_EDIT_HIGH_LEVEL, &CSorterPlcDlg::OnEnChangeEditHighLevel)
		ON_EN_SETFOCUS(IDC_EDIT_LOW_LEVEL, &CSorterPlcDlg::OnEnSetfocusEditLowLevel)
		ON_STN_CLICKED(IDC_STATIC_IS_HAS_SWITCH, &CSorterPlcDlg::OnStnClickedStaticIsHasSwitch)
		ON_STN_CLICKED(IDC_STATIC_LOW_LEVEL_TEMP_1, &CSorterPlcDlg::OnStnClickedStaticLowLevelTemp1)
END_MESSAGE_MAP()


// CSorterPlcDlg 消息处理程序

void CSorterPlcDlg::OnBnClickedButtonRun()
{
	// 启动标志 0x0864,赋值 0x0000

	

	if(this->m_ManualMode != 0)
	{
		AfxMessageBox(_T("请切换到自动模式"));
		return;
	}

	OnBnClickedRecover();

	if(G_SorterTempDlg.IsTempOK(0)
		&& G_SorterTempDlg.IsTempOK(1)
		&& G_SorterTempDlg.IsTempOK(2)
		&& G_SorterTempDlg.IsTempOK(3)
		&& G_SorterTempDlg.IsTempOK(4)
		&& G_SorterTempDlg.IsTempOK(5)
		&& G_SorterTempDlg.IsTempOK(6)
		&& G_SorterTempDlg.IsTempOK(7))
	{
		this->m_RunMode = 1;
		G_PrintRS232_Comm.Delta_Set_M(0x64,1);
	}
	else
	{
		if(this->m_RunMode == 0)
		{
			AfxMessageBox(_T("等待到温度恒定。。。。。\r\n温度恒定后系统自动进入分选状态"));

			m_WaitForRunMode = 1;
		}

	}

	

	

}



HBRUSH CSorterPlcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	switch(nCtlColor)
	{

	case CTLCOLOR_BTN:	//    Button control
		hbr = G_bkBrush;
	
		break;
	case CTLCOLOR_EDIT:	//    Edit control
		switch(pWnd->GetDlgCtrlID())
		{
		case IDC_EDIT_ALARM_MSG:
			pDC->SetTextColor(RGB(255, 0, 0));
			break;
		case IDC_EDIT_PV_LOW:
		case IDC_EDIT_PV_HIGH:
				pDC->SetTextColor(RGB(255, 0, 0));
			break;
		case IDC_EDIT_SV_LOW:
		case IDC_EDIT_SV_HIGH:
				pDC->SetTextColor(RGB(0,255, 0));
			break;

		}
		 
		
		
		break;
	case CTLCOLOR_LISTBOX:	//    List-box control
		break;
	case CTLCOLOR_MSGBOX:	//    Message box
		hbr = G_bkBrush;
		break;

	case CTLCOLOR_STATIC:	//    Static control
	//	hbr = G_bkBrush;

		switch(pWnd->GetDlgCtrlID())
			
		{
			/*
#define	X_Has_Switch							X00
#define	X_Low_Level_Input_Cylinder				X06

#define 				X10
#define X_Low_Level_Test_Switch					X11

#define X_Low_Level_Output_Cylinder				X12		
#define X_Low_Level_Fail_Cylinder				X14


#define	X_High_Level_Input_Cylinder				X16

#define X_High_Level_Test_Cylinder				X21
#define X_High_Level_Test_Switch				X22

#define X_High_Level_Output_Cylinder			X23
#define X_High_Level_Selected_Pass_Cylinder		X25
#define X_High_Level_Selected_Fail_Cylinder		X26
#define X_High_Volt_Pass						X27
			*/
		case IDC_STATIC_LOW_LEVEL_INPUT_CYCLINDER:
			if(m_X_Status & (1 << X_Low_Level_Input_Cylinder))
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 255, 255)); 
			}
			break;

			case  IDC_STATIC_LOW_LEVEL_TEMP_1:
			
			if(::G_SorterTempDlg.IsTempOK(0))
			{
				pDC->SetBkColor(RGB(0, 255, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
				
			}	
			break;

			case IDC_STATIC_LOW_LEVEL_TEMP_2:
			if(::G_SorterTempDlg.IsTempOK(1))
			{
				pDC->SetBkColor(RGB(0, 255, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
				
			}	
			break;
			case IDC_STATIC_LOW_LEVEL_TEMP_3:
				if(::G_SorterTempDlg.IsTempOK(2))
			{
				pDC->SetBkColor(RGB(0, 255, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
				
			}	
			
			break;
			case IDC_STATIC_LOW_LEVEL_TEMP_4:
				if(::G_SorterTempDlg.IsTempOK(3))
			{
				pDC->SetBkColor(RGB(0, 255, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
				
			}	
			
			break;
			case IDC_STATIC_HIGH_LEVEL_TEMP_1:
				if(::G_SorterTempDlg.IsTempOK(4))
			{
				pDC->SetBkColor(RGB(0, 255, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
				
			}	
			
			break;
			case IDC_STATIC_HIGH_LEVEL_TEMP_2:
				if(::G_SorterTempDlg.IsTempOK(5))
			{
				pDC->SetBkColor(RGB(0, 255, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
				
			}	
			break;

			case IDC_STATIC_HIGH_LEVEL_TEMP_3:
				if(::G_SorterTempDlg.IsTempOK(6))
			{
				pDC->SetBkColor(RGB(0, 255, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
				
			}	
			
			break;
			case IDC_STATIC_HIGH_LEVEL_TEMP_4:
				if(::G_SorterTempDlg.IsTempOK(7))
			{
				pDC->SetBkColor(RGB(0, 255, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
				
			}	
			
			break;
			case IDC_STATIC_LOW_LEVEL_OUTPUT_CYCLINDER:
				if(m_X_Status & (1 << X_Low_Level_Output_Cylinder))
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 255, 255)); 
			}
			break;
			case IDC_STATIC_HIGH_LEVEL_INPUT_CYCLINDER:
				if(m_X_Status & (1 << X_High_Level_Input_Cylinder))
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 255, 255)); 
			}
			break;
			case IDC_STATIC_LOW_LEVEL_FAIL_CYCLINDER:
				if(m_X_Status & (1 << X_Low_Level_Fail_Cylinder))
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 255, 255)); 
			}
			break;
			case IDC_STATIC_LOW_LEVEL_TEST_CYCLINDER:
				if(m_X_Status & (1 << X_Low_Level_Test_Cylinder))
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 255, 255)); 
			}
			break;
			case IDC_STATIC_HIGH_LEVEL_OUTPUT_CYCLINDER:
				if(m_X_Status & (1 << X_High_Level_Output_Cylinder))
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 255, 255)); 
			}
			break;
			case IDC_STATIC_HIGH_LEVEL_PASS_CYCLINDER:
			if(m_X_Status & (1 << X_High_Level_Selected_Pass_Cylinder))
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 255, 255)); 
			}
			break;

			case IDC_STATIC_IS_HAS_SWITCH:
			if(m_X_Status & (1 << X_Has_Switch))
			{
				pDC->SetBkColor(RGB(0xFF,0xFF,0xFF)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			break;
			case IDC_STATIC_HIGH_LEVEL_FAIL_CYCLINDER:
			if(m_X_Status & (1 << X_High_Level_Selected_Fail_Cylinder))
			{
				pDC->SetBkColor(RGB(255, 0, 0)); 
			}
			else
			{
				pDC->SetBkColor(RGB(255, 255, 255)); 
			}
			break;

			case IDC_STATIC_HIGH_LEVEL_TEST_CYCLINDER:
				if(m_X_Status & (1 << X_High_Level_Test_Cylinder))
				{
					pDC->SetBkColor(RGB(255, 0, 0)); 
				}
				else
				{
					pDC->SetBkColor(RGB(255, 255, 255)); 
				}
				break;
			
		}
		break;
	}

	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CSorterPlcDlg::Deal_X_Status(unsigned int X_Status)
{
//	if(m_X_Status != X_Status)
	{
		m_X_Status = X_Status;

		CWnd *w = this->GetDlgItem(IDC_EDIT_X);
		CString s;
		s.Format(_T("%04X"),m_X_Status);
		w->SetWindowTextW(s);

		//this->Invalidate(true);
		//this->ShowWindow(SW_HIDE); //只有这样，Wince才调用 OnCtlColor
		//this->ShowWindow(SW_SHOW);
	}
	
}

void CSorterPlcDlg::Deal_Y_Status(unsigned int Y_Status)
{
	if(m_Y_Status != Y_Status)
	{
		m_Y_Status = Y_Status;
	}
	CWnd *w = this->GetDlgItem(IDC_EDIT_Y);
	CString s;
	s.Format(_T("%04X"),m_Y_Status);
	w->SetWindowTextW(s);
	
	
}



BOOL CSorterPlcDlg::OnInitDialog() 
{


	CDialog::OnInitDialog();

	this->SetTimer(1,500,NULL);

	m_WaitForRunMode	=	0;

	CWnd *edit;
	CString s;
	TCHAR buf[100];

	

	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_SV_LOW);

	s.Format(_T("%5.1f"),::G_NormalConfigPara[0].Sv_8TEMP_8SSR[3]);
	edit->SetWindowTextW(s); 

	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_SV_HIGH);

	s.Format(_T("%5.1f"),::G_NormalConfigPara[0].Sv_8TEMP_8SSR[7]);
	edit->SetWindowTextW(s); 

	G_RS485_Comm.SendNormalPidCmd_8PT100_8SSR(G_NormalConfigPara[0].Sv_8TEMP_8SSR,0);

	return true;
}

void CSorterPlcDlg::OnStnClickedStaticLowLevelFailCyclinde()
{
	// TODO: 在此添加控件通知处理程序代码

		/*
	//:0105 08C8 FF002B 低温进料电磁阀动作
#define	Y_Low_Level_Input_Cylinder				//Y01

//:0105 08CB FF0028 低温检测电磁阀动作
#define Y_Low_Level_Test_Cylinder				//Y03

#define Y_Low_Level_Test_Light					//Y04

//:0105 08CC FF0027 低温出料电磁阀动作
#define Y_Low_Level_Output_Cylinder				//Y05	

//:0105 08CE FF0025 低温接料电磁阀动作
#define Y_Low_Level_Fail_Cylinder				//Y06

//3A 30 31 30 35 30 38 43 46 46 46 30 30 32 34 0D 0A
//:0105 08CF FF0024
#define	Y_High_Level_Input_Cylinder				//Y07

//:0105 08D0 FF0023
#define Y_High_Level_Test_Cylinder				Y10

//#define Y_High_Level_Test_Light				

#define Y_High_Level_Output_Cylinder			Y11

#define Y_High_Level_Selected_Cylinder			Y12

#define Y_High_Volt_Pass						//Y14

#define Y_Red_Light								//Y14
#define Y_Green_Light							//Y15
#define Y_Yellow_Light							//Y16
	*/

	if(m_ManualMode)
	{
		unsigned int c = (1 << (Y_Low_Level_Fail_Cylinder));//不知道为什么
		if(m_Y_Status & c)
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_Low_Level_Fail_Cylinder,0);
		}
		else
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_Low_Level_Fail_Cylinder,1);
		}
	}
	
}

void CSorterPlcDlg::OnStnClickedStaticLowLevelTestCyclinder()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ManualMode)
	{
		unsigned int c = (1 << Y_Low_Level_Test_Cylinder);
		if(m_Y_Status & c)
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_Low_Level_Test_Cylinder,0);
		}
		else
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_Low_Level_Test_Cylinder,1);
		}
	}
	
}

void CSorterPlcDlg::OnStnClickedStaticLowLevelInputCyclinder()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ManualMode)
	{
		unsigned int c = (1 << 1);
		if(m_Y_Status & c)
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_Low_Level_Input_Cylinder,0);
		}
		else
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_Low_Level_Input_Cylinder,1);
		}
	}
}

void CSorterPlcDlg::OnStnClickedStaticLowLevelOutputCyclinder()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ManualMode)
	{
		unsigned int c = 0x01;//(1 << Y_Low_Level_Output_Cylinder);//Y_High_Level_Input_Cylinder);
		if(m_Y_Status & c)
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_Low_Level_Output_Cylinder,0);
		}
		else
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_Low_Level_Output_Cylinder,1);
		}
	}
}

void CSorterPlcDlg::OnStnClickedStaticHighLevelOutputCyclinder()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ManualMode)
	{
		unsigned int c = (1 << (Y_High_Level_Output_Cylinder + 1));//不知道为什么
		if(m_Y_Status & c)
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_High_Level_Output_Cylinder,0);
		}
		else
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_High_Level_Output_Cylinder,1);
		}
	}
}

void CSorterPlcDlg::OnStnClickedStaticHighLevelFailCyclinder()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ManualMode)
	{
		G_PrintRS232_Comm.Delta_Set_Y(Y_High_Level_Selected_Cylinder,0);
	}
}

void CSorterPlcDlg::OnStnClickedStaticHighLevelPassCyclinder()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ManualMode)
	{
		G_PrintRS232_Comm.Delta_Set_Y(Y_High_Level_Selected_Cylinder,1);
	}
}

void CSorterPlcDlg::OnStnClickedStaticHighLevelTestCyclinder()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ManualMode)
	{
		unsigned int c = (1 << Y_High_Level_Test_Cylinder);
		if(m_Y_Status & c)
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_High_Level_Test_Cylinder,0);
		}
		else
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_High_Level_Test_Cylinder,1);
		}
	}

}

void CSorterPlcDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	//// 停止标志 0x0864,赋值 0xFF00
	G_PrintRS232_Comm.Delta_Set_M(0x65,1);

	m_WaitForRunMode = 0;
}

void CSorterPlcDlg::OnBnClickedRecover()
{
	// TODO: 在此添加控件通知处理程序代码
// 复位标志 0x0867,赋值 0xFF00

//	if(m_RecoverMode)
	{
		G_PrintRS232_Comm.Delta_Set_M(0x67,1);

		G_PrintRS232_Comm.Delta_Set_M(0x67,0);
	}
//	else
	{
//		G_RS485_Comm.Delta_Set_M(0x67,1);
	}

//	if(m_StopMode)
	{
		G_PrintRS232_Comm.Delta_Set_M(0x65,0);
	}
	
	//if(m_RunMode)
	{
		G_PrintRS232_Comm.Delta_Set_M(0x64,0);
	}
	
	CWnd *w = this->GetDlgItem(IDC_EDIT_1032);

	TCHAR buf[100];
	w->GetWindowTextW(buf,100);

	int Alarm =  _tcstod(buf,NULL);

	if(Alarm > 0)
	{
//		G_PrintRS232_Comm.Delta_Set_D32(0x32,0);
	}

}

void CSorterPlcDlg::OnBnClickedAuto()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ManualMode == 0)
	{
		CWnd * w = this->GetDlgItem(IDC_AUTO);
		w->SetWindowTextW(_T("手动模式"));
		m_ManualMode = 1;

		// 自动标志 0x0866,赋值 0xFF00,手动

		G_PrintRS232_Comm.Delta_Set_M(0x66,1);
		
	}
	else
	{
		m_ManualMode = 0;

		CWnd * w = this->GetDlgItem(IDC_AUTO);

		w->SetWindowTextW(_T("自动模式"));

		// 自动标志 0x0866,赋值 0xFF00,手动

		G_PrintRS232_Comm.Delta_Set_M(0x66,0);
	}
}

void CSorterPlcDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CSorterPlcDlg::OnBnClickedSetTemp()
{
	// TODO: 在此添加控件通知处理程序代码
	
	return ;

	G_RS485_Comm.ReadPidT(0 ,0);
	G_RS485_Comm.ReadPidT(0 ,1);
	G_RS485_Comm.ReadPidT(0 ,2);
	G_RS485_Comm.ReadPidT(0 ,3);
	G_RS485_Comm.ReadPidT(0 ,4);
	G_RS485_Comm.ReadPidT(0 ,5);
	G_RS485_Comm.ReadPidT(0 ,6);
	G_RS485_Comm.ReadPidT(0 ,7);
	G_RS485_Comm.ReadAdjustAll(0);

	G_SorterTempDlg.DoModal();
}


void CSorterPlcDlg::DisplaySendCmd(unsigned char cmd,unsigned char *cmd_buf,int len,int BoxNr)
{
	if(this->GetSafeHwnd() == 0)
	{
		return;
	}

	if(this->IsWindowVisible() != true)
	{
		return;
	}
	CString s,s1;
	static int m_SendCmdCount = 0;
	CWnd *w = GetDlgItem(IDC_EDIT_SEND);
	
	m_SendCmdCount ++;
	s.Format(_T("发送(%d): "),m_SendCmdCount);
	
	for(int i = 0; i < len; i++)
	{
		s1.Format(_T("%02X "),cmd_buf[i]);
		s+=s1;
		
	}
	w->SetWindowTextW(s);

}
void CSorterPlcDlg::DisplayReceiveCmd(unsigned char cmd,unsigned char *cmd_buf,int len,int BoxNr)
{
	static int counts = 0;
	CString s = _T("");
	CString s1;

	counts ++;

	s.Format(_T("接收:%d "),counts);
	for(int i = 0; i < len; i++)
	{
		s1.Format(_T("%02X "),(unsigned char)cmd_buf[i]);
		s+=s1;
	}
	
	CWnd *w = GetDlgItem(IDC_EDIT_REC);
	w->SetWindowTextW(s);


}

extern float fabsf_temp[8+8];

void CSorterPlcDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	float pv;
	CTime cur = CTime::GetCurrentTime();
	CTimeSpan span = cur - ::G_SorterTempDlg.m_MeterAnswerTime;
	if(span.GetTotalSeconds() > 60)
	{//没有了通信
		m_AlarmMsg = _T("温度仪表没有相应  温度仪表没有相应 ");
	}

	CWnd * w;

	if(m_AlarmMsg.GetLength() > 0)
	{
		CString s;
		s = m_AlarmMsg.Left(2);
		m_AlarmMsg = m_AlarmMsg.Right(m_AlarmMsg.GetLength() - 2) + s;

		w = this->GetDlgItem(IDC_EDIT_ALARM_MSG);
		w->SetWindowTextW(m_AlarmMsg);
	}
	else
	{
		w = this->GetDlgItem(IDC_EDIT_ALARM_MSG);
		w->SetWindowTextW(m_AlarmMsg);

		CString s;
		s.Format(_T("%4.1f,%4.1f,%4.1f(%4.1f--%4.1f),%4.1f,%4.1f,%4.1f(%4.1f--%4.1f)"),fabsf_temp[0],fabsf_temp[1],fabsf_temp[2],fabsf_temp[10],fabsf_temp[11],fabsf_temp[4],fabsf_temp[5],fabsf_temp[6],fabsf_temp[12],fabsf_temp[13]);
		w->SetWindowTextW(s);
	}

	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_TEMP_1);
	if(::G_SorterTempDlg.IsTempOK(0))
	{
		w->SetWindowTextW(_T("OK"));
	}
	else
	{
		w->SetWindowTextW(_T("F"));
	}

	//G_TempArray_8TEMP_8SSR[0].GetAt(G_TempArray_8TEMP_8SSR[0].GetSize()).Answer_8TEMP_8SSR.Pv[0] / 10.0f;

	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_TEMP_2);
	if(::G_SorterTempDlg.IsTempOK(1))
	{
		w->SetWindowTextW(_T("OK"));
	}
	else
	{
		w->SetWindowTextW(_T("F"));
	}

	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_TEMP_3);
	if(::G_SorterTempDlg.IsTempOK(2))
	{
		w->SetWindowTextW(_T("OK"));
	}
	else
	{
		w->SetWindowTextW(_T("F"));
	}

	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_TEMP_4);
	if(::G_SorterTempDlg.IsTempOK(3))
	{
		w->SetWindowTextW(_T("OK"));
	}
	else
	{
		w->SetWindowTextW(_T("F"));
	}


	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_TEMP_1);
	if(::G_SorterTempDlg.IsTempOK(4))
	{
		w->SetWindowTextW(_T("OK"));
	}
	else
	{
		w->SetWindowTextW(_T("F"));
	}

	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_TEMP_2);
	if(::G_SorterTempDlg.IsTempOK(5))
	{
		w->SetWindowTextW(_T("OK"));
	}
	else
	{
		w->SetWindowTextW(_T("F"));
	}

	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_TEMP_3);
	if(::G_SorterTempDlg.IsTempOK(6))
	{
		w->SetWindowTextW(_T("OK"));
	}
	else
	{
		w->SetWindowTextW(_T("F"));
	}

	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_TEMP_4);
	if(::G_SorterTempDlg.IsTempOK(7))
	{
		w->SetWindowTextW(_T("OK"));
	}
	else
	{
		w->SetWindowTextW(_T("F"));
	}

	if(m_WaitForRunMode == 0)
	{
		w = this->GetDlgItem(IDC_RUN);
		w->SetWindowTextW(_T("运行"));
	}
	else
	{
		w = this->GetDlgItem(IDC_RUN);
		w->SetWindowTextW(_T("正在等待。。。"));

		if(G_SorterTempDlg.IsTempOK(0)
			&& G_SorterTempDlg.IsTempOK(1)
			&& G_SorterTempDlg.IsTempOK(2)
			&& G_SorterTempDlg.IsTempOK(3)
			&& G_SorterTempDlg.IsTempOK(4)
			&& G_SorterTempDlg.IsTempOK(5)
			&& G_SorterTempDlg.IsTempOK(6)
			&& G_SorterTempDlg.IsTempOK(7))
		{

			OnBnClickedRecover();

			this->m_RunMode = 1;

			G_PrintRS232_Comm.Delta_Set_M(0x64,1); //启动

			m_WaitForRunMode = 0;

		}
	

		

	}


	CDialog::OnTimer(nIDEvent);
}

void CSorterPlcDlg::OnStnClickedStaticHighLevelInputCyclinder()
{
	// TODO: 在此添加控件通知处理程序代码

	//Y01,Y02,Y05,Y07,
	if(m_ManualMode)
	{
		unsigned int c = (1 << 15);//Y_High_Level_Input_Cylinder);
		if(m_Y_Status & c)
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_High_Level_Input_Cylinder,0);
		}
		else
		{
			G_PrintRS232_Comm.Delta_Set_Y(Y_High_Level_Input_Cylinder,1);
		}
	}

}

void CSorterPlcDlg::DealCmdAnswer(unsigned char *para, unsigned char *flash,bool IsCmdAscii,int data_len)
{


	CString s;

	static MODBUS_RS485_8TEMP_8SSR_ANSWER m_Answer_8TEMP_8SSR;
	

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
		
			w = this->GetDlgItem(IDC_EDIT_ADJ_LOW);
			s.Format(_T("%4.1f"),m_Answer_8TEMP_8SSR.Pv[3] / 10.0f);
			w->SetWindowTextW(s);

			w = this->GetDlgItem(IDC_EDIT_ADJ_HIGH);
			s.Format(_T("%4.1f"),m_Answer_8TEMP_8SSR.Pv[7] / 10.0f);
			w->SetWindowTextW(s);

		break;
		
		case CMD_READ_TEMP:
			
				
			if(data_len == sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER))
			{
				
				
					if(m_Answer_8TEMP_8SSR.Error & (1 << 3))
					{
						w = this->GetDlgItem(IDC_EDIT_PV_LOW);
						w->SetWindowTextW(_T("错误"));
					}
					else
					{
						w = this->GetDlgItem(IDC_EDIT_PV_LOW);
						s.Format(_T("%5.2f"),m_Answer_8TEMP_8SSR.Pv[3] / 100.0f);
						w->SetWindowTextW(s);
					}

					w = this->GetDlgItem(IDC_EDIT_SV_LOW);
					s.Format(_T("%5.1f"),m_Answer_8TEMP_8SSR.Sv[3] / 10.0f);
					w->SetWindowTextW(s);

					
					if(m_Answer_8TEMP_8SSR.Error & (1 << 7))
					{
						w = this->GetDlgItem(IDC_EDIT_PV_HIGH);
						w->SetWindowTextW(_T("错误"));
					}
					else
					{
						w = this->GetDlgItem(IDC_EDIT_PV_HIGH);
						s.Format(_T("%5.2f"),m_Answer_8TEMP_8SSR.Pv[7] / 100.0f);
						w->SetWindowTextW(s);
					}

					w = this->GetDlgItem(IDC_EDIT_SV_HIGH);
					s.Format(_T("%5.1f"),m_Answer_8TEMP_8SSR.Sv[7] / 10.0f);
					w->SetWindowTextW(s);
					
			}
			
				
		
		break;


	}

}

void CSorterPlcDlg::SendSv(void)
{
		TCHAR	buf[100];

		CWnd *edit;
		float temp[8];

		
			edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_LOW);
			edit->GetWindowTextW(buf,100); 
	
			temp[3] = _tcstod(buf,NULL);
			temp[2] = temp[3] - 2.0f;
			temp[1] = temp[3] - 3.0f;
			temp[0] = temp[3] - 4.0f;
			

			edit=	(CEdit*)this->GetDlgItem(IDC_EDIT_SV_HIGH);
			edit->GetWindowTextW(buf,100); 
	
			temp[7] = _tcstod(buf,NULL);
			temp[6] = temp[7] - 2.0f;
			temp[5] = temp[7] - 3.0f;
			temp[4] = temp[7] - 4.0f;


		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[0] = temp[0];
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[1] = temp[1];
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[2] = temp[2];
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[3] = temp[3];

		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[4] = temp[4];
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[5] = temp[5];
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[6] = temp[6];
		::G_NormalConfigPara[0].Sv_8TEMP_8SSR[7] = temp[7];

		::SaveNormalConfigPara();

		::G_TempArray_8TEMP_8SSR[0].RemoveAll();
		
		G_RS485_Comm.SendNormalPidCmd_8PT100_8SSR(temp,0);
}

void CSorterPlcDlg::OnEnSetfocusEditSvLow()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_SEND);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入设置温度值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
		CString s;
		TCHAR buf[100];

		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_SV_LOW);
		edit->SetWindowTextW(dlg.m_Input); 

		SendSv();

		
		::SaveNormalConfigPara();

	}
}

void CSorterPlcDlg::OnEnSetfocusEditSvHigh()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_SEND);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入设置温度值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
		CString s;
		TCHAR buf[100];

		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_SV_HIGH);
		edit->SetWindowTextW(dlg.m_Input); 

		SendSv();
		

	}
}

void CSorterPlcDlg::SendAdjust(void)
{
	TCHAR s[100];
	CWnd *w;
	float val[8];

	
		w = this->GetDlgItem(IDC_EDIT_ADJ_LOW);

		w->GetWindowText(s,100);
		val[0]  = _tcstod(s,NULL);
		val[1]  = val[0];
		val[2]  = val[0];
		val[3]  = val[0];
	
		w = this->GetDlgItem(IDC_EDIT_ADJ_HIGH);

		w->GetWindowText(s,100);
		val[4]  = _tcstod(s,NULL);
		val[5]  = val[4];
		val[6]  = val[4];
		val[7]  = val[4];

	
	G_RS485_Comm.SetAdjustAll(val ,0);
	G_RS485_Comm.ReadAdjustAll(0);

}

void CSorterPlcDlg::OnEnSetfocusEditAdjLow()
{
	// TODO: 在此添加控件通知处理程序代码
#ifdef WINCE
	CWnd *w = this->GetDlgItem(IDC_EDIT_SEND);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 修正值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJ_LOW);
		edit->SetWindowTextW(dlg.m_Input); 
		

		SendAdjust();
	}
#endif
}

void CSorterPlcDlg::OnEnSetfocusEditAdjHigh()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_SEND);

	w->SetFocus();

	CInputDlg dlg;

	dlg.m_Title = _T("请输入 修正值");

	if(dlg.DoModal() == IDOK)
	{
		dlg.m_Input.Trim(); 
		CWnd *edit;
	
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_ADJ_HIGH);
		edit->SetWindowTextW(dlg.m_Input); 
		

		SendAdjust();
	}
}

void CSorterPlcDlg::OnEnKillfocusEditLowLevel()
{
	// TODO: 在此添加控件通知处理程序代码

	
}

void CSorterPlcDlg::OnEnSetfocusEditHighLevel()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_SEND);

	w->SetFocus();

	if(AfxMessageBox(_T("需要清零码？"),MB_YESNO) == IDYES)
	{
		G_PrintRS232_Comm.Delta_Set_C(0x7C,0);
	}
}

void CSorterPlcDlg::OnEnSetfocusEditMainLevel()
{
	// TODO: 在此添加控件通知处理程序代码

	CWnd *w = this->GetDlgItem(IDC_EDIT_SEND);

	w->SetFocus();

	if(AfxMessageBox(_T("需要清零码？"),MB_YESNO) == IDYES)
	{
		G_PrintRS232_Comm.Delta_Set_C(0x7E,0);
	}
}

void CSorterPlcDlg::OnEnChangeEditHighLevel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CSorterPlcDlg::OnEnSetfocusEditLowLevel()
{
	// TODO: 在此添加控件通知处理程序代码

	CWnd *w = this->GetDlgItem(IDC_EDIT_SEND);

	w->SetFocus();

	if(AfxMessageBox(_T("需要清零码？"),MB_YESNO) == IDYES)
	{
		G_PrintRS232_Comm.Delta_Set_C(0x7A,0);
	}
}

void CSorterPlcDlg::OnStnClickedStaticIsHasSwitch()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CSorterPlcDlg::OnStnClickedStaticLowLevelTemp1()
{
	// TODO: 在此添加控件通知处理程序代码
}
