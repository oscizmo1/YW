#pragma once

#include "UnitStatus.h"
#include "MachineDefine.h"

class CUnitStatusBank
{
public:
	CUnitStatusBank(void);
	~CUnitStatusBank(void);

	CUnitStatus m_Unit[JIG_ID_MAX][JIG_CH_MAX];

	int GetUnitCount();
	CUnitStatus *GetUnit(CString strUnitID);

	void SetUnitID(CString strEQPID);
// 	void SetMoveStatus(JIG_ID jig, JIG_CH ch, STATUS_MOVE value);

// 	void SaveStatus();	
// 	void ReadStatus();
// 	void SyncNotUseOption();
};

