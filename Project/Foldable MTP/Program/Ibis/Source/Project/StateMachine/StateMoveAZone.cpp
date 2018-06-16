#include "stdafx.h"
#include "StateMoveAZone.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStateMoveAZone::Run()
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
	case stepShuttleMove:
		if(GetZoneEnd(m_Shuttle, ZONE_ID_A))
		{
			m_bRtn[0] = CellTagExist(m_Shuttle, JIG_CH_1) ? Shuttle_Vac_Check(m_Shuttle, JIG_CH_1, VAC_ON):TRUE;
		}
		m_bRtn[2] = LightCurtain_Check(m_Shuttle);
		m_bRtn[3] = Inspection_Z_UP_Check(m_Shuttle);
		
	//kjpark 20180131  Robot Status interlock 회피
		if(IsReturnOk() /*&& m_Timer.Stop(FALSE) > 1.*/)
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepShuttleMove"), m_strStateName);
			if(PDT_IF_ArmStatus_Check(m_Shuttle))
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] PDT_IF_ArmStatus_Check In Shuttle"), m_strStateName);
			}
			else
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] PDT_IF_ArmStatus_Check Out Shuttle"), m_strStateName);				
				//kjpark 20180131 Robot Status interlock 회피 더 추가
				if(Shuttle_Y_LOAD_Move(m_Shuttle) == FALSE)
				{
					theLog[m_LogIndex].AddBuf(_T("[%s] PDT_IF_ArmStatus_Check Shuttle_Y_LOAD_Move In Shuttle"), m_strStateName);
				}
				else
				{
					theTactTimeLog.m_Shuttle_MoveAZone[m_Shuttle].Start();
					nStep++;
				}
				
			}
			m_Timer.Start();
			
		}
		else if(m_Timer.Stop(FALSE) > 30.)
		{
			Inspection_Z_UP_Check(m_Shuttle);
			if(m_bRtn[0] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH1, m_Shuttle));
			else if(m_bRtn[1] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH2, m_Shuttle));
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			else if(m_bRtn[2] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[3] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1, m_Shuttle));
		}
		break;
	
	//20170913 hhkim C->A zone
	case stepShuttleY_AZone_Check:
		m_bRtn[3] = Shuttle_Y_LOAD_Check(m_Shuttle);

		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepShuttleY_AZone_Check"), m_strStateName);
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

		if(theConfigBank.m_System.m_bInlineMode)
		{
			CellLog_WaitTime_SetStartTime(m_Shuttle);

			// Inline에서는 ZONE_A를 여기서 날린다 [10/6/2017 OSC]
			if(theProcBank.m_bIsSetZone[m_Shuttle][ZONE_ID_A] == FALSE)
			{
				theProcBank.m_bIsSetZone[m_Shuttle][ZONE_ID_A] = TRUE;

// 				//20170913 hhkim Track Out
// 				CIM_CellTrackOut(m_Shuttle, JIG_CH_1);
// 				CIM_CellTrackOut(m_Shuttle, JIG_CH_2);
// 
// 				CellLog_Write(m_Shuttle);

				SetZoneA_Send(m_Shuttle);
			}
		}

		theTactTimeLog.m_Shuttle_MoveAZone[m_Shuttle].End();
		theTactTimeLog.m_RunTime[m_Shuttle].End();
		BZonetoAZone_SetTimeEnd(m_Shuttle);

		CellLog_WaitTime_SetStartTime(m_Shuttle);

		SetZoneEnd(m_Shuttle, ZONE_ID_MOVE_A);
		ResetZoneEnd(m_Shuttle, ZONE_ID_MOVE_B);				
		ResetZoneEnd(m_Shuttle, ZONE_ID_MOVE_C);				
		ResetZoneEnd(m_Shuttle, ZONE_ID_A);				
		ResetZoneEnd(m_Shuttle, ZONE_ID_B);				
		ResetZoneEnd(m_Shuttle, ZONE_ID_C);				

		nStep = stepIdle;
		break;
	}

	if(m_nStep != stepIdle)
		m_nStep = nStep;

	return nStep;
}
