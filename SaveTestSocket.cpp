// CPrintSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "SaveRecordSocket.h"
#include "typedefs.h"
#include "common.h"

CSaveRecordSocket G_SaveRecordSocket;


// CPrintSocket

CSaveRecordSocket::CSaveRecordSocket()
{
	m_SendPage = -1;
}

CSaveRecordSocket::~CSaveRecordSocket()
{

}


// CPrintSocket 成员函数

void CSaveRecordSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
//AfxMessageBox(_T("OnAccept"));
	CSocket::OnAccept(nErrorCode);
}

void CSaveRecordSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	//AfxMessageBox(_T("OnConnect"));

	CSocket::OnConnect(nErrorCode);
}

extern void PrintBMP(CString fileName);
	char RecBuf[1024*1024];

void CSaveRecordSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd *pWnd = AfxGetMainWnd();
	SOCKADDR_IN ClientAddr;
	int len = sizeof(SOCKADDR_IN);

	CString sIP;

	int nLen=ReceiveFrom(&RecBuf, sizeof(RecBuf), (SOCKADDR*)&ClientAddr, &len, 0);

#ifndef WINCE 
	
	//int nLen = ReceiveFrom(&print_data, sizeof(print_data), sIP, nPort);
	if (nLen < 0)
		return;


	EXCEL_RECORD_FILE_SOCKET_HEADER *excel = (EXCEL_RECORD_FILE_SOCKET_HEADER*)RecBuf;
	if(nLen == (sizeof(EXCEL_RECORD_FILE_SOCKET_HEADER) + excel->filelen))
	{//CE --> PC
		USHORT port  = ClientAddr.sin_port;
		int addr = ClientAddr.sin_addr.S_un.S_un_b.s_b4;

		CString dir = ::CreateCurDateDirection(addr,excel->year,excel->month,excel->day,excel->hour,excel->minute,excel->second);
		CString filename;
		CFile bmp_file;

		filename.Format(_T("\\%02d_%02d_%02d.csv"),excel->hour,excel->minute,excel->second); 
		filename = dir + filename;

		if(bmp_file.Open(filename,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ))
		{
			bmp_file.SeekToEnd();
			bmp_file.Write(&RecBuf[sizeof(EXCEL_RECORD_FILE_SOCKET_HEADER)],excel->filelen);
			bmp_file.Close(); 
		

		
			TCHAR ip[100];
			_stprintf(ip,_T("%d.%d.%d.%d"),ClientAddr.sin_addr.S_un.S_un_b.s_b1,ClientAddr.sin_addr.S_un.S_un_b.s_b2,
								ClientAddr.sin_addr.S_un.S_un_b.s_b3,ClientAddr.sin_addr.S_un.S_un_b.s_b4);
//			nLen = this->SendTo(&answer,sizeof(PRINT_DATA_ANSWER),8188/*ClientAddr.sin_port*/,ip,0);
			
			
		}
		
	}
#else //WINCE
	//AfxMessageBox(_T("OnReceive"));
#endif
	CSocket::OnReceive(nErrorCode);

}

int CSaveRecordSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CSocket::Receive(lpBuf, nBufLen, nFlags);
}
