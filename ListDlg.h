#pragma once

#include "resource.h"

#include "afxwin.h"

// CListDlg 对话框

class CListDlg : public CDialog
{
	DECLARE_DYNAMIC(CListDlg)

public:
	CListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LIST };
	virtual BOOL OnInitDialog();
public:
	CStringArray	m_ListItem;
	int				m_iSelectItem;
	CString			m_SelectString;
	CString			m_Tiltle;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_List;
public:
	afx_msg void OnLbnSetfocusList1();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnStnClickedStaticUuu();
};
