#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigDefectPattern
class CConfigDefectPattern
{
public:
	CConfigDefectPattern(void);
	~CConfigDefectPattern(void);

	CList <DEFECT_PATTERN, DEFECT_PATTERN> m_listDefectPatternBZone;	// B존에서 Display할 패턴
	CList <DEFECT_PATTERN, DEFECT_PATTERN> m_listDefectPatternDZone;	// D존에서 Display할 패턴

	DEFECT_PATTERN m_UndefinePattern;	// 매칭되는게 없을경우의 패턴

	void ReadPatternFile(CString strPath);
	BOOL FindPattern(CIM_DEFECT_POINT DefectPoint, DEFECT_PATTERN &pattern, ZONE_ID zone);
};

