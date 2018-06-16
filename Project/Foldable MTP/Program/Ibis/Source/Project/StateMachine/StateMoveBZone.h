#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateMoveBZone : 	public CUnitCtrlBank
{
public:
	CStateMoveBZone(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_SHUTTLE)
	{
		m_Shuttle = jig;
	};
	~CStateMoveBZone(void) {};

	//kjpark 20170912 MCR ��ġ���� ������ �߰�
	enum
	{
		stepStart,	
		stepCellInfomation,
		stepTrackIn,
		stepJobProcess,
		stepJobInvalidCheck,
		//kjpark 20171010 TMD Match ����
		stepTMDMatchingCheck,
		stepZoneChange,
		stepMoveInspCzone,		
		stepEnd					
	};
	
	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;
	int Run();
};
