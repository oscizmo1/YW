#pragma once


class CTactTimeUnit
{
public:
	CTactTimeUnit (void)
	{ 
		m_bStart = FALSE;
	}
	~CTactTimeUnit(void) {}

private:
	BOOL m_bStart;
	CStopWatch m_Timer;
	CString m_strUnitname;

public:
	void SetUnitName(CString strUnitName);
	void Reset();
	void Start();
	void End();
	double GetSpanTime();
};

class CTactTimeLog
{
public:
	CTactTimeLog(void);
	~CTactTimeLog(void);

	CTactTimeUnit m_Shuttle_MoveAZone[JIG_ID_MAX];
	CTactTimeUnit m_Shuttle_MoveMCR[JIG_ID_MAX];
	CTactTimeUnit m_Shuttle_MoveBZone[JIG_ID_MAX];
	CTactTimeUnit m_InspectionZ_Up[JIG_ID_MAX];
	CTactTimeUnit m_InspectionZ_Down[JIG_ID_MAX];
	CTactTimeUnit m_RunTime[JIG_ID_MAX];
	CTactTimeUnit m_Interface[JIG_ID_MAX];
	void InitUnitName();
	void Reset();
};

