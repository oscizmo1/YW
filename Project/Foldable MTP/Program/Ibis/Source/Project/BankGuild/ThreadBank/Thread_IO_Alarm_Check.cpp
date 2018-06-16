#include "StdAfx.h"
#include "Thread_IO_Alarm_Check.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CThread_IO_Alarm_Check::CThread_IO_Alarm_Check(void)
{
}


CThread_IO_Alarm_Check::~CThread_IO_Alarm_Check(void)
{
}

UINT CThread_IO_Alarm_Check::IOAlarmCheckThr( LPVOID lpParam )
{
	//////////////////////////////////////////////////////////////////////////
	// FAN Alarm, Temp Alarm ���� üũ�ϴ� Thread [12/18/2016 OSC]
	// 1�� ������ üũ �ֱ⸦ �����ϰ� �ص� �Ǵ� �͵��� ��Ƴ���

	int nThreadIndex = (int)lpParam;
	// MP2100 ����� ��� �� Thread���� Open
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_IO_ALARM_CHECK);
	CUnitCtrlFunc					_func(MP2100_THREAD_IO_ALARM_CHECK);
	memset(theProcBank.m_OldInput, 0, MAX_IN_ID*sizeof(BOOL));


	// STO�� �������� ��� Y�� �ڵ� ������ [10/8/2017 OSC]
	BOOL bSTO_Happen = FALSE;
	BOOL bEnableY1, bEnableY2, bEnableY3;


	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(1000);

		//////////////////////////////////////////////////////////////////////////
		// ���� �˶� üũ
		if(theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_FAULT)
		{
			_func.Temp_Check();
		}
		_func.EMSSwitch_Check();
		if(theProcBank.MachineIsRunState())
		{
			_func.FanAlarm_Check();
			_func.MCPower_Check();
			_func.AutoTeachKey_AutoCheck(TRUE);
		}
		_func.Door_Key_On_Check();
		//////////////////////////////////////////////////////////////////////////
		// STO üũ
		// STO�� �������� ��� Y�� �ڵ� ������.
		// �˶��� ���� ���� �ุ ���� [10/8/2017 OSC]
		if(bSTO_Happen)
		{
			if(_func.STO_Check() != STO_ALARM)
			{
				bEnableY1 = bEnableY2 = bEnableY3 = FALSE;
				if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_IO_ALARM_CHECK, AXIS_JIG_SHUTTLE_Y1).bAlarmOn == FALSE)
					bEnableY1 = TRUE;
				if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_IO_ALARM_CHECK, AXIS_JIG_SHUTTLE_Y2).bAlarmOn == FALSE)
					bEnableY2 = TRUE;

				if(bEnableY1) theDeviceMotion.ServoOff(MP2100_THREAD_IO_ALARM_CHECK, AXIS_JIG_SHUTTLE_Y1);
				if(bEnableY2) theDeviceMotion.ServoOff(MP2100_THREAD_IO_ALARM_CHECK, AXIS_JIG_SHUTTLE_Y2);
				Sleep(200);
				if(bEnableY1) theDeviceMotion.ResetAlarm(MP2100_THREAD_IO_ALARM_CHECK, AXIS_JIG_SHUTTLE_Y1);
				if(bEnableY2) theDeviceMotion.ResetAlarm(MP2100_THREAD_IO_ALARM_CHECK, AXIS_JIG_SHUTTLE_Y2);
				Sleep(1000);
				if(bEnableY1) theDeviceMotion.ServoOn(MP2100_THREAD_IO_ALARM_CHECK, AXIS_JIG_SHUTTLE_Y1);
				if(bEnableY2) theDeviceMotion.ServoOn(MP2100_THREAD_IO_ALARM_CHECK, AXIS_JIG_SHUTTLE_Y2);
				bSTO_Happen = FALSE;
			}
		}
		else
		{
			if(_func.STO_Check() == STO_ALARM)
				bSTO_Happen = TRUE;
		}
		//////////////////////////////////////////////////////////////////////////
	}

	return 0;
}
