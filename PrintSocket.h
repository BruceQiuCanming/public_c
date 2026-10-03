#pragma once

#include <afxsock.h>

#ifdef WINCE
#define BMP_FILE_NAME _T("HardDisk\\temp.bmp")
#else
#define BMP_FILE_NAME _T("C:\\temp.bmp")
#endif
#define MAX_DATA_LEN	(1024*10)

typedef struct
{
	int				MaxDataCounts;
	int				id;
	int				DataLen;
	unsigned char	Data[MAX_DATA_LEN];
}PRINT_DATA;


typedef struct
{
	int				MaxDataCounts;
	int				id;
	int				DataLen;
	int				PageWidth;
	int				PageHeight;
}PRINT_DATA_ANSWER;

typedef CArray<PRINT_DATA ,PRINT_DATA> CPintDataArray;


// CPrintSocket ÃüÁîÄ¿±ê

class CPrintSocket : public CSocket
{
public:
	CPrintSocket();
	virtual ~CPrintSocket();

	int				m_PageHeight;
	int				m_PageWidth;
	int				m_SendPage;
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
};

extern CPrintSocket G_PrintSocket;
