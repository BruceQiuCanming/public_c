// CPrintSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "PrintSocket.h"

CPrintSocket G_PrintSocket;

#ifndef WINCE
CDC	G_dcPrinterDC;

void PrintBMP(CString fileName)
{

	


	CBitmap bitmap;
	if(!bitmap.Attach(::LoadImage(   
		::GetModuleHandle(NULL), _T("C:\\123.bmp"), IMAGE_BITMAP, 0, 0,  
		 LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE))) 
	{   
		AfxMessageBox(_T("Error loading bitmap!")); 
		return ;   
   }  
	
	BITMAP bm;   
	bitmap.GetBitmap(&bm);   
	int w = bm.bmWidth;  
	 int h = bm.bmHeight;  
	
	
	G_dcPrinterDC.StartDocW(_T("测试报告"));
	G_dcPrinterDC.StartPage();

   CDC memDC;  
   memDC.CreateCompatibleDC(&G_dcPrinterDC);   
   CBitmap *pBmp = memDC.SelectObject(&bitmap);   
   memDC.SetMapMode(G_dcPrinterDC.GetMapMode());   
   G_dcPrinterDC.SetStretchBltMode(HALFTONE);   
   // now stretchblt to maximum width on page   
   G_dcPrinterDC.StretchBlt(0, 0, w, h, &memDC, 0, 0, w, h, SRCCOPY);  
   // clean up   
   memDC.SelectObject(pBmp);   
  
	G_dcPrinterDC.EndPage();
	G_dcPrinterDC.EndDoc();
}

#endif
// CPrintSocket

CPrintSocket::CPrintSocket()
{
	m_SendPage = -1;
}

CPrintSocket::~CPrintSocket()
{

}


// CPrintSocket 成员函数

void CPrintSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnAccept(nErrorCode);
}

void CPrintSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnConnect(nErrorCode);
}

extern void PrintBMP(CString fileName);

void CPrintSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd *pWnd = AfxGetMainWnd();
	SOCKADDR_IN ClientAddr;
	int len = sizeof(SOCKADDR_IN);
	PRINT_DATA print_data;
	CString sIP;

	int nLen=ReceiveFrom(&print_data, sizeof(print_data), (SOCKADDR*)&ClientAddr, &len, 0);

#ifndef WINCE 
	
	//int nLen = ReceiveFrom(&print_data, sizeof(print_data), sIP, nPort);
	if (nLen < 0)
		return;
	if(nLen == sizeof(print_data))
	{//CE --> PC
		int addr = ClientAddr.sin_addr.S_un.S_un_b.s_b4;
		CFile bmp_file;
		CString filename;
		filename.Format(_T("temp%03d.bmp"),addr); 
		if(print_data.id == 0)
		{
			::DeleteFile(filename); 
		}
		if(bmp_file.Open(filename,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ))
		{
			bmp_file.SeekToEnd();
			bmp_file.Write(print_data.Data,print_data.DataLen);

			bmp_file.Close(); 
		

		
			PRINT_DATA_ANSWER answer;
			answer.MaxDataCounts = print_data.MaxDataCounts;
			answer.id				=	print_data.id;
			answer.DataLen			=	sizeof(answer.PageHeight) + sizeof(answer.PageWidth);  
			answer.PageHeight		=	this->m_PageHeight;
			answer.PageWidth		=	this->m_PageWidth;

			TCHAR ip[100];
			_stprintf(ip,_T("%d.%d.%d.%d"),ClientAddr.sin_addr.S_un.S_un_b.s_b1,ClientAddr.sin_addr.S_un.S_un_b.s_b2,
								ClientAddr.sin_addr.S_un.S_un_b.s_b3,ClientAddr.sin_addr.S_un.S_un_b.s_b4);
			nLen = this->SendTo(&answer,sizeof(PRINT_DATA_ANSWER),8088/*ClientAddr.sin_port*/,ip,0);
			if(nLen == sizeof(PRINT_DATA_ANSWER))
			{
				AfxMessageBox(_T("回发成功!"));
			}
			if(print_data.id == print_data.MaxDataCounts / sizeof(print_data.Data))
			{//文件传输结束，可以打印
				::PrintBMP(BMP_FILE_NAME); 
			}
		}
		
	}
#else //WINCE
	if(nLen == sizeof(PRINT_DATA_ANSWER))
	{
		PRINT_DATA_ANSWER * answer = (PRINT_DATA_ANSWER *)&print_data;
		if(answer->id == this->m_SendPage)
		{
			m_SendPage ++;
			PRINT_DATA data;
			int i;
			CFile bmpFile;
			if(bmpFile.Open(BMP_FILE_NAME,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ) )
			{
				bmpFile.SeekToBegin();
				bmpFile.Seek(m_SendPage*sizeof(data.Data),CFile::begin);
				data.id = m_SendPage;
				
				data.DataLen = bmpFile.Read(data.Data,sizeof(data.Data));
				if(data.DataLen > 0)
				{
					i = G_PrintSocket.Send(&data,sizeof(PRINT_DATA),0);
				}
				else
				{
					G_PrintSocket.Close();
					m_SendPage = -1;
				}

				bmpFile.Close();
			}
		}
	}
#endif
	CSocket::OnReceive(nErrorCode);

}

int CPrintSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CSocket::Receive(lpBuf, nBufLen, nFlags);
}
