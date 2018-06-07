#pragma once

#include "ThreadBank.h"

class CThread_FDC_Alarm_Check
{
public:
	CThread_FDC_Alarm_Check(void);
	~CThread_FDC_Alarm_Check(void);

	// FDC Monitoring Thread [12/7/2016 OSC]
	static UINT FDCMonitorThr(LPVOID lpParam);
};

