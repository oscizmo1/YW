#include "StdAfx.h"
#include "ConfigCIM.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigCIM::CConfigCIM(void)
{
	InitCIMOption();
}


CConfigCIM::~CConfigCIM(void)
{
}

void CConfigCIM::InitCIMOption()
{
	CELL_TRACKING.USE = TRUE;
	CELL_TRACKING.EFID = 1;
	CELL_TRACKING.EFNAME = _T("CELL TRACKING");
	CELL_TRACKING.EFST_TYPE1 = EFST_ON;	// default
	CELL_TRACKING.EFST_TYPE2 = EFST_OFF;
	CELL_TRACKING.EFST_TYPE3 = EFST_NOTHING;
	CELL_TRACKING.EFST_TYPE4.Empty();
	if(CELL_TRACKING.USE == FALSE)
		CELL_TRACKING.EFST_Set(EFST_NOTHING);
	else
		CELL_TRACKING.EFST_Set(CELL_TRACKING.EFST_TYPE1);

	TRACKING_CONTROL.USE = TRUE;
	TRACKING_CONTROL.EFID = 2;
	TRACKING_CONTROL.EFNAME = _T("TRACKING CONTROL");
	TRACKING_CONTROL.EFST_TYPE1 = EFST_TKIN;	// default
	TRACKING_CONTROL.EFST_TYPE2 = EFST_TKOUT;
	TRACKING_CONTROL.EFST_TYPE3 = EFST_BOTH;
	TRACKING_CONTROL.EFST_TYPE4 = EFST_NOTHING;	
	if(TRACKING_CONTROL.USE == FALSE)
		TRACKING_CONTROL.EFST_Set(EFST_NOTHING);
	else
		TRACKING_CONTROL.EFST_Set(TRACKING_CONTROL.EFST_TYPE1);

	MATERIAL_TRACKING.USE = FALSE;
	MATERIAL_TRACKING.EFID = 3;
	MATERIAL_TRACKING.EFNAME = _T("MATERIAL TRACKING");
	MATERIAL_TRACKING.EFST_TYPE1 = EFST_ON; // default
	MATERIAL_TRACKING.EFST_TYPE2 = EFST_OFF;
	MATERIAL_TRACKING.EFST_TYPE3 = EFST_NOTHING;
	MATERIAL_TRACKING.EFST_TYPE4.Empty();
	if(MATERIAL_TRACKING.USE == FALSE)
		MATERIAL_TRACKING.EFST_Set(EFST_NOTHING);
	else
		MATERIAL_TRACKING.EFST_Set(MATERIAL_TRACKING.EFST_TYPE1);

	CELL_MCR_MODE.USE = TRUE;
	CELL_MCR_MODE.EFID = 4;
	CELL_MCR_MODE.EFNAME = _T("CELL MCR MODE");
	CELL_MCR_MODE.EFST_TYPE1 = EFST_USE; // default
	CELL_MCR_MODE.EFST_TYPE2 = EFST_UNUSE;
	CELL_MCR_MODE.EFST_TYPE3 = EFST_NOTHING;
	CELL_MCR_MODE.EFST_TYPE4.Empty();
	if(CELL_MCR_MODE.USE == FALSE)
		CELL_MCR_MODE.EFST_Set(EFST_NOTHING);
	else
		CELL_MCR_MODE.EFST_Set(CELL_MCR_MODE.EFST_TYPE1);

	MATERIAL_MCR_MODE.USE = FALSE;
	MATERIAL_MCR_MODE.EFID = 5;
	MATERIAL_MCR_MODE.EFNAME = _T("MATERIAL MCR MODE");
	MATERIAL_MCR_MODE.EFST_TYPE1 = EFST_USE; // default
	MATERIAL_MCR_MODE.EFST_TYPE2 = EFST_UNUSE;
	MATERIAL_MCR_MODE.EFST_TYPE3 = EFST_NOTHING;
	MATERIAL_MCR_MODE.EFST_TYPE4.Empty();
	if(MATERIAL_MCR_MODE.USE == FALSE)
		MATERIAL_MCR_MODE.EFST_Set(EFST_NOTHING);
	else
		MATERIAL_MCR_MODE.EFST_Set(MATERIAL_MCR_MODE.EFST_TYPE1);

	LOT_ASSIGN_INFORMATION.USE = FALSE;
	LOT_ASSIGN_INFORMATION.EFID = 6;
	LOT_ASSIGN_INFORMATION.EFNAME = _T("LOT ASSIGN INFORMATION");
	LOT_ASSIGN_INFORMATION.EFST_TYPE1 = EFST_AUTO; // default
	LOT_ASSIGN_INFORMATION.EFST_TYPE2 = EFST_MANU;
	LOT_ASSIGN_INFORMATION.EFST_TYPE3 = EFST_NOTHING;
	LOT_ASSIGN_INFORMATION.EFST_TYPE4.Empty();
	if(LOT_ASSIGN_INFORMATION.USE == FALSE)
		LOT_ASSIGN_INFORMATION.EFST_Set(EFST_NOTHING);
	else
		LOT_ASSIGN_INFORMATION.EFST_Set(LOT_ASSIGN_INFORMATION.EFST_TYPE1);

	AGV_ACCESS_MODE.USE = FALSE;
	AGV_ACCESS_MODE.EFID = 7;
	AGV_ACCESS_MODE.EFNAME = _T("AGV ACCESS MODE");
	AGV_ACCESS_MODE.EFST_TYPE1 = EFST_AUTO; // default
	AGV_ACCESS_MODE.EFST_TYPE2 = EFST_MANU;
	AGV_ACCESS_MODE.EFST_TYPE3 = EFST_NOTHING;
	AGV_ACCESS_MODE.EFST_TYPE4.Empty();
	if(AGV_ACCESS_MODE.USE == FALSE)
		AGV_ACCESS_MODE.EFST_Set(EFST_NOTHING);
	else
		AGV_ACCESS_MODE.EFST_Set(AGV_ACCESS_MODE.EFST_TYPE1);

	AREA_SENSOR_MODE.USE = FALSE;
	AREA_SENSOR_MODE.EFID = 8;
	AREA_SENSOR_MODE.EFNAME = _T("AREA SENSOR MODE");
	AREA_SENSOR_MODE.EFST_TYPE1 = EFST_USE; // default
	AREA_SENSOR_MODE.EFST_TYPE2 = EFST_UNUSE;
	AREA_SENSOR_MODE.EFST_TYPE3 = EFST_NOTHING;
	AREA_SENSOR_MODE.EFST_TYPE4.Empty();
	if(AREA_SENSOR_MODE.USE == FALSE)
		AREA_SENSOR_MODE.EFST_Set(EFST_NOTHING);
	else
		AREA_SENSOR_MODE.EFST_Set(AREA_SENSOR_MODE.EFST_TYPE1);

	SORT_MODE.USE = FALSE;
	SORT_MODE.EFID = 9;
	SORT_MODE.EFNAME = _T("SORT MODE");
	SORT_MODE.EFST_TYPE1 = EFST_USE;
	SORT_MODE.EFST_TYPE2 = EFST_UNUSE;	// default
	SORT_MODE.EFST_TYPE3 = EFST_NOTHING;
	SORT_MODE.EFST_TYPE4.Empty();
	if(SORT_MODE.USE == FALSE)
		SORT_MODE.EFST_Set(EFST_NOTHING);
	else
		SORT_MODE.EFST_Set(SORT_MODE.EFST_TYPE2);

	INTERLOCK_CONTROL.USE = TRUE;
	INTERLOCK_CONTROL.EFID = 10;
	INTERLOCK_CONTROL.EFNAME = _T("INTERLOCK CONTROL");
	INTERLOCK_CONTROL.EFST_TYPE1 = EFST_TRANSFER;
	INTERLOCK_CONTROL.EFST_TYPE2 = EFST_LOADING;	// default
	INTERLOCK_CONTROL.EFST_TYPE3 = EFST_STEP;
	INTERLOCK_CONTROL.EFST_TYPE4 = EFST_OWN;
	if(INTERLOCK_CONTROL.USE == FALSE)
		INTERLOCK_CONTROL.EFST_Set(EFST_NOTHING);
	else
		INTERLOCK_CONTROL.EFST_Set(INTERLOCK_CONTROL.EFST_TYPE2);
}

void CConfigCIM::ReadCIMOptionFile( CString strPath )
{
	CIni ini(strPath);
	EQUIP_FUNCTION *pFunction;
	pFunction = &CELL_TRACKING;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &TRACKING_CONTROL;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &MATERIAL_TRACKING;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &CELL_MCR_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &MATERIAL_MCR_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &LOT_ASSIGN_INFORMATION;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &AGV_ACCESS_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &AREA_SENSOR_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &SORT_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
	pFunction = &INTERLOCK_CONTROL;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		pFunction->EFST_Set(ini.GetString(_T("EQUIP_FUNCTION"), pFunction->EFNAME));
}

void CConfigCIM::SaveCIMOptionFile( CString strPath, CString strBackupPath )
{
	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

	EQUIP_FUNCTION *pFunction;
	pFunction = &CELL_TRACKING;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &TRACKING_CONTROL;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &MATERIAL_TRACKING;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &CELL_MCR_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &MATERIAL_MCR_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &LOT_ASSIGN_INFORMATION;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &AGV_ACCESS_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &AREA_SENSOR_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &SORT_MODE;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
	pFunction = &INTERLOCK_CONTROL;
	if(pFunction->EFNAME.IsEmpty() == FALSE)
		ini.WriteString(_T("EQUIP_FUNCTION"), pFunction->EFNAME, pFunction->EFST_Get());	
}

void CConfigCIM::ConfirmCIMFunction(CString strBYWHO)
{
	// 상위로 보고할 void 포인터 배열을 만든다 - LSH171130
	EQUIP_FUNCTION* FunctionChangeBuffer[EFID_MAX];
	int nBufferCount = 0;

	// 현재 설정한 값을 실재 객체에 넣어준 후 값이 변하였는지 확인하여 보고한다 - LSH171129
	if(CELL_TRACKING.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &CELL_TRACKING;
		CELL_TRACKING.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(TRACKING_CONTROL.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &TRACKING_CONTROL;
		TRACKING_CONTROL.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(MATERIAL_TRACKING.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &MATERIAL_TRACKING;
		MATERIAL_TRACKING.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(CELL_MCR_MODE.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &CELL_MCR_MODE;
		CELL_MCR_MODE.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(MATERIAL_MCR_MODE.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &MATERIAL_MCR_MODE;
		MATERIAL_MCR_MODE.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(LOT_ASSIGN_INFORMATION.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &LOT_ASSIGN_INFORMATION;
		LOT_ASSIGN_INFORMATION.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(AGV_ACCESS_MODE.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &AGV_ACCESS_MODE;
		AGV_ACCESS_MODE.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(AREA_SENSOR_MODE.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &AREA_SENSOR_MODE;
		AREA_SENSOR_MODE.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(SORT_MODE.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &SORT_MODE;
		SORT_MODE.SetChangeState(FALSE);
		nBufferCount++;
	}
	if(INTERLOCK_CONTROL.GetChangeState() == TRUE)
	{
		FunctionChangeBuffer[nBufferCount] = &INTERLOCK_CONTROL;
		INTERLOCK_CONTROL.SetChangeState(FALSE);
		nBufferCount++;
	}

	// 변경 해야하는 Function list를 상위에 보고 - LSH171130
	for(int itor=0 ; itor<nBufferCount ; itor++)
	{
		// 마지막 Index를 찾는다 - LSH171130
		if(itor >= (nBufferCount - 1))
			theSocketInterFace.m_CIM.SendCmdEqpFunctionChangeToDataPC(FunctionChangeBuffer[itor], itor, TRUE, strBYWHO);
		else
			theSocketInterFace.m_CIM.SendCmdEqpFunctionChangeToDataPC(FunctionChangeBuffer[itor], itor, FALSE, strBYWHO);
	}


}

BOOL CConfigCIM::SetFunction(EFID_INDEX EFID, CString EFST)
{
	// 값을 입력하고 결과를 가져온다 - LSH171201
	BOOL bResult = GetFunction(EFID)->EFST_Set(EFST);

	// 데이터 변경 후 결과 반환 - LSH171130
	return bResult;
}

EQUIP_FUNCTION* CConfigCIM::GetFunction(EFID_INDEX EFID)
{
	if(EFID == EFID_CELL_TRACKING)			return &CELL_TRACKING;
	if(EFID == EFID_TRACKING_CONTROL)		return &TRACKING_CONTROL;
	if(EFID == EFID_MATERIAL_TRACKING)		return &MATERIAL_TRACKING;
	if(EFID == EFID_CELL_MCR_MODE)			return &CELL_MCR_MODE;
	if(EFID == EFID_MATERIAL_MCR_MODE)		return &MATERIAL_MCR_MODE;
	if(EFID == EFID_LOT_ASSIGN_INFORMATION)	return &LOT_ASSIGN_INFORMATION;
	if(EFID == EFID_AGV_ACCESS_MODE)		return &AGV_ACCESS_MODE;
	if(EFID == EFID_AREA_SENSOR_MODE)		return &AREA_SENSOR_MODE;
	if(EFID == EFID_SORT_MODE)				return &SORT_MODE;
	if(EFID == EFID_INTERLOCK_CONTROL)		return &INTERLOCK_CONTROL;

	return NULL;
}

// 이 설비에서 사용 불가능한 거면 FALSE
BOOL CConfigCIM::Function_CheckUse(EFID_INDEX index)
{
	EQUIP_FUNCTION *pFunction = GetFunction(index);
	if(pFunction)
	{
		return pFunction->USE;
	}

	return FALSE;
}

// 전이도 확인해서 저장 불가하면 FALE
BOOL CConfigCIM::TransitionOfEFListState()
{
	// CELL_MCR_MODE가 변경되었을 경우 - LSH171201
	if(CELL_MCR_MODE.bChangeFlag)
	{
		if(CELL_MCR_MODE.EFST_Get() == EFST_USE)
		{
			if(CELL_TRACKING.EFST_Get() == EFST_NOTHING)
				CELL_TRACKING.EFST_Set(EFST_ON);

			if(TRACKING_CONTROL.EFST_Get() == EFST_NOTHING)
				TRACKING_CONTROL.EFST_Set(EFST_TKIN);
		}
		else
		{
			if(CELL_TRACKING.EFST_Get() != EFST_NOTHING)
				CELL_TRACKING.EFST_Set(EFST_NOTHING);

			if(TRACKING_CONTROL.EFST_Get() != EFST_NOTHING)
				TRACKING_CONTROL.EFST_Set(EFST_NOTHING);
		}

	}
	// CELL_TRACKING가 변경되었을 경우 - LSH171201
	if(CELL_TRACKING.bChangeFlag)
	{
		if(CELL_MCR_MODE.EFST_Get() == EFST_USE)
		{
			if(CELL_TRACKING.EFST_Get() == EFST_ON)
			{
				// TRACKING_CONTROL TKIN - LSH171130
				if(TRACKING_CONTROL.EFST_Get() == EFST_NOTHING)
					TRACKING_CONTROL.EFST_Set(EFST_TKIN);
			} 
			else if(CELL_TRACKING.EFST_Get() == EFST_OFF)
			{
				if(TRACKING_CONTROL.EFST_Get() != EFST_NOTHING)
					TRACKING_CONTROL.EFST_Set(EFST_NOTHING);
			}
			else if(CELL_TRACKING.EFST_Get() == EFST_NOTHING)
			{
				return FALSE;
			}
		}
		else
		{
			if(CELL_TRACKING.EFST_Get() != EFST_NOTHING)
				return FALSE;
		}
	}
	if(TRACKING_CONTROL.bChangeFlag)
	{
		if(TRACKING_CONTROL.EFST_Get() == EFST_TKIN)
		{
			if(CELL_TRACKING.EFST_Get() != EFST_ON)
				return FALSE;
		}
		else if(TRACKING_CONTROL.EFST_Get() == EFST_TKOUT)
		{
			return FALSE;
		}
		else if(TRACKING_CONTROL.EFST_Get() == EFST_BOTH)
		{
			return FALSE;
		}
		else if(TRACKING_CONTROL.EFST_Get() == EFST_NOTHING)
		{
			if(CELL_TRACKING.EFST_Get() == EFST_ON)
				return FALSE;
		}
	}

	return TRUE;
}


BOOL CConfigCIM::CELL_MCR_MODE_Check()
{
	if(CELL_MCR_MODE.EFST_Get() == EFST_USE)
		return TRUE;
	else
		return FALSE;
}

BOOL CConfigCIM::TRACKING_CONTROL_InCheck()
{
	if( (TRACKING_CONTROL.EFST_Get() == EFST_TKIN)
		|| (TRACKING_CONTROL.EFST_Get() == EFST_BOTH) )
		return TRUE;
	else
		return FALSE;
}

BOOL CConfigCIM::TRACKING_CONTROL_OutCheck()
{
	if( (TRACKING_CONTROL.EFST_Get() == EFST_TKOUT)
		|| (TRACKING_CONTROL.EFST_Get() == EFST_BOTH) )
		return TRUE;
	else
		return FALSE;
}

BOOL CConfigCIM::INTERLOCK_CONTROL_TransferCheck()
{
	if(INTERLOCK_CONTROL.EFST_Get() == EFST_TRANSFER)
		return TRUE;
	else
		return FALSE;
}

BOOL CConfigCIM::INTERLOCK_CONTROL_LoadingCheck()
{
	if(INTERLOCK_CONTROL.EFST_Get() == EFST_LOADING)
		return TRUE;
	else
		return FALSE;
}

BOOL CConfigCIM::INTERLOCK_CONTROL_StepCheck()
{
	if(INTERLOCK_CONTROL.EFST_Get() == EFST_STEP)
		return TRUE;
	else
		return FALSE;
}
