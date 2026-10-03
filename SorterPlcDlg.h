#pragma once
#include "resource.h"



#define PLC_RUN_MODE		0x1000
#define PLC_STOP_MODE		0x2000
#define PLC_MANUAL_MODE		0x4000
#define PLC_RECOVER_MODE	0x8000

// CSorterPlcDlg 对话框

class CSorterPlcDlg : public CDialog
{
	DECLARE_DYNAMIC(CSorterPlcDlg)

public:
	CSorterPlcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSorterPlcDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SORTER_PLC };

	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnStnClickedStaticLowLevelFailCyclinde();

	void Deal_X_Status(unsigned int X_Status);
	void CSorterPlcDlg::Deal_Y_Status(unsigned int Y_Status);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	
	unsigned int m_X_Status;
	unsigned int m_Y_Status;
	CString m_AlarmMsg;

	unsigned int m_Mode;

	unsigned int m_WaitForRunMode;
	unsigned int m_ManualMode;
	unsigned int m_RecoverMode;
	unsigned int m_RunMode;
	unsigned int m_StopMode;

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticLowLevelTestCyclinder();
	afx_msg void OnStnClickedStaticLowLevelInputCyclinder();
	afx_msg void OnStnClickedStaticLowLevelOutputCyclinder();
	afx_msg void OnStnClickedStaticHighLevelOutputCyclinder();
	afx_msg void OnStnClickedStaticHighLevelFailCyclinder();
	afx_msg void OnStnClickedStaticHighLevelPassCyclinder();
	afx_msg void OnStnClickedStaticHighLevelTestCyclinder();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedRecover();
	afx_msg void OnBnClickedAuto();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSetTemp();

	void DisplaySendCmd(unsigned char cmd,unsigned char *cmd_buf,int len,int BoxNr);
	void DisplayReceiveCmd(unsigned char cmd,unsigned char *cmd_buf,int len,int BoxNr);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStaticHighLevelInputCyclinder();
	void DealCmdAnswer(unsigned char *para, unsigned char *flash,bool IsCmdAscii,int data_len);
	afx_msg void OnEnSetfocusEditSvLow();
	afx_msg void OnEnSetfocusEditSvHigh();
	afx_msg void OnEnSetfocusEditAdjLow();
	afx_msg void OnEnSetfocusEditAdjHigh();
	void SendAdjust(void);
	void SendSv(void);
	afx_msg void OnEnKillfocusEditLowLevel();
	afx_msg void OnEnSetfocusEditHighLevel();
	afx_msg void OnEnSetfocusEditMainLevel();
	afx_msg void OnEnChangeEditHighLevel();
	afx_msg void OnEnSetfocusEditLowLevel();
	afx_msg void OnStnClickedStaticIsHasSwitch();
	afx_msg void OnStnClickedStaticLowLevelTemp1();
};

extern CSorterPlcDlg							G_SorterPlcDlg;