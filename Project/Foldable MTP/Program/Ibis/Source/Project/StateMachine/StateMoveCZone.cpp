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
	BOOL bNext = TRUE;

	//kjpark 20170912 MCR 위치에서 시퀀스 추가
	switch(nStep)
	{
	//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
	case stepStart:		
		m_bRtn[0] = CellTagExist(m_Shuttle, JIG_CH_1) ? Shuttle_Vac_Check(m_Shuttle, JIG_CH_1, VAC_ON):TRUE;
		//kjpark 20170913 MCR 위치에서 Z축  체크
		m_bRtn[2] = LightCurtain_Check(m_Shuttle);
		m_bRtn[3] = Inspection_Z_UP_Check(m_Shuttle);
		if(IsReturnOk())
		{
			// State Name을 셔틀 이름 포함해서 다시 지정한다 [9/19/2017 OSC]
			theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);
			//kjpark 20170912 MCR Retry 추가
			m_bMCRRetry[JIG_CH_1] = FALSE;

			MCR_Trigger(m_Shuttle, JIG_CH_1, OFF);

			
			//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
			Shuttle_Y_MANUAL_Move(m_Shuttle);
			Inspection_X_INSP_Move(m_Shuttle);
			theTactTimeLog.m_Shuttle_MoveMCR[m_Shuttle].Start();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 30.)
		{	
			if(m_bRtn[0] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH1, m_Shuttle));
			else if(m_bRtn[1] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH2, m_Shuttle));
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			else if(m_bRtn[2] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			//kjpark 20170913 MCR 위치에서 Z축  체크
			else if(m_bRtn[3] == FALSE)	SetError(GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_Z1_NOT_UP_POSITION, m_Shuttle));
		}
		break;
	case stepYMCRCheck:
		{
			m_bRtn[0] = LightCurtain_Check(m_Shuttle);
			m_bRtn[1] = Shuttle_Y_MANUAL_Check(m_Shuttle);
			m_bRtn[2] = Inspection_X_INSP_Check(m_Shuttle);
			if(IsReturnOk())
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] stepYMCRCheck"), m_strStateName);
				theTactTimeLog.m_Shuttle_MoveMCR[m_Shuttle].End();
				AZonetoMCR_SetTimeEnd(m_Shuttle);
				CellLog_MCRReadTime_SetStartTime(m_Shuttle);
				m_Timer.Start();
				nStep++;
			}
			else if(m_Timer.Stop(FALSE) > 10.)
			{
				// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
				if(m_bRtn[0] == FALSE)			SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
				else if(m_bRtn[1] == FALSE)		SetError(GetAlarmID_of_Shuttle(ALM_AXIS_JIG_SHUTTLE_Y1, m_Shuttle));
				else if(m_bRtn[2] == FALSE)		SetError(GetAlarmID_of_Shuttle(ALM_AXIS_INSPECTION_X1, m_Shuttle));
			}
		}
		break;
	//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
	case setpMCRRTriger:		
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] setpMCRRTriger"), m_strStateName);
			if(theProcBank.m_bDryRunMode == FALSE)
			{				
				if(theConfigBank.m_CIM.CELL_MCR_MODE_Check() == FALSE)
				{
					theProcBank.m_strLastCellID[m_Shuttle][JIG_CH_1] = SKIP;
					theProcBank.m_bMCRResultRecive[m_Shuttle][JIG_CH_1] = TRUE;
				}
				else
				{
					if(MCR_SuccessCheck(m_Shuttle, JIG_CH_1) == FALSE) MCR_Trigger(m_Shuttle, JIG_CH_1, ON);
				}

			}	
			if(theProcBank.m_bMCRStepisFirstStep[m_Shuttle] == FALSE)
			{
				theProcBank.m_bMCRStepisFirstStep[m_Shuttle] = TRUE;

			}			
			m_Timer.Start();
			nStep++;
		}
		
		break;
	case setpMCRReading:
		
		if(theProcBank.m_bDryRunMode == FALSE)
		{
			//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
			m_bRtn[0] = MCR_SuccessCheck(m_Shuttle, JIG_CH_1) ? TRUE : GetMCR_ResultRecive(m_Shuttle, JIG_CH_1);
		}		
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] setpMCRReading"), m_strStateName);

			for(int i = 0; i < JIG_CH_MAX; i++)
			{
				MCR_Trigger(m_Shuttle, (JIG_CH)i, OFF);
				// Cell ID가 성공하지 않은 것만 넣는다 [9/18/2017 OSC]
				if( (MCR_SuccessCheck(m_Shuttle, (JIG_CH)i) == FALSE))
				{
					SetMCRReadingID(m_Shuttle, (JIG_CH)i);
					// Cell ID 넣은 다음에 다시 성공 여부 확인 [9/18/2017 OSC]
					if( (MCR_SuccessCheck(m_Shuttle, (JIG_CH)i) == FALSE))
					{
						if(m_bMCRRetry[i])
						{
							SaveLastMCRImage(m_Shuttle, (JIG_CH)i);										
							m_bMCRRetry[i] = FALSE;
						}
						else
						{
							m_bMCRRetry[i] = TRUE;
						}
					}
				}
				else
				{
					m_bMCRRetry[i] = FALSE;
				}
			}

			if(m_bMCRRetry[JIG_CH_1])
			{
				theLog[m_LogIndex].AddBuf(_T("[%s] setpMCRReading Channel 1 - Retry"), m_strStateName);
				nStep = setpMCRRTriger;
			}
			else
			{
				nStep = stepCellLoading;
			}

			m_Timer.Start();			
		}
		else if(m_Timer.Stop(FALSE) > 3.)
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] setpMCRReading - Time out"),m_strStateName);
			if(m_bRtn[0] == FALSE)	
			{
				theProcBank.m_strLastCellID[m_Shuttle][JIG_CH_1] = TEXT_FAIL;
				theProcBank.m_bMCRResultRecive[m_Shuttle][JIG_CH_1] = TRUE;
			}				
			m_Timer.Start();
		}
		break;
	case stepCellLoading:
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepCellLoading"), m_strStateName);
			if(theProcBank.m_bDryRunMode == FALSE)
			{
				// Retry Check [9/27/2017 OSC]
				CIM_CellRetryCheck(m_Shuttle);
				CellLoading_Send(m_Shuttle, JIG_CH_1, FALSE);		
			}			
			m_Timer.Start();
			nStep++;
		}		
		break;
	case stepCellInfomation:
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepCellInfomation"), m_strStateName);
			if(theProcBank.m_bDryRunMode == FALSE)
			{
				if(theConfigBank.m_Option.m_bUseCellInforRequest)//셀정보 얻어오는것두 옵션 처리 하도록 함
					CIM_CellInfoRequest(m_Shuttle);
			}
			m_Timer.Start();
			nStep++;
		}		
		break;
	case stepTrackIn:
// 		if(theProcBank.m_bDryRunMode == FALSE)
// 		{
// 			//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
// 			m_bRtn[0] = CellTagExist(m_Shuttle, JIG_CH_1) ? CheckCellInfomationSuccess(m_Shuttle, JIG_CH_1) : TRUE;
// 			m_bRtn[1] = CellTagExist(m_Shuttle, JIG_CH_2) ? CheckCellInfomationSuccess(m_Shuttle, JIG_CH_2) : TRUE;
// 		}		
// 		if(IsReturnOk())
// 		{
// 			theLog[m_LogIndex].AddBuf(_T("[%s] stepTrackIn"), m_strStateName);
// 			CIM_CellTrackIn(m_Shuttle);
// 			m_Timer.Start();
// 			nStep++;
// 		}
// 		else if((m_Timer.Stop(FALSE) > 60.) || CheckCellInfomationRecive(m_Shuttle))
// 		{
// 			if(IsError() == FALSE)
// 			{
// 				CIM_SetCellInfoNG(m_Shuttle);
// 				if(theConfigBank.m_System.m_bInlineMode == FALSE)
// 				{
// 					SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_CELLINFO_NG, m_Shuttle));
// 				}
// 			}
// 		}	

		if(theProcBank.m_bDryRunMode == FALSE)
		{
			//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
			m_bRtn[0] = CellTagExist(m_Shuttle, JIG_CH_1) ? CheckCellInfomationRecive(m_Shuttle, JIG_CH_1) : TRUE;
		}		
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepTrackIn"), m_strStateName);
			m_bRtn[0] = CellTagExist(m_Shuttle, JIG_CH_1) ? CheckCellInfomationSuccess(m_Shuttle, JIG_CH_1) : TRUE;
			if(IsReturnOk() == FALSE)
			{
				CIM_SetCellInfoNG(m_Shuttle);
				if( (IsError() == FALSE) && (theConfigBank.m_System.m_bInlineMode == FALSE) )
				{
					SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_CELLINFO_NG, m_Shuttle));
				}
			}
			CIM_CellTrackIn(m_Shuttle);
			m_Timer.Start();
			nStep++;
		}
		else if((m_Timer.Stop(FALSE) > 60.))
		{
			if(IsError() == FALSE)
			{
				CIM_SetCellInfoNG(m_Shuttle);
				if(theConfigBank.m_System.m_bInlineMode == FALSE)
				{
					SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_CELLINFO_NG, m_Shuttle));
				}
			}
		}		
		break;
	case stepJobProcess:
		if(theProcBank.m_bDryRunMode == FALSE)
		{
			//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
			m_bRtn[0] = CellTagExist(m_Shuttle, JIG_CH_1) ? CheckCellJobStartRecive(m_Shuttle, JIG_CH_1): TRUE;
		}
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepJobProcess"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}		
		else if( m_Timer.Stop(FALSE) > 30. && (IsError() == FALSE) )  //10초 미만시 잡프로세스 받았는지 확인 그 이후는 타임아웃
		{
			CIM_SetTrackOutNG(m_Shuttle);
			if(theConfigBank.m_System.m_bInlineMode == FALSE)
			{
				SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_INVALID_TIME_OVER, m_Shuttle));
			}
		}
		break;
	case stepJobInvalidCheck:
		if(theProcBank.m_bDryRunMode == FALSE)
		{
			//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
			m_bRtn[0] = CellTagExist(m_Shuttle, JIG_CH_1) ? CheckCellJobStartSuccess(m_Shuttle, JIG_CH_1): TRUE;
		}
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepJobInvalidCheck"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}		
		else if(IsError() == FALSE) 
		{
			CIM_SetTrackOutNG(m_Shuttle);
			if(theConfigBank.m_System.m_bInlineMode == FALSE)
			{
				SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_JOB_INVALID, m_Shuttle));
			}
		}
		break;
	//kjpark 20171010 TMD March 구현
	case stepTMDMatchingCheck:
		if( (theProcBank.m_bDryRunMode == FALSE) && (theConfigBank.m_Option.m_bUseTMDNameMatch) )
		{
			m_bRtn[0] = GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? CheckTMDnCellIDMatch(m_Shuttle, JIG_CH_1): TRUE;
			//kjpark 2018018 TMD MACHING 특정 ProductID일때 전문자열 비교
			//kjpark 20180120 Cell Mixing 처리(강원호 선임)
			m_bRtn[2] = GetCellSkipCheck(m_Shuttle, JIG_CH_1, TRUE) ? CheckTMDnProductIDMatch(m_Shuttle, JIG_CH_1): TRUE;
		}
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepTMDMatchingCheck"), m_strStateName);
			m_Timer.Start();
			nStep++;
		}	
		else
		{
			if(theConfigBank.m_Option.m_bUseTMDNameMatch)
			{
				//kjpark 2018018 TMD MACHING 특정 ProductID일때 전문자열 비교
				//kjpark 20180120 Cell Mixing 처리(강원호 선임)
				if( (m_bRtn[0] == FALSE) || (m_bRtn[2] == FALSE))
				{
					theLog[m_LogIndex].AddBuf(_T("[%s] STEP_TMD_ID_CHECK -> Cell Mixing on Chanel1"), m_strStateName);	
					CellMixingBin2Input(m_Shuttle, JIG_CH_1);
				}
				//kjpark 2018018 TMD MACHING 특정 ProductID일때 전문자열 비교
				//kjpark 20180120 Cell Mixing 처리(강원호 선임)
				//else if( (m_bRtn[1] == FALSE) || (m_bRtn[3] == FALSE))
				//{
				//	theLog[m_LogIndex].AddBuf(_T("[%s] STEP_TMD_ID_CHECK -> Cell Mixing on Chanel2"), m_strStateName);
				//	CellMixingBin2Input(m_Shuttle, JIG_CH_2);
				//}

// 				if( (m_bRtn[0] == FALSE))	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_TMD_NG_CH1, m_Shuttle));
// 				else if( (m_bRtn[1] == FALSE))	SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_TMD_NG_CH2, m_Shuttle));			
			}
// 			else//20171001 TMDNameMatch옵션 값이 off로 되어 있다면 해당 Cell은 bin2처리 합니다.
// 			{
// 			}
		}
		break;
	case stepZoneChange:
		theLog[m_LogIndex].AddBuf(_T("[%s] stepZoneChange"), m_strStateName);
		CellLog_MCRReadTime_SetEndTime(m_Shuttle);
		Shuttle_Y_INSP_Move(m_Shuttle);
		MCRtoCZoneSetTimeStart(m_Shuttle);
		theTactTimeLog.m_Shuttle_MoveCZone[m_Shuttle].Start();
		m_Timer.Start();
		nStep++;
		break;
	case stepMoveInspCzone:
		m_bRtn[0] = LightCurtain_Check(m_Shuttle);
		m_bRtn[1] = Shuttle_Y_INSP_Check(m_Shuttle);
		m_bRtn[2] = CellTagExist(m_Shuttle, JIG_CH_1) ? Shuttle_Vac_Check(m_Shuttle, JIG_CH_1, VAC_ON):TRUE;
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepMoveInspCzone"), m_strStateName);
			theTactTimeLog.m_Shuttle_MoveCZone[m_Shuttle].End();
			m_Timer.Start();
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			// 항상 모터 체크보다 Light Curtain을 먼저 체크해서 우선권을 둔다 [9/27/2017 OSC]
			if(m_bRtn[0] == FALSE)			SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE1_LIGHT_CURTAIN, m_Shuttle));
			else if(m_bRtn[1] == FALSE)		SetError(GetAlarmID_of_Shuttle(ALM_AXIS_JIG_SHUTTLE_Y1, m_Shuttle));
			else if(m_bRtn[2] == FALSE)		SetError(GetAlarmID_of_Shuttle(ALM_SHUTTLE_1_VAC_FAIL_CH1, m_Shuttle));
		}
		break;	
	case stepEnd:
		theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
		{
			theProcBank.m_bMCRStepisFirstStep[m_Shuttle] = FALSE;
			SetZoneEnd(m_Shuttle, ZONE_ID_MOVE_C);
			nStep = stepIdle;
		}		
		break;
	}

	if(m_nStep != stepIdle)
	{
		// 시퀀스가 변경됬다면 Warning이 해결됬다고 간주 [9/27/2017 OSC]
		if(m_nStep != nStep)
		{
			m_nError = ALM_NONE;
			m_nStep = nStep;
		}
	}

	return nStep;
}
