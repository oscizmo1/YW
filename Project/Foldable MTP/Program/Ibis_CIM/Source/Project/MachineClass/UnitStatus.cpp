#include "StdAfx.h"
#include "UnitStatus.h"


CUnitStatus::CUnitStatus(void)
{
	InitStatus();
}


CUnitStatus::~CUnitStatus(void)
{
}

void CUnitStatus::AddInterlock(CString strID, CString strMsg, UINT nType)
{
	for (int i=0; i<MAX_INTERLOCK_MES; i++)
	{
		if(m_tUnitInterlockMES[i].m_cInterlockState == INTERLOCK_NONE)
		{
			m_tUnitInterlockMES[i].SetInterLock(strID, strMsg, nType);
			return;
		}
	}
}

int CUnitStatus::GetInterlockCount()
{
	int nCount = 0;
	for (int i=0; i<MAX_INTERLOCK_MES; i++)
	{
		if(m_tUnitInterlockMES[i].m_cInterlockState == INTERLOCK_HAPPEN)
		{
			nCount++;
		}
	}
	return nCount;
}

void CUnitStatus::InitStatus()
{
	m_AvilabilityState = STATUS_AVAILABILITY_UP;
	m_InterlockState = STATUS_INTERLOCK_OFF;
	m_MoveState = STATUS_MOVE_RUNNING;
	m_RunState = STATUS_RUN_NOT_CELL;
	m_FrontState = STATUS_FRONT_UP;
	m_RearState = STATUS_REAR_UP;
	m_PP_SplState = STATUS_SAMPLE_OFF;
	m_strResonCode.Empty();
	m_strDescrition.Empty();

	m_oldAvilabilityState = m_AvilabilityState;
	m_oldInterlockState = m_InterlockState;
	m_oldMoveState = m_MoveState;
	m_oldRunState = m_RunState;
	m_oldFrontState = m_FrontState;
	m_oldRearState = m_RearState;
	m_oldPP_SplState = m_PP_SplState;

	for(int i = 0; i < MAX_INTERLOCK_MES; i++)
	{
		m_tUnitInterlockMES[i].Clear();
	}

}

CString CUnitStatus::ToString()
{
	CString str;
	str.Format(_T("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"),
		m_AvilabilityState,
		m_InterlockState,
		m_MoveState,
		m_RunState,
		m_FrontState,
		m_RearState,
		m_PP_SplState,
		m_oldAvilabilityState,
		m_oldInterlockState,
		m_oldMoveState,
		m_oldRunState,
		m_oldFrontState,
		m_oldRearState,
		m_oldPP_SplState
		);

	return str;
}

void CUnitStatus::FromString(CString str)
{
	CString strAvilabilityState;
	CString strInterlockState;
	CString strMoveState;
	CString strRunState;
	CString strFrontState;
	CString strRearState;
	CString strPP_SplState;

	::AfxExtractSubString( strAvilabilityState,	str	, 0, _T(','));	
	::AfxExtractSubString( strInterlockState,		str	, 1, _T(','));	
	::AfxExtractSubString( strMoveState,			str	, 2, _T(','));	
	::AfxExtractSubString( strRunState,			str	, 3, _T(','));	
	::AfxExtractSubString( strFrontState,			str	, 4, _T(','));	
	::AfxExtractSubString( strRearState,			str	, 5, _T(','));	
	::AfxExtractSubString( strPP_SplState,			str	, 6, _T(','));	

	m_AvilabilityState =	(STATUS_AVAILABILITY)_tstoi(strAvilabilityState);
	m_InterlockState =		(STATUS_INTERLOCK)_tstoi(strInterlockState);
	m_MoveState =			(STATUS_MOVE)_tstoi(strMoveState);
	m_RunState =			(STATUS_RUN)_tstoi(strRunState);
	m_FrontState =			(STATUS_FRONT)_tstoi(strFrontState);
	m_RearState =			(STATUS_REAR)_tstoi(strRearState);
	m_PP_SplState =			(STATUS_SAMPLE)_tstoi(strPP_SplState);

	::AfxExtractSubString( strAvilabilityState,	str	, 7, _T(','));	
	::AfxExtractSubString( strInterlockState,		str	, 8, _T(','));	
	::AfxExtractSubString( strMoveState,			str	, 9, _T(','));	
	::AfxExtractSubString( strRunState,			str	, 10, _T(','));	
	::AfxExtractSubString( strFrontState,			str	, 11, _T(','));	
	::AfxExtractSubString( strRearState,			str	, 12, _T(','));	
	::AfxExtractSubString( strPP_SplState,			str	, 13, _T(','));	

	m_oldAvilabilityState =	(STATUS_AVAILABILITY)_tstoi(strAvilabilityState);
	m_oldInterlockState =		(STATUS_INTERLOCK)_tstoi(strInterlockState);
	m_oldMoveState =			(STATUS_MOVE)_tstoi(strMoveState);
	m_oldRunState =			(STATUS_RUN)_tstoi(strRunState);
	m_oldFrontState =			(STATUS_FRONT)_tstoi(strFrontState);
	m_oldRearState =			(STATUS_REAR)_tstoi(strRearState);
	m_oldPP_SplState =			(STATUS_SAMPLE)_tstoi(strPP_SplState);
}
