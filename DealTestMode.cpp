
#include "stdafx.h"
#include "DealTestMode.h"


CDealTestMode::CDealTestMode(void)
{
	
}
CDealTestMode::~CDealTestMode(void)
{

}

void CDealTestMode::AuotTestStep_NormalPID(void)
{

	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;


	cur_t	=	CTime::GetCurrentTime(); 
	span	=	cur_t - m_CurSubModeBeginTime;
	
	if(IsAllOpened())
	{
		this->ExitAutoTest();
	}
	//如果时间已到，就进入下一步
	else if(span.GetMinutes() >= G_NormalConfigPara[m_BoxNr].PidTime)
	{
		if(m_iTestMode == TEST_MODE_NORMAL_PID_CAL_OPEN_TIME)
		{
			this->ExitAutoTest();
		}
		else
		{
			m_OpenDoorIng	= false;

			m_OpenDoor_Temp	= GetControlGoalTemp(SUB_MODE_PID) + ::G_NormalConfigPara[m_BoxNr].OpenDoorReset_HeatDegree; 

			m_WorkStatus.SubMode = SUB_MODE_STEP_PID;

			m_CurSubModeBeginTime = m_CurSubModeBeginTime.GetCurrentTime(); 
			 
			int test_minutes = (GetControlGoalTemp(SUB_MODE_STEP_PID) - GetControlGoalTemp(SUB_MODE_PID))* 10 / G_NormalConfigPara[m_BoxNr].HeatSpeed;
			::G_Comm.SendStepPIDCmd(  GetControlGoalTemp(SUB_MODE_STEP_PID),test_minutes,m_BoxNr);
		}
		
	}
	else
	{
		if(m_iTestMode == TEST_MODE_NORMAL_PID_CAL_OPEN_TIME)
		{
			if((m_PID_Temp + 0.2f) >= GetControlGoalTemp(SUB_MODE_PID))
			{//开始计时
				if(m_CurSubModeBeginTime == 0)
				{
					m_CurSubModeBeginTime = CTime::GetCurrentTime();
				}
			}
		}

		int RemainTime = (G_NormalConfigPara[m_BoxNr].PidTime - span.GetMinutes())  * 60 - span.GetSeconds(); 

		this->m_TestMsg.Format(_T("恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_PID) ,(int)(RemainTime/60),(int)(RemainTime%60));
		this->UpdateData(false);
	}
}