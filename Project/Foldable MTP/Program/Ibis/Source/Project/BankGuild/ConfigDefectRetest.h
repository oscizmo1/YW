#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigDefectRetest
class CConfigDefectRetest
{
public:
	CConfigDefectRetest(void);
	~CConfigDefectRetest(void);

	// ������ L(Loss)�� �ƴ� R(Retest)�� ������ �ҷ��� ����Ʈ
	CStringList m_listRetest;

	void ReadDefectRetestFile(CString strPath);
	BOOL FindRetestList(CString strDefectName);
};

