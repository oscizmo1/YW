#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateMoveCZone : 	public CUnitCtrlBank
{
public:
	CStateMoveCZone(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_SHUTTLE)
	{
		m_Shuttle = jig;
		//kjpark 20170912 MCR Retry 추가
		m_bMCRRetry[JIG_CH_1] = FALSE;
	};
	~CStateMoveCZone(void) {};

	//kjpark 20170912 MCR 위치에서 시퀀스 추가
	enum
	{
		stepStart,	
		//kjpark 20170918 MCR 위치에서 cellSkip -> CellExist 로 체크 변경
		stepYMCRCheck,
		setpMCRRTriger,
		setpMCRReading,
		stepCellLoading,
		stepCellInfomation,
		stepTrackIn,
		stepJobProcess,
		stepJobInvalidCheck,
		//kjpark 20171010 TMD March 구현
		stepTMDMatchingCheck,
		stepZoneChange,
		stepMoveInspCzone,		
		stepEnd					
	};
	
	JIG_ID m_Shuttle;
	CInspModule_Base *m_pCurrentModule;
	//kjpark 20170912 MCR Retry 추가
	BOOL m_bMCRRetry[JIG_CH_MAX];
	int Run();
};
