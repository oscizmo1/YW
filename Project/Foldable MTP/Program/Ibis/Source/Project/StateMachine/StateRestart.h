#pragma once
#include "BankGuild/UnitCtrlBank.h"


class CStateRestart : 	public CUnitCtrlBank
{
public:
	CStateRestart(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID)
	{
		m_LogIndex = LOG_SEQUENCE_RESTART;
	};
	~CStateRestart(void) {};

	enum
	{
		//kjpark 20170915 Restart ±¸Çö
		stepStart,
		stepVacOnOff,
		stepVacChk,
		stepZUpChk,		
		stepEnd,
	};

	ALARM_ID m_LastAlarm;

	int Run();

	void Restart_VacuumOn();
	BOOL Restart_VacuumChk();
	void Restart_BlowOff();
	void Restart_ZUp();
	BOOL Restart_ZUpChk();	
};

