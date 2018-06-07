#pragma once

#include "ConfigSystem.h"
#include "ConfigOption.h"
#include "ConfigNGAlarmLimit.h"
#include "ConfigDefectPattern.h"
#include "ConfigDefectRetest.h"
#include "ConfigBZoneDefect.h"
#include "ConfigTimeOut.h"
#include "ConfigCIM.h"

//////////////////////////////////////////////////////////////////////////
// Recipe에 종속되지 않는 설정값들 관리하는 Bank
class CConfigBank
{
public:
	CConfigBank(void);
	~CConfigBank(void);

	CConfigSystem m_System;
	CConfigOption m_Option;
	CConfigNGAlarmLimit m_NGAlarmLimit;
	CConfigDefectPattern m_DefectPattern;
	CConfigDefectRetest m_DefectRetest;
	CConfigBZoneDefect m_BZoneDefect;
	CConfigTimeOut m_TimeOut;
	CConfigCIM m_CIM;

	void ReadSystemFile();
	void ReadOptionFile();
	void SaveOptionFile();
	void ReadLastDataFile();
	void SaveLastDataFile();
	//kjpark 20161121 MES code list read
	void ReadNGCodeFile();
	BOOL LoadDefectNameTable();
	void ReadNGAlarmLimitFile();
	void SaveNGAlarmLimitFile();
	void ReadDefectPatternFile();
	void ReadDefectRetestFile();
	void ReadBZoneDefectCodeFile();
	void ReadTimeOutFile();
	void SaveTimeOutFile();
	void ReadCIMFile();
	void SaveCIMFile();

	void ProgramRestartOptionInit();	// 프로그램 재시작시 항상 강제로 On/Off할 변수 처리하는 함수
};

