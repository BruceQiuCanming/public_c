// SwitchTempAdjustDlg.cpp : 实现文件
//
#include "stdafx.h"

#include "SwitchTempAdjustDlg.h"
#include "typedefs.h"
#include "common.h"
#include "InputDlg.h"
#include "welcomedlg.h"

// CSwitchTempAdjustDlg 对话框


IMPLEMENT_DYNAMIC(CSwitchTempAdjustDlg, CDialog)

CSwitchTempAdjustDlg::CSwitchTempAdjustDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSwitchTempAdjustDlg::IDD, pParent)
{
	m_CloseSelected = false;

}

CSwitchTempAdjustDlg::~CSwitchTempAdjustDlg()
{
}

void CSwitchTempAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_RADIO_CLOSE, m_RadioClose);
	DDX_Control(pDX, IDC_RADIO_OPEN, m_RadioOpen);
}


BEGIN_MESSAGE_MAP(CSwitchTempAdjustDlg, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED,10001,10500,OnEditClicked)
	ON_CONTROL_RANGE(EN_SETFOCUS,10001,10200,OnEditClicked)
	ON_BN_CLICKED(IDOK, &CSwitchTempAdjustDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_OPEN, &CSwitchTempAdjustDlg::OnBnClickedRadioOpen)
	ON_BN_CLICKED(IDC_RADIO_CLOSE, &CSwitchTempAdjustDlg::OnBnClickedRadioClose)
	ON_NOTIFY(NM_CLICK, IDC_TAB1, &CSwitchTempAdjustDlg::OnNMClickTab1)
	ON_BN_CLICKED(ID_CLEAR_ALL, &CSwitchTempAdjustDlg::OnBnClickedClearAll)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSwitchTempAdjustDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CSwitchTempAdjustDlg 消息处理程序
void CSwitchTempAdjustDlg::OnEditClicked(UINT ID)
{
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		CWnd *w  = this->GetDlgItem(ID);
		if(w != NULL)
		{
			CString s;
			if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
			{
				int y = (ID - 10001) / G_iLedNrPerLine[0];
				int x = (ID - 10001) % G_iLedNrPerLine[0];
				s.Format(_T("%d%d#= %s"),y,x + 1,dlg.m_Input); 

				m_AdjustTemp[m_CloseSelected][x*G_iLedNrPerLine[0] + y] =  _tcstod(dlg.m_Input,NULL);
			}
			else   if( G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
			{
				int y = (ID - 10001) / G_iLedNrPerLine[0];
				int x = (ID - 10001) % G_iLedNrPerLine[0];

				if(m_Tab.GetCurSel() == 0)
				{
					s.Format(_T("上%d%d#= %s"),y,x + 1,dlg.m_Input);  
					m_AdjustTemp[m_CloseSelected][x*G_iLedNrPerLine[0] + y] =  _tcstod(dlg.m_Input,NULL);

				}
				else
				{
					s.Format(_T("下%d%d#= %s"),y,x + 1,dlg.m_Input);  
					m_AdjustTemp[m_CloseSelected][x*G_iLedNrPerLine[0] + y + 64] =  _tcstod(dlg.m_Input,NULL);
				}
			}
			else
			{
				s.Format(_T("%d#= %s"),ID - 10001 + 1,dlg.m_Input);  
			}

			w->SetWindowTextW(s);  

			SaveAdjust();

		}
	}
}

void CSwitchTempAdjustDlg::ShowAdjust(void)
{

	CString filename = CONFIG_SWITCH_ADJUST_FILE_DIR;
	::CreateDirectory(filename,NULL);
	filename += _T("SwitchTempAdjust.bin");
	CFile para_file;
	int SelectedTabItem = 0;


	for(int y = 0; y < G_iLedLines[0]; y++)
	{
		for(int x = 0; x < G_iLedNrPerLine[0]; x++)
		{
			if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
			{
				CString s;
				s.Format(_T("%d%d#= %5.1f"),y, x +1 ,m_AdjustTemp[m_CloseSelected ][x*G_iLedNrPerLine[0] + y]);  
				m_Edit[y*G_iLedNrPerLine[0] + x].SetWindowTextW(s);
			}
			else if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
			{
				CString s;
				if(SelectedTabItem == 0)
				{
					s.Format(_T("上%d%d#= %5.1f "),y, x +1 ,m_AdjustTemp[m_CloseSelected][x*G_iLedNrPerLine[0] + y]); 
				}
				else
				{
					s.Format(_T("下%d%d#= %5.1f "),y, x +1 ,m_AdjustTemp[m_CloseSelected][64 + x*G_iLedNrPerLine[0] + y]);  
				}
				m_Edit[y*G_iLedNrPerLine[0] + x].SetWindowTextW(s);
			
			}
			else
			{
				if((y * G_iLedNrPerLine[0] + x) < ::G_iMaxLedNr[0]) 
				{
					CString s;
					s.Format(_T("%d#= %5.1f"),y*G_iLedNrPerLine[0] + x +1 ,m_AdjustTemp[m_CloseSelected][y*G_iLedNrPerLine[0] + x]);  
					m_Edit[y*G_iLedNrPerLine[0] + x].SetWindowTextW(s);
				}
			}
		}
	}


}
BOOL CSwitchTempAdjustDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString filename = CONFIG_SWITCH_ADJUST_FILE_DIR;
	::CreateDirectory(filename,NULL);
	filename += _T("SwitchTempAdjust.bin");
	CFile para_file;
	int SelectedTabItem;

	if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
	{
		m_Tab.ShowWindow(SW_SHOW); 
		m_Tab.InsertItem(0,_T("上夹具")); 
		m_Tab.InsertItem(1,_T("下夹具"));
		SelectedTabItem = m_Tab.GetCurSel();
		if(SelectedTabItem < 0)
		{
			SelectedTabItem = 0;
		}
		this->m_RadioClose.ShowWindow(SW_HIDE); 
		this->m_RadioOpen.ShowWindow(SW_HIDE); 
	}
	else
	{
		this->m_RadioClose.ShowWindow(SW_SHOW); 
		this->m_RadioOpen.ShowWindow(SW_SHOW); 
		m_Tab.ShowWindow(SW_HIDE); 
		SelectedTabItem = 0;
	}

	memcpy(G_AdjustTemp,m_AdjustTemp,sizeof(m_AdjustTemp));

	if(para_file.Open(filename,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		para_file.SeekToBegin();
		para_file.Read(G_AdjustTemp,sizeof(G_AdjustTemp));
		memcpy(m_AdjustTemp,G_AdjustTemp,sizeof(m_AdjustTemp));
		para_file.Close();
	}


	this->ShowWindow(SW_SHOWMAXIMIZED);
	RECT wndRect;
	this->GetWindowRect(&wndRect); 

	

	

	for(int y = 0; y < G_iLedLines[0]; y++)
	{
		for(int x = 0; x < G_iLedNrPerLine[0]; x++)
		{
			RECT rect;
			rect.left  = x * (wndRect.right / G_iLedNrPerLine[0]);
			rect.right = rect.left + (wndRect.right / G_iLedNrPerLine[0]) *0.9;
			rect.top   = y * (wndRect.bottom / (G_iLedLines[0] + 2));
			rect.bottom = rect.top + (wndRect.bottom / (G_iLedLines[0] +2)) * 0.9;
			if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
			{
				m_Edit[y*G_iLedNrPerLine[0] + x].Create(WS_BORDER | WS_CHILD | WS_VISIBLE ,rect,this,10001 + y*G_iLedNrPerLine[0] + x); 
				CString s;
				s.Format(_T("%d%d#= %5.1f"),y, x +1 ,m_AdjustTemp[0][x*G_iLedNrPerLine[0] + y]);  
				m_Edit[y*G_iLedNrPerLine[0] + x].SetWindowTextW(s);
			}
			else if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
			{
				m_Edit[y*G_iLedNrPerLine[0] + x].Create(WS_BORDER | WS_CHILD | WS_VISIBLE ,rect,this,10001 + y*G_iLedNrPerLine[0] + x); 
				CString s;
				if(SelectedTabItem == 0)
				{
					s.Format(_T("上%d%d#= %5.1f "),y, x +1 ,m_AdjustTemp[0][x*G_iLedNrPerLine[0] + y]); 
				}
				else
				{
					s.Format(_T("下%d%d#= %5.1f "),y, x +1 ,m_AdjustTemp[0][64 + x*G_iLedNrPerLine[0] + y]);  
				}
				m_Edit[y*G_iLedNrPerLine[0] + x].SetWindowTextW(s);
			
			}
			else
			{
				if((y * G_iLedNrPerLine[0] + x) < ::G_iMaxLedNr[0]) 
				{
					m_Edit[y*G_iLedNrPerLine[0] + x].Create(WS_BORDER | WS_CHILD | WS_VISIBLE ,rect,this,10001 + y*G_iLedNrPerLine[0] + x); 
					CString s;
					s.Format(_T("%d#= %5.1f"),y*G_iLedNrPerLine[0] + x +1 ,m_AdjustTemp[0][y*G_iLedNrPerLine[0] + x]);  
					m_Edit[y*G_iLedNrPerLine[0] + x].SetWindowTextW(s);
				}
			}
		}
	}
	
	ShowAdjust();


	//this->SendMessageW(BN_CLICKED,IDC_RADIO_OPEN,0);

	//this->OnBnClickedRadioOpen(); 

	return true;
}

void CSwitchTempAdjustDlg::SaveAdjust(void)
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename = CONFIG_SWITCH_ADJUST_FILE_DIR;
	::CreateDirectory(filename,NULL);
	filename += _T("SwitchTempAdjust.bin");
	CFile para_file;
	
	//保存修改的参数
	
	for(int i = 0; i < G_iLedLines[0]; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine[0]; j++)
		{
			if((i * G_iLedNrPerLine[0] + j) < ::G_iMaxLedNr[0]) 
			{
				CString s;
				m_Edit[i*G_iLedNrPerLine[0] + j].GetWindowText(s);
				int pos = s.Find(_T("=")) + 1;
				s = s.Right(s.GetLength() - pos);
				if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
				{
					m_AdjustTemp[m_CloseSelected][j*G_iLedNrPerLine[0] + i] =  _tcstod(s,NULL);
				}
				else if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
				{
					if(m_Tab.GetCurSel() == 0)
					{
						m_AdjustTemp[m_CloseSelected][j*G_iLedNrPerLine[0] + i] =  _tcstod(s,NULL);
					}
					else
					{
						m_AdjustTemp[m_CloseSelected][j*G_iLedNrPerLine[0] + i + 64] =  _tcstod(s,NULL);
					}
				}
				else
				{
					m_AdjustTemp[m_CloseSelected][i*G_iLedNrPerLine[0] + j] =  _tcstod(s,NULL);
				}

			}
		}
	}
	

	memcpy(G_AdjustTemp,m_AdjustTemp,sizeof(m_AdjustTemp));

	if(para_file.Open(filename,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		para_file.SeekToBegin();
		para_file.Write(G_AdjustTemp,sizeof(G_AdjustTemp));
		para_file.Close();
	}

}

void CSwitchTempAdjustDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename = CONFIG_SWITCH_ADJUST_FILE_DIR;
	::CreateDirectory(filename,NULL);
	filename += _T("SwitchTempAdjust.bin");
	CFile para_file;
	
	//保存修改的参数
	OnBnClickedRadioClose();  
	OnBnClickedRadioOpen();

	memcpy(G_AdjustTemp,m_AdjustTemp,sizeof(m_AdjustTemp));

	if(para_file.Open(filename,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		para_file.SeekToBegin();
		para_file.Write(G_AdjustTemp,sizeof(G_AdjustTemp));
		para_file.Close();
	}
	OnOK();
}

void CSwitchTempAdjustDlg::OnBnClickedRadioOpen()
{
	// TODO: 在此添加控件通知处理程序代码

	if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2 || G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
	{
		return;
	}

	if(m_CloseSelected == true)
	{
		for(int i = 0; i < G_iLedLines[0]; i++)
		{
			for(int j = 0; j < G_iLedNrPerLine[0]; j++)
			{
				if((i * G_iLedNrPerLine[0] + j) < ::G_iMaxLedNr[0]) 
				{
					CString s;
					m_Edit[i*G_iLedNrPerLine[0] + j].GetWindowText(s);
					int pos = s.Find(_T("=")) + 1;
					s = s.Right(s.GetLength() - pos);
					if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP )
					{
						m_AdjustTemp[1][j*G_iLedNrPerLine[0] + i] =  _tcstod(s,NULL);
					}
					else if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
					{
						if(m_Tab.GetCurSel() == 0)
						{
							m_AdjustTemp[1][j*G_iLedNrPerLine[0] + i] =  _tcstod(s,NULL);
						}
						else
						{
							m_AdjustTemp[1][j*G_iLedNrPerLine[0] + i + 64] =  _tcstod(s,NULL);
						}
					}
					else
					{
					m_AdjustTemp[1][i*G_iLedNrPerLine[0] + j] =  _tcstod(s,NULL);
					}

				}
			}
		}
	}
	m_CloseSelected = false;

	for(int i = 0; i < G_iLedLines[0]; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine[0]; j++)
		{

			if((i * G_iLedNrPerLine[0] + j) < ::G_iMaxLedNr[0]) 
			{
				CString s;
				if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
				{
					s.Format(_T("%d%d#= %5.1f"),i,j +1 ,m_AdjustTemp[0][j*G_iLedNrPerLine[0] + i]);  
				}
				else if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
				{
					if(m_Tab.GetCurSel() == 0)
					{
						s.Format(_T("上%d%d#= %5.1f"),i,j +1 ,m_AdjustTemp[0][j*G_iLedNrPerLine[0] + i]); 
					}
					else
					{
						s.Format(_T("下%d%d#= %5.1f"),i,j +1 ,m_AdjustTemp[0][j*G_iLedNrPerLine[0] + i + 64]); 
					}
				}
				else
				{
					s.Format(_T("%d#= %5.1f"),i*G_iLedNrPerLine[0] + j +1 ,m_AdjustTemp[0][i*G_iLedNrPerLine[0] + j]);  
				}
				m_Edit[i*G_iLedNrPerLine[0] + j].SetWindowTextW(s);
			}
		}
	}
}

void CSwitchTempAdjustDlg::OnBnClickedRadioClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2 || G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
	{
		return;
	}

	if(m_CloseSelected == false)
	{
		for(int i = 0; i < G_iLedLines[0]; i++)
		{
			for(int j = 0; j < G_iLedNrPerLine[0]; j++)
			{
				if((i * G_iLedNrPerLine[0] + j) < ::G_iMaxLedNr[0]) 
				{
					CString s;
					m_Edit[i*G_iLedNrPerLine[0] + j].GetWindowText(s);
					int pos = s.Find(_T("=")) + 1;
					s = s.Right(s.GetLength() - pos);
					if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
					{
						m_AdjustTemp[0][j*G_iLedNrPerLine[0] + i] =  _tcstod(s,NULL);
					}
					else if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
					{
						if(m_Tab.GetCurSel() == 0)
						{
							m_AdjustTemp[0][j*G_iLedNrPerLine[0] + i] =  _tcstod(s,NULL);
						}
						else
						{
							m_AdjustTemp[0][j*G_iLedNrPerLine[0] + i + 64] =  _tcstod(s,NULL);
						}
					}
					else
					{
						m_AdjustTemp[0][i*G_iLedNrPerLine[0] + j] =  _tcstod(s,NULL);
					}

				}
			}
		}
	}
	
	m_CloseSelected = true;

	for(int i = 0; i < G_iLedLines[0]; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine[0]; j++)
		{
			if((i * G_iLedNrPerLine[0] + j) < ::G_iMaxLedNr[0]) 
			{
				CString s;
				if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME  || G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
				{
					s.Format(_T("%d%d#= %5.1f"),i,j +1 ,m_AdjustTemp[1][j*G_iLedNrPerLine[0] + i]); 
				}
				else if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
				{
					if(m_Tab.GetCurSel() == 0)
					{
						s.Format(_T("上%d%d#= %5.1f"),i,j +1 ,m_AdjustTemp[1][j*G_iLedNrPerLine[0] + i]); 
					}
					else
					{
						s.Format(_T("下%d%d#= %5.1f"),i,j +1 ,m_AdjustTemp[1][j*G_iLedNrPerLine[0] + i + 64]); 
					}
				}
				else
				{
					s.Format(_T("%d#= %5.1f"),i*G_iLedNrPerLine[0] + j +1 ,m_AdjustTemp[1][i*G_iLedNrPerLine[0] + j]); 
				}
				m_Edit[i*G_iLedNrPerLine[0] + j].SetWindowTextW(s);
			}
		}
	}


}

void CSwitchTempAdjustDlg::OnNMClickTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString s;
					
	if(G_NormalConfigPara[0].BoxType == BOX_KSD302x2)
	{
		for(int i = 0; i < G_iLedLines[0]; i++)
		{
			for(int j = 0; j < G_iLedNrPerLine[0]; j++)
			{
				if((i * G_iLedNrPerLine[0] + j) < ::G_iMaxLedNr[0]) 
				{
					
					if(m_Tab.GetCurSel() == 0)
					{
						s.Format(_T("上%d%d#= %5.1f"),i,j +1 ,m_AdjustTemp[0][j*G_iLedNrPerLine[0] + i]); 
					}
					else
					{
						s.Format(_T("下%d%d#= %5.1f"),i,j +1 ,m_AdjustTemp[0][j*G_iLedNrPerLine[0] + i + 64]); 
					}
					
					m_Edit[i*G_iLedNrPerLine[0] + j].SetWindowTextW(s);
				}
			}
		}
	}
}

void CSwitchTempAdjustDlg::OnBnClickedClearAll()
{
	// TODO: 在此添加控件通知处理程序代码

	memset(m_AdjustTemp,0,sizeof(m_AdjustTemp)); 
	ShowAdjust();
}

void CSwitchTempAdjustDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
