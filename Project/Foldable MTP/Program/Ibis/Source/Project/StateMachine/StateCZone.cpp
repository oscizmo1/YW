#include "stdafx.h"
#include "StateCZone.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStateCZone::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	BOOL bNext = TRUE;

	switch(nStep)
	{
	case stepStart:
		//m_bRtn[1] = Inspection_Z_UP_Check(m_Shuttle);
		//kjpark 20180122 Y Shuttle 정지 후 다시 런하면 알람치는 시퀀스 수정
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);
			Shuttle_Y_INSP_Move(m_Shuttle);

			if(theProcBank.m_bIsSetZone[m_Shuttle][ZONE_ID_C] == FALSE)
			{
				theProcBank.m_bIsSetZone[m_Shuttle][ZONE_ID_C] = TRUE;
				MCRtoCZoneSetTimeEnd(m_Shuttle);
				CZone_SetTimeStart(m_Shuttle);
				CellLog_CZoneMTPReadyTime_SetStartTime(m_Shuttle);
				SetZoneC_Send(m_Shuttle);
			}

			m_Timer.Start();
			nStep++;
		}
		break;
	//kjpark 20180122 Y Shuttle 정지 후 다시 런하면 알람치는 시퀀스 수정
	case stepY_MoveCheck:
		m_bRtn[0] = LightCurtain_Check(m_Shuttle);
		m_bRtn[1] = Shuttle_Y_INSP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			// State Name을 셔틀 이름 포함해서 다시 지정한다 [9/19/2017 OSC]
			theLog[m_LogIndex].AddBuf(_T("[%s] stepY_MoveCheck"), m_strStateName);
			
			Inspection_X_INSP_Move(m_Shuttle);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			if(m_bRtn[0] == FALSE)			SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[1] == FALSE)		SetError(GetAlarmID_of_Shuttle(ALM_AXIS_JIG_SHUTTLE_Y1, m_Shuttle));
		}
		break;
		//kjpark 20170925 MTP WRITE 할때 X축 동작 
	case stepX_INSPCheck:
		m_bRtn[0] = LightCurtain_Check(m_Shuttle);
		m_bRtn[1] = Inspection_X_INSP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			Inspection_Z_INSP_Move(m_Shuttle);
			theTactTimeLog.m_InspectionZ_Down[m_Shuttle].Start();
			theLog[m_LogIndex].AddBuf(_T("[%s] stepX_INSPCheck"), m_strStateName);
			m_Timer.Start();		
			nStep++;
		}	
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			if(m_bRtn[0] == FALSE)			SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[1] == FALSE)		SetError(GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_X1, m_Shuttle));
		}
		break;
	case stepZ_DownChk:		
		m_bRtn[0] = LightCurtain_Check(m_Shuttle);
		m_bRtn[1] = Inspection_Z_INSP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepZ_DownChk"), m_strStateName);
			theTactTimeLog.m_InspectionZ_Down[m_Shuttle].End();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			if(m_bRtn[0] == FALSE)			SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[1] == FALSE)		SetError(GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1, m_Shuttle));
		}
		break;

		// ZONE_A 체크 [9/11/2017 OSC]
	case stepZoneCCheck:
		m_bRtn[0] = SetZoneC_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepZoneCCheck"), m_strStateName);
			CellLog_CZoneMTPReadyTime_SetEndTime(m_Shuttle);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetZoneC_TimeOut(m_Shuttle);
		}
		break;

	case stepBefStart:
		// Start()함수가 호출 될 때마다 m_strCurrentModuleName가 INSP_MODULE_NONE로 리셋되기 때문에 
		// 이전 진행하던건 무시되고 처음부터 다시 시작된다 [8/30/2017 OSC]
		if( m_pCurrentModule && (m_strCurrentModuleName != INSP_MODULE_NONE) )
		{
			m_pCurrentModule->Run();
			bNext = m_pCurrentModule->IsStoped();
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_CZone_Bef(m_strCurrentModuleName, m_Shuttle);
			if(m_pCurrentModule)
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepBefStart Module %s -> %s"), m_strStateName, m_strCurrentModuleName, m_pCurrentModule->m_ModuleName);
				m_strCurrentModuleName = m_pCurrentModule->m_ModuleName;
				// SetParam 안에서 Reset이 호출된다 [8/30/2017 OSC]
				m_pCurrentModule->SetParam(m_strStateName, m_nThreadID, FALSE, m_LogIndex);
				m_pCurrentModule->SetShuttle(m_Shuttle);
				m_pCurrentModule->Start();
			}
			else
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepBefStart Complete"), m_strStateName);
				m_strCurrentModuleName = INSP_MODULE_NONE;
				nStep++;
			}
		}
		break;

	case stepMustStart:
		// Start()함수가 호출 될 때마다 m_strCurrentModuleName가 INSP_MODULE_NONE로 리셋되기 때문에 
		// 이전 진행하던건 무시되고 처음부터 다시 시작된다 [8/30/2017 OSC]
		if( m_pCurrentModule && (m_strCurrentModuleName != INSP_MODULE_NONE) )
		{
			m_pCurrentModule->Run();
			bNext = m_pCurrentModule->IsStoped();
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_CZone_Must(m_strCurrentModuleName, m_Shuttle);
			if(m_pCurrentModule)
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepMustStart Module %s -> %s"), m_strStateName, m_strCurrentModuleName, m_pCurrentModule->m_ModuleName);
				m_strCurrentModuleName = m_pCurrentModule->m_ModuleName;
				// SetParam 안에서 Reset이 호출된다 [8/30/2017 OSC]
				m_pCurrentModule->SetParam(m_strStateName, m_nThreadID, FALSE, m_LogIndex);
				m_pCurrentModule->SetShuttle(m_Shuttle);
				m_pCurrentModule->Start();
			}
			else
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepMustStart Complete"), m_strStateName);
				m_strCurrentModuleName = INSP_MODULE_NONE;

				// Z축 상승 [8/10/2017 OSC]
				Inspection_Z_UP_Move(m_Shuttle);
				theTactTimeLog.m_InspectionZ_Up[m_Shuttle].Start();
				CellLog_CZoneETCTime_SetStartTime(m_Shuttle);
				nStep++;
			}
		}
		break;

	case stepAftStart:
		// Start()함수가 호출 될 때마다 m_strCurrentModuleName가 INSP_MODULE_NONE로 리셋되기 때문에 
		// 이전 진행하던건 무시되고 처음부터 다시 시작된다 [8/30/2017 OSC]
		if( m_pCurrentModule && (m_strCurrentModuleName != INSP_MODULE_NONE) )
		{
			m_pCurrentModule->Run();
			bNext = m_pCurrentModule->IsStoped();
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_CZone_Aft(m_strCurrentModuleName, m_Shuttle);
			if(m_pCurrentModule)
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepAftStart Module %s -> %s"), m_strStateName, m_strCurrentModuleName, m_pCurrentModule->m_ModuleName);
				m_strCurrentModuleName = m_pCurrentModule->m_ModuleName;
				// SetParam 안에서 Reset이 호출된다 [8/30/2017 OSC]
				m_pCurrentModule->SetParam(m_strStateName, m_nThreadID, FALSE, m_LogIndex);
				m_pCurrentModule->SetShuttle(m_Shuttle);
				m_pCurrentModule->Start();
			}
			else
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepAftStart Complete"), m_strStateName);
				m_strCurrentModuleName = INSP_MODULE_NONE;

				CellLog_CZoneETCTime_SetEndTime(m_Shuttle);
				CZone_SetTimeEnd(m_Shuttle);
				m_Timer.Start();
				nStep++;
			}
		}
		break;

	case stepZ_UpCheck:				
		m_bRtn[0] = LightCurtain_Check(m_Shuttle);
		m_bRtn[1] = Inspection_Z_UP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepZ_UpCheck"), m_strStateName);
			theTactTimeLog.m_InspectionZ_Up[m_Shuttle].End();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			if(m_bRtn[0] == FALSE)		SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[1] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1, m_Shuttle));
		}
		break;

	case stepEnd:
		theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
		// 메인화면 표시 위해 미리 존 디펙 산출 [9/12/2017 OSC]
		JudgeZoneDefect(m_Shuttle,ZONE_ID_C);

		//20170913 hhkim 최종 판정
		JudgeFinalClass(m_Shuttle,JIG_CH_1);
		CIM_CellCimJudge(m_Shuttle);
		//20170913 hhkim InspectionEnd
		CIM_CellAllInspectionEnd(m_Shuttle);

		//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
		GetMTP_Isnpection_Value(m_Shuttle, JIG_CH_1);

		// 생산정보 Update - LSH171217
		Product_CountUpdate(m_Shuttle);

		CZone_SetTimeWait(m_Shuttle);

		SetZoneEnd(m_Shuttle, ZONE_ID_C);
		if(theConfigBank.m_System.m_bInlineMode)
			PatternReset_Send(m_Shuttle, JIG_CH_MAX);
		CZonetoAZone_SetTimeStart(m_Shuttle);
		m_pCurrentModule = NULL;
		nStep = stepIdle;
		break;
	}

	if(m_nStep != stepIdle)
		m_nStep = nStep;

	return nStep;
}
