#pragma once


struct NGaAlarmLimitItem
{
	BOOL bUse;					// 사용, 미사용
	CString strMatchingName;	// 비교할 불량명 문자열
	int nCount;					// 연속 발생횟수 제한
	double dRate;				// 누적 % 제한

	NGaAlarmLimitItem()
	{
		bUse = FALSE;
		nCount = 0;
		dRate = 0.;
	}
};

class CConfigNGAlarmLimit
{
public:
	CConfigNGAlarmLimit(void);
	~CConfigNGAlarmLimit(void);

	CList <NGaAlarmLimitItem, NGaAlarmLimitItem> m_listAlarm;

	void InitList();
	void AddList(NGaAlarmLimitItem item);

	void ReadNGAlarmFile(CString strPath);
	void SaveNGAlarmLimitFile(CString strPath, CString strBackupPath);
};

