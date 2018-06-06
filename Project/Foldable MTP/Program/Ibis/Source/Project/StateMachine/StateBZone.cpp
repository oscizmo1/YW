#include "stdafx.h"
#include "StateBZone.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStateBZone::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;
	BOOL bNext = TRUE;

	switch(nStep)
	{
	case stepStart:
		// State Name�� ��Ʋ �̸� �����ؼ� �ٽ� �����Ѵ� [9/19/2017 OSC]
		theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);

		if(theConfigBank.m_System.m_bInlineMode)
		{
			theTactTimeLog.m_RunTime[m_Shuttle].Start();

			CellLog_TactTime_SetStartTime(m_Shuttle);

			//AZone_SetTimeStart(m_Shuttle); 20180312 AZONE TACT TIME ����(STATEPDT_IF -> CellLoading_SetStartTime(m_Shuttle, (JIG_CH)i); �̵�)
			m_Timer.Start();
			nStep = stepZoneACheck;
		}
		else
		{
			// ��Ʋ �����ϸ� ���� ���� [10/2/2017 OSC]
			theUnitFunc.LightCurtainMute_OnOff(m_Shuttle, ON);
			theUnitFunc.LightCurtainMuteLamp_OnOff(m_Shuttle, ON);

			m_Timer.Start();
			nStep++;
		}
		break;

		// ��� �浵�� ��� ���� On/Off ����ġ �ѹ� �� ������ �ŷ�.. [9/27/2017 OSC]
	case stepPatternOffSwitchCheck:
		if( (theProcBank.m_bDryRunMode == FALSE && theConfigBank.m_Option.m_bUseLoofTest == FALSE) 
			&& (theProcBank.m_bIsSetZone[m_Shuttle][ZONE_ID_B] == FALSE) )
		{
			if(CellTagExist(m_Shuttle, JIG_CH_1))
			{
				if(CellInfo_GetInspFinish(m_Shuttle))
				{
					m_bRtn[0] = (m_Shuttle == JIG_ID_A) ? GetInPutIOCheck(X_SHUTTLE_1_PATTERN_ONOFF_SW):TRUE;
					m_bRtn[1] = (m_Shuttle == JIG_ID_B) ? GetInPutIOCheck(X_SHUTTLE_2_PATTERN_ONOFF_SW):TRUE;
				//	m_bRtn[3] = LoadingStop_IsRequire(m_Shuttle) ? FALSE:TRUE;
				}
			}
		}
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepPatternOffSwitchCheck"), m_strStateName);

			if(theProcBank.m_bIsSetZone[m_Shuttle][ZONE_ID_B] == FALSE)
			{
				theProcBank.m_bIsSetZone[m_Shuttle][ZONE_ID_B] = TRUE;

				if(theConfigBank.m_Option.m_bUseLoofTest)
				{
					Shuttle_Vac_OnOff(m_Shuttle, JIG_CH_1, VAC_ON);
				}
				else
				{
					Shuttle_Vac_OnOff(m_Shuttle, JIG_CH_1, VAC_OFF);
				}

				// Loading Stop�� ��� R�� ������ ���� L�� �����ϰ� ��Ʈ���̸� ����Ѵ� [12/1/2017 OSC]
				if(theProcBank.LoadingStop_IsRequire())
				{
					CIM_CellLoadingStop(m_Shuttle);
				}
				//else if(theProcBank.PreUnitInterlock_IsEmpty()
				//&& theConfigBank.m_CIM.GetFunction(EFID_INTERLOCK_CONTROL)->)
				//{
				// 
				//}
				CIM_CellTrackOut(m_Shuttle);

				CellLog_Write(m_Shuttle);


				CellLoading_InitInfo(m_Shuttle, JIG_CH_1, FALSE);

				// CellSkip Reset [9/16/2017 OSC]
				AZoneCellSkip_Reset(m_Shuttle);

				// CellData ����... ���߿� �ռ��� �����ϸ� �ű⼭ ä�� ������� �޾ƿ;� �� [9/7/2017 OSC]
				AZoneCellData_Create(m_Shuttle, JIG_CH_1);

				SetZoneA_Send(m_Shuttle);
			}

			if( theProcBank.AZoneChannelNotUse_Check(m_Shuttle, JIG_CH_1))
			{
				// ���� ��ä�� ��ŵ�̸� ���� �������� �ʴ´� [9/18/2017 OSC]
			}
			else
			{
				m_Timer.Start();
				nStep++;
			}
		}
		break;

		// ZONE_A üũ [9/11/2017 OSC]
	case stepZoneACheck:
		m_bRtn[0] = SetZoneA_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepZoneACheck"), m_strStateName);

			if(theConfigBank.m_System.m_bInlineMode || theConfigBank.m_Option.m_bUseLoofTest)
			{
				CellLog_LoadingTactTime_SetEndTime(m_Shuttle);

				if(theConfigBank.m_Option.m_bUseLoofTest
					|| theConfigBank.m_System.m_bCIMQualMode)
				{
					if(theProcBank.AZoneCellNG_Check(m_Shuttle, JIG_CH_1) == FALSE)
					{
						CellLoading_InitInfo(m_Shuttle, JIG_CH_1, FALSE);
						CellLoading_SetStartTime(m_Shuttle, JIG_CH_1);
						CellLoading_Send(m_Shuttle, JIG_CH_1, TRUE);
					}
				}
				else
				{

				}

				m_Timer.Start();
				nStep = stepBefStart;
			}
			else
			{
				// �۾��� ����ġ ���� ��� [9/7/2017 OSC]
				if(theProcBank.PreInterlock_IsEmpty(EFST_LOADING))
				{
					GetMainHandler()->m_bAZoneOperatorReady[m_Shuttle] = TRUE;
				}
				m_Timer.Start();
				nStep++;
			}
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
			SetZoneA_TimeOut(m_Shuttle);
		}
		break;

		// ����ȣ���� ��û���� ������ PG ON ��ư �ѹ� �̻� ������ �� [11/21/2017 OSC]
	case stepPatternOnSwitchCheck:
		if( (theProcBank.m_bDryRunMode == FALSE)) 
		{
			m_bRtn[0] = theProcBank.AZoneCellNG_Check(m_Shuttle, JIG_CH_1) ? TRUE:CellLoading_SendCheck(m_Shuttle, JIG_CH_1);
			m_bRtn[2] = theProcBank.LoadingStop_IsRequire() ? FALSE:TRUE;

		}
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepPatternOnSwitchCheck"), m_strStateName);

			CellLog_WaitTime_SetEndTime(m_Shuttle);
			AZone_SetTimeStart(m_Shuttle);
			CellLog_TactTime_SetStartTime(m_Shuttle);
			CellLoading_SetStartTime(m_Shuttle, JIG_CH_1);

			m_Timer.Start();
			nStep++;
		}
		break;

	case stepBefStart:
		// Start()�Լ��� ȣ�� �� ������ m_strCurrentModuleName�� INSP_MODULE_NONE�� ���µǱ� ������ 
		// ���� �����ϴ��� ���õǰ� ó������ �ٽ� ���۵ȴ� [8/30/2017 OSC]
		if( m_pCurrentModule && (m_strCurrentModuleName != INSP_MODULE_NONE) )
		{
			m_pCurrentModule->Run();
			bNext = m_pCurrentModule->IsStoped();
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_AZone_Bef(m_strCurrentModuleName, m_Shuttle);
			if(m_pCurrentModule)
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepBefStart Module %s -> %s"), m_strStateName, m_strCurrentModuleName, m_pCurrentModule->m_ModuleName);
				m_strCurrentModuleName = m_pCurrentModule->m_ModuleName;
				// SetParam �ȿ��� Reset�� ȣ��ȴ� [8/30/2017 OSC]
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
		// Start()�Լ��� ȣ�� �� ������ m_strCurrentModuleName�� INSP_MODULE_NONE�� ���µǱ� ������ 
		// ���� �����ϴ��� ���õǰ� ó������ �ٽ� ���۵ȴ� [8/30/2017 OSC]
		if( m_pCurrentModule && (m_strCurrentModuleName != INSP_MODULE_NONE) )
		{
			m_pCurrentModule->Run();
			bNext = m_pCurrentModule->IsStoped();
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_AZone_Must(m_strCurrentModuleName, m_Shuttle);
			if(m_pCurrentModule)
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepMustStart Module %s -> %s"), m_strStateName, m_strCurrentModuleName, m_pCurrentModule->m_ModuleName);
				m_strCurrentModuleName = m_pCurrentModule->m_ModuleName;
				//SetParam �ȿ��� Reset�� ȣ��ȴ� [8/30/2017 OSC]
				m_pCurrentModule->SetParam(m_strStateName, m_nThreadID, FALSE, m_LogIndex);
				m_pCurrentModule->SetShuttle(m_Shuttle);
				m_pCurrentModule->Start();
			}
			else
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepMustStart Complete"), m_strStateName);
				m_strCurrentModuleName = INSP_MODULE_NONE;
				// CellLoading ���뺯���� �ִ� ���� �Űܴ�´�
				CellLoading_CopyInfo(m_Shuttle, JIG_CH_1);
				if(theConfigBank.m_System.m_bInlineMode)
				{
					// InLine���ÿ��� Cell Loading �߿��� Vac Off���·� �ϰ� �Ϸ��ϸ� On �Ѵ� [12/19/2017 OSC]
					if(CellTagExist(m_Shuttle, JIG_CH_1))
					{
						Shuttle_Vac_OnOff(m_Shuttle, JIG_CH_1, VAC_ON);
						theProcBank.m_bPDT_IF_NoCheckVacOff[m_Shuttle][JIG_CH_1] = FALSE;
					}
				}

				CellLog_AZoneETCTime_SetStartTime(m_Shuttle);
				nStep++;
			}
		}
		break;

	case stepAftStart:
		// Start()�Լ��� ȣ�� �� ������ m_strCurrentModuleName�� INSP_MODULE_NONE�� ���µǱ� ������ 
		// ���� �����ϴ��� ���õǰ� ó������ �ٽ� ���۵ȴ� [8/30/2017 OSC]
		if( m_pCurrentModule && (m_strCurrentModuleName != INSP_MODULE_NONE) )
		{
			m_pCurrentModule->Run();
			bNext = m_pCurrentModule->IsStoped();
		}
		if(bNext)
		{
			m_pCurrentModule = theInspModuleBank.GetNextModule_AZone_Aft(m_strCurrentModuleName, m_Shuttle);
			if(m_pCurrentModule)
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepAftStart Module %s -> %s"), m_strStateName, m_strCurrentModuleName, m_pCurrentModule->m_ModuleName);
				m_strCurrentModuleName = m_pCurrentModule->m_ModuleName;
				// SetParam �ȿ��� Reset�� ȣ��ȴ� [8/30/2017 OSC]
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

				if(theConfigBank.m_System.m_bInlineMode || theConfigBank.m_Option.m_bUseLoofTest)
				{
					JudgeZoneDefect(m_Shuttle,ZONE_ID_B);
					TMD_INFO_Send(m_Shuttle);
					//kjpark 20180113 Tmd Version �߰�
					Host_Version_Send(m_Shuttle);
					Client_Version_Send(m_Shuttle);
					m_Timer.Start();
					nStep = stepTMDInfoCheck;
				}
				else
				{
					m_Timer.Start();
					nStep++;
				}
			}
		}
		break;

		// �۾��� �����ư [9/11/2017 OSC]
	case stepReadySwitchCheck:
		m_bRtn[0] = ReadySwitch_Check(m_Shuttle);
		m_bRtn[1] = theProcBank.AZoneChannelNotUse_Check(m_Shuttle, JIG_CH_1) ? TRUE:Shuttle_Vac_Check(m_Shuttle, JIG_CH_1, VAC_ON);
		m_bRtn[3] = GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? AZoneDefect_GoodCheck(m_Shuttle, JIG_CH_1):TRUE;
		// �̹� A�� �����ߴٰ� �ϴ��� ���� ��ä�� ��ŵ�̸� ���� �������� �ʴ´� [9/18/2017 OSC]
		if( theProcBank.AZoneChannelNotUse_Check(m_Shuttle, JIG_CH_1))
			m_bRtn[5] = FALSE;
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepReadySwitchCheck"), m_strStateName);
			GetMainHandler()->m_bAZoneOperatorReady[m_Shuttle] = FALSE;
			theUnitFunc.LightCurtainMute_OnOff(m_Shuttle, OFF);
			theUnitFunc.LightCurtainMuteLamp_OnOff(m_Shuttle, OFF);
			AZoneCell_RemoveSkipCell(m_Shuttle);
			TMD_INFO_Send(m_Shuttle);
			CellLog_SetOperatorID(m_Shuttle);

			if(theConfigBank.m_System.m_bInlineMode == FALSE)
				theTactTimeLog.m_RunTime[m_Shuttle].Start();
			m_Timer.Start();
			nStep++;
		}
		else
		{
			// ����ȭ�� ǥ�� ���� �̸� �� ���� ���� [9/12/2017 OSC]
			// �ٽ� ������ ��� ����ؼ� ��� ����� �Ѵ� �Ф�
			JudgeZoneDefect(m_Shuttle,ZONE_ID_B);
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
		SetZoneEnd(m_Shuttle, ZONE_ID_B);
		m_pCurrentModule = NULL;
		//kjpark 20170913 MCR ��ġ���� Z��  üũ
		Inspection_Z_UP_Move(m_Shuttle);
		//kjpark 20170912 MCR ��ġ������ ��Ÿ�� �߰�
		AZonetoMCR_SetTimeStart(m_Shuttle);

		AZone_SetTimeWait(m_Shuttle);

		nStep = stepIdle;
		break;
	}

	if(m_nStep != stepIdle)
		m_nStep = nStep;

	return nStep;
}