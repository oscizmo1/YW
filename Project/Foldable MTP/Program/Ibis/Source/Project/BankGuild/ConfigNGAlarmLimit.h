#pragma once


struct NGaAlarmLimitItem
{
	BOOL bUse;					// ���, �̻��
	CString strMatchingName;	// ���� �ҷ��� ���ڿ�
	int nCount;					// ���� �߻�Ƚ�� ����
	double dRate;				// ���� % ����

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

