#ifndef _LPM260_PRINTER_H_
#define _LPM260_PRINTER_H_

#include "typedefs.h"
#include "comm.h"



#define ESC		0x1B
#define LF		0x0A
enum LPM260_CHAR_TYPE
{
	ENGLISH		=	0,	//当n=0选择16×16点阵汉字打印 
	CHINESE		=   15,	//当n=1选择8×16点阵汉字打印 
};

class CLPM260_Printer
{
	public:
	CLPM260_Printer();
	~CLPM260_Printer();

	void SelectCharType(LPM260_CHAR_TYPE CharType);
	void NextLine(void);
	void CLPM260_Printer::FeedForward(char dots );
	static void CLPM260_Printer::SetLineGap(char dots );
	void CLPM260_Printer::SetCharGap(char dots );
	void CLPM260_Printer::SetDarkness(char deep);
	static void CLPM260_Printer::PrinterInit(void);
	static void CLPM260_Printer::PrintOneLine(char * buf);
	static void CLPM260_Printer::PrintOneChineseLine(char * buf);
	void CLPM260_Printer::PrintTestPaper(void);

	void CLPM260_Printer::SetComm(CComm * comm);

	void CLPM260_Printer::PrintNoiseTest(void);
	static void CLPM260_Printer::PrintResultHead(int BoxNr,RECORD*	record,unsigned char		BoxType,int Language);
	void CLPM260_Printer::PrintTestResult(int BoxNr,RECORD*	record,int * m_Group,int group_id,CONTROL_TEMP_RANGE temp_range,unsigned char		BoxType,int Language);
	static float CLPM260_Printer::Cal3Sigma(float *data,int data_len);
	static float CLPM260_Printer::CalCp(float *data,int data_len,float USL,float LSL);
	static float CLPM260_Printer::CalCpk(float *data,int data_len,float USL,float LSL);
	static float CLPM260_Printer::CalAvr(float *data,int data_len,float USL,float LSL);
	static 	float CLPM260_Printer::CalMax(float *data,int data_len,float USL,float LSL);
	static 	float CLPM260_Printer::CalMin(float *data,int data_len,float USL,float LSL);
	static void CLPM260_Printer::EnterChineseMode(void);
	static void CLPM260_Printer::ExitChineseMode(void);
	static void CLPM260_Printer::SetPaperType(char type);
	//CComm*	m_Comm;
	
};

extern CPrintLineArray	G_PrintLineArray;;

#endif