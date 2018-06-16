#include "StdAfx.h"
#include "ThreadBank.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// Tread간의 연동 때문에 State객체들을 전부 전역으로 옮김
// State 추가시 MP2100_THREAD 인자 선정 주의할것 [12/10/2016 OSC]
CThreadBank::CThreadBank(void)
	: m_stateRestart(STATE_RESTART, MP2100_THREAD_MAIN_SEQUENCE),
	m_stateShuttle1_AZone(STATE_SHUTTLE1_AZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_A),
	m_stateShuttle1_BZone(STATE_SHUTTLE1_BZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_A),
	m_stateShuttle1_CZone(STATE_SHUTTLE1_CZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_A),
	m_stateShuttle1_MoveAZone(STATE_SHUTTLE1_MOVE_AZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_A),
	m_stateShuttle1_MoveBZone(STATE_SHUTTLE1_MOVE_BZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_A),
	m_stateShuttle1_MoveCZone(STATE_SHUTTLE1_MOVE_CZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_A),

	m_stateShuttle2_AZone(STATE_SHUTTLE2_AZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_B),
	m_stateShuttle2_BZone(STATE_SHUTTLE2_BZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_B),
	m_stateShuttle2_CZone(STATE_SHUTTLE2_CZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_B),
	m_stateShuttle2_MoveAZone(STATE_SHUTTLE2_MOVE_AZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_B),
	m_stateShuttle2_MoveBZone(STATE_SHUTTLE2_MOVE_BZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_B),
	m_stateShuttle2_MoveCZone(STATE_SHUTTLE2_MOVE_CZONE, MP2100_THREAD_MAIN_SEQUENCE, JIG_ID_B),

	m_stateShuttle1_PDT_IF(STATE_SHUTTLE1_PDT_IF, MP2100_THREAD_PDT_IF, JIG_ID_A),
	m_stateShuttle2_PDT_IF(STATE_SHUTTLE2_PDT_IF, MP2100_THREAD_PDT_IF, JIG_ID_B)
{

}


CThreadBank::~CThreadBank(void)
{
}

void CThreadBank::CreateThread()
{
	DWORD dwThreadDelay = 300;	// MP2100을 동시에 연속으로 Open하면 안되서 Delay 줌 [11/6/2016 OSC]
	Sleep(dwThreadDelay);
	if(theThreadInitializer.CreateThread(THREAD_INDEX_MAIN_SEQUENCE, CThread_Main_Sequence::MainSequenceThr, (LPVOID)THREAD_INDEX_MAIN_SEQUENCE) == FALSE)
	{
		AfxMessageBox(_T("MainSequenceThr Thread 을 실행 하지 못했습니다."));
		return;
	}

	Sleep(dwThreadDelay);
	//kjpark 20161019 IO Check Thread 추가
	if(theThreadInitializer.CreateThread(THREAD_INDEX_IO_SEQUENCE, CThread_IO_Sequence::IOSequenceThr, (LPVOID)THREAD_INDEX_IO_SEQUENCE) == FALSE)
	{
		AfxMessageBox(_T("IOSequenceThr Thread 을 실행 하지 못했습니다."));
		return;
	}

	Sleep(dwThreadDelay);
	// FDC Monitoring Thread [12/7/2016 OSC]
	if(theThreadInitializer.CreateThread(THREAD_INDEX_FDC_MOITOR, CThread_FDC_Alarm_Check::FDCMonitorThr, (LPVOID)THREAD_INDEX_FDC_MOITOR) == FALSE)
	{
		AfxMessageBox(_T("FDCMonitorThr Thread 을 실행 하지 못했습니다."));
		return;
	}

	Sleep(dwThreadDelay);
	// IO Alarm Thread [12/7/2016 OSC]
	if(theThreadInitializer.CreateThread(THREAD_INDEX_IO_ALARM_CHECK, CThread_IO_Alarm_Check::IOAlarmCheckThr, (LPVOID)THREAD_INDEX_IO_ALARM_CHECK) == FALSE)
	{
		AfxMessageBox(_T("IOAlarmCheckThr Thread 을 실행 하지 못했습니다."));
		return;
	}

	Sleep(dwThreadDelay);
	// PDT Interface Thread [9/21/2017 OSC]
	if(theThreadInitializer.CreateThread(THREAD_INDEX_PDT_IF, CThread_PDT_IF_Sequence::PDT_InterfaceThr, (LPVOID)THREAD_INDEX_PDT_IF) == FALSE)
	{
		AfxMessageBox(_T("PDT_InterfaceThr Thread 을 실행 하지 못했습니다."));
		return;
	}
}
