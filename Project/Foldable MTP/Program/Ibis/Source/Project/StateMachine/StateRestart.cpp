#include "StdAfx.h"
#include "StateRestart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStateRestart::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;

	ResetReturnValue();

	int nStep = m_nStep;

	//kjpark 20170915 Restart 구현
	switch(nStep)
	{
	case stepStart:			// Cell 정보에 따라 Vacuum On/Off
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);

			m_LastAlarm = ALM_NONE;
			theProcBank.PDT_IF_NoCheckVacOff_Clear();
			theTactTimeLog.Reset();

			nStep++;
			m_Timer.Start();
		}
		
		break;	
	case stepVacOnOff:
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepVacOnOff"), m_strStateName);
			Restart_VacuumOn();
			nStep++;
			m_Timer.Start();
		}		
		break;
	case stepVacChk:
		m_bRtn[0] = Restart_VacuumChk();
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepVacChk"), m_strStateName);
			Restart_ZUp();
			nStep++;
			m_Timer.Start();
		}
		else if(m_Timer.Stop(FALSE) > 4.)
		{
			SetError(m_LastAlarm);
		}		
		break;
	case stepZUpChk:
		m_bRtn[0] = Restart_ZUpChk();
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepZUpChk"), m_strStateName);
			Restart_BlowOff();
			nStep++;
			m_Timer.Start();
		}		
		else if(m_Timer.Stop(FALSE) > 4.)
		{
			SetError(m_LastAlarm);
		}		
		break;	
	case stepEnd:
		theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);

// 		if(theConfigBank.m_System.m_bInlineMode)
// 		{
// 			// StateMoveAZone이 그냥 Skip될거 같으면 여기서 ZONE_A를 날려주자 [10/6/2017 OSC]
// 			for(int jig = 0; jig < JIG_ID_MAX; jig++)
// 			{
// 				if( Shuttle_Y_LOAD_Check((JIG_ID)jig) && (theProcBank.m_bIsSetZone[(JIG_ID)jig][ZONE_ID_A] == FALSE) )
// 				{
// 					theProcBank.m_bIsSetZone[(JIG_ID)jig][ZONE_ID_A] = TRUE;
// 
// 					SetZoneA_Send((JIG_ID)jig);
// 				}
// 			}
// 		}

		theLossTimeLog.SetMachineStartTime();
		theLossTimeLog.DataToStringArray();
		theLossTimeLog.WriteLossTimeLog(FALSE);
		theLossTimeLog.SetMachineStopTime();

		theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_RUN);
		theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
			E_EQST_MATCH_EXECUTING, _T(""));
		nStep = stepHold;
		break;
	}
	m_nStep = nStep;

	return nStep;
}

//kjpark 20170915 Restart 구현
void CStateRestart::Restart_VacuumOn()
{
	if(theConfigBank.m_System.m_bInlineMode || theConfigBank.m_Option.m_bUseLoofTest)
	{
		for(int jig = 0; jig < JIG_ID_MAX; jig++)
		{
			for(int i = 0; i < JIG_CH_MAX; i++)
			{
				if(CellTagExist((JIG_ID)jig, (JIG_CH)i) || theConfigBank.m_Option.m_bUseLoofTest)
				{
					Shuttle_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_ON, BLOW_OFF );				
					Shuttle_Fpcb_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_ON, BLOW_OFF );				
				}
				else
				{
					Shuttle_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF, BLOW_ON);	
					Shuttle_Fpcb_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF, BLOW_ON);	
				}
			}
		}
	}
	else
	{
		for(int jig = 0; jig < JIG_ID_MAX; jig++)
		{
			for(int i = 0; i < JIG_CH_MAX; i++)
			{
				if(CellTagExist((JIG_ID)jig, (JIG_CH)i) && GetZoneEnd((JIG_ID)jig, ZONE_ID_A))
				{
					Shuttle_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_ON, BLOW_OFF );				
					Shuttle_Fpcb_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_ON, BLOW_OFF );				
				}
				else
				{
					Shuttle_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF, BLOW_ON);	
					Shuttle_Fpcb_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF, BLOW_ON);	
				}
			}
		}
	}
}

//kjpark 20170915 Restart 구현
BOOL CStateRestart::Restart_VacuumChk()
{	
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int i = 0; i < JIG_CH_MAX; i++)
		{
			if(CellTagExist((JIG_ID)jig, (JIG_CH)i))
			{
				if(GetZoneEnd((JIG_ID)jig, ZONE_ID_A))
				{
					if(Shuttle_Vac_Check((JIG_ID)jig, (JIG_CH)i, VAC_ON ) == FALSE)
					{
						m_LastAlarm = (ALARM_ID)(ALM_SHUTTLE_1_VAC_FAIL_CH1 + ALM_INDEX_SKIP * jig);
						return FALSE;
					}
				}
			}
		}
	}	
	
	return TRUE;
}

void CStateRestart::Restart_BlowOff()
{
	if(theConfigBank.m_System.m_bInlineMode || theConfigBank.m_Option.m_bUseLoofTest)
	{
		for(int jig = 0; jig < JIG_ID_MAX; jig++)
		{
			for(int i = 0; i < JIG_CH_MAX; i++)
			{
				if(CellTagExist((JIG_ID)jig, (JIG_CH)i) || theConfigBank.m_Option.m_bUseLoofTest)
				{
				}
				else
				{
					Shuttle_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF, BLOW_OFF);	
					Shuttle_Fpcb_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF, BLOW_OFF);	
				}
			}
		}
	}
	else
	{
		for(int jig = 0; jig < JIG_ID_MAX; jig++)
		{
			for(int i = 0; i < JIG_CH_MAX; i++)
			{
				if(CellTagExist((JIG_ID)jig, (JIG_CH)i) && GetZoneEnd((JIG_ID)jig, ZONE_ID_A))
				{
				}
				else
				{
					Shuttle_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF, BLOW_OFF);	
					Shuttle_Fpcb_Vac_OnOff((JIG_ID)jig, (JIG_CH)i, VAC_OFF, BLOW_OFF);	
				}
			}
		}
	}
}

//kjpark 20170915 Restart 구현
void CStateRestart::Restart_ZUp()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		// MTP 검사중이면 상승 안함 [11/24/2017 OSC]
		if( (MTPWrite_IsStarted((JIG_ID)jig) == FALSE) || MTPVerify_Check((JIG_ID)jig) )
		{
			Inspection_Z_UP_Move((JIG_ID)jig);
		}
		// 검사중이어도 이미 이상한 위치면 이동
		else if(Inspection_Z_INSP_Check((JIG_ID)jig) == FALSE)
		{
			Inspection_Z_UP_Move((JIG_ID)jig);
		}
	}
}

//kjpark 20170915 Restart 구현
BOOL CStateRestart::Restart_ZUpChk()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		if( (MTPWrite_IsStarted((JIG_ID)jig) == FALSE) || MTPVerify_Check((JIG_ID)jig) )
		{
			if(Inspection_Z_UP_Check((JIG_ID)jig) == FALSE)
			{			
				m_LastAlarm = (ALARM_ID)(ALM_AXIS_INSPECTION_Z1_NOT_UP_POSITION + ALM_INDEX_SKIP * jig);
				return FALSE;
			}
		}
		else if(Inspection_Z_INSP_Check((JIG_ID)jig) == FALSE)
		{
			if(Inspection_Z_UP_Check((JIG_ID)jig) == FALSE)
			{			
				m_LastAlarm = (ALARM_ID)(ALM_AXIS_INSPECTION_Z1_NOT_UP_POSITION + ALM_INDEX_SKIP * jig);
				return FALSE;
			}
		}
	}
	return TRUE;
}
