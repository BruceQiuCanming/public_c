// InputGlassTempDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputGlassTempDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGlassTempDlg dialog d:\work\public_c\InputGlassTempDlg.cpp



CInputGlassTempDlg::CInputGlassTempDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputGlassTempDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGlassTempDlg)
	m_Temp1 = "";
	m_Temp2 = "";
	m_Temp3 = "";
	m_Temp4 = "";
	//}}AFX_DATA_INIT
	m_InputGlassTemp_ID = 0;

}


void CInputGlassTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGlassTempDlg)
	DDX_Text(pDX, IDC_EDIT_GLASS_1, m_Temp1);
	DDX_Text(pDX, IDC_EDIT_GLASS_2, m_Temp2);
	DDX_Text(pDX, IDC_EDIT_GLASS_3, m_Temp3);
	DDX_Text(pDX, IDC_EDIT_GLASS_4, m_Temp4);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_GLASS_2, m_InputGlassTemp_2);
	DDX_Control(pDX, IDC_EDIT_GLASS_1, m_InputGlassTemp_1);
	DDX_Control(pDX, IDC_EDIT_GLASS_3, m_InputGlassTemp_3);
	DDX_Control(pDX, IDC_EDIT_GLASS_4, m_InputGlassTemp_4);
}


BEGIN_MESSAGE_MAP(CInputGlassTempDlg, CDialog)
	//{{AFX_MSG_MAP(CInputGlassTempDlg)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_2, &CInputGlassTempDlg::OnEnSetfocusEditGlass2)
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_1, &CInputGlassTempDlg::OnEnSetfocusEditGlass1)
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_3, &CInputGlassTempDlg::OnEnSetfocusEditGlass3)
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_4, &CInputGlassTempDlg::OnEnSetfocusEditGlass4)
	ON_BN_CLICKED(IDC_BUTTON_1, &CInputGlassTempDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CInputGlassTempDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CInputGlassTempDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CInputGlassTempDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CInputGlassTempDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CInputGlassTempDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CInputGlassTempDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CInputGlassTempDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CInputGlassTempDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CInputGlassTempDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_DOT, &CInputGlassTempDlg::OnBnClickedButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CInputGlassTempDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDOK, &CInputGlassTempDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGlassTempDlg message handlers


void CInputGlassTempDlg::OnEnSetfocusEditGlass2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 2;
}

void CInputGlassTempDlg::OnEnSetfocusEditGlass1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 1;
}

void CInputGlassTempDlg::OnEnSetfocusEditGlass3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 3;
}

void CInputGlassTempDlg::OnEnSetfocusEditGlass4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 4;
}
void CInputGlassTempDlg::OnBnClickedButton0_9(CString num)
{
	switch(m_InputGlassTemp_ID)
	{
	case 1:
		m_Temp1 += num;
		break;
	case 2:
		m_Temp2 += num;
		break;
	case 3:
		m_Temp3 += num;
		break;
	case 4:
		m_Temp4 += num;
		break;
	}
	this->UpdateData(false);

}
void CInputGlassTempDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("1"));
	
}

void CInputGlassTempDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("2"));
}

void CInputGlassTempDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("3"));
}

void CInputGlassTempDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("4"));
}

void CInputGlassTempDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("5"));
}

void CInputGlassTempDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("6"));
}

void CInputGlassTempDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("7"));
}

void CInputGlassTempDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("8"));
}

void CInputGlassTempDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("9"));
}

void CInputGlassTempDlg::OnBnClickedButton0()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("0"));
}

void CInputGlassTempDlg::OnBnClickedButtonDot()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("."));
}

void CInputGlassTempDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码 1000888383407
	switch(m_InputGlassTemp_ID)
	{
	case 1:
		m_Temp1 = "";
		break;
	case 2:
		m_Temp2 = "";
		break;
	case 3:
		m_Temp3 = "";
		break;
	case 4:
		m_Temp4 = "";
		break;
	}
	this->UpdateData(false);
}


BOOL CInputGlassTempDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	this->SetWindowText(this->m_Title); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CInputGlassTempDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	m_Temp1.TrimLeft();
	m_Temp1.TrimRight();

	float f1,f2,f3,f4 ;
	f1 = _tcstod(m_Temp1,NULL);
	if(f1 < -30 || f1 > 300)
	{
		AfxMessageBox(_T("1号水银温度计的温度有误(-30--300度），请修改!"));
		return;
	}

	f2 = _tcstod(m_Temp2,NULL);
	if(f2 < -30 || f2 > 300)
	{
		AfxMessageBox(_T("2号水银温度计的温度有误(-30--300度），请修改!"));
		return;
	}

	f3 = _tcstod(m_Temp3,NULL);
	if(f3 < -30 || f3 > 300)
	{
		AfxMessageBox(_T("3号水银温度计的温度有误(-30--300度），请修改!"));
		return;
	}

	f4 = _tcstod(m_Temp4,NULL);
	if(f4 < -30 || f4 > 300)
	{
		AfxMessageBox(_T("4号水银温度计的温度有误(-30--300度），请修改!"));
		return;
	}

	
	float max = -999,min = 999;
	if(max < f1)
	{
		max = f1;
	}
	if(max < f2)
	{
		max = f2;
	}
	if(max < f3)
	{
		max = f3;
	}
	if(max < f4)
	{
		max = f4;
	}

	


	if(min > f1)
	{
		min = f1;
	}
	if(min > f2)
	{
		min = f2;
	}
	if(min > f3)
	{
		min = f3;
	}
	if(min > f4)
	{
		min = f4;
	}
	
	if(max - min> 5.0f)
	{
		AfxMessageBox(_T("四角温度误差太大！！！！！！！"));
		return;
	}



	CDialog::OnOK();
}

