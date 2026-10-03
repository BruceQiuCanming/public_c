// InputGlassTempDlg.cpp : implementation file
//
#include "stdafx.h"
#ifndef WINCE
#include "HeatBox.h"
#endif
#include "InputAutoAdjustTempDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGlassTempDlg dialog


CInputAutoAdjustTempDlg::CInputAutoAdjustTempDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputAutoAdjustTempDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGlassTempDlg)
	m_Temp1 = "";
	m_Temp2 = "";
	//}}AFX_DATA_INIT
	m_InputGlassTemp_ID = 0;
}


void CInputAutoAdjustTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGlassTempDlg)
	DDX_Text(pDX, IDC_EDIT_GLASS_1, m_Temp1);
	DDX_Text(pDX, IDC_EDIT_GLASS_2, m_Temp2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_GLASS_2, m_InputGlassTemp_2);
	DDX_Control(pDX, IDC_EDIT_GLASS_1, m_InputGlassTemp_1);
}


BEGIN_MESSAGE_MAP(CInputAutoAdjustTempDlg, CDialog)
	//{{AFX_MSG_MAP(CInputGlassTempDlg)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_2, &CInputAutoAdjustTempDlg::OnEnSetfocusEditGlass2)
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_1, &CInputAutoAdjustTempDlg::OnEnSetfocusEditGlass1)
	ON_BN_CLICKED(IDC_BUTTON_1, &CInputAutoAdjustTempDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CInputAutoAdjustTempDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CInputAutoAdjustTempDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CInputAutoAdjustTempDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CInputAutoAdjustTempDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CInputAutoAdjustTempDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CInputAutoAdjustTempDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CInputAutoAdjustTempDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CInputAutoAdjustTempDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CInputAutoAdjustTempDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_DOT, &CInputAutoAdjustTempDlg::OnBnClickedButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CInputAutoAdjustTempDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDOK, &CInputAutoAdjustTempDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGlassTempDlg message handlers

void CInputAutoAdjustTempDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData();
	CDialog::OnOK();
}

void CInputAutoAdjustTempDlg::OnEnSetfocusEditGlass2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 2;
}

void CInputAutoAdjustTempDlg::OnEnSetfocusEditGlass1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 1;
}


void CInputAutoAdjustTempDlg::OnBnClickedButton0_9(CString num)
{
	switch(m_InputGlassTemp_ID)
	{
	case 1:
		m_Temp1 += num;
		break;
	case 2:
		m_Temp2 += num;
		break;

	}
	this->UpdateData(false);

}
void CInputAutoAdjustTempDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("1"));
	
}

void CInputAutoAdjustTempDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("2"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("3"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("4"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("5"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("6"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("7"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("8"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("9"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButton0()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("0"));
}

void CInputAutoAdjustTempDlg::OnBnClickedButtonDot()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(_T("."));
}

void CInputAutoAdjustTempDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_InputGlassTemp_ID)
	{
	case 1:
		m_Temp1 = "";
		break;
	case 2:
		m_Temp2 = "";
		break;

	}
	this->UpdateData(false);
}


BOOL CInputAutoAdjustTempDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(this->m_Title); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CInputAutoAdjustTempDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	m_Temp1.TrimLeft();
	m_Temp1.TrimRight();

	double f1,f2 ;
	f1 = _tcstod(m_Temp1,NULL);
	if(f1 < 0 || f1 > 200)
	{
		AfxMessageBox(_T("下位温度有误，请修改!"));
		return;
	}

	f2 = _tcstod(m_Temp2,NULL);
	if(f2 < 0 || f2 > 300)
	{
		AfxMessageBox(_T("上位温度有误，请修改!"));
		return;
	}

	if(f1 > f2)
	{
		AfxMessageBox(_T("上下位温度有误，请修改!"));
		return;
	}


	CDialog::OnOK();
}
