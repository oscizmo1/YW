#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateBZone : 	public CUnitCtrlBank
{
public:
	CStateBZone(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_BZONE)
	{
		m_Shuttle = jig;
		m_pCurrentModule = NULL;
	};
	~CStateBZone(void) {};

	enum
	{
		stepStart,

		stepPatternOffSwitchCheck,
		stepZoneACheck,
		stepPatternOnSwitchCheck,

		stepBefStart,		
		stepMustStart,		
		stepAftStart,

		stepReadySwitchCheck,

		stepTMDInfoCheck,

		stepEnd					
	};

	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;

	int Run();
};