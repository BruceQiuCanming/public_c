#include "afxwin.h"
#if !defined(AFX_INPUTADJUSTTEMPTEMPDLG_H__615FB429_BA6A_4EAA_B622_622E171F6280__INCLUDED_)
#define AFX_INPUTADJUSTTEMPTEMPDLG_H__615FB429_BA6A_4EAA_B622_622E171F6280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGlassTempDlg.h : header file
//

#include "resource.h"



/////////////////////////////////////////////////////////////////////////////
// CInputGlassTempDlg dialog

class CInputAutoAdjustTempDlg : public CDialog
{
// Construction
public:
	CInputAutoAdjustTempDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputGlassTempDlg)
	enum { IDD = IDD_DIALOG_INPUT_AUTO_ADJUST_TEMP };
	CString	m_Temp1;
	CString	m_Temp2;
	//}}AFX_DATA
	CString m_Title;
	int	m_InputGlassTemp_ID;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGlassTempDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGlassTempDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_InputGlassTemp_2;
	CEdit m_InputGlassTemp_1;
	afx_msg void OnEnSetfocusEditGlass2();
	afx_msg void OnEnSetfocusEditGlass1();
	afx_msg void OnBnClickedButton1();
	void OnBnClickedButton0_9(CString num);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButtonDot();
	afx_msg void OnBnClickedButtonClear();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGLASSTEMPDLG_H__615FB429_BA6A_4EAA_B622_622E171F6280__INCLUDED_)
