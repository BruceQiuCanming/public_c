#pragma once

#ifdef WINCE
#define IDD_DIALOG_CURVE                141
#else
#define IDD_DIALOG_CURVE                130
#endif

#include "typedefs.h"
#include "afxwin.h"
#include "comm.h"

// CCurveDlg 对话框
class CCurveDlg : public CDialog
{
	DECLARE_DYNAMIC(CCurveDlg)

public:
	CCurveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCurveDlg();
	virtual			BOOL OnInitDialog();
	CTime			m_BeginTestTime;
	//CTempArray		m_TempArray;
	int				m_NTC_ID;
	int				m_BoxNr;
	CNtcRecordArray	m_NtcRecordArray;
	void DrawTempCurve(int BoxNr);
	void DrawNtcTempCurve(void);
// 对话框数据
	enum { IDD = IDD_DIALOG_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonSave();
};


extern CCurveDlg								G_CurveDlg;