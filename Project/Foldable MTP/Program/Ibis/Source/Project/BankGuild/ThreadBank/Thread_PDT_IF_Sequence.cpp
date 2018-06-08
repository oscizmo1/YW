#include "StdAfx.h"
#include "Thread_PDT_IF_Sequence.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CThread_PDT_IF_Sequence::CThread_PDT_IF_Sequence(void)
{
	m_pfunc = NULL;
	m_bEMS = TRUE;
	m_bDoorClose = FALSE;
	m_STO = STO_ALARM;
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
// 		m_pAZone[jig] = NULL;
// 		m_pMoveBZone[jig] = NULL;
// 		m_pMoveAZone[jig] = NULL;
// 		m_pIF[jig] = NULL;

		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			m_bVacOnInput[jig][ch] = FALSE;
			m_bVacOnOutput[jig][ch] = FALSE;
		}
	}
	GetStatePointer();

	m_nRobotAlarmCode = 0;
	m_RobotStatus = CDevicePDT_IF::PDT_ROBOT_IDLE;
}


CThread_PDT_IF_Sequence::~CThread_PDT_IF_Sequence(void)
{
}

UINT CThread_PDT_IF_Sequence::PDT_InterfaceThr( LPVOID lpParam )
{
	if(theConfigBank.m_System.m_bInlineMode == FALSE)
		return 0;

	// MP2100 ����� ��� �� Thread���� Open
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_PDT_IF);

	int nThreadIndex = (int)lpParam;

	CThread_PDT_IF_Sequence *pClass = &theThreadBank.m_Thread_PDT_IF_Sequence;

	// Thread������ theUnitFunc�� ���� ���� �ȵ�!!! �Ʒ� _func�� ����
	CUnitCtrlFunc							_func(MP2100_THREAD_PDT_IF);	// �Ʒ� ��ü�� �̸��� �ʹ� �� �ϳ� �� ����
	pClass->m_pfunc = &_func;

	CONST_MACHINE_STATE::ID _MyState = CONST_MACHINE_STATE::MACHINE_PAUSE;
	CONST_MACHINE_STATE::ID _MainState = CONST_MACHINE_STATE::MACHINE_PAUSE;

	pClass->StateAllReset();

	// Ȥ�ó� ������ ���� ���� ��츦 ����ؼ� �� �����ش� [10/6/2017 OSC]
	pClass->BitInit();
	_func.MTP_IF_Servival_AllReset();
	_func.SetOutPutIO(Y_HOT_LINE_MTP_ALIVE, TRUE);
	_func.SetOutPutIO(Y_HOT_LINE_MTP_NET_READY, TRUE);

	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(2);

		if(theProcBank.m_bProgramInitFinish == FALSE)
			continue;

		if(theProcBank.m_bPDT_IF_State_Clear)
		{
			theLog[LOG_SEQUENCE].AddBuf(_T("        \t PDT_InterfaceThr State Reset"));
			_func.MTP_IF_Reset(JIG_ID_A);
			_func.MTP_IF_Reset(JIG_ID_B);
			pClass->StateAllReset();
			theProcBank.PDT_IF_NoCheckVacOff_Clear();
			theProcBank.m_bPDT_IF_State_Clear = FALSE;
		}

		// Hot Line
		if(theProcBank.ModuleIsRunState(_MainState) == FALSE)
			pClass->MTP_HotLine_Check();

		// Robot Alarm Check
		pClass->Robot_Alarm_Check();

		// ���� ���� ����
		if(_MainState != theProcBank.GetMachineState())
		{
			if( theProcBank.MachineIsRunState() && 
				(theProcBank.ModuleIsRunState(_MainState) == FALSE) )
			{				
				// ����� �����....
			}
			else if(theProcBank.ModuleIsRunState(_MainState) &&
				(theProcBank.MachineIsRunState() == FALSE) )
			{
				// �����ϴ� �����...
			}

			_MainState = theProcBank.GetMachineState();
		}

		// �ڱ� ���� ����
		if(_MyState != _MainState)
		{
			// Interface�� ���߿� �ߴܵǴ� ���� ���� ���� ���� ���°� ����Ǹ� ���� �ϴ��� ���� �ϰ� �ڽ��� �����Ѵ�.
			if( theProcBank.ModuleIsRunState(_MainState) && 
				(theProcBank.ModuleIsRunState(_MyState) == FALSE) )
			{				
				// ����� ����ô� �̹� �������̸� �����ϰ�, ���������� START�Ѵ�
				// ROBOT�� ���� �ȵǾ� ������ ��� [12/8/2017 OSC]
				if( (_func.PDT_IF_Start_Check(JIG_ID_A) == FALSE)
					&& (_func.PDT_IF_Start_Check(JIG_ID_A) == FALSE)
					&& (_func.PDT_IF_Start_Check(JIG_ID_A) == FALSE) )
				{
					if( pClass->IsAllStop() || pClass->IsError() )
					{
						theLog[LOG_SEQUENCE].AddBuf(_T("        \t PDT_InterfaceThr Resume"));
						_func.MTP_IF_Reset(JIG_ID_A);
						_func.MTP_IF_Reset(JIG_ID_B);
						pClass->StateAllReset();

						_func.MTP_IF_Servival_AllSet();

						_MyState = _MainState;
					}
				}
			}
			else if(theProcBank.ModuleIsRunState(_MyState) &&
				(theProcBank.ModuleIsRunState(_MainState) == FALSE) )
			{
				// �����ϴ� ����ô� �Ұ� �� �ϰ� �����
				if( pClass->IsAllStop() || pClass->IsError()
					|| pClass->Robot_StatusIsAlarm() )
				{
					theLog[LOG_SEQUENCE].AddBuf(_T("        \t PDT_InterfaceThr Pause"));
					_func.MTP_IF_Servival_AllReset();
					_MyState = _MainState;
				}
			}

		}

		pClass->VacuumOnOff_Check();

		// State�� Alarm ���¸� ���� ����
		if(pClass->IsError())
			continue;

		// ���� RUN ���°� �ƴϸ� ���� ����
		if(theProcBank.ModuleIsRunState(_MyState) == FALSE)
		{
			continue;
		}


		//-------------------------------------------------------------------------------------------------------------------------------
		//--- Step, Wake up! 
		//-------------------------------------------------------------------------------------------------------------------------------
		// Restart�� �Ϸ�Ǿ�� �ٸ� State�� �����Ѵ�
		if( (pClass->m_pRestart->IsHolded() == FALSE) || pClass->m_pRestart->IsStoped() )
		{
		}
		else
		{
			// Restart�� �Ϸ�Ǿ�� �ٸ� State�� �����Ѵ�
			//////////////////////////////////////////////////////////////////////////
			// �� State���� Start���� Ȯ�� �� Start ����
			// Hot Line�� ������ ���� ������ �����Ѵ�
			//////////////////////////////////////////////////////////////////////////
			if(pClass->PDT_HotLine_Check()	)
			{
				for(int i = 0; i < JIG_ID_MAX; i++)
				{
					pClass->MTP_Able_AllOnOff(_func, (JIG_ID)i);
					pClass->ConfirmStart_IF(_func, (JIG_ID)i);
				}
			}
		}



		//-------------------------------------------------------------------------------------------------------------------------------
		//--- Step, Running!
		//-------------------------------------------------------------------------------------------------------------------------------

		pClass->StateAllRun();
	}

	_func.SetOutPutIO(Y_HOT_LINE_MTP_NET_READY, FALSE);
	_func.SetOutPutIO(Y_HOT_LINE_MTP_ALIVE, FALSE);

	TRACE("PDT_InterfaceThr end\n");
	return 0;
}

void CThread_PDT_IF_Sequence::ResetReturnValue()
{
	memset(m_bRtnComp, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
	memset(m_bRtn, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
}

BOOL CThread_PDT_IF_Sequence::IsReturnOk()
{
	return (memcmp(m_bRtn, m_bRtnComp, MAX_RTN_SIZE) == 0) ? TRUE:FALSE;
}

BOOL CThread_PDT_IF_Sequence::IsAllStop()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		if(m_pIF[jig]->IsStoped() == FALSE)
			return FALSE;
	}
	return TRUE;
}

BOOL CThread_PDT_IF_Sequence::IsError()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		if(m_pIF[jig]->IsError())
			return TRUE;
	}
	return FALSE;
}

void CThread_PDT_IF_Sequence::StateAllReset()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		m_pIF[jig]->Reset();
	}
}

void CThread_PDT_IF_Sequence::StateAllRun()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		m_pIF[jig]->Run();
	}
}

void CThread_PDT_IF_Sequence::GetStatePointer( )
{
	m_pRestart = &theThreadBank.m_stateRestart;

	m_pAZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_AZone;
	m_pAZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_AZone;

	m_pMoveBZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_MoveBZone;
	m_pMoveBZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_MoveBZone;

	m_pMoveAZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_MoveAZone;
	m_pMoveAZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_MoveAZone;

	m_pIF[JIG_ID_A] = &theThreadBank.m_stateShuttle1_PDT_IF;
	m_pIF[JIG_ID_B] = &theThreadBank.m_stateShuttle2_PDT_IF;
}

void CThread_PDT_IF_Sequence::BitInit()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			m_pfunc->MTP_IF_CellInfo_Write((JIG_ID)jig, (JIG_CH)ch);
			m_pfunc->MTP_IF_CheckSensor_OnOff((JIG_ID)jig, (JIG_CH)ch, FALSE);
			m_pfunc->MTP_IF_VacuumStatus_OnOff((JIG_ID)jig, (JIG_CH)ch, FALSE);
		}
		m_pfunc->MTP_IF_Reset((JIG_ID)jig);
	}
}

void CThread_PDT_IF_Sequence::VacuumOnOff_Check()
{
	// Vacuum On Off ���¿� ���� LB�� On Off�Ѵ� [10/5/2017 OSC]
	BOOL bVacOn;
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			// CHECK_SENSOR�� �Է����̶� ����ȭ�Ѵ� [10/21/2017 OSC]
			bVacOn = m_pfunc->Shuttle_Vac_Check((JIG_ID)jig, (JIG_CH)ch, VAC_ON);
			// ������ ����ִ� ������ ������ TRUE [12/8/2017 OSC]
			if(theProcBank.m_bPDT_IF_NoCheckVacOff[jig][ch])
				bVacOn = TRUE;
			if(m_bVacOnInput[jig][ch] != bVacOn)
			{
				m_bVacOnInput[jig][ch] = bVacOn;
				if(bVacOn)
				{
					m_pfunc->MTP_IF_CheckSensor_OnOff((JIG_ID)jig, (JIG_CH)ch, TRUE);
					theLog[LOG_SEQUENCE_PDT_IF].AddBuf(_T("[SHUTTLE%d_PDT_IF] MTP_IF_CheckSensor_OnOff JIG%c CH%d Vac ON"), jig, _T('A')+jig, ch+1);
				}
				else
				{
					m_pfunc->MTP_IF_CheckSensor_OnOff((JIG_ID)jig, (JIG_CH)ch, FALSE);
					theLog[LOG_SEQUENCE_PDT_IF].AddBuf(_T("[SHUTTLE%d_PDT_IF] MTP_IF_CheckSensor_OnOff JIG%c CH%d Vac OFF"), jig, _T('A')+jig, ch+1);
				}
			}

			// VAC_ON_OFF_STATUS�� �Է��� �ƴ϶� ����̶� ����ȭ�Ѵ� [10/21/2017 OSC]
			bVacOn = m_pfunc->Shuttle_VacOut_Check((JIG_ID)jig, (JIG_CH)ch, VAC_ON);
			// ������ ����ִ� ������ ������ TRUE [12/8/2017 OSC]
			if(theProcBank.m_bPDT_IF_NoCheckVacOff[jig][ch])
				bVacOn = TRUE;
			if(m_bVacOnOutput[jig][ch] != bVacOn)
			{
				if(bVacOn)
				{
					m_pfunc->MTP_IF_VacuumStatus_OnOff((JIG_ID)jig, (JIG_CH)ch, TRUE);
					m_bVacOnOutput[jig][ch] = bVacOn;
					theLog[LOG_SEQUENCE_PDT_IF].AddBuf(_T("[SHUTTLE%d_PDT_IF] JIG%c CH%d Vac ON"), jig, _T('A')+jig, ch+1);
				}
				// OFF�� ���� VAC ������ OFF�� ������ ��ٷȴٰ� ���� [12/1/2017 OSC]
				else if(m_pfunc->Shuttle_Vac_Check((JIG_ID)jig, (JIG_CH)ch, VAC_OFF))
				{
					m_pfunc->MTP_IF_VacuumStatus_OnOff((JIG_ID)jig, (JIG_CH)ch, FALSE);
					m_bVacOnOutput[jig][ch] = bVacOn;
					theLog[LOG_SEQUENCE_PDT_IF].AddBuf(_T("JIG%c CH%d Vac Off"), _T('A')+jig, ch+1);
			}
			}
		}
	}
}

void CThread_PDT_IF_Sequence::MTP_Able_JigOnOff( JIG_ID jig )
{
	BOOL bOn = FALSE;
	BOOL bCellExist = FALSE;
	CCellInfo *pCell;

	// ���������� �ִ� Cell�� ������ ������ �ϹǷ� �츰�� [10/6/2017 OSC]
	for(int ch = 0; ch < JIG_CH_MAX; ch++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)ch);
		if(pCell)
		{
			bCellExist = TRUE;
			if(pCell->defaultData.m_LastClass != NONE_CELL)
			{
				bOn = TRUE;
				break;
			}
		}
	}

	// Cell�� ���� ��
	if(bCellExist == FALSE)
	{
		// ä�� ���� �̻���̸� �츮�� �ʴ´� 
		if(theProcBank.AZoneChannelNotUse_Check(jig, JIG_CH_1))
		{
			bOn = FALSE;
		}
		else 
		{
			// Cell�� ���� �� Loading Stop �����̸� Able�� �츮�� �ʴ´� [12/22/2017 OSC]
			if(theProcBank.LoadingStop_IsRequire())
			{
				bOn = FALSE;
			}
			// Cell�� ������ �޾ƾ� �ϹǷ� �츰��
			else
			{
				bOn = TRUE;
			}
		}
	}

	if(theProcBank.m_bAbleOnStatus[jig] != bOn)
	{
		// MP2100 �Լ� �ϳ��ϳ��� ����� �����ɷ��� 
		// ABLE �츱 �� ���⼭ �ѹ��� üũ�Ѵ� [10/15/2017 OSC]
		if(m_pfunc->Shuttle_Y_LOAD_Check(jig))
		{
			m_pfunc->MTP_IF_LoadingStop_OnOff(jig);
			m_pfunc->MTP_IF_CellInfo_Write(jig, JIG_CH_1);
			m_pfunc->MTP_IF_Able_OnOff(jig, bOn);
		}
	}
}

void CThread_PDT_IF_Sequence::MTP_Able_AllOnOff( CUnitCtrlFunc &_func, JIG_ID jig )
{
	// �ε� Ȥ�� ��ε� �� �غ� �Ǿ� ������ ABLE ON
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[2] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[3] = m_pIF[jig]->IsStoped();
	m_bRtn[4] = theConfigBank.m_System.m_bCIMQualMode ? FALSE:TRUE;
	if(IsReturnOk())
	{
		if(_func.CellInfo_CheckLoadable(jig) || _func.CellInfo_CheckUnloadable(jig))
			MTP_Able_JigOnOff(jig);
	}
}

void CThread_PDT_IF_Sequence::ConfirmStart_IF( CUnitCtrlFunc &_func, JIG_ID jig )
{
	// ���ʿ� Able�� ��� ��� ����
	ResetReturnValue();
	m_bRtn[2] = m_pIF[jig]->IsStoped();
	m_bRtn[4] = _func.MTP_IF_Able_Check(jig);	
	m_bRtn[5] = _func.PDT_IF_Servival_Check(jig);	
	m_bRtn[6] = _func.PDT_IF_Able_Check(jig);	
// 	m_bRtn[7] = _func.CellInfo_CheckLoadable(jig);	
// 	m_bRtn[8] = _func.CellInfo_CheckUnloadable(jig, JIG_CH_1) ? FALSE:TRUE;	
// 	m_bRtn[9] = _func.CellInfo_CheckUnloadable(jig, JIG_CH_2) ? FALSE:TRUE;	
	m_bRtn[10] = _func.PDT_IF_Start_Check(jig) ? FALSE:TRUE;	// ���� ��Ʈ�� ��������� ���� �� ���� ���
	if(IsReturnOk())
	{
		m_pIF[jig]->Start();
	}
}

void CThread_PDT_IF_Sequence::MTP_HotLine_Check()
{
	BOOL bEMS1 = m_pfunc->GetInPutIOCheck(X_EMS_SWITCH_1);
	BOOL bEMS2 = m_pfunc->GetInPutIOCheck(X_EMS_SWITCH_2);
	BOOL bTotal = (bEMS1 || bEMS2) ? TRUE:FALSE;
	if(bTotal != m_bEMS)
	{
		m_bEMS = bTotal;
		m_pfunc->SetOutPutIO(Y_HOT_LINE_MTP_EMERGENCY, bTotal);
	}

	BOOL bValue = m_pfunc->DoorClose_Check(FALSE);
	if(bValue != m_bDoorClose)
	{
		m_bDoorClose = bValue;
		m_pfunc->SetOutPutIO(Y_HOT_LINE_MTP_DOOR_OPEN, bValue ? FALSE:TRUE);
	}

	STO_STATE sto = m_pfunc->STO_Check();
	if(sto != m_STO)
	{
		m_STO = sto;
		m_pfunc->SetOutPutIO(Y_HOT_LINE_MTP_INTERLOCK, sto == STO_ALARM ? TRUE:FALSE);
	}
}

BOOL CThread_PDT_IF_Sequence::PDT_HotLine_Check()
{
	if(m_pfunc->GetInPutIOCheck(X_HOT_LINE_PDT_ALIVE) == FALSE)
		return FALSE;
	if(m_pfunc->GetInPutIOCheck(X_HOT_LINE_PDT_EMERGENCY))
		return FALSE;
	if(m_pfunc->GetInPutIOCheck(X_HOT_LINE_PDT_NET_READY) == FALSE)
		return FALSE;
	if(m_pfunc->GetInPutIOCheck(X_HOT_LINE_PDT_DOOR_OPEN))
		return FALSE;
	if(m_pfunc->GetInPutIOCheck(X_HOT_LINE_PDT_INTERLOCK))
		return FALSE;

	return TRUE;
}

BOOL CThread_PDT_IF_Sequence::Robot_StatusIsAlarm()
{
	if(m_RobotStatus == CDevicePDT_IF::PDT_ROBOT_ERR)
		return TRUE;
	else
		return FALSE;
}

void CThread_PDT_IF_Sequence::Robot_Alarm_Check()
{
	// ��¥�� ���� �״� �ϳ��� �ҷ��´�
	int nErrCode = theDevicePDT_IF.PDT_LW_ReadRobotAlarmCode(JIG_ID_A, JIG_CH_1);
	CDevicePDT_IF::ROBOT_STATUS status = theDevicePDT_IF.PDT_LW_ReadRobotStatus(JIG_ID_A, JIG_CH_1);
	if(m_nRobotAlarmCode != nErrCode)
	{
		// �˶��ڵ� && Robot Status ��� �����ϸ� ����
		if(nErrCode >= ALM_ROBOT_ALARM_START)
		{
			if(status == CDevicePDT_IF::PDT_ROBOT_ERR)
			{
				m_nRobotAlarmCode = nErrCode;
				if(MachineIsRobotAlarmAble())
				{
					m_pIF[JIG_ID_A]->SetError((ALARM_ID)m_nRobotAlarmCode);
// 					theProcBank.AlarmHappen((ALARM_ID)m_nRobotAlarmCode);
					theLog[LOG_SEQUENCE_PDT_IF].AddBuf(_T("ROBOT ALARM Set %d"), m_nRobotAlarmCode);
				}
				else
				{
					theLog[LOG_SEQUENCE_PDT_IF].AddBuf(_T("ROBOT ALARM Set %d Ignore"), m_nRobotAlarmCode);
				}
			}
		}
		else
		{
			m_nRobotAlarmCode = nErrCode;
			StateAllReset();
			if(theProcBank.m_AlarmID != ALM_NONE)
			{
				theLossTimeLog.SetMachineResetTime();
				theProcBank.AlarmClear();
				theLog[LOG_SEQUENCE_PDT_IF].AddBuf(_T("ROBOT ALARM Reset %d"), m_nRobotAlarmCode);
			}
		}
	}

	if(m_RobotStatus != status)
	{
		if( (m_RobotStatus == CDevicePDT_IF::PDT_ROBOT_ERR)
			&& (status != CDevicePDT_IF::PDT_ROBOT_ERR) )
		{
			// ���� �ڵ� �����
			if(ConfirmMachineRestart())
			{
				theLog[LOG_SEQUENCE_PDT_IF].AddBuf(_T("ROBOT RUN"));
				::PostMessage( GetMainWnd(), GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_Main, (LPARAM)FALSE);
				theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_CHECK);
			}
		}
		m_RobotStatus = status;
	}

}

BOOL CThread_PDT_IF_Sequence::MachineIsRobotAlarmAble()
{
	// PM���� �κ� �˶� ����
	if(GetMainHandler()->m_pGUIFrame->GetCurrentViewID() == eFORM_PM)
		return FALSE;

	return TRUE;
}

BOOL CThread_PDT_IF_Sequence::ConfirmMachineRestart()
{
	// FormMain���� Start ��ư ���� �� üũ�ϴ� ��κ��� �׸��� �� üũ�ؼ�
	// �����ϸ� TRUE
	if(theProcBank.MachineIsRunState())
		return FALSE;
	if (theProcBank.m_AlarmID != ALM_NONE )
		return FALSE;
	if(theProcBank.MachineIsRunState())
		return FALSE;
	if(theUnitFunc.DoorClose_Check() == FALSE)
		return FALSE;
	if(theUnitFunc.DoorLockOn_Check() == FALSE)
		return FALSE;
	if(theUnitFunc.GetInPutIOCheck(X_MC_ON_CHECK) == FALSE)
		return FALSE;
	STO_STATE sto = theUnitFunc.STO_Check();
	if(sto != STO_READY)
		return FALSE;
	if(theUnitFunc.AutoTeachKey_AutoCheck() == FALSE)
		return FALSE;
	if(theRecipeBank.m_bModule_LoadSuccess == FALSE)
		return FALSE;
	if(GetMainHandler()->m_pGUIFrame->GetCurrentViewID() == eFORM_PM)
		return FALSE;

	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(theDeviceMotion.IsTrouble(MP2100_THREAD_PDT_IF, (AXIS_ID)i))
			return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// ErrorLog.csv ��� [12/18/2017 OSC]
	theProcBank.WriteErrorLogCsv();


	return TRUE;
}
