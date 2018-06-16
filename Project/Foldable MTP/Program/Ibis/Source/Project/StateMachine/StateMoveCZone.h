#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateMoveCZone : 	public CUnitCtrlBank
{
public:
	CStateMoveCZone(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_SHUTTLE)
	{
		m_Shuttle = jig;
	};
	~CStateMoveCZone(void) {};

	enum
	{
		// Z축 체크 후 Y 이동
		stepStart,				
		// Y 위치 체크
		stepShuttleY_CZone_Check,

		// Vacuum Off
		stepEnd					
	};

	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;

	int Run();
};
