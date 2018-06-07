#pragma once

//////////////////////////////////////////////////////////////////////////
// CConfigDefectPattern
class CConfigDefectPattern
{
public:
	CConfigDefectPattern(void);
	~CConfigDefectPattern(void);

	CList <DEFECT_PATTERN, DEFECT_PATTERN> m_listDefectPatternBZone;	// B������ Display�� ����
	CList <DEFECT_PATTERN, DEFECT_PATTERN> m_listDefectPatternDZone;	// D������ Display�� ����

	DEFECT_PATTERN m_UndefinePattern;	// ��Ī�Ǵ°� ��������� ����

	void ReadPatternFile(CString strPath);
	BOOL FindPattern(CIM_DEFECT_POINT DefectPoint, DEFECT_PATTERN &pattern, ZONE_ID zone);
};

