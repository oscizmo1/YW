#pragma once

#include "UnitStatus.h"

class CUnitStatusBank
{
public:
	CUnitStatusBank(void);
	~CUnitStatusBank(void);

	CUnitStatus m_Unit[JIG_ID_MAX][JIG_CH_MAX];

	void SetUnitID(CString strEQPID);

	BOOL FindUnit(CString strUnitID, JIG_ID &jig, JIG_CH &ch);

	void SetCellInfo(JIG_ID jig, JIG_CH ch, CCellInfo *pCell);

	void SetMoveStatus(JIG_ID jig, JIG_CH ch, STATUS_MOVE value);
	void SetInterlock(CString strUnitID);
	void SetInterlockCinform();

	void SaveStatus();	
	void ReadStatus();
	void StatusInit();
	void SyncNotUseOption();
};

