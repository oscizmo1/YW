#include "StdAfx.h"
#include "EquipFunction.h"




CEquipFunction::CEquipFunction(void)
{
	m_bSyncStarted = FALSE;
	InitFunction();
}


CEquipFunction::~CEquipFunction(void)
{
}

void CEquipFunction::InitFunction()
{
	m_FunctionList[0].EFID = 1;
	m_FunctionList[0].EFNAME = _T("CELL TRACKING");

	m_FunctionList[1].EFID = 2;
	m_FunctionList[1].EFNAME = _T("TRACKING CONTROL");

	m_FunctionList[2].EFID = 3;
	m_FunctionList[2].EFNAME = _T("MATERIAL TRACKING");

	m_FunctionList[3].EFID = 4;
	m_FunctionList[3].EFNAME = _T("CELL MCR MODE");

	m_FunctionList[4].EFID = 5;
	m_FunctionList[4].EFNAME = _T("MATERIAL MCR MODE");

	m_FunctionList[5].EFID = 6;
	m_FunctionList[5].EFNAME = _T("LOT ASSIGN INFORMATION");

	m_FunctionList[6].EFID = 7;
	m_FunctionList[6].EFNAME = _T("AGV ACCESS MODE");

	m_FunctionList[7].EFID = 8;
	m_FunctionList[7].EFNAME = _T("AREA SENSOR MODE");

	m_FunctionList[8].EFID = 9;
	m_FunctionList[8].EFNAME = _T("SORT MODE");

	m_FunctionList[9].EFID = 10;
	m_FunctionList[9].EFNAME = _T("INTERLOCK CONTROL");
}

BOOL CEquipFunction::UpdateEquipFunction( tag_Message *pMsg )
{
	BOOL bRet = FALSE;
	CString str;
	for(int i = 0; i < MAX_EQUIP_FUNCTION_LIST; i++)
	{
		str.Format(_T("%s"), pMsg->cMsgData[i]);
		m_FunctionList[i].EFST = str;
// 		if(m_FunctionList[i].EFST)
		{
			bRet = TRUE;
		}
	}
	// 처음 동기화용으로 한 거면 보고 안한다
	if(m_bSyncStarted == FALSE)
	{
		m_bSyncStarted = TRUE;
		return FALSE;
	}
	else
	{
		return bRet;
	}
}
