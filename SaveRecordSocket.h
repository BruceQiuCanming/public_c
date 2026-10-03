#pragma once

#include <afxsock.h>

#ifdef WINCE
#define BMP_FILE_NAME _T("HardDisk\\temp.bmp")
#else
#define BMP_FILE_NAME _T("C:\\temp.bmp")
#endif
#define MAX_DATA_LEN	(1024*10)


#define  SAVE_RECORD_SERVER_PORT  8088


// CPrintSocket ÃüÁîÄ¿±ê

class CSaveRecordSocket : public CSocket
{
public:
	CSaveRecordSocket();
	virtual ~CSaveRecordSocket();

	int				m_PageHeight;
	int				m_PageWidth;
	int				m_SendPage;
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
};

extern CSaveRecordSocket G_SaveRecordSocket;
