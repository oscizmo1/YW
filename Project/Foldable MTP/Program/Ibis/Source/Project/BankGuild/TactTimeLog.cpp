#include "StdAfx.h"
#include "TactTimeLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CTactTimeUnit::SetUnitName(CString strUnitName)
{
	m_strUnitname = strUnitName;
}

void CTactTimeUnit::Reset()
{
	m_bStart = FALSE;
}

void CTactTimeUnit::Start()
{
	m_bStart = TRUE;
	double dSec = m_Timer.Stop(TRUE);
	theLog[LOG_TACT_TIME].AddBuf(_T(",%s,Start,,%.1f"), m_strUnitname, dSec);
}

void CTactTimeUnit::End()
{
	double dSec = m_Timer.Stop(FALSE);
	if(m_bStart)
	{
		// 이전에 Start를 했다면 시간 계산해서 로그 기록
		theLog[LOG_TACT_TIME].AddBuf(_T(",%s,End,%.1f"), m_strUnitname, dSec);
		Reset();
	}
}

double CTactTimeUnit::GetSpanTime()
{
	return m_Timer.CheckTime();
}

 
CTactTimeLog::CTactTimeLog(void)
{
	InitUnitName();
}


CTactTimeLog::~CTactTimeLog(void)
{
}

void CTactTimeLog::InitUnitName()
{
	CString strText;
	for(int i = 0; i < JIG_ID_MAX; i++)
	{
		strText.Format(_T("SHUTTLE%d_MoveAZone"), i+1);
		m_Shuttle_MoveAZone[i].SetUnitName(strText);
		strText.Format(_T("SHUTTLE%d_MoveMCR"), i+1);
		m_Shuttle_MoveMCR[i].SetUnitName(strText);
		strText.Format(_T("SHUTTLE%d_MoveBZone"), i+1);
		m_Shuttle_MoveBZone[i].SetUnitName(strText);
		strText.Format(_T("InspectionZ%d_Up"), i+1);
		m_InspectionZ_Up[i].SetUnitName(strText);
		strText.Format(_T("InspectionZ%d_Down"), i+1);
		m_InspectionZ_Down[i].SetUnitName(strText);
		strText.Format(_T("SHUTTLE%d_RunTime"), i+1);
		m_RunTime[i].SetUnitName(strText);
		strText.Format(_T("SHUTTLE%d_Interface"), i+1);
		m_Interface[i].SetUnitName(strText);
	}
}

void CTactTimeLog::Reset()
{
	for(int i = 0; i < JIG_ID_MAX; i++)
	{
		m_Shuttle_MoveAZone[i].Reset();
		m_Shuttle_MoveMCR[i].Reset();
		m_Shuttle_MoveBZone[i].Reset();
		m_InspectionZ_Up[i].Reset();
		m_InspectionZ_Down[i].Reset();
	}
}
