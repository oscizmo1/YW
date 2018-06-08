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
		//kjpark 20180122 Y Shuttle 정지 후 다시 런하면 알람치는 시퀀스 수정
		stepY_MoveCheck,
		stepX_INSPCheck,
		stepZ_DownChk,

		stepZoneCCheck,

		stepBefStart,		
		stepMustStart,		
		stepAftStart,		

		stepZ_UpCheck,		

		stepEnd					
	};

	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;

	int Run();
};
