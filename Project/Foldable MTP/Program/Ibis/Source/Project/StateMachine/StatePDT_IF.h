#pragma once
#include "BankGuild/UnitCtrlBank.h"

class CStatePDT_IF : 	public CUnitCtrlBank
{
public:
	CStatePDT_IF(CString strStateName, MP2100_THREAD nThreadID, JIG_ID jig) : CUnitCtrlBank(strStateName, nThreadID, FALSE, LOG_SEQUENCE_PDT_IF)
	{
		m_Shuttle = jig;
	};
	~CStatePDT_IF(void) {};

	enum
	{
		stepStart,				

		// PDT Able On Check -> MTP Start On
		stepPDT_AbleCheck,

		// PDT Vacuum On Check -> Vacuum On
		// PDT Vacuum Off Check -> Vacuum Off
		stepPDT_VacOnOff,

		// PDT Complete On -> MTP Complete On
		stepPDT_CompleteCheck,

		// PDT Bit Off Check -> MTP Bit Off
		stepPDT_BitOffCheck,

		stepEnd					
	};

	JIG_ID m_Shuttle;

	CString m_strCellID[JIG_CH_MAX];

	BOOL m_bVacOutput[JIG_CH_MAX];
	//kjpark 20180125 PDT Interface time out alarm 30->120
	CStopWatch m_TimerServival;
	int Run();
};
