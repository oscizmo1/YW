#include "StdAfx.h"
#include "InspModule_Base.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CInspModule_Base::CInspModule_Base(void)
	: m_Func(MP2100_THREAD_MAIN_PROCESS)
{
	m_ModuleName.Empty();
	m_Shuttle = JIG_ID_MAX;
}


CInspModule_Base::~CInspModule_Base(void)
{
}

void CInspModule_Base::SetShuttle( JIG_ID jig )
{
	m_Shuttle = jig;
}

void CInspModule_Base::Start()
{
	CUnitCtrl_Base::Start();
	// 꼭 이 한줄을 잊지 말것 [8/10/2017 OSC]
	m_Func.SetParam(m_strStateName, m_nThreadID, FALSE, m_LogIndex);
}
