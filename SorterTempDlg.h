#pragma once

#include "resource.h"
#include "comm.h"

// CSorterTempDlg 对话框

#define SV_8TEMP_8SSR_DELTA_1		 8.0f
#define SV_8TEMP_8SSR_DELTA_2		 4.0f
#define SV_8TEMP_8SSR_DELTA_3		 2.0f
#define SV_8TEMP_8SSR_DELTA_4		 0.0f
#define SV_8TEMP_8SSR_DELTA_5		 8.0f
#define SV_8TEMP_8SSR_DELTA_6		 4.0f
#define SV_8TEMP_8SSR_DELTA_7		 2.0f
#define SV_8TEMP_8SSR_DELTA_8		 0.0f



class CSorterTempDlg : public CDialog
{
	DECLARE_DYNAMIC(CSorterTempDlg)

public:
	CSorterTempDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSorterTempDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SORTER_TEMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MODBUS_RS485_8TEMP_8SSR_ANSWER			m_Answer_8TEMP_8SSR;
	CTime									m_MeterAnswerTime;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void DealCmdAnswer(unsigned char *para, unsigned char *flash,bool IsCmdAscii,int data_len);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditSv4();
	afx_msg void OnEnSetfocusEditSv8();
	void CSorterTempDlg::SendSv(void);
	void CSorterTempDlg::SendPIDT(int ID);
	afx_msg void OnEnSetfocusEditPidP1();
	afx_msg void OnEnSetfocusEditPidI1();
	afx_msg void OnEnSetfocusEditPidD1();

	afx_msg void OnEnSetfocusEditPidP2();
	afx_msg void OnEnSetfocusEditPidI2();
	afx_msg void OnEnSetfocusEditPidD2();

	afx_msg void OnEnSetfocusEditPidP3();
	afx_msg void OnEnSetfocusEditPidI3();
	afx_msg void OnEnSetfocusEditPidD3();

	afx_msg void OnEnSetfocusEditPidP4();
	afx_msg void OnEnSetfocusEditPidI4();
	afx_msg void OnEnSetfocusEditPidD4();

		afx_msg void OnEnSetfocusEditPidP5();
	afx_msg void OnEnSetfocusEditPidI5();
	afx_msg void OnEnSetfocusEditPidD5();

		afx_msg void OnEnSetfocusEditPidP6();
	afx_msg void OnEnSetfocusEditPidI6();
	afx_msg void OnEnSetfocusEditPidD6();

		afx_msg void OnEnSetfocusEditPidP7();
	afx_msg void OnEnSetfocusEditPidI7();
	afx_msg void OnEnSetfocusEditPidD7();

		afx_msg void OnEnSetfocusEditPidP8();
	afx_msg void OnEnSetfocusEditPidI8();
	afx_msg void OnEnSetfocusEditPidD8();

	afx_msg void OnEnSetfocusEditPV4();
	afx_msg void OnEnSetfocusEditPV8();

	afx_msg void OnEnSetfocusEditAdjust4();
	afx_msg void OnEnSetfocusEditAdjust8();

	bool CSorterTempDlg::IsTempOK(int ID);
	void SendAdjust(void);
	
	afx_msg void OnBnClickedButton1();
};

extern CSorterTempDlg							G_SorterTempDlg;