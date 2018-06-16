#include "stdafx.h"
#include "StateAZone.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStateAZone::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	BOOL bNext = TRUE;

	switch(nStep)
	{
	case stepStart:
		// State Name을 셔틀 이름 포함해서 다시 지정한다 [9/19/2017 OSC]
		theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);

		if(theConfigBank.m_System.m_bInlineMode)
		{
			theTactTimeLog.m_RunTime[m_Shuttle].Start();
			
			CellLog_TactTime_SetStartTime(m_Shuttle);

			//AZone_SetTimeStart(m_Shuttle); 20180312 AZONE TACT TIME 변경(STATEPDT_IF -> CellLoading_SetStartTime(m_Shuttle, (JIG_CH)i); 이동)
			m_Timer.Start();
			nStep++;
		}
		break;

		// ZONE_A 체크 [9/11/2017 OSC]
	case stepZoneACheck:
		m_bRtn[0] = SetZoneA_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepZoneACheck"), m_strStateName);

			if(theConfigBank.m_System.m_bInlineMode)
			{
				CellLog_LoadingTactTime_SetEndTime(m_Shuttle);
				m_Timer.Start();
				nStep++;
			}
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetZoneA_TimeOut(m_Shuttle);
		}
		break;

	case stepBefStart:
		// Start()함수가 호출 될 때마다 m_strCurrentModuleName가 INSP_MODULE_NONE로 리셋되기 때문에 
		// 이전 진행하던건 무시되고 처음부터 다시 시작된다 [8/30/2017 OSC]
		if( m_pCurrentModule && (m_strCurrentModuleName != INSP_MODULE_NONE) )
		{
			m_pCurrentModule->Run();
			bNext = m_pCurrentModule->IsStoped();
			if(theProcBank.m_bDryRunMode)
				bNext = TRUE;
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_AZone_Bef(m_strCurrentModuleName, m_Shuttle);
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
			if(theProcBank.m_bDryRunMode)
				bNext = TRUE;
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_AZone_Must(m_strCurrentModuleName, m_Shuttle);
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
				// CellLoading 전용변수에 있는 것을 옮겨담는다
				CellLoading_CopyInfo(m_Shuttle, JIG_CH_1);
				if(theConfigBank.m_System.m_bInlineMode)
				{
					// InLine모드시에는 Cell Loading 중에는 Vac Off상태로 하고 완료하면 On 한다 [12/19/2017 OSC]
					if(CellTagExist(m_Shuttle, JIG_CH_1))
					{
						Shuttle_Vac_OnOff(m_Shuttle, JIG_CH_1, VAC_ON, BLOW_OFF);
						theProcBank.m_bPDT_IF_NoCheckVacOff[m_Shuttle][JIG_CH_1] = FALSE;
					}
				}

				CellLog_AZoneETCTime_SetStartTime(m_Shuttle);
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
			if(theProcBank.m_bDryRunMode)
				bNext = TRUE;
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_AZone_Aft(m_strCurrentModuleName, m_Shuttle);
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

				CellLog_AZoneETCTime_SetEndTime(m_Shuttle);
				AZone_SetTimeEnd(m_Shuttle);

				if(theConfigBank.m_System.m_bInlineMode)
				{
					JudgeZoneDefect(m_Shuttle,ZONE_ID_A);
					TMD_INFO_Send(m_Shuttle);
					//kjpark 20180113 Tmd Version 추가
					Host_Version_Send(m_Shuttle);
					Client_Version_Send(m_Shuttle);
					m_Timer.Start();
					nStep++;
				}
			}
		}
		break;

	case stepTMDInfoCheck:
		m_bRtn[0] = CellTagExist(m_Shuttle, JIG_CH_1) ? TMD_INFO_Check(m_Shuttle, JIG_CH_1):TRUE;
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepTMDInfoCheck"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 2.)
		{
			TMD_INFO_Timeout(m_Shuttle);
		}
		break;

	case stepEnd:
		theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
		SetZoneEnd(m_Shuttle, ZONE_ID_A);
		ResetZoneEnd(m_Shuttle, ZONE_ID_MOVE_A);				
		m_pCurrentModule = NULL;
		//kjpark 20170913 MCR 위치에서 Z축  체크
		Inspection_Z_UP_Move(m_Shuttle);
		//kjpark 20170912 MCR 위치에따른 택타임 추가
		AZonetoMCR_SetTimeStart(m_Shuttle);

		AZone_SetTimeWait(m_Shuttle);

		nStep = stepIdle;
		break;
	}

	if(m_nStep != stepIdle)
		m_nStep = nStep;

	return nStep;
}
