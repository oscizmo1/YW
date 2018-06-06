#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateMoveAZone : 	public CUnitCtrlBank
{
public:
	CStateMoveAZone(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_SHUTTLE)
	{
		m_Shuttle = jig;
	};
	~CStateMoveAZone(void) {};

	enum
	{
		// Z�� üũ �� Y �̵�
		stepStart,				
		stepShuttleMove,
		// Y ��ġ üũ
		stepShuttleY_AZone_Check,

		// Vacuum Off
		stepEnd					
	};

	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;

	int Run();
};
