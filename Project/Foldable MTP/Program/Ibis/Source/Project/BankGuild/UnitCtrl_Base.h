#pragma once
#include "Device/DeviceMotion.h"
#include "Etc/StopWatch.h"
#include "InspModuleBank/InspModule_Define.h"

#define MAX_RTN_SIZE	20

class CUnitCtrl_Base
{
protected:
	CUnitCtrl_Base();
public:
	CUnitCtrl_Base(CString StateName, MP2100_THREAD nThreadID, BOOL bManual = FALSE, LOG_INDEX logindex = LOG_SEQUENCE);
	~CUnitCtrl_Base(void) {};

	enum
	{
		stepIdle	= -1,
		stepStart	= 0,
		stepHold	= 999,
	};

protected:
	CString m_strStateName;
	int		m_nStep;
	ALARM_ID	m_nError;
	MP2100_THREAD		m_nThreadID;
	CStopWatch m_Timer;
	BOOL m_bManualSequence;		// 티칭 UI 등에서 얘만 단동으로 할 경우 TRUE
	LOG_INDEX m_LogIndex;


	//////////////////////////////////////////////////////////////////////////
	// Rtn관련 변수
	BYTE m_bRtnComp[MAX_RTN_SIZE];	
	BYTE m_bRtn[MAX_RTN_SIZE];
	void ResetReturnValue();	// Rtn변수들을 전부 TRUE로 Set한다.
	BOOL IsReturnOk();			// Rtn과 RtnComp를 Compair하여 OK/NG를 판단한다.
	BOOL IsReturnOk(int nStartIndex, int nEndIndex);			// 지정한 부분만 비교한다.
	//////////////////////////////////////////////////////////////////////////

	CString m_strCurrentModuleName;


public:
	virtual	int Run()=0;
	void	Start() 
	{
		m_Timer.Start();
		m_nStep = stepStart;
		m_nError = ALM_NONE;
		m_strCurrentModuleName = INSP_MODULE_NONE;
	};
	void	Reset()
	{ 
		m_nStep = stepIdle; 
		m_nError = ALM_NONE;
	};
	BOOL IsStoped()
	{
		return m_nStep == stepIdle ? TRUE:FALSE;
	};
	BOOL IsHolded()
	{
		return m_nStep == stepHold ? TRUE:FALSE;
	};
	void SetError(ALARM_ID id, CString strDiscription=_T(""));
	BOOL IsError() { return m_nError == ALM_NONE ? FALSE:TRUE; }

	void SetParam(CString StateName, MP2100_THREAD nThreadID, BOOL bManual = FALSE, LOG_INDEX logindex = LOG_SEQUENCE);
};
