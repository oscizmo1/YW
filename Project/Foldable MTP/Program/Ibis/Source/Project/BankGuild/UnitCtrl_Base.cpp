#include "StdAfx.h"
#include "UnitCtrl_Base.h"
#include "IbisApp.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUnitCtrl_Base::CUnitCtrl_Base( CString StateName, MP2100_THREAD nThreadID, BOOL bManual /*= FALSE*/, LOG_INDEX logindex /*= LOG_SEQUENCE*/ )
{
	SetParam(StateName, nThreadID, bManual, logindex);
}

CUnitCtrl_Base::CUnitCtrl_Base()
{
	m_strStateName.Empty();
	m_nStep = stepIdle;
	m_nError = ALM_NONE;
	m_nThreadID = MP2100_THREAD_MAX;
	m_bManualSequence = FALSE;
	m_LogIndex = LOG_MAX;
}

void CUnitCtrl_Base::ResetReturnValue()
{
	memset(m_bRtnComp, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
	memset(m_bRtn, TRUE, sizeof(BYTE)*MAX_RTN_SIZE);
}

BOOL CUnitCtrl_Base::IsReturnOk()
{
	return (memcmp(m_bRtn, m_bRtnComp, MAX_RTN_SIZE) == 0) ? TRUE:FALSE;
}

BOOL CUnitCtrl_Base::IsReturnOk( int nStartIndex, int nEndIndex )
{
	// 지정한 부분만 비교한다.
	// 3~5까지만 비교하고 싶다면 IsReturnOk(3,5) 하면 그것만 비교한다
	if( (nStartIndex < 0) || (nEndIndex < 0) )
		return FALSE;
	if( (nStartIndex >= MAX_RTN_SIZE) || (nEndIndex >= MAX_RTN_SIZE) )
		return FALSE;
	int nSize = nEndIndex+1 - nStartIndex;
	return (memcmp(&m_bRtn[nStartIndex], &m_bRtnComp[nStartIndex], nSize) == 0) ? TRUE:FALSE;
}

void CUnitCtrl_Base::SetError( ALARM_ID id, CString strDiscription/*=_T("")*/ )
{
	theLog[m_LogIndex].AddBuf(_T("[ERROR STEP] error=%d (%s, %d)"), id, m_strStateName, m_nStep);
	// Error Step 확인 편하게 하기 위해 Sequence.txt에다가도 통합으로 적는다
	if(m_LogIndex != LOG_SEQUENCE)
		theLog[LOG_SEQUENCE].AddBuf(_T("[ERROR STEP] error=%d (%s, %d)"), id, m_strStateName, m_nStep);

	if(m_bManualSequence)
	{
		m_nStep = stepIdle;
		theProcBank.AlarmMsgHappen(id, strDiscription);
	}
	else
	{
		// Warning은 한 State에서 중복으로 띄우지 않는다 [9/27/2017 OSC]
		if( (IsError() == FALSE) && (theProcBank.m_AlarmMsg[id].nAlarmLevel == ALARM_LEVEL_WARNING) )
		{
			m_nError = id;
			theProcBank.WarningHappen(id, strDiscription);
		}
		else
		{
			m_nError = id;
			theProcBank.AlarmHappen(id, strDiscription);
		}
	}
}

void CUnitCtrl_Base::SetParam( CString StateName, MP2100_THREAD nThreadID, BOOL bManual /*= FALSE*/, LOG_INDEX logindex /*= LOG_SEQUENCE*/ )
{
	m_strStateName = StateName;
	Reset();
	m_nThreadID = nThreadID;
	m_bManualSequence = bManual;
	m_LogIndex = logindex;
}
