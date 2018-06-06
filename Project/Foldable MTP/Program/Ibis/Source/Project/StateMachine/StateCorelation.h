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

		// Z�� Up
		stepInspectionZ_UpMove,				

		// Z�� Up üũ -> Y Load Move
		stepInspectionZ_UpCheck,				

		// Y Load üũ -> Z Down
		stepShuttleY_AZone_Check,

		// Z�� Down üũ
		stepInspectionZ_DownCheck,				

		stepEnd					
	};

	int Run();
};
