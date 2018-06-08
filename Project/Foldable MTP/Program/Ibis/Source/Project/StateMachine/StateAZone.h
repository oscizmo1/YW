#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateAZone : 	public CUnitCtrlBank
{
public:
	CStateAZone(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_AZONE)
	{
		m_Shuttle = jig;
		m_pCurrentModule = NULL;
	};
	~CStateAZone(void) {};

	enum
	{
		// A Zone Set
		stepStart,		
		stepZoneACheck,

		stepBefStart,		
		stepMustStart,		
		stepAftStart,		

		stepTMDInfoCheck,

		stepEnd					
	};

	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;

	int Run();
};
