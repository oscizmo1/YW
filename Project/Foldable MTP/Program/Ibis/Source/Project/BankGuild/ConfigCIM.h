#pragma once

#include "Device/SocketDefine_CIM.h"
//////////////////////////////////////////////////////////////////////////
// CConfigCIM
// 1. EQUIP Function List ����


class CConfigCIM
{
public:
	CConfigCIM(void);
	~CConfigCIM(void);

protected:
	EQUIP_FUNCTION CELL_TRACKING;
	EQUIP_FUNCTION TRACKING_CONTROL;
	EQUIP_FUNCTION MATERIAL_TRACKING;
	EQUIP_FUNCTION CELL_MCR_MODE;
	EQUIP_FUNCTION MATERIAL_MCR_MODE;
	EQUIP_FUNCTION LOT_ASSIGN_INFORMATION;
	EQUIP_FUNCTION AGV_ACCESS_MODE;
	EQUIP_FUNCTION AREA_SENSOR_MODE;
	EQUIP_FUNCTION SORT_MODE;
	EQUIP_FUNCTION INTERLOCK_CONTROL;	// Unit Interlock���� ����
	
public:
	void InitCIMOption();
	void ReadCIMOptionFile(CString strPath);
	void SaveCIMOptionFile(CString strPath, CString strBackupPath);

	// ���� ���� - LSH171130
	void ConfirmCIMFunction(CString strBYWHO = BYWHO_EQP);

	BOOL SetFunction(EFID_INDEX EFID, CString EFST);
	EQUIP_FUNCTION* GetFunction(EFID_INDEX EFID);

	BOOL Function_CheckUse(EFID_INDEX index);		// �� ���񿡼� ��� �Ұ����� �Ÿ� FALSE

	BOOL TransitionOfEFListState();

	BOOL CELL_MCR_MODE_Check();
	BOOL TRACKING_CONTROL_InCheck();
	BOOL TRACKING_CONTROL_OutCheck();
	BOOL INTERLOCK_CONTROL_TransferCheck();
	BOOL INTERLOCK_CONTROL_LoadingCheck();
	BOOL INTERLOCK_CONTROL_StepCheck();
};

