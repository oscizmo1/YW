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
			pClass->ConfirmStart_BZone(_func, (JIG_ID)i);
			pClass->ConfirmStart_CZone(_func, (JIG_ID)i);
			pClass->ConfirmStart_MoveAZone(_func, (JIG_ID)i);
			pClass->ConfirmStart_MoveBZone(_func, (JIG_ID)i);
			pClass->ConfirmStart_MoveCZone(_func, (JIG_ID)i);
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
		m_pCZone[jig]->Reset();
		m_pMoveAZone[jig]->Reset();
		m_pMoveBZone[jig]->Reset();
		m_pMoveCZone[jig]->Reset();
	}
}

void CThread_Main_Sequence::StateAllRun()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		m_pAZone[jig]->Run();
		m_pBZone[jig]->Run();
		m_pCZone[jig]->Run();
		m_pMoveAZone[jig]->Run();
		m_pMoveBZone[jig]->Run();
		m_pMoveCZone[jig]->Run();
	}
}

void CThread_Main_Sequence::GetStatePointer( )
{
	m_pRestart = &theThreadBank.m_stateRestart;

	m_pAZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_AZone;
	m_pAZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_AZone;

	m_pBZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_BZone;
	m_pBZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_BZone;

	m_pCZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_CZone;
	m_pCZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_CZone;

	m_pMoveAZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_MoveAZone;
	m_pMoveAZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_MoveAZone;

	m_pMoveBZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_MoveBZone;
	m_pMoveBZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_MoveBZone;

	m_pMoveCZone[JIG_ID_A] = &theThreadBank.m_stateShuttle1_MoveCZone;
	m_pMoveCZone[JIG_ID_B] = &theThreadBank.m_stateShuttle2_MoveCZone;

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
	m_bRtn[1] = m_pBZone[jig]->IsStoped();
	m_bRtn[2] = m_pCZone[jig]->IsStoped();
	m_bRtn[3] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[4] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[5] = m_pMoveCZone[jig]->IsStoped();
	// Inline mode���� Loof test mode�� ��� Cell�� �׻� �ִٰ� �Ǵ��Ѵ� - LSH171124
	if( theConfigBank.m_Option.m_bUseLoofTest )
		m_bRtn[6] = TRUE;
	else
		m_bRtn[6] = _func.CellTagExist(m_CellPosCh1[jig]);
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_MOVE_A);
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	m_bRtn[8] = _func.GetZoneEnd(jig, ZONE_ID_A) ? FALSE:TRUE;
	// ���� ��ġ Ȯ�� [6/13/2018 OSC]
	m_bRtn[9] = _func.Shuttle_Y_LOAD_Check(jig);	
	// PDT Interface Ȯ�� [6/13/2018 OSC]
	m_bRtn[10] = m_pIF[jig]->IsStoped();
	// Inline ������� Ȯ�� [6/13/2018 OSC]
	m_bRtn[11] = theConfigBank.m_System.m_bInlineMode;
	if(IsReturnOk())
	{
		m_pAZone[jig]->Start();
	}
}

void CThread_Main_Sequence::ConfirmStart_BZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pBZone[jig]->IsStoped();
	m_bRtn[2] = m_pCZone[jig]->IsStoped();
	m_bRtn[3] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[4] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[5] = m_pMoveCZone[jig]->IsStoped();
	m_bRtn[6] = _func.CellTagExist(m_CellPosCh1[jig]);
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_MOVE_B);
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	m_bRtn[8] = _func.GetZoneEnd(jig, ZONE_ID_B) ? FALSE:TRUE;
	// ���� ��ġ Ȯ�� [6/13/2018 OSC]
	m_bRtn[9] = _func.Shuttle_Y_INSP_Check(jig);	
	if(IsReturnOk())
	{
		m_pBZone[jig]->Start();
	}
}

void CThread_Main_Sequence::ConfirmStart_CZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pBZone[jig]->IsStoped();
	m_bRtn[2] = m_pCZone[jig]->IsStoped();
	m_bRtn[3] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[4] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[5] = m_pMoveCZone[jig]->IsStoped();
	if(theConfigBank.m_System.m_bInlineMode && theConfigBank.m_System.m_bInlineConfirm)
		m_bRtn[6] = _func.CellTagExist(m_CellPosCh1[jig]);
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_MOVE_C);
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	m_bRtn[8] = _func.GetZoneEnd(jig, ZONE_ID_C) ? FALSE:TRUE;
	// ���� ��ġ Ȯ�� [6/13/2018 OSC]
	m_bRtn[9] = _func.Shuttle_Y_MANUAL_Check(jig);
	if(theConfigBank.m_System.m_bInlineMode)
	{
		// �ζ����� ��� C�� ��뿩�� Ȯ�� [6/13/2018 OSC]
		m_bRtn[10] = theConfigBank.m_System.m_bInlineConfirm ? FALSE:TRUE;
	}
	if(IsReturnOk())
	{
		m_pCZone[jig]->Start();
	}
}

void CThread_Main_Sequence::ConfirmStart_MoveAZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pBZone[jig]->IsStoped();
	m_bRtn[2] = m_pCZone[jig]->IsStoped();
	m_bRtn[3] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[4] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[5] = m_pMoveCZone[jig]->IsStoped();
	// ���� ���� �������� Ȯ�� (Cell�� �ִ� ��츸) [6/13/2018 OSC]
	if(_func.CellTagExist(m_CellPosCh1[jig]))
		m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_B);
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	if(_func.Shuttle_Y_LOAD_Check(jig) == FALSE)
		m_bRtn[8] = TRUE;	
	else
		m_bRtn[8] = _func.GetZoneEnd(jig, ZONE_ID_MOVE_A) ? FALSE:TRUE;
	m_bRtn[9] = theConfigBank.m_System.m_bInlineMode;
	if(theConfigBank.m_System.m_bInlineMode)
	{
		// PDT Robot Ȯ�� [6/13/2018 OSC]
		m_bRtn[10] = m_pIF[jig]->IsStoped();
		m_bRtn[11] = _func.PDT_IF_ArmStatus_Check(jig) ? FALSE:TRUE;
	}
	if(IsReturnOk())
	{
		m_pMoveAZone[jig]->Start();
	}
}

void CThread_Main_Sequence::ConfirmStart_MoveBZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pBZone[jig]->IsStoped();
	m_bRtn[2] = m_pCZone[jig]->IsStoped();
	m_bRtn[3] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[4] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[5] = m_pMoveCZone[jig]->IsStoped();
	m_bRtn[6] = _func.CellTagExist(m_CellPosCh1[jig]);	
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	if(theConfigBank.m_System.m_bInlineMode)
	{
		if(theConfigBank.m_System.m_bInlineConfirm)
			m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_C);	// �������� C�� ����ϴ� ��� [6/13/2018 OSC]
		else
			m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_A);
	}
	else
	{
		m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_C);
	}

	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	if(_func.Shuttle_Y_INSP_Check(jig) == FALSE)
		m_bRtn[8] = TRUE;	
	else
		m_bRtn[8] = _func.GetZoneEnd(jig, ZONE_ID_MOVE_B) ? FALSE:TRUE;
	if(theConfigBank.m_System.m_bInlineMode)
	{
		// PDT Robot Ȯ�� [6/13/2018 OSC]
		m_bRtn[10] = m_pIF[jig]->IsStoped();
		m_bRtn[11] = _func.PDT_IF_ArmStatus_Check(jig) ? FALSE:TRUE;
	}
	if(IsReturnOk())
	{
		m_pMoveBZone[jig]->Start();
	}
}

void CThread_Main_Sequence::ConfirmStart_MoveCZone( CUnitCtrlFunc &_func, JIG_ID jig )
{
	ResetReturnValue();
	m_bRtn[0] = m_pAZone[jig]->IsStoped();
	m_bRtn[1] = m_pBZone[jig]->IsStoped();
	m_bRtn[2] = m_pCZone[jig]->IsStoped();
	m_bRtn[3] = m_pMoveAZone[jig]->IsStoped();
	m_bRtn[4] = m_pMoveBZone[jig]->IsStoped();
	m_bRtn[5] = m_pMoveCZone[jig]->IsStoped();
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	if(theConfigBank.m_System.m_bInlineMode)
	{
		m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_A);
	}
	else
	{
		// �ܵ��� ��� Cell�� ���� ��츸 ���� �� Ȯ�� [6/13/2018 OSC]
		if(_func.CellTagExist(m_CellPosCh1[jig]))
			m_bRtn[7] = _func.GetZoneEnd(jig, ZONE_ID_B);
	}
	// ���� ���� �������� Ȯ�� [6/13/2018 OSC]
	if(_func.Shuttle_Y_MANUAL_Check(jig) == FALSE)
		m_bRtn[8] = TRUE;	
	else
		m_bRtn[8] = _func.GetZoneEnd(jig, ZONE_ID_MOVE_C) ? FALSE:TRUE;
	if(theConfigBank.m_System.m_bInlineMode)
	{
		m_bRtn[9] = theConfigBank.m_System.m_bInlineConfirm;
		// PDT Robot Ȯ�� [6/13/2018 OSC]
		m_bRtn[10] = m_pIF[jig]->IsStoped();
		m_bRtn[11] = _func.PDT_IF_ArmStatus_Check(jig) ? FALSE:TRUE;
	}
	if(IsReturnOk())
	{
		m_pMoveCZone[jig]->Start();
	}
}
