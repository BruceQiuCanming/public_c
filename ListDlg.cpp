// ListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ListDlg.h"
#ifdef WINCE
#include "bmp.h"
#endif
// CListDlg 对话框

IMPLEMENT_DYNAMIC(CListDlg, CDialog)

CListDlg::CListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListDlg::IDD, pParent)
{

	m_iSelectItem = -1;
}

CListDlg::~CListDlg()
{
}

void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CListDlg, CDialog)
	ON_LBN_SETFOCUS(IDC_LIST1, &CListDlg::OnLbnSetfocusList1)
	ON_BN_CLICKED(IDOK, &CListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CListDlg::OnBnClickedCancel)

END_MESSAGE_MAP()


// CListDlg 消息处理程序

BOOL CListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int i;
	CString s;

	for(i = 0; i < this->m_ListItem.GetSize(); i++)
	{
		s = m_ListItem.GetAt(i); 
		this->m_List.AddString(s);  
	}
	this->SetWindowText(m_Tiltle); 
	return true;
}
void CListDlg::OnLbnSetfocusList1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CListDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码d:\work\WinCE\TPC_HeatBox_V2\TPC_HeatBox\ListDlg.cpp

	m_iSelectItem = this->m_List.GetCurSel();

	if((m_iSelectItem >= 0) && (m_iSelectItem < m_ListItem.GetSize())) 
	{	

		this->m_List.GetText(m_iSelectItem,m_SelectString);
	}
#ifdef WINCE
	HDC hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); 
	CString name;
	name.Format(_T("\\USB HardDisk\\%s.bmp"),m_Tiltle); 
	WriteBmp(name,hdc);  
	
    DeleteDC(hdc);  
#endif
	OnOK();
}

void CListDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iSelectItem = -1;
	m_SelectString = "";
/*
   HDC hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);  
   WriteBmp(_T("\\USB HardDisk\\History.bmp"),hdc);  
   DeleteDC(hdc);  
*/
	OnCancel();
}


void CListDlg::OnStnClickedStaticUuu()
{
	// TODO: 在此添加控件通知处理程序代码
}
