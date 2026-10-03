#ifndef _LPM24_PRINTER_H_
#define _LPM24_PRINTER_H_

#include "typedefs.h"
#include "comm.h"



#define ESC		0x1B
#define LF		0x0A
enum PM24_CHAR_TYPE
{
	PM24_ENGLISH		=	0,	//当n=0选择16×16点阵汉字打印 
	PM24_CHINESE		=   15,	//当n=1选择8×16点阵汉字打印 
};

class CPM24_Printer
{
	public:
	CPM24_Printer();
	~CPM24_Printer();

	void SelectCharType(PM24_CHAR_TYPE CharType);
	void NextLine(void);
	void CPM24_Printer::FeedForward(char dots );
	static void CPM24_Printer::SetLineGap(char dots );
	void CPM24_Printer::SetCharGap(char dots );
	void CPM24_Printer::SetDarkness(char deep);
	static void CPM24_Printer::PrinterInit(void);
	static void CPM24_Printer::PrintOneLine(char * buf);
	static void CPM24_Printer::PrintOneChineseLine(char * buf);
	static void CPM24_Printer::PrintTestPaper(void);

	void CPM24_Printer::SetComm(CComm * comm);

	void CPM24_Printer::PrintNoiseTest(void);
	static void CPM24_Printer::PrintResultHead(int BoxNr,RECORD*	record,unsigned char		BoxType,int Language);
	void CPM24_Printer::PrintTestResult(int BoxNr,RECORD*	record,int * m_Group,int group_id,CONTROL_TEMP_RANGE temp_range,unsigned char		BoxType,int Language);
	static float CPM24_Printer::Cal3Sigma(float *data,int data_len);
	static float CPM24_Printer::CalCp(float *data,int data_len,float USL,float LSL);
	static float CPM24_Printer::CalCpk(float *data,int data_len,float USL,float LSL);
	static float CPM24_Printer::CalAvr(float *data,int data_len,float USL,float LSL);
	static 	float CPM24_Printer::CalMax(float *data,int data_len,float USL,float LSL);
	static 	float CPM24_Printer::CalMin(float *data,int data_len,float USL,float LSL);
	static void CPM24_Printer::EnterChineseMode(void);
	static void CPM24_Printer::ExitChineseMode(void);
	static void CPM24_Printer::SetPaperType(char type);
	CComm*	m_Comm;
	
};

extern CPrintLineArray	G_PrintLineArray;;

#endif