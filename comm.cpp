// wd_stepDlg.cpp : implementation file
//
#include "stdafx.h"
#include "comm.h"
#include "ListDlg.h"
#include "common.h"
#include "math.h"
#include "InputDlg.h"

#ifdef _HeatBox
#include "typedefs.h"
#ifdef WINCE
#include "welcomedlg.h"
#else
#include "HeatBoxDlg.h"
#endif
#else
	#include "MainDlg.h"
#endif

#ifndef _20PT100_5SSR
#include "SorterPlcDlg.h"



extern CSorterPlcDlg G_SorterPlcDlg;
#endif
unsigned char	HexBuf[1024*1024];
int				HexBufLen;
#ifndef WINCE

#include "shlwapi.h"

#pragma comment(lib,"shlwapi.lib")

#endif
unsigned char STM32_CopyRight[8] = {0,0,0,0,0,0,0,0};
unsigned char STM32_ID[][12]=
{
	{0x31,0xFF,0x6F,0x06,0x30,0x51,0x57,0x30,0x10,0x18,0x16,0x43},
};

/////////////////////////////////////////////////////////////////////////////
// CWd_stepDlg dialog

//	ON_MESSAGE(COMM_MSG, OnComm)

DELTA_PLC_SET_CMD	G_Delta_PLC_SetCmd;
DELTA_PLC_READ_CMD	G_Delta_PLC_ReadCmd;

CComm				G_RS485_Comm;

CDelta_PLC_SetCmdArray			G_Delta_PLC_SetCmdArray;
CCommCmdArray_OMRON				m_SendCmdArray_OMRON;
CCommCmdArray_32PT100			m_CmdArray_32PT100[MAX_HEAT_BOX];
CCommCmdArray_128				m_CmdArray_128[MAX_HEAT_BOX];
CCommCmdArray_192				m_CmdArray_192[MAX_HEAT_BOX];
CCommCmdArray_300				m_CmdArray_300[MAX_HEAT_BOX];
CCommCmdArray_DetlaFrequnce_ASC m_SendCmdDeltaFrequnce;

CCommCmdProgram					G_SendmCmdProgram;
int								G_ProgramBoxNr;
CCommCmdProgram_ARM				G_SendmCmdProgram_ARM;

C20Temp5SsrAnswerArray			G_20Temp5SsrAnswerArray[2];

extern CString theAppDirectory;

CWnd * G_pAboutDlg = NULL;

int		G_iLangID = 0;

bool G_bCMD_192_READ_STATUS = false;

#ifdef _ASCII_
#ifdef _DEBUG
	CString strMsgCaption = "热保护器测试系统     ";
#endif

#endif



COMM_THREAD_PARA 	G_RS485_CommPara;

COMM_THREAD_PARA 	G_PrntCommPara_RS232;

CComm				G_PrintRS232_Comm;



COMM_PROTOCOL_TYPE	G_CommProtocolType;
#ifdef WINCE
int					G_iCmdAscii[MAX_HEAT_BOX] = {0};
#else
int					G_iCmdAscii[MAX_HEAT_BOX] = {0,0,0,0,0};
#endif

unsigned char Hex2Char(unsigned char Hex)
{
	unsigned char data = 0xFF;
	if(Hex >= '0' && Hex <= '9')
	{
		data = (Hex - '0');
	}
	else if(Hex >= 'A' && Hex <= 'F')
	{
		data = (Hex - 'A' + 10 );
	}
	else if(Hex >= 'a' && Hex <= 'f')
	{
		data = (Hex - 'a' + 10 );
	}
	return data;
}

BOOL FreeMyData(LPCTSTR ResName, LPCTSTR ResType,  int Type,unsigned char *HexBuf,int *HexBufLen) 
{ 
	BOOL bRet = TRUE; 
	HINSTANCE hInst = AfxGetInstanceHandle();
	//先判断我们指定的资源是否存在 
	HRSRC hResInfo = FindResource(hInst,ResName,ResType);
	if(hResInfo == NULL) 
		return FALSE;
	HANDLE hFile = NULL; 
	DWORD dwWritten = 0;
	//开始调入指定的资源到内存 
	HGLOBAL hResData = LoadResource(hInst,hResInfo); 
	LPVOID pvResData = LockResource(hResData); 
	DWORD dwResSize = SizeofResource(hInst,hResInfo);

	*HexBufLen = dwResSize;
	memcpy(HexBuf,pvResData,dwResSize);
	
	//再创建目标文件并保存数据至文件 
	//if(Type == 1)//系统加隐藏属性 如果指定类型为1,那么加上系统和隐藏属性,防止普通用户对文件进行破坏 
	//{ 
	//	hFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
	//	NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM, NULL); 
	//	WriteFile(hFile, pvResData, dwResSize, &dwWritten, NULL); 
	//} 
	//else 
	//{ 
	//	hFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
	//	NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL); 
	//	if(WriteFile(hFile, pvResData, dwResSize, &dwWritten, NULL)	== TRUE) 
	//	{
	//	//	AfxMessageBox(FileName);
	//	}
	//}

	//if(hFile == INVALID_HANDLE_VALUE)//创建文件失败 
	//{ 
	//	FreeResource(hResData); 
	//	return FALSE; 
	//}
	////释放有关内存资源 
	//CloseHandle(hFile); 

	FreeResource(hResData);
	return TRUE; 
}

bool IsValidTemp(float temp)
{
	if(fabsf((int)(temp - (float)INVALID_TEMP)) < 10.0f)
	{
		return false;
	}
	return true;
}

unsigned char CalCheckSum(char *buf,int len)
{
	unsigned char chksum = 0;
	
	for(int i=0; i<len; i++)
			chksum+=buf[i];

	return chksum;

}

unsigned char Buf2Char(unsigned char * buf)
{
	//高位在前，低位在后
	unsigned char ret = 0;
	unsigned char c = buf[0];

	//if(c >= '0' && c <= '9')
	//{
	//	c = c-'0';
	//}
	//else if (c >= 'a' && c <= 'f')
	//{
	//	c = c-'a' + 10;
	//}
	//else if (c >= 'A' && c <= 'F')
	//{
	//	c = c-'A' + 10;
	//}
	ret = (c - '0') * 16;
	
	c = buf[1];
	//if(c >= '0' && c <= '9')
	//{
	//	c = c-'0';
	//}
	//else if (c >= 'a' && c <= 'f')
	//{
	//	c = c-'a' + 10;
	//}
	//else if (c >= 'A' && c <= 'F')
	//{
	//	c = c-'A' + 10;
	//}
	c = c - '0';
	ret += c;
	
	return ret;
}


void WriteLogFile(TCHAR * msg, int BoxNr, bool NoTimeLimit)
{
	CTime cur = CTime::GetCurrentTime();
	TCHAR buff[100];
	memset(buff,0,sizeof(buff));
	swprintf(buff,_T("\r\n%04d/%02d/%02d %02d:%02d:%02d\r\n"),
		cur.GetYear(),cur.GetMonth(),cur.GetDay(),
		cur.GetHour(),cur.GetMinute(),cur.GetSecond());

	CString sDir = theAppDirectory;

	sDir += _T("\log");
	
	::CreateDirectory(sDir ,NULL);

	CString logfile;
	logfile.Format(_T("\\logfile_%04d年%02d月%02d日.txt"),
		cur.GetYear(),cur.GetMonth(),cur.GetDay());
	logfile = sDir + logfile;


	CFile f(logfile,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate);
	f.SeekToEnd();
	f.Write(buff,wcslen(buff)*sizeof(TCHAR));
	f.Write(msg,wcslen(msg)*sizeof(TCHAR));
	f.Close();
}
void WriteLogFile(CString msg, int BoxNr, bool NoTimeLimit)
{
	if(::G_NormalConfigPara[0].bDebugMode == false)
	{
		return;
	}
	CTime cur = CTime::GetCurrentTime();
	
	CString sDir = theAppDirectory;

	sDir += _T("\log");
	
	::CreateDirectory(sDir ,NULL);

	CString logfile;
	logfile.Format(_T("\\logfile%d_%04d年%02d月%02d日.txt"),
		BoxNr+1,
		cur.GetYear(),cur.GetMonth(),cur.GetDay());
	logfile = sDir + logfile;


	CFile f(logfile,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate);
	f.SeekToEnd();
	f.Write(msg,msg.GetLength()*sizeof(TCHAR));
	f.Close();
}
void WriteLogFile(char * msg, int BoxNr, bool NoTimeLimit)
{
	CFile f(_T("logfile.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate);

	f.Write(msg,strlen(msg));
	f.Close();
}
/////////////////////////////////////////////////////////////////////////////

bool    G_IsJustCmdAscii;

void GetUsefulCommPort(CStringArray *CommStrArray)
{
	HKEY hKey;

	if(::RegOpenKeyEx(   HKEY_LOCAL_MACHINE,  
        _T("Hardware\\DeviceMap\\SerialComm"),  
        NULL,  
        KEY_READ,  
        &hKey )   ==   ERROR_SUCCESS )   //打开串口注册表对应的键值 
    {
        int i=0;
        TCHAR portName[256],commName[256];  
        DWORD dwLong,dwSize;  
        while(1)  
        {  
            dwLong   =   dwSize   =   sizeof(portName);  
            if( ::RegEnumValue(hKey,  
                i,  
                portName,  
                &dwLong,  
                NULL,  
                NULL,  
                (PUCHAR)commName,  
                &dwSize   )   ==   ERROR_NO_MORE_ITEMS )//   枚举串口  
                break;  
				CommStrArray->Add(   commName   );   // commName就是串口名字  
            i++;
        }
//        if(   pComboBox->GetCount()   ==   0   )  
   //     {  
    //        ::AfxMessageBox(_T("在HKEY_LOCAL_MACHINE:Hardware//DeviceMap//SerialComm里找不到串口!!!"));  
    //    }  
        RegCloseKey(hKey);  
    }

}

UINT ReadCommThreadProc_Delta_PLC(LPVOID pParam)
{
	COMM_THREAD_PARA *para = ((COMM_THREAD_PARA*)pParam);
	CComm *com = (CComm *)para->comm ;
	CByteArray     ReadArray;
	
	ReadArray.RemoveAll();

	static DWORD	tickcount = 0;
	static bool flashed = true;

		int l=0;
		char rec;

		while(com->bReadComm)
		{
			
			if(G_pAboutDlg != 0)
			{
				
				if((GetTickCount() - tickcount > 20 ) && ( flashed == false))
				{
					flashed = true;
					TCHAR buf[1024];
					wsprintf(buf,_T("%s"),_T("\r\n"));
					if(G_pAboutDlg->GetSafeHwnd() != NULL && G_pAboutDlg->IsWindowVisible())
					{
						::SendMessage(G_pAboutDlg->m_hWnd,COMM_MSG,(WPARAM)buf,wcslen(buf)*sizeof(TCHAR));
					}
				}
			}
			
			l=com->ReadComm(&rec,1);
			
			if(l <= 0)
			{

			}
			else if(l==1)
			{
			
				ReadArray.Add(rec); 
		
#ifdef WINCE
				if(::G_NormalConfigPara[0].BoxType == BOX_DELTA_PLC_8PT100_8SSR
					|| G_NormalConfigPara[0].BoxType == BOX_8PT100_8SSR)
				{
					ReadDeltaPLC(para,&ReadArray);
				}

#endif
					
			}
		}

		return 0;
}

void ReadDeltaPLC(COMM_THREAD_PARA *para,CByteArray     *ReadArray)
{
					if(ReadArray->GetSize() >= sizeof(DELTA_PLC_READ_CMD))
					{
						
							DELTA_PLC_READ_CMD    *answer_para =(DELTA_PLC_READ_CMD*) &(ReadArray->GetData()[ReadArray->GetSize() - sizeof(DELTA_PLC_READ_CMD)]);
							unsigned short lcr;
							lcr  = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer_para->head.Addr[0],sizeof(DELTA_PLC_READ_CMD) - 5);

							if(lcr == Hex2Char(answer_para->LCR[0]) * 0x10 + Hex2Char(answer_para->LCR[1])
								&& answer_para->head.STX == ':'
								&& answer_para->Tail[0]  == 0x0D
								&& answer_para->Tail[1]  == 0x0A)
							{
								
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_para,sizeof(DELTA_PLC_READ_CMD));
									}

									ReadArray->RemoveAll();
							}

					}


					if(ReadArray->GetSize() >= sizeof(DELTA_PLC_READ_C_ANSWER))
					{
							DELTA_PLC_READ_C_ANSWER    *answer_para =(DELTA_PLC_READ_C_ANSWER*) &(ReadArray->GetData()[ReadArray->GetSize() - sizeof(DELTA_PLC_READ_C_ANSWER)]);
							unsigned short lcr;
							lcr  = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer_para->head.Addr[0],sizeof(DELTA_PLC_READ_C_ANSWER) - 5);

							if(lcr == Hex2Char(answer_para->LCR[0]) * 0x10 + Hex2Char(answer_para->LCR[1])
								&& answer_para->head.STX == ':'
								&& answer_para->Tail[0]  == 0x0D
								&& answer_para->Tail[1]  == 0x0A)
							{
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_para,sizeof(DELTA_PLC_READ_C_ANSWER));
									}

									ReadArray->RemoveAll();
							}

					}

					if(ReadArray->GetSize() >= sizeof(DELTA_PLC_READ_D_ANSWER))
					{
							DELTA_PLC_READ_D_ANSWER    *answer_para =(DELTA_PLC_READ_D_ANSWER*) &(ReadArray->GetData()[ReadArray->GetSize() - sizeof(DELTA_PLC_READ_D_ANSWER)]);
							unsigned short lcr;
							lcr  = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer_para->head.Addr[0],sizeof(DELTA_PLC_READ_D_ANSWER) - 5);

							if(lcr == Hex2Char(answer_para->LCR[0]) * 0x10 + Hex2Char(answer_para->LCR[1])
								&& answer_para->head.STX == ':'
								&& answer_para->Tail[0]  == 0x0D
								&& answer_para->Tail[1]  == 0x0A)
							{
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_para,sizeof(DELTA_PLC_READ_D_ANSWER));
									}

									ReadArray->RemoveAll();
							}

					}

					if(ReadArray->GetSize() >= sizeof(DELTA_PLC_READ_X_ANSWER))
					{
							DELTA_PLC_READ_X_ANSWER    *answer_para =(DELTA_PLC_READ_X_ANSWER*) &(ReadArray->GetData()[ReadArray->GetSize() - sizeof(DELTA_PLC_READ_X_ANSWER)]);
							unsigned short lcr;
							lcr  = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer_para->head.Addr[0],sizeof(DELTA_PLC_READ_X_ANSWER) - 5);

							if(lcr == Hex2Char(answer_para->LCR[0]) * 0x10 + Hex2Char(answer_para->LCR[1])
								&& answer_para->head.STX == ':'
								&& answer_para->Tail[0]  == 0x0D
								&& answer_para->Tail[1]  == 0x0A)
							{
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_para,sizeof(DELTA_PLC_READ_X_ANSWER));
									}

									ReadArray->RemoveAll();
							}

					}
					
}

UINT ReadCommThreadProc(LPVOID pParam)
{
	COMM_THREAD_PARA *para = ((COMM_THREAD_PARA*)pParam);
	CComm *com = (CComm *)para->comm ;
	CByteArray     ReadArray;
	
	ReadArray.RemoveAll();

	static DWORD	tickcount = 0;
	static bool flashed = true;

		int l=0;
		char rec;

		while(com->bReadComm)
		{
			
			if((GetTickCount() - tickcount > 20 ) && ( flashed == false))
			{
				flashed = true;
				TCHAR buf[1024];
				wsprintf(buf,_T("%s"),_T("\r\n"));
				//if(G_pAboutDlg != 0)
				{
					//if(G_pAboutDlg->GetSafeHwnd() != NULL && G_pAboutDlg->IsWindowVisible())
					{
						::SendMessage(para->m_hWnd,COMM_MSG_1,(WPARAM)buf,wcslen(buf)*sizeof(TCHAR));
					}
				}
			}

			l=com->ReadComm(&rec,1);
			if(l < 0)
			{

				
			}
			else if((l==1) && (G_IsJustCmdAscii == false))
			{
				flashed = false;

				//if(G_pAboutDlg->GetSafeHwnd() != NULL && G_pAboutDlg->IsWindowVisible())
				{
					
					if(GetTickCount() - tickcount > 20)
					{
						
						TCHAR buf[1024];
						wsprintf(buf,_T("%s"),_T("\r\n"));
						::SendMessage(G_pAboutDlg->m_hWnd,COMM_MSG_1,(WPARAM)buf,wcslen(buf)*sizeof(TCHAR));
					}
					tickcount = GetTickCount();

					
					
					{
						
						TCHAR buf[1024];
						wsprintf(buf,_T("%02X "),(unsigned char) rec);
						::SendMessage(para->m_hWnd,COMM_MSG_1,(WPARAM)buf,wcslen(buf)*sizeof(TCHAR));
					}

							
					
									
				}
				
					ReadArray.Add(rec); 
		
#ifndef WINCE
				if(::G_NormalConfigPara[0].BoxType == BOX_DELTA_PLC_8PT100_8SSR)
				{
					ReadDeltaPLC(para,&ReadArray);
				}
#endif
					
					
					{

			
							if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_32TEMP_ANSWER))
							{

									MODBUS_RS485_32TEMP_ANSWER    *answer_para =(MODBUS_RS485_32TEMP_ANSWER*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_32TEMP_ANSWER)]);
									unsigned short crc;
									crc  = G_RS485_Comm.CRC16((unsigned char*)answer_para,sizeof(MODBUS_RS485_32TEMP_ANSWER) - 2);

									if((crc == answer_para->crc)
											&& (answer_para->head.para_len == (unsigned char)(sizeof(MODBUS_RS485_32TEMP_ANSWER) - 5))
											&& (answer_para->head.RS485_Addr >= '1'  )
											&& (answer_para->head.RS485_Addr <= '9'  ))
											//&& (answer_ntc_read->cmd == CMD_READ_TEMP))
									{
												
											if(para->m_hWnd != NULL)
											{
												
												::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_para,sizeof(MODBUS_RS485_32TEMP_ANSWER));
											}

											ReadArray.RemoveAll();
										
									}
							}
						
						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER))
						{

							MODBUS_RS485_8TEMP_8SSR_ANSWER    *answer_para =(MODBUS_RS485_8TEMP_8SSR_ANSWER*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER)]);
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16((unsigned char*)answer_para,sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER) - 2);

							if((crc == answer_para->crc)
									&& (answer_para->head.para_len == sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER) - 5)
									&& (answer_para->head.RS485_Addr >= '1'  )
									&& (answer_para->head.RS485_Addr <= '9'  ))
									//&& (answer_ntc_read->cmd == CMD_READ_TEMP))
							{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_para,sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER));
									}

									ReadArray.RemoveAll();
								
							}

							
							

						}
						
						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_PROGRAM))
						{
							MODBUS_RS485_CMD_PROGRAM    *answer_para =(MODBUS_RS485_CMD_PROGRAM*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_PROGRAM)]);
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16((unsigned char*)answer_para,sizeof(MODBUS_RS485_CMD_PROGRAM) - 4);

							if((crc == answer_para->crc)
									&& (answer_para->head.para_len == sizeof(answer_para->para))
									&& (answer_para->head.RS485_Addr >= '1'  )
									&& (answer_para->head.RS485_Addr <= '9'  ))
									//&& (answer_para->Tail[0] == 0x0D)
									//&& (answer_para->Tail[1] == 0x0A))
							{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_para,sizeof(MODBUS_RS485_CMD_PROGRAM));
									}

									ReadArray.RemoveAll();
								
							}
							

						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_PARA))
						{
							MODBUS_RS485_CMD_PARA    *answer_para =(MODBUS_RS485_CMD_PARA*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_PARA)]);
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16((unsigned char*)answer_para,sizeof(MODBUS_RS485_CMD_PARA) - 2);

							if((crc == answer_para->crc)
									&& (answer_para->head.para_len == sizeof(answer_para->EEPROM_Answer)+sizeof(answer_para->Temp_Answer) + sizeof(answer_para->para))
									&& (answer_para->head.RS485_Addr >= '1'  )
									&& (answer_para->head.RS485_Addr <= '9'  ))
									//&& (answer_ntc_read->cmd == CMD_READ_TEMP))
							{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_para,sizeof(MODBUS_RS485_CMD_PARA));
									}

									ReadArray.RemoveAll();
								
							}
							

						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_ARM_128))
						{
							MODBUS_RS485_CMD_ARM_128    *answer_arm =(MODBUS_RS485_CMD_ARM_128*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_ARM_128)]);
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16((unsigned char*)answer_arm,sizeof(MODBUS_RS485_CMD_ARM_128) - 2);

							if((crc == answer_arm->crc)
									&& (answer_arm->head.para_len == sizeof(answer_arm->para))
									&& (answer_arm->head.RS485_Addr >= '1'  )
									&& (answer_arm->head.RS485_Addr <= '9'  ))
									//&& (answer_ntc_read->cmd == CMD_READ_TEMP))
							{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_arm,sizeof(MODBUS_RS485_CMD_ARM_128));
									}

									ReadArray.RemoveAll();
								
							}
							

						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_128))
						{
						
							MODBUS_RS485_CMD_128    *answer_arm =(MODBUS_RS485_CMD_128*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_128)]);
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16((unsigned char*)answer_arm,sizeof(MODBUS_RS485_CMD_128) - 2);

							if((crc == answer_arm->crc)
									&& (answer_arm->head.para_len == sizeof(answer_arm->para))
									&& (answer_arm->head.RS485_Addr >= '1'  )
									&& (answer_arm->head.RS485_Addr <= '9'  ))
									//&& (answer_ntc_read->cmd == CMD_READ_TEMP))
							{
									
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_arm,sizeof(MODBUS_RS485_CMD_128));
									}

									ReadArray.RemoveAll();
								
							}
							
							if((crc == answer_arm->crc)
									&& (answer_arm->head.para_len == sizeof(answer_arm->para))
									&& (answer_arm->head.RS485_Addr >= 'A'  )
									&& (answer_arm->head.RS485_Addr <= 'Z'  ))
									//&& (answer_ntc_read->cmd == CMD_READ_TEMP))
							{
									
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_arm,sizeof(MODBUS_RS485_CMD_128));
									}

									ReadArray.RemoveAll();
								
							}
							

						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_20TEMP_5SSR_ANSWER))
						{
							MODBUS_RS485_20TEMP_5SSR_ANSWER    *answer_ntc_read =(MODBUS_RS485_20TEMP_5SSR_ANSWER*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_20TEMP_5SSR_ANSWER)]);
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16((unsigned char*)answer_ntc_read,sizeof(MODBUS_RS485_20TEMP_5SSR_ANSWER) - 2);

							if((crc == answer_ntc_read->crc)
									&& (answer_ntc_read->head.para_len == sizeof(answer_ntc_read->para))
									&& (answer_ntc_read->head.RS485_Addr >= '1'  )
									&& (answer_ntc_read->head.RS485_Addr <= '9'  )
									&& (answer_ntc_read->head.cmd == CMD_READ_TEMP
									|| answer_ntc_read->head.cmd == CMD_NORMAL_PID_OUT
									|| answer_ntc_read->head.cmd == CMD_READ_PID_T
									|| answer_ntc_read->head.cmd == CMD_SET_PID_T))
							{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_ntc_read,sizeof(MODBUS_RS485_20TEMP_5SSR_ANSWER));
									}

									ReadArray.RemoveAll();
								
							}
							

						}


						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_NTC_AD_VAL_ANSWER))
						{
							MODBUS_RS485_NTC_AD_VAL_ANSWER    *answer_ntc_read =(MODBUS_RS485_NTC_AD_VAL_ANSWER*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_NTC_AD_VAL_ANSWER)]);
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16((unsigned char*)answer_ntc_read,sizeof(MODBUS_RS485_NTC_AD_VAL_ANSWER) - 2);

							if((crc == answer_ntc_read->crc)
									&& (answer_ntc_read->head.para_len == sizeof(answer_ntc_read->para))
									&& (answer_ntc_read->head.RS485_Addr == 'A'  )
									&& (answer_ntc_read->head.cmd == CMD_READ_STATUS))
							{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_ntc_read,sizeof(MODBUS_RS485_NTC_AD_VAL_ANSWER));
									}

									ReadArray.RemoveAll();
								
							}
							

						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE))
						{
							MODBUS_RS485_ANSWER_OMRON_WRITE    *answer_omron_read =(MODBUS_RS485_ANSWER_OMRON_WRITE*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE)]);
							
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16_MODBUS((unsigned char*)answer_omron_read,sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE) - 2);

							
							if((crc == answer_omron_read->crc)
								//	&& (answer_omron_read->para_len == sizeof(answer_omron_read->para))
									&& (answer_omron_read->RS485_Addr == 1  )
									&& (answer_omron_read->cmd == CMD_OMRON_WRITE))
							{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_omron_read,sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE));
									}

									ReadArray.RemoveAll();
								
							}
							
						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_ANSWER_OMRON_READ))
						{
							MODBUS_RS485_ANSWER_OMRON_READ    *answer_omron_read =(MODBUS_RS485_ANSWER_OMRON_READ*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_ANSWER_OMRON_READ)]);
							
							unsigned short crc;
							crc  = G_RS485_Comm.CRC16_MODBUS((unsigned char*)answer_omron_read,sizeof(MODBUS_RS485_ANSWER_OMRON_READ) - 2);

							
							if((crc == answer_omron_read->crc)
									&& (answer_omron_read->para_len == sizeof(answer_omron_read->para))
									&& (answer_omron_read->RS485_Addr == 1  ))
							{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_omron_read,sizeof(MODBUS_RS485_ANSWER_OMRON_READ));
									}

									ReadArray.RemoveAll();
								
							}
							
						}

						if(/*(G_bCMD_192_READ_STATUS == false) &&*/ ReadArray.GetSize() >= sizeof(MODBUS_RS485_DISC_24_ANSWER))
						{
							MODBUS_RS485_DISC_24_ANSWER    *Bimetal =(MODBUS_RS485_DISC_24_ANSWER*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_DISC_24_ANSWER)]);
							
							if((Bimetal->crc == CComm::CRC16((unsigned char*)Bimetal,sizeof(MODBUS_RS485_DISC_24_ANSWER) -2))
									&& (Bimetal->head.RS485_Addr>= 'A' && Bimetal->head.RS485_Addr<= 'D')
									&& (Bimetal->head.para_len == sizeof(Bimetal->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)Bimetal,sizeof(MODBUS_RS485_DISC_24_ANSWER));
									}

									ReadArray.RemoveAll();
									
								}
							else
							{
						//		AfxMessageBox(_T("1"));
							}
						}


						if((G_bCMD_192_READ_STATUS == false) && ReadArray.GetSize() >= sizeof(MODBUS_RS485_DISC_ANSWER))
						{
							MODBUS_RS485_DISC_ANSWER    *Bimetal =(MODBUS_RS485_DISC_ANSWER*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_DISC_ANSWER)]);
							
							if((Bimetal->crc == CComm::CRC16((unsigned char*)Bimetal,sizeof(MODBUS_RS485_DISC_ANSWER) -2))
									&& (Bimetal->head.RS485_Addr>= 'A' && Bimetal->head.RS485_Addr<= 'D')
									&& (Bimetal->head.para_len == sizeof(Bimetal->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)Bimetal,sizeof(MODBUS_RS485_DISC_ANSWER));
									}

									ReadArray.RemoveAll();
									
								}
							else
							{
						//		AfxMessageBox(_T("1"));
							}
						}

						

						//int i;
						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_TICK_COUNT_ANSWER))
						{
							MODBUS_RS485_TICK_COUNT_ANSWER    *modbus_cmd_tick =(MODBUS_RS485_TICK_COUNT_ANSWER*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_TICK_COUNT_ANSWER)]);
							
							if((modbus_cmd_tick->crc == CComm::CRC16((unsigned char*)modbus_cmd_tick,sizeof(MODBUS_RS485_TICK_COUNT_ANSWER) -2))
									&& (modbus_cmd_tick->head.RS485_Addr>= '1' && modbus_cmd_tick->head.RS485_Addr<= '9')
									&& (modbus_cmd_tick->head.para_len == sizeof(modbus_cmd_tick->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_cmd_tick,sizeof(MODBUS_RS485_TICK_COUNT_ANSWER));
									}

									ReadArray.RemoveAll();
									
								}
							else
							{
						//		AfxMessageBox(_T("1"));
							}
						}

						if((ReadArray.GetSize() >= sizeof(DELTA_FREQUNCE_ANSWER_ASC)) && (G_CommProtocolType == COMM_PROTOCOL_DELTA_FREQUENCE_ASC))
						{
							DELTA_FREQUNCE_ANSWER_ASC    *answer =(DELTA_FREQUNCE_ANSWER_ASC*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(DELTA_FREQUNCE_ANSWER_ASC)]);
							
							if(answer->Head == ':'
								&& answer->RS485Addr[0] == 0x30
								&& answer->RS485Addr[1] == 0x31
								&& answer->cmd[0]  == 0x30 
								&& (answer->cmd[1] == 0x33 || answer->cmd[1] == 0x36) 
								&& answer->end[0] == 0x0D
								&& answer->end[1] == 0x0A )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer,sizeof(DELTA_FREQUNCE_ANSWER_ASC));
									}

									ReadArray.RemoveAll();
									
								}
							else
							{
						//		AfxMessageBox(_T("1"));
							}
						}
					}
					{
						//int i;
						if((G_bCMD_192_READ_STATUS == false) && ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_128))
						{
							MODBUS_RS485_CMD_128    *modbus_cmd_128 =(MODBUS_RS485_CMD_128*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_128)]);
							
							if((modbus_cmd_128->crc == CComm::CRC16((unsigned char*)modbus_cmd_128,sizeof(MODBUS_RS485_CMD_128) -2))
									&& (modbus_cmd_128->head.RS485_Addr>= '1' && modbus_cmd_128->head.RS485_Addr<= '9')
									&& (modbus_cmd_128->head.para_len == sizeof(modbus_cmd_128->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_cmd_128,sizeof(MODBUS_RS485_CMD_128));
									}
								//	AfxMessageBox(_T("asdsa"));
									ReadArray.RemoveAll();
									
								}
							else
							{
						//		AfxMessageBox(_T("1"));
							}
						}
						
						if((G_bCMD_192_READ_STATUS == false) && ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_128))
						{
							MODBUS_RS485_CMD_128    *modbus_cmd_128 =(MODBUS_RS485_CMD_128*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_128)]);
							
							if((modbus_cmd_128->crc == CComm::CalCheckSum_OIL((unsigned char*)modbus_cmd_128,sizeof(MODBUS_RS485_CMD_128) -2))
									&& (modbus_cmd_128->head.RS485_Addr>= '1' && modbus_cmd_128->head.RS485_Addr<= '9')
									&& (modbus_cmd_128->head.para_len == sizeof(modbus_cmd_128->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_cmd_128,sizeof(MODBUS_RS485_CMD_128));
									}
									
									ReadArray.RemoveAll();
									
								}
							else
							{
						//		AfxMessageBox(_T("1"));
							}
						}

						if((G_bCMD_192_READ_STATUS == false) && ReadArray.GetSize() >= sizeof(MODBUS_RS485_ANSWER_300))
						{
							MODBUS_RS485_ANSWER_300    *modbus_cmd_300 =(MODBUS_RS485_ANSWER_300*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_ANSWER_300)]);
							
							if((modbus_cmd_300->crc == CComm::CRC16((unsigned char*)modbus_cmd_300,sizeof(MODBUS_RS485_ANSWER_300) -2))
									&& (modbus_cmd_300->head.RS485_Addr>= '1' && modbus_cmd_300->head.RS485_Addr<= '9')
									&& (modbus_cmd_300->head.para_len == sizeof(modbus_cmd_300->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_cmd_300,sizeof(MODBUS_RS485_ANSWER_300));
									}

									ReadArray.RemoveAll();
									
								}
							else
							{
						//		AfxMessageBox(_T("1"));
							}
						}

						

						if((G_bCMD_192_READ_STATUS == false) && (ReadArray.GetSize() >= sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128)))
						{
							MODBUS_RS485_STATUS_FLASH_ANSWER_128    *modbus_status_128 =(MODBUS_RS485_STATUS_FLASH_ANSWER_128*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128)]);
							
							if((modbus_status_128->head.RS485_Addr >= 'A' && modbus_status_128->head.RS485_Addr <= 'Z')
									&&  (modbus_status_128->head.para_len == sizeof(modbus_status_128->para))
									&& (modbus_status_128->crc == CComm::CRC16((unsigned char*)modbus_status_128,sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128) -2)))
								
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_status_128,sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128));
									}

									ReadArray.RemoveAll();
									
								}
						}

						if((G_bCMD_192_READ_STATUS == false) && ReadArray.GetSize() >= sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128))
						{//OIL
							MODBUS_RS485_STATUS_FLASH_ANSWER_128    *modbus_status_128 =(MODBUS_RS485_STATUS_FLASH_ANSWER_128*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128)]);
							
							if((modbus_status_128->crc == CComm::CalCheckSum_OIL((unsigned char*)modbus_status_128,sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128) -2))
									&&  (modbus_status_128->head.RS485_Addr >= 'A' && modbus_status_128->head.RS485_Addr <= 'Z')
									&&  (modbus_status_128->head.para_len == sizeof(modbus_status_128->para)) 
									)
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_status_128,sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128));
									}

									ReadArray.RemoveAll();
									
								}
						}

						if((G_bCMD_192_READ_STATUS == false) && ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_192))
						{
							MODBUS_RS485_CMD_192    *modbus_cmd_192 =(MODBUS_RS485_CMD_192*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_192)]);
							
							if((modbus_cmd_192->crc == CComm::CRC16((unsigned char*)modbus_cmd_192,sizeof(MODBUS_RS485_CMD_192) - 2))
									&& ((modbus_cmd_192->head.RS485_Addr>= '1' && modbus_cmd_192->head.RS485_Addr<= '9')
										|| (modbus_cmd_192->head.RS485_Addr>= 'A' && modbus_cmd_192->head.RS485_Addr<= 'Z'))

									&& (modbus_cmd_192->head.para_len == sizeof(modbus_cmd_192->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_cmd_192,sizeof(MODBUS_RS485_CMD_192));
									}

									ReadArray.RemoveAll();
									
								}
						}

						if((G_bCMD_192_READ_STATUS == true) &&  (ReadArray.GetSize() >= sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_192)))
						{
							MODBUS_RS485_STATUS_FLASH_ANSWER_192    *modbus_status_192 =(MODBUS_RS485_STATUS_FLASH_ANSWER_192*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_192)]);
							
							if((modbus_status_192->crc == CComm::CRC16((unsigned char*)modbus_status_192,sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_192) - 2))
									&& (modbus_status_192->head.RS485_Addr>= 'A' && modbus_status_192->head.RS485_Addr<= 'Z')
									&& (modbus_status_192->head.para_len == sizeof(modbus_status_192->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_status_192,sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_192));
									}

									ReadArray.RemoveAll();
									
								}
						}

					

					}
			}
			else if((l==1) && (G_IsJustCmdAscii == true))
			{
				if(rec == CMD_STX)
				{
					ReadArray.RemoveAll(); 
					ReadArray.Add( CMD_STX );
				}
				else if( rec == CMD_ETX)
				{
					ReadArray.Add( CMD_ETX );
					
					
					if(para->m_hWnd != NULL)
						::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)ReadArray.GetData(),ReadArray.GetSize());

					ReadArray.RemoveAll(); 
				}
				else
				{
					ReadArray.Add( rec );
				}
			}
			else
			{
			}
		}
	WriteLogFile("EXIT READ THREAD",0,true);
	return 0;
}

CComm::CComm(void)
{
	m_hComm	= INVALID_HANDLE_VALUE;
	bXModem	=	false;
#ifdef _HeatBox
	for(int i = 0; i < ::G_CurMaxHeatBox; i++)
	{
		m_BeepStartTime[i] = CTime::GetCurrentTime();
	}
#endif

	m_OpenCount = 0;
}
CComm::~CComm(void)
{

}

int CComm::SetBaudrate(HANDLE hComm,DWORD BaudRate,BYTE ByteSize,BYTE StopBits ,BYTE Parity)
{
	DWORD err;
	

	if(hComm == INVALID_HANDLE_VALUE)
	{
		
		return -1;
	}
	if(::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR) == false)
	{

		err = GetLastError();
		if(err == ERROR_ACCESS_DENIED)
		{
			this->CloseComm(); 
			return -1;
		}
	}


	COMMTIMEOUTS timeout;
	::GetCommTimeouts(hComm,&timeout);
	
	timeout.ReadIntervalTimeout	= 10; // 读间隔超时。 接收时，两字符间最大的时延。
	timeout.ReadTotalTimeoutMultiplier = 10; // 读时间系数。 读取每字节的超时。
	timeout.ReadTotalTimeoutConstant = 10000; // 读时间常量。 读串口数据的固定超时。
    

	::SetCommTimeouts(hComm,&timeout);

	DCB m_dcb;
	
	if(!GetCommState(hComm,&m_dcb))
	{
		return -3;
	}
	else
	{
		//memset(&m_dcb,0,sizeof(m_dcb));
		//m_dcb.DCBlength =sizeof(m_dcb); 
		m_dcb.BaudRate	=	BaudRate; 
		m_dcb.ByteSize	=	ByteSize;
		m_dcb.StopBits	=	StopBits;   //BYTE StopBits;        // 0,1,2 = 1, 1.5, 2            
		m_dcb.fBinary	=	true ; 
		if(Parity == 0)
		{
			m_dcb.fParity	=	false;
		}
		else
		{
			m_dcb.fParity	=	true;
		}
		m_dcb.Parity    =	Parity;
	}

	

    if(!SetCommState(hComm,&m_dcb))
	{
		AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
		return -4;
	}

	if(!SetupComm(hComm,1024*30,1024))
	{
		AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
			return -2;
	}
	
	
	return 0;
}

int CComm::OpenComm_Delta_PLC(CString commName,COMM_THREAD_PARA *para)
{
	TCHAR sPort[20];
	DWORD err;
	//wsprintf(sPort,_T("\\\\.\\COM%d"),comm);

#ifdef WINCE
	wsprintf(sPort,_T("%s"),commName);
#else
	wsprintf(sPort,_T("\\\\.\\%s"),commName);
#endif


	
	m_hComm = CreateFile(sPort, 
			GENERIC_READ | GENERIC_WRITE, 
			0,//FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,                    // exclusive access
			NULL,                 // no security attrs
			OPEN_EXISTING,            
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // overlapped I/O   
			NULL );

	err = GetLastError();

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		if(m_OpenCount == 0)
		{
			CString s;
			if(G_iLangID == 0)
			{
				s.Format(_T("串口 %s 异常！"),commName);
			}
			else
			{
				s.Format(_T("COMM Port %s is used or not available!"),commName);
			}

			

			AfxMessageBox(s,NULL,MB_ICONERROR);
		}
		m_OpenCount ++;
		return -1;
	}
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);


	COMMTIMEOUTS timeout;
	::GetCommTimeouts(m_hComm,&timeout);
	
	timeout.ReadIntervalTimeout	= 10; // 读间隔超时。 接收时，两字符间最大的时延。
	timeout.ReadTotalTimeoutMultiplier = 10; // 读时间系数。 读取每字节的超时。
	timeout.ReadTotalTimeoutConstant = 10000; // 读时间常量。 读串口数据的固定超时。
    

	::SetCommTimeouts(m_hComm,&timeout);

	DCB m_dcb;
	
	if(!GetCommState(m_hComm,&m_dcb))
			{
				AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -3;
			}
			else //if(::G_NormalConfigPara[0].BoxType == BOX_DELTA_PLC_8PT100_8SSR)
			{
				m_dcb.BaudRate	=	9600; 
				m_dcb.ByteSize	=	7;
				m_dcb.StopBits	=	0; 
				m_dcb.fBinary	=	false ; 
				m_dcb.fParity	=	true;
				m_dcb.Parity    =	EVENPARITY;
			}
			

    if(!SetCommState(m_hComm,&m_dcb))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
			return -4;
		}
	if(!SetupComm(m_hComm,1024*30,1024))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -2;
		}
	
	//::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

	ov.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	bReadComm=true;

	if(this->bXModem == false)
	{
		para->ReadThread = AfxBeginThread(ReadCommThreadProc_Delta_PLC,(LPVOID)(para),THREAD_PRIORITY_NORMAL);
	}

	return 0;
}

int CComm::OpenComm(CString commName,COMM_THREAD_PARA *para)
{
	TCHAR sPort[20];
	DWORD err;
	//wsprintf(sPort,_T("\\\\.\\COM%d"),comm);

#ifdef WINCE
	wsprintf(sPort,_T("%s"),commName);
#else
	wsprintf(sPort,_T("\\\\.\\%s"),commName);
#endif


	
	m_hComm = CreateFile(sPort, 
			GENERIC_READ | GENERIC_WRITE, 
			0,//FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,                    // exclusive access
			NULL,                 // no security attrs
			OPEN_EXISTING,            
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // overlapped I/O   
			NULL );

	err = GetLastError();

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		if(m_OpenCount == 0)
		{
			CString s;
			if(G_iLangID == 0)
			{
				s.Format(_T("串口 %s 异常！"),commName);
			}
			else
			{
				s.Format(_T("COMM Port %s is used or not available!"),commName);
			}

			

			AfxMessageBox(s,NULL,MB_ICONERROR);
		}
		m_OpenCount ++;
		return -1;
	}
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);


	COMMTIMEOUTS timeout;
	::GetCommTimeouts(m_hComm,&timeout);
	
	timeout.ReadIntervalTimeout	= 10; // 读间隔超时。 接收时，两字符间最大的时延。
	timeout.ReadTotalTimeoutMultiplier = 10; // 读时间系数。 读取每字节的超时。
	timeout.ReadTotalTimeoutConstant = 10000; // 读时间常量。 读串口数据的固定超时。
    

	::SetCommTimeouts(m_hComm,&timeout);

	DCB m_dcb;
	
	if(!GetCommState(m_hComm,&m_dcb))
			{
				AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -3;
			}
			else
			{
				//memset(&m_dcb,0,sizeof(m_dcb));
				//m_dcb.DCBlength =sizeof(m_dcb); 
				m_dcb.BaudRate	=	9600; 
				m_dcb.ByteSize	=	8;
				m_dcb.StopBits	=	0; 
				m_dcb.fBinary	=	true ; 
				m_dcb.fParity	=	true;//false;
				m_dcb.Parity    =	NOPARITY;
			}

    if(!SetCommState(m_hComm,&m_dcb))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
			return -4;
		}
	if(!SetupComm(m_hComm,1024*30,1024))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -2;
		}
	
	//::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

	ov.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	bReadComm=true;
	
	if(this->bXModem == false)
	{
		para->ReadThread = AfxBeginThread(ReadCommThreadProc,(LPVOID)(para),THREAD_PRIORITY_NORMAL);
	}
	
	return 0;
}
int CComm::OpenComm(CString commName,COMM_THREAD_PARA *para,DCB dcb)
{
	TCHAR sPort[20];
	DWORD err;
	//wsprintf(sPort,_T("\\\\.\\COM%d"),comm);

#ifdef WINCE
	wsprintf(sPort,_T("%s"),commName);
#else
	wsprintf(sPort,_T("\\\\.\\%s"),commName);
#endif


	
	m_hComm = CreateFile(sPort, 
			GENERIC_READ | GENERIC_WRITE, 
			0,//FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,                    // exclusive access
			NULL,                 // no security attrs
			OPEN_EXISTING,            
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // overlapped I/O   
			NULL );

	err = GetLastError();

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		if(m_OpenCount == 0)
		{
			CString s;
			if(G_iLangID == 0)
			{
				s.Format(_T("串口 %s 异常！"),commName);
			}
			else
			{
				s.Format(_T("COMM Port %s is used or not available!"),commName);
			}

			

			AfxMessageBox(s,NULL,MB_ICONERROR);
		}
		m_OpenCount ++;
		return -1;
	}
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);


	COMMTIMEOUTS timeout;
	::GetCommTimeouts(m_hComm,&timeout);
	
	timeout.ReadIntervalTimeout	= 10; // 读间隔超时。 接收时，两字符间最大的时延。
	timeout.ReadTotalTimeoutMultiplier = 10; // 读时间系数。 读取每字节的超时。
	timeout.ReadTotalTimeoutConstant = 10000; // 读时间常量。 读串口数据的固定超时。
    

	::SetCommTimeouts(m_hComm,&timeout);

	DCB m_dcb;
	
		if(!GetCommState(m_hComm,&m_dcb))
			{
				AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -3;
			}
			else
			{
				//memset(&m_dcb,0,sizeof(m_dcb));
				//m_dcb.DCBlength =sizeof(m_dcb); 
				
				m_dcb.BaudRate	=	dcb.BaudRate;	//9600
				m_dcb.ByteSize	=	dcb.ByteSize;	//8
				m_dcb.StopBits	=	dcb.StopBits;	//0; 
				m_dcb.fBinary	=	dcb.fBinary;	//true ; 
				m_dcb.fParity	=	dcb.fParity;	//false;
				m_dcb.Parity    =	dcb.Parity;		//NOPARITY;
			}

		if(!SetCommState(m_hComm,&m_dcb))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
			return -4;
		}
		if(!SetupComm(m_hComm,1024*30,1024))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -2;
		}
	
	//::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

	ov.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	bReadComm=true;
	
	if(this->bXModem == false)
	{
		para->ReadThread = AfxBeginThread(ReadCommThreadProc,(LPVOID)(para),THREAD_PRIORITY_NORMAL);
	}
	
	return 0;
}

bool CComm::IsOpened(void)
{
	if(m_hComm == INVALID_HANDLE_VALUE)
		return false;
	else
		return true;
}
int CComm::CloseComm(void)
{
	bReadComm = 0;
	
	Sleep(1000);
	::CloseHandle(m_hComm); 
	m_hComm = INVALID_HANDLE_VALUE;
	

	return 0;
}

CString WriteCommMsg;
#ifdef WINCE
int CComm::WriteComm(char *buf,int len)
{
	ASSERT(buf);
	ASSERT(len);
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
    DWORD wr_len = 0;
	
	if(!IsOpened())
	{
		return -999;
	}

	//DWORD num;
	//BOOL b=ReadFile(hComm,buf,1,NULL,&ov);
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	BOOL b = WriteFile(m_hComm,buf,len,(LPDWORD)&wr_len,NULL);
	return wr_len;
}
#else
int CComm::WriteComm(char *buf,int len)
{
	
	
    OVERLAPPED ol;	
	DWORD wr_len = 0;
	DWORD sum_num = 0;
	int   writeTimer = 0;
	DWORD err;

	if(!IsOpened())
	{
		return -999;
	}

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	//return 0;

	ASSERT(buf);
	ASSERT(len);

	memset(&ol,0,sizeof(ol));
	ol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	

	//DWORD num;
	//BOOL b=ReadFile(hComm,buf,1,NULL,&ov);
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	BOOL b = WriteFile(m_hComm,buf,len,(LPDWORD)&wr_len,&ol);
	int ret	= 0;
	if(b)
	{
			sum_num=wr_len;
			while((int)sum_num<len)
			{
				writeTimer ++;
				BOOL	bResult = GetOverlappedResult(m_hComm, &ol, 
					&wr_len, true); 
				sum_num+=wr_len;
				if(writeTimer > 10000)
				{
					WriteLogFile("WriteComm writeTimer 1 break",0,false);				
					break;
				}
			}
			if(writeTimer > 1000)
			{
				ret = -8;
			}
			else
			{
				ret = -1;
			}
	}
	else
	{
		err=GetLastError();
		if(err == ERROR_IO_PENDING)
		{
			sum_num=wr_len;
			while((int)sum_num<len)
			{
				writeTimer ++;
				Sleep(1);
				BOOL	bResult = GetOverlappedResult(m_hComm, &ol, 
					&wr_len, true); 
				sum_num+=wr_len;
				if(writeTimer > 10000)
				{
					WriteLogFile("WriteComm writeTimer 2 break",0,false);				
					break;
				}
			}
			if(writeTimer > 1000)
			{
				ret = -9;
			}
			else
			{
				ret =-2;
			}
		}
		else
		{
			if(err == ERROR_ACCESS_DENIED)
			{
				CloseComm();
			}
			
//			CloseHandle(   ol.hEvent   ); 
			ret = -3;
		}
//		CloseHandle(   ol.hEvent   ); 
//		ret = -4;
	}
	CloseHandle(   ol.hEvent   ); 
	if( ret != -2)
	{
		CTime cur;
		cur = cur.GetCurrentTime();
		char buf[100];
		sprintf_s(buf,"WriteComm %d %d %d err=%d %d:%d", m_hComm, ret, sum_num, err, cur.GetMinute(),cur.GetSecond());
		WriteLogFile(buf,0,true);
	}
	return ret;
}
#endif

#ifdef WINCE
int CComm::ReadComm(char *buf,int len)
{
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	if(m_hComm == INVALID_HANDLE_VALUE)
		return -9;

	DWORD num;
	BOOL b=ReadFile(m_hComm,buf,len,&num,NULL);
	if(b)
	{
		return num;
	}
	else
	{
		
		return 0;
	}
}
#else
int CComm::ReadComm(char *buf,int len)
{
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	if(m_hComm == INVALID_HANDLE_VALUE)
		return -9;

	OVERLAPPED ol;
	static char rec[10];
	memset(&ol,0,sizeof(ol));
	ol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	DWORD num;
	//BOOL b=ReadFile(hComm,buf,1,NULL,&ov);
	BOOL b=ReadFile(m_hComm,rec,len,&num,&ol);
	if(b)
	{
		if(num<=0)
			return -1;
		else
		{
			memcpy(buf,rec,num);
			CloseHandle(   ol.hEvent   ); 
			return num;
		}
	}
	else
	{
		DWORD err=GetLastError();
		if(err==ERROR_IO_PENDING)
		{
			BOOL	bResult = GetOverlappedResult(m_hComm, &ol, 
					&num, true);
			if (bResult && num>=1)  
			{
				memcpy(buf,rec,num);
				CloseHandle(   ol.hEvent   ); 
				return num;
			}
		//	else
		//	{
		//		memcpy(buf,rec,num);
		//		//::CloseHandle(   ol.hEvent   ); 
		//		return num;
		//		//::CloseHandle(   ol.hEvent   ); 
		//		//return -2;
		//	}
		}
		else
		{
			CloseHandle(   ol.hEvent   ); 
			return -3;
		}
		
		CloseHandle(   ol.hEvent   ); 
		return -4;
	}
}
#endif

//CRC低位字节值表
unsigned char crc16_lo[]=
 {          0x0, 0xC0, 0xC1, 0x1, 0xC3, 0x3, 0x2, 0xC2, 0xC6, 0x6, 
            0x7, 0xC7, 0x5, 0xC5, 0xC4, 0x4, 0xCC, 0xC, 0xD, 0xCD, 
            0xF, 0xCF, 0xCE, 0xE, 0xA, 0xCA, 0xCB, 0xB, 0xC9, 0x9, 
            0x8, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
            0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
            0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
            0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
            0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
            0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
            0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
            0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
            0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
            0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
            0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
            0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
            0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
            0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
            0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
            0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
            0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
            0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
            0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
            0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
            0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
            0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
            0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

//'CRC高位字节值表
unsigned char crc16_hi[]=
{
			0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0,
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41,
            0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 
            0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 
            0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 
            0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41,
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40,
            0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 
            0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40,
            0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 
            0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40,
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41,
            0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0,  
			0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 
            0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0,  
			0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 
            0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 
            0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41,
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40
};
bool CComm::CheckCRC16(unsigned char *source, int  n, unsigned short source_crc)
{
	unsigned short crc;
	crc = CRC16(source,n);//,crc);
	if(crc == source_crc)
	//	&&(crc[1] == source_crc[1]))
	{
		return true;
	}
	return false;
}

unsigned int CRC16_Ex ( unsigned char *arr_buff, unsigned int len)
{

 unsigned short crc=0xFFFF;
 unsigned int i, j;
 for ( j=0; j < len; j++)
 {

   crc=crc ^*arr_buff++;
   for ( i=0; i<8; i++)
  {

       if( ( crc&0x0001) >0)
       {

           crc=crc>>1;

           crc=crc^ 0xa001;

        }
      else

          crc=crc>>1;
   }
 }
return ( crc);

}

unsigned short CComm::CRC16_MODBUS(unsigned char *source, int  n ) //, unsigned char ReturnData[2])
{
    unsigned char CRC16Hi = 0xFF;
    unsigned char CRC16Lo = 0xFF;
    int i;
    int iIndex;

    for( i = 0; i < n; i++)
	{
        iIndex = CRC16Lo ^ source[i];
        CRC16Lo = CRC16Hi ^ crc16_hi[iIndex]    ;//         '低位处理
        CRC16Hi = crc16_lo[iIndex];				//                         '高位处理
    }

	//ReturnData[1] = CRC16Hi;//                                'CRC高位
    //ReturnData[0] = CRC16Lo;//      
	
	unsigned short crc = (CRC16Hi << 8) + CRC16Lo;

	return crc;
}

unsigned short CComm::CRC16(unsigned char *source, int  n) //, unsigned char ReturnData[2])
{

#ifdef _HeatBox
	if(::G_iMaxLedNr[0] == 300)
	{
		return CRC16_MODBUS(source, n);
	}
#endif

#ifndef _OIL_    
	//MODBUS 开始是0xFFFF
    unsigned char CRC16Hi = CRC_START_VAL_1;
    unsigned char CRC16Lo = CRC_START_VAL_0;
    int i;
    int iIndex;

    for( i = 0; i < n; i++)
	{
        iIndex = CRC16Lo ^ source[i];
        CRC16Lo = CRC16Hi ^ crc16_hi[iIndex]    ;//         '低位处理
        CRC16Hi = crc16_lo[iIndex];				//                         '高位处理
    }

	//ReturnData[1] = CRC16Hi;//                                'CRC高位
    //ReturnData[0] = CRC16Lo;//                                'CRC低位
	unsigned short crc = (CRC16Hi << 8) + CRC16Lo;
	return crc;
	//unsigned short ret =CRC16_Ex(source,n);
#else
    int i;
    short iSum = 0;

    for( i = 0; i < n; i++)
	{
        iSum += source[i];
    }

	ReturnData[1] = ((iSum & 0xFF00) >> 8);
    ReturnData[0] = ((iSum & 0x00FF) );
#endif

}


unsigned short CComm::XMODEM_CRC(char *ptr, int count)
{
    unsigned short crc = 0;
    char i;

    while (--count >= 0)
    {
        crc = crc ^ (unsigned short) (*ptr++ << 8);
        i = 8;
        do
        {
            if (crc & 0x8000)
            {
                crc = crc << 1 ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
        while(--i);
    }
    return (crc);
}

unsigned short CComm::CalCheckSum_OIL(unsigned char *buf,unsigned char len)
{
	//unsigned char *buf = (unsigned char *)cmd;
	unsigned short chksum = 0;
	unsigned char i;
	for(i = 0; i < len; i++)
	{
		chksum+=buf[i];
	}
	
	return chksum;
}

MODBUS_RS485_CMD_192 *  CComm::SendReadTemp_192(unsigned char *switch_status,int BoxNr)
{
	
		static MODBUS_RS485_CMD_192 cmd;

		cmd.head.RS485_Addr = '1' + BoxNr;
		cmd.head.cmd  = CMD_READ_TEMP;
		cmd.head.para_len = sizeof(cmd.para); 
		memcpy(cmd.para,switch_status,sizeof(cmd.para));
		

		cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
		
		this->WriteComm((char*)&cmd,sizeof(cmd)); 
		return &cmd;
}

MODBUS_RS485_CMD_300 *  CComm::SendReadTemp_300(int BoxNr)
{
	
		static MODBUS_RS485_CMD_300 cmd;

		cmd.head.RS485_Addr = '1' + BoxNr;
		cmd.head.cmd  = CMD_READ_TEMP;
		cmd.head.para_len = sizeof(cmd.para); 
		
		cmd.crc = CComm::CRC16_MODBUS((unsigned char *)&cmd,sizeof(cmd)-2);
		
		this->WriteComm((char*)&cmd,sizeof(cmd)); 
		return &cmd;
}

MODBUS_RS485_CMD_128 *  CComm::SendReadTemp(int BoxNr,char *para)
{
		
	static MODBUS_RS485_CMD_128 cmd;

	memset(&cmd,0,sizeof(cmd));

	cmd.head.RS485_Addr = '1' + BoxNr;
	cmd.head.cmd  = CMD_READ_TEMP;
	cmd.head.para_len = sizeof(cmd.para); 
	memcpy(cmd.para,para,sizeof(cmd.para));
	

	cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
	
	this->WriteComm((char*)&cmd,sizeof(cmd)); 
	return &cmd;
			
}



void  CComm::SendReadTempToCmdAray(int BoxNr)
{
		
	MODBUS_RS485_CMD_128 cmd;

	memset(&cmd,0,sizeof(cmd));

	cmd.head.RS485_Addr = '1' + BoxNr;
	cmd.head.cmd  = CMD_READ_TEMP;
	cmd.head.para_len = sizeof(cmd.para); 
	

	cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
	
	AddCommCmd('1'+ BoxNr,CMD_READ_TEMP, cmd.para,BoxNr);
			
}
MODBUS_RS485_CMD_128 *  CComm::SendReadTemp(int BoxNr)
{
	unsigned char para[100];
	
	memset(para,0,sizeof(para));
	
	
	static MODBUS_RS485_CMD_128 cmd;

	cmd.head.RS485_Addr = '1' + BoxNr;
	cmd.head.cmd  = CMD_READ_TEMP;
	cmd.head.para_len = sizeof(cmd.para); 
	memcpy(cmd.para,para,sizeof(cmd.para));
	cmd.para[0] = '1';
	cmd.para[1] = '3';
	cmd.para[2] = '9';
	cmd.para[3] = '1';
	cmd.para[4] = '5';
	cmd.para[5] = '8';
	cmd.para[6] = '3';
	cmd.para[7] = '8';
	cmd.para[8] = '5';
	cmd.para[9] = '9';
	cmd.para[10] = '8';

	cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
	
	this->WriteComm((char*)&cmd,sizeof(cmd)); 
	return &cmd;
			
}

MODBUS_RS485_CMD_32PT100 *  CComm::SendReadTemp_32PT100(int BoxNr)
{
	unsigned char para[100];
	
	memset(para,0,sizeof(para));
	
	
	static MODBUS_RS485_CMD_32PT100 cmd;

	cmd.head.RS485_Addr = '1' + BoxNr;
	cmd.head.cmd  = CMD_READ_TEMP;
	cmd.head.para_len = sizeof(cmd.para); 
	memcpy(cmd.para,para,sizeof(cmd.para));
	cmd.para[0] = '1';
	cmd.para[1] = '3';
	cmd.para[2] = '9';
	cmd.para[3] = '1';
	cmd.para[4] = '5';
	cmd.para[5] = '8';
	cmd.para[6] = '3';
	cmd.para[7] = '8';
	cmd.para[8] = '5';
	cmd.para[9] = '9';
	cmd.para[10] = '8';

	cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
	
	this->WriteComm((char*)&cmd,sizeof(cmd)); 
	return &cmd;
			
}

bool CComm::IsOmronMeter(BOX_TYPE BoxType)
{
	if(BoxType == BOX_DISC_FLAT
		|| BoxType == BOX_32_OMRON  
		|| BoxType == BOX_45_OMRON  
		|| BoxType == BOX_64_OMRON
		|| BoxType == BOX_128_OMRON)
	{
			return true;
	}

	return false;
}

void CComm::SetCmdString_ASCII(ASCII_RS485_COMMAND *cmd,int equip, int cmdId, short para[4])
{
#ifndef WINCE
		cmd->header.start		=	CMD_STX;
		cmd->end				=	CMD_ETX;
		cmd->header.BoxNr	=	'0'+9;
		cmd->header.des_id		=	equip;
		cmd->header.command		=	cmdId;
		
		
		char p[16];
		
		{
			sprintf_s(p,		"%04d", para[0]);
			memcpy(cmd->val+0,p,4); 
			sprintf_s(p,		"%04d", para[1]);
			memcpy(cmd->val+4,p,4); 
			sprintf_s(p,		"%04d", para[2]);
			memcpy(cmd->val+8,p,4); 
			sprintf_s(p,		"%04d",	para[3]);
			memcpy(cmd->val+12,p,4); 
		}
		
		
		unsigned char *pCmd=(unsigned char*)cmd;
		unsigned char chksum=0;

		chksum = CalCheckSum((char*)&pCmd[1], sizeof(ASCII_RS485_COMMAND)-4);

		cmd->chk[0]= (chksum & 0x0F)    + '0';
		cmd->chk[1]=((chksum & 0xF0) >> 4)+ '0';
#endif
}
void CComm::SendPcbBoardType(int BoxNr)
{
		unsigned char para[100];
		memset(para,0,sizeof(para));
#ifdef _HeatBox		
		para[0]	=	G_NormalConfigPara[BoxNr].InterfaceBoardType;
#endif

		AddCommCmd ( '1'+ BoxNr,CMD_WRITE_BOARD_TYPE, para,BoxNr);
}

void CComm::SendTM7705_OSC(int BoxNr)
{
#ifdef _HeatBox
		unsigned char para[100];
		memset(para,0,sizeof(para));
		
		para[0]	=	G_NormalConfigPara[BoxNr].TM7705_NoOSC;


		AddCommCmd ( '1'+ BoxNr,CMD_WRITE_TM7705_OSC, para,BoxNr);
#endif
}


void CComm::AddCommCmd( int equip, COMMAND_ID  cmdId, unsigned char* para,int BoxNr)
{
		int i;

		if(G_NormalConfigPara[BoxNr].BoxType == BOX_SENSATA_32PT100)
		{

				MODBUS_RS485_CMD_32PT100 cmd;


				if(CMD_SET_ADDR == cmdId)
				{
					cmd.head.RS485_Addr = 0;
				}
				else
				{
					cmd.head.RS485_Addr = equip;
					cmd.head.cmd		= cmdId;
					cmd.head.para_len	= sizeof(cmd.para); 
					for(i = 0; i < sizeof(cmd.para); i++)
					{
						cmd.para[i] = para[i];
					}
					
					
					cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
					
					m_CmdArray_32PT100[BoxNr].Add(cmd);
				}



			return;
		}

#ifdef _HeatBox
		switch(G_NormalConfigPara[0].BoxType)
		{
	

		case BOX_192:
			{
				MODBUS_RS485_CMD_192 cmd;

				cmd.head.RS485_Addr = equip;
				cmd.head.cmd  = cmdId;
				cmd.head.para_len = sizeof(cmd.para); 
				for(i = 0; i < sizeof(cmd.para); i++)
				{
					cmd.para[i] = para[i];
				}
				
				
				cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
				m_CmdArray_192[BoxNr].Add(cmd);
			}
			break;
		case 0:
		default:
			{
				MODBUS_RS485_CMD_128 cmd;
				if(CMD_SET_ADDR == cmdId)
				{
					cmd.head.RS485_Addr = 0;
				}
				else
				{
				}
					cmd.head.RS485_Addr = equip;
				cmd.head.cmd  = cmdId;
				cmd.head.para_len = sizeof(cmd.para); 
				for(i = 0; i < sizeof(cmd.para); i++)
				{
					cmd.para[i] = para[i];
				}
				
				
				cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
				if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType))
				{
					m_CmdArray_128[BoxNr].Add(cmd); 
					m_CmdArray_128[BoxNr].Add(cmd); 
					m_CmdArray_128[BoxNr].Add(cmd); 
				}
				else
				{
					m_CmdArray_128[BoxNr].Add(cmd);
				}
			}
		break;
		}
#else

				MODBUS_RS485_CMD_128 cmd;
				if(CMD_SET_ADDR == cmdId)
				{
					cmd.head.RS485_Addr = 0;
				}
				else
				{
					cmd.head.RS485_Addr = equip;
				}
				cmd.head.cmd  = cmdId;
				cmd.head.para_len = sizeof(cmd.para); 
				for(i = 0; i < sizeof(cmd.para); i++)
				{
					cmd.para[i] = para[i];
				}
				
				
				cmd.crc = CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2);
				
				m_CmdArray_128[BoxNr].Add(cmd);
				
#endif
}


void CComm::SetDeltaFrequenceRun(bool run)
{
	DELTA_FREQUNCE_CMD_HEX cmd_hex;

	cmd_hex.RS485Addr	= 01;
	cmd_hex.cmd			= 0x06;
	cmd_hex.para_addr	= 0x2000;	
	if(run)
	{
		cmd_hex.para = 0x0002;
	}
	else
	{
		cmd_hex.para = 0x0001; 
	}
	
	SetCmdString_DeltaFrequence(1,cmd_hex);
}

void CComm::SetCmdString_DeltaFrequence(int equip, DELTA_FREQUNCE_CMD_HEX cmd_hex)
{

		DELTA_FREQUNCE_CMD_ASC cmd_asc;	
		cmd_asc.Head = ':';
		sprintf((char*)cmd_asc.RS485Addr,"%02X",cmd_hex.RS485Addr);
		sprintf((char*)cmd_asc.cmd,"%02X",cmd_hex.cmd);
		sprintf((char*)cmd_asc.para_addr,"%04X",cmd_hex.para_addr);
		sprintf((char*)cmd_asc.para,"%04X",cmd_hex.para);
		unsigned char check = 0;
		for(int i = 0; i < sizeof(cmd_hex) ; i++)
		{
			check += ((unsigned char*)&cmd_hex)[i];
		}
		check = 0x100 - check;
		sprintf((char*)cmd_asc.check,"%02X",check);
		cmd_asc.end[0] = 0x0D;
		cmd_asc.end[1] = 0x0A; 
		m_SendCmdDeltaFrequnce.Add(cmd_asc); 
		

}

unsigned short PcShort2OmronShort(unsigned short v)
{
	SHORT_BUF pc;
	SHORT_BUF omron;

	pc.v = v;
	omron.buf[0] = pc.buf[1];
	omron.buf[1] = pc.buf[0];

	return  omron.v; 
}
unsigned short OmronShort2PcShort(unsigned short v)
{
	
	SHORT_BUF pc;
	SHORT_BUF omron;
	omron.v = v;
	
	pc.buf[0] = omron.buf[1];
	pc.buf[1] = omron.buf[0];

	return  pc.v; 

}

void CComm::SetPIDT_P_Omron(float v,int BoxNr)
{
		unsigned char para[100];

		memset(para,0,sizeof(para));

		*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_P);
		
		*((short * )&para[2]) = PcShort2OmronShort(v*10.0f);

		SetCmdString_OMRON(BoxNr + 1 ,CMD_OMRON_WRITE, para);
}

void CComm::SetPIDT_I_Omron(int v,int BoxNr)
{
		unsigned char para[100];

		memset(para,0,sizeof(para));

		*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_I);
		
		*((short * )&para[2]) = PcShort2OmronShort(v);

		SetCmdString_OMRON(BoxNr + 1 ,CMD_OMRON_WRITE, para);
}

void CComm::SetPIDT_D_Omron(int v,int BoxNr)
{
		unsigned char para[100];

		memset(para,0,sizeof(para));

		*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_D);
		
		*((short * )&para[2]) = PcShort2OmronShort(v);

		SetCmdString_OMRON(BoxNr + 1 ,CMD_OMRON_WRITE, para);
}

void CComm::ReadPIDT_P_Omron(int BoxNr)
{
	unsigned char para[100];
		
	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_P);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));

	SetCmdString_OMRON(BoxNr + 1 ,CMD_OMRON_READ, para);
}
void CComm::ReadPIDT_I_Omron(int BoxNr)
{
	unsigned char para[100];
		
	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_I);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));

	SetCmdString_OMRON(BoxNr + 1 ,CMD_OMRON_READ, para);
}
void CComm::ReadPIDT_D_Omron(int BoxNr)
{
	unsigned char para[100];
		
	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_D);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));

	SetCmdString_OMRON(BoxNr + 1 ,CMD_OMRON_READ, para);
}

void CComm::ReadPIDT_T_Omron(int BoxNr)
{
	unsigned char para[100];
		
	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_T);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));

	SetCmdString_OMRON(BoxNr + 1 ,CMD_OMRON_READ, para);
}


void CComm::ReadPidT(int BoxNr)
{
#ifdef _HeatBox		
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType))
	{
		ReadPIDT_P_Omron(BoxNr);
		ReadPIDT_I_Omron(BoxNr);
		ReadPIDT_D_Omron(BoxNr);
		ReadPIDT_T_Omron(BoxNr);
	}
	else
#endif
	{

		unsigned char para[100];
		memset(para,0,sizeof(para));

		AddCommCmd( '1' + BoxNr ,CMD_READ_PID_T, para,BoxNr);
	}
}

void CComm::ReadPidT(int BoxNr,char ID)
{


		unsigned char para[100];
		memset(para,0,sizeof(para));
		para[sizeof(PID)] = ID;
		AddCommCmd( '1' + BoxNr ,CMD_READ_PID_T, para,BoxNr);

}

void CComm::SendStepPidCoolCmd(float temp, short time,int BoxNr,bool UsedFan)
{
#ifdef _HeatBox	
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType ))
	{
		SendDirectHeatCmd_OMRON(0.0f,BoxNr); 
	}
	else
#endif
	{
		short t = (short)(temp * 10 + 0.05f);

		unsigned char para[100];

		memset(para,0,sizeof(para));
		
		memcpy(&para[0],&t,sizeof(short));
		
		memcpy(&para[2],&time,sizeof(short));

		para[4] = UsedFan;

		AddCommCmd('1'+BoxNr,CMD_STEP_PID_COOL, para,BoxNr);
	}


}

void CComm::SendPidCoolCmd(float temp,int BoxNr,bool UsedFan)
{
#ifdef _HeatBox
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType ))
	{
		SendDirectHeatCmd_OMRON(temp,BoxNr); 
	}
	else
#endif
	{
		short Temp;

		if(temp >= 0)
		{
			Temp = (short)(temp *10 + 0.05f);
		}
		else
		{
			Temp = (short)(temp *10 - 0.05f);
		}

		unsigned char para[16];

		memset(para,0,sizeof(para));
		
		memcpy(para,&Temp,sizeof(short));
		
		para[2] = UsedFan;
		
		AddCommCmd('1' + BoxNr,CMD_PID_COOL, para,BoxNr);
	}


}

void CComm::SendEPM1270Reset(bool reset,int BoxNr)
{

	CString cmdStr;	
	unsigned char para[100];

	memset(para,0,sizeof(para));

	if(reset)
	{
		para[0] =  '1';
	}
	else
	{
		para[0] =  '0';
	}

	AddCommCmd( 'A'+BoxNr,CMD_CPLD_RESET, para,BoxNr);

}

int CComm::SendStepPIDCmd(float TopTemp, int minutes,int BoxNr)
{
	CString cmdStr;	
#ifdef _HeatBox
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType ))
	{
		return 0;
	}
#endif
	unsigned char para[100];
	memset(para,0,sizeof(para));

	short temp = (short)(TopTemp *10.0f  + 0.05f);
	memcpy(para,&temp,sizeof(short));
	
	if(minutes < 1)
	{
		minutes = 1;
	}
	memcpy(&para[2],&minutes,sizeof(short));



	CComm::AddCommCmd('1'+BoxNr,CMD_STEP_PID_OUT, para,BoxNr);


	return 0;
}

void CComm::SendDirectHeatCmd(float temp,int BoxNr)
{
#ifdef _HeatBox	
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType ))
	{
		SendDirectHeatCmd_OMRON(temp,BoxNr);
	}
	else
#endif
	{
		CString cmdStr;	
		
		unsigned char para[100];
		memset(para,0,sizeof(para));

		short Temp = (short)(temp *10 + 0.05f);

		memcpy(para,&Temp,sizeof(short));

		AddCommCmd( '1'+BoxNr,CMD_FULL_OUT, para,BoxNr);

	}
	
}

void CComm::SendStopHeatCmd(float temp,int BoxNr)
{
#ifdef _HeatBox
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType ))
	{
		SendDirectHeatCmd_OMRON(0.0f,BoxNr);
	}
	else
#endif
	{
		unsigned char para[100];
		
		memset(para,0,sizeof(para));
		short Temp;

		if(temp >= 0)
		{
			Temp = (short)(temp *10 + 0.05f);
		}
		else
		{
			Temp = (short)(temp *10 - 0.05f);
		}

		memcpy(para,&Temp,sizeof(short));

		AddCommCmd( '1' + BoxNr,CMD_STOP_OUT, para,BoxNr);
	}

}

void CComm::SendStepUpPidCmd_Omron(float *m_LastStepUpPidTemp_Omron,float HeatSpeed,int BoxNr)
{
	if(HeatSpeed  <= 0)
	{
		return;
	}
#ifdef _HeatBox
	if(!CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType ))
	{
		return;
	}
#endif
	static DWORD tick = ::GetTickCount();

	
	if((::GetTickCount() - tick) > 6000)//每6秒发一次命令 
	{
		unsigned char para[100];

		tick = ::GetTickCount();

		memset(para,0,sizeof(para));

		*m_LastStepUpPidTemp_Omron += HeatSpeed/10.0f; 

		*((short * )&para[0]) = PcShort2OmronShort(OMRON_SP);

		//*((short * )&para[2]) = PcShort2OmronShort((short)(OMRON_Get_MeterTemp_By_Sv(*m_LastStepUpPidTemp_Omron,BoxNr) *10));
		*((short * )&para[2]) = PcShort2OmronShort((short)(*m_LastStepUpPidTemp_Omron*10.0f));
		
		SetCmdString_OMRON( BoxNr  + 1, CMD_OMRON_WRITE, para );
	}

}



void CComm::SendStepPIDDownCmd_Omron(float *m_LastStepPidDownTemp_Omron,float CoolSpeed,int BoxNr)
{
	static DWORD tick = ::GetTickCount();
	
	if((::GetTickCount() - tick) > 6000)//每6秒发一次命令 
	{
		unsigned char para[100];
		
		tick = ::GetTickCount();

		
		*m_LastStepPidDownTemp_Omron -= CoolSpeed; 
		
		memset(para,0,sizeof(para));

		*((short * )&para[0]) = PcShort2OmronShort(OMRON_SP);

		*((short * )&para[2]) = PcShort2OmronShort((short)(OMRON_Get_MeterTemp_By_Sv(*m_LastStepPidDownTemp_Omron,BoxNr) *10));
		
		SetCmdString_OMRON(BoxNr + 1, CMD_OMRON_WRITE, para);
	}
}

void CComm::SendStepUpPidCmd_Bruce(float *m_LastStepUpPidTemp_Bruce,int BoxNr)
{
#ifdef _HeatBox
	static DWORD tick = ::GetTickCount();
	
	if((::GetTickCount() - tick) > 6000)//每6秒发一次命令 
	{
		
		tick = ::GetTickCount();

		
		*m_LastStepUpPidTemp_Bruce += G_NormalConfigPara[BoxNr].HeatSpeed; 

		SendNormalPidCmd(*m_LastStepUpPidTemp_Bruce,BoxNr);
	}
#endif
}

void CComm::SendStepDownPidCmd_Bruce(float *m_LastStepDownPidTemp_Bruce,int BoxNr)
{
#ifdef _HeatBox
	static DWORD tick = ::GetTickCount();
	
	if((::GetTickCount() - tick) > 6000)//每6秒发一次命令 
	{
		tick = ::GetTickCount();
		
		*m_LastStepDownPidTemp_Bruce -= G_NormalConfigPara[BoxNr].CoolSpeed / 100.0f; 

		SendNormalPidCmd(*m_LastStepDownPidTemp_Bruce,BoxNr );
	}
#endif
}

void CComm::SendNormalPidCmd(float temp1,float temp2 ,int BoxNr)
{

	{
		CString cmdStr;	
		
		unsigned char para[16];
		memset(para,0,sizeof(para));

		short Temp =(short)( temp1 *10);
		memcpy(para,&Temp,sizeof(short));
		 Temp =(short)( temp2 *10);
		memcpy(&para[2],&Temp,sizeof(short));

		AddCommCmd('1'+ BoxNr,CMD_NORMAL_PID_OUT, para,BoxNr);
	}
	
}

void CComm::SendNormalPidCmd(float temp ,int BoxNr)
{
#ifdef _HeatBox	
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType ))
	{

//		SendDirectHeatCmd_OMRON(temp,G_NormalConfigPara[BoxNr].OMRON_0C_Adjust,G_NormalConfigPara[BoxNr].OMRON_200C_Adjust,BoxNr,G_NormalConfigPara[BoxNr].BoxType);
	}
	else
#endif
	{
		CString cmdStr;	
		
		unsigned char para[16];
		memset(para,0,sizeof(para));

		short Temp =(short)( temp *10);
		memcpy(para,&Temp,sizeof(short));

		AddCommCmd('1'+ BoxNr,CMD_NORMAL_PID_OUT, para,BoxNr);
	}
	
}
void CComm::SendNormalPidCmd_8PT100_8SSR(float temp[8] ,int BoxNr)
{

	{
		CString cmdStr;	
		
		unsigned char para[16];
		memset(para,0,sizeof(para));

		short Temp;
		for(int i = 0; i < 8; i++)
		{
			Temp =(short)( temp[i] *10);
			memcpy(&para[i * 2],&Temp,sizeof(short));
		}

		AddCommCmd('1'+ BoxNr,CMD_NORMAL_PID_OUT, para,BoxNr);
	}
	
}
	
void CComm::SendSetFan(char fan ,int BoxNr)
{
	unsigned char para[100];

	memset(para,0,sizeof(para));

	
	para[0] = fan; 
	


	AddCommCmd('1'+BoxNr,CMD_FAN, para,BoxNr);

}

void CComm::SendGetStm32ID(int BoxNr)
{
	unsigned char para[100];

	memset(para,0,sizeof(para));

	
	


	AddCommCmd('1'+BoxNr,CMD_STM32_UNIQUE_ID, para,BoxNr);

}

void CComm::SendBeepCmd(char  beep_flag ,int BoxNr)
{
	unsigned char para[100];

	memset(para,0,sizeof(para));

	if(beep_flag)
	{
		para[0] = 1; 
		m_BeepStartTime[BoxNr] = CTime::GetCurrentTime();
	}
	else
	{
		para[0] = 0;
	}


	AddCommCmd('1'+BoxNr,CMD_SET_BEEP, para,BoxNr);

	

	return;
}
int CComm::ReadPidCount(int BoxNr)
{
#ifdef _HeatBox
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType))
	{
		return 0;
	}
#endif

	unsigned char para[100];
	memset(para,0,sizeof(para));

	AddCommCmd('1'+BoxNr,CMD_READ_STEP_PID_COUNT, para,BoxNr);

	
	return 0;
}

int CComm::SetPidCount(int BoxNr)
{
#ifdef _HeatBox
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType))
	{
		return 0;
	}
#endif

	unsigned char para[100];
	memset(para,0,sizeof(para));

	AddCommCmd('1'+BoxNr,CMD_WRITE_STEP_PID_COUNT, para,BoxNr);

	
	return 0;
}




int CComm::Send_NTC_TH2512_Relay(char ID,int BoxNr,unsigned char pass[3])
{
#ifdef _HeatBox
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType))
	{
		return 0;
	}
#endif

	unsigned char para[100];
	memset(para,0,sizeof(para));

	para[0] = ID; 
	para[1] = pass[0]; 
	para[2] = pass[1]; 
	para[3] = pass[2]; 

	m_CmdArray_128[BoxNr].RemoveAll();

	AddCommCmd('A'+BoxNr,CMD_SET_NTC24_TH2512_RELAY, para,BoxNr);

	
	return 0;
}

int CComm::SendOpenDoorCmd(char opendoor_flag,int BoxNr)
{
#ifdef _HeatBox
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType))
	{
		return 0;
	}
#endif

	unsigned char para[100];
	memset(para,0,sizeof(para));
//	G_HeatBoxDlg.m_DoorIsOpenned = opendoor_flag;
	if(opendoor_flag)
	{//open door
		para[0] = opendoor_flag; 
	}
	else
	{
		para[0] = 0;
	}
	AddCommCmd('1'+BoxNr,CMD_SET_RELAY, para,BoxNr);

	
	return 0;
}

int CComm::SendSet192InterfacePcbType(char type,int BoxNr)
{
#ifdef _HeatBox
	if(CComm::IsOmronMeter(G_NormalConfigPara[BoxNr].BoxType))
	{
		return 0;
	}
#endif

	unsigned char para[100];
	memset(para,0,sizeof(para));
//	G_HeatBoxDlg.m_DoorIsOpenned = opendoor_flag;
	para[0] = type; 
	
	AddCommCmd('A'+BoxNr,CMD_SET_PCB_TYPE_192_INTERFACE, para,BoxNr);

	
	return 0;
}
//int							G_ProgramStarted = 0;
MODBUS_RS485_CMD_PROGRAM program_cmd;
MODBUS_RS485_ARM_CMD_PROGRAM program_arm_cmd;

char bin[1024*1024];
void CComm::XModem_Start(int BoxNr,int BoardType,int NoOSC)
{

#ifndef _DEBUG	
	/*
	static unsigned long psw;
	CInputDlg dlg_psw;

	CTime	day = CTime::GetCurrentTime();
	dlg_psw.m_Title.Format(_T("请输入密码 %04d%02d%02d%02d"),day.GetYear(),day.GetMonth(),day.GetDay(),day.GetHour());

	unsigned long y = (day.GetYear() * 3+ 5) % 100;
	unsigned long m = (day.GetMonth() * 5+ 5) % 100;
	unsigned long d = (day.GetDay() * 7+ 5) % 100;
	unsigned long h = (day.GetHour() * 9+ 5) % 100;
	if(psw !=  (y*1000000 + m *10000 + d*100 + h))
	{
		if(dlg_psw.DoModal() == IDOK)
		{
			unsigned long temp = _tcstoul(dlg_psw.m_Input,NULL,10);
			
			
			

			if(temp != (y*1000000 + m *10000 + d*100 + h))
			{
				AfxMessageBox(_T("密码错误"));
				return;
			}

			psw = (m *10000 + d*100 + h);
		}
		else
		{
			return;
		}
	}
	*/
#endif

	CListDlg dlg;

#ifdef _HeatBox
	
	dlg.m_ListItem.Add(_T("TPC_TEMPBOARD_MEGE16_R75.hex"));
	dlg.m_ListItem.Add(_T("TPC_TEMPBOARD_MEGE16_R82.hex"));
	dlg.m_ListItem.Add(_T("TPC_TEMPBOARD_MEGE32_R75.hex"));
	dlg.m_ListItem.Add(_T("TPC_TEMPBOARD_MEGE32_R82.hex"));
	dlg.m_ListItem.Add(_T("TPC_TEMPBOARD_MEGE16_R75_20MA.hex"));

#ifndef WINCE
	dlg.m_ListItem.Add(_T("LI_SIR_TEMP.hex"));
	dlg.m_ListItem.Add(_T("HEATBOX_TEMP_TM7705_LED_192_R75.hex"));
	dlg.m_ListItem.Add(_T("HEATBOX_TEMP_TM7705_LED_192_R82.hex"));
	dlg.m_ListItem.Add(_T("INTERFACE_192.hex"));
#endif

	dlg.m_ListItem.Add(_T("BIMETAL_V3_A.hex"));
	dlg.m_ListItem.Add(_T("BIMETAL_V3_B.hex"));
	dlg.m_ListItem.Add(_T("BIMETAL24_V3_A.hex"));
	dlg.m_ListItem.Add(_T("BIMETAL24_V3_B.hex"));
	dlg.m_ListItem.Add(_T("NTC_32.hex"));
	dlg.m_ListItem.Add(_T("NTC_64.hex"));

#ifndef WINCE
	dlg.m_ListItem.Add(_T("TPC_TEMPBOARD_MEGE16_R75_192点.hex"));
#endif
	dlg.m_ListItem.Add(_T("JIU_YANG_MEGE16_R75.hex"));
	dlg.m_ListItem.Add(_T("JIU_YANG_MEGE32_R75.hex"));
#else
	dlg.m_ListItem.Add(_T("20TempBoard_5SSR_Mega16_R75.hex"));
#endif

#ifndef _20PT100_5SSR
	dlg.m_ListItem.Add(_T("TPC_TempBoard_Mega16_R75_8PT100_8SSR.hex"));

#endif

#ifndef WINCE
	dlg.m_ListItem.Add(_T("TPC_TEMPBOARD_STM32_R75.hex"));

	dlg.m_ListItem.Add(_T("TPC_TEMPBOARD_MEGA16_R75_DISC.hex"));
#endif

//	AfxMessageBox(_T("IDR_TPC_TEMPBOARD_MEGA16_R75_GAIN_4"));

	dlg.m_ListItem.Add(_T("IDR_TPC_TEMPBOARD_MEGA16_R75_GAIN_4.hex"));
	dlg.m_ListItem.Add(_T("IDR_NTC_INTERFACE_24.hex"));
	dlg.m_ListItem.Add(_T("IDR_TPC_TempBoard_Mega16_R75_EXT_INPUT.hex"));
	dlg.m_ListItem.Add(_T("IDR_NTC_INTERFACE_24_BATH.hex"));
	dlg.m_ListItem.Add(_T("IDR_NTC_24_LED.hex"));
	dlg.m_ListItem.Add(_T("IDR_NTC_24_RELAY_LED.hex"));
	dlg.m_ListItem.Add(_T("输入接口板_32路.hex"));
	dlg.m_ListItem.Add(_T("输入接口板_64路.hex"));
	dlg.m_ListItem.Add(_T("TEMP2_2BTA_10IN_12OUT_BOARD.hex"));
	dlg.m_ListItem.Add(_T("TEMP2_1BTA_10IN_12OUT_BOARD.hex"));

	dlg.m_ListItem.Add(_T("TPC_DC_AMP_8.hex"));
	dlg.m_ListItem.Add(_T("TEMP1_1BTA_10IN_12OUT_BOARD.hex"));
	dlg.m_ListItem.Add(_T("TPC_TempBoard_Mega16_8PT100_1SSR_1BEEP_INTERFACE.hex"));
	dlg.m_ListItem.Add(_T("TPC_LLTB_2TEMP_24OUT.hex"));


	
	if(dlg.DoModal() != IDOK)
	{
		return;
	}
unsigned int ID[]=
{
#ifdef _HeatBox
	IDR_TPC_TEMPBOARD_MEGA16_R75,
	IDR_TPC_TEMPBOARD_MEGA16_R82,
	IDR_TPC_TEMPBOARD_MEGA32_R75,
	IDR_TPC_TEMPBOARD_MEGA32_R82,
	IDR_TPC_TEMPBOARD_MEGA16_R75_20MA,
#ifndef WINCE
	IDR_LI_SIR_TEMP,
	IDR_HEATBOX_TEMP_TM7705_LED_192_R75,
	IDR_HEATBOX_TEMP_TM7705_LED_192_R82,
	IDR_INTERFACE_192,
#endif
	IDR_BIMETAL_V3_A,
	IDR_BIMETAL_V3_B,
	IDR_BIMETAL24_V3_A,
	IDR_BIMETAL24_V3_B,
	IDR_NTC_32,
	IDR_NTC_64,
#ifndef WINCE
	IDR_TPC_TEMPBOARD_MEGA16_R75_192,
#endif
	IDR_JIU_YANG_MEGA16_R75,
	IDR_JIU_YANG_MEGA32_R75,
#else
	IDR_1PT100_1NTC_18K_5SSR_Mega16_R75_HEX,
#endif
#ifndef _20PT100_5SSR
#ifndef WINCE
	IDR_TPC_TempBoard_Mega16_8PT100_8SSR_HEX,
#endif
#endif

#ifndef WINCE
	IDR_TPC_TEMPBOARD_STM32_R75,
	IDR_TPC_TEMPBOARD_MEGA16_R75_DISC,
#endif

	IDR_TPC_TEMPBOARD_MEGA16_R75_GAIN_4,
#ifndef WINCE
	IDR_NTC_INTERFACE_24,
	IDR_TPC_TempBoard_Mega16_R75_EXT_INPUT,
	IDR_NTC_INTERFACE_24_BATH,
	IDR_NTC_24_LED,
	IDR_NTC_24_RELAY_LED,
	IDR_HEX_INTERFACE_32,
	IDR_HEX_INTERFACE_64,
	IDR_TEMP2_2BTA_10IN_12OUT_BOARD,
	IDR_TEMP2_1BTA_10IN_12OUT_BOARD,
	IDR_TPC_DC_AMP_8,
	IDR_TEMP1_1BTA_10IN_12OUT_BOARD,
	IDR_TPC_TempBoard_Mega16_8PT100_1SSR_1BEEP_INTERFACE,
	IDR_LLTB_2TEMP_24OUT,
#endif
};
	
	memset(bin,0xFF,sizeof(bin));

	
	dlg.m_SelectString.Trim(); 
	if(dlg.m_SelectString.GetLength() <= 0)
	{
		return;
	}

	if(dlg.m_iSelectItem < 0)
	{
		return;
	}

	memset(HexBuf,0,sizeof(HexBuf));
	if(::FreeMyData(MAKEINTRESOURCE(ID[dlg.m_iSelectItem]),_T("HEX"),0,HexBuf,&HexBufLen) == false)
	{
		CString s;
		s.Format(_T("Error ID[dlg.m_iSelectItem] = %d,%d"),ID[dlg.m_iSelectItem],dlg.m_iSelectItem);
		AfxMessageBox(s);
		return;
	}

	memset(bin,0xFF,sizeof(bin));
	int datalen;
	int page;
#ifndef WINCE
	if(ID[dlg.m_iSelectItem] != IDR_TPC_TEMPBOARD_STM32_R75)
#endif
	{
		
		datalen = Hex2Bin(HexBuf,HexBufLen,bin,sizeof(bin));
		CString s;
		s.Format(_T("ID[dlg.m_iSelectItem]=%d,HexBufLen = %d,datalen = %d"),ID[dlg.m_iSelectItem],HexBufLen,datalen);
		AfxMessageBox(s);

		page = (datalen / 128);
		if((datalen % 128) > 0)
		{
			page ++;
		}
		else
		{
			AfxMessageBox(_T("空文件!!!!!!!!!!!"));
			return;
		}
		if(page > 15*8)
		{
			AfxMessageBox(_T("文件太大,超出单片机容量!!!!!!!!!!!"));
			return;
		}
		unsigned short crc = CRC16((unsigned char*)bin,datalen);
	
		s.Format(_T("datalen= %d, crc = %d"),datalen,crc);
		AfxMessageBox(s);

		
		
		


		for(int i= 0; i < page; i++)
		{
			memset(&program_cmd,0,sizeof(program_cmd));
			program_cmd.head.RS485_Addr		= '1' + BoxNr;
			program_cmd.head.cmd			= CMD_PROGRAM_DATA;
			program_cmd.head.para_len		= sizeof(program_cmd.para);
			memcpy(&program_cmd.para[1],&bin[128 * i],128);
			program_cmd.para[0]				= i;

			if(i == 0)
			{
				program_cmd.para[1 + FLASH_PARA_ADDR] = BoxNr + '1';
				program_cmd.para[2 + FLASH_PARA_ADDR] = BoardType;
				program_cmd.para[3 + FLASH_PARA_ADDR] = NoOSC;
			}
			program_cmd.crc					= CComm::CRC16((unsigned char*)&program_cmd,sizeof(program_cmd) - 2); 
	//		program_cmd.Tail[0]				=	0x0D;
	//		program_cmd.Tail[1]				=	0x0A;

			G_SendmCmdProgram.InsertAt(0,program_cmd);
		}
	//	s.Format(_T("%d包数据"),G_SendmCmdProgram.GetSize());
	//	AfxMessageBox(s);
	}
#ifndef WINCE
	else
	{
		datalen = Hex2Bin_ARM(HexBuf,HexBufLen,bin,sizeof(bin));
		page = (datalen / 1024);
		if((datalen % 1024) > 0)
		{
			page ++;
		}
		else
		{
			AfxMessageBox(_T("空文件!!!!!!!!!!!"));
			return;
		}
		if(page > 15*8)
		{
			AfxMessageBox(_T("文件太大,超出单片机容量!!!!!!!!!!!"));
			return;
		}

		unsigned short crc = CRC16((unsigned char*)bin,datalen);
		CString s;
		s.Format(_T("datalen= %d, crc = %d"),datalen,crc);
		//AfxMessageBox(s);


		for(int i= 0; i < page; i++)
		{
			memset(&program_arm_cmd,0,sizeof(program_arm_cmd));
			program_arm_cmd.head.RS485_Addr		= '1' + BoxNr;
			program_arm_cmd.head.cmd			= CMD_PROGRAM_DATA;
			program_arm_cmd.head.para_len		= sizeof(program_arm_cmd.para);
			memcpy(program_arm_cmd.para,&bin[1024 * i],1024);
			program_arm_cmd.head.page			= i;
			program_arm_cmd.crc					= CComm::CRC16((unsigned char*)&program_arm_cmd,sizeof(program_arm_cmd) - 2); 
	//		program_cmd.Tail[0]				=	0x0D;
	//		program_cmd.Tail[1]				=	0x0A;

			G_SendmCmdProgram_ARM.Add(program_arm_cmd);

		
		}

	}
	
#endif	

//	memset(&program_cmd,0,sizeof(program_cmd));
//		program_cmd.RS485_Addr	= '1';
//		program_cmd.cmd			= CMD_PROGRAM_START;
//		program_cmd.para_len	= sizeof(program_cmd.para);
//		memcpy(&program_cmd.para[1],&bin[128 * 0],128);
//		program_cmd.para[0]		= 0;
//		program_cmd.crc			= CComm::CRC16((unsigned char*)&program_cmd,sizeof(program_cmd) - 2); 
//		G_SendmCmdProgram.InsertAt(0,program_cmd);
//
	
}
void CComm::XModem_Start_ByFile(int BoxNr)
{

	CString strFilter= _T("Hex文件(*.hex)|(*.hex)|| "); 
	CFileDialog dlg(true, 
	_T("Hex "), 
	_T("*.hex"), 
	OFN_OVERWRITEPROMPT, 
	strFilter, 
	NULL); 

	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	
	memset(bin,0xFF,sizeof(bin));

	
	

	memset(HexBuf,0,sizeof(HexBuf));
	CString FileName = dlg.GetPathName(); 
	
	CFile f;
	if(f.Open(FileName,CFile::modeRead))
	{
		f.SeekToBegin();
		HexBufLen = f.Read(HexBuf,sizeof(HexBuf));
	}
	else
	{
		AfxMessageBox(_T("没有找到文件"));
		return;
	}
	memset(bin,0xFF,sizeof(bin));
	int datalen = Hex2Bin(HexBuf,HexBufLen,bin,sizeof(bin));
	
	unsigned short crc = CRC16((unsigned char*)bin,datalen);
	
	int page = (datalen / 128);
	if((datalen % 128) > 0)
	{
		page ++;
	}
	if(datalen == 0)
	{
		AfxMessageBox(_T("空文件!!!!!!!!!!!"));
		return;
	}
	if(page > 15*8)
	{
		AfxMessageBox(_T("文件太大,超出单片机容量!!!!!!!!!!!"));
		return;
	}

	/*	memset(&program_cmd,0,sizeof(program_cmd));
		program_cmd.head.RS485_Addr		= '1' + BoxNr;
		program_cmd.head.cmd			= CMD_PROGRAM_START;
		program_cmd.head.para_len		= sizeof(program_cmd.para);
		memcpy(&program_cmd.para[1],&bin[128 * 0],128);
		program_cmd.para[0]				= 0;
		program_cmd.crc					= CComm::CRC16((unsigned char*)&program_cmd,sizeof(program_cmd) - 2); 
//		program_cmd.Tail[0]				= 0x0D;
//		program_cmd.Tail[1]				= 0x0A;
		G_SendmCmdProgram.Add(program_cmd);
*/

	for(int i= 0; i < page; i++)
	{
		memset(&program_cmd,0,sizeof(program_cmd));
		program_cmd.head.RS485_Addr		= '1'+ BoxNr;
		program_cmd.head.cmd			= CMD_PROGRAM_DATA;
		program_cmd.head.para_len		= sizeof(program_cmd.para);
		memcpy(&program_cmd.para[1],&bin[128 * i],128);
		program_cmd.para[0]				= i;

		if(i == 0)
		{
			program_cmd.para[1 + FLASH_PARA_ADDR] = 0 + '1';
			program_cmd.para[2 + FLASH_PARA_ADDR] = ::G_NormalConfigPara[0].InterfaceBoardType;
			program_cmd.para[3 + FLASH_PARA_ADDR] = ::G_NormalConfigPara[0].TM7705_NoOSC;
		}

		program_cmd.crc					= CComm::CRC16((unsigned char*)&program_cmd,sizeof(program_cmd) - 2); 
		//program_cmd.Tail[0]				= 0x0D;
		//program_cmd.Tail[1]				= 0x0A;
		G_SendmCmdProgram.Add(program_cmd);
	}

	
}

void CComm::ARM_XModem_Start_ByFile(int BoxNr)
{

	CString strFilter= _T("Hex文件(*.hex)|(*.hex)|| "); 
	CFileDialog dlg(true, 
	_T("Hex "), 
	_T("*.hex"), 
	OFN_OVERWRITEPROMPT, 
	strFilter, 
	NULL); 

	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	
	memset(bin,0xFF,sizeof(bin));

	
	

	memset(HexBuf,0,sizeof(HexBuf));
	CString FileName = dlg.GetPathName(); 
	
	CFile f;
	if(f.Open(FileName,CFile::modeRead))
	{
		f.SeekToBegin();
		HexBufLen = f.Read(HexBuf,sizeof(HexBuf));
	}
	else
	{
		AfxMessageBox(_T("没有找到文件"));
		return;
	}
	memset(bin,0xFF,sizeof(bin));
	int datalen = Hex2Bin_ARM(HexBuf,HexBufLen,bin,sizeof(bin));
	
	unsigned short crc = CRC16((unsigned char*)bin,datalen);
	

	int page = (datalen / 1024);
	if((datalen % 1024) > 0)
	{
		page ++;
	}
	if(datalen == 0)
	{
		AfxMessageBox(_T("空文件!!!!!!!!!!!"));
		return;
	}
	if(page > 60)
	{
		AfxMessageBox(_T("文件太大,超出单片机容量!!!!!!!!!!!"));
		return;
	}
	for(int i= 0; i < page; i++)
	{
		memset(&program_arm_cmd,0,sizeof(program_arm_cmd));
		program_arm_cmd.head.RS485_Addr	= '1';
		program_arm_cmd.head.cmd		= CMD_PROGRAM_DATA;
		program_arm_cmd.head.para_len	= sizeof(program_arm_cmd.para);
		memcpy(&program_arm_cmd.para,&bin[1024 * i],1024);
		program_arm_cmd.head.page			= i;
		program_arm_cmd.crc				= CComm::CRC16((unsigned char*)&program_arm_cmd,sizeof(program_arm_cmd) - 2); 
		G_SendmCmdProgram_ARM.Add(program_arm_cmd);
	}

//	G_ProgramStarted = 0;

	//G_RS485_Comm.SendProgramStart(0);

	
}


#define HEX_TYPE_DataRecord						0
#define HEX_TYPE_EndOfFileRecord				1
#define HEX_TYPE_ExtendedSegmentAddressRecord	2
#define HEX_TYPE_StartSegmentAddressRecord		3
#define HEX_TYPE_ExtendedLinearAddressRecord	4
#define HEX_TYPE_StartLinearAddressRecord		5

typedef struct
{
	unsigned char header;
	unsigned char dataLen[2];
	unsigned char dataAddr[4];
	unsigned char dataType[2];
	unsigned char data[100];
}HEX_LINE;
// ===========================
//HEX文件以行为单位。每行以字符 ‘:’ (0x3A)开头，以回车--换行符0x0D, 0x0A为结束。
//每行开始和结束之间的所有内容，都是以字符形式表现的。
//8 位数据：数据如果是 0xFE ，那么转换到HEX格式的行里面就是0x3F 0x4E。
//16位数据：如16位地址，先显示高位，后显示底位。
//例如 0xABCD，转换成HEX格式文件后变成 0x3A 0x3B 0x3C 0x3D，显示出来以后就是ABCD。
//数据部分---内容每2个字符看做一个HEX数据。
//
//:020000040800F2    ----->第一行数据： 0x02 0x00 0x00  0x04  0x08  0x00  0xF2
//第一个 0x02 为数据长度。
//紧跟着后面的0x00 0x00 为地址。
//再后面的0x04为数据类型，类型共分一下几类：
//'00' Data Record                    '00'数据记录
//'01' End of File Record           '01'文件记录结束
//'02' Extended Segment Address Record     '02'扩展段地址记录
//'03' Start Segment Address Record        '03'开始段地址记录
//'04' Extended Linear Address Record      '04'扩展线性地址记录
//'05' Start Linear Address Record          '05'启动线性地址记录
//
//然后，接着0x04后面的两个 0x00 0x00就是数据。最后一个0xF2是校验码。
//HEX文件的每一行都是这样的格式：
//            <0x3a>
//            [数据长度1Byte]
//            [数据地址2Byte]
//            [数据类型1Byte]
//            [数据nByte]
//            [校验1Byte]
//            <0x0d>
//            <0x0a>
//校验值：每一行的最后一个值为此行数据的校验和
//
//开头 长度  地址  数据类型   数据   校验值
// :      02   0000    04       0800     F2
//
// 校验和算法：
//    校验和的算法为：计算从0x3A 以后（不包括0x3A）的所有各字节的和模256的余。
// 即各字节二进制算术和，不计超过256的溢出值，然后用0x100减去这个算数累加和，
// 得出得值就是此行得校验和。
//举例：---十六进制运算：02+00+00+04+08+00=0E
//               校验值：0x100 - 0x0E =0xF2
//
//10+00+00+00+30+0B+00+20+CD+01+00+08+E1+2D+00+08+E1+2D+00+08=36D
//        36D--->十进制877--->%256=109==0x6D 
//        校验值：0x100-0x6D=0X93
//
// //stm32f4为32位数据，以0x00 0x00每16位数据作为一帧数据发送       校验位    回车   换行
//:     10  0000    00    300B 0020 CD01 0008 E12D 0008 E12D 0008   93      <0X0D>  <0X0A>
//
//:10000000300B0020CD010008E12D0008E12D000893
//安装上面的数据行格式分析如下:
//            <0x3a>
//            [数据长度1Byte]  10
//            [数据地址2Byte]  00 00
//            [数据类型1Byte]  00
//            [数据nByte]      30 0B 00 20 CD 01 00 08 E1 2D 00 08 E1 2D 00 08
//            [校验1Byte]       C0
//            <0x0d>
//            <0x0a>
//
//每行中的数据并不是一定有的，第二个直接的数据长度为0，那么这行就没有数据。
//由于每行标识数据地址的只有2Byte，所以最大只能到64K，为了可以保存更大数据地址数据，就有了Extended LinearAddress Record。
//如果这行的数据类型是0x04，那么，这行的数据就是随后数据的基地址。
//:020000040800F2
//:10000000300B0020CD010008E12D0008E12D000893
//:10001000E12D0008E12D0008E12D0008000000009E
//第一行，是Extended LinearAddress Record，里面的基地址是0x0004，
//第二行是Data Record，里面的地址值是0x0000。那么数据300B0020CD010008E12D0008E12D0008
//要写入FLASH中的地址为(0x0004 << 16) | 0x0000，也就是写入FLASH的0x40000这个地址。
//同样，第三行的数据的写入地址为0x40010。
//当一个HEX文件的数据超过64k的时候，文件中就会出现多个Extended Linear Address Record。
//
//:00000001FF
//End of File Record 行是每一个HEX文件的最后一行。例如：
//这样的一行数据内容是固定的，数据长度为0，地址为0。
//
//
//HEX文件如下：
//:020000040800F2
//:10000000300B0020CD010008E12D0008E12D000893
//:10001000E12D0008E12D0008E12D0008000000009E
//          1      2       3       4       5       6       7       8       9       10
//: 10 0020 0000 0000 0000 0000 0000 0000 00E1 2D00 08BA    末尾的回车换行省略
//:10003000E12D000800000000E12D0008E12D00087E
//:10004000E12D0008E12D0008E12D0008E12D000858
//:10005000E12D0008E12D0008E12D0008E12D000848
//:10006000E12D0008E12D0008E12D0008E12D000838
//:10004000E12D0008E12D0008E12D0008E12D000858
//:10005000E12D0008E12D0008E12D0008E12D000848
//:10006000E12D0008E12D0008E12D0008E12D000838
//:10007000E12D0008E12D0008E12D0008E12D000828
//    ......
//:107780002C0A29C02C8B7A44040B2A07438813E463
//:107790006A12081A6B04011F1A0C243201690400D2
//:04000005080000ED02
//
//:00000001FF

unsigned char Hex2Char(unsigned char Hex[2])
{
	unsigned char c = 0;
	if(Hex[0] >= '0' && Hex[0] <= '9')
	{
		c = 16 * (Hex[0] - '0');
	}
	else if(Hex[0] >= 'A' && Hex[0] <= 'F')
	{
		c = 16 * (Hex[0] - 'A' + 10);
	}
	else if(Hex[0] >= 'a' && Hex[0] <= 'f')
	{
		c = 16 * (Hex[0] - 'a' + 10);
	}

	if(Hex[1] >= '0' && Hex[1] <= '9')
	{
		c +=  (Hex[1] - '0');
	}
	else if(Hex[1] >= 'A' && Hex[1] <= 'F')
	{
		c +=  (Hex[1] - 'A' + 10);
	}
	else if(Hex[1] >= 'a' && Hex[1] <= 'f')
	{
		c +=  (Hex[1] - 'a' + 10);
	}
	return c;

}



int CComm::Hex2Bin(CString HexFile,char *binData,int MaxLen)
{
	
	int HexPos = 0;
	unsigned int BinPos = 0;

	CFile f;
	if(f.Open(HexFile,CFile::modeReadWrite |  CFile::shareDenyWrite))
	{
		f.SeekToBegin();
		int fileLen = f.Read(HexBuf,sizeof(HexBuf));
		f.Close(); 

		if(fileLen == 0)
		{
			return 0;
		}
		
		while(HexPos < fileLen)
		{
			HEX_LINE hex_line;
			int i;
			if(HexBuf[HexPos] == ':')
			{
				memset(&hex_line,0,sizeof(hex_line));
				i = 0;
				while(1)
				{
					((char*)&hex_line)[i] = HexBuf[HexPos+i];
					if(((char*)&hex_line)[i] == 0x0A)
					{
						break;
					}
					i++;
				}
				int data_len = Hex2Char(hex_line.dataLen);
				unsigned int dataAddr = Hex2Char(&hex_line.dataAddr[0])*(int)256 +  Hex2Char(&hex_line.dataAddr[2]);
				for(int j = 0; j < data_len; j++)
				{
					binData[dataAddr + j] = Hex2Char(&hex_line.data[j*2]);

					if(BinPos < (dataAddr + j))
					{
						BinPos = (dataAddr + j);
					}
				}
				
				HexPos += i;
				continue;
			}
			
			HexPos ++;
		}
	}
	else
	{
		CString s = HexFile;
		s+= _T("\r\n 没有找到此文件"); 
		AfxMessageBox(s);
	}
	return BinPos;
}

int CComm::Hex2Bin(unsigned char *Hex,int HexBufLen ,char *binData,int MaxLen)
{
	
	int HexPos = 0;
	unsigned int BinPos = 0;

//	CFile f;
//	if(f.Open(HexFile,CFile::modeReadWrite |  CFile::shareDenyWrite))
	{
//		f.SeekToBegin();
//		int fileLen = f.Read(HexBuf,sizeof(HexBuf));
//		f.Close(); 

		if(HexBufLen <= 0)
		{
			return 0;
		}
		
		while(HexPos < HexBufLen)
		{
			HEX_LINE hex_line;
			int i;
			if(Hex[HexPos] == ':')
			{
				memset(&hex_line,0,sizeof(hex_line));
				i = 0;
				while(1)
				{
					((char*)&hex_line)[i] = Hex[HexPos+i];
					if(((char*)&hex_line)[i] == 0x0A)
					{
						break;
					}
					i++;
				}
				int data_len = Hex2Char(hex_line.dataLen);
				unsigned int dataAddr = Hex2Char(&hex_line.dataAddr[0])*(int)256 +  Hex2Char(&hex_line.dataAddr[2]);
#ifdef _ARM	
				if(dataAddr >= 0x4000)
#endif
				{
#ifdef _ARM	
					dataAddr -= 0x4000;
#endif
					for(int j = 0; j < data_len; j++)
					{
						binData[dataAddr + j] = Hex2Char(&hex_line.data[j*2]);

						if(BinPos < (dataAddr + j))
						{
							BinPos = (dataAddr + j);
						}
					}
				}
				
				HexPos += i;
				continue;
			}
			
			HexPos ++;
		}
	}
	
	return BinPos;
}


void CComm::SetCmdString_OMRON(int equip, COMMAND_ID cmdId, unsigned char* para)
{

		int i;
		int box = 0;
		
		MODBUS_RS485_CMD_OMRON cmd;
		
		cmd.RS485_Addr = equip;

		cmd.cmd  = cmdId;
		for(i = 0; i < sizeof(cmd.para); i++)
		{
			cmd.para[i] = para[i];
		}
		

		cmd.crc = CComm::CRC16_MODBUS((unsigned char *)&cmd,sizeof(cmd)-2);
		
		m_SendCmdArray_OMRON.Add(cmd);


}
void CComm::ReadAdjustAll_OMRON(int BoxNr)
{
	unsigned char para[100];
	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PV_ADJUST);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));
	
	SetCmdString_OMRON( BoxNr + 1, CMD_OMRON_READ, para );
}

float CComm::OMRON_Get_AdjustedTemp(float temp,int BoxNr)
{
#ifdef _HeatBox
	return temp + temp * (G_NormalConfigPara[BoxNr].OMRON_200C_Adjust  - G_NormalConfigPara[BoxNr].OMRON_0C_Adjust) / 200.0f + G_NormalConfigPara[BoxNr].OMRON_0C_Adjust;
#endif
	return 0;
}

float CComm::OMRON_Get_MeterTemp_By_Sv(float sv,int BoxNr)
{
#ifdef _HeatBox
	return (sv - G_NormalConfigPara[BoxNr].OMRON_0C_Adjust) / ( 1 + (G_NormalConfigPara[BoxNr].OMRON_200C_Adjust  - G_NormalConfigPara[BoxNr].OMRON_0C_Adjust) / 200.0f ); 
#endif
	return 0;
}

void CComm::SendAdjustAll_OMRON(float temp,int BoxNr)
{
#ifdef _HeatBox	
		unsigned char para[100];
		memset(para,0,sizeof(para));

		*((short * )&para[0]) = PcShort2OmronShort(OMRON_PV_ADJUST);

		*((short * )&para[2]) = PcShort2OmronShort((short)(temp *10));
		
		SetCmdString_OMRON(BoxNr + 1, CMD_OMRON_WRITE, para);
#endif
}

void CComm::SendDirectHeatCmd_OMRON(float temp, int BoxNr)
{
	
#ifdef _HeatBox
		unsigned char para[100];
		memset(para,0,sizeof(para));

		*((short * )&para[0]) = PcShort2OmronShort(OMRON_SP);

		*((short * )&para[2]) = PcShort2OmronShort((short)(OMRON_Get_MeterTemp_By_Sv(temp,BoxNr) *10));
		
		SetCmdString_OMRON( BoxNr + 1, CMD_OMRON_WRITE, para);
#endif
}

int CComm::SendAirControlCmd(bool AirOn,bool MeterAirOn,int BoxNr,CString *msg)
{
#ifdef _HeatBox
	unsigned char para[100];

	memset(para,0,sizeof(para));

	if(G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_FLAT)
	{
		if(AirOn == 0)
		{
			para[0] = 0;
			AddCommCmd( '1' + BoxNr,CMD_AIR_CTL, para ,BoxNr);
		}
		else
		{
			para[0] = 1;
			AddCommCmd( '1' + BoxNr,CMD_AIR_CTL, para,BoxNr);
		}

		return para[0];
	}

	
	if(G_NormalConfigPara[BoxNr].AirControlEquipment == 0)
	{
		//AfxMessageBox(_T("没有配置空调"));
		return -1;
	}
	

	

#ifndef _XIN_QI_NENG_
//	m_AirOnIsWaiting = false;

	if((AirOn == true) && (MeterAirOn == false))
	{
		CTime		AC_ShutDownTime(G_NormalConfigPara[BoxNr].stAirControlCloseTime);

		CTimeSpan	span	=	CTime::GetCurrentTime()	- AC_ShutDownTime;
		
		if((span.GetTotalMinutes() >= AIR_WAIT_MINUTES) || (G_NormalConfigPara[BoxNr].BoxType ==(unsigned char)( BOX_48_CHANGSHENG_LOW_TEMP)))
		{
			para[0]										= 1; 
		}
		else
		{
			int seconds = (AIR_WAIT_MINUTES * 60 - span.GetTotalSeconds());
			
			msg->Format(_T("请等待%02d秒空调%02d:%02d:%02d"),seconds,AC_ShutDownTime.GetHour(),AC_ShutDownTime.GetMinute(),AC_ShutDownTime.GetSecond());
			return -2;
		}

		AddCommCmd( '1' + BoxNr,CMD_AIR_CTL, para,BoxNr);

	//	this->SendOpenDoorCmd(false,BoxNr);				//开空调就要关门关鼓风 

	}
	else if((AirOn == false) && (MeterAirOn == true))
	{
		SYSTEMTIME st;
		CTime::GetCurrentTime().GetAsSystemTime(st);   
		if(G_NormalConfigPara[BoxNr].stAirControlCloseTime.wMinute != st.wMinute && G_NormalConfigPara[BoxNr].stAirControlCloseTime.wMinute != (st.wMinute -1))
		{//每2分钟保存一次空调最后运行时间
			G_NormalConfigPara[BoxNr].stAirControlCloseTime = st;
			
			SaveNormalConfigPara(); 
		}
		
		para[0] = 0;
		AddCommCmd( '1'+ BoxNr,CMD_AIR_CTL, para,BoxNr);

	}
	else if(MeterAirOn == true)
	{
		SYSTEMTIME st;
		CTime::GetCurrentTime().GetAsSystemTime(st);   
		if(G_NormalConfigPara[BoxNr].stAirControlCloseTime.wMinute != st.wMinute && G_NormalConfigPara[BoxNr].stAirControlCloseTime.wMinute != (st.wMinute -1))
		{//每2分钟保存一次空调最后运行时间
			G_NormalConfigPara[BoxNr].stAirControlCloseTime = st;
			
			SaveNormalConfigPara(); 
		}
	}


	return para[0];
#endif

#endif
	return 0;
}

void CComm::SendSetUsefulSensors(int Sensors,int BoxNr)
{
	MODBUS_RS485_CMD_128 cmd;
	
	cmd.head.RS485_Addr = '1'+BoxNr;
	cmd.head.cmd = CMD_WRITE_USEFUL_SENSORS;
	cmd.head.para_len = sizeof(cmd.para);
	memset(cmd.para,0,sizeof(cmd.para));  
	
	
	memcpy(&cmd.para[0],&Sensors,sizeof(short)); 



	cmd.crc = CRC16((unsigned char*)&cmd,sizeof(cmd) - 2); 	

	AddCommCmd(cmd.head.RS485_Addr,(COMMAND_ID)cmd.head.cmd, cmd.para,BoxNr);
}

void CComm::SendAT(float temp,int BoxNr)
{
	unsigned char para[100];
	unsigned short short_a = temp * 10.0f;
	
	
	memset(para,0,sizeof(para));
	memcpy(para,&short_a,sizeof(short_a));

	AddCommCmd( '1' + BoxNr,CMD_AT, para,BoxNr  );
}

void CComm::SendCoolAT(float temp,int BoxNr)
{
	unsigned char para[100];
	unsigned short short_a = temp * 10.0f;
	
	
	memset(para,0,sizeof(para));
	memcpy(para,&short_a,sizeof(short_a));

	AddCommCmd( '1',CMD_COOL_AT, para,BoxNr  );
}

void CComm::Delta_Read_X(void)
{

	Delta_SendReadCmd(0x1, DELTA_READ_POINT_X ,DELTA_X_ADDR,0x20);
}
void CComm::Delta_Read_Y(void)
{
	Delta_SendReadCmd(0x1, DELTA_READ_POINT_Y ,DELTA_Y_READ_ADDR,0x20);
}

void CComm::Delta_Read_D32(void)
{

	Delta_SendReadCmd(0x1, DELTA_READ_D ,DELTA_D_ADDR + 0x32,0x01);
}

void CComm::Delta_Set_D32(int ID,int val)
{

	Delta_SendSetCmd(0x1, DELTA_SET_POINT ,DELTA_D_ADDR + ID,val);
}

void CComm::Delta_Set_M(int ID,char On)
{

	Delta_SendSetCmd(0x1, DELTA_SET_POINT ,DELTA_M_ADDR + ID,On);
}

void CComm::Delta_Set_Y(int ID,char On)
{

	Delta_SendSetCmd(0x1, DELTA_SET_POINT ,DELTA_Y_ADDR + ID,On);
}

void CComm::Delta_Set_C(int ID,char On)
{

	Delta_SendSetCmd(0x1, DELTA_SET_POINT ,DELTA_C_ADDR + ID,0);
}

void CComm::Delta_Read_M(int Addr,int len)
{

	Delta_SendReadCmd(0x1, DELTA_READ_M ,DELTA_M_ADDR + Addr,0x10);
}

void CComm::Delta_Read_C_7A(void)
{

	Delta_SendReadCmd(0x1, DELTA_READ_C ,DELTA_C_ADDR + 0x7A,0x05);
}

int   CComm::Delta_Cal_LCR(unsigned char *cmd,int len)
{
	char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	unsigned char lcr = 0;
	unsigned char data = 0;
	unsigned char hex  = 0;
	unsigned char *buf = cmd;
	
	for(int i = 0; i < len / 2; i++)
	{
		hex = Hex2Char(buf[i*2]);
		if( hex == 0xFF)
		{
			return 0xFFFF;
		}
		else
		{
			lcr += hex * 0x10;
		}

		hex = Hex2Char(buf[i*2+1]);
		if( hex == 0xFF)
		{
			return 0xFFFF;
		}
		else
		{
			lcr += hex;
		}

	}

	return (0x100 - lcr);

}

void CComm::Delta_SendReadCmd(unsigned char PlcAddr,unsigned char Cmd,unsigned short DataAddr,unsigned short DataLen)
{
#ifndef _20PT100_5SSR
	char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	DELTA_PLC_READ_CMD cmd;
	cmd.head.STX		= ':';
	cmd.head.Addr[0]	= TEXT[PlcAddr / 0x10];
	cmd.head.Addr[1]	= TEXT[PlcAddr % 0x10];

	cmd.head.Cmd[0]	= TEXT[Cmd / 0x10];
	cmd.head.Cmd[1]	= TEXT[Cmd % 0x10];

	cmd.Data_Addr[0] = TEXT[ (DataAddr  %  0x10000) / 0x1000];
	cmd.Data_Addr[1] = TEXT[ (DataAddr  %   0x1000) /  0x100];
	cmd.Data_Addr[2] = TEXT[ (DataAddr  %    0x100) /   0x10];
	cmd.Data_Addr[3] = TEXT[ (DataAddr  %     0x10) /    0x1];

	cmd.Data_Len[0] = TEXT[ (DataLen  %  0x10000) /  0x1000];
	cmd.Data_Len[1] = TEXT[ (DataLen  %   0x1000) /   0x100];
	cmd.Data_Len[2] = TEXT[ (DataLen  %    0x100) /    0x10];
	cmd.Data_Len[3] = TEXT[ (DataLen  %     0x10) /     0x1];

	unsigned short lcr = Delta_Cal_LCR((unsigned char *)&cmd.head.Addr[0],sizeof(DELTA_PLC_READ_CMD) - 5);

	if(lcr < 0x100)
	{
		cmd.LCR[0] = TEXT[ lcr / 0x10];
		cmd.LCR[1] = TEXT[ lcr % 0x10];

		cmd.Tail[0]	=	0x0D;
		cmd.Tail[1]	=	0x0A;

		this->WriteComm((char*)&cmd,sizeof(cmd));
		
		G_Delta_PLC_ReadCmd  = cmd;
		memset(&G_Delta_PLC_SetCmd,0,sizeof(G_Delta_PLC_SetCmd));
	}

	if(G_SorterPlcDlg.GetSafeHwnd())
	{
		if(G_SorterPlcDlg.IsWindowVisible())
		{
			G_SorterPlcDlg.DisplaySendCmd(cmd.head.Cmd[1],(unsigned char*)&cmd,sizeof(cmd),0);
		}
	}
#endif
}






void CComm::Delta_SendSetCmd(unsigned char PlcAddr,unsigned char Cmd,unsigned short DataAddr,unsigned char On)
{
#ifndef _20PT100_5SSR
	char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	DELTA_PLC_SET_CMD cmd;
	cmd.head.STX		= ':';
	cmd.head.Addr[0]	= TEXT[PlcAddr / 0x10];
	cmd.head.Addr[1]	= TEXT[PlcAddr % 0x10];

	cmd.head.Cmd[0]	= TEXT[Cmd / 0x10];
	cmd.head.Cmd[1]	= TEXT[Cmd % 0x10];

	cmd.Data_Addr[0] = TEXT[ (DataAddr  %  0x10000) / 0x1000];
	cmd.Data_Addr[1] = TEXT[ (DataAddr  %   0x1000) /  0x100];
	cmd.Data_Addr[2] = TEXT[ (DataAddr  %    0x100) /   0x10];
	cmd.Data_Addr[3] = TEXT[ (DataAddr  %     0x10) /    0x1];

	if(On)
	{
		cmd.Data[0] = 'F';
		cmd.Data[1] = 'F';
		cmd.Data[2] = '0';
		cmd.Data[3] = '0';
	}
	else
	{
		cmd.Data[0] = '0';
		cmd.Data[1] = '0';
		cmd.Data[2] = '0';
		cmd.Data[3] = '0';
	}

	unsigned short lcr = Delta_Cal_LCR((unsigned char *)&cmd.head.Addr[0],sizeof(DELTA_PLC_SET_CMD) - 5);

	if(lcr < 0x100)
	{
		cmd.LCR[0] = TEXT[ lcr / 0x10];
		cmd.LCR[1] = TEXT[ lcr % 0x10];

		cmd.Tail[0]	=	0x0D;
		cmd.Tail[1]	=	0x0A;

	
		G_Delta_PLC_SetCmdArray.Add(cmd);
		
	}

	if(G_SorterPlcDlg.GetSafeHwnd())
	{
		if(G_SorterPlcDlg.IsWindowVisible())
		{
			G_SorterPlcDlg.DisplaySendCmd(cmd.head.Cmd[1],(unsigned char*)&cmd,sizeof(cmd),0);
		}
	}
#endif
		
}


void CComm::ReadUsedSensor(int BoxNr)
{
	unsigned char para[100];
	memset(para,0,sizeof(para));

//	AddCommCmd( '1'+BoxNr,CMD_READ_USEFUL_SENSORS, para,BoxNr);
		
}

void CComm::ReadAdjustLow(int BoxNr)
{
	unsigned char para[100];
	memset(para,0,sizeof(para));

	AddCommCmd( '1'+BoxNr,CMD_READ_ADJUST_LOW, para,BoxNr);
		
}

void CComm::ReadAdjustHigh(int BoxNr)
{
	unsigned char para[100];
	memset(para,0,sizeof(para));
	
	AddCommCmd( '1'+BoxNr,CMD_READ_ADJUST_HIGH, para,BoxNr);
}

void CComm::SetAdjustHigh(float *val,int BoxNr)
{
	unsigned char para[100];
	unsigned short short_a;
	memset(para,0,sizeof(para));
	if(::G_NormalConfigPara[0].BoxType == BOX_SENSATA_32PT100)
	{
		for(int i = 0; i < 32; i++)
		{
			para[i]	=	val[i] * 10.0f;
		}

	}
	else
	{
		short_a = (short)(val[0] * 10.0f);
		((short*)para)[0] = short_a;

		short_a = (short)(val[1] * 10.0f);
		((short*)para)[1] = short_a;

		short_a = (short)(val[2] * 10.0f);
		((short*)para)[2] = short_a;

		short_a = (short)(val[3] * 10.0f);
		((short*)para)[3] = short_a;
	}


	AddCommCmd( '1'+BoxNr,CMD_SET_ADJUST_HIGH, para,BoxNr);
}

void CComm::SetAdjustLow(float *val,int BoxNr)
{
	unsigned char para[100];
	unsigned short short_a;
	memset(para,0,sizeof(para));

	if(::G_NormalConfigPara[0].BoxType == BOX_SENSATA_32PT100)
	{
		for(int i = 0; i < 32; i++)
		{
			para[i]	=	val[i] * 10.0f;
		}

	}
	else
	{
		short_a = (short)(val[0] * 10.0f);
		((short*)para)[0] = short_a;

		short_a = (short)(val[1] * 10.0f);
		((short*)para)[1] = short_a;

		short_a = (short)(val[2] * 10.0f);
		((short*)para)[2] = short_a;

		short_a = (short)(val[3] * 10.0f);
		((short*)para)[3] = short_a;
	}


	AddCommCmd( '1'+BoxNr,CMD_SET_ADJUST_LOW, para,BoxNr);
}

void CComm::SetAdjustAll(float val,int BoxNr)
{
	unsigned char para[100];
	unsigned short short_a;
	memset(para,0,sizeof(para));

	
	{
		if(val >= 0)
		{
			short_a = (short)(val * 10.0f);
		}
		else
		{
			short_a = (short)(val * 10.0f);
		}
		((short*)para)[0] = short_a;
	}

	AddCommCmd( '1'+BoxNr,CMD_SET_ADJUST_ALL, para,BoxNr);
}

void CComm::SetAdjustAll(float* val,int BoxNr)
{
	unsigned char para[100];
	unsigned short short_a;
	memset(para,0,sizeof(para));
	for(int i = 0; i < 8; i++)
	{
		short_a = (short)(val[i] * 10.0f);
		
		((short*)para)[i] = short_a;
	}


	AddCommCmd( '1'+BoxNr,CMD_SET_ADJUST_ALL, para,BoxNr);
}

void CComm::SendSetHighVolt(int BoxNr,unsigned char bHighVolt)
{
	
	unsigned char para[100];
	memset(para,0,sizeof(para));
	para[0] = bHighVolt;
	
	AddCommCmd( '1'+BoxNr,CMD_SET_HIGH_VOLT, para,BoxNr);

}
void CComm::ReadAdjustAll(int BoxNr)
{
	unsigned char para[100];
	memset(para,0,sizeof(para));
	
	AddCommCmd( '1'+BoxNr,CMD_READ_ADJUST_ALL, para,BoxNr);
}

void CComm::SetDisplayType(int DisplayType,int BoxNr)
{
	unsigned char	para[16];
	memset(para,0,sizeof(para));
	para[0] = DisplayType;  
	AddCommCmd('A'+BoxNr,CMD_SET_DISPLAY_TYPE, para,BoxNr);
}

void CComm::SetAddress(int BoxNr)
{
		unsigned char para[32];
		memset(para,0,sizeof(para));
		para[0] = BoxNr + '1';
		AddCommCmd ( 0 ,CMD_SET_ADDR, para,BoxNr);
}

void CComm::SetPID_T(short *val,int BoxNr)
{
	unsigned char para[100];

	memset(para,0,sizeof(para));
	for(int i = 0; i < 8; i++)
	{
		((short*)para)[i] = val[i];
	}

	AddCommCmd( '1'+BoxNr,CMD_SET_PID_T, para,BoxNr);
}

void CComm::SetPID_T(short *val,int BoxNr,int ID)
{
	unsigned char para[100];

	memset(para,0,sizeof(para));
	for(int i = 0; i < 8; i++)
	{
		((short*)para)[i] = val[i];
	}
	para[14] = ID;
	AddCommCmd( '1'+BoxNr,CMD_SET_PID_T, para,BoxNr);
}

MODBUS_RS485_CMD_192 * CComm::SendReadStatus_192(int BoxNr)
{
	
	unsigned char para[100];
	
	memset(para,0,sizeof(para));
	
//	if(G_NormalConfigPara[BoxNr].MaxLedNrType == 1)
	{
	static MODBUS_RS485_CMD_192 cmd_192;

	cmd_192.head.RS485_Addr = 'A' + BoxNr;
	cmd_192.head.cmd  = CMD_READ_STATUS;
	cmd_192.head.para_len = sizeof(cmd_192.para); 
	memcpy(cmd_192.para,para,sizeof(cmd_192.para));
	
	cmd_192.crc = CComm::CRC16((unsigned char *)&cmd_192,sizeof(cmd_192)-2);
	
	this->WriteComm((char*)&cmd_192,sizeof(cmd_192)); 
	return &cmd_192;
	}
	
}

MODBUS_RS485_CMD_32PT100 * CComm::SendReadStatus_32PT100(int BoxNr)
{
#ifdef _HeatBox	
	unsigned char para[100];
	
	memset(para,0xFF,sizeof(para));
	
	
	static MODBUS_RS485_CMD_32PT100 cmd_128;

	cmd_128.head.RS485_Addr = 'A' + BoxNr;
	cmd_128.head.cmd  = CMD_READ_STATUS;
	cmd_128.head.para_len = sizeof(cmd_128.para); 
	memcpy(cmd_128.para,para,sizeof(cmd_128.para));

	if(BoxNr == 0 && ::G_NormalConfigPara[0].BoxType == BOX_NTC)
	{
		cmd_128.para[0] = G_NormalConfigPara[0].SwitchType;
#ifdef WINCE
		//short max = G_HeatBoxDlg.m_TempRange.open_temp_max *10;
		//short min = G_HeatBoxDlg.m_TempRange.open_temp_min *10;
		short max,min;
		if(G_NormalConfigPara[0].SwitchType == 0)
		{
			float R = ::G_HeatBoxDlg.m_TempRange.open_temp_max * 1000;
			max = 4096*R/(10000+R);

			R = ::G_HeatBoxDlg.m_TempRange.open_temp_min * 1000;

			min = 4096*R/(10000+R);;
		}
		else
		{
			float R = ::G_HeatBoxDlg.m_TempRange.open_temp_max * 1000;
			max = 4096*R/(100000+R);

			R = G_HeatBoxDlg.m_TempRange.open_temp_min * 1000;

			min = 4096*R/(100000+R);;
		}
#else
		short max,min;
		if(G_NormalConfigPara[0].SwitchType == 0)
		{
			float R = ::G_MainDlg->m_SwitchDlg[0].m_WorkMode.m_TempRange.open_temp_max * 1000;
			max = 4096*R/(10000+R);

			R = ::G_MainDlg->m_SwitchDlg[0].m_WorkMode.m_TempRange.open_temp_min * 1000;

			min = 4096*R/(10000+R);;
		}
		else
		{
			float R = ::G_MainDlg->m_SwitchDlg[0].m_WorkMode.m_TempRange.open_temp_max * 1000;
			max = 4096*R/(100000+R);

			R = ::G_MainDlg->m_SwitchDlg[0].m_WorkMode.m_TempRange.open_temp_min * 1000;

			min = 4096*R/(100000+R);;
		}
#endif
	
		memcpy(&cmd_128.para[1],&min,sizeof(short));
		memcpy(&cmd_128.para[3],&max,sizeof(short));
	}

	cmd_128.crc = CComm::CRC16((unsigned char *)&cmd_128,sizeof(cmd_128)-2);
	
	this->WriteComm((char*)&cmd_128,sizeof(cmd_128)); 
	return &cmd_128;
#endif
	return 0;
	
}

MODBUS_RS485_CMD_128 * CComm::SendReadStatus(int BoxNr)
{
#ifdef _HeatBox	
	unsigned char para[100];
	
	memset(para,0xFF,sizeof(para));
	
	
	static MODBUS_RS485_CMD_128 cmd_128;

	cmd_128.head.RS485_Addr = 'A' + BoxNr;
	cmd_128.head.cmd  = CMD_READ_STATUS;
	cmd_128.head.para_len = sizeof(cmd_128.para); 
	memcpy(cmd_128.para,para,sizeof(cmd_128.para));

	if(BoxNr == 0 && ::G_NormalConfigPara[0].BoxType == BOX_NTC)
	{
		cmd_128.para[0] = G_NormalConfigPara[0].SwitchType;
#ifdef WINCE
		//short max = G_HeatBoxDlg.m_TempRange.open_temp_max *10;
		//short min = G_HeatBoxDlg.m_TempRange.open_temp_min *10;
		short max,min;
		if(G_NormalConfigPara[0].SwitchType == 0)
		{
			float R = ::G_HeatBoxDlg.m_TempRange.open_temp_max * 1000;
			max = 4096*R/(10000+R);

			R = ::G_HeatBoxDlg.m_TempRange.open_temp_min * 1000;

			min = 4096*R/(10000+R);;
		}
		else
		{
			float R = ::G_HeatBoxDlg.m_TempRange.open_temp_max * 1000;
			max = 4096*R/(100000+R);

			R = G_HeatBoxDlg.m_TempRange.open_temp_min * 1000;

			min = 4096*R/(100000+R);;
		}
#else
		short max,min;
		if(G_NormalConfigPara[0].SwitchType == 0)
		{
			float R = ::G_MainDlg->m_SwitchDlg[0].m_WorkMode.m_TempRange.open_temp_max * 1000;
			max = 4096*R/(10000+R);

			R = ::G_MainDlg->m_SwitchDlg[0].m_WorkMode.m_TempRange.open_temp_min * 1000;

			min = 4096*R/(10000+R);;
		}
		else
		{
			float R = ::G_MainDlg->m_SwitchDlg[0].m_WorkMode.m_TempRange.open_temp_max * 1000;
			max = 4096*R/(100000+R);

			R = ::G_MainDlg->m_SwitchDlg[0].m_WorkMode.m_TempRange.open_temp_min * 1000;

			min = 4096*R/(100000+R);;
		}
#endif
	
		memcpy(&cmd_128.para[1],&min,sizeof(short));
		memcpy(&cmd_128.para[3],&max,sizeof(short));
	}

	cmd_128.crc = CComm::CRC16((unsigned char *)&cmd_128,sizeof(cmd_128)-2);
	
	this->WriteComm((char*)&cmd_128,sizeof(cmd_128)); 
	return &cmd_128;
#endif
	return 0;
	
}
MODBUS_RS485_CMD_300 * CComm::SendReadStatus_300(int BoxNr)
{
#ifdef _HeatBox	
	unsigned char para[100];
	
	memset(para,0,sizeof(para));
	
	
	static MODBUS_RS485_CMD_300 cmd_128;

	cmd_128.head.RS485_Addr = 'A' + BoxNr;
	cmd_128.head.cmd  = CMD_READ_STATUS;
	cmd_128.head.para_len = sizeof(cmd_128.para); 
	memcpy(cmd_128.para,para,sizeof(cmd_128.para));

	cmd_128.crc = CComm::CRC16_MODBUS((unsigned char *)&cmd_128,sizeof(cmd_128)-2);
	
	this->WriteComm((char*)&cmd_128,sizeof(cmd_128)); 
	return &cmd_128;
#endif
	return 0;
}

MODBUS_RS485_CMD_128 *  CComm::SendReadBimetalStatus(int BoxNr,unsigned char SoundLevel)
{
#ifdef _HeatBox
	static MODBUS_RS485_CMD_128 cmd;

	unsigned char para[100];
	
	memset(para,0,sizeof(para));

	//和ReadTemp一样，不能用AddCommCmd命令
//	AddCommCmd( 'A'+BoxNr ,CMD_READ_DISC_STATUS, para,BoxNr);

	cmd.head.RS485_Addr = 'A'+ BoxNr ;
				
	cmd.head.cmd = CMD_READ_DISC_STATUS;

	cmd.head.para_len = sizeof(cmd.para);

	memset(cmd.para,0,sizeof(cmd.para));

	cmd.para[0] = SoundLevel;

	cmd.crc = CRC16((unsigned char*)&cmd,sizeof(cmd) - 2); 

	this->WriteComm((char*)&cmd,sizeof(cmd)); 
	return &cmd;
#endif
	return 0;
}

void CComm::SendProgramStart(int BoxNr)
{
	unsigned char para[100];
	
	memset(para,0,sizeof(para));
	
	if(m_CmdArray_128[BoxNr].GetSize() > 0)
	{
		m_CmdArray_128[BoxNr].RemoveAll();
	}

	AddCommCmd( '1'+BoxNr ,CMD_PROGRAM_START, para,BoxNr);

}

void CComm::SendAutoTest(bool test,int BoxNr)
{
	unsigned char para[100];
	
	memset(para,0,sizeof(para));
	if(test)
	{
		para[0] = 1;
	}
	else
	{
		para[0] = 0;
	}
	AddCommCmd( '1'+BoxNr ,CMD_AUTO_TEST, para,BoxNr);

}

COMMAND_STRING G_CommandString[] = 
{
	{CMD_NULL                   ,		_T("CMD_NULL")},
	{CMD_STOP_OUT               ,       _T("停止加热")},
	{CMD_FULL_OUT               ,       _T("全速加热")},
	{CMD_NORMAL_PID_OUT         ,       _T("恒温")},
	{CMD_STEP_PID_OUT           ,       _T("线性升温")},
	{CMD_READ_TEMP				,       _T("读当前温度")},
	{CMD_READ_PID_T				,       _T("读PID参数")},
	{CMD_SET_PID_T				,       _T("设置PID参数")},
	{CMD_SET_ADJUST_LOW			,       _T("设置0°修正值")},
	{CMD_SET_ADJUST_HIGH		,       _T("设置200°修正值")},
	{CMD_READ_ADJUST_LOW		,       _T("读0°修正值")},
	{CMD_READ_ADJUST_HIGH		,       _T("读200°修正值")},
  
//  	{CMD_READ_FLASH				,       _T("读闪动")},
	{CMD_SET_SWITCH_NORMAL_MODE	,       _T("设置保护器类型")},
	{CMD_AUTO_TEST				,       _T("开始测试")},
	{CMD_SET_HIGH_VOLT			,       _T("设置高压")},
	{CMD_SET_RELAY				,       _T("设置开关门")},
	{CMD_SET_BEEP				,		_T("蜂鸣器")},
  	{CMD_READ_AUTHOR			,       _T("CMD_READ_AUTHOR")},
	{CMD_READ_BUILDDATE			,       _T("CMD_READ_BUILDDATE")},
	{CMD_READ_PRODUCEDATE		,       _T("CMD_READ_PRODUCEDATE")},
	{CMD_SAVE_ADJUST_BOARD		,       _T("CMD_SAVE_ADJUST_BOARD")},
	{CMD_READ_ADJUST_BOARD_LOW	,       _T("CMD_READ_ADJUST_BOARD_LOW")},
	{CMD_READ_ADJUST_BOARD_HIGH	,       _T("CMD_READ_ADJUST_BOARD_HIGH")},
	{CMD_READ_SN				,       _T("CMD_READ_SN")},
	{CMD_SET_ADJUST_ALL			,       _T("设置总调")},
    
    {CMD_CPLD_RESET				,       _T("设置 CPLD 复位")},
    
    {CMD_READ_STATUS			,       _T("读保护器状态")},
//	{CMD_READ_OPEN_FLASH		,       _T("读动作闪动")},
//	{CMD_READ_CLOSE_FLASH		,       _T("读复位闪动")},
	{CMD_START_OPEN_FLASH_TEST	,       _T("开始动闪测试")},
	{CMD_START_CLOSE_FLASH_TEST	,       _T("开始复位测试")},
	{CMD_END_FLASH_TEST			,       _T("结束闪动测试")},
	{CMD_SET_FLASH_TEST_SPEED	,       _T("设置闪动测试的速度")},
    
    {CMD_SET_DISPLAY_TYPE		,       _T("设置显示模式")},
	{CMD_READ_ADJUST_ALL		,       _T("读温度总调")},
	{CMD_WRITE_STEP_PID_COUNT	,       _T("CMD_WRITE_STEP_PID_COUNT")},
	{CMD_READ_STEP_PID_COUNT	,       _T("CMD_READ_STEP_PID_COUNT")},
	{CMD_AIR_CTL				,       _T("设置 制冷")},
	{CMD_STEP_PID_COOL			,       _T("设置 线性 降温 ")},
	{CMD_PROGRAM_START			,		_T("开始更新程序")},
	{CMD_PROGRAM_DATA			,		_T("更新程序...")},
	{CMD_WRITE_USEFUL_SENSORS	,		_T("设置传感器数量")},
//	{CMD_READ_USEFUL_SENSORS	,		_T("读取传感器数量")},	
	{CMD_SET_PCB_TYPE_192_INTERFACE,	_T("设置192点夹具板类型")},
	{CMD_SET_ADDR,						_T("设置设备地址")},
	{CMD_STM32_UNIQUE_ID,				_T("CPU ID")},
	{CMD_READ_DISC_STATUS,			_T("读双片")},

};                              






CString CComm::GetCmdString(unsigned char cmd)
{
		CString s;
		for(int i = 0 ; i < sizeof(G_CommandString) / sizeof(COMMAND_STRING); i++)
		{
			if(G_CommandString[i].ID == cmd)
			{
				s.Format(_T(" %X"), cmd); 
				s = G_CommandString[i].str + s;
				return s;
			}
		}

		s.Format(_T("无效的命令 %X"),cmd);

		return  s;
}

//========

char  s_buf[1000],s1_buf[100];

void ProgramPara(int data_len,int BoxNr,MODBUS_RS485_CMD_PARA	*Para_Answer)
{
#ifdef WINCE
	if(::G_HeatBoxDlg.m_WorkMode.m_SubMode != SUB_MODE_STOP)
	{
		return;
	}
#elif _20PT100_5SSR
#else
	if(G_MainDlg->m_SwitchDlg[BoxNr].m_WorkMode.m_SubMode != SUB_MODE_STOP)
	{
		return;
	}
#endif
#ifndef _20PT100_5SSR
		if(data_len == sizeof(MODBUS_RS485_CMD_PARA))
		{
			memset(&program_cmd,0,sizeof(program_cmd));
			program_cmd.head.RS485_Addr	= '1' + BoxNr;
			program_cmd.head.cmd			= CMD_PROGRAM_DATA;
			program_cmd.head.para_len	= sizeof(program_cmd.para);
			memcpy(&program_cmd.para[1],&Para_Answer->EEPROM_Answer,sizeof(Para_Answer->EEPROM_Answer));
			program_cmd.para[0]		= (0x3B80 / 128);
			
			program_cmd.crc			= CComm::CRC16((unsigned char*)&program_cmd,sizeof(program_cmd) - 2); 
//			program_cmd.Tail[0]				= 0x0D;
//			program_cmd.Tail[1]				= 0x0A;

			if(BoxNr >= 0 && BoxNr <= 9)
			{
#ifdef WINCE
				if (::G_HeatBoxDlg.m_WorkMode.m_SubMode != SUB_MODE_CAL_LOW
					&& G_HeatBoxDlg.m_WorkMode.m_SubMode != SUB_MODE_CAL_HIGH)
				{
					G_SendmCmdProgram.InsertAt(0,program_cmd);
				}

#else
			   if (G_MainDlg->m_SwitchDlg[BoxNr].m_WorkMode.m_SubMode != SUB_MODE_CAL_LOW
					&& G_MainDlg->m_SwitchDlg[BoxNr].m_WorkMode.m_SubMode != SUB_MODE_CAL_HIGH)
				{
					G_SendmCmdProgram.InsertAt(0,program_cmd);
				}
#endif
			}
			
		}
#endif
}

LRESULT DealOnComm_Delta_PLC(WPARAM wParam,LPARAM lParam,CWnd * ParentWnd)
{

#ifdef _HeatBox
#ifdef WINCE
	CWelcomeDlg* MainWnd = (CWelcomeDlg *) ParentWnd;
#else
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg *) ParentWnd;
#endif
#else
	CMainDlg* MainWnd = (CMainDlg *) ParentWnd;
#endif

	int i,BoxNr,data_len;
	CTime t;
	t=t.GetCurrentTime(); 
	unsigned char rec_cmd[1024];

	

	float temp[4];
	short para[4];
	short short_a;

	memset(rec_cmd,0,sizeof(rec_cmd));
	memcpy(rec_cmd,(char*)wParam,lParam);
	data_len = lParam;


#ifdef WINCE
	if(::G_NormalConfigPara[0].BoxType == BOX_DELTA_PLC_8PT100_8SSR
		|| G_NormalConfigPara[0].BoxType == BOX_8PT100_8SSR)
	{
		DealDeltaPLC(rec_cmd, data_len );
		return 0;
	}
#endif
	return 0;
}


void DealDeltaPLC(unsigned char *rec_cmd, int data_len )
{
#ifndef _20PT100_5SSR

#ifdef WINCE
	CComm *comm = &G_PrintRS232_Comm;
#else
	CComm *comm = &G_RS485_Comm;
#endif

	CString s,s1;

	DELTA_PLC_READ_X_ANSWER					*PLC_Read_X_Answer		=	(DELTA_PLC_READ_X_ANSWER *)rec_cmd;
	DELTA_PLC_READ_Y_ANSWER					*PLC_Read_Y_Answer		=	(DELTA_PLC_READ_Y_ANSWER *)rec_cmd;
	DELTA_PLC_READ_D_ANSWER					*PLC_Read_D_Answer		=	(DELTA_PLC_READ_D_ANSWER *)rec_cmd;

	DELTA_PLC_READ_C_ANSWER					*PLC_Read_C_Answer		=	(DELTA_PLC_READ_C_ANSWER *)rec_cmd;
	DELTA_PLC_READ_M_ANSWER					*PLC_Read_M_Answer		=	(DELTA_PLC_READ_M_ANSWER *)rec_cmd;

	DELTA_PLC_READ_CMD						*PLC_Read_Cmd			=	(DELTA_PLC_READ_CMD *)rec_cmd;

	DELTA_PLC_SET_CMD						*PLC_Set_Cmd_Answer		=	(DELTA_PLC_SET_CMD *)rec_cmd;

	if(G_SorterPlcDlg.GetSafeHwnd())
	{
		if(G_SorterPlcDlg.IsWindowVisible())
		{
			G_SorterPlcDlg.DisplayReceiveCmd(PLC_Read_X_Answer->head.Cmd[1],(unsigned char*)PLC_Read_X_Answer,sizeof(DELTA_PLC_READ_X_ANSWER),0);
		}
	}

	

	if((G_Delta_PLC_SetCmd.head.STX != 0)
		&& (G_Delta_PLC_SetCmd.head.Cmd[0] != 0)
		&& (G_Delta_PLC_SetCmd.head.Cmd[1] != 0))
	{//设置命令

		if(G_Delta_PLC_SetCmd.Data_Addr[0] == '0'
			&& G_Delta_PLC_SetCmd.Data_Addr[1] == '8'
			&& G_Delta_PLC_SetCmd.Data_Addr[2] == '6'
			&& G_Delta_PLC_SetCmd.Data_Addr[3] == '6')
		{
			
			if(PLC_Set_Cmd_Answer->Data[0] == 'F' 
				&& PLC_Set_Cmd_Answer->Data[1] == 'F' 
				&& PLC_Set_Cmd_Answer->Data[2] == '0' 
				&& PLC_Set_Cmd_Answer->Data[3] == '0' )
			{
				CWnd *w;
				if(G_SorterPlcDlg.GetSafeHwnd() && G_SorterPlcDlg.IsWindowVisible())
				{
					w = G_SorterPlcDlg.GetDlgItem(IDC_AUTO);
					w->SetWindowTextW(_T("手工模式"));
					G_SorterPlcDlg.m_ManualMode = 1;
					w = G_SorterPlcDlg.GetDlgItem(IDC_RUN);
					w->EnableWindow(false);
					w = G_SorterPlcDlg.GetDlgItem(IDC_STOP);
					w->EnableWindow(false);
					w = G_SorterPlcDlg.GetDlgItem(IDC_RECOVER);
					w->EnableWindow(false);
					
				}
			}
			else if(PLC_Set_Cmd_Answer->Data[0] == '0' 
				&& PLC_Set_Cmd_Answer->Data[1] == '0' 
				&& PLC_Set_Cmd_Answer->Data[2] == '0' 
				&& PLC_Set_Cmd_Answer->Data[3] == '0' )
			{
					CWnd *w;
					if(G_SorterPlcDlg.GetSafeHwnd() && G_SorterPlcDlg.IsWindowVisible())
					{
						w = G_SorterPlcDlg.GetDlgItem(IDC_AUTO);
						w->SetWindowTextW(_T("自动模式"));
						G_SorterPlcDlg.m_ManualMode = 0;
						w = G_SorterPlcDlg.GetDlgItem(IDC_RUN);
						w->EnableWindow(true);
						w = G_SorterPlcDlg.GetDlgItem(IDC_STOP);
						w->EnableWindow(true);
						w = G_SorterPlcDlg.GetDlgItem(IDC_RECOVER);
						w->EnableWindow(true);
					}
			}
		}
		else if(G_Delta_PLC_SetCmd.Data_Addr[0] == '0'
			&& G_Delta_PLC_SetCmd.Data_Addr[1] == 'E'
			&& G_Delta_PLC_SetCmd.Data_Addr[2] == '7'
			&& G_Delta_PLC_SetCmd.Data_Addr[3] == 'C')
		{
			CWnd *w;
			w = G_SorterPlcDlg.GetDlgItem(IDC_EDIT_HIGH_LEVEL);
			s.Format(_T("%d"),0);
			w->SetWindowTextW(s);
		}

		else if(G_Delta_PLC_SetCmd.Data_Addr[0] == '0'
			&& G_Delta_PLC_SetCmd.Data_Addr[1] == 'E'
			&& G_Delta_PLC_SetCmd.Data_Addr[2] == '7'
			&& G_Delta_PLC_SetCmd.Data_Addr[3] == 'A')
		{
			CWnd *w;
			w = G_SorterPlcDlg.GetDlgItem(IDC_EDIT_LOW_LEVEL);
			s.Format(_T("%d"),0);
			w->SetWindowTextW(s);
		}


		else if(G_Delta_PLC_SetCmd.Data_Addr[0] == '0'
			&& G_Delta_PLC_SetCmd.Data_Addr[1] == 'E'
			&& G_Delta_PLC_SetCmd.Data_Addr[2] == '7'
			&& G_Delta_PLC_SetCmd.Data_Addr[3] == 'E')
		{
			CWnd *w;
			w = G_SorterPlcDlg.GetDlgItem(IDC_EDIT_MAIN_LEVEL);
			s.Format(_T("%d"),0);
			w->SetWindowTextW(s);
		}

		return;
	}

	if(G_Delta_PLC_ReadCmd.Data_Addr[0] == '0'
		&& G_Delta_PLC_ReadCmd.Data_Addr[1] == '8'
		&& G_Delta_PLC_ReadCmd.Data_Addr[2] == '6'
		&& G_Delta_PLC_ReadCmd.Data_Addr[3] == '0')
	{
		if(G_SorterPlcDlg.GetSafeHwnd())
		{
			if(G_SorterPlcDlg.IsWindowVisible())
			{
				CWnd *w;
				CString s;
				unsigned int count;

				count = Hex2Char(&PLC_Read_M_Answer->Data[0]) * 256;
				count += Hex2Char(&PLC_Read_M_Answer->Data[2]) ;
				w = G_SorterPlcDlg.GetDlgItem(IDC_EDIT_0860);
				s.Format(_T("%X"),count);
				w->SetWindowTextW(s);

				w = G_SorterPlcDlg.GetDlgItem(IDC_AUTO);

				G_SorterPlcDlg.m_Mode = count;

				if(G_SorterPlcDlg.m_Mode & PLC_MANUAL_MODE)
				{
					w->SetWindowTextW(_T("手工模式"));
					G_SorterPlcDlg.m_ManualMode = 1;
					w = G_SorterPlcDlg.GetDlgItem(IDC_RUN);
					w->EnableWindow(false);
					w = G_SorterPlcDlg.GetDlgItem(IDC_STOP);
					w->EnableWindow(false);
					w = G_SorterPlcDlg.GetDlgItem(IDC_RECOVER);
					w->EnableWindow(false);
				}
				else
				{
					w->SetWindowTextW(_T("自动模式"));
					G_SorterPlcDlg.m_ManualMode = 0;
					w = G_SorterPlcDlg.GetDlgItem(IDC_RUN);
					w->EnableWindow(true);
					w = G_SorterPlcDlg.GetDlgItem(IDC_STOP);
					w->EnableWindow(true);
					w = G_SorterPlcDlg.GetDlgItem(IDC_RECOVER);
					w->EnableWindow(true);
				}

				w = G_SorterPlcDlg.GetDlgItem(IDC_RECOVER);


				if(G_SorterPlcDlg.m_Mode & PLC_RECOVER_MODE)
				{
					G_SorterPlcDlg.m_RecoverMode = 1;
					w->SetWindowTextW(_T("复位模式2"));
				}
				else
				{
					G_SorterPlcDlg.m_RecoverMode = 0;
					w->SetWindowTextW(_T("复位模式"));
				}

				if(G_SorterPlcDlg.m_Mode & PLC_RUN_MODE)
				{
					G_SorterPlcDlg.m_RunMode = 1;
				}
				else
				{
					G_SorterPlcDlg.m_RunMode = 0;
				}
				if(G_SorterPlcDlg.m_Mode & PLC_STOP_MODE)
				{
					G_SorterPlcDlg.m_StopMode = 1;
				}
				else
				{
					G_SorterPlcDlg.m_StopMode = 0;
				}

			}
		}
		

	}
	else if(G_Delta_PLC_ReadCmd.Data_Addr[0] == '1'
		&& G_Delta_PLC_ReadCmd.Data_Addr[1] == '0'
		&& G_Delta_PLC_ReadCmd.Data_Addr[2] == '3'
		&& G_Delta_PLC_ReadCmd.Data_Addr[3] == '2')
	{
		if(G_SorterPlcDlg.GetSafeHwnd())
		{
			if(G_SorterPlcDlg.IsWindowVisible())
			{
				CWnd *w;
				CString s;
				unsigned int count;

				count = Hex2Char(&PLC_Read_D_Answer->Data[0]) * 256;
				count += Hex2Char(&PLC_Read_D_Answer->Data[2]) ;
				w = G_SorterPlcDlg.GetDlgItem(IDC_EDIT_1032);
				s.Format(_T("%X"),count);
				w->SetWindowTextW(s);

				//answer.Data[1] = '0';//BIT 3，BIT 2计数到报警，BIT 1高温分选报警，BIT 0高温出料报警，
				//answer.Data[2] = '0';//BIT 3高温检测报警，BIT 2高温进料报警，BIT 1低温接料报警，BIT 0低温分选报警，
				//answer.Data[3] = '0'; //BIT 3低温出料报警，BIT 2低温检测报警，BIT 1电流测试报警，BIT 0进料报警，
				if(G_SorterPlcDlg.m_ManualMode == 0)
				{
					if(count & 0x0001)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("进料报警  ");
					}
					else if(count & 0x0002)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("电流测试报警  ");
					}
					else if(count & 0x0004)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("低温检测报警  ");
					}
					else if(count & 0x0008)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("低温出料报警  ");
					}
					else if(count & 0x0010)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("低温分选报警  ");
					}
					else if(count & 0x0020)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("低温接料报警  ");
					}
					else if(count & 0x0040)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("高温进料报警  ");
					}
					else if(count & 0x0080)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("高温检测报警  ");
					}
					else if(count & 0x0100)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("高温出料报警  ");
					}
					else if(count & 0x0200)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("高温分选报警  ");
					}
					else if(count & 0x0400)
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("计数到报警  ");
					}
					else
					{
						G_SorterPlcDlg.m_AlarmMsg = _T("");
					}

					if(G_SorterPlcDlg.m_AlarmMsg.GetLength() > 0)
					{
						G_SorterPlcDlg.m_AlarmMsg += G_SorterPlcDlg.m_AlarmMsg;
						G_SorterPlcDlg.m_AlarmMsg += G_SorterPlcDlg.m_AlarmMsg;
						G_SorterPlcDlg.m_AlarmMsg += G_SorterPlcDlg.m_AlarmMsg;
					}
				}
				
				

			}
			
		}
		

	}
	else if(G_Delta_PLC_ReadCmd.Data_Addr[0] == '0'
		&& G_Delta_PLC_ReadCmd.Data_Addr[1] == '4'
		&& G_Delta_PLC_ReadCmd.Data_Addr[2] == '0'
		&& G_Delta_PLC_ReadCmd.Data_Addr[3] == '0')
	{//X stutas
		
		unsigned int X_status  = 0;
			
		X_status += Hex2Char(&PLC_Read_X_Answer->Data[6]) * 0x1000000;
		X_status += Hex2Char(&PLC_Read_X_Answer->Data[4]) *   0x10000;
		X_status += Hex2Char(&PLC_Read_X_Answer->Data[2]) *     0x100;
		X_status += Hex2Char(&PLC_Read_X_Answer->Data[0]) *       0x1;

		if(G_SorterPlcDlg.GetSafeHwnd())
		{
			if(G_SorterPlcDlg.IsWindowVisible())
			{
				G_SorterPlcDlg.Deal_X_Status(X_status);
			}
		}
	}
	else if(G_Delta_PLC_ReadCmd.Data_Addr[0] == '0'
		&& G_Delta_PLC_ReadCmd.Data_Addr[1] == '5'
		&& G_Delta_PLC_ReadCmd.Data_Addr[2] == '0'
		&& G_Delta_PLC_ReadCmd.Data_Addr[3] == '0')
	{//X stutas
		
		unsigned int Y_status  = 0;
			
		Y_status += Hex2Char(&PLC_Read_Y_Answer->Data[0]) *    0x1;
		Y_status += Hex2Char(&PLC_Read_Y_Answer->Data[2]) *  0x100;
	
		if(G_SorterPlcDlg.GetSafeHwnd())
		{
			if(G_SorterPlcDlg.IsWindowVisible())
			{
				G_SorterPlcDlg.Deal_Y_Status(Y_status);
				
				
			}
		}

	}
	else if(G_Delta_PLC_ReadCmd.Data_Addr[0] == '0'
		&& G_Delta_PLC_ReadCmd.Data_Addr[1] == 'E'
		&& G_Delta_PLC_ReadCmd.Data_Addr[2] == '7'
		&& G_Delta_PLC_ReadCmd.Data_Addr[3] == 'A')
	{
		if(G_SorterPlcDlg.GetSafeHwnd())
		{
			if(G_SorterPlcDlg.IsWindowVisible())
			{
				CWnd *w;
				int count;
				count = Hex2Char(&PLC_Read_C_Answer->Data[0]) * 256;
				count += Hex2Char(&PLC_Read_C_Answer->Data[2]) ;
				w = G_SorterPlcDlg.GetDlgItem(IDC_EDIT_LOW_LEVEL);
				s.Format(_T("%d"),count / 2);
				w->SetWindowTextW(s);


				count = Hex2Char(&PLC_Read_C_Answer->Data[8]) * 256;
				count += Hex2Char(&PLC_Read_C_Answer->Data[10]) ;
				w = G_SorterPlcDlg.GetDlgItem(IDC_EDIT_HIGH_LEVEL);
				s.Format(_T("%d"),count);
				w->SetWindowTextW(s);

				count = Hex2Char(&PLC_Read_C_Answer->Data[16]) * 256;
				count += Hex2Char(&PLC_Read_C_Answer->Data[18]) ;
				w = G_SorterPlcDlg.GetDlgItem(IDC_EDIT_MAIN_LEVEL);
				s.Format(_T("%d"),count);
				w->SetWindowTextW(s);
				
			}
		}
	}
	
	return;

	if(data_len == sizeof(DELTA_PLC_READ_CMD))
	{
		if(PLC_Read_Cmd->head.Cmd[1] == '2' || PLC_Read_Cmd->head.Cmd[1] == '3')
		{

			if(PLC_Read_Cmd->Data_Addr[0] == '1'
				&& PLC_Read_Cmd->Data_Addr[1] == '0'
				&& PLC_Read_Cmd->Data_Addr[2] == '3'
				&& PLC_Read_Cmd->Data_Addr[3] == '2')
			{
				char buf[15] = {0x3A,0x30,0x31,0x30,0x33,0x30,0x32,0x30,0x30,0x30,0x30,0x46,0x41,0x0D,0x0A};
				DELTA_PLC_READ_D_ANSWER answer;
				answer.head.STX = ':';
				answer.head.Addr[0] = PLC_Read_Cmd->head.Addr[0];
				answer.head.Addr[1] = PLC_Read_Cmd->head.Addr[1];
				answer.head.Cmd[0] =  PLC_Read_Cmd->head.Cmd[0];
				answer.head.Cmd[1] =  PLC_Read_Cmd->head.Cmd[1];
				answer.Data_Len[0] = '0';
				answer.Data_Len[1] = '2';
				answer.Data[0] = '0';
				answer.Data[1] = '0';//BIT 3，BIT 2计数到报警，BIT 1高温分选报警，BIT 0高温出料报警，
				answer.Data[2] = '0';//BIT 3高温检测报警，BIT 2高温进料报警，BIT 1低温接料报警，BIT 0低温分选报警，
				answer.Data[3] = '0'; //BIT 3低温出料报警，BIT 2低温检测报警，BIT 1电流测试报警，BIT 0进料报警，
				
				unsigned short lcr = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer.head.Addr[0],sizeof(DELTA_PLC_READ_D_ANSWER) - 5);

				char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

				if(lcr < 0x100)
				{
					answer.LCR[0] = TEXT[ lcr / 0x10];
					answer.LCR[1] = TEXT[ lcr % 0x10];

					answer.Tail[0]	=	0x0D;
					answer.Tail[1]	=	0x0A;

					comm->WriteComm((char*)&answer,sizeof(DELTA_PLC_READ_D_ANSWER));
					if(G_SorterPlcDlg.GetSafeHwnd())
					{
						if(G_SorterPlcDlg.IsWindowVisible())
						{
							G_SorterPlcDlg.DisplaySendCmd(answer.head.Cmd[1],(unsigned char*)&answer,sizeof(answer),0);
						}
					}
				}
			}
			else if(PLC_Read_Cmd->Data_Addr[0] == '0'
				&& PLC_Read_Cmd->Data_Addr[1] == '4'
				&& PLC_Read_Cmd->Data_Addr[2] == '0'
				&& PLC_Read_Cmd->Data_Addr[3] == '0')
			{
				DELTA_PLC_READ_X_ANSWER answer;
				answer.head.STX = ':';
				answer.head.Addr[0] = PLC_Read_Cmd->head.Addr[0];
				answer.head.Addr[1] = PLC_Read_Cmd->head.Addr[1];
				answer.head.Cmd[0] =  PLC_Read_Cmd->head.Cmd[0];
				answer.head.Cmd[1] =  PLC_Read_Cmd->head.Cmd[1];
				answer.Data_Len[0] = '0';
				answer.Data_Len[1] = '4';
				answer.Data[0] = '0';
				answer.Data[1] = '0';
				answer.Data[2] = '0';
				answer.Data[3] = '0';
				answer.Data[4] = '0';
				answer.Data[5] = '0';
				answer.Data[6] = '0';
				answer.Data[7] = '0';
				
				unsigned short lcr = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer.head.Addr[0],sizeof(DELTA_PLC_READ_X_ANSWER) - 5);

				char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

				if(lcr < 0x100)
				{
					answer.LCR[0] = TEXT[ lcr / 0x10];
					answer.LCR[1] = TEXT[ lcr % 0x10];

					answer.Tail[0]	=	0x0D;
					answer.Tail[1]	=	0x0A;

					comm->WriteComm((char*)&answer,sizeof(DELTA_PLC_READ_X_ANSWER));
					if(G_SorterPlcDlg.GetSafeHwnd())
					{
						if(G_SorterPlcDlg.IsWindowVisible())
						{
							G_SorterPlcDlg.DisplaySendCmd(answer.head.Cmd[1],(unsigned char*)&answer,sizeof(answer),0);
						}
					}
				}
			}
			else if(PLC_Read_Cmd->Data_Addr[0] == '0'
				&& PLC_Read_Cmd->Data_Addr[1] == 'E'
				&& PLC_Read_Cmd->Data_Addr[2] == '7'
				&& PLC_Read_Cmd->Data_Addr[3] == 'A')
			{
				DELTA_PLC_READ_C_ANSWER answer;
				//3A, 30 31, 30 33, 30 41, 30 46 42 36, 30 30 30 30, 30 37 38 38, 30 30 30 30 ,30 32 39 44 46 46 0D 0A
				answer.head.STX = ':';
				answer.head.Addr[0] = PLC_Read_Cmd->head.Addr[0];
				answer.head.Addr[1] = PLC_Read_Cmd->head.Addr[1];
				answer.head.Cmd[0] =  PLC_Read_Cmd->head.Cmd[0];
				answer.head.Cmd[1] =  PLC_Read_Cmd->head.Cmd[1];
				answer.Data_Len[0] = '0';
				answer.Data_Len[1] = 'A';
				answer.Data[0] = '0';	//低温
				answer.Data[1] = 'F';
				answer.Data[2] = 'B';
				answer.Data[3] = '6';

				answer.Data[4] = '0';
				answer.Data[5] = '0';
				answer.Data[6] = '0';
				answer.Data[7] = '0';

				answer.Data[8] = '0';	//高温数
				answer.Data[9] = '7';
				answer.Data[10] = '8';
				answer.Data[11] = '8';

				answer.Data[12] = '0';
				answer.Data[13] = '0';
				answer.Data[14] = '0';
				answer.Data[15] = '0';

				answer.Data[16] = '0'; //良品数
				answer.Data[17] = '2';
				answer.Data[18] = '9';
				answer.Data[19] = 'D';


				
				unsigned short lcr = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer.head.Addr[0],sizeof(DELTA_PLC_READ_C_ANSWER) - 5);

				char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

				if(lcr < 0x100)
				{
					answer.LCR[0] = TEXT[ lcr / 0x10];
					answer.LCR[1] = TEXT[ lcr % 0x10];

					answer.Tail[0]	=	0x0D;
					answer.Tail[1]	=	0x0A;

					G_RS485_Comm.WriteComm((char*)&answer,sizeof(DELTA_PLC_READ_C_ANSWER));
					if(G_SorterPlcDlg.GetSafeHwnd())
					{
						if(G_SorterPlcDlg.IsWindowVisible())
						{
							G_SorterPlcDlg.DisplaySendCmd(answer.head.Cmd[1],(unsigned char*)&answer,sizeof(answer),0);
						}
					}
				}
			}
			else if(PLC_Read_Cmd->Data_Addr[0] == '0'
				&& PLC_Read_Cmd->Data_Addr[1] == '8'
				&& PLC_Read_Cmd->Data_Addr[2] == '6'
				&& PLC_Read_Cmd->Data_Addr[3] == '0')
			{
				//3A 30 31 30 32 30 32 30 30 30 30 46 42 0D 0A
				DELTA_PLC_READ_M_ANSWER answer;
				answer.head.STX = ':';
				answer.head.Addr[0] = PLC_Read_Cmd->head.Addr[0];
				answer.head.Addr[1] = PLC_Read_Cmd->head.Addr[1];
				answer.head.Cmd[0] =  PLC_Read_Cmd->head.Cmd[0];
				answer.head.Cmd[1] =  PLC_Read_Cmd->head.Cmd[1];
				answer.Data_Len[0] = '0';
				answer.Data_Len[1] = '2';
				answer.Data[0] = '1'; //BIT 2为手动标志， BIT 3为复位动标志
				answer.Data[1] = '0';
				answer.Data[2] = '4';
				answer.Data[3] = '0';
				
				unsigned short lcr = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer.head.Addr[0],sizeof(DELTA_PLC_READ_M_ANSWER) - 5);

				char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

				if(lcr < 0x100)
				{
					answer.LCR[0] = TEXT[ lcr / 0x10];
					answer.LCR[1] = TEXT[ lcr % 0x10];

					answer.Tail[0]	=	0x0D;
					answer.Tail[1]	=	0x0A;

					comm->WriteComm((char*)&answer,sizeof(DELTA_PLC_READ_M_ANSWER));
					if(G_SorterPlcDlg.GetSafeHwnd())
					{
						if(G_SorterPlcDlg.IsWindowVisible())
						{
							G_SorterPlcDlg.DisplaySendCmd(answer.head.Cmd[1],(unsigned char*)&answer,sizeof(answer),0);
						}
					}
				}
			}
			else if(PLC_Read_Cmd->Data_Addr[0] == '0'
				&& PLC_Read_Cmd->Data_Addr[1] == '8'
				&& PLC_Read_Cmd->Data_Addr[2] == 'C'
				&& PLC_Read_Cmd->Data_Addr[3] == '0')
			{
				//3A 30 31 30 32 30 32 30 30 30 30 46 42 0D 0A
				DELTA_PLC_READ_X_ANSWER answer;
				answer.head.STX = ':';
				answer.head.Addr[0] = PLC_Read_Cmd->head.Addr[0];
				answer.head.Addr[1] = PLC_Read_Cmd->head.Addr[1];
				answer.head.Cmd[0] =  PLC_Read_Cmd->head.Cmd[0];
				answer.head.Cmd[1] =  PLC_Read_Cmd->head.Cmd[1];
				answer.Data_Len[0] = '0';
				answer.Data_Len[1] = '4';
				answer.Data[0] = '0'; //BIT 2为手动标志
				answer.Data[1] = '0';
				answer.Data[2] = '0';
				answer.Data[3] = '0';
				answer.Data[4] = '0'; 
				answer.Data[5] = '0';
				answer.Data[6] = '0';
				answer.Data[7] = '0';
				
				unsigned short lcr = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer.head.Addr[0],sizeof(DELTA_PLC_READ_X_ANSWER) - 5);

				char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

				if(lcr < 0x100)
				{
					answer.LCR[0] = TEXT[ lcr / 0x10];
					answer.LCR[1] = TEXT[ lcr % 0x10];

					answer.Tail[0]	=	0x0D;
					answer.Tail[1]	=	0x0A;

					comm->WriteComm((char*)&answer,sizeof(DELTA_PLC_READ_X_ANSWER));
					if(G_SorterPlcDlg.GetSafeHwnd())
					{
						if(G_SorterPlcDlg.IsWindowVisible())
						{
							G_SorterPlcDlg.DisplaySendCmd(answer.head.Cmd[1],(unsigned char*)&answer,sizeof(answer),0);
						}
					}
				}
			}
			else if(PLC_Read_Cmd->Data_Addr[0] == '0'
				&& PLC_Read_Cmd->Data_Addr[1] == '5'
				&& PLC_Read_Cmd->Data_Addr[2] == '0'
				&& PLC_Read_Cmd->Data_Addr[3] == '0')
			{
				//3A 30 31 30 32 30 32 30 30 30 30 46 42 0D 0A
				DELTA_PLC_READ_Y_ANSWER answer;
				answer.head.STX = ':';
				answer.head.Addr[0] = PLC_Read_Cmd->head.Addr[0];
				answer.head.Addr[1] = PLC_Read_Cmd->head.Addr[1];
				answer.head.Cmd[0] =  PLC_Read_Cmd->head.Cmd[0];
				answer.head.Cmd[1] =  PLC_Read_Cmd->head.Cmd[1];
				answer.Data_Len[0] = '0';
				answer.Data_Len[1] = '2';
				answer.Data[0] = '0'; //BIT 2为手动标志
				answer.Data[1] = '0';
				answer.Data[2] = '0';
				answer.Data[3] = '0';
				
				
				unsigned short lcr = G_RS485_Comm.Delta_Cal_LCR((unsigned char *)&answer.head.Addr[0],sizeof(DELTA_PLC_READ_Y_ANSWER) - 5);

				char TEXT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

				if(lcr < 0x100)
				{
					answer.LCR[0] = TEXT[ lcr / 0x10];
					answer.LCR[1] = TEXT[ lcr % 0x10];

					answer.Tail[0]	=	0x0D;
					answer.Tail[1]	=	0x0A;

					comm->WriteComm((char*)&answer,sizeof(DELTA_PLC_READ_Y_ANSWER));
					if(G_SorterPlcDlg.GetSafeHwnd())
					{
						if(G_SorterPlcDlg.IsWindowVisible())
						{
							G_SorterPlcDlg.DisplaySendCmd(answer.head.Cmd[1],(unsigned char*)&answer,sizeof(answer),0);
						}
					}
				}
			}
			else 
			{//
				AfxMessageBox(_T("ERROR"));
			}
		}
		else if(sizeof(DELTA_PLC_SET_CMD) == data_len)
		{
		
		}



		return ;
	}

#endif	

}

LRESULT DealOnComm(WPARAM wParam,LPARAM lParam,CWnd * ParentWnd)
{
#ifdef _HeatBox
#ifdef WINCE
	CWelcomeDlg* MainWnd = (CWelcomeDlg *) ParentWnd;
#else
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg *) ParentWnd;
#endif
#else
	CMainDlg* MainWnd = (CMainDlg *) ParentWnd;
#endif

	int i,BoxNr,data_len;
	CTime t;
	t=t.GetCurrentTime(); 
	unsigned char rec_cmd[1024];

	

	float temp[4];
	short para[4];
	short short_a;

	memset(rec_cmd,0,sizeof(rec_cmd));
	memcpy(rec_cmd,(char*)wParam,lParam);
	data_len = lParam;

	CString s,s1;

#ifndef WINCE
	if(::G_NormalConfigPara[0].BoxType == BOX_DELTA_PLC_8PT100_8SSR)
	{
		DealDeltaPLC(rec_cmd, data_len );
		return 0;
	}
#endif

	MODBUS_RS485_HEADER						*head;

	MODBUS_RS485_32TEMP_ANSWER				*Answer_32TEMP			=	(MODBUS_RS485_32TEMP_ANSWER *)rec_cmd;

	MODBUS_RS485_8TEMP_8SSR_ANSWER			*Answer_8TEMP_8SSR		=	(MODBUS_RS485_8TEMP_8SSR_ANSWER *)rec_cmd;

	MODBUS_RS485_CMD_PARA					*Para_Answer			=	(MODBUS_RS485_CMD_PARA *)rec_cmd;

	MODBUS_RS485_CMD_ARM_128				*Arm_Answer				=	(MODBUS_RS485_CMD_ARM_128 *)rec_cmd;
	MODBUS_RS485_20TEMP_5SSR_ANSWER			*Temp20_5SSR			=	(MODBUS_RS485_20TEMP_5SSR_ANSWER *)rec_cmd;
	MODBUS_RS485_NTC_AD_VAL_ANSWER			*NTC_Answer				=	(MODBUS_RS485_NTC_AD_VAL_ANSWER *)rec_cmd;

	ASCII_RS485_COMMAND_READ_TEMP			*ascii_cmd				=	(ASCII_RS485_COMMAND_READ_TEMP *) rec_cmd;

	MODBUS_RS485_DISC_ANSWER				*DiscAnswer				=	(MODBUS_RS485_DISC_ANSWER *)rec_cmd;
	MODBUS_RS485_DISC_24_ANSWER				*Disc24_Answer			=	(MODBUS_RS485_DISC_24_ANSWER *)rec_cmd;
	MODBUS_RS485_CMD_128					*modbus_cmd_128			=	(MODBUS_RS485_CMD_128*) rec_cmd;
	MODBUS_RS485_STATUS_FLASH_ANSWER_128	* modbus_status_cmd_128	=	(MODBUS_RS485_STATUS_FLASH_ANSWER_128*) rec_cmd;
	MODBUS_RS485_ANSWER_300					*modbus_answer_300		=	(MODBUS_RS485_ANSWER_300*) rec_cmd;
	
	MODBUS_RS485_CMD_192					*modbus_cmd_192			=	(MODBUS_RS485_CMD_192*) rec_cmd;
	MODBUS_RS485_STATUS_FLASH_ANSWER_192	* modbus_status_cmd_192	=	(MODBUS_RS485_STATUS_FLASH_ANSWER_192*) rec_cmd;

	MODBUS_RS485_TICK_COUNT_ANSWER			*modbus_cmd_tick		=	(MODBUS_RS485_TICK_COUNT_ANSWER*)rec_cmd;

	unsigned char addr ;

	

	if(G_CommProtocolType == COMM_PROTOCOL_ASC)
	{
		unsigned char chksum;

		chksum = CalCheckSum((char*)&rec_cmd[1], lParam-4);
		
		if(	((chksum / 16 +'0') != rec_cmd[lParam-2])
			|| ((chksum % 16 +'0') != rec_cmd[lParam-3]))
		{
			return 0;
		}
		
		addr  = ascii_cmd->header.BoxNr;
	}
	else if(G_CommProtocolType == COMM_PROTOCOL_DELTA_FREQUENCE_ASC)
	{
		if(m_SendCmdDeltaFrequnce.GetCount() > 0)
		{
			m_SendCmdDeltaFrequnce.RemoveAt(0); 
		}
		DELTA_FREQUNCE_ANSWER_ASC    *answer = (DELTA_FREQUNCE_ANSWER_ASC*) rec_cmd;
		
		MainWnd->DisplayReceiveCmd(0,(unsigned char*)rec_cmd,sizeof(DELTA_FREQUNCE_ANSWER_ASC),0);

	//	m_DebugDlg[0].UpdateData(false);
	}
	else 
	{
		addr = modbus_cmd_128->head.RS485_Addr;

		if((data_len != sizeof(MODBUS_RS485_CMD_128))
			&& (data_len != sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128))
			&& (data_len != sizeof(MODBUS_RS485_CMD_192))
			&& (data_len != sizeof(MODBUS_RS485_CMD_300))
			&& (data_len != sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_192))
			&& (data_len != sizeof(MODBUS_RS485_TICK_COUNT_ANSWER))
			&& (data_len != sizeof(MODBUS_RS485_CMD_PROGRAM))
			&& (data_len != sizeof(MODBUS_RS485_CMD_ARM_128))
			&& (data_len != sizeof(MODBUS_RS485_DISC_ANSWER))
			&& (data_len != sizeof(MODBUS_RS485_DISC_24_ANSWER))
			&& (data_len != sizeof(MODBUS_RS485_NTC_AD_VAL_ANSWER))
			&& (data_len != sizeof(MODBUS_RS485_ANSWER_300))
			&& (data_len != sizeof(MODBUS_RS485_20TEMP_5SSR_ANSWER))
			&& (data_len != sizeof(MODBUS_RS485_CMD_PARA))
			&& (data_len != sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER))
			&& (data_len != sizeof(MODBUS_RS485_32TEMP_ANSWER))
			)
		{
			return 0;
		}
	}
	

	if(addr >= '1' && addr <= '9')
	{
		BoxNr = addr - '1';
	}
	else if(addr >= 'A' && addr <= ('A' + 8))
	{
		BoxNr = addr - 'A';
	}
	else
	{
		return 0;
	}
	
	
#ifdef _HeatBox	
#ifndef WINCE
	
	CWnd * w;
	if(::G_NormalConfigPara[0].BoxType ==  BOX_DISC_24
		|| ::G_NormalConfigPara[0].BoxType ==  BOX_DISC_40)
	{
		w = MainWnd->m_DebugDlg[0].GetDlgItem(IDC_EDIT_RECEIVE_CMD_COUNT);
		MainWnd->m_DebugDlg[0].m_ReceiveCmdCount++;
		s.Format(_T("%d"), MainWnd->m_DebugDlg[0].m_SendCmdCount - MainWnd->m_DebugDlg[0].m_ReceiveCmdCount);
		w->SetWindowTextW(s); 
		MainWnd->m_DebugDlg[BoxNr].UpdateData(false);
	}
	else
	{
		w = MainWnd->m_DebugDlg[BoxNr].GetDlgItem(IDC_EDIT_RECEIVE_CMD_COUNT);
		MainWnd->m_DebugDlg[BoxNr].m_ReceiveCmdCount++;
		s.Format(_T("%d"), MainWnd->m_DebugDlg[BoxNr].m_SendCmdCount - MainWnd->m_DebugDlg[BoxNr].m_ReceiveCmdCount);
		w->SetWindowTextW(s);
		MainWnd->m_DebugDlg[BoxNr].UpdateData(false);
	}
#endif	
#else
	
#endif
	
	unsigned char cmd;
	if(G_IsJustCmdAscii == true)
	{
		cmd = ascii_cmd->header.command;
	}
	else
	{
		switch(data_len)
		{
		case sizeof(MODBUS_RS485_32TEMP_ANSWER):
			cmd = Answer_32TEMP->head.cmd;
			head = &Answer_32TEMP->head;
			break;
		case sizeof(MODBUS_RS485_CMD_128):
			cmd = modbus_cmd_128->head.cmd;
			head = &modbus_cmd_128->head;
			break;
		case sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128):
			cmd = modbus_status_cmd_128->head.cmd;
			head = &modbus_status_cmd_128->head;
			break;
		case  sizeof(MODBUS_RS485_CMD_192):
			cmd = modbus_cmd_192->head.cmd;
			head = &modbus_cmd_192->head;
			break;
		case sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_192):
			cmd = modbus_status_cmd_192->head.cmd;
			head = &modbus_status_cmd_192->head;
			break;
		case sizeof(MODBUS_RS485_TICK_COUNT_ANSWER):
			cmd = modbus_cmd_tick->head.cmd;
			head = &modbus_cmd_tick->head;
			break;
		case sizeof(MODBUS_RS485_CMD_PROGRAM):

			break;
		case sizeof(MODBUS_RS485_DISC_ANSWER):
			cmd = DiscAnswer->head.cmd;
			head = &DiscAnswer->head;
			break;
		case sizeof(MODBUS_RS485_NTC_AD_VAL_ANSWER):
			cmd = NTC_Answer->head.cmd;
			head = &NTC_Answer->head;
			break;
		case sizeof(MODBUS_RS485_ANSWER_300):
			cmd = modbus_answer_300->head.cmd;
			head = &modbus_answer_300->head;
			break;
		case sizeof(MODBUS_RS485_CMD_PARA):
			cmd = Para_Answer->head.cmd;
			head = &Para_Answer->head;
			break;
		default:
			cmd = modbus_cmd_128->head.cmd;
			head = &modbus_cmd_128->head;
			break;
		}
	}

	if(G_IsJustCmdAscii == true)
	{
		if(ascii_cmd->header.command != CMD_READ_TEMP)
		{
#ifdef _HeatBox				
			if(G_iMaxLedNr[BoxNr] == 192)
			{
				if(m_CmdArray_192[BoxNr].GetSize() > 0)
				{
					m_CmdArray_192[BoxNr].RemoveAt(0); 
				}
			}
			else
#endif
			{
				if(m_CmdArray_128[BoxNr].GetSize() > 0)
				{
					m_CmdArray_128[BoxNr].RemoveAt(0); 
				}
			}
		}
		MainWnd->DisplayReceiveCmd(ascii_cmd->header.command,(unsigned char*)ascii_cmd,sizeof(ASCII_RS485_COMMAND_READ_TEMP),BoxNr);

	}
	else //MODBUS
	{
		if(head->cmd  != CMD_READ_STATUS
			&&  head->cmd  != CMD_READ_TEMP
			&& head->cmd  != CMD_READ_DISC_STATUS)
		
		{
			{
				char buf[100];
				sprintf(buf,"接收: %02X",modbus_cmd_128->head.cmd);
				WriteLogFile(buf,0);
			}
#ifdef _HeatBox		
			if(G_iMaxLedNr[BoxNr] == 300)
			{
				if(m_CmdArray_300[BoxNr].GetSize() > 0)
				{
					m_CmdArray_300[BoxNr].RemoveAt(0); 
				}
			}
			else if(G_iMaxLedNr[BoxNr] == 192)
			{
				if(m_CmdArray_192[BoxNr].GetSize() > 0)
				{
					m_CmdArray_192[BoxNr].RemoveAt(0); 
				}
			}
			else
#endif
			{
				if(::G_NormalConfigPara[0].BoxType == BOX_SENSATA_32PT100)
				{
					if(m_CmdArray_32PT100[BoxNr].GetSize() > 0)
					{
						
						m_CmdArray_32PT100[BoxNr].RemoveAt(0); 
					}
				}
				else
				{
					if(m_CmdArray_128[BoxNr].GetSize() > 0)
					{
						
						m_CmdArray_128[BoxNr].RemoveAt(0); 
					}
				}
			}
		}
		
		if(modbus_cmd_128->head.cmd == CMD_PROGRAM_DATA)
		{
#ifdef _HeatBox	
			//for(int i = 0; i < ::G_CurMaxHeatBox; i++)
			{
				MODBUS_RS485_CMD_128 *CMD_PROGRAM_cmd = (MODBUS_RS485_CMD_128 *)rec_cmd;
				//if(CMD_PROGRAM_cmd->para[0] == 0)
				{
					::G_RS485_Comm.OSCCAL = CMD_PROGRAM_cmd->para[1]; //获得线路板的OSC,加密用，防止抄板
				}
				MainWnd->DisplayReceiveCmd(cmd,(unsigned char*)rec_cmd,data_len,G_ProgramBoxNr);
			}
#else
			MainWnd->DisplayReceiveCmd(cmd,(unsigned char*)rec_cmd,data_len,0);
#endif
		}
		else
		{
#ifdef _HeatBox	
			if(::G_NormalConfigPara[0].BoxType ==  BOX_DISC_24
				|| ::G_NormalConfigPara[0].BoxType ==  BOX_DISC_40
				|| ::G_NormalConfigPara[0].BoxType ==  BOX_DISC_16)
			{
				MainWnd->DisplayReceiveCmd(cmd,(unsigned char*)rec_cmd,data_len,0);
			}
			else
#endif
			{
				MainWnd->DisplayReceiveCmd(cmd,(unsigned char*)rec_cmd,data_len,BoxNr);
			}
		}
	}


//	s.Format(_T("cmd = 0x%02x"),cmd);
//	AfxMessageBox(s);


	switch(head->cmd)
	{

	case CMD_SET_PCB_TYPE_192_INTERFACE:

		ProgramPara(data_len,BoxNr,Para_Answer);

		if(NEW_192_INTERFACE_PCB == modbus_cmd_192->para[0])
		{
			AfxMessageBox(_T("192点夹具板类型\r\n新线路板\r\n设置成功"));
		}
		else
		{
			AfxMessageBox(_T("192点夹具板类型\r\n老线路板\r\n设置成功"));
		}
		break;
	case CMD_READ_DISC_STATUS:
#ifndef _20PT100_5SSR
#ifdef WINCE
		G_HeatBoxDlg.DealCmdAnswer(rec_cmd,0,0,data_len);
		G_DebugDlg.DealCmdAnswer(CMD_READ_DISC_STATUS,data_len,rec_cmd,0,0); 

#else
		MainWnd->m_SwitchDlg[0].DealCmdAnswer(rec_cmd,0,0,data_len);
		MainWnd->m_DebugDlg[0].DealCmdAnswer(CMD_READ_DISC_STATUS,data_len,rec_cmd,0,0); 
#endif
#endif
		break;
	case CMD_WRITE_TM7705_OSC:
		AfxMessageBox(_T("TM7705晶振设置成功"));
		break;
	case CMD_WRITE_BOARD_TYPE:
		
		ProgramPara(data_len,BoxNr,Para_Answer);

		AfxMessageBox(_T("接口板设置成功"));
		break;
	case CMD_READ_TICK_COUNT:
		
		break;
	case CMD_WRITE_USEFUL_SENSORS:

		ProgramPara(data_len,BoxNr,Para_Answer);

		{
			AfxMessageBox(_T("传感器数量设置成功"));
		}
		break;
	case CMD_AT:
		AfxMessageBox(_T("开始自整定"));
		break;
	case CMD_STM32_UNIQUE_ID:
		{
			for(int i = 0; i < sizeof(STM32_ID)/sizeof(STM32_ID[0]); i++)
			{
				if(STM32_ID[BoxNr][0] == modbus_cmd_128->para[0]
				&& STM32_ID[BoxNr][1] == modbus_cmd_128->para[1]
				&& STM32_ID[BoxNr][2] == modbus_cmd_128->para[2]
				&& STM32_ID[BoxNr][3] == modbus_cmd_128->para[3]
				&& STM32_ID[BoxNr][4] == modbus_cmd_128->para[4]
				&& STM32_ID[BoxNr][5] == modbus_cmd_128->para[5]
				&& STM32_ID[BoxNr][6] == modbus_cmd_128->para[6]
				&& STM32_ID[BoxNr][7] == modbus_cmd_128->para[7]
				&& STM32_ID[BoxNr][8] == modbus_cmd_128->para[8]
				&& STM32_ID[BoxNr][9] == modbus_cmd_128->para[9]
				&& STM32_ID[BoxNr][10] == modbus_cmd_128->para[10]
				&& STM32_ID[BoxNr][11] == modbus_cmd_128->para[11])
				{
					STM32_CopyRight[BoxNr] = 1;
				}

				if(STM32_CopyRight[BoxNr] == 0)
				{
					AfxMessageBox(_T("版权所有，盗版必究!!!"));
				}
				

			}
		}
		break;
	case CMD_SET_ADDR:

		ProgramPara(data_len,BoxNr,Para_Answer);


		AfxMessageBox(_T("编号设置成功"));
	
		break;

	case CMD_PROGRAM_START:
		AfxMessageBox(_T("CMD_PROGRAM_START"));
		if(G_SendmCmdProgram.GetSize() > 0) 
		{
		//	::G_ProgramStarted = 1;
		}
		break;
	case CMD_PROGRAM_DATA:
		if(G_SendmCmdProgram.GetSize() > 0) 
		{
			G_SendmCmdProgram.RemoveAt(0); 
		}
		else
		{
			G_SendmCmdProgram_ARM.RemoveAt(0);
		}
		if(G_SendmCmdProgram.GetSize() == 0 && G_SendmCmdProgram_ARM.GetSize() == 0) 
		{
#ifdef _HeatBox
#ifndef WINCE
			MainWnd->SetTimer(1,G_NormalConfigPara[0].CommTimer,NULL);
#else
			if(G_RS485_Comm.IsOmronMeter(::G_NormalConfigPara[0].BoxType))
			{
				MainWnd->SetTimer(1,500,NULL); 
			}
			else if(::G_NormalConfigPara[0].BoxType == BOX_DISC_40 
				|| G_NormalConfigPara[0].BoxType == BOX_DISC_16 
				|| ::G_NormalConfigPara[0].BoxType == BOX_DISC_24 
				||  ::G_NormalConfigPara[0].BoxType == BOX_DISC_FLAT) 
			{
				MainWnd->SetTimer(1,300,NULL); 
			}
			else
			{
				MainWnd->SetTimer(1,500,NULL); 
			}
#endif
			CWnd *w;

		//	for(int i = 0; i < ::G_CurMaxHeatBox; i++)
			{
#ifdef WINCE
				w = G_DebugDlg.GetDlgItem(IDC_BUTTON_PROGRAM);
#else
				w= MainWnd->m_DebugDlg[G_ProgramBoxNr].GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE);
#endif
				w->SetWindowTextW(_T("更新固件"));
			}	
			if(G_SpeechArray.GetSize() < 10)
			{
				::G_SpeechArray.Add (_T("程序更新结束"));
			}
			
			if(G_NormalConfigPara[0].bSpeech == false )
			{
				AfxMessageBox(_T("程序更新结束"));

			}

			if(::G_NormalConfigPara[G_ProgramBoxNr].BoxType != BOX_DISC_24 
				&& G_NormalConfigPara[G_ProgramBoxNr].BoxType != BOX_DISC_40
				&& G_NormalConfigPara[G_ProgramBoxNr].BoxType != BOX_DISC_16)
			{
				//G_RS485_Comm.SendPcbBoardType(G_ProgramBoxNr);
			}

#else
			AfxMessageBox(_T("程序更新结束"));
#endif
		}
		else
		{
			int i = G_SendmCmdProgram.GetSize();
			if(i == 0)
			{
				i = G_SendmCmdProgram_ARM.GetSize();
			}
			if(((i % 10) == 0) && (i != 0))
			{
				s.Format(_T("%d"),i);
#ifdef _HeatBox	
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (s);
				}
#endif
			}
			s.Format(_T("更新..%d"),i);
#ifdef _HeatBox			
			CWnd *w;
			//for(int i = 0; i < ::G_CurMaxHeatBox; i++)
			{
				  
#ifdef WINCE
				w = G_DebugDlg.GetDlgItem(IDC_BUTTON_PROGRAM); 
				w->SetWindowTextW(s);
#else
				w= MainWnd->m_DebugDlg[G_ProgramBoxNr].GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE); 
				w->SetWindowTextW(s);
#endif
			}	
#endif
			 
		}
		//AfxMessageBox(_T("CMD_PROGRAM_DATA"));
		break;
//	case CMD_PROGRAM_BOOT_START:
//		if(this->G_SendmCmdProgram[BoxNr].GetSize() > 0) 
//		{
//			G_SendmCmdProgram[BoxNr].RemoveAt(0); 
//		}
//		//AfxMessageBox(_T("CMD_PROGRAM_START"));
//		break;
//	case CMD_PROGRAM_BOOT_DATA:
//		if(this->G_SendmCmdProgram[BoxNr].GetSize() > 0) 
//		{
//			G_SendmCmdProgram[BoxNr].RemoveAt(0); 
//		}
//		if(this->G_SendmCmdProgram[BoxNr].GetSize() == 0) 
//		{
//			AfxMessageBox(_T("程序更新结束"));
//		}
//		//AfxMessageBox(_T("CMD_PROGRAM_DATA"));
//		break;
//
//	case CMD_PROGRAM_END:
//		if(this->G_SendmCmdProgram[BoxNr].GetSize() > 0) 
//		{
//			G_SendmCmdProgram[BoxNr].RemoveAt(0); 
//		}
//		//AfxMessageBox(_T("程序更新结束"));
//		break;
//
	case CMD_PID_COOL:
		WriteLogFile("CMD_PID_COOL",BoxNr+1);
		break;
	case CMD_STEP_PID_COOL:
		WriteLogFile("CMD_STEP_DOWN_PID_OUT",BoxNr+1);
		break;
//		
//	case CMD_AIR_CTL:
//		{
//			
//			if(modbus_cmd_128->para[0] == 0)
//			{ //表示空调开
//				
//#ifdef WINCE
//				::G_HeatBoxDlg.m_TempPara.OutputStatus &=~(1 << METER_OUTPUT_STATUS_BIT_AIR);
//				
//				G_HeatBoxDlg.Draw_PV_SV(); 
//
//#else
//				MainWnd->m_SwitchDlg[BoxNr].m_TempPara.OutputStatus |= (1 << METER_OUTPUT_STATUS_BIT_AIR);
//				MainWnd->m_SwitchDlg[BoxNr].m_Meter.m_IsAcOn = false;
//#endif
//			}
//			else
//			{
//				
//#ifdef WINCE
//				G_HeatBoxDlg.m_TempPara.OutputStatus |= (1 << METER_OUTPUT_STATUS_BIT_AIR);
//
//				G_HeatBoxDlg.Draw_PV_SV(); 
//				//AfxMessageBox(_T("CMD_AIR_CTL ON"));
//#else
//				MainWnd->m_SwitchDlg[BoxNr].m_TempPara.OutputStatus &=~(1 << METER_OUTPUT_STATUS_BIT_AIR);
//				MainWnd->m_SwitchDlg[BoxNr].m_Meter.m_IsAcOn = true;
//#endif
//			}
//			WriteLogFile("CMD_SET_COOL_RELAY",BoxNr+1);
//		}
//		break;
//		
	case CMD_SET_SWITCH_NORMAL_MODE:
		{
			WriteLogFile("CMD_SET_SWITCH_NORMAL_MODE",BoxNr+1);
		}
		break;
//#ifndef _ASCII_
	case CMD_WRITE_STEP_PID_COUNT:
		{
#ifdef _HeatBox
			int *count;
			char s[100];

			if(G_iMaxLedNr[BoxNr] == 192)
			{
				 count =  (int*)modbus_cmd_192->para;
			}
			else
			{
				count =  (int*)modbus_cmd_128->para;
			}
#ifdef WINCE
			sprintf(s,"CMD_WRITE_STEP_PID_COUNT 使用 = %d", *count);
#else
			sprintf_s(s,"CMD_WRITE_STEP_PID_COUNT 使用 = %d", *count);
#endif
			WriteLogFile(s,BoxNr+1);
#endif
		}
		break;
//#endif
//#ifndef _ASCII_
	case CMD_READ_STEP_PID_COUNT:
		
		{
#ifdef _HeatBox
			int *count;
			char s[100];
			if(G_iMaxLedNr[BoxNr] == 192)
			{
				 count =  (int*)modbus_cmd_192->para;
			}
			else
			{
				count =  (int*)modbus_cmd_128->para;
			}
#ifdef WINCE
			sprintf(s,"CMD_READ_STEP_PID_COUNT = %d", *count);
#else
			sprintf_s(s,"CMD_READ_STEP_PID_COUNT = %d", *count);
#endif
			WriteLogFile(s,BoxNr+1);
#endif
		}
		break;
//#endif
    case CMD_AUTO_TEST:
		{
#ifdef _HeatBox
			//AfxMessageBox(_T("CMD_AUTO_TEST"));
#ifndef WINCE 
			char buf[100];
			sprintf_s(buf,"CMD_AUTO_TEST %5.1f",MainWnd->m_SwitchDlg[BoxNr].m_PidTemp);  
			WriteLogFile(buf,BoxNr+1);
#endif
#endif
		}
		break;
//		
//	case CMD_SET_RELAY:
//		{
//			if(modbus_cmd_128->para[0] == 0)
//			{
//#ifdef WINCE
//				::G_HeatBoxDlg.m_Meter.m_IsDoorOpened = true;  
//#else
//				MainWnd->m_SwitchDlg[BoxNr].m_Meter.m_IsDoorOpened = true;
//#endif
//			}
//			else
//			{
//#ifdef WINCE
//				::G_HeatBoxDlg.m_Meter.m_IsDoorOpened = false; 
//#else
//				MainWnd->m_SwitchDlg[BoxNr].m_Meter.m_IsDoorOpened = false;
//#endif
//
//			}
//		}
//		break;
//		
//#ifndef _ASCII_
	case CMD_CPLD_RESET:

		WriteLogFile("CMD_CPLD_RESET",BoxNr+1);

		break;
//#endif


	case CMD_SET_ADJUST_ALL: //	case CMD_SET_DISPLAY_TYPE:

	ProgramPara(data_len,BoxNr,Para_Answer);

#ifdef _HeatBox
#ifndef WINCE
		MainWnd->KillTimer(1);
		MainWnd->SetTimer(1,::G_NormalConfigPara[0].CommTimer ,NULL); 
		if(addr >= 'A')
		{
			WriteLogFile("CMD_SET_DISPLAY_TYPE",BoxNr+1);
		}
		else
		{
			WriteLogFile("CMD_SET_ADJUST_ALL",BoxNr+1);
		}
#endif	
#endif
		break;

//#ifndef _ASCII_
	case CMD_READ_ADJUST_ALL:
#ifdef _HeatBox		
#ifdef WINCE
		G_DebugDlg.DealCmdAnswer( CMD_READ_ADJUST_ALL,data_len,rec_cmd,0,G_IsJustCmdAscii);
	
		if(::G_SorterTempDlg.GetSafeHwnd() && G_SorterTempDlg.IsWindowVisible())
		{
			::G_SorterTempDlg.DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
		}

		if(::G_SorterPlcDlg.GetSafeHwnd() && G_SorterPlcDlg.IsWindowVisible())
		{
			::G_SorterPlcDlg.DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
		}

#else
		WriteLogFile("CMD_READ_ADJUST_ALL",BoxNr+1);
		MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_READ_ADJUST_ALL,data_len,rec_cmd,0,G_IsJustCmdAscii);
#endif
#endif
		break;
//#endif
//#ifndef _ASCII_
	case CMD_SET_FLASH_TEST_SPEED:
		WriteLogFile("CMD_SET_FLASH_TEST_SPEED",BoxNr+1);
		break;
//#endif
//#ifndef _ASCII_
	case CMD_SET_HIGH_VOLT:
		WriteLogFile("CMD_SET_HIGH_VOLT",BoxNr+1);
		AfxMessageBox(_T("设置高压"));
		break;
//#endif

//#ifndef _ASCII_
	case CMD_READ_STATUS:
		

#ifdef _HeatBox
		if(G_NormalConfigPara[BoxNr].BoxType == BOX_NTC)
		{
			if(data_len == sizeof(MODBUS_RS485_NTC_AD_VAL_ANSWER))
			{
#ifdef WINCE
				G_HeatBoxDlg.DealCmdAnswer(rec_cmd,0,G_IsJustCmdAscii,data_len);
				G_DebugDlg.DealCmdAnswer( CMD_READ_STATUS,data_len,rec_cmd);
#else
				MainWnd->m_SwitchDlg[BoxNr].DealCmdAnswer(rec_cmd,0,G_IsJustCmdAscii,data_len);
				MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_READ_STATUS,data_len,rec_cmd,0,G_IsJustCmdAscii);
#endif
			}
		}
		else
		{
			if(data_len == (2 + 1 + G_iMaxLedNr[BoxNr]/8 * 2 + 2))
			{
				//192点灯板
#ifndef WINCE
				memcpy(MainWnd->m_SwitchStatus[BoxNr], modbus_cmd_192->para,G_iMaxLedNr[BoxNr]/8);
#endif	

#ifdef WINCE
				G_HeatBoxDlg.DealCmdAnswer(rec_cmd,0,G_IsJustCmdAscii,data_len);
				G_DebugDlg.DealCmdAnswer( CMD_READ_STATUS,data_len,rec_cmd);
#else
				MainWnd->m_SwitchDlg[BoxNr].DealCmdAnswer(rec_cmd,0,G_IsJustCmdAscii,data_len);
				MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_READ_STATUS,data_len,rec_cmd,0,G_IsJustCmdAscii);
#endif
				if(G_TakeoutDlg.GetSafeHwnd() != NULL)
				{
					if(G_TakeoutDlg.IsWindowVisible())
					{
						::G_TakeoutDlg.ShowTestResult(true);
					}
				}
			}
			else
			{
#ifndef WINCE
				char s[100];
				sprintf_s(s,"CMD_READ_FPGA_STATUS datalen = %d\r\n", data_len);
				WriteLogFile(s,BoxNr+1);
#endif
			}
		}
#endif
		break;

	case CMD_SET_BEEP:
		G_RS485_Comm.m_BeepStartTime[BoxNr] = CTime::GetCurrentTime();
	case CMD_SET_RELAY:
		if(::G_NormalConfigPara[0].BoxType == BOX_8PT100_8SSR)
		{
			break;
		}

	case CMD_AIR_CTL:
	case CMD_READ_TEMP:

		{

#ifndef _HeatBox

			unsigned char HEAD[4] = {0xAA,0xAA,0xAA,0xAA};
			unsigned char TAIL[4] = {0xBB,0xBB,0xBB,0xBB};
			unsigned short CheckSum = 0;
			typedef struct
			{
				unsigned char HEAD[4];
				unsigned short Temp[40];
				unsigned short CheckSum;
				unsigned char TAIL[4];

			}SSR_ANSWER_TO_PLC;

			SSR_ANSWER_TO_PLC answer_to_plc;
			memset(&answer_to_plc,0,sizeof(answer_to_plc));

			answer_to_plc.HEAD[0] = 0xAA;
			answer_to_plc.HEAD[1] = 0xAA;
			answer_to_plc.HEAD[2] = 0xAA;
			answer_to_plc.HEAD[3] = 0xAA;

			answer_to_plc.TAIL[0] = 0xBB;
			answer_to_plc.TAIL[1] = 0xBB;
			answer_to_plc.TAIL[2] = 0xBB;
			answer_to_plc.TAIL[3] = 0xBB;

			
			
			if(BoxNr == 0)
			{
				for(int i = 0; i < 20; i++)
				{
					answer_to_plc.Temp[i] = (Temp20_5SSR->para.Temp[i] / 100.0f + G_AdjustTemp[0][0][i]) * 100.0f;
				}
			}
			else
			{
				for(int i = 0; i < 20; i++)
				{
					answer_to_plc.Temp[i+ 20] = (Temp20_5SSR->para.Temp[i] / 100.0f + G_AdjustTemp[0][0][i+20]) * 100.0f;
				}
			}
			for(int i = 0; i < sizeof(answer_to_plc.Temp); i++)
			{
				answer_to_plc.CheckSum += ((unsigned char*)answer_to_plc.Temp)[i];
			}
			
				G_PrintRS232_Comm.WriteComm((char*)&answer_to_plc,sizeof(answer_to_plc));
				
				MainWnd->DisplaySendCmd_PLC(0,(unsigned char*)&answer_to_plc,sizeof(answer_to_plc),BoxNr);

				

			
			if(BoxNr == 0)
			{
				if(G_20Temp5SsrAnswerArray[0].GetSize() > 10000)
				{
					G_20Temp5SsrAnswerArray[0].RemoveAt(0,100);
				}

				
				G_20Temp5SsrAnswerArray[0].Add(*Temp20_5SSR);
				
				for(int i = 0; i < 20; i++)
				{
					if(Temp20_5SSR->para.Temp[i] != 0)
					{
						if(Temp20_5SSR->para.Temp[i]/ 100.0f > 200.0f)
						{
							MainWnd->m_Temp[i] = Temp20_5SSR->para.Temp[i]/ 100.0f;
						}
						else
						{
							MainWnd->m_Temp[i] = Temp20_5SSR->para.Temp[i]/ 100.0f + G_AdjustTemp[0][0][i];
						}
					}
				//	MainWnd->m_Temp[i] = (unsigned short)Temp20_5SSR->para.Temp[i];
				}
				MainWnd->m_PV1 = MainWnd->m_Temp[19];
				MainWnd->m_SV1 = Temp20_5SSR->para.Sv / 10.0f;
				MainWnd->m_HeatPercent = Temp20_5SSR->para.HeatPercent;
				MainWnd->UpdateData(false);
			
			}
			else
			{
				int UsedPT100 = 0;
				float Pv = 0;
				if(G_20Temp5SsrAnswerArray[1].GetSize() > 10000)
				{
					G_20Temp5SsrAnswerArray[1].RemoveAt(0,100);
				}
				//if(HasZero == 0)
				{
					G_20Temp5SsrAnswerArray[1].Add(*Temp20_5SSR);
				}

				for(int i = 0; i < 20; i++)
				{
					if(Temp20_5SSR->para.Temp[i] != 0)
					{
						MainWnd->m_Temp[i+20] = Temp20_5SSR->para.Temp[i]/ 100.0f  + G_AdjustTemp[0][0][i+20];
						if(::G_NormalConfigPara[0].UsedPT100[1][i/8] & (1 << (i % 8)) )
						{
							UsedPT100++;
							Pv += MainWnd->m_Temp[i+20];
						}
					}
				}
				MainWnd->m_PV2 = MainWnd->m_Temp[39];
				MainWnd->m_SV2 = Temp20_5SSR->para.Sv / 10.0f;
				MainWnd->m_HeatPercent2 = Temp20_5SSR->para.HeatPercent;
				MainWnd->UpdateData(false);
				CString s;
				if(UsedPT100 > 0)
				{
					s.Format(_T("%6.2f"),Pv / UsedPT100);
				}
				CWnd * w;
				w = MainWnd->GetDlgItem(IDC_EDIT_PV2);
				w->SetWindowTextW(s);
				MainWnd->UpdateData(false);
				
			//	if(::G_CurveDlg.IsWindowVisible())
				{
			//		G_CurveDlg.DrawTempCurve(1);
				}

			}

			
#else
#ifndef WINCE
			if(data_len == sizeof(MODBUS_RS485_32TEMP_ANSWER))
			{
				MainWnd->m_SwitchDlg[BoxNr].DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
		
				if(MainWnd->m_CurveDlg[BoxNr].IsWindowVisible())
				{
					MainWnd->m_CurveDlg[BoxNr].DrawTempCurve(BoxNr);//UpdateTemp(m_SwitchDlg[BoxNr].m_CurTemp);
				}
				MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_READ_TEMP,data_len,rec_cmd,0,G_IsJustCmdAscii);

				if(G_TakeoutDlg.GetSafeHwnd() != NULL)
				{
					if(G_TakeoutDlg.IsWindowVisible())
					{
						
						
						//::G_TakeoutDlg.ShowTestResult(true);
						
						
					}
				}
				break;
			}
#endif
			if(data_len == sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER))
			{
#ifdef WINCE

				if(G_TempArray_8TEMP_8SSR[BoxNr].GetSize() > 1000)
				{
					G_TempArray_8TEMP_8SSR[BoxNr].RemoveAt(0);
				}
				TEMP_RECORD_8TEMP_8SSR tr;
				static CTime last_time[MAX_HEAT_BOX];
				CTime cur = CTime::GetCurrentTime();
				if(cur.GetSecond() !=  last_time[BoxNr].GetSecond())
				{
					tr.time		=	cur.GetTime();
					tr.Answer_8TEMP_8SSR = *Answer_8TEMP_8SSR;
					G_TempArray_8TEMP_8SSR[BoxNr].Add(tr);
					last_time[BoxNr] = cur;
				}
				
				if(G_HeatBoxDlg.IsWindowVisible())
				{
					::G_HeatBoxDlg.DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
				}
				if(G_SorterTempDlg.GetSafeHwnd() && G_SorterTempDlg.IsWindowVisible())
				{
					
					::G_SorterTempDlg.DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
				}
				else
				{
					G_SorterTempDlg.m_MeterAnswerTime = CTime::GetCurrentTime();
				}
				if(::G_SorterPlcDlg.GetSafeHwnd() && G_SorterPlcDlg.IsWindowVisible())
				{
					::G_SorterPlcDlg.DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
				}

				if(::G_CurveDlg.IsWindowVisible())
				{
					G_CurveDlg.DrawTempCurve(BoxNr);//UpdateTemp(m_SwitchDlg[BoxNr].m_CurTemp);
				}
				::G_DebugDlg.DealCmdAnswer( CMD_READ_TEMP,data_len,rec_cmd,0,G_IsJustCmdAscii);
#else
				if(G_TempArray_8TEMP_8SSR[BoxNr].GetSize() > 1000)
				{
					G_TempArray_8TEMP_8SSR[BoxNr].RemoveAt(0);
				}
				TEMP_RECORD_8TEMP_8SSR tr;
				static CTime last_time[MAX_HEAT_BOX];
				CTime cur = CTime::GetCurrentTime();
				if(cur.GetSecond() !=  last_time[BoxNr].GetSecond())
				{
					tr.time		=	cur.GetTime();
					tr.Answer_8TEMP_8SSR = *Answer_8TEMP_8SSR;
					G_TempArray_8TEMP_8SSR[BoxNr].Add(tr);
					last_time[BoxNr] = cur;
				}

				MainWnd->m_SwitchDlg[BoxNr].DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
		
				if(MainWnd->m_CurveDlg[BoxNr].IsWindowVisible())
				{
					MainWnd->m_CurveDlg[BoxNr].DrawTempCurve(BoxNr);//UpdateTemp(m_SwitchDlg[BoxNr].m_CurTemp);
				}
				MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_READ_TEMP,data_len,rec_cmd,0,G_IsJustCmdAscii);
#endif
				break;
			}

			MODBUS_RS485_TEMP_ANSWER_PARA *temp_para = (MODBUS_RS485_TEMP_ANSWER_PARA *)modbus_cmd_128->para;
			
			
			
			for(i = 0;i < 4; i++)
			{
				short_a = 0;
				char *buf = (char*)&short_a;

				
//#ifdef _ASCII_	
				if(G_IsJustCmdAscii == true)
				{
					if(ascii_cmd->val[i*4] == '_')
					{
					
						short_a = (ascii_cmd->val[i*4 + 1] - '0')*100
								+(ascii_cmd->val[i*4 + 2] - '0')*10
								+(ascii_cmd->val[i*4 + 3] - '0')*1;
						short_a  = 0 - short_a;
					}
					else
					{
						short_a = (ascii_cmd->val[i*4 ] - '0')*1000
								+(ascii_cmd->val[i*4 + 1] - '0')*100
								+(ascii_cmd->val[i*4 + 2] - '0')*10
								+(ascii_cmd->val[i*4 + 3] - '0')*1;
					}
					para[i] = short_a ;

					temp[i] = para[i] /10.0f;
				//	m_SwitchDlg[BoxNr].DealCmdAnswer(rec_cmd,0,G_IsJustCmdAscii);
				}
				else
				{//MODBUS
#ifdef _HeatBox
					if(G_iMaxLedNr[BoxNr] == 192)
					{
						buf[0] =  modbus_cmd_192->para[i*2];
						buf[1]  = modbus_cmd_192->para[i*2+1];
					}
					else
#endif
					{
						buf[0] =  modbus_cmd_128->para[i*2];
						buf[1]  = modbus_cmd_128->para[i*2+1];
					}

					MODBUS_RS485_TEMP_ANSWER_PARA *temp_para = (MODBUS_RS485_TEMP_ANSWER_PARA *)modbus_cmd_128->para;
					if(temp_para->TempDecimalPlaces == 1)
					{
						temp[i] = short_a /10.0f;

						para[i] = (short)(temp[i] * 10.0f);
					}
					else
					{
						temp[i] = short_a /100.0f;
						para[i] = (short)(temp[i] * 10.0f);
					}

				}


				
			}

#ifndef WINCE
#ifdef		_DEBUG
			//if(CheckSameTemp(BoxNr))
			//{
			//	CString cmdStr;	
			//	char para[16];
			//	memset(para,0,sizeof(para));

			//	WriteLogFile("CheckSameTemp SendStopHeatCmd",BoxNr+1);

			//	G_RS485_Comm.SendStopHeatCmd( BoxNr);
			//}
			
#endif		
#endif
		//	char str_buf[100];
		//	sprintf_s(str_buf,"%5.2f,%5.2f,%5.2f,%5.2f",temp[0],temp[1],temp[2],temp[3]);
		//	WriteLogFile(str_buf,BoxNr + 1);
				
			//m_SwitchDlg[BoxNr].UpdateData(false);
			
#ifdef _HeatBox				

			{
#ifndef WINCE
				static CTime last_time[MAX_HEAT_BOX];
				TEMP_RECORD tr;
				CTime cur = CTime::GetCurrentTime();
				if(cur.GetSecond() !=  last_time[BoxNr].GetSecond())
				{
					tr.time		=	cur.GetTime();
					tr.temp[0]	=	MainWnd->m_SwitchDlg[BoxNr].m_CurTemp[0]; 
					tr.temp[1]	=	MainWnd->m_SwitchDlg[BoxNr].m_CurTemp[1]; 
					tr.temp[2]	=	MainWnd->m_SwitchDlg[BoxNr].m_CurTemp[2]; 
					tr.temp[3]	=	MainWnd->m_SwitchDlg[BoxNr].m_CurTemp[3]; 
					tr.temp[4]	=	temp_para->Sv / 10.0f;
					G_TempArray[BoxNr].Add(tr);
					last_time[BoxNr] = cur;
				}

				//if(BoxNr == 0)
				{

					if(G_TempArray[BoxNr].GetCount() > CAL_SPEED_PERIOD)
					{
						TEMP_RECORD tr_0,tr_1;
						float Speed = 0;

						//for(int i = 1; i < CAL_SPEED_PERIOD,i < G_TempRecordArray[BoxNr].GetSize() ; i++)
						//{
						//	Speed += 60 *(m_TempTickArray.GetAt(i).Temp - m_TempTickArray.GetAt(0).Temp) * 1000.0f / (m_TempTickArray.GetAt(i).ticks - m_TempTickArray.GetAt(0).ticks);
						//}
				
						tr_0 = G_TempArray[BoxNr].GetAt(G_TempArray[BoxNr].GetCount() - 1);
						tr_1 = G_TempArray[BoxNr].GetAt(G_TempArray[BoxNr].GetCount() - CAL_SPEED_PERIOD);
						CTime t0(t = CTime(tr_0.time));
						CTime t1(t = CTime(tr_1.time));
						CTimeSpan ts = t0 -t1;
						float temp_0 = (tr_0.temp[0] +  tr_0.temp[1] + tr_0.temp[2] + tr_0.temp[3] ) / 4.0f;
						float temp_1 = (tr_1.temp[0] +  tr_1.temp[1] + tr_1.temp[2] + tr_1.temp[3] ) / 4.0f;
						Speed = (temp_0 - temp_1  ) / ts.GetTotalSeconds() * 60.0f;

						MainWnd->m_SwitchDlg[BoxNr].m_Speed = Speed ;
					//	char buf[100];
					//	sprintf(buf,"Speed = %5.1f, %5.1f,%5.1f,%03d,%02d:%02d:%02d,%02d:%02d:%02d",Speed,temp_0,temp_1,ts.GetSeconds(),t0.GetHour(),t0.GetMinute(),t0.GetSecond(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());
					//	::WriteLogFile(buf,BoxNr); 
						
					}
				}
#endif




			}

#endif
			
			//保存当前开关状态
			
			//更新有关窗口
#ifdef _HeatBox
#ifdef WINCE
			if(::G_HeatBoxDlg.IsWindowVisible())
			{
				G_HeatBoxDlg.DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
			}
			if(::G_DebugDlg.IsWindowVisible())
			{
				G_DebugDlg.DealCmdAnswer( CMD_READ_TEMP,data_len,rec_cmd,0,G_IsJustCmdAscii);
			}
			if(G_TempArray[BoxNr].GetSize() > 10000)
			{
				G_TempArray[BoxNr].RemoveAt(0,100); 
			}

			TEMP_RECORD temp;
			if(temp_para->TempDecimalPlaces == 1)
			{
				temp.temp[0] =  temp_para->Temp[0] / 10.0f;
				temp.temp[1] =  temp_para->Temp[1] / 10.0f;
				temp.temp[2] =  temp_para->Temp[2] / 10.0f;
				temp.temp[3] =  temp_para->Temp[3] / 10.0f;
			}
			else
			{
				temp.temp[0] =  temp_para->Temp[0] / 100.0f;
				temp.temp[1] =  temp_para->Temp[1] / 100.0f;
				temp.temp[2] =  temp_para->Temp[2] / 100.0f;
				temp.temp[3] =  temp_para->Temp[3] / 100.0f;
			}
			temp.temp[4] =  temp_para->Sv		 / 10.0f;
			CTime cur = CTime::GetCurrentTime(); 
			temp.time  = cur.GetTime(); 

			temp.HeatPercent		=  temp_para->HeatPercent;

			G_Meter[BoxNr].m_TempAnswerPara = *temp_para;
			if(::G_NormalConfigPara[0].BoxType == BOX_JIU_YANG)
			{
				static int last_secs = 0;

				int cur_sec	= CTime::GetCurrentTime().GetSecond();

				//if((cur_sec != last_secs) && ((cur_sec % 5) == 0))
				{
					G_TempArray[BoxNr].Add(temp);
				}
			}
			else
			{
				G_TempArray[BoxNr].Add(temp);
			}
		
			G_CurveDlg.DrawTempCurve(BoxNr);

			if(G_CalDlg.IsWindowVisible())
			{
				G_CalDlg.DealCmdAnswer(CMD_READ_TEMP,(MODBUS_RS485_CMD_128*)rec_cmd,NULL);
			}
#else
			MainWnd->m_SwitchDlg[BoxNr].DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
		
			if(MainWnd->m_CurveDlg[BoxNr].IsWindowVisible())
			{
				MainWnd->m_CurveDlg[BoxNr].DrawTempCurve(BoxNr);//UpdateTemp(m_SwitchDlg[BoxNr].m_CurTemp);
			}
			MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_READ_TEMP,data_len,rec_cmd,0,G_IsJustCmdAscii);

			if(G_TakeoutDlg.GetSafeHwnd() != NULL)
			{
				if(G_TakeoutDlg.IsWindowVisible())
				{
					
					if(temp_para->iError && 0xFFF0)
					{
						unsigned int i = temp_para->iError && 0xFFF0;
						i = (i >> 16);
						G_TakeoutDlg.m_iFailHighLevel_Take[BoxNr]	+= ((temp_para->iError & 0x00F0) >> 4);
						G_TakeoutDlg.m_iFailLowLevel_Take[BoxNr]	+= ((temp_para->iError & 0x0F00) >> 8);
						G_TakeoutDlg.m_iFailOtherLevel_Take[BoxNr]	+= ((temp_para->iError & 0xF000) >> 12);
						CWnd * w;
						CString s;

						w = G_TakeoutDlg.GetDlgItem(IDC_EDIT_OTHER_FAIL_LEVEL_TAKE);
						s.Format(_T("%d"), G_TakeoutDlg.m_iFailOtherLevel_Take[BoxNr]);
						w->SetWindowTextW(s);

						w = G_TakeoutDlg.GetDlgItem(IDC_EDIT_HIGH_FAIL_LEVEL_TAKE);
						s.Format(_T("%d"), G_TakeoutDlg.m_iFailHighLevel_Take[BoxNr]);
						w->SetWindowTextW(s);

						w = G_TakeoutDlg.GetDlgItem(IDC_EDIT_LOW_FAIL_LEVEL_TAKE);
						s.Format(_T("%d"), G_TakeoutDlg.m_iFailLowLevel_Take[BoxNr]);
						w->SetWindowTextW(s);
						G_TakeoutDlg.UpdateData(false);
					}
					//::G_TakeoutDlg.ShowTestResult(true);
					
					
				}
			}
#endif	
#endif
#endif
		}
		break;


	case CMD_READ_PID_T:
		WriteLogFile("CMD_READ_PID_T",BoxNr+1);
#ifdef _HeatBox
#ifdef WINCE
		memcpy(&G_PID[0].P ,		&modbus_cmd_128->para[0],sizeof(short) * 7);
		
		if(G_SorterTempDlg.GetSafeHwnd() && G_SorterTempDlg.IsWindowVisible())
		{
			::G_SorterTempDlg.DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
		}

		if(::G_SorterPlcDlg.GetSafeHwnd() && G_SorterPlcDlg.IsWindowVisible())
		{
			::G_SorterPlcDlg.DealCmdAnswer(&rec_cmd[0],0,G_IsJustCmdAscii,data_len);
		}

		if(G_DebugDlg.IsWindowVisible())
		{
			G_DebugDlg.DealCmdAnswer(CMD_READ_PID_T,data_len,rec_cmd,0,G_IsJustCmdAscii);
		}
		if(G_CalDlg.IsWindowVisible())
		{
			G_CalDlg.DealCmdAnswer(CMD_READ_TEMP,(MODBUS_RS485_CMD_128*)rec_cmd,NULL);
		}
#else
		MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer(CMD_READ_PID_T,data_len,rec_cmd,0,G_IsJustCmdAscii);
#endif
#elif _20PT100_5SSR
		if(BoxNr == 0)
		{
			memcpy(&G_PID[BoxNr].P ,		&modbus_cmd_128->para[0],sizeof(short) * 7);
			

			MainWnd->m_P1 = G_PID[BoxNr].P;
			MainWnd->m_I1 = G_PID[BoxNr].I;
			MainWnd->m_D1 = G_PID[BoxNr].D;
			MainWnd->UpdateData(false);
			

		}
		else if(BoxNr == 1)
		{
			memcpy(&G_PID[BoxNr].P ,		&modbus_cmd_128->para[0],sizeof(short) * 7);
			MainWnd->m_P2 = G_PID[BoxNr].P;
			MainWnd->m_I2 = G_PID[BoxNr].I;
			MainWnd->m_D2 = G_PID[BoxNr].D;
			MainWnd->UpdateData(false);
		}
#endif
		break;
	case CMD_SET_PID_T:
		{
			CString sss;
			if(data_len == sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER))
			{
				//sss.Format(_T("data_len = %d,CMD_SET_PID_T=%d"),data_len,Answer_8TEMP_8SSR->);
			
				AfxMessageBox(sss);
			}
			else
			{
				//sss.Format(_T("data_len = %d,CMD_SET_PID_T=%d"),data_len,modbus_cmd_128->para[14]);
			
				//AfxMessageBox(sss);
			}
		}

		//ProgramPara(data_len,BoxNr,Para_Answer);

		WriteLogFile("CMD_SET_PID_T",BoxNr+1);
		break;
	case CMD_SET_ADJUST_LOW:
		ProgramPara(data_len,BoxNr,Para_Answer);
		WriteLogFile("CMD_SET_ADJUST_LOW",BoxNr+1);
#ifdef _HeatBox
#ifdef WINCE
		G_CalDlg.DealCmdAnswer( CMD_READ_ADJUST_LOW,(MODBUS_RS485_CMD_128*)rec_cmd,0); 
#else
		MainWnd->KillTimer(1);
		MainWnd->SetTimer(1,::G_NormalConfigPara[0].CommTimer ,NULL); 
		
#endif
#endif
		break;
	case CMD_SET_ADJUST_HIGH:
		ProgramPara(data_len,BoxNr,Para_Answer);

		WriteLogFile("CMD_SET_ADJUST_HIGH",BoxNr+1);
#ifdef _HeatBox
#ifdef WINCE
		G_CalDlg.DealCmdAnswer( CMD_READ_ADJUST_LOW,(MODBUS_RS485_CMD_128*)rec_cmd,0); 
#else
		MainWnd->KillTimer(1);
		MainWnd->SetTimer(1,::G_NormalConfigPara[0].CommTimer ,NULL); 
		
#endif
#endif
		break;
	case CMD_READ_ADJUST_LOW:
#ifndef _20PT100_5SSR
#ifdef WINCE
		G_DebugDlg.DealCmdAnswer( CMD_READ_ADJUST_LOW,data_len,rec_cmd,0,G_IsJustCmdAscii);
#else

		MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_READ_ADJUST_LOW,data_len,rec_cmd,0,G_IsJustCmdAscii);
#endif
#endif
		break;
	case CMD_READ_ADJUST_HIGH:
		WriteLogFile("CMD_READ_ADJUST_HIGH",BoxNr+1);
#ifdef _HeatBox		
#ifdef WINCE
		G_DebugDlg.DealCmdAnswer( CMD_READ_ADJUST_HIGH,data_len,rec_cmd,0,G_IsJustCmdAscii);
		G_CalDlg.DealCmdAnswer( CMD_READ_ADJUST_LOW,(MODBUS_RS485_CMD_128*)rec_cmd,0); 
#else
		
		//  if(modbus_cmd_128->para[12] == '8'
		//	&& modbus_cmd_128->para[13] == '5'
		//	&& modbus_cmd_128->para[14] == '9'
		//	&& modbus_cmd_128->para[15] == '8')
		{
			MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_READ_ADJUST_HIGH,data_len,rec_cmd,0,G_IsJustCmdAscii);
		}
#endif
#endif

		break;

	case CMD_READ_AUTHOR:
	//	WriteLogFile("CMD_READ_AUTHOR",BoxNr+1);
	//	memcpy(&G_CopyRight.m_Author[BoxNr],&rec_cmd[3],sizeof(G_CopyRight.m_Author[0]));
		break;
	case CMD_READ_BUILDDATE:
	//	WriteLogFile("CMD_READ_BUILDDATE",BoxNr+1);
	//	memcpy(&G_CopyRight.m_BuildDate[BoxNr],&rec_cmd[3],sizeof(G_CopyRight.m_BuildDate[0]));
		break;
	
	case CMD_READ_PRODUCEDATE:
//		
//#ifndef _ASCII_
//		WriteLogFile("CMD_READ_PRODUCEDATE",BoxNr+1);
//		if(G_iMaxLedNr == 192)
//		{
//			memcpy(&G_CopyRight.m_ProductDate[BoxNr][0],modbus_cmd_192->para,sizeof(G_CopyRight.m_ProductDate[0]));
//		}
//		else
//		{
//			memcpy(&G_CopyRight.m_ProductDate[BoxNr][0],modbus_cmd_128->para,sizeof(G_CopyRight.m_ProductDate[0]));
//		}
//		for(i=0; i < 8; i++)
//		{
//			G_CopyRight.m_ProductDate[BoxNr][i] +='0';
//		}
//#endif
//		
		break;

	case CMD_READ_SN:
//
//
//#ifndef _ASCII_
//		WriteLogFile("CMD_READ_SN",BoxNr+1);
//		memcpy(&G_CopyRight.m_SN[BoxNr],modbus_cmd_128->para ,sizeof(unsigned int));
//#endif
//		
//		break;
//
//	case CMD_SET_BEEP:
//		if(modbus_cmd_128->para[0] != 0)
//			{ //表示空调开
//				G_RS485_Comm.m_BeepStartTime[BoxNr] = CTime::GetCurrentTime();
//				
//#ifdef WINCE
//				::G_HeatBoxDlg.m_TempPara.OutputStatus &=~(1 << METER_OUTPUT_STATUS_BIT_BEEP);
//				
//				G_HeatBoxDlg.Draw_PV_SV(); 
//
//#else
//				
//				MainWnd->m_SwitchDlg[BoxNr].m_TempPara.OutputStatus |= (1 << METER_OUTPUT_STATUS_BIT_BEEP);
//				MainWnd->m_SwitchDlg[BoxNr].m_Meter.m_IsBeepOn = true;
//#endif
//			}
//			else
//			{
//				G_RS485_Comm.m_BeepStartTime[BoxNr] = CTime::GetCurrentTime();
//				
//#ifdef WINCE
//				G_HeatBoxDlg.m_TempPara.OutputStatus |= (1 << METER_OUTPUT_STATUS_BIT_BEEP);
//
//				G_HeatBoxDlg.Draw_PV_SV(); 
//				//AfxMessageBox(_T("CMD_AIR_CTL ON"));
//#else
//				MainWnd->m_SwitchDlg[BoxNr].m_TempPara.OutputStatus &=~(1 << METER_OUTPUT_STATUS_BIT_BEEP);
//				MainWnd->m_SwitchDlg[BoxNr].m_Meter.m_IsBeepOn = false;
//#endif
//			}
////		AfxMessageBox(_T("CMD_SET_BEEP sfsd"));
//		break;
		
	case CMD_NORMAL_PID_OUT:

#ifdef _HeatBox
		{
#ifndef WINCE
			
		char buf[100];
		sprintf_s(buf,"CMD_NORMAL_PID_OUT %5.1f",MainWnd->m_SwitchDlg[BoxNr].m_PidTemp);  
		WriteLogFile(buf,BoxNr+1);
#endif
		}
#ifdef WINCE
		G_DebugDlg.DealCmdAnswer( CMD_NORMAL_PID_OUT,data_len,rec_cmd,0,G_IsJustCmdAscii);
#else
		MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_NORMAL_PID_OUT,data_len,rec_cmd,0,G_IsJustCmdAscii);
#endif
#endif
		break;
    case CMD_STEP_PID_OUT:

		{
			;
		}
#ifdef _HeatBox
#ifndef WINCE
		char buf[100];
			sprintf_s(buf,"CMD_STEP_PID_OUT %5.1f",MainWnd->m_SwitchDlg[BoxNr].m_PidTemp);  
			WriteLogFile(buf,BoxNr+1);
		MainWnd->m_DebugDlg[BoxNr].DealCmdAnswer( CMD_STEP_PID_OUT,data_len,rec_cmd,0,G_IsJustCmdAscii);
#else
		G_DebugDlg.DealCmdAnswer( CMD_STEP_PID_OUT,data_len,rec_cmd,0,G_IsJustCmdAscii);
#endif
#endif
		break;
    case 	CMD_STOP_OUT:
		{
#ifdef _HeatBox
#ifndef WINCE
			char buf[100];
			sprintf_s(buf,"CMD_STOP_OUT %5.1f",MainWnd->m_SwitchDlg[BoxNr].m_PidTemp);  
			WriteLogFile(buf,BoxNr+1);
#endif
#endif
		}
		break;
	case 	CMD_FULL_OUT:
		{

#ifdef _HeatBox
#ifndef WINCE
			char buf[100];
			sprintf_s(buf,"CMD_FULL_OUT %5.1f",MainWnd->m_SwitchDlg[BoxNr].m_PidTemp);  
			WriteLogFile(buf,BoxNr+1);
#endif
#endif
		}
		break;
	case CMD_SET_DISPLAY_TYPE:
		{
#ifdef _HeatBox
#ifndef WINCE
			char buf[100];
			sprintf_s(buf,"CMD_SET_DISPLAY_TYPE %5.1f",MainWnd->m_SwitchDlg[BoxNr].m_PidTemp);  
			WriteLogFile(buf,BoxNr+1);
#endif
#endif
		}
		break;
    default:
		{
#ifdef _HeatBox
#ifndef  WINCE
			s.Format(_T("错误命令 = %X:"),(unsigned char)rec_cmd[1]);
			for(int i = 0; i < data_len; i++)
			{
				s1.Format(_T("%02X "),(unsigned char)rec_cmd[i]);
				s+=s1;
			}

			
			MainWnd->m_SwitchDlg[BoxNr].m_TestMsg = s;

			MainWnd->m_SwitchDlg[BoxNr].UpdateData(false);

			{
				char buf[10240];
				char buf1[1024];
				sprintf(buf,"错误命令 = %X:",(unsigned char)rec_cmd[1]);
				for(int i = 0; i < data_len; i++)
				{
					sprintf(buf1,"%02X ",(unsigned char)rec_cmd[i]);
					strcat(buf,buf1);
				}
				::WriteLogFile(buf,BoxNr); 
			}
#endif
#endif

		}
		break;
	}

	return 0;

}

int CComm::Hex2Bin_ARM(unsigned char *Hex,int HexBufLen ,char *binData,int MaxLen)
{
	
	int HexPos = 0;
	unsigned int BinPos = 0;

//	CFile f;
//	if(f.Open(HexFile,CFile::modeReadWrite |  CFile::shareDenyWrite))
	{
//		f.SeekToBegin();
//		int fileLen = f.Read(HexBuf,sizeof(HexBuf));
//		f.Close(); 

		if(HexBufLen <= 0)
		{
			return 0;
		}
		
		while(HexPos < HexBufLen)
		{
			HEX_LINE hex_line;
			int i;
			if(Hex[HexPos] == ':')
			{
				memset(&hex_line,0,sizeof(hex_line));
				i = 0;
				while(1)
				{
					((char*)&hex_line)[i] = Hex[HexPos+i];
					if(((char*)&hex_line)[i] == 0x0A)
					{
						break;
					}
					i++;
				}
				int data_len = Hex2Char(hex_line.dataLen);
				unsigned int dataAddr = Hex2Char(&hex_line.dataAddr[0])*(int)256 +  Hex2Char(&hex_line.dataAddr[2]);

				if(dataAddr >= 0x4000)
				{

					dataAddr -= 0x4000;

					for(int j = 0; j < data_len; j++)
					{
						binData[dataAddr + j] = Hex2Char(&hex_line.data[j*2]);

						if(BinPos < (dataAddr + j))
						{
							BinPos = (dataAddr + j);
						}
					}
				}
				
				HexPos += i;
				continue;
			}
			
			HexPos ++;
		}
	}
	
	return BinPos;
}
