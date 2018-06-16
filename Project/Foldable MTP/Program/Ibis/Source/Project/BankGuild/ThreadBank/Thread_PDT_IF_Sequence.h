#pragma once

#include "ThreadBank.h"

class CThread_PDT_IF_Sequence
{
public:
	CThread_PDT_IF_Sequence(void);
	~CThread_PDT_IF_Sequence(void);

	// PDT Interface Thread [9/21/2017 OSC]
	static UINT PDT_InterfaceThr(LPVOID lpParam);

private:
	BYTE m_bRtnComp[MAX_RTN_SIZE];	
	BYTE m_bRtn[MAX_RTN_SIZE];

	void ResetReturnValue();
	BOOL IsReturnOk();


	BOOL m_bEMS;
	BOOL m_bDoorClose;
	STO_STATE m_STO;
	BOOL m_bVacOnInput[JIG_ID_MAX][JIG_CH_MAX];
	BOOL m_bVacOnOutput[JIG_ID_MAX][JIG_CH_MAX];
	CUnitCtrlFunc *m_pfunc;

public:
	CStateRestart		*m_pRestart;
	CStateAZone			*m_pAZone[JIG_ID_MAX];
	CStateMoveAZone		*m_pMoveAZone[JIG_ID_MAX];
	CStateMoveBZone		*m_pMoveBZone[JIG_ID_MAX];
	CStateMoveCZone		*m_pMoveCZone[JIG_ID_MAX];
	CStatePDT_IF		*m_pIF[JIG_ID_MAX];

	BOOL IsAllStop();
	BOOL IsError();
	void StateAllReset();
	void StateAllRun();

	void GetStatePointer( );

	void BitInit();
	// Vacuum On Off 상태에 맞춰 LB를 On Off한다 [10/5/2017 OSC]
	void VacuumOnOff_Check();

	// 패널 주고 받을 수 있는 채널은 모두 Able 살린다 [10/5/2017 OSC]
	void MTP_Able_JigOnOff(JIG_ID jig);

	void MTP_Able_AllOnOff( CUnitCtrlFunc &_func, JIG_ID jig );

	void ConfirmStart_IF( CUnitCtrlFunc &_func, JIG_ID jig );

	// 도어 및 STO 상태에 따라 HotLine을 살려준다. L/C이 없으니 STO로 대체 [10/7/2017 OSC]
	void MTP_HotLine_Check();

	BOOL PDT_HotLine_Check();

	// Robot Alarm 확인해서 대신 띄워준다 [10/7/2017 OSC]
	int m_nRobotAlarmCode;
	CDevicePDT_IF::ROBOT_STATUS m_RobotStatus;
	BOOL Robot_StatusIsAlarm();
	void Robot_Alarm_Check();

	// Robot Alarm 대신 띄워준 후 Robot이 정상화되면 
	// 검사기꺼 상태 보고 알아서 다시 자동 START한다
	BOOL MachineIsRobotAlarmAble();	// 로봇 알람을 띄워줄 수 있는 상태인지..
	BOOL ConfirmMachineRestart();	// 로봇이 Run 할 때 자동 Run이 가능한지..

};

