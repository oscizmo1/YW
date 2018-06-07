#include "StdAfx.h"
#include "ConfigDefectPattern.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigDefectPattern::CConfigDefectPattern(void)
{	
}


CConfigDefectPattern::~CConfigDefectPattern(void)
{
}

void CConfigDefectPattern::ReadPatternFile(CString strPath)
{
	m_listDefectPatternBZone.RemoveAll();

	DEFECT_PATTERN pattern;

	// 파일이 존재하는지 점검
	if(CFileSupport::IsFile(strPath) == FALSE) 	
	{	
		return;
	}

	CString strLine;						// 파일에서 읽어온 데이터	
	CString strDefectName;
	CString strPatternName;
	CString strPatternNo;
	CString strColor;
	CString strThickness;
	CString strSize;
	CString strZone;

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strPath.GetString(), _T("r"));	// UNICODE"));
	if (e != 0) return ;

	CStdioFile		file(fStream);
	//map<CString, DEFECT_MES_CODE> iMapDefectList;
	// 파일에서 1줄씩 읽어와서 처리한다.
	int nIndex = 0;
	while ( file.ReadString(strLine) )
	{
		if (strLine.IsEmpty() )	continue;			// 공백일 경우 무시
		if (strLine.GetLength() < 6 )	continue;	// 길이가 너무 작아도 무시
		if (strLine.Left(1) == _T("?"))	continue;	// 처음 글자가 " ? " 일경우 다음 문장으로 넘어간다.;
		if (strLine.Left(1) == _T("/"))	continue;	// 처음 글자가 " / " 일경우 다음 문장으로 넘어간다.;
		if (strLine.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;
		if (strLine.Left(1) == _T(";"))	continue;	// 처음 글자가 " ; " 일경우 다음 문장으로 넘어간다.;

		// No,Defect Name, Pattern Name, Pattern No, Color(0x******), Line Thickness, Square Size
		::AfxExtractSubString( strDefectName,	strLine	, 1, ',');			
		::AfxExtractSubString( strPatternName,	strLine	, 2, ',');			
		::AfxExtractSubString( strPatternNo,	strLine	, 3, ',');			
		::AfxExtractSubString( strColor,		strLine	, 4, ',');			
		::AfxExtractSubString( strThickness,	strLine	, 5, ',');			
		::AfxExtractSubString( strSize,			strLine	, 6, ',');			
		::AfxExtractSubString( strZone,			strLine	, 7, ',');	// Display할 존	

		if ( strDefectName.IsEmpty() || strPatternName.IsEmpty() || strPatternNo.IsEmpty() || strColor.IsEmpty() ) continue;			// 공백이라면 무시

		strDefectName.MakeUpper();
		strPatternName.MakeUpper();

		pattern.nPatternNo = _tstoi(strPatternNo.GetString());
		pattern.strDefectName = strDefectName;
		pattern.strPatternName = strPatternName;
		pattern.colorLine = _tstoi(strColor.GetString());
		pattern.nThickness = _tstoi(strThickness.GetString());
		pattern.nSize = _tstoi(strSize.GetString());

		if(strDefectName == TEXT_UNDEFINED)
			m_UndefinePattern = pattern;
		else if(strZone == _T("B"))
			m_listDefectPatternBZone.AddTail(pattern);
		else if(strZone == _T("D"))
			m_listDefectPatternDZone.AddTail(pattern);
	}

	file.Close();

}

BOOL CConfigDefectPattern::FindPattern(CIM_DEFECT_POINT DefectPoint, DEFECT_PATTERN &pattern, ZONE_ID zone)
{
// 	DEFECT_PATTERN listpattern;
// 	CList <DEFECT_PATTERN, DEFECT_PATTERN> *pList;
// 	if(zone == ZONE_ID_B)
// 		pList = &m_listDefectPatternBZone;
// 	else if(zone == ZONE_ID_D)
// 		pList = &m_listDefectPatternDZone;
// 
// 	CString strDefectName = DefectPoint.strDefectName;
// 	CString strPatternName;
// 	CStringList listPatternName;	// 위에서 불량 하나당 내려오는 패턴 갯수
// 	CTokenizer t(DefectPoint.strPatternName);
// 	int nPatternCnt = t.GetTokenCount(_T(","));
// 	listPatternName.RemoveAll();
// 	for(int i = 0; i < nPatternCnt; i++)
// 	{
// 		listPatternName.AddTail(t.GetNextToken(_T(",")));
// 	}
// 
// 	POSITION pos = pList->GetHeadPosition();
// 	POSITION posPatternName;
// 
// 	while(pos)
// 	{
// 		// 처음 구현은 불량명만 비교하게 구현했지만 원래는 패턴명까지 같이 비교해야 한다.
// 		// 같은 불량이어도 다른 패턴에서 더 잘보이는 경우 대응하기 위함..  [6/2/2017 OSC]
// 		listpattern = pList->GetNext(pos);
// 		if(listpattern.strDefectName == strDefectName)
// 		{
// 			posPatternName = listPatternName.GetHeadPosition();
// 			while(posPatternName)
// 			{
// 				strPatternName = listPatternName.GetNext(posPatternName);
// 				if(listpattern.strPatternName.Find(strPatternName) >= 0)
// 				{
// 					pattern = listpattern;
// 					return TRUE;
// 				}
// 			}
// 		}
// 	}
	return FALSE;
}
