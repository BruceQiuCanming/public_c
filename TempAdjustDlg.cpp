// TempAdjustDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "TempAdjustDlg.h"
#include "InputDlg.h"
#include "common.h"

// CTempAdjustDlg 对话框

IMPLEMENT_DYNAMIC(CTempAdjustDlg, CDialog)

CTempAdjustDlg::CTempAdjustDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTempAdjustDlg::IDD, pParent)
	, m_TempAdjustType(0)
{
	m_bSetFocusOnPara4 = true;
	m_TempAdjustType   = 0;
}

CTempAdjustDlg::~CTempAdjustDlg()
{
}

void CTempAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTempAdjustDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTempAdjustDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(EN_SETFOCUS,10001,10200,OnEditClicked)
	ON_BN_CLICKED(IDC_RADIO1, &CTempAdjustDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTempAdjustDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDCANCEL, &CTempAdjustDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTempAdjustDlg 消息处理程序
void CTempAdjustDlg::OnEditClicked(UINT id)
{

	if(m_bSetFocusOnPara4 == false)
	{
		return;
	}
	m_bSetFocusOnPara4 = false;

	CWnd *w = this->GetDlgItem(IDOK); 
	
	CInputDlg dlg;

	dlg.m_Title.Format(_T("%d: = %5.1f"),id - 10000,m_AdjustTemp[m_TempAdjustType][id - 10001]);

	if(dlg.DoModal() == IDOK)
	{
		m_AdjustTemp[m_TempAdjustType][id - 10001] = _tcstod(dlg.m_Input,NULL);//atof(buf);

		CWnd * edit = this->GetDlgItem(id);
		
		CString s;
		s.Format(_T("%d#= %5.1f"),id - 10000 ,m_AdjustTemp[m_TempAdjustType][id - 10001]);  
		
		edit->SetWindowTextW(s);
		
	}
	

	w->SetFocus(); 

	m_bSetFocusOnPara4 = true;

}

BOOL CTempAdjustDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->ShowWindow( SW_SHOWMAXIMIZED);

	RECT wndRect;
	this->GetWindowRect(&wndRect); 



	for(int i = 0; i < G_iLedLines[m_BoxNr]; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine[m_BoxNr]; j++)
		{
			RECT rect;
			rect.left  = j * (wndRect.right / G_iLedNrPerLine[m_BoxNr]);
			rect.right = rect.left + (wndRect.right / G_iLedNrPerLine[m_BoxNr]) *0.9;
			rect.top   = (i + 2)* (wndRect.bottom / (G_iLedLines[m_BoxNr] + 2));
			rect.bottom = rect.top + (wndRect.bottom / (G_iLedLines[m_BoxNr] + 2)) * 0.9;

			m_Edit[i*G_iLedNrPerLine[m_BoxNr] + j].Create(0x50010080,rect,this,10001 + i*G_iLedNrPerLine[m_BoxNr] + j); 
			
			CString s;
			s.Format(_T("%d#= %5.1f"),i*G_iLedNrPerLine[m_BoxNr] + j +1 ,m_AdjustTemp[0][i*G_iLedNrPerLine[m_BoxNr] + j]);  
			m_Edit[i*G_iLedNrPerLine[m_BoxNr] + j].SetWindowTextW(s);

		}
	}
	
	CButton* b = (CButton*) GetDlgItem(IDC_RADIO1);
	b->SetCheck(1);

	return true;
}
void CTempAdjustDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CTempAdjustDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码

	m_TempAdjustType = 0;

	for(int i = 0; i < G_iLedLines[m_BoxNr]; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine[m_BoxNr]; j++)
		{
			CString s;
			s.Format(_T("%d#= %5.1f"),i*G_iLedNrPerLine[m_BoxNr] + j +1 ,m_AdjustTemp[0][i*G_iLedNrPerLine[m_BoxNr] + j]);  
			m_Edit[i*G_iLedNrPerLine[m_BoxNr] + j].SetWindowTextW(s);
		}
	} 

}

void CTempAdjustDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码

	m_TempAdjustType = 1;

	for(int i = 0; i < G_iLedLines[m_BoxNr]; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine[m_BoxNr]; j++)
		{
			CString s;
			s.Format(_T("%d#= %5.1f"),i*G_iLedNrPerLine[m_BoxNr] + j +1 ,m_AdjustTemp[1][i*G_iLedNrPerLine[m_BoxNr] + j]);  
			m_Edit[i*G_iLedNrPerLine[m_BoxNr] + j].SetWindowTextW(s);
		}
	} 

}

void CTempAdjustDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
