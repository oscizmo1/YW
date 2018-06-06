#include "StdAfx.h"
#include "InspModule_CoprIctTest.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInspModule_CoprIctTest::CInspModule_CoprIctTest(void)
	: CInspModule_Base()
{
	m_ModuleName = INSP_MODULE_COPR_ICT_TEST;
}


CInspModule_CoprIctTest::~CInspModule_CoprIctTest(void)
{
}

int CInspModule_CoprIctTest::Run()
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

		m_Func.COPR_ICTTest_Send(m_Shuttle);
		m_Timer.Start();		
		nStep++;
		break;

	case stepEnd:				
		m_bRtn[0] = m_Func.GetCellSkipCheck(m_Shuttle, JIG_CH_1) ? m_Func.COPR_ICTTest_Check(m_Shuttle, JIG_CH_1) : TRUE;		
		if(IsReturnOk())
		{
			theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd"), m_strStateName);
			nStep = stepIdle;
		}
		else if(m_Timer.Stop(FALSE) > theConfigBank.m_TimeOut.m_dCoprIctTest)
		{
			m_Func.COPR_ICTTest_TimeOut(m_Shuttle);
			m_Timer.Start();
			theLog[m_LogIndex].AddBuf(_T("[%s] stepEnd - Time Out"), m_strStateName);
		}
		break;

	}

	m_nStep = nStep;

	return nStep;
}
