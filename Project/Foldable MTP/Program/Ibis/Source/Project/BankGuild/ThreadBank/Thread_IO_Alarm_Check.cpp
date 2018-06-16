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
	// FAN Alarm, Temp Alarm 등을 체크하는 Thread [12/18/2016 OSC]
	// 1초 정도로 체크 주기를 느긋하게 해도 되는 것들을 모아놓음

	int nThreadIndex = (int)lpParam;
	// MP2100 사용할 경우 각 Thread별로 Open
	theDeviceMotion.Open_Mp2100(MP2100_THREAD_IO_ALARM_CHECK);
	CUnitCtrlFunc					_func(MP2100_THREAD_IO_ALARM_CHECK);
	memset(theProcBank.m_OldInput, 0, MAX_IN_ID*sizeof(BOOL));


	// STO가 해제됬을 경우 Y축 자동 서보온 [10/8/2017 OSC]
	BOOL bSTO_Happen = FALSE;
	BOOL bEnableY1, bEnableY2, bEnableY3;


	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(1000);

		//////////////////////////////////////////////////////////////////////////
		// 센서 알람 체크
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
		// STO 체크
		// STO가 해제됬을 경우 Y축 자동 서보온.
		// 알람이 뜨지 않은 축만 하자 [10/8/2017 OSC]
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
