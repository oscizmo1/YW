#include "StdAfx.h"
#include "Thread_Main_Sequence.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CThread_Main_Sequence::CThread_Main_Sequence(void)
{
	m_pfunc = NULL;
	GetStatePointer();
	GetCELL_POS();
}


CThread_Main_Sequence::~CThread_Main_Sequence(void)
{
}

UINT CThread_Main_Sequence::MainSequenceThr( LPVOID lpParam )
{
	int nThreadIndex = (int)lpParam;

	// MP2100 ����� ��� �� Thread���� Open
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_MAIN_SEQUENCE);
	// Thread������ theUnitFunc�� ���� ���� �ȵ�!!! �Ʒ� _func�� ����
	CUnitCtrlFunc				_func(MP2100_THREAD_MAIN_SEQUENCE);	// �Ʒ� ��ü�� �̸��� �ʹ� �� �ϳ� �� ����

	CThread_Main_Sequence *pClass = &theThreadBank.m_Thread_Main_Sequence;
	pClass->m_pfunc = &_func;

	CONST_MACHINE_STATE::ID _LastState = CONST_MACHINE_STATE::MACHINE_PAUSE;


	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(2);	

		//-------------------------------------------------------------------------------------------------------------------------------
		//--- EQ State Check
		//-------------------------------------------------------------------------------------------------------------------------------
		if(_LastState != theProcBank.GetMachineState())
		{
			if( theProcBank.MachineIsRunState() && 
				(theProcBank.ModuleIsRunState(_LastState) == FALSE) )
			{				
				// ����� �����....
				theLog[LOG_SEQUENCE].AddBuf(_T("        \tMain Thread Resume"));

				pClass->m_pRestart->Start();

				pClass->StateAllReset();
			}
			else if(theProcBank.ModuleIsRunState(_LastState) &&
				(theProcBank.MachineIsRunState() == FALSE) )
			{
				// �����ϴ� �����...
				pClass->m_pRestart->Reset();

				theLog[LOG_SEQUENCE].AddBuf(_T("        \tMain Thread Pause"));
			}

			_LastState = theProcBank.GetMachineState();
		}

		pClass->Cell_CheckExist(_func);

		// ���� Alarm ���¸� ���� ����
		if(theProcBank.m_AlarmID != ALM_NONE)
		{
			continue;

		}

		if(pClass->Confirm_Interlock(_func) == FALSE)
			continue;

		//// ���� RUN ���°� �ƴϸ� ���� ����
		if(theProcBank.ModuleIsRunState(_LastState) == FALSE)
		{
			continue;

		}

		//-------------------------------------------------------------------------------------------------------------------------------
		//--- Step, Wake up! 
		//-------------------------------------------------------------------------------------------------------------------------------
		//Restart�� �Ϸ�Ǿ�� �ٸ� State�� �����Ѵ�
			if( (pClass->m_pRestart->IsHolded() == FALSE) || pClass->m_pRestart->IsStoped() )
			{

				pClass->m_pRestart->Run();
				continue;

			}

		//////////////////////////////////////////////////////////////////////////
		// �� State���� Start���� Ȯ�� �� Start ����
		//////////////////////////////////////////////////////////////////////////
		for(int i = 0; i < JIG_ID_MAX; i++)
		{
			pClass->ConfirmStart_AZone(_func, (JIG_ID)i);
			pClass->ConfirmStart_MoveBZone(_func, (JIG_ID)i);
			pClass->ConfirmStart_MoveAZone(_func, (JIG_ID)i);
			pClass->ConfirmStart_BZone(_func, (JIG_ID)i);
		}

		//-------------------------------------------------------------------------------------------------------------------------------
		//--- Step, Running!
		//-------------------------------------------------------------------------------------------------------------------------------

		pClass->StateAllRun();
	}


	TRACE("Main Thread end\n");
	return 0;
}

void CThread_Main_Sequence::ResetReturnValue()
{
	memset(m_bRtnComp, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
	memset(m_bRtn, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
}

BOOL CThread_Main_Sequence::IsReturnOk()
{
	return (memcmp(m_bRtn, m_bRtnComp, MAX_RTN_SIZE) == 0) ? TRUE:FALSE;
}

void CThread_Main_Sequence::StateAllReset()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		m_pAZone[jig]->Reset();
		m_pBZone[jig]->Reset();
		m_pMoveBZone[jig]->Reset();
		m_pMoveAZone[jig]->Reset();
	}
}

void CThread_Main_Sequence::StateAllRun()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		m_pAZone[jig]->Run();
		m_pBZone[jig]->Run();
		m_pMoveBZone[jig]->Run();
		m_pMoveAZone[jig]->Run();
	}
}

void CThread_Main_Sequence::GetStatePointer( )
{
	m_pRestart = &theThreadBank.m_stateRestart;

	m_pAZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_AZone;
	m_pAZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_AZone;

	m_pBZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_BZone;
	m_pBZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_BZone;

	m_pMoveBZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_MoveBZone;
	m_pMoveBZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_MoveBZone;

	m_pMoveAZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_MoveAZone;
	m_pMoveAZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_MoveAZone;

	m_pIF[JIG_ID_A] = &theThreadBank.m_stateShuttle1_PDT_IF;
	m_pIF[JIG_ID_B] = &theThreadBank.m_stateShuttle2_PDT_IF;
}

void CThread_Main_Sequence::GetCELL_POS()
{
	m_CellPosCh1[JIG_ID_A] = CELL_POS_SHUTTLE1_CH1;
	m_CellPosCh1[JIG_ID_B] = CELL_POS_SHUTTLE2_CH1;
}

void CThread_Main_Sequence::Cell_CheckExist(CUnitCtrlFunc &_func)
{
	int nState = theProcBank.m_bMachineCellExist;
	//////////////////////////////////////////////////////////////////////////
	// �� ���� ���� �ڵ� ���� [6/15/2017 OSC]

	//180402 kyj TP �̻� ����� ���ۿ� �ִ� �� ���� üũ �ּ�ó��  
	//if(theProcBank.RetryCellInfo_IsExist())
	//{
	//	if(nState == FALSE)
	//	{
	//		nState = TRUE;
	//	}
	//}
	//else
	{
		if(theConfigBank.m_System.m_bInlineMode)
		{
			if(_func.CellTagExist(CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1))
			{
				if(nState == FALSE)
				{
					nState = TRUE;
				}
			}
			else
			{
				if(nState)
				{
					nState = FALSE;
				}
			}
		}
		else
		{
			if( 
					(	_func.CellTagExist(CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1) == FALSE	)
					||
					(	(_func.CellLoading_RecvCheck(JIG_ID_A) == FALSE)
						&& (_func.CellLoading_RecvCheck(JIG_ID_B) == FALSE)	
					)
				)
			{
				if(nState)
				{
					nState = FALSE;
				}
			}
			else
			{
				if(nState == FALSE)
				{
					nState = TRUE;
				}
			}
		}
	}
	if(nState != theProcBank.m_bMachineCellExist)
	{
		theProcBank.m_bMachineCellExist = nState;
		_func.CIM_CellExistInMachine(theProcBank.m_bMachineCellExist);
	}

}

BOOL CThread_Main_Sequence::Confirm_Interlock(CUnitCtrlFunc &_func)
{
// 	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INTERLOCK)
// 		return TRUE;

	// ������ ���� Interlock�� �޴� ��� Interlock�� �߻���Ų��. - LSH171124
	// (���� ������� Interlock����)
	if(theProcBank.PreInterlock_IsEmpty(EFST_STEP) == FALSE)
	{
		_func.PreInterlockHappen(EFST_STEP);
		return FALSE;
	}
	// ������ ���� Interlock�� ���� �� ���� ���� �ִ� Cell�� ��� Unloading�� �߻���Ų��. - LSH171124
	// (Cell�� loading�� �� ����.)
	else if(theProcBank.PreInterlock_IsEmpty(EFST_LOADING) == FALSE)
	{
		if(_func.LoadingStop_IsAble())
		{
			_func.PreInterlockHappen(EFST_LOADING);
			return FALSE;
		}
	}
	// ������ ���� Interlock�� ���� �� ���� ���� �ִ� Cell�� ��� Unloading��ġ�� ������ �߻� - LSH171124
	// (Cell�� Unloading�� �� ����.)
	else if(theProcBank.PreInterlock_IsEmpty(EFST_TRANSFER) == FALSE)
	{
		if(_func.TransferStop_IsAble())
		{
			_func.PreInterlockHappen(EFST_TRANSFER);
			return FALSE;
		}
	}
	else if(theProcBank.PreUnitInterlock_IsEmpty(JIG_ID_MAX) == FALSE)
	{
		if(theConfigBank.m_CIM.INTERLOCK_CONTROL_StepCheck())
		{
			_func.PreUnitInterlockHappen();
			return FALSE;
		}
		else if(theConfigBank.m_CIM.INTERLOCK_CONTROL_LoadingCheck())
		{
			// Unit Interlock�� ��� ���� ���� �ߵ��ؾ� �� [12/4/2017 OSC]
			if(theProcBank.PreUnitInterlock_IsEmpty(JIG_ID_MAX) == FALSE)
			{
				if(_func.LoadingStop_IsAble())
				{
					_func.PreUnitInterlockHappen();
					return FALSE;
				}
			}
// 			else if(theProcBank.PreUnitInterlock_IsEmpty(JIG_ID_B) == FALSE)
// 			{
// 				if(_func.LoadingStop_IsAble(JIG_ID_B))
// 				{
// 					_func.PreUnitInterlockHappen();
// 					return FALSE;
// 				}
// 			}
// 			else if(theProcBank.PreUnitInterlock_IsEmpty(JIG_ID_C) == FALSE)
// 			{
// 				if(_func.LoadingStop_IsAble(JIG_ID_C))
// 				{
// 					_func.PreUnitInterlockHappen();
// 					return FALSE;
// 				}
// 			}
		}
		else if(theConfigBank.m_CIM.INTERLOCK_CONTROL_TransferCheck())
		{
			if(_func.TransferStop_IsAble())
			{
				_func.PreUnitInterlockHappen();
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CThread_Main_Sequence::ConfirmStart_AZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[2] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[3] = m_pBZone[jig]->IsStoped();
	m_bRtn[4] = _func.GetZoneEnd(jig, ZONE_ID_A) ? FALSE:TRUE;
	m_bRtn[5] = _func.Shuttle_Y_LOAD_Check(jig);	
	if(theConfigBank.m_System.m_bInlineMode)
	{
// 		m_bRtn[6] = theProcBank.AZoneCellSkip_Check(jig, JIG_CH_1) ? TRUE:_func.CellTagExist(jig, JIG_CH_1);	
// 		m_bRtn[7] = theProcBank.AZoneCellSkip_Check(jig, JIG_CH_2) ? TRUE:_func.CellTagExist(jig, JIG_CH_2);
		
		// Inline mode���� Loof test mode�� ��� Cell�� �׻� �ִٰ� �Ǵ��Ѵ� - LSH171124
		if( theConfigBank.m_Option.m_bUseLoofTest )
			m_bRtn[8] = TRUE;
		else
			m_bRtn[8] = ( _func.CellTagExist(jig, JIG_CH_1) ) ? TRUE:FALSE;
		m_bRtn[9] = _func.CellInfo_CheckUnloadable(jig) ? FALSE:TRUE;	
		m_bRtn[10] = m_pIF[jig]->IsStoped();
	}
	if(IsReturnOk())
	{
		// ���� �׷����� ������ MP2100 �Լ� �ѹ� ȣ���ϸ� �ǰ� ���� �ɸ���..
		// �׷��� MP2100 �Լ��� ���� �������� üũ�Ͽ� ȣ�� Ƚ���� �ִ��� ���δ� [10/17/2017 OSC]
// 		ResetReturnValue();
// 		m_bRtn[5] = _func.Shuttle_Y_LOAD_Check(jig);	
// 		if(IsReturnOk())
		{
			m_pAZone[jig]->Start();
		}
	}
}

void CThread_Main_Sequence::ConfirmStart_MoveBZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[2] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[3] = m_pBZone[jig]->IsStoped();
	m_bRtn[4] = _func.CellTagExist(m_CellPosCh1[jig], m_CellPosCh2[jig]);	
	m_bRtn[5] = _func.GetZoneEnd(jig, ZONE_ID_A);
	m_bRtn[6] = _func.GetZoneEnd(jig, ZONE_ID_B) ? FALSE:TRUE;
	if(_func.Shuttle_Y_INSP_Check(jig) && _func.GetZoneEnd(jig, ZONE_ID_MOVE_B))
		m_bRtn[7] = FALSE;	
	if(theConfigBank.m_System.m_bInlineMode)
	{
		m_bRtn[8] = _func.CellInfo_CheckUnloadable(jig) ? FALSE:TRUE;	
		m_bRtn[10] = m_pIF[jig]->IsStoped();
		m_bRtn[12] = _func.PDT_IF_ArmStatus_Check(jig) ? FALSE:TRUE;
	}
	if(IsReturnOk())
	{
		// ���� �׷����� ������ MP2100 �Լ� �ѹ� ȣ���ϸ� �ǰ� ���� �ɸ���..
		// �׷��� MP2100 �Լ��� ���� �������� üũ�Ͽ� ȣ�� Ƚ���� �ִ��� ���δ� [10/17/2017 OSC]
// 		ResetReturnValue();
// 		m_bRtn[7] = _func.Shuttle_Y_INSP_Check(jig) ? FALSE:TRUE;
// 		if(IsReturnOk())
		{
			m_pMoveBZone[jig]->Start();
		}
	}
}

void CThread_Main_Sequence::ConfirmStart_MoveAZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[2] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[3] = m_pBZone[jig]->IsStoped();
	// A, C�� �ƹ��� �Ϸ� �� ��ų�, C���� �Ϸ�Ǿ� ������ ���� [9/8/2017 OSC]
	if(_func.GetZoneEnd(jig, ZONE_ID_B))
		m_bRtn[4] = TRUE;
	else if( (_func.GetZoneEnd(jig, ZONE_ID_A) == FALSE) && (_func.GetZoneEnd(jig, ZONE_ID_B) == FALSE) )
		m_bRtn[4] = TRUE;
	else if( (_func.CellTagExist(jig, JIG_CH_1) == FALSE) )
		m_bRtn[4] = TRUE;
	else
		m_bRtn[4] = FALSE;
	if(_func.Shuttle_Y_LOAD_Check(jig) && _func.GetZoneEnd(jig, ZONE_ID_MOVE_A))
		m_bRtn[5] = FALSE;	
	if(theConfigBank.m_System.m_bInlineMode)
	{
		m_bRtn[10] = m_pIF[jig]->IsStoped();
		m_bRtn[12] = _func.PDT_IF_ArmStatus_Check(jig) ? FALSE:TRUE;
	}
	if(IsReturnOk())
	{
		// ���� �׷����� ������ MP2100 �Լ� �ѹ� ȣ���ϸ� �ǰ� ���� �ɸ���..
		// �׷��� MP2100 �Լ��� ���� �������� üũ�Ͽ� ȣ�� Ƚ���� �ִ��� ���δ� [10/17/2017 OSC]
// 		ResetReturnValue();
// 		m_bRtn[5] = _func.Shuttle_Y_LOAD_Check(jig) ? FALSE:TRUE;	
// 		if(IsReturnOk())
		{
			m_pMoveAZone[jig]->Start();
		}
	}
}

void CThread_Main_Sequence::ConfirmStart_BZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[2] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[3] = m_pBZone[jig]->IsStoped();
	m_bRtn[4] = _func.CellTagExist(m_CellPosCh1[jig], m_CellPosCh2[jig]);
	m_bRtn[5] = _func.GetZoneEnd(jig, ZONE_ID_B) ? FALSE:TRUE;
	m_bRtn[6] = _func.Shuttle_Y_INSP_Check(jig);	
	m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_MOVE_B);
	if(IsReturnOk())
	{
		// ���� �׷����� ������ MP2100 �Լ� �ѹ� ȣ���ϸ� �ǰ� ���� �ɸ���..
		// �׷��� MP2100 �Լ��� ���� �������� üũ�Ͽ� ȣ�� Ƚ���� �ִ��� ���δ� [10/17/2017 OSC]
// 		ResetReturnValue();
// 		m_bRtn[6] = _func.Shuttle_Y_INSP_Check(jig);	
// 		if(IsReturnOk())
		{
			m_pBZone[jig]->Start();
		}
	}
}
