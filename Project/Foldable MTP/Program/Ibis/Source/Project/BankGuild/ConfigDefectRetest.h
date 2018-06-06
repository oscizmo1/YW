#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigDefectRetest
class CConfigDefectRetest
{
public:
	CConfigDefectRetest(void);
	~CConfigDefectRetest(void);

	// 상위에 L(Loss)이 아닌 R(Retest)로 보고할 불량명 리스트
	CStringList m_listRetest;

	void ReadDefectRetestFile(CString strPath);
	BOOL FindRetestList(CString strDefectName);
};

