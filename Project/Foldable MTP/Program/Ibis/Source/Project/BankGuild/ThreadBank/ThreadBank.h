#pragma once

#include "StateMachine/StateRestart.h"
#include "StateMachine/StateAZone.h"
#include "StateMachine/StateMoveBZone.h"
#include "StateMachine/StateMoveAZone.h"
#include "StateMachine/StateBZone.h"
#include "StateMachine/StatePDT_IF.h"

#include "Thread_FDC_Alarm_Check.h"
#include "Thread_IO_Alarm_Check.h"
#include "Thread_IO_Sequence.h"
#include "Thread_Main_Sequence.h"
#include "Thread_PDT_IF_Sequence.h"

class CThreadBank
{
public:
	CThreadBank(void);
	~CThreadBank(void);

	CStateRestart					m_stateRestart;
	CStateAZone						m_stateShuttle1_AZone;
	CStateMoveAZone					m_stateShuttle1_MoveAZone;
	CStateMoveBZone					m_stateShuttle1_MoveBZone;
	CStateBZone						m_stateShuttle1_BZone;

	CStateAZone						m_stateShuttle2_AZone;
	CStateMoveAZone					m_stateShuttle2_MoveAZone;
	CStateMoveBZone					m_stateShuttle2_MoveBZone;
	CStateBZone						m_stateShuttle2_BZone;

	CStatePDT_IF					m_stateShuttle1_PDT_IF;
	CStatePDT_IF					m_stateShuttle2_PDT_IF;


	// 굳이 객체 생성 안해도 되는 것들은 안해도 된다 [10/17/2017 OSC]
	CThread_Main_Sequence m_Thread_Main_Sequence;
// 	CThread_IO_Alarm_Check	m_Thread_IO_Alarm_Check;
	CThread_IO_Sequence		m_Thread_IO_Sequence;
	CThread_PDT_IF_Sequence m_Thread_PDT_IF_Sequence;

	void CreateThread();
};

