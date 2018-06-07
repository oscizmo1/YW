#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateMoveCZone : 	public CUnitCtrlBank
{
public:
	CStateMoveCZone(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_SHUTTLE)
	{
		m_Shuttle = jig;
		//kjpark 20170912 MCR Retry �߰�
		m_bMCRRetry[JIG_CH_1] = FALSE;
	};
	~CStateMoveCZone(void) {};

	//kjpark 20170912 MCR ��ġ���� ������ �߰�
	enum
	{
		stepStart,	
		//kjpark 20170918 MCR ��ġ���� cellSkip -> CellExist �� üũ ����
		stepYMCRCheck,
		setpMCRRTriger,
		setpMCRReading,
		stepCellLoading,
		stepCellInfomation,
		stepTrackIn,
		stepJobProcess,
		stepJobInvalidCheck,
		//kjpark 20171010 TMD March ����
		stepTMDMatchingCheck,
		stepZoneChange,
		stepMoveInspCzone,		
		stepEnd					
	};
	
	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;
	//kjpark 20170912 MCR Retry �߰�
	BOOL m_bMCRRetry[JIG_CH_MAX];
	int Run();
};
