#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigBZoneDefect
class CConfigBZoneDefect
{
public:
	CConfigBZoneDefect(void);
	~CConfigBZoneDefect(void);


	TDefectList			m_rDefectList;			// Defect�� ���� �����͸� �����Ѵ� ����ü

	void ReadBZoneDefectCodeFile(CString strPath);
};

