#pragma once
#include "ConfigNGAlarmLimit.h"


class NGAlarmItem
{
public:
	CList <BOOL, BOOL> m_NGCount;

	BOOL bContinue;			// TRUE�� ���� ���� �߻���
	int nContinueNGCount;	// ���ӹ߻� ī��Ʈ
	int nTotalNGCount;		// �����߻� ī��Ʈ

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

#ifdef NOT_USE//���߿� ����ϸ� ��������� ���?
public:
	BOOL CheckTolMaxOverAlarm(JIG_ID jig, JIG_CH ch);
	void AddTolMaxOver(JIG_ID jig, JIG_CH ch, BOOL bNG);
	void ResetTolMaxOver(JIG_ID jig, JIG_CH ch);
#endif
};

