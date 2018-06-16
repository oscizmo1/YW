#pragma once

#include "StateMachine/StateRestart.h"
#include "StateMachine/StateAZone.h"
#include "StateMachine/StateBZone.h"
#include "StateMachine/StateCZone.h"
#include "StateMachine/StateMoveAZone.h"
#include "StateMachine/StateMoveBZone.h"
#include "StateMachine/StateMoveCZone.h"
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
	CStateBZone						m_stateShuttle1_BZone;
	CStateCZone						m_stateShuttle1_CZone;
	CStateMoveAZone					m_stateShuttle1_MoveAZone;
	CStateMoveBZone					m_stateShuttle1_MoveBZone;
	CStateMoveCZone					m_stateShuttle1_MoveCZone;

	CStateAZone						m_stateShuttle2_AZone;
	CStateBZone						m_stateShuttle2_BZone;
	CStateCZone						m_stateShuttle2_CZone;
	CStateMoveAZone					m_stateShuttle2_MoveAZone;
	CStateMoveBZone					m_stateShuttle2_MoveBZone;
	CStateMoveCZone					m_stateShuttle2_MoveCZone;

	CStatePDT_IF					m_stateShuttle1_PDT_IF;
	CStatePDT_IF					m_stateShuttle2_PDT_IF;


	// 굳이 객체 생성 안해도 되는 것들은 안해도 된다 [10/17/2017 OSC]
	CThread_Main_Sequence m_Thread_Main_Sequence;
// 	CThread_IO_Alarm_Check	m_Thread_IO_Alarm_Check;
	CThread_IO_Sequence		m_Thread_IO_Sequence;
	CThread_PDT_IF_Sequence m_Thread_PDT_IF_Sequence;

	void CreateThread();
};

