#pragma once


#include "resource.h"



#include "afxcmn.h"
#include "afxwin.h"


// CSwitchTempAdjustDlg 对话框

class CSwitchTempAdjustDlg : public CDialog
{
	DECLARE_DYNAMIC(CSwitchTempAdjustDlg)

public:
	CSwitchTempAdjustDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSwitchTempAdjustDlg();
	void OnEditClicked(UINT ID);

	CEdit m_Edit[192];
	float  m_AdjustTemp[2][192];
	bool   m_CloseSelected;

// 对话框数据
	enum { IDD = IDD_DIALOG_SWITCH_ADJUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	void SaveAdjust(void);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioOpen();
	afx_msg void OnBnClickedRadioClose();
	CTabCtrl m_Tab;
	afx_msg void OnNMClickTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_RadioClose;
	CButton m_RadioOpen;
	afx_msg void OnBnClickedClearAll();
	void CSwitchTempAdjustDlg::ShowAdjust(void);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
