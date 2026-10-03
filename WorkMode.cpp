
#include "stdafx.h"

#ifdef WINCE

#include "HeatBoxDlg.h"
#include "MessageBox.h"
#include "WelcomeDlg.h"
#include "InputAutoAdjustTempDlg.h"
#include "InputGlassTempDlg.h"
#include "InputDlg.h"
#else

#include "resource.h"
#include "SwitchDlg.h"
#include "DlgInfomation.h"
#include "HeatBoxDlg.h"
#include "InputGlassTempDlg.h"
#include "InputAutoAdjustTempDlg.h"
#include "inputDlg.h"
#endif
#include "typedefs.h"
#include "common.h"
#include "comm.h"
#include "LPM260_Printer.h"
#include <shlwapi.h>
#include <winbase.h>
#include "WorkMode.h"
#ifndef WINCE
#include "RecordViewDlg.h"
#include "HttpTools.h"

#include "DongguanBetterMesMsgDlg.h"
#include "TopbandVietnamMesDlg.h"

#endif
CString  ModifyMsg = 
#ifdef WINCE
_T("\
2021/12/24 修改反复开关门测动闪时关门升温的指示温度狂升的错误 \r\n  \
2021-12-21 修正有时候温度值只显示一部分的问题,开机先进入校温界面再入其他界面就会有此问题 \r\n\
2021-12-15 上位机程序自带线路板程序，方便在线升级 \r\n\
2021-11-05 修改了制冷测试过程中的Bug \r\n\
2021-08-25 可以从老程序中恢复 \r\n\
2020-10-28 开门复位的判断在动作温度区间低于复位温度区间时无效 \r\n\
2020-08-23 在有工位修正值以后，自动调整过加热度数 \r\n\
2019-12-11 自动校温前清除温度总调 \r\n\
2019-12-08 压缩测试记录文件，将保存的记录数量提升到24000条 \r\n\
2019-11-17 修改拔板界面的显示测试时间错误 \r\n\
2019-10-11 显示硬盘剩余空间大小，可保存测试记录条数 \r\n\
2019-9-19 可调KST在调整温度的过程中不判断闪动 \r\n\
2019-8-17 增加闪动测试开关 \r\n\
2019-8-17 历史记录可以批量删除 \r\n\
2019-08-07  OMRON温度表也可以校温补偿  \r\n\
2019-06-01  降温也可以恒温，降温速度可控，可以进行热补偿  \r\n\
2019-05-01 增加KSD302x2  \r\n\
2019-02-22 制冷和制热改为双PID参数  \r\n\
2019/01/25 根据自动校温过程,确定各个温度点按一定速度升温所需要的加热大概比例 \r\n\
2019/01/20 增加OMRON仪表测温,同时测试32路可调温度控制器 \r\n\
2019/01/20 对低温品:增加降温测动作温度,升温测复位温度的功能 \r\n\
2018/12/17 可以在历史记录里,把历史记录一个个的保存到U盘 \r\n\
2018/12/11 可以看到每个发送命令的文字提示,方便查找错误 \r\n\
2018/10/18 每个工位可以设置动作复位温度修正值 \r\n\
2018/10/17 可以临时修改参数 \r\n\
2018/09/09   增加复位温度类型：温度范围+温度差 \r\n\
2018/08/21 自动更新触摸屏程序,同时备份老程序到U盘 \r\n\
2018/07/04 更正了历史记录里数据曲线显示错误 \r\n\
2018/06/21 增加重复测试功能，重复测试次数在参数里设置 \r\n\
2018/05/15 历史记录同时以Excel文件和Bmp图片保存到U盘 \r\n\
2018/05/14 可以删除历史记录 \r\n\
2018/05/14 在参数界面增加一个选项,空调降温回升的时候选择是 匀速升温还是快速升温 \r\n\
2018/05/09 将PTC的整合到一个版本 \r\n\
2018/03/07 对于空调系统,先降温后的全速升温改为恒温,空调不关闭 \r\n\
2018/03/04 RS232也可以控制系统 \r\n\
2018/01/06 加了扬州英谛的功能 \r\n\
2017/12/23 优化了温度分布图,低于最低或高于最高测试温度的一个放到一个格子显示 \r\n\
2017/12/08 优化了通信协议，程序更加简洁 \r\n\
2017/12/08 同时支持128点，192点，KSD302 \r\n\
2017/11/30 增加打印 3Dlta,Cp,Cpk \r\n\
2017/11/30 增加分组打印 \r\n\
2017/11/14 增加屏幕分辨率的自适应功能 \r\n\
2017/11/10 增加插入U盘自动更新的功能 \r\n\
2017/11/08 复位温度增加动复位温度差 \r\n\
2017/11/05 增加票据打印机 \r\n\
2017/11/02 空调降温，配合电磁阀控制降温速度 \r\n\
2017/11/02 增加大仪表面板 \r\n\
2017/10/25 参数全部保存为文件，没有数量的限制 \r\n\
2016-10-03 增加修改说明 \r\n ");
#else
_T(" \
2023/12/11 测复闪再升温后，根据设置关闭或者开门\r\n \
2022/10/18 增加历史记录的分类输出 \r\n \
2022/06/29 增加参数设置的密码 \r\n \
2022/05/10 增加双路保护器 KW-2T03 \r\n \
2021/12/28 上位机程序自带线路板程序，方便在线升级 \r\n \
2021/12/24 修改反复开关门测动闪时关门升温的指示温度狂升的错误 \r\n \
2021/08/24 增加双片测试功能 \r\n \
2021/08/24 修复工位温度修正 \r\n \
2021/07/15 修改复位温度类型为\"动复温差\"时的复位下限的bug \r\n \
2021/07/14 PTC产品不测PTC时，以PTC的上限电阻值判断有无产品 \r\n \
2021/07/14 全面启用整合版本，生产界面显示仪表盘 \r\n \
2021/03/03 192点出现通讯的错误代码 \r\n \
2021/02/15 温度分布直方图上显示的统计数字旋转90°显示 \r\n \
2020/12/14 增加分组打印 \r\n \
2020/09/23 在生产界面增加快捷键 F1--F5表示5个烘箱开始测试，F6--F10表示5个烘箱的拔板 \r\n \
2020/09/23 在拔板界面增加快捷键 PageUp 和 PageDown,进行规格选择 \r\n \
2020/09/23 [关于界面]用F12快捷键 \r\n \
2020/03/03 校温时自动开关门  \r\n \
2020/02/24 在关于界面，自动搜索计算机上可用的串口  \r\n \
2020/02/23 增加保存测试过程的温度曲线，在历史记录里打开温度曲线；同时，在测试过程中双击温度曲线，可以全屏幕显示曲线  \r\n \
2019/12/11 自动校温前清除温度总调 \r\n \
2019/11/04 程序只能在计算机的 program files目录下运行，否则就拷贝到这个目录再运行 \r\n \
2019/11/04 可以统计产量 \r\n \
2019/10/11 历史记录改成以时间目录保存，方便查找 \r\n \
2019/08/12 在复位状态下也判断有无PTC,有的PTC电阻太大，超出了线路板上的测试量程，只能等降温回到量程内再测试 \r\n \
2019/05/18 程序可以自己拷贝自己到program files 目录，并且在桌面建立快捷方式 \r\n \
2019/04/27 可以关闭闪动测试功能 \r\n \
2019/02/10 取消取消电源管理，避免睡眠、待机功能 \r\n \
2019/01/10 修改串口号后立即生效 \r\n \
2018/10/29 增加自整定功能,在调试界面,AT按钮左边输入整定温度,再点AT就行 \r\n \
2018/10/06 校温稳定度为90秒不大于0.4度 \r\n \
2018/10/06 可以在计算机上设置控制箱编号 \r\n \
2018/09/10 修改错误:程序运行时,没有显示温度修正值 \r\n \
2018/09/08 判断温度连续10次相同的话,发送命令,强行停止加热 \r\n \
2018/08/27 参数类型增加：复位温度范围和温度差同时起作用 \r\n \
2018/08/10 修改错误:参数为温度差时,复大品判断错误 \r\n \
2018/03/12 增加拔板界面的即时打印功能 \r\n \
2018/03/08 保存为Excel文件时,增加标题 \r\n \
2018/03/07 修改错误:保存为Excel文件时,产品序号没有排序 \r\n \
2017/12/23 优化了温度分布图,低于最低或高于最高测试温度的一个放到一个格子显示 \r\n \
2017-12-20 优化了控制命令的转发方式  \r\n \
2017-12-20 优化了通讯数据的读取和处理方式  \r\n \
2017-08-31 历史记录自动保存到 D:\\\\历史记录\\  目录下  \r\n \
2017-08-23 数据库全部改为文件方式 \r\n \
2017-08-06 修改查看历史记录时，拔板显示错误的问题 \r\n \
2017-08-06 拔板错误时，也显示产品编号 \r\n \
2017-08-03 修改在打开温度分布图的时候，通信中断的错误 \r\n \
2017-07-09 增加降温速度控制 \r\n \
2017-04-24 在选择不测试闪动后，屏蔽动闪和复闪  \r\n \
2017-04-24 增加高温品(常胜需要) \r\n \
2017-04-24 恢复离线拔板 \r\n \
2017-03-24 开始测试后，拔板按钮显示为黑色的\"在线查看\",测试结束后显示为红色的|\"在线拔板\" \r\n \
2017-03-20 开门复位的温差列表框是0-10度，程序里是0-5度，统一为0-10度 \r\n \
2017-03-01 打印时，可以选择只打印合格品或者主规格 \r\n \
2017-02-16 MODBUS和ASCII通信可以自由选择 \r\n \
2017-02-16 通信命令没有收到仪表的响应，会一直重发，保证联机 \r\n \
2017-02-05 历史记录也可以用拔板查看 \r\n \
2017-02-05 不用控件自绘温度曲线和温度分布图 \r\n \
2017-02-02 当复位温度最大最小值相同时，可以单测动作温度 \r\n \
2017-02-02 输入框改为软键盘操作 \r\n \
2017-01-13 128点，192点合并成一个版本 \r\n \
2017-01-11 修改错误：在只选一个测试规格的时候，低温品等全部归到了不动作里面 \r\n \
2016-12-23 增加功能：所有动作的产品都复位了才停止测试  \r\n \
2016-12-20 温度分布图数量上加入不动作和不复位的统计  \r\n \
2016-12-18 温度分布图上加上数量  \r\n \
2016-12-18 测试记录查询列表加上网格  \r\n \
2016-12-15 测试记录查询需要同时判断结束时间  \r\n \
2016-12-08 报表添加测试说明  \r\n \
2016-12-08 报表可以按照温度排序  \r\n \
2016-12-07 调试：PID恒温可以设置时间，时间单位为秒  \r\n \
2016-12-07 温度分布图添加百分比显示  \r\n \
2016-12-06 在每次开始测试时增加备注一项，方便记录查询  \r\n \
2016-12-06 测试记录里增加备注一项，方便记录查询  \r\n \
2016-12-06 输入数字对话框可以输入负数  \r\n \
2016-11-29 查询测试结果左列表里增加 测试参数 项  \r\n \
2016-10-29 对话框标题 可以在AboutDlg 下任意修改 \r\n \
2016-10-22 修改拔板时分隔线闪烁的问题 	\r\n \
2016-10-22 修改拔板时候温度的显示颜色 	\r\n \
2016-10-17 没有校温不能进行测试 	\r\n \
2016-10-08 一个窗口的参数修改立即在其他烘箱也生效 	\r\n \
2016-10-08 打印报表使用的\"公司名称\" 可以在AboutDlg 下任意修改 \r\n \
2016-10-03 增加修改说明 \r\n ");
#endif

extern int		G_iCmdAscii[MAX_HEAT_BOX];

CWorkMode::CWorkMode(void)
{
	//memset(&m_WorkStatus,0,sizeof(m_WorkStatus)); 
	
	m_SubMode = SUB_MODE_STOP;

	return;	
}
CWorkMode::~CWorkMode(void)
{

}


void CWorkMode::SetTestMode(int TestMode)
{

	m_iTestMode				=	TestMode;

}

SUB_MODE CWorkMode::AutoTestStep_Ready(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
	//如果现在的温度已经达到目标温度，就进入下一步
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	if(G_NormalConfigPara[BoxNr].BoxType == BOX_OIL_BATHS)
	{//油槽，油浴，恒温到动作温度前

		if(CurTemp < GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr))
		{
			//m_SubMode				=	SUB_MODE_PID;
			m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
			float goal = GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr);
			if(G_Meter[BoxNr].GetCurSv() != goal)
			{
				if(G_Meter[BoxNr].m_PcbTestMode != PCB_SUB_MODE_NORMAL_PID)
				{
					//G_Meter[BoxNr].m_PcbTestMode = PCB_SUB_MODE_NORMAL_PID;
					//G_RS485_Comm.SendNormalPidCmd(goal,BoxNr); 
				}
			}
			//CString msg;
			//G_RS485_Comm.SendAirControlCmd(false,G_Meter[BoxNr].m_IsAcOn,BoxNr,&msg); 
		}
		else if(CurTemp < (GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr) + 1.0f))
		{
			if(G_Meter[BoxNr].m_IsDoorOpened == true)
			{
				G_Meter[BoxNr].m_IsDoorOpened = false;
				G_RS485_Comm.SendStopHeatCmd(-50.0f,BoxNr);
				G_RS485_Comm.SendOpenDoorCmd(false,BoxNr);
			}
		}
		else if(CurTemp > (GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr) + 2.0f))
		{
			if(G_Meter[BoxNr].m_IsDoorOpened == false)
			{
				G_Meter[BoxNr].m_IsDoorOpened = true;
				G_RS485_Comm.SendStopHeatCmd(-50.0f,BoxNr);
				G_RS485_Comm.SendOpenDoorCmd(true,BoxNr);
			}
		}

	}
	else if(G_NormalConfigPara[BoxNr].AirControlEquipment == 0)
	{
		//AfxMessageBox(_T("a-1"));
		m_SubMode				=	SUB_MODE_DIRECT_HEAT;
		m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
		G_RS485_Comm.SendDirectHeatCmd(GetControlGoalTemp(SUB_MODE_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr),BoxNr); 
	}
	else
	{
		if(CurTemp < GetControlGoalTemp(SUB_MODE_STEP_COOL,HeatCool,false,m_TempRange,BoxNr))
		{//AfxMessageBox(_T("a0"));
			m_SubMode				=	SUB_MODE_PID;
			m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
			float goal = GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr);
			G_RS485_Comm.SendNormalPidCmd(goal,BoxNr); 
			CString msg;
			G_RS485_Comm.SendAirControlCmd(false,G_Meter[BoxNr].m_IsAcOn,BoxNr,&msg); 
		}
		else
		{
			float goal = GetControlGoalTemp(SUB_MODE_STEP_COOL,HeatCool,false,m_TempRange,BoxNr);
			if(G_Meter[BoxNr].m_Sv == 0.0f)
			{
				G_Meter[BoxNr].m_Sv = goal;
				G_RS485_Comm.SendStopHeatCmd(goal,BoxNr);
			}
			if(CurTemp > 40.0f)
			{
				if(G_Meter[BoxNr].m_IsDoorOpened == false)
				{
					G_Meter[BoxNr].m_IsDoorOpened = true;
					G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
				}
			}
			else
			{
				if(G_Meter[BoxNr].m_IsDoorOpened == true)
				{
					G_Meter[BoxNr].m_IsDoorOpened = false;
					G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); 
				}
			}
			CTime cur = CTime::GetCurrentTime();
			CTimeSpan span = cur - this->m_CurSubModeBeginTime; 
			int sec = span.GetTotalSeconds();
			int minutes = (sec / 60);
			sec = (sec % 60);
			MainWnd->m_TestMsg.Format(_T("全速降温到 %5.1f度 %d:%02d"),GetControlGoalTemp(SUB_MODE_STEP_COOL,HeatCool,false,m_TempRange,BoxNr),minutes,sec );
			MainWnd->UpdateData(false);

			StartAirControl(ParentWnd,BoxNr,CurTemp);

			
		}
	}

	return m_SubMode;

}
SUB_MODE CWorkMode::AutoTestStep_DirectHeat(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
	//如果现在的温度已经达到目标温度，就进入下一步
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	if((CurTemp + 0.3f )>=  GetControlGoalTemp(SUB_MODE_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr))
	{

		m_SubMode = SUB_MODE_PID;
			
		::G_RS485_Comm.SendNormalPidCmd(GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr),BoxNr);

		m_CurSubModeBeginTime = CTime::GetCurrentTime(); 

		
	}
	else
	{
		float goal_temp = GetControlGoalTemp(SUB_MODE_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr);
		CTime cur = CTime::GetCurrentTime();
		CTimeSpan span = cur - this->m_CurSubModeBeginTime; 
		int sec = span.GetTotalSeconds();
		int minutes = (sec / 60);
		sec = (sec % 60);
		MainWnd->m_TestMsg.Format(_T("全速升温到 %5.1f度 %d:%02d"),goal_temp,minutes,sec );
		MainWnd->UpdateData(false);

		if(::G_Meter[BoxNr].m_PcbTestMode != PCB_SUB_MODE_DIRECT_HEAT)
		{
			//G_RS485_Comm.SendDirectHeatCmd(goal_temp,BoxNr);
			//G_Meter[BoxNr].m_PcbTestMode = PCB_SUB_MODE_DIRECT_HEAT;
		}
		
	}

	return m_SubMode;
}

void CWorkMode::Timer_Cal(CWnd * ParentWnd,int BoxNr,float CurTemp[4])
{
	
	#ifdef WINCE
		CCalDlg * MainWnd = (CCalDlg*)ParentWnd;
	#else
		CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
	#endif

		switch(m_SubMode)
		{
		case SUB_MODE_CAL_LOW:
			this->AutoTestStep_CalLow(ParentWnd,BoxNr,CurTemp); 
			break;
		case SUB_MODE_CAL_HIGH:
			this->AutoTestStep_CalHigh(ParentWnd,BoxNr,CurTemp); 
			break;
		case SUB_MODE_CAL_INPUT_GLASS_TEMP:
			
			break;
		}

}

static int G_PLC_READ_ID = 0;

void CWorkMode::Timer_Sorter(void)
{


	CComm *comm_PLC;
	CComm *comm_8TEMP_8SSR;
#ifdef WINCE
		comm_PLC = &G_PrintRS232_Comm;
		comm_8TEMP_8SSR = &G_RS485_Comm;
#else
		comm_PLC = &G_RS485_Comm;
		comm_8TEMP_8SSR = &G_RS485_Comm;
#endif
	
	if(G_Delta_PLC_SetCmdArray.GetSize() > 0)
	{
		DELTA_PLC_SET_CMD cmd = G_Delta_PLC_SetCmdArray.GetAt(0);

		comm_PLC->WriteComm((char*)&cmd,sizeof(cmd));

		if(G_SorterPlcDlg.GetSafeHwnd())
		{
			if(G_SorterPlcDlg.IsWindowVisible())
			{
				G_SorterPlcDlg.DisplaySendCmd(cmd.head.Cmd[1],(unsigned char*)&cmd,sizeof(cmd),0);
			}
		}

		G_Delta_PLC_SetCmd  = cmd;
		
		memset(&G_Delta_PLC_ReadCmd,0,sizeof(G_Delta_PLC_ReadCmd));
		G_Delta_PLC_SetCmdArray.RemoveAt(0);
		return;
	}

	if(G_SendmCmdProgram.GetCount() > 0)
	{
		MODBUS_RS485_CMD_PROGRAM cmd = G_SendmCmdProgram.GetAt(0); 

		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
		G_CommProtocolType = COMM_PROTOCOL_MODBUS_PROGRAM;

		

		comm_8TEMP_8SSR->WriteComm((char*)&cmd,sizeof(cmd));
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),0); 
#else
		for(int i = 0; i < ::G_CurMaxHeatBox; i++)
		{
			DisplaySendCmd((CWnd*)&G_MainDlg->m_DebugDlg[i],cmd.head.cmd,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),i); 
		}
#endif
		return;
	}

	
	if(m_CmdArray_128[0].GetSize() > 0)
	{
			static MODBUS_RS485_CMD_128 cmd_128;
			cmd_128 = m_CmdArray_128[0].GetAt(0);  
			G_CommProtocolType = COMM_PROTOCOL_MODBUS_128;
			G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
			G_RS485_Comm.WriteComm((char*)&cmd_128,sizeof(cmd_128));
			return;
	}
	
	
#ifdef WINCE
	G_PLC_READ_ID++;
	if(G_PLC_READ_ID > 4)
	{
		G_PLC_READ_ID = 0;
	}
	switch(G_PLC_READ_ID)
	{
	case 0:
		
		comm_PLC->Delta_Read_X();
			break;
	case 1:
		comm_PLC->Delta_Read_D32();
			break;
	case 2:
		comm_PLC->Delta_Read_C_7A();
		break;
	case 3:
//		comm_PLC->Delta_Read_M60();
			break;
	case 4:
		
		comm_PLC->Delta_Read_Y();
			break;

	}

	MODBUS_RS485_CMD_128 *cmd = comm_8TEMP_8SSR->SendReadTemp(0);
	if(G_DebugDlg.GetSafeHwnd() && G_DebugDlg.IsWindowVisible())
	{
		DisplaySendCmd((CWnd*)&G_DebugDlg,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(0));
	}
#else

	G_PLC_READ_ID++;
	if(G_PLC_READ_ID > 5)
	{
		G_PLC_READ_ID = 0;
	}
	switch(G_PLC_READ_ID)
	{
	case 0:
		
		comm_PLC->Delta_Read_X();
			break;
	case 1:
		comm_PLC->Delta_Read_D32();
			break;
	case 2:
		comm_PLC->Delta_Read_C_7A();
		break;
	case 3:
		comm_PLC->Delta_Read_M(0x60,0x10);
			break;
	case 4:
		
		comm_PLC->Delta_Read_Y();
			break;

	case 5:
		MODBUS_RS485_CMD_128 *cmd = comm_8TEMP_8SSR->SendReadTemp(0);
		DisplaySendCmd((CWnd*)&G_MainDlg->m_DebugDlg[0] ,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(0));

		break;
	}

	
#endif
	
}

void CWorkMode::Timer(CWnd * ParentWnd,int BoxNr,float CurTemp[4],SWITCH_CONFIG_PARA	SwitchConfigPara)
{


#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	float PidTemp = (CurTemp[0] + CurTemp[1] + CurTemp[2] + CurTemp[3]) / 4.0f;

	

	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	CString cmdStr;
	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;

	if(G_Meter[BoxNr].m_IsBeepOn)
	{
		CTimeSpan ts;
		ts = CTime::GetCurrentTime() - G_RS485_Comm.m_BeepStartTime[BoxNr];
		int sec = ts.GetTotalSeconds();
		if(sec > 20)
		{
			if(m_CmdArray_128[BoxNr].GetSize() > 0)
			{
				MODBUS_RS485_CMD_128 cmd = m_CmdArray_128[BoxNr].GetAt(0);
				if(cmd.head.cmd != CMD_SET_BEEP)
				{
					G_RS485_Comm.SendBeepCmd(false,BoxNr);
				}
			}
			else
			{
				G_RS485_Comm.SendBeepCmd(false,BoxNr);
			}
		}
	}

	if(G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_24
		|| G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_16
		|| G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40)
	{
		MainWnd->DrawBiMetalLED(); 
	}
#ifndef WINCE 
	else if(G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_128_OLD_DISPLAY && (::G_iMaxLedNr[BoxNr] == 128))
	{
		MainWnd->DrawLED_128_OLD_DISPLAY();
	}

#endif
	else
	{
		MainWnd->DrawLED();
	}

	Timer_Cal(ParentWnd,BoxNr,CurTemp);

	cur_t = cur_t.GetCurrentTime();

	if(MainWnd->m_IsTaking == true)
	{
		G_Meter[BoxNr].m_MeterAnswerTime =  CTime::GetCurrentTime();
	}

	span = CTime::GetCurrentTime() - G_Meter[BoxNr].m_MeterAnswerTime;

	
	if( span.GetTotalSeconds() > 20)
	{
		MainWnd->m_TestTimeMsg.Format(_T("#%d 温度仪表无响应!"),BoxNr+1);
		
		span = CTime::GetCurrentTime() - G_Meter[BoxNr].m_LedAnswerTime[0];
	
		if( (span.GetTotalSeconds() % 30) == 29)
		{
			//WriteLogFile("灯板无响应!",BoxNr+1,true);
		}
		if( span.GetTotalSeconds() > 20)
		{
			MainWnd->m_TestTimeMsg = _T("温度仪表、灯板无响应!");
		}
		MainWnd->UpdateData(false);
	
	}
	else
	{
	
		if(MainWnd->m_TestTimeMsg.Find(_T("无响应")) >= 0)
		{
			MainWnd->m_TestTimeMsg = _T("");
			MainWnd->UpdateData(false);
		}
	}

		
	if(m_SubMode != SUB_MODE_STOP)
	{//自动测试状态
		m_EndTestTime = CTime::GetCurrentTime();
		
		CTimeSpan ts = m_EndTestTime - m_BeginTestTime;
		
		if(ts.GetTotalMinutes() < 0)
		{//不允许时间有夫的，安诺电子反应有这现象，但不知道原因
			m_BeginTestTime = CTime::GetCurrentTime();
			m_EndTestTime	= CTime::GetCurrentTime();

		}
		MainWnd->m_TestTimeMsg.Format(_T("本次测试用时 %02d:%02d:%02d"),ts.GetHours() ,ts.GetMinutes() ,ts.GetSeconds() );
	
		
		if(HeatCool == HEAT_FIRST_COOL_SECOND)
		{
			//switch(m_SubMode)
			switch(m_SubMode)
			{//判断是否要进入下一个测试阶段
				case SUB_MODE_READY:
					AutoTestStep_Ready(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
				break;
				case SUB_MODE_DIRECT_HEAT:
					AutoTestStep_DirectHeat(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					break;
				case SUB_MODE_PID: //恒温
					AuotTestStep_NormalPID(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					break;
				case SUB_MODE_STEP_PID://匀速加热
					AutoTestStep_StepPID(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					
					break;
				case SUB_MODE_PID_2: //恒温
					AuotTestStep_NormalPID_2(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					break;
				case SUB_MODE_STEP_PID_2://匀速加热
					AutoTestStep_StepPID_2(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					break;
				case SUB_MODE_DIRECT_COOL: //全速降温
					
					AutoTestStep_DircetCool(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					break;
				case SUB_MODE_STEP_COOL: //匀速降温
					
					AutoTestStep_StepCool(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					break;
				case SUB_MODE_PID_COOL: //匀速降温
					AutoTestStep_PidCool(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
						break;
				case SUB_MODE_HEAT_AGAIN:

					AutoTestStep_HeatAgain(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					
					break;
				case SUB_MODE_STEP_TOP_PID:
					AutoTestStep_TopPID(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
					break;
			}
		}
		else if(HeatCool == COOL_FIRST_HEAT_SECOND)
		{
			switch(m_SubMode)
			{
			case SUB_COOL_FIRST_MODE_READY:
				AutoTestStep_CoolFirst_Ready(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
				break;
			case SUB_COOL_FIRST_MODE_DIRECT_COOL:
				AutoTestStep_CoolFirst_DirectCool(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
				break;
			case SUB_COOL_FIRST_MODE_COOL_PID:
				AutoTestStep_CoolFirst_CoolPid(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
				break;
			case SUB_COOL_FIRST_MODE_STEP_COOL_PID:
				AutoTestStep_CoolFirst_StepCoolPid(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
				break;
			case SUB_COOL_FIRST_MODE_HEAT_PID:
				AutoTestStep_CoolFirst_HeatPid(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
				break;
			case SUB_COOL_FIRST_MODE_STEP_HEAT_PID:
				AutoTestStep_CoolFirst_StepHeatPid(MainWnd,BoxNr,PidTemp,SwitchConfigPara);
				break;
			}
		}

	}

}

bool CWorkMode::IsAllOpened(int BoxNr)
{
	int i;
#ifdef _CHENG_HONG_
	return false;
#endif
	if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40)
	{
		for(i = 0; i < 40; i++)
		{
			if((G_SwitchTestResult[BoxNr][i].IsUsed == true) &&  (G_SwitchTestResult[BoxNr][i].IsOpenned == false)) 
			{
				return false;
			}

		}
	}
	else if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_24)
	{
		for(i = 0; i < 24; i++)
		{
			if((G_SwitchTestResult[BoxNr][i].IsUsed == true) &&  (G_SwitchTestResult[BoxNr][i].IsOpenned == false)) 
			{
				return false;
			}

		}
	}
	else if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_16)
	{
		for(i = 0; i < 16; i++)
		{
			if((G_SwitchTestResult[BoxNr][i].IsUsed == true) &&  (G_SwitchTestResult[BoxNr][i].IsOpenned == false)) 
			{
				return false;
			}

		}
	}
	else
	{
		for(i = 0; i < G_iMaxLedNr[BoxNr]; i++)
		{
			if((G_SwitchTestResult[BoxNr][i].IsUsed == true) &&  (G_SwitchTestResult[BoxNr][i].IsOpenned == false)) 
			{
				return false;
			}

		}
	}
	return true;
}

SUB_MODE CWorkMode::AuotTestStep_NormalPID(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{

#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;

	
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	cur_t	=	CTime::GetCurrentTime(); 
	span	=	cur_t - m_CurSubModeBeginTime;
	
	int seconds  = span.GetTotalSeconds();

	if(IsAllOpened(BoxNr))
	{
		ExitAutoTest(MainWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode);

	}
	else if((CurTemp + 2.0f)< GetControlGoalTemp(SUB_MODE_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr))
	{//低于直接加热的温度不计时间
		m_CurSubModeBeginTime	= CTime::GetCurrentTime(); 
		MainWnd->m_TestMsg.Format(_T("加热到 %5.1f度"),GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr));
		MainWnd->UpdateData(false);

		if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_24 
			|| ::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_16
			|| ::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40)
		{
			
			if(G_Meter[BoxNr].m_PcbTestMode != PCB_SUB_MODE_INCREASE_PID)
			{
				//float goal = GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr);
				//G_RS485_Comm.SendStepPIDCmd(goal,(goal - CurTemp)/10.0f, BoxNr);
				//G_Meter[BoxNr].m_PcbTestMode = PCB_SUB_MODE_INCREASE_PID;
			}
		}
		else
		{
			if(::G_Meter[BoxNr].m_PcbTestMode != PCB_SUB_MODE_NORMAL_PID)
			{
			//	G_RS485_Comm.SendNormalPidCmd(GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr),BoxNr);
			//	G_Meter[BoxNr].m_PcbTestMode = PCB_SUB_MODE_NORMAL_PID;
			}
		}

	}
	//如果时间已到，就进入下一步
	else if((seconds / 60) >= G_NormalConfigPara[BoxNr].PidTime)
	{
		if(G_NormalConfigPara[BoxNr].TestMode == TEST_MODE_NORMAL_PID_CAL_OPEN_TIME)
		{
			m_SubMode	= SUB_MODE_STOP;
		}
		else
		{
			//m_OpenDoorIng	= false;

			m_OpenDoor_Temp			= GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr) + ::G_NormalConfigPara[BoxNr].OpenDoorReset_HeatDegree; 

			m_SubMode				= SUB_MODE_STEP_PID;

			m_CurSubModeBeginTime	= CTime::GetCurrentTime(); 
			 
			float goal =  GetControlGoalTemp(SUB_MODE_STEP_PID,HeatCool,false,m_TempRange,BoxNr);
			float speed	= G_NormalConfigPara[BoxNr].HeatSpeed / 10.0f;
			if(speed <= 0)
			{
				speed = 1.0f;
			}
			int test_minutes = (goal - GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr)) / speed;
			
			if(test_minutes < 1)
			{
				test_minutes = 1;
			}

			::G_RS485_Comm.SendStepPIDCmd(  goal ,test_minutes,BoxNr);
			

	
#ifdef WINCE
			CString msg;
			G_RS485_Comm.SendAirControlCmd(false,G_Meter[BoxNr].m_IsAcOn,0,&msg);
				
#else
				CString msg;
				G_RS485_Comm.SendAirControlCmd(false,G_Meter[BoxNr].m_IsAcOn,MainWnd->m_BoxNr,&msg);
#endif
		}
		
	}
	else
	{
		if(G_NormalConfigPara[BoxNr].TestMode == TEST_MODE_NORMAL_PID_CAL_OPEN_TIME)
		{
			if((CurTemp + 0.2f) >= GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr))
			{//开始计时
				if(m_CurSubModeBeginTime == 0)
				{
					m_CurSubModeBeginTime = CTime::GetCurrentTime();
				}
			}
		}


		int RemainTime = G_NormalConfigPara[BoxNr].PidTime * 60 - seconds; 
		//char buf[1000];
		//sprintf(buf,("恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr+1) ,(int)(RemainTime/60),(int)(RemainTime%60));
		//::WriteLogFile(buf,BoxNr);

		//MainWnd->m_TestMsg.Format(_T("恒温在 %5.1f度 %02d:%02d(%d)"),GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr) ,(int)(RemainTime/60),(int)(RemainTime%60),G_NormalConfigPara[BoxNr].PidTime);
		
		
		MainWnd->m_TestMsg.Format(_T("恒温在%5.1f度 %02d:%02d"),
			GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr),
			RemainTime / 60,
			RemainTime % 60);
		MainWnd->UpdateData(false);

		if(::G_Meter[BoxNr].m_PcbTestMode != PCB_SUB_MODE_NORMAL_PID)
		{
			//G_RS485_Comm.SendNormalPidCmd(GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr),BoxNr);
			//G_Meter[BoxNr].m_PcbTestMode = PCB_SUB_MODE_NORMAL_PID;
		}
	}

	return m_SubMode;
}

void CWorkMode::AutoTestStep_StepPID(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
	//如果现在的温度已经达到目标温度，就进入下一步
	// 或者时间已到
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);
		

	if((CurTemp  >=  m_TempRange.open_temp_max + ::G_NormalConfigPara[BoxNr].OverHeatGap )
		|| IsAllOpened(BoxNr))
	{
		m_CurSubModeBeginTime = CTime::GetCurrentTime();

//		if(IsAllOpened(BoxNr))
//		{
		//	::WriteLogFile("AutoTestStep_StepPID  IsAllOpened",this->BoxNr + 1);
//		}

		if(m_TempRange.close_temp_min == m_TempRange.close_temp_max)
		{//不用测复位，直接结束测试
		//	AfxMessageBox(_T("1"));
			::WriteLogFile("AutoTestStep_StepPID  close_temp_min == close_temp_max",BoxNr + 1);
			this->ExitAutoTest(MainWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode); 
		}
		else
		{

			if(::G_NormalConfigPara[BoxNr].TestMode == TEST_MODE_TOP_TEMP_PID)
			{
				m_CurSubModeBeginTime	= CTime::GetCurrentTime();
				m_SubMode = SUB_MODE_STEP_TOP_PID;
				G_RS485_Comm.SendNormalPidCmd(GetControlGoalTemp(SUB_MODE_STEP_TOP_PID,HeatCool,0,m_TempRange,BoxNr),BoxNr);
			}
			else
			{
				
				m_CurSubModeBeginTime	=	CTime::GetCurrentTime();
				
				if(::G_iMaxLedNr[BoxNr] == 192)
				{//开门就是开空调
					if(::G_NormalConfigPara[BoxNr].AirControlEquipment  == 0)
					{
						G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
					}
					else
					{
						if((CurTemp < G_NormalConfigPara[BoxNr].AirControlEquipment) && (G_NormalConfigPara[BoxNr].AirControlEquipment > 0))
						{
							G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
						}
					}
				}
				else
				{
					G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
				}
				if(::G_NormalConfigPara[BoxNr].CoolSpeed > 0)
				{
					if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_24 
						|| ::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_16
						|| ::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40)
					{
						G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); 
						
						float goal = GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,false,m_TempRange,BoxNr);

						int minutes = (CurTemp - goal)/10.0f;
						if(minutes < 1)
						{
							minutes = 1;
						}
						//CString s;
						//s.Format(_T("goal = %5.1f,minutes=%d"),goal,minutes);
						//AfxMessageBox(s);

						G_RS485_Comm.SendStepPidCoolCmd(goal,minutes,BoxNr,true);//::G_NormalConfigPara[BoxNr].CoolDownAutoOpenDoor);
						m_SubMode = SUB_MODE_PID_COOL;
						m_CurSubModeBeginTime = CTime::GetCurrentTime();
						MainWnd->m_TestMsg.Format(_T("恒温到 %5.1f度"),GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,false,m_TempRange,BoxNr));
						MainWnd->UpdateData(false);
						m_CurSubModeBeginTime = CTime::GetCurrentTime();
					}
					else
					{
					
						m_SubMode = SUB_MODE_PID_COOL;
						
						
						float MaxOpenTemp = this->GetHighestOpenTemp(ParentWnd,BoxNr,CurTemp,SwitchConfigPara);

						if(::G_NormalConfigPara[BoxNr].BoxType == BOX_OIL_BATHS)
						{
							if(MaxOpenTemp < GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,0,m_TempRange,BoxNr))
							{
								G_RS485_Comm.SendPidCoolCmd(MaxOpenTemp - SwitchConfigPara.CloseHighLevelDiffMinus,BoxNr,true);
							
								MainWnd->m_TestMsg.Format(_T("恒温到 %5.1f度 "),MaxOpenTemp);
							}
							else
							{
								G_RS485_Comm.SendPidCoolCmd(GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,0,m_TempRange,BoxNr),BoxNr,::G_NormalConfigPara[0].CoolDownAutoOpenDoor);
							
								MainWnd->m_TestMsg.Format(_T("恒温到 %5.1f度 "),GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,0,m_TempRange,BoxNr));
							}
						}
						else
						{
							
								G_RS485_Comm.SendPidCoolCmd(GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,0,m_TempRange,BoxNr),BoxNr,::G_NormalConfigPara[0].CoolDownAutoOpenDoor);
							
								MainWnd->m_TestMsg.Format(_T("恒温到 %5.1f度 "),GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,0,m_TempRange,BoxNr));
						}
					}

					MainWnd->UpdateData(false);
				}
				else
				{
					G_RS485_Comm.SendStopHeatCmd(0,BoxNr);
					G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
					m_SubMode = SUB_MODE_DIRECT_COOL;
					MainWnd->m_TestMsg.Format(_T("降温到 %5.1f度 "),GetControlGoalTemp(SUB_MODE_DIRECT_COOL,HeatCool,0,m_TempRange,BoxNr));
					MainWnd->UpdateData(false);
				}
#ifdef WINCE				
				CWnd *button;
				button = MainWnd->GetDlgItem(IDC_BUTTON_START);
				{
					CString msg[] = 
					{
						_T("停止测试"),
						_T("Stop TesT"),
					};
					button->SetWindowText(msg[::G_NormalConfigPara[BoxNr].Language]);
				}
#endif
				if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
				{
					G_RS485_Comm.SendEPM1270Reset(true,BoxNr);
				}
			}
			
		}

	}
	else
	{
		
		CTime cur = cur.GetCurrentTime();
		CTimeSpan span = cur - m_CurSubModeBeginTime; 
		

		if(m_iTestMode == TEST_MODE_OPEN_DOOR_RESET || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
		{
			OpenDoor_For_OpenFlashTest(MainWnd,BoxNr,CurTemp,SwitchConfigPara);
		}
		else
		{
			int sec = span.GetTotalSeconds();
			MainWnd->m_TestMsg.Format(_T("线性升温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_STEP_PID,HeatCool,0,m_TempRange,BoxNr),(sec / 60),(sec % 60));
			MainWnd->UpdateData(false);

			if(::G_Meter[BoxNr].m_PcbTestMode != PCB_SUB_MODE_INCREASE_PID && G_Meter[BoxNr].GetCurPv(G_Meter[BoxNr].m_TempAnswerPara) > GetControlGoalTemp(SUB_MODE_PID,HeatCool,0,m_TempRange,BoxNr))
			{
				float goal =  GetControlGoalTemp(SUB_MODE_STEP_PID,HeatCool,false,m_TempRange,BoxNr);
				float speed	= G_NormalConfigPara[BoxNr].HeatSpeed / 10.0f;
				if(speed <= 0)
				{
					speed = 1.0f;
				}
				int test_minutes = (goal - G_Meter[BoxNr].GetCurPv(G_Meter[BoxNr].m_TempAnswerPara)) / speed;
				
				if(test_minutes < 1)
				{
					test_minutes = 1;
				}

				//::G_RS485_Comm.SendStepPIDCmd(  goal ,test_minutes,BoxNr);
				//G_Meter[BoxNr].m_PcbTestMode = PCB_SUB_MODE_INCREASE_PID;
			}
		}
		
	}
}

void CWorkMode::ExitAutoTest(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara,CString barcode)
{

#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

//	m_WorkStatus.AutoTestFlag = false;
	
	

#ifdef WINCE
	CWnd * button;
	button = MainWnd->GetDlgItem(IDC_BUTTON_START);
	{
		CString msg[] = 
		{
			_T("开始生产"),
			_T("TEST"),
		};
		button->SetWindowText(msg[::G_iLangID]); 
		button->EnableWindow(true);
	}
	{
		CString msg[] = 
		{
			_T("自动测试停止"),
			_T("Test stopped"),
		};
		MainWnd->m_TestMsg = msg[::G_iLangID];
		MainWnd->UpdateData(false); 

	}

	CButton *b = (CButton*)MainWnd->GetDlgItem(IDC_BUTTON_TAKEBOARD); 
	b->Invalidate();  

#else
	CWnd * button;
	button = MainWnd->GetDlgItem(IDC_CHECK_AUTO_PROCESS);
	button->SetWindowText(_T("开始生产")); 
	MainWnd->m_TestMsg = _T("自动测试停止");
	button = MainWnd->GetDlgItem(IDC_BUTTON_TAKEOUT);
	button->EnableWindow(true);
	
	button = MainWnd->GetDlgItem(IDC_RADIO_CLOSE);
	button->EnableWindow(true);
	button = MainWnd->GetDlgItem(IDC_RADIO_OPEN);
	button->EnableWindow(true);
#endif

#ifndef WINCE
	if(::G_NormalConfigPara[BoxNr].BoxType == BOX_OIL_BATHS)
	{

		m_SubMode   = 	SUB_MODE_READY;
		button = MainWnd->GetDlgItem(IDC_CHECK_AUTO_PROCESS);
		button->SetWindowText(_T("开始生产")); 
		MainWnd->m_TestMsg = _T("恒温等待...");
		button = MainWnd->GetDlgItem(IDC_BUTTON_TAKEOUT);
		button->EnableWindow(true);
		
		button = MainWnd->GetDlgItem(IDC_RADIO_CLOSE);
		button->EnableWindow(true);
		button = MainWnd->GetDlgItem(IDC_RADIO_OPEN);
		button->EnableWindow(true);
	}
	else
#endif
	{
		m_SubMode = SUB_MODE_STOP;
	}
	


#ifndef WINCE
	if(::G_iCmdAscii[BoxNr] != 1)  
	{
		::G_RS485_Comm.SendAutoTest(false,BoxNr); 
	}
#endif
	


	G_RS485_Comm.SendAutoTest(false,BoxNr);

	G_RS485_Comm.SendStopHeatCmd(0,BoxNr);
	
	


	int cool = G_NormalConfigPara[BoxNr].AirControlEquipment;
	if(cool != 0)
	{
		if(G_Meter[BoxNr].m_IsAcOn == true)
		{
			CString  msg;
			G_RS485_Comm.SendAirControlCmd(false,G_Meter[BoxNr].m_IsAcOn ,BoxNr,&msg);
		}
	}


	G_RS485_Comm.SendStopHeatCmd(0,BoxNr);
	
	CTimeSpan span = m_EndTestTime - m_BeginTestTime;
	if(span.GetTotalSeconds() > 10)
	{//测试总时间必须大于90秒,否则测试记录无效
		SaveTestRecord(BoxNr,barcode); 
	}

	if(G_NormalConfigPara[BoxNr].RepeatTestCounts <= 0)
	{
		::ReadNormalConfigPara();
	}
	else
	{
		G_NormalConfigPara[BoxNr].RepeatTestCounts --;
#ifdef WINCE
		MainWnd->OnBnClickedButtonStart();
#else
		MainWnd->OnCheckStartTest(); 
#endif
	}

#ifndef WINCE	
	CButton *b = (CButton*)MainWnd->GetDlgItem(IDC_BUTTON_TAKEOUT); 
	
	if(b != NULL)
	{
		
		HBITMAP bitmap = b->GetBitmap();

		if(bitmap == NULL)
		{
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TAKE_BOARD)));
		}
		else
		{
			DeleteObject(bitmap);
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TAKE_BOARD)));
		}
	}
#endif

	CString s;
	s.Format(_T("%d号烘箱测试结束!") ,BoxNr + 1);
	if(G_SpeechArray.GetSize() < 10)
	{
		::G_SpeechArray.Add(s);
	}

	G_RS485_Comm.SendBeepCmd(true,BoxNr);

	if(G_NormalConfigPara[BoxNr].StopTestAutoCloseDoor == 0)
	{
		G_RS485_Comm.SendOpenDoorCmd(true,BoxNr);
	}
	else
	{
		G_RS485_Comm.SendOpenDoorCmd(false,BoxNr);
	}

}

void CWorkMode::OpenDoor_For_OpenFlashTest(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#define HEAT_DEGREE	m_OpenDoorResetHeatDegree
#define COOL_DEGREE	m_OpenDoorResetCoolDegree
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	if(m_SubMode != SUB_MODE_STEP_PID)
	{
		return;
	}

	if(G_Meter[BoxNr].m_IsDoorOpened == false)
	{//关门中，温度先升到设定的温度
		CTime cur = CTime::GetCurrentTime(); 
		CTimeSpan span = cur - m_OpenDoor_StartTime;
		
		if(((m_OpenDoor_Temp <= CurTemp) /*&& OpennedAndNotFlashTested()*/)
		//	|| (CurTemp >=  m_TempRange.open_temp_max)
		//	|| (span.GetTotalMinutes() >= 1)
			)
		{
			int count = 0;
			for(int i = 0; i < G_iMaxLedNr[BoxNr]; i++)
			{
				if((G_SwitchTestResult[BoxNr][i].IsOpenned) && (G_SwitchTestResult[BoxNr][i].IsOpenFlashTested == false))
				{
					count ++;
				}
			}
			if(count == 0)
			{//在此次升温区间内，没有产品动作
				m_OpenDoor_StartTime = CTime::GetCurrentTime(); 

				float toptemp = GetControlGoalTemp(SUB_MODE_STEP_PID,HeatCool,0,m_TempRange,BoxNr );
				
				int minutes = (toptemp - m_OpenDoor_Temp)*10 / G_NormalConfigPara[BoxNr].HeatSpeed;

#ifndef WINCE			
				char buf[200];
				sprintf_s(buf,sizeof(buf),"minutes = %d",minutes);
				WriteLogFile(buf,BoxNr+1,true);
#endif

				m_OpenDoor_Temp += ::G_NormalConfigPara[BoxNr].OpenDoorReset_HeatDegree;
				
				MainWnd->m_TestMsg.Format(_T("开关门升温到%5.1f度"),m_OpenDoor_Temp);
				MainWnd->UpdateData(false);

				G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); 

				if(toptemp < m_OpenDoor_Temp)
				{
					toptemp = m_OpenDoor_Temp;
				}
				
				if(minutes < 1)
				{//有可能升温速率太小，导致温度升不上去
					//minutes = 1;
					G_RS485_Comm.SendDirectHeatCmd(toptemp+1.0,BoxNr);
				}
				else
				{
					//G_RS485_Comm.SendStepPIDCmd(toptemp,minutes,BoxNr);
					G_RS485_Comm.SendDirectHeatCmd(toptemp,BoxNr);
				}
			}
			else
			{//有产品动作
				m_OpenDoor_StartTime = CTime::GetCurrentTime(); 
				MainWnd->m_TestMsg.Format(_T("开关门降温到%5.1f度"),m_OpenDoor_Temp - ::G_NormalConfigPara[BoxNr].OpenDoorReset_CoolDegree );
				MainWnd->UpdateData(false);
				G_RS485_Comm.SendStopHeatCmd(0,BoxNr);
				G_RS485_Comm.SendOpenDoorCmd(true,BoxNr);
			}
		}
		else
		{
			MainWnd->m_TestMsg.Format(_T("开关门升温到%5.1f度"),m_OpenDoor_Temp);
			MainWnd->UpdateData(false);
		}
	}
	else
	{//降温中，温度降到设定的温度
		CTime cur = CTime::GetCurrentTime(); 
		CTimeSpan span = cur - m_OpenDoor_StartTime;
			

		if(((m_OpenDoor_Temp	- ::G_NormalConfigPara[BoxNr].OpenDoorReset_CoolDegree  ) >= CurTemp) 
			|| (span.GetTotalMinutes() >= 5))
		{
			m_OpenDoor_StartTime = CTime::GetCurrentTime(); 
			


			float toptemp = GetControlGoalTemp(SUB_MODE_STEP_PID,HeatCool,0,m_TempRange,BoxNr );
			
			int minutes = (toptemp - m_OpenDoor_Temp)*10 / G_NormalConfigPara[BoxNr].HeatSpeed;

#ifndef WINCE			
			char buf[200];
			sprintf_s(buf,sizeof(buf),"minutes = %d",minutes);
			WriteLogFile(buf,BoxNr+1,true);
#endif

			m_OpenDoor_Temp += ::G_NormalConfigPara[BoxNr].OpenDoorReset_HeatDegree;
			
			MainWnd->m_TestMsg.Format(_T("开关门升温到%5.1f"),m_OpenDoor_Temp);
			MainWnd->UpdateData(false);
			

			G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); //WriteLogFile("17",BoxNr+1,true);

			
			if(minutes < 1)
			{//有可能升温速率太小，导致温度升不上去
				//minutes = 1;
				G_RS485_Comm.SendDirectHeatCmd(toptemp+1.0,BoxNr);
			}
			else
			{
				G_RS485_Comm.SendStepPIDCmd(toptemp,minutes,BoxNr);
			}
			
			

		}
		else
		{
			MainWnd->m_TestMsg.Format(_T("开关门降温到%5.1f"),m_OpenDoor_Temp - ::G_NormalConfigPara[BoxNr].OpenDoorReset_CoolDegree );
			MainWnd->UpdateData(false);
		}
	}

}

void CWorkMode::ReadSwitchConfigPara(CWnd * ParentWnd,int BoxNr)
{

#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	CFile para_file;
	CString filename;

#ifdef WINCE
	filename = CONFIG_PARA_FILE_DIR;
	if(G_NormalConfigPara[BoxNr].SelParaName[0] == 0)
	{
		memset(&m_SwitchConfigPara,0,sizeof(m_SwitchConfigPara)); 
	}
	else
	{
		filename += G_NormalConfigPara[BoxNr].SelParaName; 
		filename.Trim(); 
		
		if(para_file.Open(filename,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ))
		{
			if(para_file.Read(&m_SwitchConfigPara,sizeof(m_SwitchConfigPara)) == sizeof(m_SwitchConfigPara))
			{
				memcpy(m_SwitchConfigPara.Name,G_NormalConfigPara[BoxNr].SelParaName,sizeof(m_SwitchConfigPara.Name)); 
			}
			else
			{
				memset(&m_SwitchConfigPara,0,sizeof(m_SwitchConfigPara)); 
			}
			para_file.Close(); 
					
		}
	}

	MainWnd->m_iNormalPidMinutes			=	G_NormalConfigPara[BoxNr].PidTime;
	MainWnd->m_iNormalPidCoolMinutes		=	G_NormalConfigPara[BoxNr].PidCoolTime;
	MainWnd->m_fStepPidSpeed				=	(G_NormalConfigPara[BoxNr].HeatSpeed + 1)/ 10.0f;
	if(MainWnd->m_fStepPidSpeed <= 0)
	{
		MainWnd->m_fStepPidSpeed	= 1.0f;
	}
	
	if(G_NormalConfigPara[BoxNr].CoolSpeed == 0)
	{
		CString msg[] = 
		{
			 _T("全速降温"),
			 _T("Full Cool"),
		};
		MainWnd->m_sStepCoolPidSpeed = msg[::G_NormalConfigPara[BoxNr].Language];
	}
	else
	{
		MainWnd->m_sStepCoolPidSpeed.Format(_T("%5.1f"),G_NormalConfigPara[BoxNr].CoolSpeed / 10.0f);
	}
	
	MainWnd->m_SelParaName				= G_NormalConfigPara[BoxNr].SelParaName;
	MainWnd->m_SelectedTestMode			= G_NormalConfigPara[BoxNr].TestMode;
//	m_OpenDoorReset_HeatDegree	= G_NormalConfigPara[BoxNr].OpenDoorReset_HeatDegree / 10.0f;
//	m_OpenDoorReset_CoolDegree	= G_NormalConfigPara[BoxNr].OpenDoorReset_CoolDegree / 10.0f;
//	m_TopPidTempMinutes			= m_OpenDoorReset_HeatDegree;
//	m_AirControlEquipment		= G_NormalConfigPara[BoxNr].AirControlEquipment;
//	MainWnd->m_SwitchType				= G_NormalConfigPara[BoxNr].SwitchType; 
	MainWnd->m_RepeatTestCounts			= G_NormalConfigPara[BoxNr].RepeatTestCounts;

	if(G_NormalConfigPara[BoxNr].BoxType == ::BOX_NTC)
	{
			if(G_NormalConfigPara[BoxNr].SwitchType == 0)
		{
			
			CString msg[] = 
			{
				_T("10K档"),
				_T("10K"),
			};
			MainWnd->m_StrType = msg[::G_NormalConfigPara[BoxNr].Language];
					
		}
		else
		{
			CString msg[] = 
			{
				_T("100K档"),
				_T("100K"),
			};
			MainWnd->m_StrType = msg[::G_NormalConfigPara[BoxNr].Language];
			
		}
	}
	else
	{
		if(G_NormalConfigPara[0].BoxType == BOX_DISC_40
				|| G_NormalConfigPara[0].BoxType == BOX_DISC_24
				|| G_NormalConfigPara[0].BoxType == BOX_DISC_16)
		{
				G_NormalConfigPara[BoxNr].SwitchType = 1;
				CString msg[] = 
				{
					_T("常开"),
					_T("Open"),
				};
				MainWnd->m_StrType = msg[::G_NormalConfigPara[BoxNr].Language];
		}
		else
		{
			if(G_NormalConfigPara[BoxNr].SwitchType == 0)
			{
				
				CString msg[] = 
				{
					_T("常闭"),
					_T("Close"),
				};
				MainWnd->m_StrType = msg[::G_NormalConfigPara[BoxNr].Language];
						
			}
			else
			{
				CString msg[] = 
				{
					_T("常开"),
					_T("Open"),
				};
				MainWnd->m_StrType = msg[::G_NormalConfigPara[BoxNr].Language];
				
			}
		}
	}
	
	if(G_NormalConfigPara[BoxNr].AirControlEquipment == 0)
	{
		CString msg[] = 
		{
			_T("无"),
			_T("No"),
		};
		MainWnd->m_sAc = msg[::G_NormalConfigPara[BoxNr].Language];
	}
	else 
	{	
		MainWnd->m_sAc.Format(_T("<%d°"),::G_NormalConfigPara[BoxNr].AirControlEquipment);
	}
	
	if((MainWnd->m_SelectedTestMode < 0) || (MainWnd->m_SelectedTestMode >= MainWnd->m_WorkModeStrArray.GetSize()))
	{
		MainWnd->m_SelectedTestMode = 0;
	}
	if(MainWnd->m_SelectedTestMode < MainWnd->m_WorkModeStrArray.GetSize())
	{
		MainWnd->m_WorkModeText = MainWnd->m_WorkModeStrArray.GetAt(MainWnd->m_SelectedTestMode);
	}
	if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_24 
		|| G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_16 
		|| G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40)
	{
		MainWnd->m_StrMainLevel.Format(_T("%.1f,%.1f,--%.1f,%.1f"), 
			m_SwitchConfigPara.MainLevelSpanMinus,m_SwitchConfigPara.MainLevelSpanPlus,m_SwitchConfigPara.CloseLevelSpanMinus,m_SwitchConfigPara.CloseLevelSpanPlus);
		if(m_SwitchConfigPara.HighLevelSpanMinus != 0.0f 
			|| m_SwitchConfigPara.HighLevelSpanPlus != 0.0f 
			|| m_SwitchConfigPara.CloseHighLevelSpanMinus!= 0.0f 
			|| m_SwitchConfigPara.CloseHighLevelSpanPlus != 0.0f)
		{
			MainWnd->m_StrHighLevel.Format(_T("%.1f,%.1f,--%.1f,%.1f,"), 
				m_SwitchConfigPara.HighLevelSpanMinus,m_SwitchConfigPara.HighLevelSpanPlus,m_SwitchConfigPara.CloseHighLevelSpanMinus,m_SwitchConfigPara.CloseHighLevelSpanPlus);
		}
		else
		{
			MainWnd->m_StrHighLevel = _T("");
		}
		if(m_SwitchConfigPara.LowLevelSpanMinus != 0.0f 
			|| m_SwitchConfigPara.LowLevelSpanPlus != 0.0f
			|| m_SwitchConfigPara.CloseLowLevelSpanMinus != 0.0f
			|| m_SwitchConfigPara.CloseLowLevelSpanPlus != 0.0f)
		{
			MainWnd->m_StrLowLevel.Format(_T("%.1f,%.1f,--%.1f,%.1f"), 
				m_SwitchConfigPara.LowLevelSpanMinus,m_SwitchConfigPara.LowLevelSpanPlus,m_SwitchConfigPara.CloseLowLevelSpanMinus,m_SwitchConfigPara.CloseLowLevelSpanPlus);
		}
		else
		{
			MainWnd->m_StrLowLevel = _T("");
		}
	}
	else if(m_SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_RANGE)
	{
		MainWnd->m_StrMainLevel.Format(_T("%.1f,%.1f,%.1f,%.1f,%.1f,%.1f"), 
			m_SwitchConfigPara.MainLevel,-m_SwitchConfigPara.MainLevelSpanMinus,m_SwitchConfigPara.MainLevelSpanPlus,m_SwitchConfigPara.CloseLevel,-m_SwitchConfigPara.CloseLevelSpanMinus,m_SwitchConfigPara.CloseLevelSpanPlus);
		MainWnd->m_StrHighLevel.Format(_T("%.1f,%.1f,%.1f,%.1f,%.1f,%.1f"), 
			m_SwitchConfigPara.HighLevel,-m_SwitchConfigPara.HighLevelSpanMinus,m_SwitchConfigPara.HighLevelSpanPlus,m_SwitchConfigPara.CloseHighLevel,-m_SwitchConfigPara.CloseHighLevelSpanMinus,m_SwitchConfigPara.CloseHighLevelSpanPlus);
		MainWnd->m_StrLowLevel.Format(_T("%.1f,%.1f,%.1f,%.1f,%.1f,%.1f"), 
			m_SwitchConfigPara.LowLevel,-m_SwitchConfigPara.LowLevelSpanMinus,m_SwitchConfigPara.LowLevelSpanPlus,m_SwitchConfigPara.CloseLowLevel,-m_SwitchConfigPara.CloseLowLevelSpanMinus,m_SwitchConfigPara.CloseLowLevelSpanPlus);
	}
	else
	{
		MainWnd->m_StrMainLevel.Format(_T("%.1f,%.1f,%.1f,--,%.1f,%.1f"), 
			m_SwitchConfigPara.MainLevel,-m_SwitchConfigPara.MainLevelSpanMinus,m_SwitchConfigPara.MainLevelSpanPlus,m_SwitchConfigPara.CloseLevelSpanMinus,m_SwitchConfigPara.CloseLevelSpanPlus);
		MainWnd->m_StrHighLevel.Format(_T("%.1f,%.1f,%.1f,--,%.1f,%.1f"), 
			m_SwitchConfigPara.HighLevel,-m_SwitchConfigPara.HighLevelSpanMinus,m_SwitchConfigPara.HighLevelSpanPlus,m_SwitchConfigPara.CloseHighLevelSpanMinus,m_SwitchConfigPara.CloseHighLevelSpanPlus);
		MainWnd->m_StrLowLevel.Format(_T("%.1f,%.1f,%.1f,--,%.1f,%.1f"), 
			m_SwitchConfigPara.LowLevel,-m_SwitchConfigPara.LowLevelSpanMinus,m_SwitchConfigPara.LowLevelSpanPlus,m_SwitchConfigPara.CloseLowLevelSpanMinus,m_SwitchConfigPara.CloseLowLevelSpanPlus);

	}

#endif

}



void CWorkMode::EnterAutoTest(CWnd * ParentWnd,int BoxNr,float CurTemp[4],SWITCH_CONFIG_PARA	SwitchConfigPara)
{

#ifdef _DONGGUAN_BETTER
		CDongguanBetterMesMsgDlg MesMsgDlg;
		MesMsgDlg.DoModal();
	

#endif

#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	SetTestMode(::G_NormalConfigPara[BoxNr].TestMode);//  MainWnd->m_iTestMode);

	for(int i = 0; i < 40; i++)
	{
		G_BiMetalRecordArray[BoxNr][i].RemoveAll();
	}

#ifdef WINCE
	G_TempArray[BoxNr].RemoveAll();
#endif

	G_NormalConfigPara[BoxNr].SoundOutputType	=	0;
	m_DiscID									=	0;


	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	if(m_TempRange.open_temp_max == m_TempRange.open_temp_min)
	{
		CString msg[] = {
			_T("请选择正确的测试参数！"),
			_T("Please select a correct TEST PARAMETER！")
						};
		if(G_SpeechArray.GetSize() < 10)
		{
			G_SpeechArray.Add (msg[G_iLangID]);
		}
		AfxMessageBox(msg[G_iLangID]);
		return;
	}


#ifndef _OIL_
		
		double min_temp = 999.9,max_temp = -999.9,pid_temp = 0;;
		
		for(int i = 0; i < 4; i++)
		{
			if(min_temp > CurTemp[i])
			{
				min_temp = CurTemp[i];
			}
			
			if(max_temp < CurTemp[i])
			{
				max_temp = CurTemp[i];
			}
			pid_temp += CurTemp[i];
		}
		pid_temp /= 4.0f;

		if(max_temp - min_temp > 30.0f)
		{
#ifdef _DEBUG
			CString msg[] = {_T("传感器温度不均匀，请检查传感器\r\nPID=%5.2f, max=%5.2f,Min=%5.2f \r\n %5.2f,%5.2f,%5.2f,%5.2f"),
								_T("Differences of the four temperatures sensors  are too big,Please check them.\r\nPID=%5.2f,max=%5.1f,Min=%5.1f \r\n %5.1f,%5.1f,%5.1f,%5.1f") 
			};

			CString s;
			s.Format(msg[G_iLangID],CurTemp, max_temp,min_temp,CurTemp[0],CurTemp[1],CurTemp[2],CurTemp[3]);
			if(G_SpeechArray.GetSize() < 10)
			{
				G_SpeechArray.Add (s);
			}
			if(::G_NormalConfigPara[0].BoxType != BOX_8PT100_8SSR)
			{
				AfxMessageBox(s);
				return;
			}
			
#endif
		}

		if((CurTemp[0]<= -40.0f ||  CurTemp[0]> 300)
			&& (CurTemp[1]<= -40.0f ||  CurTemp[1]> 300)
			&& (CurTemp[2]<= -40.0f ||  CurTemp[2]> 300)
			&& (CurTemp[3]<= -40.0f ||  CurTemp[3]> 300))
		{
#ifndef _DEBUG
			CString s;
			if(G_SpeechArray.GetSize() < 10)
			{
				G_SpeechArray.Add (_T("传感器温度不正常，请检查传感器"));
			}
			s.Format(_T("%d#烘箱 传感器温度不正常，请检查传感器"),BoxNr + 1);
			AfxMessageBox(s);
			return;
#endif
		}

		
		

#endif
		
		

		if(G_NormalConfigPara[BoxNr].RepeatTestCounts <= 0)
		{
			
			CTimeSpan span = CTime::GetCurrentTime() - G_Meter[BoxNr].m_MeterAnswerTime;

			if( span.GetTotalSeconds() > 10)
			{
				
#ifndef _DEBUG
				CString s;
				CString msg[] = {	_T("温度仪表无响应,请检查通讯线路"),
					_T("NO ANSWER FROM TEMPETRURE METER,PLEASE CHECK COMMUNICATION")};
				s.Format(_T("%s %d#烘箱"), msg[G_iLangID],BoxNr+1);
				if(G_SpeechArray.GetSize() < 10)
				{
					G_SpeechArray.Add (s);
				}

				AfxMessageBox(s,MB_ICONSTOP);
				return;
#endif
			}
			else
			{
				span = CTime::GetCurrentTime() - G_Meter[BoxNr].m_LedAnswerTime[0];
			
				if( span.GetTotalSeconds() > 10)
				{
						
#ifndef _DEBUG
					CString msg[] = {	_T("灯板无响应!"),
						_T("NO ANSWER FROM LED BOARD")};
					if(G_SpeechArray.GetSize() < 10)
					{
						G_SpeechArray.Add (msg[G_iLangID]);
					}
					AfxMessageBox(msg[G_iLangID],MB_ICONSTOP);
					//return;
#endif
				}
				
			}

			if(IsPT100Caled(BoxNr) == false)
			{
#ifndef _DEBUG
				CString msg[] = {	_T("传感器没有校温，请检查传感器"),
					_T("TEMPERATURE SERSORS ARE NOT CALIBRATED, PLEASE CHECK!")};
				G_SpeechArray.Add (msg[G_iLangID]);
				AfxMessageBox(msg[::G_iLangID]);
				return;
#endif
			}
#ifndef _DEBUG
			if(G_NormalConfigPara[BoxNr].AdjustLow[0] == 0.0f
				&& G_NormalConfigPara[BoxNr].AdjustLow[1] == 0.0f
				&& G_NormalConfigPara[BoxNr].AdjustLow[2] == 0.0f
				&& G_NormalConfigPara[BoxNr].AdjustLow[3] == 0.0f)
			{
				CString msg[] = {	_T("无温度修正值"),
					_T("No temperature correction value!")};
				G_SpeechArray.Add (msg[G_iLangID]);
				AfxMessageBox(msg[::G_iLangID]);
				return;
			}

			if(G_NormalConfigPara[BoxNr].AdjustHigh[0] == 0.0f
				&& G_NormalConfigPara[BoxNr].AdjustHigh[1] == 0.0f
				&& G_NormalConfigPara[BoxNr].AdjustHigh[2] == 0.0f
				&& G_NormalConfigPara[BoxNr].AdjustHigh[3] == 0.0f)
			{
				CString msg[] = {	_T("无温度修正值"),
					_T("No temperature correction value!")};
				G_SpeechArray.Add (msg[G_iLangID]);
				AfxMessageBox(msg[::G_iLangID]);
				return;
			}
#endif
		if(HeatCool == HEAT_FIRST_COOL_SECOND)
		{
			CString str;
			CString str2;
			if(G_iLangID == 0)
			{
			str.Format(_T("全速加热到   % 5.1f度\r\n恒温在           % 5.1f度 %2d分钟\r\n线性加热到   % 5.1f度% 5.1f度/分钟\r\n"),
				GetControlGoalTemp(SUB_MODE_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr),
				GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr), G_NormalConfigPara[BoxNr].PidTime,
				GetControlGoalTemp(SUB_MODE_STEP_PID,HeatCool,false,m_TempRange,BoxNr),   (G_NormalConfigPara[BoxNr].HeatSpeed)/ 10.0f);
			}
			else
			{
				str.Format(_T("Fully heat up to: % 5.1f°\r\n Constant temperature: % 8.1f°( %2dmin)\r\n\r\nLinearly heated to   % 5.1f°(% 5.1f°/min)\r\n"),
				GetControlGoalTemp(SUB_MODE_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr),
				GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr), G_NormalConfigPara[BoxNr].PidTime,
				GetControlGoalTemp(SUB_MODE_STEP_PID,HeatCool,false,m_TempRange,BoxNr),   (G_NormalConfigPara[BoxNr].HeatSpeed)/ 10.0f);
				
			}
			if(G_NormalConfigPara[BoxNr].SwitchType == 0)
			{
				str2 = _T("保护器类型   常闭\r\n");
			}
			else
			{
				str2 = _T("保护器类型   常开\r\n");
			}
			str += str2;
			
			if(m_TempRange.close_temp_min == m_TempRange.close_temp_max)
			{
				str2 = _T("不测复位温度");
			}
			else
			{
				str2.Format(_T("线性降温到  %5.1f度 \r\n"),GetControlGoalTemp(SUB_MODE_STEP_COOL,HeatCool,false,m_TempRange,BoxNr));
				str += str2;
				str2.Format(_T("复位温度      % 5.1f度\r\n"),m_TempRange.close_temp_min);
			}

			str += str2;
#ifdef WINCE
			CMessageBox box;
			box.m_Msg = str;
			if( box.DoModal()  != IDOK)
			{
				return;
			}
#else
			CDlgInfomation dlg;
			
			dlg.m_BoxNr = BoxNr;
			if(G_NormalConfigPara[0].bBarcode == 0)
			{
				G_NormalConfigPara[BoxNr].TestMemo[sizeof(G_NormalConfigPara[BoxNr].TestMemo)-1] = 0;
				dlg.m_Memo = G_NormalConfigPara[BoxNr].TestMemo;
			}
			
			G_NormalConfigPara[BoxNr].Tester[sizeof(G_NormalConfigPara[BoxNr].Tester) - 1] = 0;
			dlg.m_Tester = G_NormalConfigPara[BoxNr].Tester;

			dlg.m_Info = str;
			if(G_SpeechArray.GetSize() < 10)
			{
				G_SpeechArray.Add(str);
			}
			dlg.m_Barcode = _T("");//__T("161102711,1446-202605191779157027959,20260314,Made in China,30,KSD0856112\r\n");
			if(dlg.DoModal() != IDOK)
			{
				return ;
			}
			MainWnd->m_TestMemo = dlg.m_Memo;
			MainWnd->m_Barcode  = dlg.m_Barcode;
			if(MainWnd->m_Barcode.GetLength() > 0)
			{
				int len;
				TCHAR c;
				while(MainWnd->m_Barcode.GetLength() > 0)
				{
					len = MainWnd->m_Barcode.GetLength();
					c = MainWnd->m_Barcode.GetAt(len - 1);
					if(c == '\r' || c == '\n')
					{
						MainWnd->m_Barcode = MainWnd->m_Barcode.Left(len - 1);
					}
					else
					{
						break;
					}

				}
			}
			G_MainDlg->m_TempParaDlg[BoxNr].m_ParaName = MainWnd->m_TestMemo;
			G_MainDlg->m_TempParaDlg[BoxNr].UpdateData(false);
#endif			
			//int ret = MessageBox(str,strMsgCaption,MB_OKCANCEL | MB_ICONINFORMATION);
			//if( ret != IDOK)
			//{
			//	return;
			//}

		}

		else
		{
			CString str;
			CString str2;
			if(G_iLangID == 0)
			{
			str.Format(_T("全速升温到   % 5.1f度\r\n全速降温到   % 5.1f度\r\n降温恒温在         % 5.1f度 %2d分钟\r\n线性降温到   % 5.1f度% 5.1f度/分钟\r\n"),
				GetControlGoalTemp(SUB_COOL_FIRST_MODE_READY,HeatCool,false,m_TempRange,BoxNr),
				GetControlGoalTemp(SUB_COOL_FIRST_MODE_DIRECT_COOL,HeatCool,false,m_TempRange,BoxNr),
				GetControlGoalTemp(SUB_COOL_FIRST_MODE_COOL_PID,HeatCool,false,m_TempRange,BoxNr), G_NormalConfigPara[BoxNr].PidTime,
				GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_COOL_PID,HeatCool,false,m_TempRange,BoxNr),   (G_NormalConfigPara[BoxNr].HeatSpeed + 1)/ 10.0f);
			}
			else
			{
				str.Format(_T("Fully heat up to: % 5.1f°\r\n Fully cool to: % 8.1f°\r\n Constant Cool( %2dmin)\r\n\r\nLinearly heated to   % 5.1f°(% 5.1f°/min)\r\n"),
				GetControlGoalTemp(SUB_COOL_FIRST_MODE_READY,HeatCool,false,m_TempRange,BoxNr),
				GetControlGoalTemp(SUB_COOL_FIRST_MODE_DIRECT_COOL,HeatCool,false,m_TempRange,BoxNr),
				GetControlGoalTemp(SUB_COOL_FIRST_MODE_COOL_PID,HeatCool,false,m_TempRange,BoxNr), G_NormalConfigPara[BoxNr].PidTime,
				GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_COOL_PID,HeatCool,false,m_TempRange,BoxNr),   (G_NormalConfigPara[BoxNr].HeatSpeed + 1)/ 10.0f);
				
			}
			if(G_NormalConfigPara[BoxNr].SwitchType == 0)
			{
				str2 = _T("保护器类型：常闭\r\n");
			}
			else
			{
				str2 = _T("保护器类型：常开\r\n");
			}
			str += str2;
			
			if(m_TempRange.close_temp_min == m_TempRange.close_temp_max)
			{
				str2 = _T("不测复位温度");
			}
			else
			{
				str2.Format(_T("升温恒温到 %5.1f度 \r\n"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_HEAT_PID,HeatCool,false,m_TempRange,BoxNr));
				str += str2;
				str2.Format(_T("线性升温到  % 5.1f度\r\n"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_HEAT_PID,HeatCool,false,m_TempRange,BoxNr));
			}

			str += str2;
#ifdef WINCE
			CMessageBox box;
			box.m_Msg = str;
			if( box.DoModal()  != IDOK)
			{
				return;
			}
#else
			CDlgInfomation dlg;
			CString s;
			s.Format(_T("TEST MEMO %d"), BoxNr);
			dlg.m_BoxNr = BoxNr;
		
			dlg.m_Memo = AfxGetApp()->GetProfileStringW(_T("SETTING"), s ,_T("请输入本次测试的说明"));
			dlg.m_Info = str;
			if(G_SpeechArray.GetSize() < 10)
			{
				G_SpeechArray.Add(str);
			}
			if(dlg.DoModal() != IDOK)
			{
				return ;
			}
			MainWnd->m_TestMemo = dlg.m_Memo;

			
			G_MainDlg->m_TempParaDlg[BoxNr].m_ParaName = MainWnd->m_TestMemo;
			G_MainDlg->m_TempParaDlg[BoxNr].UpdateData(false);
#endif			
			
			
		}
#ifndef WINCE		
		::G_TempArray[BoxNr].RemoveAll();  
#endif
	}
		

	m_BeginTestTime = CTime::GetCurrentTime(); 
	m_EndTestTime   = CTime::GetCurrentTime(); 


	m_CurSubModeBeginTime = CTime::GetCurrentTime();



	G_RS485_Comm.SendAutoTest(true,BoxNr);
	G_RS485_Comm.SendOpenDoorCmd(false,BoxNr);

	//m_WorkStatus.AutoTestFlag = true;

	MainWnd->m_TestMsg = _T("自动测试开始");

	

#ifndef WINCE

	G_TakeoutDlg.m_iFailHighLevel_Take[BoxNr]	= 0;
	G_TakeoutDlg.m_iFailLowLevel_Take[BoxNr]	= 0;
	G_TakeoutDlg.m_iFailOtherLevel_Take[BoxNr] = 0;
	CWnd *button;
	button = MainWnd->GetDlgItem(IDC_CHECK_AUTO_PROCESS);
	button->SetWindowText(_T("停止生产"));
	button = MainWnd->GetDlgItem(IDC_RADIO_CLOSE);
	button->EnableWindow(false);
	button = MainWnd->GetDlgItem(IDC_RADIO_OPEN);
	button->EnableWindow(false);
#else
	CWnd *button;
	button = MainWnd->GetDlgItem(IDC_BUTTON_START);
	{
		CString msg[] = 
		{
			_T("停止升温"),
			_T("Stop Heat"),
		};
		button->SetWindowText(msg[::G_NormalConfigPara[BoxNr].Language]);
	}
	{
		CString msg[] = 
		{
			_T("自动测试开始"),
			_T("Start..."),
		};
		MainWnd->m_TestMsg = msg[::G_NormalConfigPara[BoxNr].Language];
		MainWnd->UpdateData(false); 
	}
	CButton *b = (CButton*)MainWnd->GetDlgItem(IDC_BUTTON_TAKEBOARD); 
	b->Invalidate();  
#endif
	

	//button = GetDlgItem(IDC_BUTTON_TAKEOUT);
	//button->EnableWindow(false);
	//button = GetDlgItem(IDC_COMBO_FLASH_GAP);
	//button->EnableWindow(false);

	if(HeatCool == HEAT_FIRST_COOL_SECOND )
	{
		if(G_NormalConfigPara[0].BoxType == BOX_OIL_BATHS)
		{
				m_SubMode				=	SUB_MODE_PID;
				m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
				float goal = GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr);
				G_RS485_Comm.SendNormalPidCmd(goal,BoxNr); 
		}
		else if((m_TempRange.close_temp_min < pid_temp) && (G_NormalConfigPara[BoxNr].AirControlEquipment > 0) && (m_TempRange.close_temp_min !=  m_TempRange.close_temp_max))
		{
			if(AfxMessageBox(_T("箱体内实时温度高于复位温度下限\r\n 需要先降箱体内实时温度吗？"),MB_YESNO) == IDYES)
			{
				m_SubMode   = 	SUB_MODE_READY;
			}
			else
			{
				m_SubMode				=	SUB_MODE_PID;
				m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
				float goal = GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr);
				if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_16
					|| ::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_24
					|| ::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40)
				{
					G_RS485_Comm.SendStepPIDCmd(goal,(goal - pid_temp)/10.0f, BoxNr);
				}
				else
				{
					G_RS485_Comm.SendNormalPidCmd(goal,BoxNr); 
				}
				
				CString msg;
				G_RS485_Comm.SendAirControlCmd(false,G_Meter[BoxNr].m_IsAcOn,BoxNr,&msg); 
			}
		}
		else
		{
				m_SubMode				=	SUB_MODE_PID;
				m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
				float goal = GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr);
				if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_16
					|| ::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_24
					|| ::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40)
				{
					G_RS485_Comm.SendStepPIDCmd(goal,(goal - pid_temp)/10.0f, BoxNr);
				}
				else
				{
					G_RS485_Comm.SendNormalPidCmd(goal,BoxNr); 
				}
				CString msg;
				G_RS485_Comm.SendAirControlCmd(false,G_Meter[BoxNr].m_IsAcOn,BoxNr,&msg); 
		}
		
	}
	else if(HeatCool == COOL_FIRST_HEAT_SECOND)
	{
		m_SubMode   = 	SUB_COOL_FIRST_MODE_READY;
	}
	

/*	

	m_SubMode   = 	SUB_MODE_DIRECT_HEAT;

	G_RS485_Comm.SendDirectHeatCmd(GetControlGoalTemp(SUB_MODE_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr),BoxNr);
	
	G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); //WriteLogFile("13",BoxNr+1,true);
*/

	if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
	{
		G_RS485_Comm.SendEPM1270Reset(false,BoxNr);
	}


#ifndef WINCE
	CButton *b = (CButton*)MainWnd->GetDlgItem(IDC_BUTTON_TAKEOUT); 

	if(b != NULL)
	{
		
		HBITMAP bitmap = b->GetBitmap();

		if(bitmap == NULL)
		{
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TESTING)));
		}
		else
		{
			DeleteObject(bitmap);
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TESTING)));
		}
	}
#endif

	InitSwitchStatus(BoxNr);

}

bool CWorkMode::IsPT100Caled(int BoxNr) 
{

#ifdef WINCE
	
#else
	if(G_MainDlg->m_DebugDlg[BoxNr].m_AdjustLow[0] == 0.0f
		&& G_MainDlg->m_DebugDlg[BoxNr].m_AdjustLow[1] == 0.0f
		&& G_MainDlg->m_DebugDlg[BoxNr].m_AdjustLow[2] == 0.0f
		&& G_MainDlg->m_DebugDlg[BoxNr].m_AdjustLow[3] == 0.0f
	)
	{
		return false;
	}

	if(G_MainDlg->m_DebugDlg[BoxNr].m_AdjustHigh[0] == 0.0f
		&& G_MainDlg->m_DebugDlg[BoxNr].m_AdjustHigh[1] == 0.0f
		&& G_MainDlg->m_DebugDlg[BoxNr].m_AdjustHigh[2] == 0.0f
		&& G_MainDlg->m_DebugDlg[BoxNr].m_AdjustHigh[3] == 0.0f
	)
	{
		return false;
	}
#endif	 
	return true;
}

SUB_MODE CWorkMode::AuotTestStep_NormalPID_2(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{

#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;


	cur_t	=	CTime::GetCurrentTime(); 
	span	=	cur_t - m_CurSubModeBeginTime;
	

	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	//如果时间已到，就进入下一步
			
	if( span.GetTotalMinutes() >= G_NormalConfigPara[BoxNr].PidTime)
	{
		
//		m_OpenDoorIng	= false;

		m_OpenDoor_Temp	=  GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr) + ::G_NormalConfigPara[BoxNr].OpenDoorReset_HeatDegree; 

		m_SubMode = SUB_MODE_STEP_PID_2;

		m_CurSubModeBeginTime = CTime::GetCurrentTime(); 
		
		
		int test_minutes = (10.0f)  * 10.0f /  G_NormalConfigPara[BoxNr].HeatSpeed;
			//(GetControlGoalTemp(SUB_MODE_STEP_PID) - GetControlGoalTemp(SUB_MODE_PID))* 10 / m_iStepPidSpeed;
		//SendControlCmd(CMD_STEP_PID_OUT,m_TempRange.open_temp_max + G_iHighHighLevelDelta + 10.0f/*GetControlGoalTemp(SUB_MODE_STEP_PID)*/,test_minutes);
		if(test_minutes < 1)
		{
			test_minutes = 1;
		}
		G_RS485_Comm.SendStepPIDCmd(m_TempRange.open_temp_max + G_iHighHighLevelDelta + 10.0f,test_minutes,BoxNr);

		
	}
	else
	{
//	    m_WorkStatus.GoalTemp = /*GetControlGoalTemp(SUB_MODE_PID)*/ (m_TempRange.open_temp_max + G_iHighHighLevelDelta) * (short)10; 
		int RemainTime = (G_NormalConfigPara[BoxNr].PidTime - span.GetTotalMinutes())  * 60 - span.GetSeconds(); 

		MainWnd->m_TestMsg.Format(_T("恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_PID,HeatCool,false,m_TempRange,BoxNr) ,(int)(RemainTime/60),(int)(RemainTime%60));
		MainWnd->UpdateData(false);
	}

	return m_SubMode;
}

void CWorkMode::AutoTestStep_StepPID_2(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	if((CurTemp  >=  (m_TempRange.open_temp_max + G_iHighHighLevelDelta + 10.0f))
		|| IsAllOpened(BoxNr))
	{
		if(IsAllOpened(BoxNr))
		{
		//	::WriteLogFile("AutoTestStep_StepPID  IsAllOpened",this->m_BoxNr + 1);
		}

		if(m_TempRange.close_temp_min == m_TempRange.close_temp_max)
		{//不用测复位，直接结束测试
			::WriteLogFile("AutoTestStep_StepPID  close_temp_min == close_temp_max",BoxNr + 1);
			ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode); 
		}
		else
		{

			m_SubMode = SUB_MODE_DIRECT_COOL;
			m_CurSubModeBeginTime	=	CTime::GetCurrentTime();

			G_RS485_Comm.SendStopHeatCmd(0,BoxNr);
			G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
			MainWnd->m_TestMsg.Format(_T("降温到 %5.1f度 "),m_TempRange.close_temp_min);
			MainWnd->UpdateData(false);

			
			if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
			{
				G_RS485_Comm.SendEPM1270Reset(true,BoxNr);
			}

		}

	}
	else
	{
		
		CTime cur = cur.GetCurrentTime();
		CTimeSpan span = cur - m_CurSubModeBeginTime; 
		

		if(m_iTestMode == TEST_MODE_OPEN_DOOR_RESET || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
		{
			OpenDoor_For_OpenFlashTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara);
		}
		else
		{
			
			
			MainWnd->m_TestMsg.Format(_T("线性升温到 %5.1f度 %02d:%02d"),m_TempRange.open_temp_max + G_iHighHighLevelDelta + 10.0f,span.GetTotalMinutes(),span.GetSeconds());
			MainWnd->UpdateData(false);
		}
		
	}
}

void CWorkMode::OpenDoor_For_CloseFlashTest(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	int	i;

	if(m_iTestMode == TEST_MODE_CLOSE_DOOR_REPEAT || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
	{
	
		if(m_SubMode == SUB_MODE_DIRECT_COOL)
		{//cool 
			
			if(IsAllClosed(BoxNr))
			{
				ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode);
			}
		
			CTimeSpan span = CTime::GetCurrentTime() - m_CloseDoorStartTime;
			
			if(G_Meter[BoxNr].m_IsDoorOpened == false)
			{//开始升温，只要有一个产品复位
				MainWnd->m_TestMsg.Format(_T("开关门：关门升温%5.1f度"),m_CloseDoorTopTemp);
				MainWnd->UpdateData(false);
				

				if(m_CloseDoorTopTemp < CurTemp)
				{
					m_CloseDoorStartTime = m_CloseDoorStartTime.GetCurrentTime();
					G_RS485_Comm.SendStopHeatCmd(0,BoxNr);
					m_CloseDoorStartTemp		=	CurTemp;
					m_CloseDoorTopTemp			=	-50,.0f;
					G_RS485_Comm.SendOpenDoorCmd(true,BoxNr);

					for(i = 0; i < G_iMaxLedNr[BoxNr]; i++)
					{
						if((G_SwitchTestResult[BoxNr][i].IsClosed) && (G_SwitchTestResult[BoxNr][i].IsCloseFlashTested == false))
						{
							G_SwitchTestResult[BoxNr][i].IsCloseFlashTested = true;
						}
					}
				
					
				}
			}
			else //if(G_Meter[BoxNr].m_IsDoorOpened == true)
			{//门开着，开始降温
				
				int count = 0;

				for(i = 0; i < G_iMaxLedNr[BoxNr]; i++)
				{
					if((G_SwitchTestResult[BoxNr][i].IsClosed) 
						&& (G_SwitchTestResult[BoxNr][i].IsCloseFlashTested == false)
						&& (CurTemp < m_TempRange.close_temp_max) 
						&& (G_SwitchTestResult[BoxNr][i].CloseTemp < m_TempRange.close_temp_max))
					{
						count ++;

						if(m_CloseDoorTopTemp < (G_SwitchTestResult[BoxNr][i].CloseTemp  + G_NormalConfigPara[BoxNr].OpenDoorReset_HeatDegree))
						{
							m_CloseDoorTopTemp = (G_SwitchTestResult[BoxNr][i].CloseTemp  + G_NormalConfigPara[BoxNr].OpenDoorReset_HeatDegree);
						}
					}
				}
				if(count > 0)
				{
					G_RS485_Comm.SendOpenDoorCmd(false,BoxNr);
					G_RS485_Comm.SendNormalPidCmd(m_CloseDoorTopTemp,BoxNr);
					MainWnd->m_TestMsg.Format(_T("开关门：关门升温到 %5.1f度,%d"),m_CloseDoorTopTemp );
					MainWnd->UpdateData(false);
				}
				else
				{
					MainWnd->m_TestMsg.Format(_T("开关门：开门降温"));
					MainWnd->UpdateData(false);
				}
			}
		}
	}
}

void CWorkMode::AutoTestStep_DircetCool(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;


	
	
	if(::G_NormalConfigPara[BoxNr].CoolSpeed > 0)
	{//降温也要控制
		if((CurTemp < GetControlGoalTemp(SUB_MODE_DIRECT_COOL,HeatCool,false,m_TempRange,BoxNr))
			|| IsAllClosed(BoxNr))
		{
			if(G_iMaxLedNr[BoxNr] != 192) 
			{
				G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); 
			}
			float goal = GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,false,m_TempRange,BoxNr);
			G_RS485_Comm.SendPidCoolCmd(goal,BoxNr,::G_NormalConfigPara[BoxNr].CoolDownAutoOpenDoor);
			m_SubMode = SUB_MODE_PID_COOL;
			m_CurSubModeBeginTime = CTime::GetCurrentTime();
			MainWnd->m_TestMsg.Format(_T("恒温到 %5.1f度"),GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,false,m_TempRange,BoxNr));
			MainWnd->UpdateData(false);
			m_CurSubModeBeginTime = CTime::GetCurrentTime();
		}
		else
		{//每分钟1度
			
			if(StartAirControl(ParentWnd,BoxNr,CurTemp) == -2)
			{}
			else
			{
				span = CTime::GetCurrentTime() - m_CurSubModeBeginTime;
				int sec = span.GetTotalSeconds();
				float goal = GetControlGoalTemp(SUB_MODE_DIRECT_COOL ,HeatCool,false,m_TempRange,BoxNr);
				MainWnd->m_TestMsg.Format(_T("全速降温到%5.1f度 %d:%02d"), goal,(sec/ 60),(sec % 60));
				MainWnd->UpdateData(false);
			}
			
		}
	}
	else
	{
		

		cur_t =  CTime::GetCurrentTime(); 
		
		if(m_iTestMode != TEST_MODE_CLOSE_DOOR_ONCE)
		{
			double stop_temp ;
			if(m_iTestMode == TEST_MODE_CLOSE_DOOR_REPEAT || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
			{
				OpenDoor_For_CloseFlashTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara);
			}
			else
			{
				if(::G_NormalConfigPara[BoxNr].StopTestType == 0)
				{//降温到最低复位温度还是一直降温
					stop_temp = m_TempRange.close_temp_min;
				}
				else
				{
					stop_temp = -50.0f;
				}
				if((CurTemp < stop_temp ) || (m_TempRange.close_temp_max ==  m_TempRange.close_temp_min)
					|| IsAllClosed(BoxNr))
				{
					
						ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode);
					
				}
				else
				{
					if(G_NormalConfigPara[BoxNr].StopTestType == 0)
					{
						MainWnd->m_TestMsg.Format(_T("降温到 %5.1f度 "),stop_temp);
					}
					else
					{
						MainWnd->m_TestMsg = _T("降温到所有产品都复位 ");
					}
					if(m_iTestMode == TEST_MODE_NORMAL)
					{//烘箱带空调，在一定温度以下自动开空调
						StartAirControl(ParentWnd,BoxNr,CurTemp);
					}
					MainWnd->UpdateData(false);
				}
			}
		}
		else //if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
		{
			
			{
				if((CurTemp < m_TempRange.close_temp_min)
					|| IsAllClosed(BoxNr))
				{
					if((GetLowestCloseTemp(BoxNr) < m_TempRange.close_temp_max)
						&& (GetLowestCloseTemp(BoxNr) < 200))
					{
						if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
						{
						
							::G_RS485_Comm.SendDirectHeatCmd(GetLowestOpenTemp(ParentWnd,BoxNr,CurTemp,SwitchConfigPara) - 5.0f,BoxNr);
							G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); 
							m_SubMode = SUB_MODE_HEAT_AGAIN;
							m_CurSubModeBeginTime	=	CTime::GetCurrentTime();
						}
						else
						{
							ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode);
						}
					}
					else
					{
						ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode);
					}
				}
				else
				{
					MainWnd->m_TestMsg.Format(_T("降温到 %5.1f度 "),m_TempRange.close_temp_min);
					MainWnd->UpdateData(false);
				}

				if(m_iTestMode == TEST_MODE_NORMAL)
				{//烘箱带空调，在一定温度以下自动开空调
					StartAirControl(ParentWnd,BoxNr,CurTemp);
				}
			}
		}
	}
}


bool CWorkMode::IsAllClosed(int BoxNr)
{
	int i;

	for(i = 0; i < G_iMaxLedNr[BoxNr]; i++)
	{
		if(m_iTestMode == TEST_MODE_CLOSE_DOOR_REPEAT || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
		{
			if((G_SwitchTestResult[BoxNr][i].IsUsed == true) 
			&& (G_SwitchTestResult[BoxNr][i].IsOpenned == true)
			&& (G_SwitchTestResult[BoxNr][i].IsClosed  == false)) 
			{
				return false;
			}

			if((G_SwitchTestResult[BoxNr][i].IsUsed == true) 
			&& (G_SwitchTestResult[BoxNr][i].IsOpenned == true)
			&& (G_SwitchTestResult[BoxNr][i].IsClosed  == true)
			&& (G_SwitchTestResult[BoxNr][i].IsCloseFlashTested  == false)) 
			{
				return false;
			}
		}
		else
		{
			if((G_SwitchTestResult[BoxNr][i].IsUsed == true) 
			&& (G_SwitchTestResult[BoxNr][i].IsOpenned == true)
			&& (G_SwitchTestResult[BoxNr][i].IsClosed  == false)
			) 
			{
				return false;
			}
		}

		


	}
	return true;
}

int CWorkMode::GetLowestOpenTemp(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	int i;
	int temp = 99999.9f;

	for(i = 0; i < G_iMaxLedNr[BoxNr]; i ++)
	{
		if(G_SwitchTestResult[BoxNr][i].IsOpenned && G_SwitchTestResult[BoxNr][i].IsUsed)
		{
			if(G_SwitchTestResult[BoxNr][i].OpenTemp < temp)
			{
				temp = G_SwitchTestResult[BoxNr][i].OpenTemp;
			}
		}
	}


	return temp;
}

float CWorkMode::GetHighestOpenTemp(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	int i;
	float temp = 0;

	for(i = 0; i < G_iMaxLedNr[BoxNr]; i ++)
	{
		if(G_SwitchTestResult[BoxNr][i].IsOpenned && G_SwitchTestResult[BoxNr][i].IsUsed)
		{
			if(G_SwitchTestResult[BoxNr][i].OpenTemp > temp)
			{
				temp = G_SwitchTestResult[BoxNr][i].OpenTemp;
			}
		}
	}

	

	return temp;
}

int CWorkMode::GetHighestCloseTemp(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	int i;
	int temp = 0;

	for(i = 0; i < G_iMaxLedNr[BoxNr]; i ++)
	{
		if(G_SwitchTestResult[BoxNr][i].IsClosed && G_SwitchTestResult[BoxNr][i].IsUsed)
		{
			if(G_SwitchTestResult[BoxNr][i].CloseTemp > temp)
			{
				temp = G_SwitchTestResult[BoxNr][i].CloseTemp;
			}
		}
	}

	if(temp > m_TempRange.close_temp_max )
	{
		temp = m_TempRange.close_temp_max;
	}
	temp += 5;

	return temp;
}

void CWorkMode::AutoTestStep_PidCool(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	int	span_minutes,span_seconds;
	CTime cur = cur.GetCurrentTime();
	CTimeSpan ts = cur - m_CurSubModeBeginTime; 
	span_seconds = (G_NormalConfigPara[BoxNr].PidTime * 60) - ts.GetTotalSeconds();
	span_minutes	 = span_seconds / 60;

	span_seconds = (span_seconds % 60);
#ifdef WINCE
	if(ts.GetTotalSeconds() > (G_NormalConfigPara[BoxNr].PidCoolTime * 60)
#else
	if(ts.GetTotalSeconds() > (G_NormalConfigPara[BoxNr].PidTime * 60)
#endif
	|| IsAllClosed(BoxNr))
	{
		m_SubMode = SUB_MODE_STEP_COOL;
		m_CurSubModeBeginTime = CTime::GetCurrentTime();
		float goal = GetControlGoalTemp(SUB_MODE_STEP_COOL,HeatCool,false,m_TempRange,BoxNr);
		int minutes = (CurTemp - goal) / (::G_NormalConfigPara[BoxNr].CoolSpeed / 10.0f) + 0.5f;
		if(minutes < 1)
		{
			minutes = 1;
		}
		if(::G_NormalConfigPara[BoxNr].BoxType == BOX_OIL_BATHS)
		{
			G_RS485_Comm.SendStepPidCoolCmd(goal,minutes,BoxNr,true); 
		}
		else
		{
			//CString s;
			//s.Format(_T("goal = %5.1f,minutes= %d,false"),goal,minutes);
			//AfxMessageBox(s);
			G_RS485_Comm.SendStepPidCoolCmd(goal,minutes,BoxNr,false);//::G_NormalConfigPara[BoxNr].CoolDownAutoOpenDoor); 
		}
		MainWnd->m_TestMsg.Format(_T("线性降温到 %5.1f度 %02d:%02d"),goal,span_minutes,span_seconds % 60);
		MainWnd->UpdateData(false);
	}
	else if(CurTemp > GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,false,m_TempRange,BoxNr)  +  3.0f)
	{//高于直接降温的温度不计时间
		/*if(CurTemp > 40.0f)
		{
			if(G_Meter[BoxNr].m_IsDoorOpened == false)
			{
				G_Meter[BoxNr].m_IsDoorOpened = true;
				G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
			}
		}
		else
		{
			if(G_Meter[BoxNr].m_IsDoorOpened == true
			{
				G_Meter[BoxNr].m_IsDoorOpened = false;
				G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); 
			}
		}*/
		m_CurSubModeBeginTime	= CTime::GetCurrentTime(); 
		MainWnd->m_TestMsg.Format(_T("降温到 %5.1f度"),GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,false,m_TempRange,BoxNr));
		MainWnd->UpdateData(false);
		StartAirControl(ParentWnd,BoxNr,CurTemp);
	}
	else
	{
		

		

		if(m_iTestMode == TEST_MODE_OPEN_DOOR_RESET || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
		{
			OpenDoor_For_OpenFlashTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara);

		}
		else
		{
			
			MainWnd->m_TestMsg.Format(_T("恒温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_PID_COOL,HeatCool,false,m_TempRange,BoxNr),span_minutes,span_seconds % 60);
			MainWnd->UpdateData(false);
		}

		StartAirControl(ParentWnd,BoxNr,CurTemp);
		
	}


}

void CWorkMode::AutoTestStep_StepCool(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	if((CurTemp  <  GetControlGoalTemp(SUB_MODE_STEP_COOL,HeatCool,false,m_TempRange,BoxNr))
		|| IsAllClosed(BoxNr))
	{
		ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode); 
	}
	else
	{
		

		int	span_minutes,span_seconds;
		CTime cur = cur.GetCurrentTime();
		CTimeSpan ts = cur - m_CurSubModeBeginTime; 
		span_seconds = ts.GetTotalSeconds();
		span_minutes	 = span_seconds / 60;
		float goal = GetControlGoalTemp(SUB_MODE_STEP_COOL,HeatCool,false,m_TempRange,BoxNr);

		if(::G_Meter[BoxNr].m_PcbTestMode == PCB_SUB_MODE_STEP_PID_COOL)
		{
			static int count = 0;
			if((G_Meter[BoxNr].m_Pv > (G_Meter[BoxNr].m_Sv + 2.0f))
				&& (G_Meter[BoxNr].m_Speed > (0 -(::G_NormalConfigPara[0].CoolSpeed / 10.0f) /2))
				&& (G_Meter[BoxNr].m_TempAnswerPara.HeatPercent <= -80))
			{
				count++;
				if(count > 10)
				{
					
					int minutes = (CurTemp - goal) / (::G_NormalConfigPara[0].CoolSpeed / 10.0f) + 0.5f;
					if(minutes < 1)
					{
						minutes = 1;
					}
					G_RS485_Comm.SendStepPidCoolCmd(goal,minutes,BoxNr,true);
				}
			}
			else
			{
				count = 0;
			}
		}
		

		if(m_iTestMode == TEST_MODE_OPEN_DOOR_RESET || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
		{
			OpenDoor_For_OpenFlashTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara);

		}
		else
		{
			
			MainWnd->m_TestMsg.Format(_T("线性降温到 %5.1f度 %02d:%02d"),goal,span_minutes,span_seconds % 60);
			MainWnd->UpdateData(false);
		}

		StartAirControl(ParentWnd,BoxNr,CurTemp);
		
	}

}
void CWorkMode::AutoTestStep_HeatAgain(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);


	if((GetLowestCloseTemp(BoxNr) < m_TempRange.close_temp_max)
						&& (GetLowestCloseTemp(BoxNr) < 200))
	{//此时至少有一个复位的产品	
		
		MainWnd->m_TestMsg.Format(_T("升温到 %5.1f度 "),(float)GetLowestOpenTemp(ParentWnd,BoxNr,CurTemp,SwitchConfigPara) - 5.0f);
		MainWnd->UpdateData(false);
		if(CurTemp > GetLowestOpenTemp(ParentWnd,BoxNr,CurTemp,SwitchConfigPara) - 5.0f)
		{
			::G_RS485_Comm.SendStopHeatCmd(0,BoxNr);

			ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode);
			
		}
	}
	else
	{
			G_RS485_Comm.SendStopHeatCmd(0,BoxNr);

			ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode);
			
	}

}
void CWorkMode::AutoTestStep_TopPID(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{

#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	CTimeSpan span =  CTime::GetCurrentTime() - m_CurSubModeBeginTime;

	if(span.GetTotalMinutes() >= ::G_NormalConfigPara[BoxNr].TopPidMinutes) 
	{
				m_SubMode = SUB_MODE_DIRECT_COOL;
		
				m_CurSubModeBeginTime	=	CTime::GetCurrentTime();
				G_RS485_Comm.SendStopHeatCmd(0,BoxNr);

				if(::G_iMaxLedNr[BoxNr] == 192)
				{//开门就是开空调
					if(::G_NormalConfigPara[BoxNr].AirControlEquipment == 0)
					{
						G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
					}
					else
					{
						if((CurTemp < ::G_NormalConfigPara[BoxNr].AirControlEquipment) && (::G_NormalConfigPara[BoxNr].AirControlEquipment >  0))
						{
							G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
						}
					}
				}
				else
				{
					G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
				}
				MainWnd->m_TestMsg.Format(_T("降温到 %5.1f度 "),GetControlGoalTemp(SUB_MODE_DIRECT_COOL,HeatCool,false,m_TempRange,BoxNr)  );
				MainWnd->UpdateData(false);

				
				if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
				{
					G_RS485_Comm.SendEPM1270Reset(true,BoxNr);
				}
	}
	else
	{

		MainWnd->m_TestMsg.Format(_T("顶部恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_STEP_TOP_PID,HeatCool,false,m_TempRange,BoxNr),span.GetTotalMinutes(),span.GetSeconds());
		MainWnd->UpdateData(false);
	
	}

}

void CWorkMode::OnClickStartButton(CWnd * ParentWnd,int BoxNr,float CurTemp[4],SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	float PidTemp = (CurTemp[0] + CurTemp[1] + CurTemp[2] + CurTemp[3]) / 4.0f;


	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);



	if(m_SubMode == SUB_MODE_STOP)
	{
		this->EnterAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara); 
	}
	else if(::G_NormalConfigPara[0].BoxType == BOX_OIL_BATHS && m_SubMode == SUB_MODE_READY)
	{
		CString str;

		if(G_iLangID == 0)
		{
			str.Format(_T("%d 号油浴正在等待测试,\r\n开始测试选 \"是\",终止测试选\"否\""),BoxNr+1);
		}
		else
		{
			str.Format(_T("%d# Oil bath is ready to test, Stop it?"),BoxNr+1);
		}
		int ret = MainWnd->MessageBox(str, strMsgCaption[G_iLangID],MB_YESNO | MB_ICONQUESTION);
		if( ret == IDYES)
		{
			this->EnterAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara);
		}
		else
		{
			m_SubMode = SUB_MODE_STOP;
			G_RS485_Comm.SendStopHeatCmd(-50.0f,BoxNr);
		}
	}
	else
	{
		CString str;
		if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE && m_SubMode == SUB_MODE_DIRECT_COOL)
		{//提前终止测试功能

			if(G_iLangID == 0)
			{
				str.Format(_T("%d 号烘箱正在测试，需要停止吗?"),BoxNr+1);
			}
			else
			{
				str.Format(_T("%d# Oven is testing, Stop it?"),BoxNr+1);
			}
			int ret = MainWnd->MessageBox(str, strMsgCaption[G_iLangID],MB_OKCANCEL | MB_ICONQUESTION);
			if( ret != IDOK)
			{
				return;
			}

			G_NormalConfigPara[BoxNr].RepeatTestCounts = 0;
			
			G_RS485_Comm.SendDirectHeatCmd(GetHighestCloseTemp(ParentWnd,BoxNr,PidTemp,SwitchConfigPara),BoxNr);

			G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); 
			m_SubMode = SUB_MODE_STOP;
		}
		else if(m_iTestMode == TEST_MODE_NORMAL && m_SubMode == SUB_MODE_STEP_PID)
		{
			if(G_iLangID == 0)
			{
				str.Format(_T("%d 号烘箱正在升温，需要提前降温吗?"),BoxNr+1);
			}
			else
			{
				str.Format(_T("%d# is heating, Stop it?"),BoxNr+1);
			}
			
			int ret = MainWnd->MessageBox(str, strMsgCaption[G_iLangID],MB_OKCANCEL | MB_ICONQUESTION);
			if( ret != IDOK)
			{
				return;
			}

			G_RS485_Comm.SendStopHeatCmd(0,BoxNr);
			G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 

			m_SubMode = SUB_MODE_DIRECT_COOL;

			m_CurSubModeBeginTime = CTime::GetCurrentTime();
#ifdef WINCE
			CWnd *button;
			button = MainWnd->GetDlgItem(IDC_BUTTON_START);
			{
				CString msg[] = 
				{
					_T("停止升温"),
					_T("Stop Heat"),
				};
				button->SetWindowText(msg[::G_NormalConfigPara[BoxNr].Language]);
			}
#endif
		}
		else
		{
			if(G_iLangID == 0)
			{
				str.Format(_T("%d 号烘箱正在测试，需要停止吗?"),BoxNr+1);
			}
			else
			{
				str.Format(_T("%d# is testing, Stop it?"),BoxNr+1);
			}
			
			int ret = MainWnd->MessageBox(str, strMsgCaption[G_iLangID],MB_OKCANCEL | MB_ICONQUESTION);
			if( ret != IDOK)
			{
				return;
			}

			G_NormalConfigPara[BoxNr].RepeatTestCounts = 0;

			ExitAutoTest(ParentWnd,BoxNr,PidTemp,SwitchConfigPara,MainWnd->m_Barcode);
#ifndef WINCE
			CWnd *button;
			button = MainWnd->GetDlgItem(IDC_BUTTON_TAKEOUT);
			button->EnableWindow(true);
#endif			
			m_EndTestTime = CTime::GetCurrentTime(); 
		}
	}
}

void CWorkMode::DealStatusNormalClose(CWnd * ParentWnd,int BoxNr,int SwitchNr,SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status,float m_PidTemp,
												bool m_DoorIsOpenned,int TestMode, bool bSwitchMustBeAdjusted)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	if(this->GetOpenMode())
	{//升温
			//if(m_SwitchType == 0)
			{//常闭
				//假如是闪动产品，就将最后一次动作时的温度计作动作温度
				if(TestMode == TEST_MODE_OPEN_DOOR_RESET || TestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
				{//否则就要对同一个产品反复开关门了
					if(SwitchTestResult->IsClosing == true
						&& SwitchTestResult->IsUsed == true
						&& SwitchTestResult->IsOpenned == true
						)
					{
						SwitchTestResult->IsOpenFlash = true;
#ifndef WINCE
						SwitchTestResult->OpenFlashTemp	= 1000.0f + m_PidTemp;
#endif
					}
				}
				else
				{
					if(SwitchTestResult->IsClosing == true
						&& SwitchTestResult->IsUsed == true
						&& SwitchTestResult->IsOpenned == true
						/*&& (G_NormalConfigPara[BoxNr].NoFlashTest == false)*/
						)
					{
						if (bSwitchMustBeAdjusted == true 
							&& (m_SubMode == SUB_MODE_DIRECT_HEAT || m_SubMode == SUB_MODE_PID))  
						{//可调KST不测闪动
						}
						else
						{
							SwitchTestResult->IsOpenFlash = true;
#ifndef WINCE
							SwitchTestResult->OpenFlashTemp	= 2000.0f + m_PidTemp;
#endif
						}
						SwitchTestResult->IsOpenned = false; 
						SwitchTestResult->OpenTemp = 0.0f;
				
					}

					
					if( pcb_flash_status )
					{
						if((G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_PTC) || (G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_ANTI_PTC))
						{
							if(SwitchTestResult->IsClosing == false && m_SubMode != SUB_MODE_STOP)
							{
								SwitchTestResult->IsNoPTC = true;
							}
						}
						else if(G_NormalConfigPara[BoxNr].BoxType == BOX_128 && G_NormalConfigPara[BoxNr].InterfaceBoardType == HIGH_VOLT_BOARD)
						{
							if(pcb_flash_status)
							{//闪动
								 SwitchTestResult->IsOpenFlash = true;
							}
						}
						else if(TestMode != TEST_MODE_CLOSE_DOOR_ONCE)
						{
							//if(G_MainDlg->m_NoPcbFlashTest == 0)
							{
								SwitchTestResult->IsOpenFlash = true;
#ifndef WINCE
								SwitchTestResult->OpenFlashTemp = 3000+m_PidTemp;
#endif
							}
						
						}
					}
					else
					{
						if((G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_PTC) || (G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_ANTI_PTC))
						{//任何时候检测到有PTC都判断为有PTC,防止PTC安装不到位的情况，也防止保护器断开的瞬间PTC没有接入电路的误判
							if(SwitchTestResult->IsClosing == false)
							{
								SwitchTestResult->IsNoPTC = false;
							}
						}
					}


				}
				

				//判断是否是刚打开
				if(SwitchTestResult->IsOpenned == false
					&& SwitchTestResult->IsClosing == false
					&& SwitchTestResult->IsUsed == true
					)
				{
					SwitchTestResult->IsOpenned = true;
					
#ifdef _CHENG_HONG_

					if((m_PidTemp >= this->m_SwitchConfigPara.MainLevel - m_SwitchConfigPara.MainLevelSpanMinus - 1.5f)
						&& (m_PidTemp <= this->m_SwitchConfigPara.MainLevel + m_SwitchConfigPara.MainLevelSpanPlus + 1.5f))
					{
						SwitchTestResult->OpenTemp	= m_SwitchConfigPara.MainLevel + (m_PidTemp - m_SwitchConfigPara.MainLevel) * m_SwitchConfigPara.MainLevelSpanMinus / (( m_SwitchConfigPara.MainLevelSpanMinus + m_SwitchConfigPara.MainLevelSpanPlus) / 2 + 1.5f) ;
					}
					else
					{
						SwitchTestResult->OpenTemp	=  m_PidTemp + ::G_SwitchTempAdjust[m_BoxNr][SwitchNr][0];
					}
#else
					if(G_NormalConfigPara[0].BoxType == BOX_KSD302_TIME 
						|| G_NormalConfigPara[0].BoxType == BOX_KSD302_TEMP)
					{
						if(SwitchNr < 64)
						{
							SwitchTestResult->OpenTemp	=  m_PidTemp + ::G_AdjustTemp[BoxNr][0][SwitchNr];
						}
						else
						{
							SwitchTestResult->OpenTemp	=  m_PidTemp + ::G_AdjustTemp[BoxNr][0][SwitchNr - 64];
						}
					}
					else
					{
						SwitchTestResult->OpenTemp	=  m_PidTemp + ::G_AdjustTemp[BoxNr][0][SwitchNr];
					}
#endif
#ifndef WINCE
					if(TestMode == TEST_MODE_NORMAL_PID_CAL_OPEN_TIME)
					{
						//SwitchTestResult->OpenTime	= GetSpanSeconds(CTime::GetCurrentTime());//借用于开始恒温时间
						//SwitchTestResult->CloseTime	= GetSpanSeconds(CTime::GetCurrentTime());//借用于动作时间时间
						if(m_CurSubModeBeginTime == 0)
						{
							SwitchTestResult->OpenTime = 0;
						}
						else
						{
							CTimeSpan ts = CTime::GetCurrentTime() - m_CurSubModeBeginTime;
							SwitchTestResult->OpenTime = ts.GetTotalSeconds();
						}
					}
					else

					{
						SwitchTestResult->OpenTime	= ::GetSpanSeconds(CTime::GetCurrentTime()) ;
					}
#endif

				}

				//判断是否是刚打开又闭合，则重新设置为不动作
				if(TestMode == TEST_MODE_OPEN_DOOR_RESET || TestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
				{
				}
				else
				{
					if(SwitchTestResult->IsOpenned == true
						&& SwitchTestResult->IsClosing == true
						&& SwitchTestResult->IsUsed == true
						)
					{
						SwitchTestResult->IsOpenned = false; 
						SwitchTestResult->OpenTemp	= 0;
	#ifndef WINCE
						SwitchTestResult->OpenTime	= 0;
	#endif
					}
				}

			}
			
	}//升温



	else if(GetCloseMode())
	{//降温
		//if(m_SwitchType == 0)
		{//常闭
			//判断是否是刚复位过
			if(TestMode == TEST_MODE_CLOSE_DOOR_ONCE)
			{
				if(m_SubMode == SUB_MODE_HEAT_AGAIN)
				{
					if(pcb_flash_status)
					{
						if(SwitchTestResult->IsCloseFlash == false)
						{
								SwitchTestResult->IsCloseFlash		= true;
#ifndef WINCE
								SwitchTestResult->CloseFlashTemp	= 9000+m_PidTemp;
#endif
						}
						
					}

					if(SwitchTestResult->IsClosing		== false
						&& SwitchTestResult->IsUsed		== true
						&& SwitchTestResult->IsClosed	== true
						&& SwitchTestResult->IsCloseFlash == false
						)
					{
							SwitchTestResult->IsCloseFlash		= true;
#ifndef WINCE
							SwitchTestResult->CloseFlashTemp	= 5000.0f + m_PidTemp;
							
#endif
					}
				}
				else
				{
					if(SwitchTestResult->IsClosing	== false
					&& SwitchTestResult->IsUsed		== true
					&& SwitchTestResult->IsClosed	== true
					&& SwitchTestResult->IsCloseFlash == false
					&& G_NormalConfigPara[BoxNr].BoxType != HEAT_BOX_TYPE_PTC
					&& G_NormalConfigPara[BoxNr].BoxType != HEAT_BOX_TYPE_ANTI_PTC)
					{
						SwitchTestResult->IsCloseFlash		= true;
#ifndef WINCE
						SwitchTestResult->CloseFlashTemp	= 2000.0f + m_PidTemp;
#endif
					}

					if(pcb_flash_status)
					{
						if(TestMode != TEST_MODE_CLOSE_DOOR_REPEAT && TestMode != TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
						{
							if(SwitchTestResult->IsCloseFlash == false)
							{
								{
									SwitchTestResult->IsCloseFlash		= true;
#ifndef WINCE
									SwitchTestResult->CloseFlashTemp	= 7000+m_PidTemp;
#endif
								}
							}
						}
					}
				}
			}
			else //TestMode != TEST_MODE_CLOSE_DOOR_ONCE
			{
				if(SwitchTestResult->IsClosing		== false
					&& SwitchTestResult->IsUsed		== true
					&& SwitchTestResult->IsClosed	== true
					&& SwitchTestResult->IsCloseFlash == false
					//&& (G_NormalConfigPara[BoxNr].NoFlashTest  == false)
					)
				{
					if((m_iTestMode == TEST_MODE_CLOSE_DOOR_REPEAT || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET) && (G_Meter[BoxNr].m_IsDoorOpened == false))
					{
						if(m_PidTemp < SwitchTestResult->OpenTemp)
						{
		
							SwitchTestResult->IsCloseFlash		= true;
#ifndef WINCE
							SwitchTestResult->CloseFlashTemp	= 1000.0f + m_PidTemp;
#endif
						}
					}
					else
					{
						
					SwitchTestResult->IsCloseFlash		= true;

		
#ifndef WINCE
					SwitchTestResult->CloseFlashTemp	= m_PidTemp;
#endif
					}
				}

				if(pcb_flash_status)
				{
					if(TestMode != TEST_MODE_CLOSE_DOOR_REPEAT 
						&& TestMode != TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET
						&& G_NormalConfigPara[BoxNr].BoxType != HEAT_BOX_TYPE_PTC)
					{
						if(SwitchTestResult->IsCloseFlash == false)
						{
						
							SwitchTestResult->IsCloseFlash		= true;
#ifndef WINCE
							SwitchTestResult->CloseFlashTemp	= 6000+m_PidTemp;
#endif
						}
					}
					else if(G_NormalConfigPara[BoxNr].BoxType == BOX_128 && G_NormalConfigPara[BoxNr].InterfaceBoardType == HIGH_VOLT_BOARD)
					{
						if(pcb_flash_status)
						{//闪动
							 SwitchTestResult->IsOpenFlash = true;
						}
					}
				}
				
			
			}

				
			if(SwitchTestResult->IsOpenned == true
				&& SwitchTestResult->IsClosing == true
				&& SwitchTestResult->IsClosed == false
				&& SwitchTestResult->IsUsed == true)
			{//记录第一次复位时的温度

				SwitchTestResult->IsClosed	= true;

				SwitchTestResult->CloseTemp = m_PidTemp + ::G_AdjustTemp[BoxNr][1][SwitchNr];
#ifndef WINCE
				SwitchTestResult->CloseTime = GetSpanSeconds(CTime::GetCurrentTime());
#endif

			}
		}
		
	}//降温
}

void CWorkMode::DealStatusNormalOpen(int BoxNr,int SwitchTestID,SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status,float m_PidTemp,
												bool m_DoorIsOpenned,int TestMode)
{
	if(this->GetOpenMode())
	{//升温
			
			{//常开
				//假如是闪动产品，就将最后一次动作时的温度计作动作温度
				if(SwitchTestResult->IsClosing		== false
					&& SwitchTestResult->IsUsed		== true
					&& SwitchTestResult->IsOpenned	== true
					&& G_NormalConfigPara[BoxNr].NoFlashTest == false
					)
				{
					SwitchTestResult->IsOpenFlash	= true;
					SwitchTestResult->IsOpenned		= false; 
					SwitchTestResult->OpenTemp		= 0.0f;
				}

				//判断是否是刚打开
				if(SwitchTestResult->IsOpenned		== false
					&& SwitchTestResult->IsClosing	== true
					&& SwitchTestResult->IsUsed		== true
					)
				{
					SwitchTestResult->IsOpenned		= true;
					/*if(G_NormalConfigPara[BoxNr].BoxType == BOX_YangZhouYingDi)
					{
						SwitchTestResult->OpenTemp = m_PidTemp + ((m_CurTemp[0] + m_CurTemp[1]) / 2 - m_PidTemp) / 4 * (4 - ( SwitchTestID % 8)); 
					}
					else*/
					{
						SwitchTestResult->OpenTemp = m_PidTemp + G_AdjustTemp[0][0][SwitchTestID];
					}
					
				}

				if(G_NormalConfigPara[BoxNr].BoxType == BOX_128 && G_NormalConfigPara[BoxNr].InterfaceBoardType == HIGH_VOLT_BOARD)
				{
					if(pcb_flash_status)
					{//闪动
						 SwitchTestResult->IsOpenFlash = true;
					}
				}
			}
	}//升温
	else if(GetCloseMode())
	{//降温
		
		{//常开
			//判断是否是刚复位过
			if(SwitchTestResult->IsClosing		== true
				&& SwitchTestResult->IsUsed		== true
				&& SwitchTestResult->IsClosed	== true
				)
			{
				if(TestMode == TEST_MODE_CLOSE_DOOR_REPEAT || TestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
				{
					SwitchTestResult->IsCloseFlash = true;
				}
				else
				{
					SwitchTestResult->IsClosed		= false; 
					SwitchTestResult->CloseTemp		= 0.0f;
				}
			}


			if(G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_PTC)
			{
				if(pcb_flash_status)
				{
					if(SwitchTestResult->IsClosing == false)
					{
						SwitchTestResult->IsNoPTC = true;
					}
				}

			}
			else if(G_NormalConfigPara[BoxNr].BoxType == BOX_128_PTC_NOT_TESTED)
			{
			}
			else if(G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_ANTI_PTC)
			{
				if((SwitchTestResult->IsOpenned == true ) && (SwitchTestResult->IsClosed == false))
				{
					if(pcb_flash_status)
					{
						if(m_SubMode != SUB_MODE_STOP)
						{
							SwitchTestResult->IsNoPTC  = true;
						}
					}
					else
					{
						if(SwitchTestResult->IsClosing == false)
						{
							SwitchTestResult->IsNoPTC = false;
						}
					}
				}
			}
			else
			{
				if(G_NormalConfigPara[BoxNr].BoxType == BOX_128 && G_NormalConfigPara[BoxNr].InterfaceBoardType == HIGH_VOLT_BOARD)
				{
					if(pcb_flash_status)
					{//闪动
						 SwitchTestResult->IsCloseFlash = true;
					}
				}
				else if(pcb_flash_status && (G_NormalConfigPara[BoxNr].NoFlashTest == false))
				{
					SwitchTestResult->IsCloseFlash = true;
				}
			}
								
			if(SwitchTestResult->IsUsed == true
				&& SwitchTestResult->IsOpenned == true
				&& SwitchTestResult->IsClosed == false
				&& SwitchTestResult->IsClosing == false)
			{//记录第一次复位时的温度

				SwitchTestResult->IsClosed = true;

				if(m_DoorIsOpenned == true && (TestMode == TEST_MODE_CLOSE_DOOR_REPEAT || TestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET))
				{
					SwitchTestResult->CloseTemp = m_PidTemp + G_AdjustTemp[BoxNr][1][SwitchTestID];
				}
				else
				{
					SwitchTestResult->CloseTemp = m_PidTemp + G_AdjustTemp[BoxNr][1][SwitchTestID];
				}
				
			
			}//记录第一次复位时的温度
		}//常开
	}//降温
}

void CWorkMode::Send_MES(RECORD record,bool bMsg)
{//拓邦
//上传MES
#ifndef WINCE
	const CString strAgent;

	CString   response = L"";
	CString strMethod = L"";

	CString strUrl ;
	CString  postData; 
	CString  s;

	for(int i = 0; i < 128; i++)
	{
		strUrl = ::G_NormalConfigPara[0].MES_URL;
		
		if(strUrl.GetLength() < 5)
		{
			break;
		}
	//	CRecordViewDlg::CreateMES_String(&record,&postData);

		postData	= _T("04;");
		//postData	+= record.Tester;
		postData	+= _T("9994;");
		postData	+= record.Memo;
		strUrl		+= postData;
		strUrl		+= _T(";温度开关测试;温度开关;温度开关;");
		
		CTime BeginTestTime	= STD_TIME + CTimeSpan(0,0,record.StartTime ,0);
		s.Format(_T("%04d%02d%02d%02d%02d;"),	BeginTestTime.GetYear(),BeginTestTime.GetMonth(),BeginTestTime.GetDay(),BeginTestTime.GetHour(),BeginTestTime.GetMinute());
		strUrl		+= s;
		if(record.TestResult[i].IsUsed)
		{
			
			CONTROL_TEMP_RANGE temp_range;
			HEAT_COOL_ORDER HeatOrCool = ::CheckHeatOrCoolMode(0,record.ConfigPara,&temp_range,record.BoxType); 

			TEST_RESULT_LEVEL level = CheckTestLevel(record.ConfigPara,record.TestResult[i],HeatOrCool,temp_range,record.TestResult[i],record.BoxType);
			
			if(record.TestResult[i].OpenTemp >=  temp_range.open_temp_min && record.TestResult[i].OpenTemp <= temp_range.open_temp_max)
			{
				s.Format(_T("%d;动作温度:%5.1f::PASS;"),i+1,record.TestResult[i].OpenTemp);
			}
			else
			{
				s.Format(_T("%d;动作温度:%5.1f::FAIL;"),i+1,record.TestResult[i].OpenTemp);
			}
			strUrl		+= s;

			if(record.TestResult[i].CloseTemp >=  temp_range.close_temp_min && record.TestResult[i].CloseTemp <= temp_range.close_temp_max)
			{
				s.Format(_T(";复位温度:%5.1f::FAIL;"),i+1,record.TestResult[i].CloseTemp);
			}
			else
			{
				s.Format(_T(";复位温度:%5.1f::FAIL;"),i+1,record.TestResult[i].CloseTemp);
			}
			strUrl		+= s;

			//s += GetLevelName(level);
			//s += _T(";");
	
		}
		else
		{
			continue;
			//s .Format(_T("%d,无产品;"),i+1);
			
		}
		
			
		
		postData = _T("");

		if(HttpTools::HttpRequest(strMethod,strUrl,postData,response,IE_AGENT,bMsg) != HTTP_SUCCESS)
		{
			break;
		}
	}
#endif
	}

void TcharToChar(const TCHAR* tchStr, char* chStr, int bufSize)
{
    if (!tchStr || !chStr || bufSize <= 0)
        return;

    USES_CONVERSION;
    // 转换并安全拷贝
    strncpy(chStr, T2A(tchStr), bufSize - 1);
    chStr[bufSize - 1] = 0;
}

void CWorkMode::Send_Better_MES(RECORD *record)
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
	addr.sin_port = htons(::G_NormalConfigPara[0].MES_TCP_Port);        // 端口
   
	char buf[1000];
	sprintf(buf,"%d.%d.%d.%d",G_NormalConfigPara[0].MES_IP[0],
		G_NormalConfigPara[0].MES_IP[1],
		G_NormalConfigPara[0].MES_IP[2],
		G_NormalConfigPara[0].MES_IP[3]);
	addr.sin_addr.s_addr = inet_addr(buf); // IP

    if(connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        AfxMessageBox(L"连接失败");
        return;
    }

    char sendBuf[1000];

	memset(sendBuf,0,sizeof(sendBuf));
	memset(buf,0,sizeof(buf));

	//strcat(sendBuf,"{测试时间:");
	strcat(sendBuf,"{");

	//strcat(sendBuf,"设备编码:");
	TcharToChar(G_NormalConfigPara[0].MES_Equipment_Code,buf,sizeof(buf));
	strcat(sendBuf,buf);
	strcat(sendBuf,",");

	CTime starttime = STD_TIME + CTimeSpan(0,0,record->StartTime,0);
	sprintf(buf,"%d/%d/%d %d:%d:%d",starttime.GetYear(),starttime.GetMonth(),starttime.GetDay(),
		starttime.GetHour(),starttime.GetMinute(),starttime.GetSecond());
	strcat(sendBuf,buf);
	strcat(sendBuf,",");

	
	
	//strcat(sendBuf,"工作站编码:");
	TcharToChar(G_NormalConfigPara[0].MES_WorkStation_Code,buf,sizeof(buf));
	strcat(sendBuf,buf);
	strcat(sendBuf,",");

	//strcat(sendBuf,"工作站名称:");
//	TcharToChar(G_NormalConfigPara[0].MES_WorkStation_Name,buf,sizeof(buf));
//	strcat(sendBuf,buf);
//	strcat(sendBuf,",");

	//strcat(sendBuf,"工单编码:");
	TcharToChar(G_NormalConfigPara[0].MES_WorkSheet_Code,buf,sizeof(buf));
	strcat(sendBuf,buf);
	strcat(sendBuf,",");

	//strcat(sendBuf,"产品编码:");
	TcharToChar(G_NormalConfigPara[0].MES_Product_Code,buf,sizeof(buf));
	strcat(sendBuf,buf);
	strcat(sendBuf,",");


	//strcat(sendBuf,"产品名称:");
//	TcharToChar(G_NormalConfigPara[0].MES_Product_Name,buf,sizeof(buf));
//	strcat(sendBuf,buf);
//	strcat(sendBuf,",");

		
	float open_max_temp, open_min_temp,open_avr_temp,close_max_temp,close_min_temp,close_avr_temp;
	int total_product,total_main_level;
	CRecordViewDlg::TongJi(record,&open_max_temp, &open_min_temp, &open_avr_temp, &close_max_temp, &close_min_temp, &close_avr_temp, &total_product, &total_main_level);
						
	//strcat(sendBuf,"产品数:");
	sprintf(buf,"%d,",total_product);
	strcat(sendBuf,buf);
	
	//strcat(sendBuf,"良品数:");
	sprintf(buf,"%d,",total_main_level);
	strcat(sendBuf,buf);

	//strcat(sendBuf,"良品率:");
	if(total_product > 0)
	{
		sprintf(buf,"%5.1f%%,",total_main_level * 100.0f /total_product );
	}
	else
	{
		sprintf(buf,"%5.1f%%,",0.0f );
	}
	strcat(sendBuf,buf);

	//strcat(sendBuf,"不良品数:");
	sprintf(buf,"%d,",total_product - total_main_level);
	strcat(sendBuf,buf);

	//strcat(sendBuf,"不良品率:");
	if(total_product > 0)
	{
		sprintf(buf,"%5.1f%%,",100.0f- total_main_level * 100.0f /total_product );
	}
	else
	{
		sprintf(buf,"%5.1f%%,",0.0f );
	}
	strcat(sendBuf,buf);

	
	//CONTROL_TEMP_RANGE	m_TempRange;
	int					m_BoxNr = 0;
	CheckHeatOrCoolMode(m_BoxNr,record->ConfigPara  , &m_TempRange,G_NormalConfigPara[m_BoxNr].BoxType);

	//strcat(sendBuf,"动作温度:");
	sprintf(buf,"%5.1f - %5.1f,",m_TempRange.open_temp_min,m_TempRange.open_temp_max);
	strcat(sendBuf,buf);
	
	//strcat(sendBuf,"动作温度:");
	sprintf(buf,"%5.1f - %5.1f,",m_TempRange.open_temp_min,m_TempRange.open_temp_max);
	strcat(sendBuf,buf);
	
	//strcat(sendBuf,"复位温度:");
	sprintf(buf,"%5.1f - %5.1f,",m_TempRange.close_temp_min,m_TempRange.close_temp_max);
	strcat(sendBuf,buf);
	


	strcat(sendBuf,"}\r\n");
	send(sock, sendBuf, strlen(sendBuf), 0);

    AfxMessageBox(L"MES 发送成功");

    closesocket(sock);
    WSACleanup();
}

void CWorkMode::SaveTestRecord(int BoxNr,CString barcode)
{
	 
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,m_SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

#ifndef WINCE
	CFile RecFile;
	CString fName,s;
	RECORD	record;
	CTime cur;
	CTimeSpan span;
	cur = cur.GetCurrentTime();

	memset(&record,0,sizeof(record));

	span				= m_BeginTestTime - STD_TIME; 
	record.StartTime   	= span.GetTotalMinutes();

	span				= m_EndTestTime - STD_TIME; 
	record.EndTime  	= span.GetTotalMinutes();
	record.SwitchType	= G_NormalConfigPara[BoxNr].SwitchType;
	record.ConfigPara	= m_SwitchConfigPara;
	//record.ConfigParaEx = m_SwitchConfigPara.para_ex ;

	for(int i = 0; i < G_iMaxLedNr[BoxNr]; i++)
	{
		record.TestResult[i] = G_SwitchTestResult[BoxNr][i];  
	}
//	memset(record.ParaName,0,sizeof(record.ParaName));
//	for(int i = 0; i < sizeof(record.ParaName)/sizeof(TCHAR),i < m_ParaName.GetLength() ; i++)
//	{
//		record.ParaName[i] = m_ParaName.GetAt(i); 
//	}



	record.size = sizeof(record);
	

	//for(int group = 0; group < MAX_GROUPS; group++)
	{
		
	
		//HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(m_SwitchConfigPara.para  , &m_TempRange,G_NormalConfigPara[m_BoxNr].BoxType);

		HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,m_SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

		G_MainDlg->m_LPM260_Printer.PrintTestResult(BoxNr,&record,0,0,m_TempRange,::G_NormalConfigPara[BoxNr].BoxType,0); 
	}

	//保存生产记录

	
	memcpy(record.Memo,G_NormalConfigPara[BoxNr].TestMemo,sizeof( record.Memo));

	memcpy(record.Tester,G_NormalConfigPara[BoxNr].Tester,sizeof( record.Tester));



	CFile f;
	CString file_name;

	CString sDir;
	
	sDir = CreateCurDateDirection(BoxNr);

	

	//file_name.Format(_T("\\%04d%02d%02d%02d%02d%02d-%04d%02d%02d%02d%02d%02d.record"),
	//	m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth(),m_BeginTestTime.GetDay(),m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(),
	//	m_EndTestTime.GetYear(),m_EndTestTime.GetMonth(),m_EndTestTime.GetDay(),m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());
	//file_name = record.ParaName;
	//if(file_name.GetLength() > 20) 
	//{
	//	file_name = file_name.Left(20);
	//}

	s.Format(_T("\\%02d时%02d分%02d秒  "), m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond());   
	file_name =	s +file_name +_T(".record"); 

	if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		
		f.SeekToBegin();
		f.Write(&record,sizeof(record)); 
		f.Close(); 
	}

	file_name.Replace(_T(".record"),_T(".cur")); 

	if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		
		f.SeekToBegin();
		 
		f.Write(::G_TempArray[BoxNr].GetData(),sizeof(TEMP_RECORD)*G_TempArray[BoxNr].GetSize()); 
		f.Close(); 
	}
	
	///////////////////////////////
	

	file_name.Format(_T("record%d.bin"),BoxNr);
	if(f.Open(theAppDirectory+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		
		f.SeekToBegin();
		//f.Write(&G_SwitchTestResult[m_BoxNr],sizeof(G_SwitchTestResult[m_BoxNr])); 
		f.Write(&record,sizeof(record)); 
		f.Close(); 
	}


	
#else
	CFile RecFile;
	CString fName;
	


	CTimeSpan span;

	CFile f;
	CString file_name;

	CString sDir;
	sDir.Format(_T("HardDisk\\log\\")) ;
	::CreateDirectory(sDir,NULL);

	{
		CString msg[] = 
		{
			_T("HardDisk\\log\\%04d年\\"),
			_T("HardDisk\\log\\%04d\\"),
		};
		sDir.Format(msg[::G_NormalConfigPara[BoxNr].Language],m_BeginTestTime.GetYear()) ;
		::CreateDirectory(sDir,NULL); 
	}
	{
		CString msg[] = 
		{
			_T("HardDisk\\log\\%04d年\\%02d月\\"),
			_T("HardDisk\\log\\%04d\\%02d\\"),
		};
		sDir.Format(msg[::G_NormalConfigPara[BoxNr].Language],m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth()) ;
		::CreateDirectory(sDir,NULL); 
	}
	{
		CString msg[] = 
		{
			_T("HardDisk\\log\\%04d年\\%02d月\\%02d日\\"),
			_T("HardDisk\\log\\%04d\\%02d\\%02d\\"),
		};
		sDir.Format(msg[::G_NormalConfigPara[BoxNr].Language],m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth(),m_BeginTestTime.GetDay()) ;
		::CreateDirectory(sDir,NULL); 
	}
	{
		CString msg[] = 
		{
			_T("%02d时%02d分%02d秒 - %02d时%02d分%02d秒.record"),
			_T("%02dH%02dM%02dS - %02dH%02dM%02dS.record")
		};
		file_name.Format(msg[::G_NormalConfigPara[BoxNr].Language],
			m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(),
			m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());
	}

	switch(G_NormalConfigPara[BoxNr].BoxType)
	{
#ifdef WINCE
	case BOX_YangZhouYingDi:
		{
			RECORD_YANGZHOU_YINGDI	record_YangZhouYingDi;
			memset(&record_YangZhouYingDi,0,sizeof(record_YangZhouYingDi));
			span = m_BeginTestTime  - STD_TIME; 
			record_YangZhouYingDi.StartTime = span.GetTotalMinutes();

			span = m_EndTestTime   - STD_TIME; 
			record_YangZhouYingDi.EndTime  = span.GetTotalMinutes();


			record_YangZhouYingDi.m_SwitchType		= ::G_NormalConfigPara[BoxNr].SwitchType;
			record_YangZhouYingDi.ConfigPara		= m_SwitchConfigPara;
			record_YangZhouYingDi.temp_range		= m_TempRange; 
			record_YangZhouYingDi.m_HeatCoolOrder	= HeatCool;

			memcpy(	record_YangZhouYingDi.TestResult, G_SwitchTestResult,  sizeof(record_YangZhouYingDi.TestResult));

			memcpy( record_YangZhouYingDi.Barcode, m_Barcode,sizeof(record_YangZhouYingDi.Barcode));  
			/*
			for(int group = 0; group < MAX_GROUPS; group++)
			{
				main->m_EH32_Printer.PrintTestResult(record_128,m_Group,group); 
			}
			*/
			
			if(f.Open(_T("HardDisk\\record.bin"), CFile::modeReadWrite | CFile::modeCreate))
			{
				f.SeekToBegin();
				f.Write(&record_YangZhouYingDi,sizeof(record_YangZhouYingDi)); 
				f.Close(); 
			}

			if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
			{
		
				f.SeekToBegin();
				f.Write(&record_YangZhouYingDi,sizeof(record_YangZhouYingDi)); 
				f.Close(); 
			}

			memset(m_Barcode,0,sizeof(m_Barcode));

			if(G_NormalConfigPara[BoxNr].SaveCurve)
			{
				CString msg[] = 
				{
					_T("%02d时%02d分%02d秒 - %02d时%02d分%02d秒.curve"),
					_T("%02dH%02dM%02dS - %02dH%02dM%02dS.curve")
				};
				file_name.Format(msg[::G_NormalConfigPara[BoxNr].Language] ,
					m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(),
					m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());
				
				if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
				{
					
					f.SeekToBegin();
					for(int i = 0; i < G_TempArray[BoxNr].GetSize(); i++)
					{ 
						TEMP_RECORD temp = G_TempArray[BoxNr].GetAt(i);
						f.Write(&temp  ,sizeof(TEMP_RECORD)); 
					}
					f.Close(); 
				}
			}
		}
		break;
#endif
	case BOX_192:
		{
			/*
			RECORD_192	record_192;
			memset(&record_192,0,sizeof(record_192));
			span = this->m_BeginTestTime  - STD_TIME; 
			record_192.StartTime = span.GetTotalMinutes();

			span = this->m_EndTestTime   - STD_TIME; 
			record_192.EndTime  = span.GetTotalMinutes();


			record_192.m_SwitchType		= m_SwitchType;
			record_192.ConfigPara		= m_SwitchConfigPara;
			record_192.temp_range		= m_TempRange; 
			record_192.m_HeatCoolOrder	= m_HeatCoolOrder;

			memcpy(	record_192.TestResult, G_SwitchTestResult,  sizeof(record_192.TestResult));
			
			//for(int group = 0; group < MAX_GROUPS; group++)
			//{
			//	main->m_EH32_Printer.PrintTestResult(record_192,m_Group,group); 
			//}
			
			
			if(f.Open(_T("HardDisk\\record.bin"), CFile::modeReadWrite | CFile::modeCreate))
			{
				f.SeekToBegin();
				f.Write(&record_192,sizeof(record_192)); 
				f.Close(); 
			}

			if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
			{
				
				f.SeekToBegin();
				f.Write(&record_192,sizeof(record_192)); 
				f.Close(); 
			}

			if(G_NormalConfigPara[BoxNr].SaveCurve)
			{
				CString msg[] = 
				{
					_T("%02d时%02d分%02d秒 - %02d时%02d分%02d秒.curve"),
					_T("%02dH%02dM%02dS - %02dH%02dM%02dS.curve")
				};
				file_name.Format(msg[::G_NormalConfigPara[BoxNr].Language] ,
					m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(),
					m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());
				
				if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
				{
					
					f.SeekToBegin();
					for(int i = 0; i < G_CurveDlg.m_TempArray.GetSize(); i++)
					{
						TEMP_RECORD temp = G_CurveDlg.m_TempArray.GetAt(i);
						f.Write(&temp  ,sizeof(TEMP_RECORD)); 
					}
					f.Close(); 
				}
			}
			*/
		}
		break;
	case BOX_128:
	case HEAT_BOX_TYPE_PTC:
	case BOX_128_PTC_NOT_TESTED:
	case HEAT_BOX_TYPE_ANTI_PTC:

	default:
		{

			RECORD						record_128;
			memset(&record_128,0,sizeof(record_128));
			span						= m_BeginTestTime  - STD_TIME; 

			

			record_128.StartTime		= span.GetTotalMinutes();

			span						= m_EndTestTime   - STD_TIME; 
			record_128.EndTime			= span.GetTotalMinutes();

			record_128.SwitchType		= ::G_NormalConfigPara[BoxNr].SwitchType ;
			record_128.ConfigPara		= m_SwitchConfigPara;
			record_128.HeatSpeed		=::G_NormalConfigPara[0].HeatSpeed / 10.0f;
			record_128.CoolSpeed		=::G_NormalConfigPara[0].CoolSpeed / 10.0f;

			memcpy(	record_128.TestResult, G_SwitchTestResult,  sizeof(record_128.TestResult));
			
			if(::G_NormalConfigPara[0].PrinterType == 1)
			{
				G_MainDlg->m_PM24_Printer.PrintOneLine("");
				G_MainDlg->m_PM24_Printer.PrintOneLine("");
				G_MainDlg->m_PM24_Printer.PrintOneLine("");
				for(int group = 0; group < MAX_GROUPS; group++)
				{

					G_MainDlg->m_PM24_Printer.PrintTestResult(BoxNr,&record_128,G_Group[BoxNr],group,m_TempRange,::G_NormalConfigPara[BoxNr].BoxType, ::G_NormalConfigPara[BoxNr].Language); 
				}

				G_MainDlg->m_PM24_Printer.PrintOneLine("");
				G_MainDlg->m_PM24_Printer.PrintOneLine("");
				G_MainDlg->m_PM24_Printer.PrintOneLine("");
			}
			else
			{
				G_MainDlg->m_LPM260_Printer.PrintOneLine("");
				G_MainDlg->m_LPM260_Printer.PrintOneLine("");
				G_MainDlg->m_LPM260_Printer.PrintOneLine("");
				for(int group = 0; group < MAX_GROUPS; group++)
				{

					G_MainDlg->m_LPM260_Printer.PrintTestResult(BoxNr,&record_128,G_Group[BoxNr],group,m_TempRange,::G_NormalConfigPara[BoxNr].BoxType, ::G_NormalConfigPara[BoxNr].Language); 
				}

				G_MainDlg->m_LPM260_Printer.PrintOneLine("");
				G_MainDlg->m_LPM260_Printer.PrintOneLine("");
				G_MainDlg->m_LPM260_Printer.PrintOneLine("");
			}

			

			if(f.Open(_T("HardDisk\\record.bin"), CFile::modeReadWrite | CFile::modeCreate))
			{
				f.SeekToBegin();
				f.Write(&record_128,sizeof(record_128)); 
				f.Close(); 
			}

			if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
			{
				
				f.SeekToBegin();
				f.Write(&record_128,sizeof(record_128)); 
				f.Close(); 
			}

			if(G_NormalConfigPara[BoxNr].BoxType == BOX_KSD302_TIME 
				|| G_NormalConfigPara[BoxNr].BoxType == BOX_KSD302_TEMP)
			{

				SaveRecord2ExcelCSV_KSD302_CE(record_128,0,_T(""));
				SaveRecord2ExcelCSV_KSD302_CE(record_128,0,_T("Harddisk\\a.csv"));

				m_BeginTestTime = STD_TIME + CTimeSpan(0,0,record_128.StartTime,0);

				SendExcelCSV_2_PC(_T("Harddisk\\a.csv"),
						m_BeginTestTime.GetYear(),
						m_BeginTestTime.GetMonth(),
						m_BeginTestTime.GetDay(),
						m_BeginTestTime.GetHour(),
						m_BeginTestTime.GetMinute(),
						m_BeginTestTime.GetSecond());
			}
			else
			{
				SaveRecord2ExcelCSV_CE(record_128,0,_T(""));
			}
			
			
			if(G_NormalConfigPara[BoxNr].SaveCurve)
			{
				CString msg[] = 
				{
					_T("%02d时%02d分%02d秒 - %02d时%02d分%02d秒.curve"),
					_T("%02dH%02dM%02dS - %02dH%02dM%02dS.curve")
				};
				file_name.Format(msg[::G_NormalConfigPara[BoxNr].Language] ,
					m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(),
					m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());
#ifdef WINCE				
				if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
				{
					
					f.SeekToBegin();
					for(int i = 0; i < G_TempArray[BoxNr].GetSize(); i++)
					{
						TEMP_RECORD temp = G_TempArray[BoxNr].GetAt(i);
						f.Write(&temp  ,sizeof(TEMP_RECORD)); 
					}
					f.Close(); 
				}
#endif
			}
			
#ifdef WINCE
			if(::G_NormalConfigPara[BoxNr].BoxType == BOX_NTC) 
			{
				CString msg[] = 
				{
					_T("%02d时%02d分%02d秒 - %02d时%02d分%02d秒.NTC"),
					_T("%02dH%02dM%02dS - %02dH%02dM%02dS.NTC")
				};
				file_name.Format(msg[::G_NormalConfigPara[BoxNr].Language] ,
					m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(),
					m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());
				
				if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
				{
					
					f.SeekToBegin();
					for(int i = 0; i < G_CurveDlg.m_NtcRecordArray.GetSize(); i++)
					{
						NTC_RECORD temp = G_CurveDlg.m_NtcRecordArray.GetAt(i);
						f.Write(&temp  ,sizeof(NTC_RECORD)); 
					}
					f.Close(); 
				}
			}
#endif

#ifdef WINCE			
			if(::G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40 
				|| G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_FLAT 
				|| BOX_DISC_16 == G_NormalConfigPara[BoxNr].BoxType
				|| BOX_DISC_24 == G_NormalConfigPara[BoxNr].BoxType) 
			{
				CString msg[] = 
				{
					_T("%02d时%02d分%02d秒 - %02d时%02d分%02d秒.BDM"),
					_T("%02dH%02dM%02dS - %02dH%02dM%02dS.BDM")
				};
				file_name.Format(msg[::G_NormalConfigPara[BoxNr].Language] ,
					m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(),
					m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());
				
				if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
				{
					
					f.SeekToBegin();
					for(int k = 0; k < 40; k++)
					{
						for(int i = 0; i < G_BiMetalRecordArray[BoxNr][k].GetSize(); i++)
						{
							DISC_RECORD temp = G_BiMetalRecordArray[BoxNr][k].GetAt(i);

							f.Write(&temp  ,sizeof(DISC_RECORD)); 
						}
					}
					f.Close(); 
				}

				
			}
#endif
		}
		break;
	}
#endif

#ifndef WINCE
	

#ifdef _TopBand_Vietnam
	CTopbandVietnamMesDlg::MES_Collection(BoxNr,&record,0,127,barcode);
	
#elif _DONGGUAN_BETTER
	Send_Better_MES(&record);
#else
	Send_MES(record,false);
#endif

#endif

}


bool IsOtherBiMetalNoise(unsigned char id,unsigned char Noise[20])
{

	return false;

	if(Noise[id] == 0)
	{
		return false;
	}
	int iNoise[20];
	for(int i = 0; i < 20; i++)
	{
			iNoise[i] = Noise[i];
	}
//相差多少算干扰
#define DELTA_NOISE		30

	int group = (id / 5);
	int mod   = (id % 5);
	switch(mod)
	{
	case 0:
		if(iNoise[0 + group*5] < (iNoise[1 + group*5] - DELTA_NOISE)
			|| iNoise[0 + group*5] < (iNoise[2 + group*5] - DELTA_NOISE)
			|| iNoise[0 + group*5] < (iNoise[3 + group*5] - DELTA_NOISE)
			|| iNoise[0 + group*5] < (iNoise[4 + group*5] - DELTA_NOISE))
			{
				return true;
			}
		break;
	case 1:
		if(iNoise[1 + group*5] < (iNoise[0 + group*5]  - DELTA_NOISE)
			|| iNoise[1 + group*5] < (iNoise[2 + group*5] - DELTA_NOISE)
			|| iNoise[1 + group*5] < (iNoise[3 + group*5] - DELTA_NOISE)
			|| iNoise[1 + group*5] < (iNoise[4 + group*5] - DELTA_NOISE))
			{
				return true;
			}
		break;
	case 2:
		if(iNoise[2 + group*5] < (iNoise[0 + group*5] - DELTA_NOISE) 
			|| iNoise[2 + group*5] < (iNoise[1 + group*5] - DELTA_NOISE)
			|| iNoise[2 + group*5] < (iNoise[3 + group*5] - DELTA_NOISE)
			|| iNoise[2 + group*5] < (iNoise[4 + group*5] - DELTA_NOISE))
			{
				return true;
			}
			break;
	case 3:
		if(iNoise[3 + group*5] < (iNoise[0 + group*5] - DELTA_NOISE) 
			|| iNoise[3 + group*5] < (iNoise[1 + group*5] - DELTA_NOISE)
			|| iNoise[3 + group*5] < (iNoise[2 + group*5] - DELTA_NOISE)
			|| iNoise[3 + group*5] < (iNoise[4 + group*5] - DELTA_NOISE))
			{
				return true;
			}
			break;
	case 4:
		if(iNoise[4 + group*5] < (iNoise[0 + group*5]  - DELTA_NOISE)
			|| iNoise[4 + group*5] < (iNoise[1 + group*5] - DELTA_NOISE)
			|| iNoise[4 + group*5] < (iNoise[2 + group*5] - DELTA_NOISE)
			|| iNoise[4 + group*5] < (iNoise[3 + group*5] - DELTA_NOISE))
			{
				return true;
			}
			break;


	}
	return false;
}

//void CWorkMode::DealStatus_Disc(SUB_MODE SubMode,MODBUS_RS485_DISC_ANSWER *answer_cmd,float PidTemp,CONTROL_TEMP_RANGE	m_TempRange)
void CWorkMode::DealStatus_DiscOpenTemp(int BoxNr,DISC_RECORD *BiMetalRecord,unsigned char SoundVal,unsigned char MaxSoundVal,int SameCounts,int DiscID,float PidTemp,CONTROL_TEMP_RANGE	m_TempRange)
{
	SYSTEMTIME st;
	::GetLocalTime(&st); 
	bool SameSound2Continue = false;

	if(SameCounts <= 10)
	{
		BiMetalRecord->st				=	st; 
		BiMetalRecord->temp				=	PidTemp; 
		BiMetalRecord->SoundCounts[0]	=   SoundVal;	
		BiMetalRecord->id				=   DiscID;
		BiMetalRecord->IsActMode		=	1;
		BiMetalRecord->SameCounts		=	SameCounts;
		//前后2次的时间在1秒钟以内，就认为是连续的声音被听了2次
		////最后2条合并，并且移除最后一条

		if(G_BiMetalRecordArray[BoxNr][DiscID].GetSize() > 0)
		{
			//AfxMessageBox(_T("size  > 0"));
			int size = G_BiMetalRecordArray[BoxNr][DiscID].GetSize();
			DISC_RECORD dr0 = G_BiMetalRecordArray[BoxNr][DiscID].GetAt(size-1);
			
			CTime ct1(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
			CTime ct0(dr0.st.wYear,dr0.st.wMonth,dr0.st.wDay,dr0.st.wHour,dr0.st.wMinute,dr0.st.wSecond);
			CTimeSpan sp = ct1 - ct0;
				
			if(sp.GetTotalSeconds() <= 1)
			{
				//CString s;
				//s.Format(_T("SameSound2Continue = true %d"),G_BiMetalRecordArray[BoxNr][DiscID].GetSize());
				//AfxMessageBox(s);
				SameSound2Continue = true;
				SoundVal += dr0.SoundCounts[0];
				{
					char buf[100];
					sprintf(buf,"C %02d, %d ,%d,",DiscID+1,dr0.SoundCounts[0],BiMetalRecord->SoundCounts[0]);
					//CLPM260_Printer::PrintOneLine(buf);
				}
				BiMetalRecord->SoundCounts[0] += dr0.SoundCounts[0];
				G_BiMetalRecordArray[BoxNr][DiscID].RemoveAt(size-1);
				G_BiMetalRecordArray[BoxNr][DiscID].Add(*BiMetalRecord);
			}
			else
			{
				//CString s;
				//s.Format(_T("SameSound2Continue = false %d"),G_BiMetalRecordArray[BoxNr][DiscID].GetSize());
				//AfxMessageBox(s);
				G_BiMetalRecordArray[BoxNr][DiscID].Add(*BiMetalRecord); 
			}
		}
		
		else
		{
			//AfxMessageBox(_T("Size = 0"));
			G_BiMetalRecordArray[BoxNr][DiscID].Add(*BiMetalRecord); 
		}
	}

	
	//if(IsOtherBiMetalNoise(i,answer_cmd->para) == false)
	{
		
		if(G_SwitchTestResult[BoxNr][DiscID].IsOpenned == false)
		{
			/*if(PidTemp < (m_TempRange.open_temp_min - 10.0f))
			{//动作温度太低，忽略
			}
			else*/
			{
				if(SameCounts == 1)
				{
					G_SwitchTestResult[BoxNr][DiscID].IsOpenned			= true;
					G_SwitchTestResult[BoxNr][DiscID].OpenTemp			= PidTemp  + G_AdjustTemp[BoxNr][0][DiscID];
					G_SwitchTestResult[BoxNr][DiscID].OpenSoundStrong	= SoundVal;
				}
				else
				{
					G_SwitchTestResult[BoxNr][DiscID].IsOpenned			= true;
					G_SwitchTestResult[BoxNr][DiscID].OpenTemp			= PidTemp  + G_AdjustTemp[BoxNr][0][DiscID];
					G_SwitchTestResult[BoxNr][DiscID].OpenSoundStrong	= SoundVal;
				}

			}
		}
		else
		{
			if(G_SwitchTestResult[BoxNr][DiscID].OpenSoundStrong > 0)
			{
				//if(CheckSameSound2Continue(BoxNr,DiscID))
				if(SameSound2Continue)
				{//同一个声音连续听了2次,忽略
					
				}
				else
				{
					G_SwitchTestResult[BoxNr][DiscID].IsOpenFlash = true;
				}
			}
			//声音大的优先
			//相等时，判断 SameCounts
			if(G_SwitchTestResult[BoxNr][DiscID].OpenSoundStrong < SoundVal)
			{//取声音最大的为准

				G_SwitchTestResult[BoxNr][DiscID].IsOpenned			= true;
				G_SwitchTestResult[BoxNr][DiscID].OpenTemp			= PidTemp  + G_AdjustTemp[BoxNr][0][DiscID];
				G_SwitchTestResult[BoxNr][DiscID].OpenSoundStrong	= SoundVal;

			}
			else if(G_SwitchTestResult[BoxNr][DiscID].OpenSoundStrong == SoundVal)
			{
				if(SameCounts == 1)
				{
					G_SwitchTestResult[BoxNr][DiscID].IsOpenned			= true;
					G_SwitchTestResult[BoxNr][DiscID].OpenTemp			= PidTemp  + G_AdjustTemp[BoxNr][0][DiscID];
					G_SwitchTestResult[BoxNr][DiscID].OpenSoundStrong	= SoundVal;
				}
			}

		}
	}
}

bool CWorkMode::CheckSameSound2Continue(int BoxNr,int DiscID)
{
	//同一个声音连续听了2次

	int size = G_BiMetalRecordArray[BoxNr][DiscID].GetSize();
	if(size < 2)
	{
		return false;
	}

					
						
	DISC_RECORD dr1 = G_BiMetalRecordArray[BoxNr][DiscID].GetAt(size-2);
	DISC_RECORD dr0 = G_BiMetalRecordArray[BoxNr][DiscID].GetAt(size-1);
	
	CTime ct1(dr1.st.wYear,dr1.st.wMonth,dr1.st.wDay,dr1.st.wHour,dr1.st.wMinute,dr1.st.wSecond);
	CTime ct0(dr0.st.wYear,dr0.st.wMonth,dr0.st.wDay,dr0.st.wHour,dr0.st.wMinute,dr0.st.wSecond);
	CTimeSpan sp = ct0 - ct1;
		
	if(sp.GetTotalSeconds() <= 1)
	{
		char buf[100];
		sprintf(buf,"C %02d, %d ,%d,",DiscID+1,dr0.SoundCounts[0],dr1.SoundCounts[0]);
		//CLPM260_Printer::PrintOneLine(buf);

		//最后2条合并，并且移除最后一条
		dr1.SoundCounts[0] += dr0.SoundCounts[0];
		G_BiMetalRecordArray[BoxNr][DiscID].RemoveAt(size-1);
		G_BiMetalRecordArray[BoxNr][DiscID].RemoveAt(size-2);
		G_BiMetalRecordArray[BoxNr][DiscID].Add(dr1);
		return true;
	}

	return false;

}

void CWorkMode::DealStatus_DiscCloseTemp(int BoxNr,DISC_RECORD *BiMetalRecord,unsigned char SoundVal,unsigned char MaxSoundVal,int SameCounts,int DiscID,float PidTemp,CONTROL_TEMP_RANGE	m_TempRange)
{
		SYSTEMTIME st;
		::GetLocalTime(&st); 
		bool SameSound2Continue = false;

		BiMetalRecord->st				=	st;
		BiMetalRecord->temp				=	PidTemp; 
		BiMetalRecord->SoundCounts[0]	=	SoundVal;
		BiMetalRecord->id				=   DiscID;
		BiMetalRecord->IsActMode		=	0;
		BiMetalRecord->SameCounts		=	SameCounts;

		if(G_BiMetalRecordArray[BoxNr][DiscID].GetSize() > 0)
		{
			int size = G_BiMetalRecordArray[BoxNr][DiscID].GetSize();
			DISC_RECORD dr0 = G_BiMetalRecordArray[BoxNr][DiscID].GetAt(size-1);
			
			CTime ct1(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
			CTime ct0(dr0.st.wYear,dr0.st.wMonth,dr0.st.wDay,dr0.st.wHour,dr0.st.wMinute,dr0.st.wSecond);
			CTimeSpan sp = ct1 - ct0;
				
			if(sp.GetTotalSeconds() <= 1)
			{
				SameSound2Continue = true;
				
				{
					char buf[100];
					sprintf(buf,"C %02d, %d ,%d,",DiscID+1,dr0.SoundCounts[0],SoundVal);
					//CLPM260_Printer::PrintOneLine(buf);
				}

				SoundVal += dr0.SoundCounts[0];
				BiMetalRecord->SoundCounts[0] = SoundVal;
				G_BiMetalRecordArray[BoxNr][DiscID].RemoveAt(size-1);
				G_BiMetalRecordArray[BoxNr][DiscID].Add(*BiMetalRecord);
			}
			else
			{
				G_BiMetalRecordArray[BoxNr][DiscID].Add(*BiMetalRecord); 
			}
		}
		
		else
		{
			G_BiMetalRecordArray[BoxNr][DiscID].Add(*BiMetalRecord); 
		}
		

		if(SoundVal < 100)
		{
			//return;
		}

		//if((SameCounts <=  1) || (SameCounts >= 2 && CurMaxID==i))
//		if(IsOtherBiMetalNoise(i,answer_cmd->para) == false)
		{
			//char buf[40];
			//sprintf(buf,"%d = %5.1f",POS_A[i] + 1, m_PidTemp);
			//this->m_LPM260_Printer->Print(buf);  

			if(G_SwitchTestResult[BoxNr][DiscID].IsClosed == false)
			{
				if(G_SwitchTestResult[BoxNr][DiscID].IsOpenned == true)
				{
					if(/*PidTemp > (m_TempRange.close_temp_max + 10.0f) ||*/ PidTemp > (G_SwitchTestResult[BoxNr][DiscID].OpenTemp - 3.0f))
					{//复位温度太高，忽略
					}
					else
					{
						G_SwitchTestResult[BoxNr][DiscID].IsClosed			= true;
						G_SwitchTestResult[BoxNr][DiscID].CloseTemp			= PidTemp   + G_AdjustTemp[BoxNr][1][DiscID];
						G_SwitchTestResult[BoxNr][DiscID].CloseSoundStrong	= SoundVal;
					}
				}
				else
				{
					if(PidTemp > (m_TempRange.close_temp_max + 10.0f) )
					{//复位温度太高，忽略
					}
					else
					{
						
						G_SwitchTestResult[BoxNr][DiscID].IsClosed			= true;
						G_SwitchTestResult[BoxNr][DiscID].CloseTemp			= PidTemp   + G_AdjustTemp[BoxNr][1][DiscID];
						G_SwitchTestResult[BoxNr][DiscID].CloseSoundStrong	= SoundVal;
					}
				}
			}
			else
			{
				if(G_SwitchTestResult[BoxNr][DiscID].CloseSoundStrong > 0)
				{
					int ActCount = 0;
					for(int i = 0; i < G_BiMetalRecordArray[BoxNr][DiscID].GetSize(); i++)
					{
						if(G_BiMetalRecordArray[BoxNr][DiscID].GetAt(i).IsActMode == 1)
						{
							ActCount ++;
						}
					}
					
					//if(CheckSameSound2Continue(BoxNr,DiscID))
					if(SameSound2Continue)
					{//同一个声音连续听了2次,忽略
					}
					else
					{
						G_SwitchTestResult[BoxNr][DiscID].IsCloseFlash = true;
					}


				}
				if(G_SwitchTestResult[BoxNr][DiscID].CloseSoundStrong < SoundVal)
				{//取声音最大的为准
					G_SwitchTestResult[BoxNr][DiscID].IsClosed			= true;
					G_SwitchTestResult[BoxNr][DiscID].CloseTemp			= PidTemp		  + G_AdjustTemp[BoxNr][1][DiscID];
					G_SwitchTestResult[BoxNr][DiscID].CloseSoundStrong	= SoundVal;
				}
			}
		}
}

//#define SOUND_COUNTS	G_NormalConfigPara[0].SoundLevel_Time

void CWorkMode::DealStatus_Disc(int BoxNr,MODBUS_RS485_DISC_ANSWER *answer_cmd,float PidTemp)
{

	CONTROL_TEMP_RANGE	m_TempRange;
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,m_SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

//	int BoxNr,int SwitchTestID,SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status,float m_PidTemp,
//												bool m_DoorIsOpenned,int TestMode);

	static DISC_RECORD BiMetalRecord;
	int i;

	unsigned char RS485_Addr = answer_cmd->head.RS485_Addr - 'A'; 


/*
				A					B				
实际编号 1	2	3	4	5	|| 6	7	8	9	10
通讯编号 1	3	5	7	9	|| 21  23  25  27  29

实际编号 11	12	13	14	15	|| 16	17	18	19	20
通讯编号 11	13	15	17	19	|| 31  33  35  37  39

实际编号 21	22	23	24	25	|| 26	27	28	29	30
通讯编号 0	2	4	6	8	|| 20	22	24	26	28

实际编号 31	32	33	34	35	|| 36	37	38	39	40
通讯编号 10	12	14	16	18	|| 30	32	34	36	38
*/

	int MaxSound = 20;
	int CurMaxID = -1;
	int POS_A[24] = //{	 0, 1, 2, 3 ,4,
					//	10,11,12,13,14,
					//	20,21,22,23,24,
					//	30,31,32,33,34,
					//	0,0,0,0};
						{	 4,3,2,1,0,
						14,13,12,11,10,
						24,23,22,21,20,
						34,33,32,31,30,
						0,0,0,0};

	//int POS_DESK_DISC[20] ={9,7,5,3,1,19,17,15,13,11,8,6,4,2,0,18,16,14,12,10};
		//{14,4,13,3,12,2,11,1,10,0
		// 19,9,18,8,17,7,16,6,15,5};
	

	if(BOX_DISC_24 == G_NormalConfigPara[BoxNr].BoxType)
	{
		MaxSound = 24;
		for(int i = 0; i < MaxSound; i++)
		{
			 POS_A[i] = i;
		}
	}
	else if(BOX_DISC_16 == G_NormalConfigPara[BoxNr].BoxType)
	{
		MaxSound = 16;
		for(int i = 0; i < MaxSound; i++)
		{
			 POS_A[i] = i;
		}
	}
	//G_NormalConfigPara[BoxNr].SoundOutputType ==	0	正常测试,显示温度 
	//											1	显示声音值
	//											2	不开风扇噪音自检
	//											3	开风扇噪音自检

	if(G_NormalConfigPara[BoxNr].SoundOutputType == 3 || G_NormalConfigPara[BoxNr].SoundOutputType == 2 || G_NormalConfigPara[BoxNr].SoundOutputType == 1 )
	{//噪音自检

		

		if(RS485_Addr == 0)
		{
			
			{

				for(i = 0; i < MaxSound; i ++)
				{
					G_Noise.CurCounts[POS_A[i]] = answer_cmd->para[i];	
					if(answer_cmd->para[i] > 0 && (G_Noise.SelfCheckTimeCount > 5 || G_NormalConfigPara[BoxNr].SoundOutputType == 1))
					{
						G_Noise.SomeCountsNoZero = 1;
						if(G_NormalConfigPara[BoxNr].SoundOutputType == 2 || G_NormalConfigPara[BoxNr].SoundOutputType == 1)
						{
							if(G_Noise.MaxCounts[POS_A[i]][0] < G_Noise.CurCounts[POS_A[i]] + 200 * G_NormalConfigPara[BoxNr].SoundLevel_Heat)
							{
								G_Noise.MaxCounts[POS_A[i]][0] = G_Noise.CurCounts[POS_A[i]] + 200 * G_NormalConfigPara[BoxNr].SoundLevel_Heat;
							}

							G_Noise.MaxLevel[POS_A[i]][0] = G_NormalConfigPara[BoxNr].SoundLevel_Heat;
	#ifndef WINCE						
							if(G_NormalConfigPara[BoxNr].SoundOutputType == 1)
							{
								if(::G_SpeechArray.GetSize() < 5) 
								{
									CString s;
									s.Format(_T("%d号噪音%d"),POS_A[i]+1,G_Noise.CurCounts[POS_A[i]]);
									::G_SpeechArray.Add (s);
								}
							}
	#endif
						
						}
						else if(G_NormalConfigPara[BoxNr].SoundOutputType == 3)
						{
							if(G_Noise.MaxCounts[POS_A[i]][1] < G_Noise.CurCounts[POS_A[i]] + 200 * G_NormalConfigPara[BoxNr].SoundLevel_Cool)
							{
								G_Noise.MaxCounts[POS_A[i]][1] = G_Noise.CurCounts[POS_A[i]] + 200 * G_NormalConfigPara[BoxNr].SoundLevel_Cool;
							}

							G_Noise.MaxLevel[POS_A[i]][1] = G_NormalConfigPara[BoxNr].SoundLevel_Cool;
						}
						
					}

				}
			}
		}
		else if(RS485_Addr == 1)
		{
			for(i = 0; i < MaxSound; i ++)
			{
				G_Noise.CurCounts[POS_A[i] + 5] = answer_cmd->para[i];	
					
				if(answer_cmd->para[i] > 0 && (G_Noise.SelfCheckTimeCount > 5 || G_NormalConfigPara[BoxNr].SoundOutputType == 1))
				{
					G_Noise.SomeCountsNoZero = 1;
					if(G_NormalConfigPara[BoxNr].SoundOutputType == 2 || G_NormalConfigPara[BoxNr].SoundOutputType == 1)
					{
						if(G_Noise.MaxCounts[POS_A[i] + 5][0] < G_Noise.CurCounts[POS_A[i] + 5] + 200 * G_NormalConfigPara[BoxNr].SoundLevel_Heat )
						{
							G_Noise.MaxCounts[POS_A[i] + 5][0] = G_Noise.CurCounts[POS_A[i] + 5] + 200 * G_NormalConfigPara[BoxNr].SoundLevel_Heat;
						}
			
						G_Noise.MaxLevel[POS_A[i] + 5][0] = G_NormalConfigPara[BoxNr].SoundLevel_Heat;
					}
					else if(G_NormalConfigPara[BoxNr].SoundOutputType == 3)
					{
						if(G_Noise.MaxCounts[POS_A[i] + 5][1] < G_Noise.CurCounts[POS_A[i] + 5] + 200 * G_NormalConfigPara[BoxNr].SoundLevel_Cool )
						{
							G_Noise.MaxCounts[POS_A[i] + 5][1] = G_Noise.CurCounts[POS_A[i] + 5] + 200 * G_NormalConfigPara[BoxNr].SoundLevel_Cool;
						}
			
						G_Noise.MaxLevel[POS_A[i] + 5][1] = G_NormalConfigPara[BoxNr].SoundLevel_Cool;
					}
				}
				
			}
			if(G_NormalConfigPara[BoxNr].SoundOutputType == 2 || G_NormalConfigPara[BoxNr].SoundOutputType == 3)
			{
				G_Noise.SelfCheckTimeCount ++;
#ifdef WINCE
				if(G_NormalConfigPara[BoxNr].SoundOutputType == 2)
				{
					if(G_DebugDlg.IsWindowVisible())
					{
						CString s;
						s.Format(_T("升温噪音等级:%d，次数=%d"),G_NormalConfigPara[BoxNr].SoundLevel_Heat,G_Noise.SelfCheckTimeCount);
						
						::G_DebugDlg.m_NoiseSelfCheckMsg = s;
						G_DebugDlg.UpdateData(false); 
						
					}
					if(::G_HeatBoxDlg.IsWindowVisible())
					{
						CString s;
						s.Format(_T("升温噪音等级:%d，测试次数=%d"),G_NormalConfigPara[BoxNr].SoundLevel_Heat,G_Noise.SelfCheckTimeCount);
						
						
						::G_HeatBoxDlg.m_TestMsg = s;
						G_HeatBoxDlg.UpdateData(false); 
					}
				}
				else
				{
					if(G_DebugDlg.IsWindowVisible())
					{
						CString s;
						s.Format(_T("降温噪音等级:%d，测试次数=%d"),G_NormalConfigPara[BoxNr].SoundLevel_Cool,G_Noise.SelfCheckTimeCount);
						::G_DebugDlg.m_NoiseSelfCheckMsg = s;
						G_DebugDlg.UpdateData(false); 
						
					}
					if(::G_HeatBoxDlg.IsWindowVisible())
					{
						CString s;
						s.Format(_T("降温噪音等级:%d，测试次数=%d"),G_NormalConfigPara[BoxNr].SoundLevel_Cool,G_Noise.SelfCheckTimeCount);
						
						::G_HeatBoxDlg.m_TestMsg = s;
						G_HeatBoxDlg.UpdateData(false); 
					}
				}
#endif
			}
			
		}

		if((G_Noise.SelfCheckTimeCount > 50) && (G_NormalConfigPara[BoxNr].SoundOutputType == 2 || G_NormalConfigPara[BoxNr].SoundOutputType == 3))
		{
			if(G_Noise.SomeCountsNoZero == 0 && G_NormalConfigPara[BoxNr].SoundOutputType == 2)
			{
				G_NormalConfigPara[BoxNr].SoundOutputType = 3;
				G_Noise.SelfCheckTimeCount			  = 0;
#ifdef WINCE
				G_RS485_Comm.SendOpenDoorCmd(true,::G_NormalConfigPara[BoxNr].BoxNr);
#else
				G_RS485_Comm.SendOpenDoorCmd(true,0);
#endif
			}
			else if(G_Noise.SomeCountsNoZero == 0 && G_NormalConfigPara[BoxNr].SoundOutputType == 3)
			{
				G_NormalConfigPara[BoxNr].SoundOutputType = 0;
#ifdef WINCE
				G_RS485_Comm.SendOpenDoorCmd(false,::G_NormalConfigPara[BoxNr].BoxNr);
#else
				G_RS485_Comm.SendOpenDoorCmd(false,0);
#endif
				
				SaveNormalConfigPara();

				G_MainDlg->m_LPM260_Printer.PrintNoiseTest(); 

					
				CFile	f;
				::CreateDirectoryW( NOISE_FILE_DIR,NULL);

				if(f.Open(NOISE_FILE_NAME,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
				{
					f.SeekToBegin();
					f.Write(&G_Noise,sizeof(G_Noise));
					f.Close();
				}
			}
			else
			{
				G_Noise.SomeCountsNoZero	=	0;
				G_Noise.SelfCheckTimeCount	=	0;
				
				if(G_NormalConfigPara[BoxNr].SoundOutputType == 2)
				{
					G_NormalConfigPara[BoxNr].SoundLevel_Heat ++;
				}
				if(G_NormalConfigPara[BoxNr].SoundOutputType == 3)
				{
					G_NormalConfigPara[BoxNr].SoundLevel_Cool ++;
				}
			}
		}


	}



	//crc = G_RS485_Comm.CRC16((unsigned char *)answer_cmd,sizeof(MODBUS_RS485_DISC_ANSWER) - 2); 	
	
	//if(crc == answer_cmd->crc)
	{
	

		unsigned char CurSoundCount[24];
		unsigned char CurMaxSoundCount = 0;
		unsigned int  SameCounts = 0;

		if(BOX_DISC_24 == G_NormalConfigPara[BoxNr].BoxType)
		{
			for(i = 0; i < 24; i ++)
			{
				CurSoundCount[i] = answer_cmd->para[i];	

				if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
				{
					SameCounts ++;
					{
						char buf[100];
						sprintf(buf,"%02d %6.1f Sound=%d Same=%d,",i+1,PidTemp,CurSoundCount[i],SameCounts);
					//	CLPM260_Printer::PrintOneLine(buf);
					}
					
					if(CurSoundCount[i] > CurMaxSoundCount)
					{
						CurMaxSoundCount  = CurSoundCount[i];
						CurMaxID = i;
					}
					
				}
			}
			if(SameCounts > 0)
			{
				char buf[100];
				sprintf(buf,"A%02d %6.1f MaxSound=%d Same=%d,",CurMaxID+1,PidTemp,CurMaxSoundCount,SameCounts);
			//	CLPM260_Printer::PrintOneLine(buf);
			}
		}
		else if(BOX_DISC_16 == G_NormalConfigPara[BoxNr].BoxType)
		{
			for(i = 0; i < 16; i ++)
			{
				CurSoundCount[i] = answer_cmd->para[i];	

				if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
				{
					SameCounts ++;
					{
						char buf[100];
						sprintf(buf,"%02d %6.1f Sound=%d Same=%d,",i+1,PidTemp,CurSoundCount[i],SameCounts);
					//	CLPM260_Printer::PrintOneLine(buf);
					}
					
					if(CurSoundCount[i] > CurMaxSoundCount)
					{
						CurMaxSoundCount  = CurSoundCount[i];
						CurMaxID = i;
					}
					
				}
			}
			if(SameCounts > 0)
			{
				char buf[100];
				sprintf(buf,"B%02d %6.1f MaxSound=%d Same=%d,",CurMaxID+1,PidTemp,CurMaxSoundCount,SameCounts);
				//CLPM260_Printer::PrintOneLine(buf);
			}
		}
		else
		{
			for(i = 0; i < 20; i ++)
			{
				CurSoundCount[i] = answer_cmd->para[i];	

				if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
				{
					SameCounts ++;

					char buf[100];
					if(	RS485_Addr == 0)
					{
						sprintf(buf,"%02d %6.1f Sound=%d Same=%d,",i+1,PidTemp,CurSoundCount[i],SameCounts);
					}
					else
					{
						sprintf(buf,"%02d %6.1f Sound=%d Same=%d,",i+21,PidTemp,CurSoundCount[i],SameCounts);
					}
				
					if(CurSoundCount[i] > CurMaxSoundCount)
					{
						CurMaxSoundCount  = CurSoundCount[i];
						CurMaxID = i;
					}
				}
			}
		}

		if(SameCounts == 0 )
		{
			return;
		}

		
		if(G_NormalConfigPara[BoxNr].SoundOutputType == 0 && SameCounts >= 2)
		{
			SYSTEMTIME st;
			::GetLocalTime(&st); 
			if(BOX_DISC_24 == G_NormalConfigPara[BoxNr].BoxType)
			{
				for(int i = 0; i < 24; i++)
				{
					if(answer_cmd->para[i] > 0)
					{
						BiMetalRecord.st				=	st; 
						BiMetalRecord.temp				=	PidTemp; 
						BiMetalRecord.SoundCounts[0]	=   answer_cmd->para[i];	
						BiMetalRecord.id				=   i;
						if(this->GetOpenMode())
						{
							BiMetalRecord.IsActMode		=	1;
						}
						else
						{
							BiMetalRecord.IsActMode		=	0;
						}
						BiMetalRecord.SameCounts		=	SameCounts;
						G_BiMetalRecordArray[BoxNr][i].Add(BiMetalRecord); 
					}
				}

				for(int i = 0; i < 24; i++)
				{
					if(answer_cmd->para[i] <= (CurMaxSoundCount  * 2 / 3))
					{//同时听到多个声音，从概率上判定，只有最高的是真数据，其它的干扰
						answer_cmd->para[i] =	0;
						CurSoundCount[i]	=	0;
					}
					else
					{
					}
				}

			}
			if(BOX_DISC_16 == G_NormalConfigPara[BoxNr].BoxType)
			{
				for(int i = 0; i < 16; i++)
				{
					if(answer_cmd->para[i] > 0)
					{
						BiMetalRecord.st				=	st; 
						BiMetalRecord.temp				=	PidTemp; 
						BiMetalRecord.SoundCounts[0]	=   answer_cmd->para[i];	
						BiMetalRecord.id				=   i;
						if(this->GetOpenMode())
						{
							BiMetalRecord.IsActMode		=	1;
						}
						else
						{
							BiMetalRecord.IsActMode		=	0;
						}
						BiMetalRecord.SameCounts		=	SameCounts;
						G_BiMetalRecordArray[BoxNr][i].Add(BiMetalRecord); 
					}
				}

				for(int i = 0; i < 16; i++)
				{
					if(answer_cmd->para[i] <= (CurMaxSoundCount  * 2 / 3))
					{//同时听到10个声音，从概率上判定，只有最高的是真数据，其它的干扰
						answer_cmd->para[i] =	0;
						CurSoundCount[i]	=	0;
					}
				}

			}
			if(BOX_DISC_40 == G_NormalConfigPara[BoxNr].BoxType)
			{
				for(int i = 0; i < 20; i++)
				{
					if(answer_cmd->para[i] > 0)
					{
						BiMetalRecord.st				=	st; 
						BiMetalRecord.temp				=	PidTemp; 
						BiMetalRecord.SoundCounts[0]	=   answer_cmd->para[i];
						if(	RS485_Addr == 0)
						{
							BiMetalRecord.id				=   POS_A[i];
						}
						else
						{
							BiMetalRecord.id				=   POS_A[i] + 5;
						}
						if(this->GetOpenMode())
						{
							BiMetalRecord.IsActMode		=	1;
						}
						else
						{
							BiMetalRecord.IsActMode		=	0;
						}
						BiMetalRecord.SameCounts		=	SameCounts;
						G_BiMetalRecordArray[BoxNr][i].Add(BiMetalRecord); 
					}
				}
				
				for(int i = 0; i < 20; i++)
				{
					if(answer_cmd->para[i] <= (CurMaxSoundCount * 2 / 3))
					{//同时听到10个声音，从概率上判定，只有最高的是真数据，其它的干扰(邻居的声音)
						answer_cmd->para[i] =	0;
						CurSoundCount[i]	=	0;
					}
					
				}


			}

		}
		

				
		if(this->GetOpenMode())
		{//测动作
			if(BOX_DISC_24 == G_NormalConfigPara[BoxNr].BoxType)
			{
				
				for(i = 0; i < 24; i ++)
				{
					//if(G_NormalConfigPara[BoxNr].SoundOutputType == 0)
					{//显示测试的温度
						if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
						{
							
							if(	RS485_Addr == 0)
							{
								
									
									DealStatus_DiscOpenTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i],PidTemp,m_TempRange);
								
								
							}//(	RS485_Addr == 0)

							else if(	RS485_Addr == 1)
							{
								DealStatus_DiscOpenTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i]+5,PidTemp,m_TempRange);
								
							}//(	RS485_Addr == 1)

						}//(answer_cmd->para[i] > SOUND_COUNTS)
			
					}
				}
			}
			else if(BOX_DISC_16 == G_NormalConfigPara[BoxNr].BoxType)
			{
				
				for(i = 0; i < 16; i ++)
				{
					//if(G_NormalConfigPara[BoxNr].SoundOutputType == 0)
					{//显示测试的温度
						if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
						{
							
							if(	RS485_Addr == 0)
							{
								
									
									DealStatus_DiscOpenTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i],PidTemp,m_TempRange);
								
								
							}//(	RS485_Addr == 0)

							else if(	RS485_Addr == 1)
							{
								DealStatus_DiscOpenTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i]+5,PidTemp,m_TempRange);
								
							}//(	RS485_Addr == 1)

						}//(answer_cmd->para[i] > SOUND_COUNTS)
			
					}
				}
			}
			else
			{

				for(i = 0; i < 20; i ++)
				{
					//if(G_NormalConfigPara[BoxNr].SoundOutputType == 0)
					{//显示测试的温度
						if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
						{
							
							if(	RS485_Addr == 0)
							{
								DealStatus_DiscOpenTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i],PidTemp,m_TempRange);

								
							}//(	RS485_Addr == 0)

							else if(	RS485_Addr == 1)
							{
								DealStatus_DiscOpenTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i]+5,PidTemp,m_TempRange);
								
							}//(	RS485_Addr == 1)

						}//(answer_cmd->para[i] > SOUND_COUNTS)
			
					}
				}
			}
		}//测动作
		else if(GetCloseMode())
		{//降温
			
			if(BOX_DISC_24 == G_NormalConfigPara[0].BoxType)
			{
				
				for(i = 0; i < 24;i ++)
				{
					//if(G_NormalConfigPara[BoxNr].SoundOutputType == 0)
					{//显示测量的温度
						if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
						{
							if(	RS485_Addr == 0)
							{
									DealStatus_DiscCloseTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i],PidTemp,m_TempRange);
								
							}
							if(	RS485_Addr == 1)
							{
									DealStatus_DiscCloseTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i]+5,PidTemp,m_TempRange);
							}

						}
					}
						
				}
			
				
			}
			else if(BOX_DISC_16 == G_NormalConfigPara[0].BoxType)
			{
				
				for(i = 0; i < 16;i ++)
				{
					//if(G_NormalConfigPara[BoxNr].SoundOutputType == 0)
					{//显示测量的温度
						if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
						{
							if(	RS485_Addr == 0)
							{
									DealStatus_DiscCloseTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i],PidTemp,m_TempRange);
								
							}
							if(	RS485_Addr == 1)
							{
									DealStatus_DiscCloseTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i]+5,PidTemp,m_TempRange);
							}

						}
					}
						
				}
			
				
			}
			else
			{

				for(i = 0; i < 20;i ++)
				{
					//if(G_NormalConfigPara[BoxNr].SoundOutputType == 0)
					{//显示测量的温度
						if(CurSoundCount[i] > G_NormalConfigPara[0].SoundLevel_Time)
						{
							if(	RS485_Addr == 0)
							{
									DealStatus_DiscCloseTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i],PidTemp,m_TempRange);
								
							}
							if(	RS485_Addr == 1)
							{
									DealStatus_DiscCloseTemp(BoxNr,&BiMetalRecord,answer_cmd->para[i],CurMaxSoundCount,SameCounts,POS_A[i]+5,PidTemp,m_TempRange);
							}

						}
					}
						
				}
				
				
			}
			
		}

	
}
}

int CWorkMode::StartAirControl(CWnd * ParentWnd,int BoxNr,float CurTemp)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif


		int cool = G_NormalConfigPara[BoxNr].AirControlEquipment;
		if(cool != 0)
		{
			if(CurTemp < cool)
			{
				CString msg;
				if(G_RS485_Comm.SendAirControlCmd(true,G_Meter[BoxNr].m_IsAcOn,BoxNr,&msg) == -2)
				{
					MainWnd->m_TestMsg = msg;
					MainWnd->UpdateData(false); 
					return -2;
				}
			}
		}

		return 0;
}

int CWorkMode::StopAirControl(CWnd * ParentWnd,int BoxNr,float CurTemp)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

		if(G_Meter[BoxNr].m_IsAcOn == false)
		{
			return 0;
		}

		int cool = G_NormalConfigPara[BoxNr].AirControlEquipment;
		if(cool != 0)
		{
			if(CurTemp > cool)
			{
				CString msg;
				G_RS485_Comm.SendAirControlCmd(false,G_Meter[BoxNr].m_IsAcOn,BoxNr,&msg);
				return 0;
			}
		}
		


		return 0;
}

bool CWorkMode::GetOpenMode(void)
{
	if(m_SubMode == SUB_MODE_DIRECT_HEAT
		|| m_SubMode == SUB_MODE_PID
		|| m_SubMode == SUB_MODE_STEP_PID
		|| m_SubMode == SUB_MODE_PID_2
		|| m_SubMode == SUB_MODE_STEP_PID_2
		|| m_SubMode == SUB_MODE_STEP_TOP_PID
		|| m_SubMode == SUB_COOL_FIRST_MODE_COOL_PID
		|| m_SubMode == SUB_COOL_FIRST_MODE_STEP_COOL_PID
		)
	{
		return true;
	}
	return false;
}

bool CWorkMode::GetCloseMode(void)
{
	if(m_SubMode  == SUB_MODE_DIRECT_COOL
		  || m_SubMode == SUB_MODE_PID_COOL
		  || m_SubMode == SUB_MODE_STEP_COOL
		  || m_SubMode == SUB_MODE_HEAT_AGAIN
		  || m_SubMode == SUB_COOL_FIRST_MODE_HEAT_PID 
		  || m_SubMode == SUB_COOL_FIRST_MODE_STEP_HEAT_PID)
	{
		return true;
	}

	return false;
}

void CWorkMode::InitSwitchStatus(int BoxNr)
{
	if(G_NormalConfigPara[BoxNr].BoxType == BOX_NTC)
	{
		for(int i = 0; i < G_iMaxLedNr[BoxNr]; i++)
		{
			G_SwitchTestResult[BoxNr][i].IsUsed					= true;	//产品是否插入
			G_SwitchTestResult[BoxNr][i].IsClosing				= false;
			
			G_SwitchTestResult[BoxNr][i].IsOpenned				= false;	//是否曾经打开过
			G_SwitchTestResult[BoxNr][i].IsClosed				= false;		//是否曾经复位过
			G_SwitchTestResult[BoxNr][i].IsOpenFlash			= false;		//是否动作时闪动
			//G_SwitchTestResult[BoxNr][i].OpenFlashCount		= 0;
			G_SwitchTestResult[BoxNr][i].IsCloseFlash			= false;	//是否复位时闪动
			//G_SwitchTestResult[BoxNr][i].CloseFlashCount      = 0; 
			G_SwitchTestResult[BoxNr][i].IsCloseFlashTested		= false;	//是否复位时闪动
			G_SwitchTestResult[BoxNr][i].IsOpenFlashTested		= false;
			G_SwitchTestResult[BoxNr][i].IsNoPTC				= false;
			G_SwitchTestResult[BoxNr][i].OpenTemp				= 0.0f;		//动作温度
			G_SwitchTestResult[BoxNr][i].CloseTemp				= 0.0f;	//复位温度

			G_SwitchTestResult[BoxNr][i].IsOpenFlashTested		= false;
			G_SwitchTestResult[BoxNr][i].Group					=	::G_NormalConfigPara[BoxNr].Group[i]; 
			
			G_SwitchTestResult[BoxNr][i].KSD302_TickCount		=	0;
			G_SwitchTestResult[BoxNr][i].OpenSoundStrong		=	0;
			G_SwitchTestResult[BoxNr][i].CloseSoundStrong		=	0;

			

		}
		return;
	}
	if(G_NormalConfigPara[BoxNr].SwitchType == 0)
			{//常闭
				
				for(int i = 0; i < G_iMaxLedNr[BoxNr]; i++)
				{
					if(G_SwitchTestResult[BoxNr][i].IsClosing)
					{
						G_SwitchTestResult[BoxNr][i].IsUsed			= true;	//产品是否插入
					}
					else
					{
						G_SwitchTestResult[BoxNr][i].IsUsed				= false;	//产品是否插入
					}
					
					G_SwitchTestResult[BoxNr][i].IsOpenned				= false;	//是否曾经打开过
					G_SwitchTestResult[BoxNr][i].IsClosed				= false;		//是否曾经复位过
					G_SwitchTestResult[BoxNr][i].IsOpenFlash			= false;		//是否动作时闪动
					//G_SwitchTestResult[BoxNr][i].OpenFlashCount		= 0;
					G_SwitchTestResult[BoxNr][i].IsCloseFlash			= false;	//是否复位时闪动
					//G_SwitchTestResult[BoxNr][i].CloseFlashCount      = 0; 
					G_SwitchTestResult[BoxNr][i].IsCloseFlashTested		= false;	//是否复位时闪动
					G_SwitchTestResult[BoxNr][i].IsOpenFlashTested		= false;
					G_SwitchTestResult[BoxNr][i].IsNoPTC				= false;
					G_SwitchTestResult[BoxNr][i].OpenTemp				= 0.0f;		//动作温度
					G_SwitchTestResult[BoxNr][i].CloseTemp				= 0.0f;	//复位温度
#ifndef WINCE
					G_SwitchTestResult[BoxNr][i].OpenFlashTemp			= 0.0f;		//动作温度
					G_SwitchTestResult[BoxNr][i].CloseFlashTemp			= 0.0f;	//复位温度
					G_SwitchTestResult[BoxNr][i].OpenTime				= 0;
					G_SwitchTestResult[BoxNr][i].CloseTime				= 0;
#endif
					G_SwitchTestResult[BoxNr][i].IsOpenFlashTested		= false;
					G_SwitchTestResult[BoxNr][i].Group					=	::G_NormalConfigPara[BoxNr].Group[i]; 
					
					G_SwitchTestResult[BoxNr][i].KSD302_TickCount		=	0;
					G_SwitchTestResult[BoxNr][i].OpenSoundStrong		=	0;
					G_SwitchTestResult[BoxNr][i].CloseSoundStrong		=	0;

		//			G_SwitchTestResult[BoxNr][i].IsOpenned				= true;	//是否曾经打开过
		//			G_SwitchTestResult[BoxNr][i].IsClosed				= true;		//是否曾经复位过
		//			G_SwitchTestResult[BoxNr][i].IsUsed					= true;	//是否曾经打开过
		//			G_SwitchTestResult[BoxNr][i].OpenTemp				= i + 20;		//是否曾经复位过

		//			G_SwitchTestResult[BoxNr][i].CloseTemp				= i - 30;		//是否曾经复位过

					

				}
			}
			else
			{//常开
				
				for(int i = 0; i < G_iMaxLedNr[BoxNr]; i++)
				{
					if(G_SwitchTestResult[BoxNr][i].IsClosing)
					{
						G_SwitchTestResult[BoxNr][i].IsUsed				= false;	//产品是否插入
					}
					else
					{
						G_SwitchTestResult[BoxNr][i].IsUsed				= true;	//产品是否插入
					}
					G_SwitchTestResult[BoxNr][i].IsOpenned				= false;	//是否曾经打开过
					G_SwitchTestResult[BoxNr][i].IsClosed				= false;		//是否曾经复位过
					G_SwitchTestResult[BoxNr][i].IsOpenFlash			= false;		//是否动作时闪动
					G_SwitchTestResult[BoxNr][i].IsCloseFlash			= false;	//是否复位时闪动
					G_SwitchTestResult[BoxNr][i].IsCloseFlashTested		= false;	//是否复位时闪动
					G_SwitchTestResult[BoxNr][i].IsNoPTC				= false;

					G_SwitchTestResult[BoxNr][i].OpenTemp				= 0.0f;		//动作温度
					G_SwitchTestResult[BoxNr][i].CloseTemp				= 0.0f;	//复位温度	
#ifndef WINCE
					G_SwitchTestResult[BoxNr][i].OpenFlashTemp		= 0.0f;		//动作温度
					G_SwitchTestResult[BoxNr][i].OpenTime				= 0;
					G_SwitchTestResult[BoxNr][i].CloseTime			= 0;
					G_SwitchTestResult[BoxNr][i].CloseFlashTemp		= 0.0f;	//复位温度	
#endif

					G_SwitchTestResult[BoxNr][i].Group					=	::G_NormalConfigPara[BoxNr].Group[i];
					G_SwitchTestResult[BoxNr][i].KSD302_TickCount		=	0;
					G_SwitchTestResult[BoxNr][i].OpenSoundStrong		=	0;
					G_SwitchTestResult[BoxNr][i].CloseSoundStrong		=	0;
					
				}
	}
}

void CWorkMode::AutoTestStep_CoolFirst_Ready(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	CONTROL_TEMP_RANGE		m_TempRange;
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	if(G_NormalConfigPara[BoxNr].AirControlEquipment == 0)
	{
		//m_SubMode				=	SUB_MODE_DIRECT_HEAT;
		//m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
		//G_RS485_Comm.SendDirectHeatCmd(GetControlGoalTemp(SUB_MODE_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr),BoxNr); 
	}
	else
	{
		if(CurTemp > GetControlGoalTemp(SUB_COOL_FIRST_MODE_READY,HeatCool,false,m_TempRange,BoxNr))
		{
			InitSwitchStatus(BoxNr);

			m_SubMode				=	SUB_COOL_FIRST_MODE_DIRECT_COOL;
			m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
			float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_DIRECT_COOL,HeatCool,false,m_TempRange,BoxNr);
			G_RS485_Comm.SendStopHeatCmd(0.0f,BoxNr);
			
			this->StartAirControl(ParentWnd,BoxNr,CurTemp); 
			
			if(::G_NormalConfigPara[BoxNr].AirControlEquipment > 0)
			{
				if(CurTemp > 40.0f )
				{
					if(G_Meter[BoxNr].m_IsDoorOpened == false)
					{
						G_Meter[BoxNr].m_IsDoorOpened = true;
						G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
					}
				}
			}
			else
			{
				if(G_Meter[BoxNr].m_IsDoorOpened == false)
				{
					G_Meter[BoxNr].m_IsDoorOpened = true;
					G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
				}
			}

		}
		else
		{
			float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_READY,HeatCool,false,m_TempRange,BoxNr);

			if(G_Meter[BoxNr].m_PcbTestMode != PCB_SUB_MODE_DIRECT_HEAT)
			{
				//G_Meter[BoxNr].m_PcbTestMode = PCB_SUB_MODE_DIRECT_HEAT;
				//G_RS485_Comm.SendDirectHeatCmd(goal,BoxNr);
			}

			
			if(G_Meter[BoxNr].m_IsDoorOpened == true)
			{
				G_Meter[BoxNr].m_IsDoorOpened = false;
				G_RS485_Comm.SendOpenDoorCmd(false,BoxNr); 
			}
			
			CTime cur = CTime::GetCurrentTime();
			CTimeSpan span = cur - this->m_CurSubModeBeginTime; 
			int sec = span.GetTotalSeconds();
			int minutes = (sec / 60);
			sec = (sec % 60);
			MainWnd->m_TestMsg.Format(_T("全速升温到 %5.1f度 %d:%02d"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_READY,HeatCool,false,m_TempRange,BoxNr),minutes,sec );
			MainWnd->UpdateData(false);

			StopAirControl(MainWnd,BoxNr,CurTemp);  

		}
	}

}

void CWorkMode::AutoTestStep_CoolFirst_DirectCool(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	CONTROL_TEMP_RANGE		m_TempRange;
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);
	
	int	span_minutes,span_seconds;
	CTime cur = cur.GetCurrentTime();
	CTimeSpan ts = cur - m_CurSubModeBeginTime; 
#ifdef WINCE 
	int pid_time = (G_NormalConfigPara[BoxNr].PidCoolTime * 60);
#else
	int pid_time = (G_NormalConfigPara[BoxNr].PidTime * 60);
#endif
	span_seconds = pid_time - ts.GetTotalSeconds();

	span_minutes	 = span_seconds / 60;

	span_seconds = (span_seconds % 60);

	if(CurTemp < GetControlGoalTemp(SUB_COOL_FIRST_MODE_DIRECT_COOL,HeatCool,false,m_TempRange,BoxNr)
		|| IsAllOpened(BoxNr))
	{
		m_SubMode				=	SUB_COOL_FIRST_MODE_COOL_PID;
		m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
		float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_COOL_PID,HeatCool,false,m_TempRange,BoxNr);
		G_RS485_Comm.SendPidCoolCmd(goal,BoxNr,::G_NormalConfigPara[0].CoolDownAutoOpenDoor);
		m_CurSubModeBeginTime = CTime::GetCurrentTime();
		MainWnd->m_TestMsg.Format(_T("恒温到 %5.1f度"),goal);
		MainWnd->UpdateData(false);
	}
	else
	{
			MainWnd->m_TestMsg.Format(_T("全速降温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_DIRECT_COOL,HeatCool,false,m_TempRange,BoxNr));
			MainWnd->UpdateData(false);

			StartAirControl(ParentWnd,BoxNr,CurTemp);
	}

}

void CWorkMode::AutoTestStep_CoolFirst_CoolPid(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
	#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	CONTROL_TEMP_RANGE		m_TempRange;
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);
	
	int	span_minutes,span_seconds;
	CTime cur = cur.GetCurrentTime();
	CTimeSpan ts = cur - m_CurSubModeBeginTime; 
#ifdef WINCE 
	int pid_time = (G_NormalConfigPara[BoxNr].PidCoolTime * 60);
#else
	int pid_time = (G_NormalConfigPara[BoxNr].PidTime * 60);
#endif
	span_seconds = pid_time - ts.GetTotalSeconds();

	span_minutes	 = span_seconds / 60;

	span_seconds = (span_seconds % 60);

	if(ts.GetTotalSeconds() > pid_time 
		|| IsAllOpened(BoxNr))
	{
			{
				m_SubMode				=	SUB_COOL_FIRST_MODE_STEP_COOL_PID;
				m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
				float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_COOL_PID,HeatCool,false,m_TempRange,BoxNr);
				MainWnd->m_TestMsg.Format(_T("匀速降温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_COOL_PID,HeatCool,false,m_TempRange,BoxNr));
				MainWnd->UpdateData(false);

				if(::G_NormalConfigPara[BoxNr].CoolSpeed <= 0)
				{
					::G_NormalConfigPara[BoxNr].CoolSpeed = 10;
				}
				int minutes = (CurTemp - goal) / (::G_NormalConfigPara[BoxNr].CoolSpeed / 10.0f);   
				if(minutes < 1)
				{
					minutes = 1;
				}
				G_RS485_Comm.SendStepPidCoolCmd(goal,minutes,BoxNr,::G_NormalConfigPara[BoxNr].CoolDownAutoOpenDoor); 

				this->StartAirControl(ParentWnd,BoxNr,CurTemp); 
				
				if(::G_NormalConfigPara[BoxNr].AirControlEquipment > 0)
				{
					if(CurTemp > 40.0f)
					{
						if(G_Meter[BoxNr].m_IsDoorOpened == false)
						{
							G_Meter[BoxNr].m_IsDoorOpened = true;
							G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
						}
					}
				}
				else
				{
						if(G_Meter[BoxNr].m_IsDoorOpened == false)
						{
							G_Meter[BoxNr].m_IsDoorOpened = true;
							G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
						}
				}
			}
	}
	else
	{
			MainWnd->m_TestMsg.Format(_T("降温恒温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_COOL_PID,HeatCool,false,m_TempRange,BoxNr),span_minutes,span_seconds % 60);
			MainWnd->UpdateData(false);

			StartAirControl(ParentWnd,BoxNr,CurTemp);
	}

}

void CWorkMode::AutoTestStep_CoolFirst_StepCoolPid(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	CONTROL_TEMP_RANGE		m_TempRange;
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	int	span_minutes,span_seconds;
	CTime cur		= cur.GetCurrentTime();
	CTimeSpan ts	= cur - m_CurSubModeBeginTime; 
	span_seconds	= ts.GetTotalSeconds();
	span_minutes	= span_seconds / 60;

	span_seconds	= (span_seconds % 60);

	

	if((CurTemp < GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_COOL_PID,HeatCool,false,m_TempRange,BoxNr))
		|| IsAllOpened(BoxNr))
	{

			if(m_TempRange.close_temp_min == m_TempRange.close_temp_max)
			{//不测复位,结束测试
				this->ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode); 
			}
			else
			{
				if(IsAllOpened(BoxNr))
				{
					m_SubMode				=	SUB_COOL_FIRST_MODE_HEAT_PID;
					m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
					float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_HEAT_PID,HeatCool,false,m_TempRange,BoxNr);

					G_RS485_Comm.SendNormalPidCmd(goal,BoxNr); 
					MainWnd->m_TestMsg.Format(_T("升温恒温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_HEAT_PID,HeatCool,false,m_TempRange,BoxNr));
					MainWnd->UpdateData(false);
				}
				else
				{
					m_SubMode				=	SUB_COOL_FIRST_MODE_HEAT_PID;
					m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
					float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_HEAT_PID,HeatCool,false,m_TempRange,BoxNr);

					G_RS485_Comm.SendNormalPidCmd(goal,BoxNr); 
					MainWnd->m_TestMsg.Format(_T("升温恒温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_HEAT_PID,HeatCool,false,m_TempRange,BoxNr));
					MainWnd->UpdateData(false);

					this->StartAirControl(ParentWnd,BoxNr,CurTemp); 
					
					if(::G_NormalConfigPara[BoxNr].AirControlEquipment > 0)
					{
						if(CurTemp > 40.0f)
						{
							if(G_Meter[BoxNr].m_IsDoorOpened == false)
							{
								G_Meter[BoxNr].m_IsDoorOpened = true;
								G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
							}
						}
					}
					else
					{
							if(G_Meter[BoxNr].m_IsDoorOpened == false)
							{
								G_Meter[BoxNr].m_IsDoorOpened = true;
								G_RS485_Comm.SendOpenDoorCmd(true,BoxNr); 
							}
					}
				}
			}
	}
	else
	{
		
			float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_COOL_PID,HeatCool,false,m_TempRange,BoxNr);
			MainWnd->m_TestMsg.Format(_T("匀速降温到 %5.1f度 %02d:%02d"),goal,span_minutes,span_seconds);
			MainWnd->UpdateData(false);
		
			StartAirControl(ParentWnd,BoxNr,CurTemp);
	}

}

void CWorkMode::AutoTestStep_CoolFirst_HeatPid(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	CONTROL_TEMP_RANGE		m_TempRange;
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	
	int	span_minutes,span_seconds;
	CTime cur		= cur.GetCurrentTime();
	CTimeSpan ts	= cur - m_CurSubModeBeginTime; 
	span_seconds	= ::G_NormalConfigPara[BoxNr].PidTime * 60- ts.GetTotalSeconds();
	

	
	if(IsAllClosed(BoxNr))
	{
		this->ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode); 
	}
	/*else if(CurTemp < GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr))
	{//为什么要这个代码？
			m_CurSubModeBeginTime = CTime::GetCurrentTime();

			m_SubMode				=	SUB_COOL_FIRST_MODE_HEAT_PID;
			m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
			float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_HEAT_PID,HeatCool,false,m_TempRange,BoxNr);


			if(G_RS485_Comm.m_LastCmd_128[BoxNr].cmd != CMD_NORMAL_PID_OUT)
			{
				G_RS485_Comm.SendNormalPidCmd(goal,BoxNr); 
			}
			MainWnd->m_TestMsg.Format(_T("升温恒温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_HEAT_PID,HeatCool,false,m_TempRange,BoxNr));
			MainWnd->UpdateData(false);

				
	}*/
	else
	{
		if(span_seconds <= 0)
		{
			m_CurSubModeBeginTime = CTime::GetCurrentTime();

			m_SubMode				=	SUB_COOL_FIRST_MODE_STEP_HEAT_PID;
			m_CurSubModeBeginTime	=	CTime::GetCurrentTime(); 
			float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_HEAT_PID,HeatCool,false,m_TempRange,BoxNr);
			if(::G_NormalConfigPara[BoxNr].HeatSpeed <= 0)
			{
				::G_NormalConfigPara[BoxNr].HeatSpeed = 10;
			}
			int minutes = (goal - CurTemp) / (::G_NormalConfigPara[BoxNr].HeatSpeed / 10.0f);
			if(minutes <= 0)
			{
				minutes = 0;
			}

			G_RS485_Comm.SendStepPIDCmd(goal,minutes,BoxNr); 

			MainWnd->m_TestMsg.Format(_T("匀速升温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_HEAT_PID,HeatCool,false,m_TempRange,BoxNr));
			MainWnd->UpdateData(false);
		}
		else
		{
			span_minutes	= span_seconds / 60;

			span_seconds	= (span_seconds % 60);
		
			float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_HEAT_PID,HeatCool,false,m_TempRange,BoxNr);
			MainWnd->m_TestMsg.Format(_T("升温恒温到 %5.1f度 %02d:%02d"),goal,span_minutes,span_seconds);
			MainWnd->UpdateData(false);
		
			StartAirControl(ParentWnd,BoxNr,CurTemp);
		}
	}
}

void CWorkMode::AutoTestStep_CoolFirst_StepHeatPid(CWnd * ParentWnd,int BoxNr,float CurTemp,SWITCH_CONFIG_PARA	SwitchConfigPara)
{

#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif
	CONTROL_TEMP_RANGE		m_TempRange;
	
	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(BoxNr,SwitchConfigPara  , &m_TempRange,G_NormalConfigPara[BoxNr].BoxType);

	int	span_seconds;
	CTime cur		= cur.GetCurrentTime();
	CTimeSpan ts	= cur - m_CurSubModeBeginTime; 
	span_seconds	= ::G_NormalConfigPara[BoxNr].PidTime * 60- ts.GetTotalSeconds();
	
	float goal = GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_HEAT_PID,HeatCool,false,m_TempRange,BoxNr);
	
	if(IsAllClosed(BoxNr))
	{
		this->ExitAutoTest(ParentWnd,BoxNr,CurTemp,SwitchConfigPara,MainWnd->m_Barcode); 
	}
	else if(CurTemp < GetControlGoalTemp(SUB_COOL_FIRST_MODE_STEP_DIRECT_HEAT,HeatCool,false,m_TempRange,BoxNr))
	{
		
	}

}

void CWorkMode::DisplaySendCmd(CWnd * ParentWnd,unsigned char cmd,unsigned char *cmd_buf,int len,int BoxNr)
{
	static int LastSendBox = 0;
#ifdef WINCE
	CDebugDlg * DebugDlg = (CDebugDlg*)ParentWnd; 
#else
	CDebugDlg * DebugDlg = (CDebugDlg*)ParentWnd; 
#endif;
	CString s,s1;


	DebugDlg->m_SendCmdCount++;
#ifdef WINCE
	DebugDlg->UpdateData(false);
#else
	CWnd *w = DebugDlg->GetDlgItem(IDC_EDIT_SEND_CMD_COUNT);
						
	s.Format(_T("%d"), DebugDlg->m_SendCmdCount);
	w->SetWindowTextW(s); 

	w = DebugDlg->GetDlgItem(IDC_EDIT_RECEIVE_CMD_COUNT);
	s.Format(_T("%d"),DebugDlg->m_SendCmdCount - DebugDlg->m_ReceiveCmdCount);
	w->SetWindowTextW(s);
#endif

	DebugDlg->m_CalMsg.Format(_T("发:%s"),CComm::GetCmdString(cmd));
	s = _T("");
	for(int i = 0; i < len; i++)
	{
		s1.Format(_T("%02X "),cmd_buf[i]);
		s+=s1;
		
	}
	DebugDlg->G_CommSendString = _T("发送:") + s;

#ifndef WINCE
	

	
		G_AboutDlg.G_CommRec += _T("\r\n") ;
		if(G_AboutDlg.G_CommRec.GetLength() > 3)
		{
			WriteLogFile(G_AboutDlg.G_CommRec,LastSendBox);
		}
		G_AboutDlg.G_CommSend = G_AboutDlg.G_CommRec + G_AboutDlg.G_CommSend;
		
		G_AboutDlg.G_CommSend = _T("\r\n") + G_AboutDlg.G_CommSend;

		//CString s1;
		SYSTEMTIME systemTime;
		GetSystemTime(&systemTime); // 获取当前 UTC 时间

		s1.Format(_T("%02d:%02d:%02d:%03d  发送:"),
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond,systemTime.wMilliseconds);
		//G_AboutDlg.G_CommSend +=  formattedTime;// + G_CommRec;
		s1 += s;
		s1 += _T("\r\n");
		if(s.Left(2) == _T("31")
			|| s.Left(2) == _T("41"))
		{
			LastSendBox = 0;
		}
		else if(s.Left(2) == _T("32")
			|| s.Left(2) == _T("42"))
		{
			LastSendBox = 1;
		}
		else if(s.Left(2) == _T("33")
			|| s.Left(2) == _T("43"))
		{
			LastSendBox = 2;
		}
		else if(s.Left(2) == _T("34")
			|| s.Left(2) == _T("44"))
		{
			LastSendBox = 3;
		}

		WriteLogFile(s1,LastSendBox);
		if(G_AboutDlg.G_CommSend.GetLength() > 10000)
		{
			G_AboutDlg.G_CommSend = _T("");
		}
		G_AboutDlg.G_CommSend = s1 + G_AboutDlg.G_CommSend;
		G_AboutDlg.G_CommRec = _T("");


	if(G_AboutDlg.GetSafeHwnd() && G_AboutDlg.IsWindowVisible())
	{	
		w = G_AboutDlg.GetDlgItem( IDC_EDIT_COM_REC );

		w->SetWindowTextW(G_AboutDlg.G_CommSend);
	}

#endif

	if(DebugDlg->IsWindowVisible())
	{
		DebugDlg->UpdateData(false);
	}

}

void CWorkMode::TempCmd_Disc(CWnd * ParentWnd,int MeterID)
{
#ifdef WINCE
	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &G_HeatBoxDlg.m_WorkMode;
#else
	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_WorkMode;
#endif;

	if(m_SendCmdDeltaFrequnce.GetCount() > 0)
	{
		DELTA_FREQUNCE_CMD_ASC cmd = m_SendCmdDeltaFrequnce.GetAt(0); 
		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,7,2,0);

		G_CommProtocolType = COMM_PROTOCOL_DELTA_FREQUENCE_ASC;

 		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(DELTA_FREQUNCE_CMD_ASC),0); 
#else
		DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],0,(unsigned char*)&cmd,sizeof(DELTA_FREQUNCE_CMD_ASC),0); 
#endif
		return;
	}

	if(G_SendmCmdProgram_ARM.GetCount() > 0)
	{
		MODBUS_RS485_ARM_CMD_PROGRAM cmd = G_SendmCmdProgram_ARM.GetAt(0); 

		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,38400,8,0,0);
		G_CommProtocolType = COMM_PROTOCOL_MODBUS_PROGRAM;

		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(MODBUS_RS485_ARM_CMD_PROGRAM),0); 
#else
		for(int i = 0; i < ::G_CurMaxHeatBox; i++)
		{
			DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[i],cmd.head.cmd,(unsigned char*)&cmd,sizeof(MODBUS_RS485_ARM_CMD_PROGRAM),i); 
		}
#endif
		return;
	}

	if(G_SendmCmdProgram.GetCount() > 0)
	{
		MODBUS_RS485_CMD_PROGRAM cmd = G_SendmCmdProgram.GetAt(0); 

		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
		G_CommProtocolType = COMM_PROTOCOL_MODBUS_PROGRAM;

		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),0); 
#else
		for(int i = 0; i < ::G_CurMaxHeatBox; i++)
		{
			DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[i],cmd.head.cmd,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),i); 
		}
#endif
		return;
	}


	G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);

	static MODBUS_RS485_CMD_128 last_Cmd128;
	static int					repeat_counts = 0;

	G_bCMD_192_READ_STATUS = false;

	MODBUS_RS485_CMD_128 * cmd;

	if(m_CmdArray_128[MeterID % G_CurMaxHeatBox].GetSize() == 0)
	{
					
		if(G_RS485_Comm.IsOpened())
		{
			if(G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == BOX_DISC_24 
				|| G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == BOX_DISC_16 
				|| G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == BOX_DISC_40 )
			{
				
				static unsigned char BOX_DISC_RS485_Addr = 0;
				static unsigned int count = 0;
				
				

				switch(BOX_DISC_RS485_Addr)
				{
				case 0:
				default:
					{
						cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadTemp(MeterID % G_CurMaxHeatBox); 
#ifdef WINCE
#else
						DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox] ,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
#endif
					}
					break;
				case 1:
					{
						switch(G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundOutputType)
						{
						case 1:
							{
								if(WorkMode->GetOpenMode())
								{//升温过程
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								}
								else if(WorkMode->GetCloseMode())
								{
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Cool ); 
								}
								else
								{
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								}
							}
							break;
						case 2:
							{
								cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								//DisplaySendCmd((CWnd*)&G_DebugDlg,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
							}
						break;
						case 3:
							{
								cmd =(MODBUS_RS485_CMD_128*) G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Cool ); 
								//DisplaySendCmd((CWnd*)&G_DebugDlg,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
							}
						break;
						case 0:
						default:
							{
								if(WorkMode->GetOpenMode())
								{//升温过程
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								}
								else if(WorkMode->GetCloseMode())
								{
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Cool ); 
								}
								else
								{
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								}
							}
						break;
						}
#ifdef WINCE
#else
						DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox] ,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
#endif
					}
					break;
				case 2:
					{
						switch(G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundOutputType)
						{
						case 1:
							{
								if(WorkMode->GetOpenMode())
								{//升温过程
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus(1 ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								}
								else if(WorkMode->GetCloseMode())
								{
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus(1 ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Cool ); 
								}
								else
								{
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus(1 ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								}
							}
							break;
						case 2:
							{
								cmd = G_RS485_Comm.SendReadBimetalStatus(1,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
							}
						break;
						case 3:
							{
								cmd = G_RS485_Comm.SendReadBimetalStatus(1,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Cool ); 
							}
						break;
						case 0:
						default:
							{
								if(WorkMode->GetOpenMode())
								{//升温过程
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								}
								else if(WorkMode->GetCloseMode())
								{
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Cool ); 
								}
								else
								{
									cmd = (MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus((MeterID % G_CurMaxHeatBox) ,::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].SoundLevel_Heat ); 
								}

							}
						break;
						}
#ifdef WINCE
#else
						DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox] ,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
#endif
					}
					break;
				}


				BOX_DISC_RS485_Addr ++;
				if(G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == BOX_DISC_40)
				{
					if(BOX_DISC_RS485_Addr > 2)
					{
						BOX_DISC_RS485_Addr = 0;
					}
				}
				else if(G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == BOX_DISC_16  
					|| G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == BOX_DISC_24)
				{
					if(BOX_DISC_RS485_Addr > 1)
					{
						BOX_DISC_RS485_Addr = 0;
					}
				}
			}
		}
	}
	else
	{
		if(G_RS485_Comm.IsOpened())
		{
						
				G_IsJustCmdAscii = false;
				static MODBUS_RS485_CMD_128 cmd_128;
				cmd_128 = m_CmdArray_128[(MeterID % G_CurMaxHeatBox)].GetAt(0);  
				cmd = &cmd_128;
				G_CommProtocolType = COMM_PROTOCOL_MODBUS_128;
				G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
				
				G_RS485_Comm.WriteComm((char*)&cmd_128,sizeof(cmd_128));
#ifdef WINCE
#else				
				DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox] ,cmd_128.head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
#endif
				if(last_Cmd128.head.cmd == cmd_128.head.cmd)
				{
					repeat_counts ++;
					if(repeat_counts > 2)
					{
#ifdef WINCE
						G_HeatBoxDlg.m_TestMsg.Format(_T("重复命令 %02X,%02X,%02X,%d次,%s"),last_Cmd128.head.cmd,last_Cmd128.para[0],last_Cmd128.para[1],  repeat_counts,CComm::GetCmdString((COMMAND_ID)last_Cmd128.head.cmd)); 
						G_HeatBoxDlg.UpdateData(false); 
#else
#endif
					}
					if(repeat_counts >= 5)
					{//为了兼容老的线路板,下面这些命令不支持
						
						if(last_Cmd128.head.cmd == (char)CMD_STEP_PID_COOL
							|| last_Cmd128.head.cmd == (char)CMD_PID_COOL
							|| last_Cmd128.head.cmd == (char)CMD_AT
							|| last_Cmd128.head.cmd == (char)CMD_COOL_AT
							|| last_Cmd128.head.cmd == (char)CMD_PERCENT_HEAT_WITH_AIR
							|| last_Cmd128.head.cmd == (char)CMD_AT_END
							)
						{
							if(m_CmdArray_128[(MeterID % G_CurMaxHeatBox)].GetSize() > 0) 
							{
								m_CmdArray_128[(MeterID % G_CurMaxHeatBox)].RemoveAt(0); 
							}
						}
					}
				}

		}
	}
}



void CWorkMode::TempCmd_TX300W_POWER(CWnd * ParentWnd,int MeterID)
{

}

void CWorkMode::TempCmd_128(CWnd * ParentWnd,int MeterID) 
{
	
	
#ifdef WINCE
	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &G_HeatBoxDlg.m_WorkMode;
#else
	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_WorkMode;
#endif;



	G_bCMD_192_READ_STATUS = false;



	if(m_SendCmdDeltaFrequnce.GetCount() > 0)
	{
		DELTA_FREQUNCE_CMD_ASC cmd = m_SendCmdDeltaFrequnce.GetAt(0); 
		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,7,2,0);

		G_CommProtocolType = COMM_PROTOCOL_DELTA_FREQUENCE_ASC;

 		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(DELTA_FREQUNCE_CMD_ASC),0); 
#else
		DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],0,(unsigned char*)&cmd,sizeof(DELTA_FREQUNCE_CMD_ASC),0); 
#endif
		return;
	}
#ifndef WINCE
	if(G_SendmCmdProgram_ARM.GetCount() > 0 )
	{
		CWnd *w;
		CString s;

		MainDlg->SetTimer(1,1200,NULL);

		w= MainDlg->m_DebugDlg[0].GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE); 
		s.Format(_T("更新..%d"),G_SendmCmdProgram_ARM.GetCount());
		w->SetWindowTextW(s);

		MODBUS_RS485_ARM_CMD_PROGRAM cmd = G_SendmCmdProgram_ARM.GetAt(0); 

		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,38400,8,0,0);
		G_CommProtocolType = COMM_PROTOCOL_MODBUS_PROGRAM;

		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));

#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(MODBUS_RS485_ARM_CMD_PROGRAM),0); 
#else
		for(int i = 0; i < ::G_CurMaxHeatBox; i++)
		{
			DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[i],cmd.head.cmd,(unsigned char*)&cmd,sizeof(MODBUS_RS485_ARM_CMD_PROGRAM),i); 
		}
#endif
		return;
	}
#endif

	if(G_SendmCmdProgram.GetCount() > 0 )
	{

		CWnd *w;
		CString s;
#ifndef WINCE
		w= MainDlg->m_DebugDlg[G_ProgramBoxNr].GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE); 
#else
		w= G_DebugDlg.GetDlgItem(IDC_BUTTON_PROGRAM); 
#endif
		s.Format(_T("更新..%d"),G_SendmCmdProgram.GetCount());
		w->SetWindowTextW(s);
		
		MODBUS_RS485_CMD_PROGRAM cmd = G_SendmCmdProgram.GetAt(0); 

		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
		G_CommProtocolType = COMM_PROTOCOL_MODBUS_PROGRAM;

		if(cmd.para[0] == 0)
		{
			cmd.para[4 + FLASH_PARA_ADDR] = G_RS485_Comm.OSCCAL;
			cmd.crc		= CComm::CRC16((unsigned char*)&cmd,sizeof(cmd) - 2); 
		}

		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),0); 
#else
		//for(int i = 0; i < ::G_CurMaxHeatBox; i++)
		{
			DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[G_ProgramBoxNr],cmd.head.cmd,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),G_ProgramBoxNr); 
		}
#endif
		return;
	}

	MainDlg->SetTimer(1,G_NormalConfigPara[0].CommTimer,NULL);

	G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
	

	if(G_iCmdAscii[MeterID % G_CurMaxHeatBox] == 1)
	{
	
		ASCII_RS485_COMMAND		ascii_cmd;
		MODBUS_RS485_CMD_128	modbus_cmd;

		if(m_CmdArray_128[MeterID % G_CurMaxHeatBox].GetSize() == 0)
		{	
			if(G_RS485_Comm.IsOpened())
			{
				short para[4];
				
				{
					G_RS485_Comm.SetCmdString_ASCII(&ascii_cmd,'1' + MeterID % G_CurMaxHeatBox,CMD_READ_TEMP,para);

					G_IsJustCmdAscii = true;
					
					G_CommProtocolType = COMM_PROTOCOL_ASC;
					G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
					G_RS485_Comm.WriteComm((char*)&ascii_cmd,sizeof(ascii_cmd));
#ifdef WINCE
					DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&ascii_cmd,sizeof(ASCII_RS485_COMMAND),(MeterID % G_CurMaxHeatBox)); 
#else
					DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],0,(unsigned char*)&ascii_cmd,sizeof(ASCII_RS485_COMMAND),(MeterID % G_CurMaxHeatBox)); 
#endif				
				}
			}
		}
		else
		{
			
			if(G_RS485_Comm.IsOpened())
			{
		
				modbus_cmd = m_CmdArray_128[MeterID % G_CurMaxHeatBox].GetAt(0);

				{
					G_IsJustCmdAscii = true;
					
					G_RS485_Comm.SetCmdString_ASCII(&ascii_cmd,'1' + MeterID % G_CurMaxHeatBox,modbus_cmd.head.cmd,(short*)modbus_cmd.para);
					
					G_CommProtocolType = COMM_PROTOCOL_ASC;
					G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
					G_RS485_Comm.WriteComm((char*)&ascii_cmd,sizeof(ascii_cmd));
#ifdef WINCE
					DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&ascii_cmd,sizeof(ascii_cmd),MeterID % G_CurMaxHeatBox); 
#else
					DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],0,(unsigned char*)&ascii_cmd,sizeof(ascii_cmd),MeterID % G_CurMaxHeatBox); 
#endif					
				}
			}
		}

		
	}
	else
	{ // MODBUS CMD

		MODBUS_RS485_CMD_128 * cmd;

			if(m_CmdArray_128[MeterID % G_CurMaxHeatBox].GetSize() == 0)
			{
							
				if(G_RS485_Comm.IsOpened())
				{
					
					{
#ifndef WINCE
						if(false)//((MainDlg->m_SwitchDlg[G_TakeoutDlg.m_BoxNr].m_WorkMode.m_SubMode == SUB_MODE_STOP ) && ((MeterID % G_CurMaxHeatBox)  == G_TakeoutDlg.m_BoxNr) && (::G_TakeoutDlg.GetSafeHwnd() != NULL) && (MainDlg->m_SwitchDlg[G_TakeoutDlg.m_BoxNr].m_IsTaking == true)) ||  (::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == HEAT_BOX_TYPE_HANJIE) )
						{
							G_bCMD_192_READ_STATUS = false;
							
							cmd = G_RS485_Comm.SendReadStatus((MeterID % G_CurMaxHeatBox));
							//DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],cmd->head.cmd,(unsigned char*)cmd,sizeof(MODBUS_RS485_CMD_128),MeterID % G_CurMaxHeatBox); 
							static long last_ticket;
							CTime cur = CTime::GetCurrentTime();
							long t = ::GetTickCount();
							CString s;
							

							s.Format(_T("%d,%d,测试开始时间：%02d/%02d/%02d %02d:%02d:%02d  当前时间%d:%d:%d 通讯数量=%d 接收=%d  间隔:%d毫秒"),MeterID , G_CurMaxHeatBox, G_TakeoutDlg.m_BeginTestTime.GetYear(),G_TakeoutDlg.m_BeginTestTime.GetMonth(),G_TakeoutDlg.m_BeginTestTime.GetDay(),  G_TakeoutDlg.m_BeginTestTime.GetHour(),G_TakeoutDlg.m_BeginTestTime.GetMinute(),G_TakeoutDlg.m_BeginTestTime.GetSecond(), cur.GetHour(),cur.GetMinute(),cur.GetSecond(),G_MainDlg->m_DebugDlg[G_TakeoutDlg.m_BoxNr].m_SendCmdCount,::G_MainDlg->m_DebugDlg[G_TakeoutDlg.m_BoxNr].m_ReceiveCmdCount ,  t - last_ticket);
							last_ticket = t;
							if(G_TakeoutDlg.GetSafeHwnd() != NULL)
							{
								if(G_TakeoutDlg.IsWindowVisible())
								{
									G_TakeoutDlg.SetWindowTextW(s);
								}
							}

						}
						else
#endif 
						if(MeterID < G_CurMaxHeatBox)
						{
							if(::G_NormalConfigPara[0].BoxType == BOX_TEMP2_10IN_12OUT)
							{
								LoadLifeTestBench_WORK_PARA para;
								memset(&para,0,sizeof(para));
								para.Temp_Sv				= 300;
								para.Temp_Sv_2				= 300;
								para.IsClosing				= 0;
								para.IsClosing_2			= 0;
								para.Temp_Control_Mode		= PCB_SUB_MODE_STOP;//PCB_SUB_MODE_NORMAL_PID;//PCB_SUB_MODE_DIRECT_HEAT;
								para.Temp_Control_Mode_2    = PCB_SUB_MODE_STOP;//PCB_SUB_MODE_NORMAL_PID;//PCB_SUB_MODE_DIRECT_HEAT;
								para.Beep					=	1;
								//para.Temp_Control_Mode		= PCB_SUB_MODE_DIRECT_HEAT;
								//para.Temp_Control_Mode_2    = PCB_SUB_MODE_DIRECT_HEAT;

								cmd = G_RS485_Comm.SendReadTemp(MeterID % G_CurMaxHeatBox,(char*)&para);
							}
							else
							{
								cmd = G_RS485_Comm.SendReadTemp(MeterID % G_CurMaxHeatBox);
							}

						}
						else
						{
							
							cmd = G_RS485_Comm.SendReadStatus(MeterID - G_CurMaxHeatBox);

						}
					}

				
				}

							if(G_RS485_Comm.IsOpened())
							{
				#ifdef WINCE
							DisplaySendCmd((CWnd*)&G_DebugDlg,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
				#else
							DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox] ,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
				#endif
							}

			}
			else //m_CmdArray_128[CurBoxNr].GetSize() != 0)

			{
				if(G_RS485_Comm.IsOpened())
				{
								
						G_IsJustCmdAscii = false;
						static MODBUS_RS485_CMD_128 cmd_128;
						cmd_128 = m_CmdArray_128[(MeterID % G_CurMaxHeatBox)].GetAt(0);  
						cmd = &cmd_128;
						G_CommProtocolType = COMM_PROTOCOL_MODBUS_128;
						G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
						
						G_RS485_Comm.WriteComm((char*)&cmd_128,sizeof(cmd_128));

#ifdef WINCE
					DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd_128,sizeof(cmd_128),MeterID % G_CurMaxHeatBox); 
#else
					DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],0,(unsigned char*)&cmd_128,sizeof(cmd_128),MeterID % G_CurMaxHeatBox); 
#endif		

						if(G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd == cmd_128.head.cmd)
						{
							G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] ++;
							if(G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] > 2)
							{
#ifdef WINCE
								G_HeatBoxDlg.m_TestMsg.Format(_T("重复命令 %02X,%02X,%02X,%d次,%s"),
									G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd,
									G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].para[0],
									G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].para[1],  
									G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox],
									CComm::GetCmdString((COMMAND_ID)G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd)); 
								G_HeatBoxDlg.UpdateData(false); 
#else
								MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_TestMsg.Format(_T("重复命令 %02X,%02X,%02X,%d次,%s"),
									G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd,
									G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].para[0],
									G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].para[1],  
									G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox],
									CComm::GetCmdString((COMMAND_ID)G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd)); 
								MainDlg->UpdateData(false);
#endif
							}
							if(G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] >= 5)
							{//为了兼容老的线路板,下面这些命令不支持
								

								if(G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_STEP_PID_COOL
									|| G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_PID_COOL
									|| G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_AT
									|| G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_COOL_AT
									|| G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_AT_END
									|| G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_SET_BEEP
									)
								{
									if(m_CmdArray_128[(MeterID % G_CurMaxHeatBox)].GetSize() > 0) 
									{

										
										m_CmdArray_128[(MeterID % G_CurMaxHeatBox)].RemoveAt(0); 
									}
								}
							}
						}

						G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox] = cmd_128;
						

				}
			}


	}
}

void CWorkMode::TempCmd_32PT100(CWnd * ParentWnd,int MeterID) 
{
	
#ifndef WINCE

	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_WorkMode;




	G_bCMD_192_READ_STATUS = false;





	if(G_SendmCmdProgram.GetCount() > 0 )
	{

		CWnd *w;
		CString s;
#ifndef WINCE
		w= MainDlg->m_DebugDlg[G_ProgramBoxNr].GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE); 
#else
		w= G_DebugDlg.GetDlgItem(IDC_BUTTON_PROGRAM); 
#endif
		s.Format(_T("更新..%d"),G_SendmCmdProgram.GetCount());
		w->SetWindowTextW(s);
		
		MODBUS_RS485_CMD_PROGRAM cmd = G_SendmCmdProgram.GetAt(0); 

		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
		G_CommProtocolType = COMM_PROTOCOL_MODBUS_PROGRAM;

		if(cmd.para[0] == 0)
		{
			cmd.para[4 + FLASH_PARA_ADDR] = G_RS485_Comm.OSCCAL;
			cmd.crc		= CComm::CRC16((unsigned char*)&cmd,sizeof(cmd) - 2); 
		}

		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),0); 
#else
		//for(int i = 0; i < ::G_CurMaxHeatBox; i++)
		{
			DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[G_ProgramBoxNr],cmd.head.cmd,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),G_ProgramBoxNr); 
		}
#endif
		return;
	}

	MainDlg->SetTimer(1,G_NormalConfigPara[0].CommTimer,NULL);

	G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
	

	
	{ // MODBUS CMD

		MODBUS_RS485_CMD_32PT100 * cmd;

			if(m_CmdArray_32PT100[MeterID % G_CurMaxHeatBox].GetSize() == 0)
			{
							
				if(G_RS485_Comm.IsOpened())
				{
					
					{
#ifndef WINCE
						if(false)//((MainDlg->m_SwitchDlg[G_TakeoutDlg.m_BoxNr].m_WorkMode.m_SubMode == SUB_MODE_STOP ) && ((MeterID % G_CurMaxHeatBox)  == G_TakeoutDlg.m_BoxNr) && (::G_TakeoutDlg.GetSafeHwnd() != NULL) && (MainDlg->m_SwitchDlg[G_TakeoutDlg.m_BoxNr].m_IsTaking == true)) ||  (::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == HEAT_BOX_TYPE_HANJIE) )
						{
							G_bCMD_192_READ_STATUS = false;
							
							cmd = G_RS485_Comm.SendReadStatus_32PT100((MeterID % G_CurMaxHeatBox));
							//DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],cmd->head.cmd,(unsigned char*)cmd,sizeof(MODBUS_RS485_CMD_128),MeterID % G_CurMaxHeatBox); 
							static long last_ticket;
							CTime cur = CTime::GetCurrentTime();
							long t = ::GetTickCount();
							CString s;
							

							s.Format(_T("%d,%d,测试开始时间：%02d/%02d/%02d %02d:%02d:%02d  当前时间%d:%d:%d 通讯数量=%d 接收=%d  间隔:%d毫秒"),MeterID , G_CurMaxHeatBox, G_TakeoutDlg.m_BeginTestTime.GetYear(),G_TakeoutDlg.m_BeginTestTime.GetMonth(),G_TakeoutDlg.m_BeginTestTime.GetDay(),  G_TakeoutDlg.m_BeginTestTime.GetHour(),G_TakeoutDlg.m_BeginTestTime.GetMinute(),G_TakeoutDlg.m_BeginTestTime.GetSecond(), cur.GetHour(),cur.GetMinute(),cur.GetSecond(),G_MainDlg->m_DebugDlg[G_TakeoutDlg.m_BoxNr].m_SendCmdCount,::G_MainDlg->m_DebugDlg[G_TakeoutDlg.m_BoxNr].m_ReceiveCmdCount ,  t - last_ticket);
							last_ticket = t;
							if(G_TakeoutDlg.GetSafeHwnd() != NULL)
							{
								if(G_TakeoutDlg.IsWindowVisible())
								{
									G_TakeoutDlg.SetWindowTextW(s);
								}
							}

						}
						else
#endif 
						if(MeterID < G_CurMaxHeatBox)
						{
								cmd = G_RS485_Comm.SendReadTemp_32PT100(MeterID % G_CurMaxHeatBox);
						}
						else
						{
							
								cmd = G_RS485_Comm.SendReadStatus_32PT100(MeterID - G_CurMaxHeatBox);

						}
					}

				
				}

							if(G_RS485_Comm.IsOpened())
							{
				#ifdef WINCE
							DisplaySendCmd((CWnd*)&G_DebugDlg,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
				#else
							DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox] ,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
				#endif
							}

			}
			else //m_CmdArray_128[CurBoxNr].GetSize() != 0)

			{
				if(G_RS485_Comm.IsOpened())
				{
								
						G_IsJustCmdAscii = false;
						static MODBUS_RS485_CMD_32PT100 cmd_32PT100;
						cmd_32PT100 = m_CmdArray_32PT100[(MeterID % G_CurMaxHeatBox)].GetAt(0);  
						cmd = &cmd_32PT100;
						G_CommProtocolType = COMM_PROTOCOL_MODBUS_128;
						G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
						
						G_RS485_Comm.WriteComm((char*)&cmd_32PT100,sizeof(cmd_32PT100));


						DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],0,(unsigned char*)&cmd_32PT100,sizeof(cmd_32PT100),MeterID % G_CurMaxHeatBox); 
	

						if(G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].head.cmd == cmd_32PT100.head.cmd)
						{
							G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] ++;
							if(G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] > 2)
							{

								MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_TestMsg.Format(_T("重复命令 %02X,%02X,%02X,%d次,%s"),
									G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].head.cmd,
									G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].para[0],
									G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].para[1],  
									G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox],
									CComm::GetCmdString((COMMAND_ID)G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].head.cmd)); 
								MainDlg->UpdateData(false);

							}
							if(G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] >= 5)
							{//为了兼容老的线路板,下面这些命令不支持
								

								if(G_RS485_Comm.m_LastCmd_128[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_STEP_PID_COOL
									|| G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_PID_COOL
									|| G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_AT
									|| G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_COOL_AT
									|| G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_AT_END
									|| G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_SET_BEEP
									)
								{
									if(m_CmdArray_32PT100[(MeterID % G_CurMaxHeatBox)].GetSize() > 0) 
									{

										
										m_CmdArray_32PT100[(MeterID % G_CurMaxHeatBox)].RemoveAt(0); 
									}
								}
							}
						}

						G_RS485_Comm.m_LastCmd_32PT100[MeterID % G_CurMaxHeatBox] = cmd_32PT100;
						

				}
			}


	}
#endif;
}

void CWorkMode::TempCmd_300(CWnd * ParentWnd,int MeterID) 
{
	
#ifdef WINCE
	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &G_HeatBoxDlg.m_WorkMode;
#else
	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_WorkMode;
#endif;



	G_bCMD_192_READ_STATUS = false;



	

	if(G_SendmCmdProgram.GetCount() > 0)
	{
		MODBUS_RS485_CMD_PROGRAM cmd = G_SendmCmdProgram.GetAt(0); 

		G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
		G_CommProtocolType = COMM_PROTOCOL_MODBUS_PROGRAM;

		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),0); 
#else
		for(int i = 0; i < ::G_CurMaxHeatBox; i++)
		{
			DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[i],cmd.head.cmd,(unsigned char*)&cmd,sizeof(MODBUS_RS485_CMD_PROGRAM),i); 
		}
#endif
		return;
	}

	if(::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType ==  BOX_DISC_24 
		|| G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == BOX_DISC_16
		|| G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == BOX_DISC_40)
	{
		TempCmd_Disc(ParentWnd,MeterID);
		return;
	}



	{ // MODBUS CMD

		MODBUS_RS485_CMD_300 * cmd;

			if(m_CmdArray_300[MeterID % G_CurMaxHeatBox].GetSize() == 0)
			{
							
				if(G_RS485_Comm.IsOpened())
				{
					
					{
#ifndef WINCE
						if(((MainDlg->m_SwitchDlg[G_TakeoutDlg.m_BoxNr].m_WorkMode.m_SubMode == SUB_MODE_STOP ) && ((MeterID % G_CurMaxHeatBox)  == G_TakeoutDlg.m_BoxNr) && (::G_TakeoutDlg.GetSafeHwnd() != NULL) && (MainDlg->m_SwitchDlg[G_TakeoutDlg.m_BoxNr].m_IsTaking == true)) ||  (::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType == HEAT_BOX_TYPE_HANJIE) )
						{
							G_bCMD_192_READ_STATUS = false;
							
							cmd = G_RS485_Comm.SendReadStatus_300((MeterID % G_CurMaxHeatBox));
							//DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],cmd->head.cmd,(unsigned char*)cmd,sizeof(MODBUS_RS485_CMD_128),MeterID % G_CurMaxHeatBox); 
							static long last_ticket;
							CTime cur = CTime::GetCurrentTime();
							long t = ::GetTickCount();
							CString s;
							

							s.Format(_T("%d,%d,测试开始时间：%02d/%02d/%02d %02d:%02d:%02d  当前时间%d:%d:%d 通讯数量=%d 接收=%d  间隔:%d毫秒"),MeterID , G_CurMaxHeatBox, G_TakeoutDlg.m_BeginTestTime.GetYear(),G_TakeoutDlg.m_BeginTestTime.GetMonth(),G_TakeoutDlg.m_BeginTestTime.GetDay(),  G_TakeoutDlg.m_BeginTestTime.GetHour(),G_TakeoutDlg.m_BeginTestTime.GetMinute(),G_TakeoutDlg.m_BeginTestTime.GetSecond(), cur.GetHour(),cur.GetMinute(),cur.GetSecond(),G_MainDlg->m_DebugDlg[G_TakeoutDlg.m_BoxNr].m_SendCmdCount,::G_MainDlg->m_DebugDlg[G_TakeoutDlg.m_BoxNr].m_ReceiveCmdCount ,  t - last_ticket);
							last_ticket = t;
							if(G_TakeoutDlg.GetSafeHwnd() != NULL)
							{
								if(G_TakeoutDlg.IsWindowVisible())
								{
									G_TakeoutDlg.SetWindowTextW(s);
								}
							}

						}
						else
#endif 
						if(MeterID < G_CurMaxHeatBox)
						{
							
							cmd = G_RS485_Comm.SendReadTemp_300(MeterID % G_CurMaxHeatBox);

						}
						else
						{
							
							cmd = G_RS485_Comm.SendReadStatus_300(MeterID - G_CurMaxHeatBox);

						}
					}

				
				}


			}
			else //m_CmdArray_128[CurBoxNr].GetSize() != 0)

			{
				if(G_RS485_Comm.IsOpened())
				{
								
						G_IsJustCmdAscii = false;
						static MODBUS_RS485_CMD_300 cmd_300;
						cmd_300 = m_CmdArray_300[(MeterID % G_CurMaxHeatBox)].GetAt(0);  
						cmd = &cmd_300;
						G_CommProtocolType = COMM_PROTOCOL_MODBUS_300;
						
						G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
						
						G_RS485_Comm.WriteComm((char*)&cmd_300,sizeof(cmd_300));

						if(G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd == cmd_300.head.cmd)
						{

							G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] ++;
							
							if(G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] > 2)
							{
#ifdef WINCE
								G_HeatBoxDlg.m_TestMsg.Format(_T("重复命令 %02X,%02X,%02X,%d次,%s"),
									G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd,
									G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].para[0],
									G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].para[1],  
									G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox],
									CComm::GetCmdString((COMMAND_ID)G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd)); 
								G_HeatBoxDlg.UpdateData(false); 
#else
								MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_TestMsg.Format(_T("重复命令 %02X,%02X,%02X,%d次,%s"),
									G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd,
									G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].para[0],
									G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].para[1],  
									G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox],
									CComm::GetCmdString((COMMAND_ID)G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd)); 
								MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].UpdateData(false); 
#endif
							}
							
							if(G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] >= 5)
							{//为了兼容老的线路板,下面这些命令不支持
								if(G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_STEP_PID_COOL
									|| G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_PID_COOL
									|| G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_AT
									|| G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_COOL_AT
									|| G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_AT_END
									|| G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_SET_BEEP
									)
								{
									if(m_CmdArray_300[(MeterID % G_CurMaxHeatBox)].GetSize() > 0) 
									{
										m_CmdArray_300[(MeterID % G_CurMaxHeatBox)].RemoveAt(0); 
									}
								}
							}
						}

						G_RS485_Comm.m_LastCmd_300[MeterID % G_CurMaxHeatBox] = cmd_300;

				}
			}
			if(G_RS485_Comm.IsOpened())
			{
#ifdef WINCE
			DisplaySendCmd((CWnd*)&G_DebugDlg,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),(MeterID % G_CurMaxHeatBox));
#else
			DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox] ,cmd->head.cmd, (unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_300),(MeterID % G_CurMaxHeatBox));
#endif
			}

	}
}


void CWorkMode::TempCmd_192(CWnd * ParentWnd,int MeterID) 
{
#ifndef  WINCE

#ifdef WINCE
	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &G_HeatBoxDlg.m_WorkMode;
#else
	CHeatBoxDlg * MainDlg = (CHeatBoxDlg*)ParentWnd; 
	CWorkMode   * WorkMode = &MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_WorkMode;
#endif;

	G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
	 
	if(G_SendmCmdProgram.GetCount() > 0)
	{
		G_bCMD_192_READ_STATUS = false;

		MODBUS_RS485_CMD_PROGRAM cmd = G_SendmCmdProgram.GetAt(0); 
		
		::G_CommProtocolType = COMM_PROTOCOL_MODBUS_192;

		G_RS485_Comm.WriteComm((char*)&cmd,sizeof(cmd));
		for(int i = 0; i < ::G_CurMaxHeatBox; i++)
		{
#ifdef WINCE
		DisplaySendCmd((CWnd*)&G_DebugDlg,0,(unsigned char*)&cmd,sizeof(DELTA_FREQUNCE_CMD_ASC),0); 
#else
		DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],0,(unsigned char*)&cmd,sizeof(DELTA_FREQUNCE_CMD_ASC),0); 
#endif
		}
		return;
	}


	{ // MODBUS CMD
		MODBUS_RS485_CMD_192 cmd_192;
	
	
			if(m_CmdArray_192[MeterID % G_CurMaxHeatBox].GetSize() == 0)
			{
				if(G_RS485_Comm.IsOpened())
				{
					G_bCMD_192_READ_STATUS = false;

					if((MeterID < G_CurMaxHeatBox) && (MainDlg->m_SwitchDlg[MeterID % G_CurMaxHeatBox].m_IsTaking == false) && (::G_NormalConfigPara[MeterID % G_CurMaxHeatBox].BoxType != HEAT_BOX_TYPE_HANJIE))
					{
						G_bCMD_192_READ_STATUS = false;
						
						MODBUS_RS485_CMD_192 * cmd = G_RS485_Comm.SendReadTemp_192(&MainDlg->m_SwitchStatus[MeterID % G_CurMaxHeatBox][0],MeterID);
						DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],cmd->head.cmd,(unsigned char*)cmd,sizeof(MODBUS_RS485_CMD_192),(MeterID % G_CurMaxHeatBox)); 
	

					}
					else
					{
						G_bCMD_192_READ_STATUS = true;
						if(MeterID >= G_CurMaxHeatBox)
						{
							MODBUS_RS485_CMD_192 * cmd = (MODBUS_RS485_CMD_192*)G_RS485_Comm.SendReadStatus_192(MeterID - G_CurMaxHeatBox);
							DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],cmd->head.cmd,(unsigned char*)cmd,sizeof(MODBUS_RS485_CMD_192),(MeterID % G_CurMaxHeatBox)); 
	
						}
						else
						{
							MODBUS_RS485_CMD_192 * cmd = (MODBUS_RS485_CMD_192*)G_RS485_Comm.SendReadStatus_192(MeterID);
							DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],cmd->head.cmd,(unsigned char*)cmd,sizeof(MODBUS_RS485_CMD_192),(MeterID % G_CurMaxHeatBox)); 
	
						}
					}

					
				}
			}
			else
			{
				
				
				if(G_RS485_Comm.IsOpened())
				{
					/*if(G_CopyRight.IsCopyRightOK() == false)
					{
						WriteLogFile("CopyRight == false",m_MeterID % G_CurMaxHeatBox + 1);
					}
					else*/
					{
						
						
						G_IsJustCmdAscii = false;
						cmd_192 = m_CmdArray_192[MeterID % G_CurMaxHeatBox].GetAt(0);  
						G_RS485_Comm.SetBaudrate(G_RS485_Comm.m_hComm,9600,8,0,0);
						::G_CommProtocolType = COMM_PROTOCOL_MODBUS_192;

						G_RS485_Comm.WriteComm((char*)&cmd_192,sizeof(cmd_192));
						
						if(cmd_192.head.RS485_Addr >= 'A' && cmd_192.head.RS485_Addr <= 'Z')
						{
							G_bCMD_192_READ_STATUS = true;
						}
						else
						{
							G_bCMD_192_READ_STATUS = false;
						}

						DisplaySendCmd((CWnd*)&MainDlg->m_DebugDlg[MeterID % G_CurMaxHeatBox],cmd_192.head.cmd,(unsigned char*)&cmd_192,sizeof(MODBUS_RS485_CMD_192),(MeterID % G_CurMaxHeatBox)); 
	
						
					}
					

					if(G_RS485_Comm.m_LastCmd_192[MeterID % G_CurMaxHeatBox].head.cmd == cmd_192.head.cmd)
					{
						G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] ++;
						if(G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] > 2)
						{
#ifdef WINCE
							G_HeatBoxDlg.m_TestMsg.Format(_T("重复命令 %02X,%02X,%02X,%d次,%s"),
								G_RS485_Comm.m_LastCmd_192.cmd,
								G_RS485_Comm.m_LastCmd_192.para[0],
								G_RS485_Comm.m_LastCmd_192.para[1],  
								G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox],CComm::GetCmdString((COMMAND_ID)G_RS485_Comm.m_LastCmd_192.cmd)); 
							G_HeatBoxDlg.UpdateData(false); 
#else
#endif
						}
						if(G_RS485_Comm.repeat_counts[MeterID % G_CurMaxHeatBox] >= 5)
						{//为了兼容老的线路板,下面这些命令不支持
							if(G_RS485_Comm.m_LastCmd_192[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_STEP_PID_COOL
								|| G_RS485_Comm.m_LastCmd_192[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_PID_COOL
								|| G_RS485_Comm.m_LastCmd_192[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_AT
								|| G_RS485_Comm.m_LastCmd_192[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_COOL_AT
								|| G_RS485_Comm.m_LastCmd_192[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_AT_END
								|| G_RS485_Comm.m_LastCmd_192[MeterID % G_CurMaxHeatBox].head.cmd == (char)CMD_SET_BEEP
								)
							{
								if(m_CmdArray_192[(MeterID % G_CurMaxHeatBox)].GetSize() > 0) 
								{
									m_CmdArray_192[(MeterID % G_CurMaxHeatBox)].RemoveAt(0); 
								}
							}
						}
					}

					G_RS485_Comm.m_LastCmd_192[MeterID % G_CurMaxHeatBox] = cmd_192;
				}
			}
		
	}
#endif

}

bool CWorkMode::CheckTempStable(CWnd * ParentWnd,int BoxNr,float CurTemp	)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
	CCalDlg * DebugDlg    = &G_CalDlg; 
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
	CDebugDlg  * DebugDlg = &G_MainDlg->m_DebugDlg[BoxNr]; 
#endif

#define STABLE_SECONDS		90L

	DWORD tick = ::GetTickCount();
	double min_temp = 999.0f;
	double max_temp = -999.0f;
	int count = 0;
	for(int i = 0; i < 	m_PidTempArray.GetCount(); i++)
	{
		if(m_PidTempArray.GetAt(i).TimeTick > (tick - STABLE_SECONDS * 1000L)) //最近90秒
		{
			count ++;
			if(min_temp  > m_PidTempArray.GetAt(i).temp)
			{
				min_temp = m_PidTempArray.GetAt(i).temp;
			}

			if(max_temp  < m_PidTempArray.GetAt(i).temp)
			{
				max_temp = m_PidTempArray.GetAt(i).temp;
			}

		}
	}

	
#ifdef WINCE
	CString s;
	s.Format(_T("T%d %d %6.2f %6.2f %6.2f"),count,m_SubMode,CurTemp,max_temp,min_temp);
	DebugDlg->SetWindowTextW(s); 
#else
	DebugDlg->m_CalText.Format(_T("T%d %d %6.2f %6.2f %6.2f"),count,m_SubMode,CurTemp,max_temp,min_temp);
#endif

	float MaxGap = 0.3;
	if(::G_NormalConfigPara[BoxNr].InterfaceBoardType == LI_SIR_BOARD)
	{
		MaxGap = 0.6;
	}
	
	if((max_temp > min_temp) && ((max_temp - min_temp) < MaxGap) && (count > (STABLE_SECONDS / 4)))
	{

		DebugDlg->UpdateData(false);

		return true;
	}

	return false;
}

void CWorkMode::AutoTestStep_CalLow(CWnd * ParentWnd,int BoxNr,float CurTemp[4])
{
#ifdef WINCE
	CCalDlg * DebugDlg = (CCalDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
	CDebugDlg  * DebugDlg = &G_MainDlg->m_DebugDlg[BoxNr]; 
#endif

	float pid_temp = (CurTemp[0] + CurTemp[1] + CurTemp[2] + CurTemp[3]) / 4.0f;

	TEMP_TIME tt;
	tt.temp = pid_temp;
	tt.TimeTick = ::GetTickCount(); 
	if(m_PidTempArray.GetCount() > 1000)
	{
		m_PidTempArray.RemoveAt(0);
	}
	m_PidTempArray.Add(tt); 
#ifdef WINCE
	if(DebugDlg->m_List.GetCount() > 20)
	{
		DebugDlg->m_List.ResetContent();
	}
#endif
	
	if((pid_temp >= (m_CalLowPidTemp[BoxNr] - 2.0f)) && (pid_temp <= (m_CalLowPidTemp[BoxNr] + 2.0f)))
	{
		if(CheckTempStable(ParentWnd,BoxNr,pid_temp)) 
		{
			m_SubMode = SUB_MODE_CAL_INPUT_GLASS_TEMP;
			CWnd *w;
			CString	s;

			for(int i = 0; i < 4; i++)
			{
				w	=	DebugDlg->GetDlgItem(IDC_EDIT_PARA45+i);
				s.Format(_T("%f"),CurTemp[i]);
				w->SetWindowTextW(s);
				m_MeterLow[BoxNr][i] = CurTemp[i];  
			}
			
			DebugDlg->UpdateData(false);
			ParentWnd->KillTimer(1);

			InputGlassLowTemp(ParentWnd,BoxNr);

			ParentWnd->SetTimer(1,500,NULL);
		}
	}
}

void CWorkMode::AutoTestStep_CalHigh(CWnd * ParentWnd,int BoxNr,float CurTemp[4])
{
#ifdef WINCE
	CCalDlg * DebugDlg = (CCalDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
	CDebugDlg  * DebugDlg = &G_MainDlg->m_DebugDlg[BoxNr]; 
#endif



	float pid_temp = (CurTemp[0] + CurTemp[1] + CurTemp[2] + CurTemp[3]) / 4.0f;

	TEMP_TIME tt;
	tt.temp = pid_temp;
	tt.TimeTick = ::GetTickCount(); 
	if(m_PidTempArray.GetCount() > 1000)
	{
		m_PidTempArray.RemoveAt(0);
	}
	m_PidTempArray.Add(tt); 

	if((pid_temp >= (m_CalHighPidTemp[BoxNr] - 2.0f)) && (pid_temp <= (m_CalHighPidTemp[BoxNr] + 2.0f)))
	{
		if(CheckTempStable(ParentWnd,BoxNr,pid_temp)) 
		{
			m_SubMode = SUB_MODE_CAL_INPUT_GLASS_TEMP;

			CWnd *w;
			CString	s;

			for(int i = 0; i < 4; i++)
			{
				w	=	DebugDlg->GetDlgItem(IDC_EDIT_PARA49+i);
				s.Format(_T("%f"),CurTemp[i]);
				w->SetWindowTextW(s);
				m_MeterHigh[BoxNr][i] = CurTemp[i];  
			}
			//m_PidBeginTime = -1;
			DebugDlg->UpdateData(false);
			ParentWnd->KillTimer(1);
			InputGlassHighTemp(ParentWnd,BoxNr);
			ParentWnd->SetTimer(1,500,NULL);

			this->CalAdjustTemp( ParentWnd,BoxNr);

			m_SubMode = SUB_MODE_STOP;
		}
	}
}


void CWorkMode::InputGlassLowTemp(CWnd * ParentWnd,int BoxNr)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
	CCalDlg * DebugDlg    = &G_CalDlg; 
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
	CDebugDlg  * DebugDlg = &G_MainDlg->m_DebugDlg[BoxNr]; 
#endif

	char para[16];
	memset(para,0,sizeof(para));

	CInputGlassTempDlg dlg;


	CString s;
	s.Format(_T("%d#烘箱: "),BoxNr+1);


		dlg.m_Title = s;
		dlg.m_Title += _T("请输入4根水银温度计的度数");
//		m_CalMsg = (dlg.m_Title);
		dlg.m_Temp1 = "";
		dlg.m_Temp2 = "";
		dlg.m_Temp3 = "";
		dlg.m_Temp4 = "";
		
		CWnd * w;

		if(dlg.DoModal() == IDOK)
		{
			m_InputGlassLow[BoxNr][0] = _tcstod(dlg.m_Temp1,NULL);
			w = DebugDlg->GetDlgItem(IDC_EDIT_PARA53);
			w->SetWindowText(dlg.m_Temp1);

			m_InputGlassLow[BoxNr][1] = _tcstod(dlg.m_Temp2,NULL);
			w = DebugDlg->GetDlgItem(IDC_EDIT_PARA54);
			w->SetWindowText(dlg.m_Temp2);
			
			m_InputGlassLow[BoxNr][2] = _tcstod(dlg.m_Temp3,NULL);
			w = DebugDlg->GetDlgItem(IDC_EDIT_PARA55);
			w->SetWindowText(dlg.m_Temp3);
			
			m_InputGlassLow[BoxNr][3] = _tcstod(dlg.m_Temp4,NULL);
			w = DebugDlg->GetDlgItem(IDC_EDIT_PARA56);
			w->SetWindowText(dlg.m_Temp4);

		}
	
		G_RS485_Comm.SendNormalPidCmd(m_CalHighPidTemp[BoxNr], BoxNr);

		
		m_SubMode = SUB_MODE_CAL_HIGH;

}

void CWorkMode::InputGlassHighTemp(CWnd * ParentWnd,int BoxNr)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
	CCalDlg * DebugDlg    = &G_CalDlg; 
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
	CDebugDlg  * DebugDlg = &G_MainDlg->m_DebugDlg[BoxNr]; 
#endif

	char para[16];
	memset(para,0,sizeof(para));

	CInputGlassTempDlg dlg;
	
	CString s;
	s.Format(_T("   %d#烘箱: "),BoxNr+1);


		dlg.m_Title = s;
		dlg.m_Title += _T("请输入4根水银温度计的度数");
//		m_CalMsg = (dlg.m_Title);
		dlg.m_Temp1 = "";
		dlg.m_Temp2 = "";
		dlg.m_Temp3 = "";
		dlg.m_Temp4 = "";
		
		if(dlg.DoModal() == IDOK)
		{
			m_InputGlassHigh[BoxNr][0] = _tcstod(dlg.m_Temp1,NULL);
			CWnd * w = DebugDlg->GetDlgItem(IDC_EDIT_PARA57);
			w->SetWindowText(dlg.m_Temp1);

			m_InputGlassHigh[BoxNr][1] = _tcstod(dlg.m_Temp2,NULL);
			w = DebugDlg->GetDlgItem(IDC_EDIT_PARA58);
			w->SetWindowText(dlg.m_Temp2);
			
			m_InputGlassHigh[BoxNr][2] = _tcstod(dlg.m_Temp3,NULL);
			w = DebugDlg->GetDlgItem(IDC_EDIT_PARA59);
			w->SetWindowText(dlg.m_Temp3);
			
			m_InputGlassHigh[BoxNr][3] = _tcstod(dlg.m_Temp4,NULL);
			w = DebugDlg->GetDlgItem(IDC_EDIT_PARA60);
			w->SetWindowText(dlg.m_Temp4);

		}

		m_SubMode = SUB_MODE_STOP;
	
		CalAdjustTemp(ParentWnd,BoxNr);

		G_RS485_Comm.SendStopHeatCmd(0,BoxNr);

		
		

}

void CWorkMode::EnterCalMode(CWnd * ParentWnd,int BoxNr)
{
#ifdef WINCE
	CCalDlg * DebugDlg = (CCalDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd	= (CSwitchDlg*)ParentWnd;
	CDebugDlg  * DebugDlg	= &G_MainDlg->m_DebugDlg[BoxNr]; 
#endif
	

	if(m_SubMode == SUB_MODE_CAL_LOW
		|| m_SubMode == SUB_MODE_CAL_HIGH
		|| m_SubMode == SUB_MODE_CAL_INPUT_GLASS_TEMP)
	{
		if(AfxMessageBox(_T("重新开始校温吗？"),MB_OKCANCEL) == IDOK)
		{
			m_SubMode = SUB_MODE_CAL_LOW;
		}
		else
		{
			return;
		}

	}
	
	m_PidTempArray.RemoveAll(); 

	G_RS485_Comm.SendOpenDoorCmd(false,BoxNr);

	CInputDlg dlg;

		
	dlg.m_Title = _T("请输入校温密码");
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_Input == _T("1357")
			|| dlg.m_Input == _T("8598"))
		{
			for(int i = 0; i < 4; i++)
			{
				::G_NormalConfigPara[BoxNr].AdjustLow[i] = 0;
				::G_NormalConfigPara[BoxNr].AdjustHigh[i] = 0;
			}
			::SaveNormalConfigPara();
#ifdef WINCE
			CInputAutoAdjustTempDlg AdjustTempdlg;
			AdjustTempdlg.m_Temp1 = _T("60.0");
			AdjustTempdlg.m_Temp2 = _T("140.0");
			if(AdjustTempdlg.DoModal() != IDOK)
			{
				return;
			}

			m_CalLowPidTemp[BoxNr]		= _tcstod(AdjustTempdlg.m_Temp1,NULL);
			m_CalHighPidTemp[BoxNr]		= _tcstod(AdjustTempdlg.m_Temp2,NULL);

			//m_CalLowPidTemp[BoxNr]		= dlg.mDebugDlg->m_CalLowTemp; 
			//m_CalHighPidTemp[BoxNr]		= DebugDlg->m_CalHighTemp; 
			CString s;
			s.Format(_T("恒温到%5.1f度,%5.1f度"),m_CalLowPidTemp[BoxNr],m_CalHighPidTemp[BoxNr]);
			AfxMessageBox(s);

			DebugDlg->m_List.InsertString (0,_T("请在夹具帽子上插入4根水银温度计(能标记40--160度)"));
#else
			CInputAutoAdjustTempDlg AdjustTempdlg;
			AdjustTempdlg.m_Temp1 = _T("60.0");
			AdjustTempdlg.m_Temp2 = _T("140.0");
			if(AdjustTempdlg.DoModal() != IDOK)
			{
				return;
			}

			m_CalLowPidTemp[BoxNr]		= _tcstod(AdjustTempdlg.m_Temp1,NULL);
			m_CalHighPidTemp[BoxNr]		= _tcstod(AdjustTempdlg.m_Temp2,NULL);
			DebugDlg->m_CalMsg = (_T("请在夹具帽子上插入4根水银温度计(能标记40--160度)"));
#endif

			m_SubMode = SUB_MODE_CAL_LOW;
			
			
			G_RS485_Comm.SetAdjustAll(0.0f, BoxNr);
			G_RS485_Comm.ReadAdjustAll( BoxNr);
			
			float val[4] = {0,0,0,0};
			G_RS485_Comm.SetAdjustLow(val,BoxNr);

			G_RS485_Comm.ReadAdjustLow(BoxNr);

		
			G_RS485_Comm.SetAdjustHigh(val,BoxNr);

			G_RS485_Comm.ReadAdjustHigh(BoxNr);

			
			G_RS485_Comm.SendNormalPidCmd(m_CalLowPidTemp[BoxNr], BoxNr);

#ifdef WINCE
		
			s.Format(_T("恒温在 %5.1f度"),m_CalLowPidTemp[BoxNr]);
			DebugDlg->m_List.InsertString(0,s);
#else
			DebugDlg->m_CalMsg.Format(_T("恒温在 %5.1f度"),m_CalLowPidTemp[BoxNr]);
#endif

			DebugDlg->UpdateData(false);

			

		}
		else
		{
			AfxMessageBox(_T("校温密码错误"));
		}
	}

	
}

void CWorkMode::CalAdjustTemp(CWnd * ParentWnd,int BoxNr)
{
#ifdef WINCE
	CHeatBoxDlg * MainWnd		= (CHeatBoxDlg*)ParentWnd;
	CCalDlg		* DebugDlg		= &G_CalDlg; 
#else
	CSwitchDlg * MainWnd		= (CSwitchDlg*)ParentWnd;
	CDebugDlg  * DebugDlg		= &G_MainDlg->m_DebugDlg[BoxNr]; 
#endif

	int i;
	double glass_low_avr,glass_high_avr;
	CString s;
	CWnd *w;
	//记录一个低温点，一个高温点，根据线性方程求0,200度的误差
	//低温点一般取常温，高温点最好在150度，要分别恒温 
	/*s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_InputGlassLow[0] ,m_InputGlassLow[1] ,m_InputGlassLow[2] ,m_InputGlassLow[3]);
	AfxMessageBox(s);

	s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_InputGlassHigh[0] ,m_InputGlassHigh[1] ,m_InputGlassHigh[2] ,m_InputGlassHigh[3]);
	AfxMessageBox(s);

		s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_MeterLow[0] ,m_MeterLow[1] ,m_MeterLow[2] ,m_MeterLow[3]);
	AfxMessageBox(s);

		s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_MeterHigh[0] ,m_MeterHigh[1] ,m_MeterHigh[2] ,m_MeterHigh[3]);
	AfxMessageBox(s);
*/
	glass_low_avr	=(m_InputGlassLow[BoxNr][0] +m_InputGlassLow[BoxNr][1] +m_InputGlassLow[BoxNr][2] +m_InputGlassLow[BoxNr][3] )/4;
	glass_high_avr	=(m_InputGlassHigh[BoxNr][0]+m_InputGlassHigh[BoxNr][1]+m_InputGlassHigh[BoxNr][2]+m_InputGlassHigh[BoxNr][3])/4;

	/*
	m_MeterLow[0]  = 60;
	m_MeterHigh[0] = 140;
	glass_low_avr = 58;
	glass_high_avr = 130;
*/
	for(i=0;i<4;i++)
	{
		// y = kx + delta
		double delta_low	= (glass_low_avr - m_MeterLow[BoxNr][i]); 
		double delta_high	= (glass_high_avr- m_MeterHigh[BoxNr][i]);
		
		double k = (glass_high_avr-glass_low_avr)/(m_MeterHigh[BoxNr][i]-m_MeterLow[BoxNr][i]);
		
		double delta = glass_low_avr - k * m_MeterLow[BoxNr][i];
		

		m_WriteAdjustLow[BoxNr][i] = ((int)(delta * 10.0f + 0.5))/ 10.0f ;
		s.Format(_T("%5.1f"),m_WriteAdjustLow[BoxNr][i]); 	
		w =DebugDlg->GetDlgItem(IDC_EDIT_PARA29 + i);
		w->SetWindowTextW(s); 

		m_WriteAdjustHigh[BoxNr][i] = ((int)((200.0*k + delta) * 10.0f + 0.5))/10.0f - 200.0f;
		s.Format(_T("%5.1f"),m_WriteAdjustHigh[BoxNr][i]); 	
		CWnd *w =DebugDlg->GetDlgItem(IDC_EDIT_PARA33 + i);
		w->SetWindowTextW(s); 


	

	}

	for(i=0;i<4;i++)
	{
		G_NormalConfigPara[BoxNr].Cal_LowMeterTemp[i]		=	m_MeterLow[BoxNr][i];

		G_NormalConfigPara[BoxNr].Cal_HighMeterTemp[i]		=	m_MeterHigh[BoxNr][i];
		G_NormalConfigPara[BoxNr].Cal_LowStandardTemp[i]	=	m_InputGlassLow[BoxNr][i];
		G_NormalConfigPara[BoxNr].Cal_HighStandardTemp[i]	=	m_InputGlassHigh[BoxNr][i];
	}
	SaveNormalConfigPara();

	G_RS485_Comm.SetAdjustLow(m_WriteAdjustLow[BoxNr], BoxNr);
	G_RS485_Comm.ReadAdjustLow(BoxNr);
	G_RS485_Comm.ReadAdjustLow(BoxNr);

	G_RS485_Comm.SetAdjustHigh(m_WriteAdjustHigh[BoxNr], BoxNr);
	
	G_RS485_Comm.ReadAdjustHigh(BoxNr);
	G_RS485_Comm.ReadAdjustHigh(BoxNr);

	DebugDlg->UpdateData(false);

}


void CWorkMode::DealCmdAnswer(CWnd * ParentWnd,int BoxNr,unsigned char *para, unsigned char *flash,bool IsCmdAscii,int data_len)
{


#ifdef WINCE
	CHeatBoxDlg * MainWnd = (CHeatBoxDlg*)ParentWnd;
#else
	CSwitchDlg * MainWnd = (CSwitchDlg*)ParentWnd;
#endif

	CString s;
	CFile f;
	CString file_name;
	unsigned short crc;
	bool pcb_flash_status = false; 

	m_iTestMode = ::G_NormalConfigPara[BoxNr].TestMode;

	ASCII_RS485_COMMAND_READ_TEMP *ascii_cmd =(ASCII_RS485_COMMAND_READ_TEMP *) para;
	char ascii_status[(192 + 7) / 8];

	if(IsCmdAscii == true)
	{
		for(int i = 0; i < sizeof(ascii_status); i++)
		{
			ascii_status[i] = (ascii_cmd->val[RS485_PARA_LEN + i*2] - '0' ) *16
				+ (ascii_cmd->val[RS485_PARA_LEN + i*2 + 1] - '0' );
		}
	}
	MODBUS_RS485_32TEMP_ANSWER				*Answer_32TEMP				=	(MODBUS_RS485_32TEMP_ANSWER*)para;
	MODBUS_RS485_8TEMP_8SSR_ANSWER			*Answer_8TEMP_8SSR			=	(MODBUS_RS485_8TEMP_8SSR_ANSWER*)para;
	MODBUS_RS485_NTC_AD_VAL_ANSWER			*NTC_Answer					=	(MODBUS_RS485_NTC_AD_VAL_ANSWER*) para;
	MODBUS_RS485_CMD_128					*modbus_cmd_128				=	(MODBUS_RS485_CMD_128*) para;
	MODBUS_RS485_STATUS_FLASH_ANSWER_128	*modbus_status_answer_128	=	(MODBUS_RS485_STATUS_FLASH_ANSWER_128*) para;

	MODBUS_RS485_CMD_192					*modbus_cmd_192				=	(MODBUS_RS485_CMD_192*) para;
	MODBUS_RS485_STATUS_FLASH_ANSWER_192	*modbus_status_answer_192	=	(MODBUS_RS485_STATUS_FLASH_ANSWER_192*) para;

	MODBUS_RS485_DISC_ANSWER				*BimetalDisc				=	(MODBUS_RS485_DISC_ANSWER *)para;


	unsigned char cmd;
	
	if(IsCmdAscii == false)
	{
		cmd  = modbus_cmd_128->head.cmd;
	}
	else
	{
		cmd = ascii_cmd->header.command;
	}


	switch(cmd)
	{
//	case CMD_READ_USEFUL_SENSORS:
//		break;
		case CMD_READ_DISC_STATUS:
			{
				
				DealStatus_Disc(BoxNr,BimetalDisc, MainWnd->m_PidTemp); 
				MainWnd->DrawBiMetalLED(); 
			}
			break;
		case CMD_SET_BEEP:
			//AfxMessageBox(_T("CMD_SET_BEEP"));
		case CMD_SET_RELAY:
		case CMD_AIR_CTL:
		case CMD_READ_TEMP:
			
			
			G_Meter[BoxNr].m_MeterAnswerTime		= CTime::GetCurrentTime();
			G_Meter[BoxNr].m_LedAnswerTime[0]		= CTime::GetCurrentTime();
			if(IsCmdAscii == false)
			{
				if(::G_NormalConfigPara[BoxNr].BoxType == BOX_SENSATA_32PT100)
				{
					MODBUS_RS485_32TEMP_ANSWER answer_para;
					memcpy(&answer_para,modbus_cmd_128,sizeof(answer_para));
					CRect rect;
					MainWnd->GetClientRect(&rect);
					rect.left += 5;
					rect.right -= 2;
					rect.bottom = 85;
					G_Meter[BoxNr].Init(MainWnd->GetSafeHwnd() ,MainWnd->GetWindowDC(),rect,answer_para);
					G_Meter[BoxNr].DrawMeter(BoxNr,answer_para);
					break;

				}
				else if(::G_NormalConfigPara[BoxNr].BoxType == BOX_TEMP2_10IN_12OUT)
				{
					MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT answer_para;
					memcpy(&answer_para,modbus_cmd_128->para,sizeof(answer_para));
					CRect rect;
					MainWnd->GetClientRect(&rect);
					rect.left += 5;
					rect.right -= 2;
					rect.bottom = 85;
					G_Meter[BoxNr].Init(MainWnd->GetSafeHwnd() ,MainWnd->GetWindowDC(),rect,answer_para);
					G_Meter[BoxNr].DrawMeter(BoxNr,answer_para);

				}
				else if(data_len == sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER))
				{

					memset(&MainWnd->m_TempPara,0,sizeof(MainWnd->m_TempPara));

					for(int i = 0; i < 4; i++)
					{
						MainWnd->m_CurTemp[i]		= Answer_8TEMP_8SSR->Pv[i] / 100.0f;
						MainWnd->m_TempPara.Temp[i] = Answer_8TEMP_8SSR->Pv[i];
						
					}
#ifdef WINCE
					
					MainWnd->Draw_PV_SV(Answer_8TEMP_8SSR);
#else
					CRect rect;
					MainWnd->GetClientRect(&rect);
					rect.left += 5;
					rect.right -= 2;
					rect.bottom = 85;

					

					G_Meter[BoxNr].Init(MainWnd->GetSafeHwnd() ,MainWnd->GetWindowDC(),rect,*Answer_8TEMP_8SSR);
					G_Meter[BoxNr].DrawMeter(BoxNr,*Answer_8TEMP_8SSR);
#endif
					break;
				}
				else
				{
					memcpy(&MainWnd->m_TempPara,modbus_cmd_128->para,sizeof(MainWnd->m_TempPara));
					if(MainWnd->m_TempPara.TempDecimalPlaces == 1)
					{
						MainWnd->m_CurTemp[0] = MainWnd->m_TempPara.Temp[0] / 10.0f;
						MainWnd->m_CurTemp[1] = MainWnd->m_TempPara.Temp[1] / 10.0f;
						MainWnd->m_CurTemp[2] = MainWnd->m_TempPara.Temp[2] / 10.0f;
						MainWnd->m_CurTemp[3] = MainWnd->m_TempPara.Temp[3] / 10.0f;
					}
					else
					{
						MainWnd->m_CurTemp[0] = MainWnd->m_TempPara.Temp[0] / 100.0f;
						MainWnd->m_CurTemp[1] = MainWnd->m_TempPara.Temp[1] / 100.0f;
						MainWnd->m_CurTemp[2] = MainWnd->m_TempPara.Temp[2] / 100.0f;
						MainWnd->m_CurTemp[3] = MainWnd->m_TempPara.Temp[3] / 100.0f;
					}

					
					
				}

				if(::G_iCmdAscii[BoxNr] == 2)//OIL
				{
					MainWnd->m_CurTemp[1] = MainWnd->m_CurTemp[0];
					MainWnd->m_CurTemp[2] = MainWnd->m_CurTemp[0];
					MainWnd->m_CurTemp[3] = MainWnd->m_CurTemp[0];
				}
	
			}
			else
			{
				memset(&MainWnd->m_TempPara,0,sizeof(MainWnd->m_TempPara));
				
				{
					for(int i = 0; i < 4; i++)
					{
						MainWnd->m_CurTemp[i] = (ascii_cmd->val[i*4+1] - '0' ) * 10 + (ascii_cmd->val[i*4+2] - '0' )  + (ascii_cmd->val[i*4+3] - '0' ) * 0.1f;
						if(ascii_cmd->val[i*4+0] == '_')
						{
							MainWnd->m_CurTemp[i] = 0 - MainWnd->m_CurTemp[i];
						}
						MainWnd->m_TempPara.Temp[i] = MainWnd->m_CurTemp[i]*100.0f;
					}
				}
			}

			MainWnd->m_PidTemp = (MainWnd->m_CurTemp[0] +MainWnd-> m_CurTemp[1] + MainWnd->m_CurTemp[2] + MainWnd->m_CurTemp[3] ) / 4.0f;  //平均，和四舍五入


#ifdef WINCE
			MainWnd->UpdateData(false);
			MainWnd->Draw_PV_SV();
#else

				{
					CRect rect;
					MainWnd->GetClientRect(&rect);
					rect.left += 5;
					rect.right -= 2;
					rect.bottom = 85;

					G_Meter[BoxNr].Init(MainWnd->GetSafeHwnd() ,MainWnd->GetWindowDC(),rect,MainWnd->m_TempPara);
					G_Meter[BoxNr].DrawMeter(BoxNr);
				}
#endif
				//	m_Meter.Draw(m_TempPara);
			if(IsCmdAscii == false)
			{
				break;	
			}
			
	
		case CMD_READ_STATUS:

			{
					CRect rect;
					MainWnd->GetClientRect(&rect);
					rect.left += 5;
					rect.right -= 2;
					rect.bottom = 85;

					G_Meter[BoxNr].Init(MainWnd->GetSafeHwnd() ,MainWnd->GetWindowDC(),rect,MainWnd->m_TempPara);
					G_Meter[BoxNr].DrawMeter(BoxNr);
		   }
			
			if(IsCmdAscii == false)
			{
				G_Meter[BoxNr].m_LedAnswerTime[0] = CTime::GetCurrentTime();
			}
		
			if(IsCmdAscii == false)
			{
				int size;
				if(G_NormalConfigPara[BoxNr].BoxType == BOX_NTC)
				{
				
					short v;
					for(int i = 0; i < 64; i++)
					{
						v = 0;
						memcpy(&v,&NTC_Answer->para[i * 2],2);
						//x/(10+x) = v/4096
						//x = (v/4096)*(10 + x)
						//(1 - v / 4096)x = 10*v/4096
						//x = 10*v/4096 /(1 - v / 4096) 
						if(v > 4000)
						{
							::G_SwitchTestResult[BoxNr][i].OpenTemp = -1;
						}
						else
						{
							if(G_NormalConfigPara[BoxNr].SwitchType == 0)
							{
								::G_SwitchTestResult[BoxNr][i].OpenTemp = 10.0f*v/4096.0f /(1 - v / 4096.0f);
								//::G_SwitchTestResult[BoxNr][i].OpenTemp = ::G_SwitchTestResult[BoxNr][i].OpenTemp * 0.8 + 10.0f*v/4096.0f /(1 - v / 4096.0f) * 0.2;
							}
							else
							{
								::G_SwitchTestResult[BoxNr][i].OpenTemp = 100.0f*v/4096.0f /(1 - v / 4096.0f);
							}
						}
					}
				}
				else
				{
					if(G_iMaxLedNr[BoxNr] == 192)
					{
						size = sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_192);
						crc = CComm::CRC16((unsigned char *)para, size - 2); 
						if(crc == modbus_status_answer_192->crc)
						{

						}
						else
						{
							break;
						}
					}
					else
					{
						size = sizeof(MODBUS_RS485_STATUS_FLASH_ANSWER_128);
						
						if(CComm::CRC16((unsigned char *)para, size - 2) == modbus_status_answer_128->crc 
							|| CComm::CalCheckSum_OIL((unsigned char *)para, size - 2) == modbus_status_answer_128->crc )
						{

						}
						else
						{
							break;
						}

					}
				}
			
				if( BOX_DESK_OVEN == G_NormalConfigPara[BoxNr].BoxType)
				{
					if(modbus_status_answer_128->head.para_len != 128 / 8 *2)
					{
						break;
					}
				}
				else
				{
					if(modbus_status_answer_128->head.para_len != G_iMaxLedNr[BoxNr] / 8 *2)
					{
						break;
					}
				}
			}
		
			int iMaxLedNr = G_iMaxLedNr[BoxNr];

			if(::G_NormalConfigPara[BoxNr].InterfaceBoardType ==   DESK_OVEN_BOARD)
			{
				iMaxLedNr = 16;
				for(int i = 16 ; i < G_iMaxLedNr[BoxNr]; i++)
				{
					G_SwitchTestResult[BoxNr][i].IsClosing = false;  
				}
			}
	
			
			for(int i = 0; i <  iMaxLedNr ; i++)
			{
					if(G_iMaxLedNr[BoxNr] == 192)
					{
							if((modbus_status_answer_192->para[i/8] & ( 0x01 << (i % 8))) != 0)
							{
								G_SwitchTestResult[BoxNr][i].IsClosing = false;   
							}
							else
							{
								G_SwitchTestResult[BoxNr][i].IsClosing = true;   
							}

					}
					else //(G_iMaxLedNr[m_BoxNr] != 192)
					{
						if(IsCmdAscii == false)
						{
#ifndef WINCE
							if((G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_PTC) && (::G_MainDlg->m_DebugDlg[BoxNr].m_bJiajuTesting == true))
							{
								if((modbus_status_answer_128->para[i/8 + 16] & (0x80 >> (i % 8))) != 0)
								{
									G_SwitchTestResult[BoxNr][i].IsClosing = false;   
								}
								else
								{
									G_SwitchTestResult[BoxNr][i].IsClosing = true;   
								}
							}
							else if( G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_PTC_NO_TEST)
							{
								if((modbus_status_answer_128->para[i/8] & (0x80 >> (i % 8))) != 0)
								{
									G_SwitchTestResult[BoxNr][i].IsClosing = false;   
								}
								else
								{
									G_SwitchTestResult[BoxNr][i].IsClosing = true;   
								}
							}
							else
#endif
							{
								if((modbus_status_answer_128->para[i/8] & (0x80 >> (i % 8))) != 0)
								{
									G_SwitchTestResult[BoxNr][i].IsClosing = false;   
								}
								else
								{
									G_SwitchTestResult[BoxNr][i].IsClosing = true;   
								}
							}
							if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
							{
								if((modbus_status_answer_128->para[(int)(i/8) + (int)((G_iMaxLedNr[BoxNr]+7)/8)] & ( 0x80 >> (i % 8))) != 0)
								{
									pcb_flash_status = true;
								}
								else
								{
									pcb_flash_status = false;
								}
							}
							else
							{
								if((G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_PTC) || (G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_ANTI_PTC))
								{
									

									if((modbus_status_answer_128->para[(int)(i/8) + (int)((G_iMaxLedNr[BoxNr]+7)/8)] & ( 0x80 >> (i % 8))) != 0)
									{//闪动或者有无PTC
										if(G_SwitchTestResult[BoxNr][i].IsClosing == false && this->m_SubMode != SUB_MODE_STOP)
										{
											pcb_flash_status = true;
											G_SwitchTestResult[BoxNr][i].IsNoPTC = true;
										}
									}
									else
									{
										pcb_flash_status = false;
										if(G_SwitchTestResult[BoxNr][i].IsClosing == false)
										{
											G_SwitchTestResult[BoxNr][i].IsNoPTC = false;
										}
									}
			
								}
								else if(/*G_NormalConfigPara[BoxNr].BoxType == BOX_128 && */G_NormalConfigPara[BoxNr].InterfaceBoardType == HIGH_VOLT_BOARD)
								{
									if((modbus_status_answer_128->para[(int)(i/8) + (int)((G_iMaxLedNr[BoxNr]+7)/8)] & ( 0x80 >> (i % 8))) != 0)
									{//闪动
										/*if(MainWnd->IsWindowVisible())
										{
											MainWnd->m_TestMsg.Format(_T("pcb_flash_status= %d"),i+1);
											MainWnd->UpdateData(false);
										}*/
										pcb_flash_status = true;
									}
									else
									{
										pcb_flash_status = false;
									}
								}
								else
								{
									pcb_flash_status = false;
								}
								
							}
						}
						else
						{
							if((ascii_status[i/8] & (0x80 >> (i % 8))) != 0)
							{
								G_SwitchTestResult[BoxNr][i].IsClosing = false;   
							}
							else
							{
								G_SwitchTestResult[BoxNr][i].IsClosing = true;   
							}

							
						}	
							
					}
			
					if(m_SubMode != SUB_MODE_STOP)
					{
						
						if(G_NormalConfigPara[BoxNr].SwitchType == 0)
						{//常闭
							//DealStatusNormalClose(&G_SwitchTestResult[m_BoxNr][i],pcb_flash_status,i);
							DealStatusNormalClose(ParentWnd,BoxNr,i,&G_SwitchTestResult[BoxNr][i],pcb_flash_status, MainWnd->m_PidTemp,G_Meter[BoxNr].m_IsDoorOpened ,::G_NormalConfigPara[BoxNr].TestMode,false);   
						}
						else
						{
							//DealStatusNormalOpen(&G_SwitchTestResult[m_BoxNr][i],pcb_flash_status,i);
							DealStatusNormalOpen(BoxNr,i,&G_SwitchTestResult[BoxNr][i],pcb_flash_status,MainWnd->m_PidTemp,G_Meter[BoxNr].m_IsDoorOpened ,::G_NormalConfigPara[BoxNr].TestMode);   
						}
					}

			}
		
					
					
#ifndef WINCE				
				if(G_TakeoutDlg.GetSafeHwnd() != NULL)
				{
					if( G_TakeoutDlg.IsWindowVisible())
					{
						if(BoxNr == G_TakeoutDlg.m_BoxNr)
						{
							for(int i = 0; i < G_iMaxLedNr[BoxNr]; i++)
							{
								memcpy(&G_TakeDlgSwitchTestResult[0][i], &G_SwitchTestResult[BoxNr][i], sizeof(G_TakeDlgSwitchTestResult[0][i]));
							}

							G_TakeoutDlg.ShowTestResult(true); 
						}
					}
				}
#endif
				if(G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_24 
					|| G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_16
					|| G_NormalConfigPara[BoxNr].BoxType == BOX_DISC_40)
				{
					MainWnd->DrawBiMetalLED(); 
				}
#ifdef WINCE
				if(G_TakeoutDlg.IsWindowVisible() && (G_TakeoutDlg.m_bHistory == false))
				{

					memcpy(G_TakeoutDlg.m_SwitchTestResult, &G_SwitchTestResult[0], sizeof(G_TakeoutDlg.m_SwitchTestResult));
					if(G_TakeoutDlg.m_ShowDetailType == 0)
					{

						G_TakeoutDlg.ShowTestResult(true); 
					}
				}
#endif
#ifndef WINCE
				else if(G_NormalConfigPara[BoxNr].BoxType == HEAT_BOX_TYPE_128_OLD_DISPLAY)
				{
					MainWnd->DrawLED_128_OLD_DISPLAY();
				}
#endif
				else
				{
					MainWnd->DrawLED();
				}
				
		
		break;


	}

}
