#include "StdAfx.h"
#include "InspModule_CellLoading.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInspModule_CellLoading::CInspModule_CellLoading(void)
	: CInspModule_Base()
{
	m_ModuleName = INSP_MODULE_CELL_LOADING;
}


CInspModule_CellLoading::~CInspModule_CellLoading(void)
{
}

int CInspModule_CellLoading::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;
	ResetReturnValue();
	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:		
		// State Name을 모듈 이름 포함해서 다시 지정한다 [9/19/2017 OSC]
		m_strStateName += m_ModuleName;
		theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);

// 		// Start 시간만 기록하고 실제 Cell Loading은 작업자 버튼으로 수행 [9/8/2017 OSC]
// 		m_Func.CellLoading_SetTimeStart(m_Shuttle, JIG_CH_1);
// 		m_Func.CellLoading_SetTimeStart(m_Shuttle, JIG_CH_2);
		m_Timer.Start();		
		nStep++;
		break;


		// 1st Recv Check -> if retry -> Reset
	case stepRecvCheck:				
		if(theConfigBank.m_System.m_bInlineMode)
		{
			m_bRtn[0] = theProcBank.AZoneCellNG_Check(m_Shuttle, JIG_CH_1) ? TRUE:m_Func.CellLoading_RecvCheck(m_Shuttle, JIG_CH_1);			
		}
		else
		{
			m_bRtn[0] = m_Func.GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? m_Func.CellLoading_GoodCheck(m_Shuttle, JIG_CH_1) : TRUE;			
		}
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepRecvCheck"), m_strStateName);

			if( theConfigBank.m_System.m_bInlineMode
				&& theConfigBank.m_Option.m_bUseRetryAA )
			{
				// AA 리트라이 여부 확인
				m_bRtn[0] = theProcBank.AZoneCellNG_Check(m_Shuttle, JIG_CH_1) ? TRUE:m_Func.CellLoading_GoodCheck(m_Shuttle, JIG_CH_1);		
				if(IsReturnOk())
				{
					nStep = stepEnd;
				}
				else
				{
					// Reset을 날려 컨택을 푼다
					if(m_bRtn[0] == FALSE)	m_Func.PatternReset_Send(m_Shuttle, JIG_CH_1);
					m_Timer.Start();		
					nStep++;
				}
			}
			else
			{
				nStep = stepEnd;
			}
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_TimeOut.m_dCellLoading)
		{
			if(theConfigBank.m_System.m_bInlineMode)
			{
				m_Func.CellLoading_TimeOut(m_Shuttle);
			}
		}
		break;

		// Reset Delay -> Cell Loading
	case stepResetDelay:				
		m_bRtn[0] = m_Timer.Stop(FALSE) > 2. ? TRUE:FALSE;
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepResetDelay"), m_strStateName);

			// 리트라이 할 것만 다시 날린다
			if(theProcBank.AZoneChannelNotUse_Check(m_Shuttle, JIG_CH_1) == FALSE)
			{
				if(m_Func.CellLoading_GoodCheck(m_Shuttle, JIG_CH_1) == FALSE)
				{
					m_Func.CellLoading_InitInfo(m_Shuttle, JIG_CH_1, TRUE);
					m_Func.CellLoading_Send(m_Shuttle, JIG_CH_1, TRUE);
				}
			}
			m_Timer.Start();		
			nStep++;
		}
		break;

	case stepEnd:				
		if(theConfigBank.m_System.m_bInlineMode)
		{
			m_bRtn[0] = theProcBank.AZoneCellNG_Check(m_Shuttle, JIG_CH_1) ? TRUE:m_Func.CellLoading_RecvCheck(m_Shuttle, JIG_CH_1);			
		}
		else
		{
			m_bRtn[0] = m_Func.GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? m_Func.CellLoading_GoodCheck(m_Shuttle, JIG_CH_1) : TRUE;		
		}
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
			m_Func.CellLoading_SetEndTime(m_Shuttle, JIG_CH_1);
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_TimeOut.m_dCellLoading)
		{
			if(theConfigBank.m_System.m_bInlineMode)
			{
				m_Func.CellLoading_TimeOut(m_Shuttle);
			}
		}
		break;

	}

	m_nStep = nStep;

	return nStep;
}
