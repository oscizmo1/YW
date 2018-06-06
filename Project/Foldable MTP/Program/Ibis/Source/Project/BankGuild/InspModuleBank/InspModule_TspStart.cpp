#include "StdAfx.h"
#include "InspModule_TspStart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInspModule_TspStart::CInspModule_TspStart(void)
	: CInspModule_Base()
{
	m_ModuleName = INSP_MODULE_TSP_START;
}


CInspModule_TspStart::~CInspModule_TspStart(void)
{
}

int CInspModule_TspStart::Run()
{
	if(m_nStep<=stepIdle)
		return m_nStep;
	ResetReturnValue();
	int nStep = m_nStep;

	switch(nStep)
	{
	case stepStart:		
		// State Name�� ��� �̸� �����ؼ� �ٽ� �����Ѵ� [9/19/2017 OSC]
		m_strStateName += m_ModuleName;
		theLog[m_LogIndex].AddBuf(_T("[%s] stepStart"), m_strStateName);

		m_Func.TSP_START_Send(m_Shuttle);
		m_Timer.Start();		
		nStep++;
		break;

	case stepEnd:				
		m_bRtn[0] = m_Func.GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? m_Func.TSP_START_Check(m_Shuttle, JIG_CH_1) : TRUE;		
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_TimeOut.m_dTspStart)
		{
			m_Func.TSP_START_TimeOut(m_Shuttle);
			m_Timer.Start();
			theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd - Time Out"), m_strStateName);
		}
		break;

	}

	m_nStep = nStep;

	return nStep;
}
