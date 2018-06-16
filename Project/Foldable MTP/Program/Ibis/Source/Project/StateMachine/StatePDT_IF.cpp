#include "stdafx.h"
#include "StatePDT_IF.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CStatePDT_IF::Run()
{
	if(m_nStep<=stepIdle)
	{
		//kjpark 20180125 PDT Interface time out alarm 30->120
		m_TimerServival.Start();
		return m_nStep;
	}

	
	//kjpark 20180125 PDT Interface time out alarm 30->120
	if(PDT_IF_Servival_Check(m_Shuttle) == FALSE
		&& m_TimerServival.Stop(FALSE) > 120.)
	{
		theLog[m_LogIndex].AddBuf(_T("[%s] PDT_IF_Servival_Check FAIL Not connect"), m_strStateName);
		//SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_PDT_ROBOT_STOP, m_Shuttle));
		return m_nStep;
	}
	else
	{
		m_TimerServival.Start();
	}

	ResetReturnValue();

	int nStep = m_nStep;

	switch(nStep)
	{

	case stepStart:
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);
			theTactTimeLog.m_Interface[m_Shuttle].Start();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 5.)
		{
		}
		break;

		// PDT Able On Check -> MTP Start On
	case stepPDT_AbleCheck:
		m_bRtn[0] = PDT_IF_Able_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_AbleCheck"), m_strStateName);

			CellLog_WaitTime_SetEndTime(m_Shuttle);

			m_bVacOutput[JIG_CH_1] = Shuttle_VacOut_Check(m_Shuttle, JIG_CH_1, VAC_ON);
			MTP_IF_Start_OnOff(m_Shuttle, TRUE);
// 			// 파기가 안되서 Vac Off를 미리 해 놓고 PDT 신호만 끄지 않는다 [12/8/2017 OSC]
// 			if(CellTagExist(m_Shuttle, JIG_CH_1))
// 			{
// 				theProcBank.m_bPDT_IF_NoCheckVacOff[m_Shuttle][JIG_CH_1] = TRUE;
// 			}
// 			Shuttle_Vac_OnOff(m_Shuttle, JIG_CH_1, VAC_OFF);
			m_Timer.Start();
			nStep++;
		}
		//kjpark 20180125 PDT Interface time out alarm 30->120
		else if(m_Timer.Stop(FALSE) > 120.)
		{
			if(m_bRtn[0] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_PDT_ABLE_CHECK, m_Shuttle));
		}
		break;

		// PDT Vacuum On Check -> Vacuum On
		// PDT Vacuum Off Check -> Vacuum Off
	case stepPDT_VacOnOff:
		m_bRtn[1] = PDT_IF_Complete_Check(m_Shuttle);
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_VacOnOff"), m_strStateName);
			// 혹시 아직도 살아있으면 해제 [12/8/2017 OSC]
			theProcBank.m_bPDT_IF_NoCheckVacOff[m_Shuttle][JIG_CH_1] = FALSE;

			m_Timer.Start();
			nStep++;
		}
		else
		{
			//kjpark 20180125 PDT Interface time out alarm 30->120
			if(m_Timer.Stop(FALSE) > 120.)
			{
				if(m_bRtn[1] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_PDT_COMPLETE_CHECK, m_Shuttle));
			}
			else
			{
				// Complete 살릴 때 까지 로봇이 신호 주는 대로 On Off를 살린다
				BOOL bValue;
				for(int i = 0; i < JIG_CH_MAX; i++)
				{
					bValue = PDT_IF_VacOnReq_Check(m_Shuttle, (JIG_CH)i);
					theLog[m_LogIndex].AddBuf(_T("bValue = PDT_IF_VacOnReq_Check(m_Shuttle, (JIG_CH)i)"));
					if( bValue && (m_bVacOutput[i] == FALSE) )
					{
						m_bVacOutput[i] = TRUE;
						// FPC 들림 때문에 VAC On 신호 줄 때 Contact 시도 [12/8/2017 OSC]
						CellLoading_InitInfo(m_Shuttle, (JIG_CH)i, FALSE);
						CellLoading_SetStartTime(m_Shuttle, (JIG_CH)i);
						AZone_SetTimeStart(m_Shuttle); //20180312 AZONE TACT TIME 변경
						CellLoading_Send(m_Shuttle, (JIG_CH)i, TRUE);
						theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_VacOnOff JIG%c CH%d Contact On"), 
							m_strStateName, _T('A')+m_Shuttle, i+1);

						//////////////////////////////////////////////////////////////////////////
						// Vac On을 하지 말고 Contact 하라고 해서 다시 강제비트 On [12/19/2017 OSC]
						theProcBank.m_bPDT_IF_NoCheckVacOff[m_Shuttle][i] = TRUE;
						//////////////////////////////////////////////////////////////////////////
						Shuttle_Vac_OnOff(m_Shuttle, (JIG_CH)i, VAC_ON, BLOW_OFF);
						theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_VacOnOff JIG%c CH%d Vac On"), 
							m_strStateName, _T('A')+m_Shuttle, i+1);
						//////////////////////////////////////////////////////////////////////////

					}
					bValue = PDT_IF_VacOffReq_Check(m_Shuttle, (JIG_CH)i);
					if( bValue && m_bVacOutput[i] )
					{
						m_bVacOutput[i] = FALSE;
						// PDT에서 On or Off신호가 오면 이전까지 강제로 On을 살려준걸 해제 [12/8/2017 OSC]
						theProcBank.m_bPDT_IF_NoCheckVacOff[m_Shuttle][i] = FALSE;
						Shuttle_Vac_OnOff(m_Shuttle, (JIG_CH)i, VAC_OFF, BLOW_ON);	// 나중에 OFF 구현 필요 [6/16/2018 OSC]
						theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_VacOnOff JIG%c CH%d Vac Off"), 
							m_strStateName, _T('A')+m_Shuttle, i+1);
					}
				}
			}
		}
		break;

		// PDT Complete On -> MTP Complete On
	case stepPDT_CompleteCheck:
		m_bRtn[0] = PDT_IF_Start_Check(m_Shuttle);
		m_bRtn[1] = PDT_IF_Complete_Check(m_Shuttle);
// 		m_bRtn[2] = m_bVacOutput[JIG_CH_1] ? Shuttle_Vac_Check(m_Shuttle, JIG_CH_1, VAC_ON):TRUE;
// 		m_bRtn[3] = m_bVacOutput[JIG_CH_2] ? Shuttle_Vac_Check(m_Shuttle, JIG_CH_2, VAC_ON):TRUE;
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_CompleteCheck"), m_strStateName);

			// 로봇이 언로딩과 로딩을 동시에 하기에 여기서 Vac 상태를 보고
			// Cell정보 삭제와 재생성을 한다
			for(int i = 0; i< JIG_CH_MAX; i++)
			{
				// 일단 기존 Cell은 삭제
				if(theCellBank.GetCellTag(m_Shuttle, (JIG_CH)i).IsExist())
				{
					theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_CompleteCheck CH%d Unloaded"), m_strStateName, i+1);
					// Loading Stop인 경우 R인 것을들 전부 L로 보고하고 리트라이를 취소한다 [12/1/2017 OSC]
					if(theProcBank.LoadingStop_IsRequire())
						CIM_CellLoadingStop(m_Shuttle);
					CIM_CellTrackOut(m_Shuttle, (JIG_CH)i);
					CellLog_Write(m_Shuttle, (JIG_CH)i);
					InlineCellData_Remove(m_Shuttle, (JIG_CH)i);
					// CellSkip Reset [9/16/2017 OSC]
					AZoneCellSkip_Reset(m_Shuttle);
				}
				// Vac On 신호를 주고 있으면 생성
				if(m_bVacOutput[i])
				{
					m_strCellID[i] = PDT_IF_CellID_Read(m_Shuttle, (JIG_CH)i);
					InlineCellData_Create(m_Shuttle, (JIG_CH)i, m_strCellID[i]);
					theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_CompleteCheck Load CellID CH%d:%s"), 
						m_strStateName, i+1, m_strCellID[i]);
				}

				MTP_IF_CellInfo_Write(m_Shuttle, (JIG_CH)i);
			}

			MTP_IF_Complete_OnOff(m_Shuttle, TRUE);
			m_Timer.Start();
			nStep++;
		}
		//kjpark 20180125 PDT Interface time out alarm 30->120
		else if(m_Timer.Stop(FALSE) > 120.)
		{
			if(m_bRtn[0] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_PDT_START_CHECK, m_Shuttle));
			if(m_bRtn[1] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_PDT_COMPLETE_CHECK, m_Shuttle));
			if(m_bRtn[2] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH1, m_Shuttle));
			if(m_bRtn[3] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH2, m_Shuttle));
		}
		break;

		// PDT Complete On -> MTP Complete On
	case stepPDT_BitOffCheck:
		m_bRtn[0] = PDT_IF_Able_Check(m_Shuttle) ? FALSE:TRUE;
		m_bRtn[1] = PDT_IF_Start_Check(m_Shuttle) ? FALSE:TRUE;
		m_bRtn[2] = PDT_IF_Complete_Check(m_Shuttle) ? FALSE:TRUE;
		m_bRtn[3] = PDT_IF_VacOnReq_Check(m_Shuttle, JIG_CH_1) ? FALSE:TRUE;
		m_bRtn[5] = PDT_IF_VacOffReq_Check(m_Shuttle, JIG_CH_1) ? FALSE:TRUE;
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepPDT_BitOffCheck"), m_strStateName);
			MTP_IF_Able_OnOff(m_Shuttle, FALSE);
			MTP_IF_Start_OnOff(m_Shuttle, FALSE);
			MTP_IF_Complete_OnOff(m_Shuttle, FALSE);

			m_Timer.Start();
			nStep++;
		}
		//kjpark 20180125 PDT Interface time out alarm 30->120
		else if(m_Timer.Stop(FALSE) > 120.)
		{
			SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_PDT_BIT_CLEAR_CHECK, m_Shuttle));
		}
		break;
	
	case stepEnd:

		theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
		theTactTimeLog.m_Interface[m_Shuttle].End();
		CellLog_LoadingTactTime_SetStartTime(m_Shuttle);
		nStep = stepIdle;
		break;
	}

	if(m_nStep != stepIdle)
		m_nStep = nStep;

	return nStep;
}
