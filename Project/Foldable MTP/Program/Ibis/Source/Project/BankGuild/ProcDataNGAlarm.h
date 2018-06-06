#pragma once
#include "ConfigNGAlarmLimit.h"


class NGAlarmItem
{
public:
	CList <BOOL, BOOL> m_NGCount;

	BOOL bContinue;			// TRUE면 현재 연속 발생중
	int nContinueNGCount;	// 연속발생 카운트
	int nTotalNGCount;		// 누적발생 카운트

	NGAlarmItem()
	{
		Clear();
	}
	void Clear()
	{
		m_NGCount.RemoveAll();
		nContinueNGCount = 0;
		nTotalNGCount = 0;
	}
};

class CProcDataNGAlarm
{
public:
	CProcDataNGAlarm(void);
	~CProcDataNGAlarm(void);

private:
	//NGAlarmItem m_TolMaxOver[JIG_ID_MAX][JIG_CH_MAX];
	BOOL AlarmCheck(NGAlarmItem *pItem, NGaAlarmLimitItem *pLimit);
	void AddAlarm(BOOL bNG, NGAlarmItem *pItem, NGaAlarmLimitItem *pLimit);

#ifdef NOT_USE//나중에 사용하면 참고용으로 사용?
public:
	BOOL CheckTolMaxOverAlarm(JIG_ID jig, JIG_CH ch);
	void AddTolMaxOver(JIG_ID jig, JIG_CH ch, BOOL bNG);
	void ResetTolMaxOver(JIG_ID jig, JIG_CH ch);
#endif
};

