#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateCZone : 	public CUnitCtrlBank
{
public:
	CStateCZone(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_CZONE)
	{
		m_Shuttle = jig;
		m_pCurrentModule = NULL;
	};
	~CStateCZone(void) {};

	enum
	{
		stepStart,

		stepTiltUpCheck,

		stepPatternOffSwitchCheck,
		stepZoneACheck,
		stepPatternOnSwitchCheck,

		stepBefStart,		
		stepMustStart,		
		stepAftStart,

		stepReadySwitchCheck,

		stepTiltDownCheck,

		stepTMDInfoCheck,

		stepEnd					
	};

	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;

	int Run();
};