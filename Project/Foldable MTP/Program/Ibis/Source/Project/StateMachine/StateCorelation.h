#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStateCorelation : 	public CUnitCtrlBank
{
public:
	CStateCorelation(CString strStateName, MP2100_THREAD nThreadID) : CUnitCtrlBank(strStateName, nThreadID, TRUE, LOG_SEQUENCE)
	{
	};
	~CStateCorelation(void) {};

	enum
	{
		stepStart,				

		// Z축 Up
		stepInspectionZ_UpMove,				

		// Z축 Up 체크 -> Y Load Move
		stepInspectionZ_UpCheck,				

		// Y Load 체크 -> Z Down
		stepShuttleY_AZone_Check,

		// Z축 Down 체크
		stepInspectionZ_DownCheck,				

		stepEnd					
	};

	int Run();
};
