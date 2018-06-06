#pragma once

#include "ThreadBank.h"

class CThread_IO_Alarm_Check
{
public:
	CThread_IO_Alarm_Check(void);
	~CThread_IO_Alarm_Check(void);

	// I/O Alarm Check Thread [12/18/2016 OSC]
	static UINT IOAlarmCheckThr(LPVOID lpParam);
};

