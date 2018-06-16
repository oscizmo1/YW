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
	// Vacuum On Off ���¿� ���� LB�� On Off�Ѵ� [10/5/2017 OSC]
	void VacuumOnOff_Check();

	// �г� �ְ� ���� �� �ִ� ä���� ��� Able �츰�� [10/5/2017 OSC]
	void MTP_Able_JigOnOff(JIG_ID jig);

	void MTP_Able_AllOnOff( CUnitCtrlFunc &_func, JIG_ID jig );

	void ConfirmStart_IF( CUnitCtrlFunc &_func, JIG_ID jig );

	// ���� �� STO ���¿� ���� HotLine�� ����ش�. L/C�� ������ STO�� ��ü [10/7/2017 OSC]
	void MTP_HotLine_Check();

	BOOL PDT_HotLine_Check();

	// Robot Alarm Ȯ���ؼ� ��� ����ش� [10/7/2017 OSC]
	int m_nRobotAlarmCode;
	CDevicePDT_IF::ROBOT_STATUS m_RobotStatus;
	BOOL Robot_StatusIsAlarm();
	void Robot_Alarm_Check();

	// Robot Alarm ��� ����� �� Robot�� ����ȭ�Ǹ� 
	// �˻�Ⲩ ���� ���� �˾Ƽ� �ٽ� �ڵ� START�Ѵ�
	BOOL MachineIsRobotAlarmAble();	// �κ� �˶��� ����� �� �ִ� ��������..
	BOOL ConfirmMachineRestart();	// �κ��� Run �� �� �ڵ� Run�� ��������..

};

