#include "stdafx.h"
#include "LPM260_Printer.h"
#include "common.h"
#include <math.h>

extern CComm	G_PrintRS232_Comm;
static CPrintLineArray	G_PrintLineArray;


CLPM260_Printer::CLPM260_Printer(void) 
{

}
CLPM260_Printer::~CLPM260_Printer(void)
{
}
void CLPM260_Printer::SetComm(CComm * comm)
{

	//m_Comm = comm;


}

void CLPM260_Printer::SetPaperType(char type)
{
    char buf[20];
    buf[0] = 0x1F;
    buf[1] = 0x41;
    buf[2] = type ; //0 连续的热敏纸 1 标签纸
    if(G_PrintRS232_Comm.IsOpened())
    {
        G_PrintRS232_Comm.WriteComm(buf,3);

    }
}

void CLPM260_Printer::EnterChineseMode(void)
{
	char buf[20];
	memset(buf,0,sizeof(buf));
	buf[0] = 0x1b;
	buf[1] = 0x40;
	buf[2] = 0x1C;
	buf[3] = 0x26;

	if(::G_NormalConfigPara[0].AutoPrintForwardType == 0)
	{
		G_PrintRS232_Comm.WriteComm(buf,4);
	}
	else
	{
		PrintLine pl;
		memset(&pl,0,sizeof(pl));
		strcpy(pl.buf,buf);
		::G_PrintLineArray.Add(pl);
	}
}
void CLPM260_Printer::ExitChineseMode(void)
{
	char buf[20];
	memset(buf,0,sizeof(buf));
	buf[0] = 0x1b;
	buf[1] = 0x40;
	buf[2] = 0x1C;
	buf[3] = 0x2E;

	if(::G_NormalConfigPara[0].AutoPrintForwardType == 0)
	{
		G_PrintRS232_Comm.WriteComm(buf,4);
	}
	else
	{
		PrintLine pl;
		memset(&pl,0,sizeof(pl));
		strcpy(pl.buf,buf);
		::G_PrintLineArray.Add(pl);
	}
}

void CLPM260_Printer::SelectCharType(LPM260_CHAR_TYPE CharType)
{
	char buf[20];
	buf[0] = ESC;
	buf[1] = '8';
	buf[2] = CharType;
	G_PrintRS232_Comm.WriteComm(buf,3);

}

void CLPM260_Printer::NextLine(void)
{
	char buf[20];
	buf[0] = LF;
	G_PrintRS232_Comm.WriteComm(buf,1);

}

void CLPM260_Printer::FeedForward(char dots )
{
	char buf[20];
	buf[0] = ESC;
	buf[1] = 'J';
	buf[2] = dots;
	buf[3] = 0x0d;
	buf[4] = 0x0a;
	G_PrintRS232_Comm.WriteComm(buf,5);
}

void CLPM260_Printer::SetLineGap(char dots )
{
	char buf[20];
	buf[0] = ESC;
	buf[1] = 0x33;
	buf[2] = dots;
	buf[3] = 0x0d;
	buf[4] = 0x0a;
	G_PrintRS232_Comm.WriteComm(buf,5);
}

void CLPM260_Printer::SetCharGap(char dots )
{
	char buf[20];
	buf[0] = ESC;
	buf[1] = 0x20;
	buf[2] = dots;
	buf[3] = 0x0d;
	buf[4] = 0x0a;
	G_PrintRS232_Comm.WriteComm(buf,5);
}

void CLPM260_Printer::SetDarkness(char deep )
{
	/*
	char buf[20];
	buf[0] = ESC;
	buf[1] = 0x74;
	buf[2] = 0x2B;
	buf[3] = deep;
	G_PrintRS232_Comm.WriteComm(buf,4);
	*/
}

void CLPM260_Printer::PrinterInit(void)
{

	char buf[20];
	buf[0] = ESC;
	buf[1] = 0x40; //@

	if(G_PrintRS232_Comm.IsOpened())
	{
		G_PrintRS232_Comm.WriteComm(buf,2);
	}
	else
	{
//		AfxMessageBox(_T("Not Open"));
	}

	SetLineGap(20);
}

void CLPM260_Printer::PrintOneChineseLine(char * str)
{

	if(G_PrintRS232_Comm.IsOpened() == false )
	{
		return;
	}

	if(::G_NormalConfigPara[0].AutoPrintForwardType == 0 )
	{
		char buf[20];
		memset(buf,0,sizeof(buf));
		buf[0] = '\n';
		EnterChineseMode();
		G_PrintRS232_Comm.WriteComm(str,strlen(str));
		G_PrintRS232_Comm.WriteComm(buf,1);
		Sleep(50);
		ExitChineseMode();
	}
	else
	{
		char buf[20];
		memset(buf,0,sizeof(buf));
		buf[0] = '\n';
		ExitChineseMode();
		PrintLine pl;
		strcpy(pl.buf,str);
		strcat(pl.buf,buf); 
		::G_PrintLineArray.Add(pl);  
		EnterChineseMode();
		
	}
}

void CLPM260_Printer::PrintOneLine(char * str)
{
	if(G_PrintRS232_Comm.IsOpened() == false )
	{
		return;
	}
	
	if(::G_NormalConfigPara[0].AutoPrintForwardType == 0)
	{
		char buf[20];
		memset(buf,0,sizeof(buf));
		buf[0] = '\n';
		
		G_PrintRS232_Comm.WriteComm(str,strlen(str));
		G_PrintRS232_Comm.WriteComm(buf,1);
		Sleep(50);
	}
	else
	{
		char buf[20];
		memset(buf,0,sizeof(buf));
		buf[0] = '\n';
		
		PrintLine pl;
		strcpy(pl.buf,str);
		strcat(pl.buf,buf); 
		::G_PrintLineArray.Add(pl);
	}

}

void CLPM260_Printer::PrintTestPaper(void)
{
	/*
	char buf[20];
	memset(buf,0,sizeof(buf));
	buf[0] = 0x12;
	buf[1] = 0x54;
	G_PrintRS232_Comm.WriteComm(buf,2);
*/
	PrintOneChineseLine("常州市润邦电子科技有限公司");
	PrintOneChineseLine("服务电话：13915838598");
	PrintOneLine("");
	PrintOneLine("");
	PrintOneLine("");
	PrintOneLine("");
}

static float open_data[MAX_LED_NR],close_data[MAX_LED_NR],diff_data[MAX_LED_NR];
bool IsHaveSameGroupId(int * m_Group,int group_id)
{
	for( int i = 0; i < 128; i++)
	{
		if(m_Group[i] == group_id)
		{
			return true;
		}
	}

	return false;
}
void CLPM260_Printer::PrintTestResult(int BoxNr,RECORD*	record,int * m_Group,int group_id,CONTROL_TEMP_RANGE temp_range,unsigned char BoxType,int Language)
{



	if(::G_NormalConfigPara[BoxNr].AutoPrintType == 0)
	{
		return;
	}

	char buf[100],temp_buf[100];
	
	float open3sigma = 0,close3sigma = 0,diff3sigma = 0;
	float open_Cp = 0,close_Cp = 0;
	float open_Cpk =0, close_Cpk = 0;

	CTime cur = CTime::GetCurrentTime(); 

	

	

	
	//CHeatBoxDlg::CheckHeatOrCoolMode(record->ConfigPara, &temp_range);
	
	int OpenTempNr[200],CloseTempNr[200];
	float SumOpenTemp = 0, SumCloseTemp = 0;
	float MinOpenTemp = 1000,MaxOpenTemp = -1000;
	float MinCloseTemp = 1000,MaxCloseTemp = -1000;
	float MinDiff = 1000,MaxDiff = -1000;
	int   TotalOpenTemp = 0,TotalCloseTemp = 0;
	memset(OpenTempNr,0,sizeof(OpenTempNr));
	memset(CloseTempNr,0,sizeof(CloseTempNr));

	int PrintMaxNr = G_iMaxLedNr[BoxNr];

	switch(G_NormalConfigPara[0].BoxType)
	{
	case BOX_DISC_24:
		PrintMaxNr = 24;
		break;
	case BOX_DISC_40:
		PrintMaxNr = 40;
		break;

	}


	for(int i = PrintMaxNr - 1; i>= 0; i--)
	{
		if(m_Group != NULL)
		{
			if(m_Group[i] != group_id)
			{
				continue;
			}
		}

		if(record->TestResult[i].IsUsed)
		{ 
			if(record->TestResult[i].IsClosed && record->TestResult[i].IsOpenned)
			{
				SumOpenTemp  += record->TestResult[i].OpenTemp;
				SumCloseTemp += record->TestResult[i].CloseTemp;
				if(MinOpenTemp > record->TestResult[i].OpenTemp)
				{
					MinOpenTemp = record->TestResult[i].OpenTemp;
				}
				if(MaxOpenTemp < record->TestResult[i].OpenTemp)
				{
					MaxOpenTemp = record->TestResult[i].OpenTemp;
				}
				if(MinCloseTemp > record->TestResult[i].CloseTemp)
				{
					MinCloseTemp = record->TestResult[i].CloseTemp;
				}
				if(MaxCloseTemp < record->TestResult[i].CloseTemp)
				{
					MaxCloseTemp = record->TestResult[i].CloseTemp;
				}
				float diff = record->TestResult[i].OpenTemp - record->TestResult[i].CloseTemp;
				if(MinDiff > diff)
				{
					MinDiff = diff;
				}
				if(MaxDiff < diff)
				{
					MaxDiff = diff;
				}

				open_data[TotalOpenTemp]	= record->TestResult[i].OpenTemp;
				close_data[TotalCloseTemp]	= record->TestResult[i].CloseTemp;
				diff_data[TotalCloseTemp]	= record->TestResult[i].OpenTemp - record->TestResult[i].CloseTemp;
				TotalOpenTemp ++;
				TotalCloseTemp ++;
				
				OpenTempNr[(int)(record->TestResult[i].OpenTemp)] ++;
				CloseTempNr[(int)(record->TestResult[i].CloseTemp)] ++;
			}
			else if(record->TestResult[i].IsOpenned)
			{
				SumOpenTemp  += record->TestResult[i].OpenTemp;
				if(MinOpenTemp > record->TestResult[i].OpenTemp)
				{
					MinOpenTemp = record->TestResult[i].OpenTemp;
				}
				if(MaxOpenTemp < record->TestResult[i].OpenTemp)
				{
					MaxOpenTemp = record->TestResult[i].OpenTemp;
				}

				open_data[TotalOpenTemp] = record->TestResult[i].OpenTemp;
				TotalOpenTemp ++;
				OpenTempNr[(int)(record->TestResult[i].OpenTemp)] ++;
			
			}
			else if(record->TestResult[i].IsClosed)
			{
				SumCloseTemp  += record->TestResult[i].CloseTemp;
				if(MinCloseTemp > record->TestResult[i].CloseTemp)
				{
					MinCloseTemp = record->TestResult[i].CloseTemp;
				}
				if(MaxCloseTemp < record->TestResult[i].CloseTemp)
				{
					MaxCloseTemp = record->TestResult[i].CloseTemp;
				}

				close_data[TotalCloseTemp] = record->TestResult[i].CloseTemp;
				TotalCloseTemp ++;
				CloseTempNr[(int)(record->TestResult[i].CloseTemp)] ++;
			
			}
			else //没有动作
			{

			}

		}
	}
	
	//open_data[TotalOpenTemp] = record.TestResult[i].OpenTemp;
	//			close_data[TotalCloseTemp] = record.TestResult[i].CloseTemp;
	
	

	if((TotalOpenTemp == 0) && (TotalCloseTemp == 0))
	{
		return;
	}

	PrinterInit();

	SetDarkness(5);

	if(::G_NormalConfigPara[0].AutoPrintForwardType == false)
	{

		if(m_Group != NULL)
		{
			if(IsHaveSameGroupId(m_Group,group_id))
			{
				PrintOneLine(" ");

				PrintOneLine(" ");

				PrintOneLine(" ");
			}
		}
		

	//	FeedForward(200); 
	}

	
	if(group_id == 0)
	{
		PrintOneLine(" ");
	}
	int OpenLow = 0, CloseLow = 0;
	int OpenHigh = 0,CloseHigh = 0;

	for(int i = 0; i < (int)temp_range.open_temp_min; i++)
	{
		OpenLow += OpenTempNr[i];
	}
	for(int i = 0; i < (int)temp_range.close_temp_min; i++)
	{
		CloseLow+= CloseTempNr[i];
	}
	for(int i = (int)temp_range.open_temp_max; i < 200; i++)
	{
		OpenHigh += OpenTempNr[i];
	}
	for(int i = (int)temp_range.close_temp_max;i < 200; i++)
	{
		CloseHigh += CloseTempNr[i];
		
	}

	
	
	
	

	//if(group_id == 0)
	{
		CLPM260_Printer::PrintResultHead( BoxNr,record,BoxType,Language);


	}
	if(Language == 0)
	{
		sprintf(buf,"动作 : %5.1f --- %5.1f",temp_range.open_temp_min,temp_range.open_temp_max );
		PrintOneChineseLine(buf);
	}
	else
	{
		sprintf(buf,"ACT : %5.1f --- %5.1f",temp_range.open_temp_min,temp_range.open_temp_max );
		PrintOneLine(buf);
	}
	if(temp_range.close_temp_min != temp_range.close_temp_max)
	{
		if(Language == 0)
		{
			sprintf(buf,"复位 : %5.1f --- %5.1f",temp_range.close_temp_min,temp_range.close_temp_max);
			PrintOneChineseLine(buf);
		}
		else
		{
			sprintf(buf,"RET : %5.1f --- %5.1f",temp_range.close_temp_min,temp_range.close_temp_max);
			PrintOneLine(buf);
		}
	}

	//详细动作复位温度
	if(Language == 0)
	{
		PrintOneChineseLine("编号    动作      复位    差值");
	}
	else
	{
		PrintOneLine("No.     ACT       RET     DIF");
	}
	

	if(group_id == 0)
	{
		PrintOneLine("----");
	}

	int AllNotUsed = -1;

	for(int i = 0; i < PrintMaxNr ; i++)
	{
		if(m_Group != NULL)
		{
			if(m_Group[i] != group_id)
			{
				continue;
			}
		}
		if((i % 10) == 0)
		{
			 AllNotUsed = -1;

			for(int j = i;j < i+10; j++)
			{
				if(j < G_iMaxLedNr[BoxNr])
				{
					if(record->TestResult[j].IsUsed)
					{
						AllNotUsed = i;
						break;
					}
				}
			}

			if(AllNotUsed < 0)
			{
				i += 9;
				if(i >= G_iMaxLedNr[BoxNr])
				{
					break;
				}
				continue;
			}
		}
		if(record->TestResult[i].IsUsed)
		{
			if(record->TestResult[i].IsClosed && record->TestResult[i].IsOpenned)
			{
				char c1,c2;
				if(record->TestResult[i].OpenTemp >= temp_range.open_temp_min && record->TestResult[i].OpenTemp <= temp_range.open_temp_max)
				{
					/*if(record->TestResult[i].IsOpenFlash)
					{
						c1 = '*';
					}
					else*/
					{
						c1 = ' ';
					}
				}
				else
				{
					/*if(record->TestResult[i].IsOpenFlash)
					{
						c1 = '*';
					}
					else*/
					{
						c1 = 'X';
					}
				}
				if(record->TestResult[i].CloseTemp  >= temp_range.close_temp_min && record->TestResult[i].CloseTemp <= temp_range.close_temp_max)
				{
					/*if(record->TestResult[i].IsCloseFlash)
					{
						c2 = '*';
					}
					else*/
					{
						c2 = ' ';
					}
				}
				else
				{
					/*if(record->TestResult[i].IsCloseFlash)
					{
						c2 = '*';
					}
					else*/
					{
						c2 = 'X';
					}
				}
				sprintf(buf,"%3d   %c %5.1f  %c %5.1f  %5.1f",i +1,c1,record->TestResult[i].OpenTemp,c2,record->TestResult[i].CloseTemp,record->TestResult[i].OpenTemp - record->TestResult[i].CloseTemp);   
				PrintOneLine(buf);
			}
			else if(record->TestResult[i].IsOpenned)
			{
				char c1;
				if(record->TestResult[i].OpenTemp >= temp_range.open_temp_min && record->TestResult[i].OpenTemp <= temp_range.open_temp_max)
				{
					c1 = ' ';
				}
				else
				{
					c1 = 'X';
				}
				sprintf(buf,"%3d   %c %5.1f",i +1,c1,record->TestResult[i].OpenTemp);  
				PrintOneLine(buf);
			} 
			else if(record->TestResult[i].IsClosed)
			{
				char c1;
				if(record->TestResult[i].CloseTemp >= temp_range.close_temp_min && record->TestResult[i].CloseTemp <= temp_range.close_temp_max)
				{
					c1 = ' ';
				}
				else
				{
					c1 = 'X';
				}
				sprintf(buf,"%3d            %c %5.1f",i +1,c1,record->TestResult[i].CloseTemp);  
				PrintOneLine(buf);
			} 
			else
			{
				sprintf(buf,"%3d",i +1);   
				PrintOneLine(buf);
			}

		}
		else
		{
			sprintf(buf,"%3d",i +1);   
			PrintOneLine(buf);
		}

		if(((i +1) % 10) == 0)
		{
			PrintOneLine(" ");
		}


	}

	
	//=======
//温度分布


	if(::G_NormalConfigPara[BoxNr].AutoPrintType == 1)
	{
		Sleep(100);

		int CurOpenTemp  = (int)temp_range.open_temp_min;
		int CurCloseTemp = (int)temp_range.close_temp_min;
		char count_str[40][7]=
		{
			"      ",
			"1     ",
			"2     ",
			"3     ",
			"4     ",
			"5     ",
			"51    ",
			"52    ",
			"53    ",
			"54    ",
			"55    ",
			"551   ",
			"552   ",
			"553   ",
			"554   ",
			"555   ",
			"5551  ",
			"5552  ",
			"5553  ",
			"5554  ",
			"5555  ",
			"55551 ",
			"55552 ",
			"55553 ",
			"55554 ",
			"55555 ",
			"555551",
			"555552",
			"555553",
			"555554",
			"555555",

		};

		sprintf(buf,"<        %s<        %s",count_str[OpenLow],count_str[CloseLow]);
		PrintOneLine(buf);

		while(1)
		{
			char OpenTempStr[100];
			char CloseTempStr[100];

			if((CurOpenTemp > temp_range.open_temp_max) && (CurCloseTemp > temp_range.close_temp_max))
			{
				break;
			}
			
			

			memset(OpenTempStr,0,sizeof(OpenTempStr));
			if(CurOpenTemp < temp_range.open_temp_max)
			{
				sprintf(OpenTempStr,"%03d      %s",	CurOpenTemp,count_str[OpenTempNr[CurOpenTemp]]);
			}
			else if(CurOpenTemp == temp_range.open_temp_max)
			{
				sprintf(OpenTempStr,">        %s",count_str[OpenHigh]);
			}
			else
			{
				strcat(OpenTempStr,"               ");
			}
			

			if(CurCloseTemp < temp_range.close_temp_max)
			{
				sprintf(CloseTempStr,"%03d      %s",	CurCloseTemp,count_str[CloseTempNr[CurCloseTemp]]);
				strcat(OpenTempStr,CloseTempStr);
				
			}
			else if(CurCloseTemp == (int)temp_range.close_temp_max)
			{
				sprintf(CloseTempStr,">        %s",	count_str[CloseHigh]);
				strcat(OpenTempStr,CloseTempStr);
				
			}
			else
			{

			}
			
			PrintOneLine(OpenTempStr);

			CurOpenTemp	++;
			CurCloseTemp ++;
			
			
		}
	}
	
	
	
//温度分布
//=======
	
	
	if(SumOpenTemp != 0)
	{
		if((TotalOpenTemp > 0) && ( TotalCloseTemp > 0 ))
		{
			if(Language == 0)
			{
				sprintf(buf,"最小:   %5.1f    %5.1f  %5.1f",MinOpenTemp,MinCloseTemp,MinDiff);
				PrintOneChineseLine(buf);

							
				sprintf(buf,"最大:   %5.1f    %5.1f  %5.1f",MaxOpenTemp,MaxCloseTemp,MaxDiff);
				PrintOneChineseLine(buf);


				sprintf(buf,"差  :   %5.1f    %5.1f  %5.1f",MaxOpenTemp - MinOpenTemp,MaxCloseTemp - MinCloseTemp,MaxDiff - MinDiff);
				PrintOneChineseLine(buf);
			}
			else
			{
				sprintf(buf,"MIN:   %5.1f    %5.1f  %5.1f",MinOpenTemp,MinCloseTemp,MinDiff);
				PrintOneLine(buf);

							
				sprintf(buf,"MAX:   %5.1f    %5.1f  %5.1f",MaxOpenTemp,MaxCloseTemp,MaxDiff);
				PrintOneLine(buf);


				sprintf(buf,"R  :   %5.1f    %5.1f  %5.1f",MaxOpenTemp - MinOpenTemp,MaxCloseTemp - MinCloseTemp,MaxDiff - MinDiff);
				PrintOneLine(buf);
			}

			
		}
		else if(TotalOpenTemp > 0)
		{
			if(Language == 0)
			{
				sprintf(buf,"最小:   %5.1f",MinOpenTemp);
				PrintOneChineseLine(buf);


				sprintf(buf,"最大:   %5.1f",MaxOpenTemp);
				PrintOneChineseLine(buf);


				sprintf(buf,"差  :   %5.1f",MaxOpenTemp - MinOpenTemp);
				PrintOneChineseLine(buf);
			}
			else
			{
				sprintf(buf,"MIN:   %5.1f",MinOpenTemp);
				PrintOneLine(buf);


				sprintf(buf,"MAX:   %5.1f",MaxOpenTemp);
				PrintOneLine(buf);


				sprintf(buf,"R  :   %5.1f",MaxOpenTemp - MinOpenTemp);
				PrintOneLine(buf);
			}
	
			
		}
	}

	if((TotalOpenTemp != 0) && (TotalCloseTemp != 0))
	{
		if(Language == 0)
		{
			sprintf(buf,"平均:   %5.1f    %5.1f  %5.1f",SumOpenTemp / TotalOpenTemp,SumCloseTemp / TotalCloseTemp,SumOpenTemp / TotalOpenTemp - SumCloseTemp / TotalCloseTemp);
			PrintOneChineseLine(buf);
		}
		else
		{
			sprintf(buf,"AVE:   %5.1f    %5.1f  %5.1f",SumOpenTemp / TotalOpenTemp,SumCloseTemp / TotalCloseTemp,SumOpenTemp / TotalOpenTemp - SumCloseTemp / TotalCloseTemp);
			PrintOneLine(buf);
		}

	}
	else if(TotalOpenTemp != 0)
	{
		if(Language == 0)
		{
			sprintf(buf,"平均:   %5.1f",SumOpenTemp / TotalOpenTemp);
			PrintOneChineseLine(buf);
		}
		else
		{
			sprintf(buf,"AVE:   %5.1f",SumOpenTemp / TotalOpenTemp);
			PrintOneLine(buf);
		}

	}

	if((TotalOpenTemp != 0) && (TotalCloseTemp != 0))
	{
		sprintf(buf,"3Dl :   %5.1f    %5.1f  %5.1f",
			Cal3Sigma(open_data,TotalOpenTemp),
			Cal3Sigma(close_data,TotalCloseTemp),
			Cal3Sigma(diff_data,TotalCloseTemp));
		buf[1] = 0xA6;
		buf[2] = 0xc4; //0xA6C4 为δ
		PrintOneChineseLine(buf);


		sprintf(buf,"Cp  :   %5.1f    %5.1f",
			CalCp(open_data,TotalOpenTemp,temp_range.open_temp_max,temp_range.open_temp_min),
			CalCp(close_data,TotalCloseTemp,temp_range.close_temp_max,temp_range.close_temp_min));
		PrintOneLine(buf);


		sprintf(buf,"Cpk :   %5.1f    %5.1f",
			CalCpk(open_data,TotalOpenTemp,temp_range.open_temp_max,temp_range.open_temp_min),
			CalCpk(close_data,TotalCloseTemp,temp_range.close_temp_max,temp_range.close_temp_min));
		PrintOneLine(buf);


		
	}
	else if(TotalOpenTemp != 0)
	{
		sprintf(buf,"3Dl :   %5.1f",
			Cal3Sigma(open_data,TotalOpenTemp));
		buf[1] = 0xA6;
		buf[2] = 0xc4; //0xA6C4 为δ
		PrintOneLine(buf);


		sprintf(buf,"Cp  :    %5.1f",
			CalCp(open_data,TotalOpenTemp,temp_range.open_temp_max,temp_range.open_temp_min));
		PrintOneLine(buf);


		sprintf(buf,"Cpk :    %5.1f",
			CalCpk(open_data,TotalOpenTemp,temp_range.open_temp_max,temp_range.open_temp_min));
		PrintOneLine(buf);

		
	}
#ifdef _RAINBOW	
//	PrintOneChineseLine("常州市润邦电子科技有限公司");
//	PrintOneChineseLine("服务热线： 13915838598");
#endif

	if(::G_NormalConfigPara[0].AutoPrintForwardType == 0 )
	{

		PrintOneLine("F");

		PrintOneLine("G");

		PrintOneLine("H");

	//	FeedForward(200); 
	}
	else
	{
		//int count = G_PrintLineArray.GetCount() ;
		while(::G_PrintLineArray.GetCount() > 0)
		{
			int size = G_PrintLineArray.GetCount(); 
			PrintLine	pl = G_PrintLineArray.GetAt(size - 1);
			::G_PrintRS232_Comm.WriteComm(pl.buf,strlen(pl.buf)); 
			Sleep(strlen(pl.buf));
			/*if(size < 10)
			{
				CString s,s1;
				s.Format(_T("%d,%d,"),count, size);
				for(int i = 0; i < strlen(pl.buf); i++)
				{
					s1.Format(_T("%X,"),(unsigned char )pl.buf[i]);
					s+=s1;
				}
				AfxMessageBox(s);
			}*/
			G_PrintLineArray.RemoveAt(size - 1); 
		}
	}

}




float CLPM260_Printer::Cal3Sigma(float *data,int data_len)
{
	if(data_len <= 1)
		return -1;

	/*
(1)先算样本均值m: 
m=(1/n)*(a[0]+a[1]+...+a[n-1]) 

(2)再算样本方差sigma^2: 
sigma^2=[1/n]*{(a[0]-m)^2+(a[2]-m)^2+...+(a[n-1]-m)^2} 

(3)把样本方差开根号就是样本标准差sigma 

(4)把sigma乘以3就行了
*/
	float avr = 0;
	float sigma = 0;
	for(int i = 0; i < data_len; i++)
	{
		avr += data[i];
	}
	avr /= data_len;


	for(int i = 0; i < data_len; i++)
	{
		sigma += (avr - data[i]) * (avr - data[i]); 
	}
	
	return  sqrt(sigma / (data_len - 1)) * 3;
	
}


/*
Cp = (USL- LSL)/6Sigma
SC = (USL + LSL)/2
Ca = |平均X - SC| /((USL -LSL) /2 )
Cpk = Cp * (1 -Ca)
*/
float CLPM260_Printer::CalCp(float *data,int data_len,float USL,float LSL)
{
	return (USL - LSL) / Cal3Sigma(data,data_len) / 2;
}

float CLPM260_Printer::CalMax(float *data,int data_len,float USL,float LSL)
{
	float Max = -999.99f;
	for(int i = 0; i < data_len; i++)
	{
		if(data[i] > Max)
		{
			Max = data[i];
		}
	}
	return Max;
}

float CLPM260_Printer::CalMin(float *data,int data_len,float USL,float LSL)
{
	float Min = 999.99f;
	for(int i = 0; i < data_len; i++)
	{
		if(data[i] < Min)
		{
			Min = data[i];
		}
	}
	return Min;
}

float CLPM260_Printer::CalAvr(float *data,int data_len,float USL,float LSL)
{
	float sum = 0;
	for(int i = 0; i < data_len; i++)
	{
		sum += data[i];
	}
	return sum / data_len;
}

float CLPM260_Printer::CalCpk(float *data,int data_len,float USL,float LSL)
{
	float avr = 0;
	for(int i = 0; i < data_len; i++)
	{
		avr += data[i];
	}
	avr /= data_len;

	float Ca = (avr - (USL + LSL)/2) / ((USL -LSL) / 2);

	if(Ca < 0)
	{
		Ca = 0 -Ca;
	}

	return CalCp(data,data_len,USL,LSL) * ( 1 - Ca);
}

void CLPM260_Printer::PrintNoiseTest(void)
{
	char buf[100];

		if(::G_NormalConfigPara[0].AutoPrintForwardType == false)
		{
			sprintf(buf,"%s","\n");
			CLPM260_Printer::PrintOneLine(buf);
			CLPM260_Printer::PrintOneLine(buf);
			CLPM260_Printer::PrintOneLine(buf);
		}
		sprintf(buf,"%s","NOISE TEST");
		CLPM260_Printer::PrintOneLine(buf);
		CTime cur = CTime::GetCurrentTime();
		sprintf(buf,"TEST TIME:%04d/%02d/%02d %02d:%02d:%02d",cur.GetYear(),cur.GetMonth(),cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond());
		CLPM260_Printer::PrintOneLine(buf);
		sprintf(buf,"%s","Nr   MaxNoise  MaxLevel");
		CLPM260_Printer::PrintOneLine(buf);

		unsigned short MaxNoise[2] = {0,0},MinNoise[2] = {999,999};
		for(int i = 0; i < 40; i++)
		{

			if(MaxNoise[0] < G_Noise.MaxCounts[i][0])
			{
				MaxNoise[0] = G_Noise.MaxCounts[i][0];
			}

			if(MinNoise[0] > G_Noise.MaxCounts[i][0])
			{
				MinNoise[0] = G_Noise.MaxCounts[i][0];
			}

			if(MaxNoise[1] < G_Noise.MaxCounts[i][1])
			{
				MaxNoise[1] = G_Noise.MaxCounts[i][1];
			}

			if(MinNoise[1] > G_Noise.MaxCounts[i][1])
			{
				MinNoise[1] = G_Noise.MaxCounts[i][1];
			}
			sprintf(buf,"%2d  %5d  %5d  %5d  %5d",i + 1,G_Noise.MaxCounts[i][0],G_Noise.MaxLevel[i][0]+1,G_Noise.MaxCounts[i][1],G_Noise.MaxLevel[i][1]+1);  
			CLPM260_Printer::PrintOneLine(buf);
		
			Sleep(20);
		}
		sprintf(buf,"Max=%d,%d  Min= %d,%d",MaxNoise[0],MaxNoise[1],MinNoise[1],MinNoise[1]);
		CLPM260_Printer::PrintOneLine(buf);

		if(::G_NormalConfigPara[0].AutoPrintForwardType == 0)
		{
			sprintf(buf,"%s","\n");
			CLPM260_Printer::PrintOneLine(buf);
			CLPM260_Printer::PrintOneLine(buf);
			CLPM260_Printer::PrintOneLine(buf);
		}
		else
		{
			while(::G_PrintLineArray.GetCount() > 0)
			{
				int size = G_PrintLineArray.GetCount(); 
				PrintLine	pl = G_PrintLineArray.GetAt(size - 1);
				::G_PrintRS232_Comm.WriteComm(pl.buf,strlen(pl.buf)); 
				Sleep(strlen(pl.buf));
				G_PrintLineArray.RemoveAt(size - 1); 
			}
		}
}

void CLPM260_Printer::PrintResultHead(int BoxNr,RECORD*	record,unsigned char		BoxType,int Language)
{
	char buf[100],temp_buf[100];
	CTime cur = CTime::GetCurrentTime(); 
  
	
		if(BoxType == BOX_DISC_40 )
		{
			if(Language == 0)
			{
				PrintOneChineseLine("测试机： 常州润邦 RB-DISC-40");
			}
			else
			{
				PrintOneLine("< RESULT BY Changzhoun Rainbow DISC-40>");
			}

		}
		else if(BoxType == BOX_DISC_24 )
		{
			if(Language == 0)
			{
				PrintOneChineseLine("测试机： 常州润邦 RB-DISC-24");
			}
			else
			{
				PrintOneLine("< RESULT BY Changzhoun Rainbow DISC-24>");
			}

		}
		
		else
		{
			if(Language == 0)
			{
				PrintOneChineseLine("< 测试机： TPC-128 润邦>");
			}
			else
			{
				PrintOneLine("< RESULT BY RainBow TPC 128>");
			}
		}
		if(Language == 0)
		{
			sprintf(buf,"打印时间 %0d-%02d-%02d %02d:%02d:%02d",cur.GetYear(),cur.GetMonth(),cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond());      
			PrintOneChineseLine(buf);
		}
		else
		{
			sprintf(buf,"PRINT TIME %0d-%02d-%02d %02d:%02d:%02d",cur.GetYear(),cur.GetMonth(),cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond());      
			PrintOneLine(buf);
		}

		CTime BeginTestTime	= STD_TIME + CTimeSpan(0,0,record->StartTime ,0);

		if(Language == 0)
		{
			sprintf(buf,"开始时间 %0d-%02d-%02d %02d:%02d:%02d",BeginTestTime.GetYear(),BeginTestTime.GetMonth(),BeginTestTime.GetDay(),BeginTestTime.GetHour(),BeginTestTime.GetMinute(),BeginTestTime.GetSecond());      
			PrintOneChineseLine(buf);
		}
		else
		{
			sprintf(buf,"BEGIN TIME %0d-%02d-%02d %02d:%02d:%02d",BeginTestTime.GetYear(),BeginTestTime.GetMonth(),BeginTestTime.GetDay(),BeginTestTime.GetHour(),BeginTestTime.GetMinute(),BeginTestTime.GetSecond());      
			PrintOneLine(buf);
		}

		CTime EndTestTime	= STD_TIME + CTimeSpan(0,0,record->EndTime,0);
		if(Language == 0)
		{
			sprintf(buf,"结束时间 %0d-%02d-%02d %02d:%02d:%02d",EndTestTime.GetYear(),EndTestTime.GetMonth(),EndTestTime.GetDay(),EndTestTime.GetHour(),EndTestTime.GetMinute(),EndTestTime.GetSecond());      
			PrintOneChineseLine(buf);
		}
		else
		{
			sprintf(buf,"END  TIME: %0d-%02d-%02d %02d:%02d:%02d",EndTestTime.GetYear(),EndTestTime.GetMonth(),EndTestTime.GetDay(),EndTestTime.GetHour(),EndTestTime.GetMinute(),EndTestTime.GetSecond());      
			PrintOneLine(buf);
		}

		memset(temp_buf,0,sizeof(temp_buf));
		for(int i = 0; i < sizeof(record->ConfigPara.Name) ; i++)
		{
			temp_buf[i] = record->ConfigPara.Name[i];
		}
		if(Language == 0)
		{
			//memcpy(temp_buf,"90±5",5);
			memset(buf,0,sizeof(buf));
			CString s,s1;
			int len = strlen(temp_buf);
			for(int i = 0; i < len; i++)
			{
				if((unsigned char)temp_buf[i] == (unsigned char)0xB1) //±,转换为中文的0xA1C0
				{
					for(int j = len; j > i+1; j--)
					{
						temp_buf[j] = temp_buf[j-1];
					}
					len++;
					temp_buf[len]	= 0;
					temp_buf[i]		= 0xA1;
					temp_buf[i+1]	= 0xC0;

				}
				s1.Format(_T("%X,"),temp_buf[i]);
				s+=s1;
			}
			
			sprintf(buf,"参数 : %s",temp_buf);
				
			PrintOneChineseLine(buf);
		}
		else
		{
			sprintf(buf,"WORK : %s",temp_buf);
			PrintOneLine(buf);
		}

		if(Language == 0)
		{
			sprintf(buf,"速度 : %5.1f度/Min, %5.1f度/Min",record->HeatSpeed,record->CoolSpeed);
			PrintOneChineseLine(buf);
		}
		else
		{
			sprintf(buf,"Speed: %5.1fC/Min, %5.1fC/Min",record->HeatSpeed,record->CoolSpeed);
			PrintOneLine(buf);
		}
	}
