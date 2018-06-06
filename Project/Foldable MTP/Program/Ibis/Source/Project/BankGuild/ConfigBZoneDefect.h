#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigBZoneDefect
class CConfigBZoneDefect
{
public:
	CConfigBZoneDefect(void);
	~CConfigBZoneDefect(void);


	TDefectList			m_rDefectList;			// Defect에 대한 데이터를 관리한는 구조체

	void ReadBZoneDefectCodeFile(CString strPath);
};

