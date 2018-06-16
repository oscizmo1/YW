#include "stdafx.h"
#include "StateMoveCZone.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStateMoveCZone::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{

	case stepStart:
		if(theConfigBank.m_System.m_bInlineMode)
		{
			if(PDT_IF_ArmStatus_Check(m_Shuttle))
				m_bRtn[0] = FALSE;
		}
		m_bRtn[2] = LightCurtain_Check(m_Shuttle);
		m_bRtn[3] = Inspection_Z_UP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);
			Shuttle_Y_MANUAL_Move(m_Shuttle);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 300.)
		{
			if(m_bRtn[0] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_PDT_ROBOT_STOP, m_Shuttle));
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			else if(m_bRtn[2] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			//kjpark 20170913 MCR 위치에서 Z축  체크
			else if(m_bRtn[3] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1_NOT_UP_POSITION, m_Shuttle));
		}
		break;
	case stepShuttleY_CZone_Check:
		m_bRtn[3] = Shuttle_Y_MANUAL_Check(m_Shuttle);

		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepShuttleY_CZone_Check"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 120.)
		{
			if(m_bRtn[0] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH1, m_Shuttle));
			else if(m_bRtn[1] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH2, m_Shuttle));
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			else if(m_bRtn[2] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[3] == FALSE)	
			{
				if(PDT_IF_ArmStatus_Check(m_Shuttle) == FALSE)
					SetError(GetAlarmID_of_Shuttle(ALM_AXIS_JIG_SHUTTLE_Y1, m_Shuttle));
				else
				{
					theLog[m_LogIndex].AddBuf(_T("[%s] Shuttle_Y_LOAD_Move And Robot Status ON = Interlock"), m_strStateName);
					m_Timer.Start();
					nStep = stepStart;
				}
			}
		}
		break;


	case stepEnd:

		theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
		//ResetZoneEnd(m_Shuttle);	
		// A존 도착 시점 ~ 다음 A존 도착 시점
		CellLog_SetUnloadTime(m_Shuttle);

		CellLog_TactTime_SetEndTime(m_Shuttle);

		theTactTimeLog.m_Shuttle_MoveAZone[m_Shuttle].End();
		theTactTimeLog.m_RunTime[m_Shuttle].End();
		BZonetoAZone_SetTimeEnd(m_Shuttle);

		CellLog_WaitTime_SetStartTime(m_Shuttle);

		SetZoneEnd(m_Shuttle, ZONE_ID_MOVE_C);
		if(theConfigBank.m_System.m_bInlineMode == FALSE)
		{
			ResetZoneEnd(m_Shuttle, ZONE_ID_MOVE_A);				
			ResetZoneEnd(m_Shuttle, ZONE_ID_MOVE_B);				
			ResetZoneEnd(m_Shuttle, ZONE_ID_A);				
			ResetZoneEnd(m_Shuttle, ZONE_ID_B);				
			ResetZoneEnd(m_Shuttle, ZONE_ID_C);				
		}

		nStep = stepIdle;
		break;
	}

	if(m_nStep != stepIdle)
		m_nStep = nStep;

	return nStep;
}
