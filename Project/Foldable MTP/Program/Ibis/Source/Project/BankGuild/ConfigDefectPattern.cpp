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

	// ������ �����ϴ��� ����
	if(CFileSupport::IsFile(strPath) == FALSE) 	
	{	
		return;
	}

	CString strLine;						// ���Ͽ��� �о�� ������	
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
	// ���Ͽ��� 1�پ� �о�ͼ� ó���Ѵ�.
	int nIndex = 0;
	while ( file.ReadString(strLine) )
	{
		if (strLine.IsEmpty() )	continue;			// ������ ��� ����
		if (strLine.GetLength() < 6 )	continue;	// ���̰� �ʹ� �۾Ƶ� ����
		if (strLine.Left(1) == _T("?"))	continue;	// ó�� ���ڰ� " ? " �ϰ�� ���� �������� �Ѿ��.;
		if (strLine.Left(1) == _T("/"))	continue;	// ó�� ���ڰ� " / " �ϰ�� ���� �������� �Ѿ��.;
		if (strLine.Left(1) == _T("!"))	continue;	// ó�� ���ڰ� " ! " �ϰ�� ���� �������� �Ѿ��.;
		if (strLine.Left(1) == _T(";"))	continue;	// ó�� ���ڰ� " ; " �ϰ�� ���� �������� �Ѿ��.;

		// No,Defect Name, Pattern Name, Pattern No, Color(0x******), Line Thickness, Square Size
		::AfxExtractSubString( strDefectName,	strLine	, 1, ',');			
		::AfxExtractSubString( strPatternName,	strLine	, 2, ',');			
		::AfxExtractSubString( strPatternNo,	strLine	, 3, ',');			
		::AfxExtractSubString( strColor,		strLine	, 4, ',');			
		::AfxExtractSubString( strThickness,	strLine	, 5, ',');			
		::AfxExtractSubString( strSize,			strLine	, 6, ',');			
		::AfxExtractSubString( strZone,			strLine	, 7, ',');	// Display�� ��	

		if ( strDefectName.IsEmpty() || strPatternName.IsEmpty() || strPatternNo.IsEmpty() || strColor.IsEmpty() ) continue;			// �����̶�� ����

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
// 	CStringList listPatternName;	// ������ �ҷ� �ϳ��� �������� ���� ����
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
// 		// ó�� ������ �ҷ��� ���ϰ� ���������� ������ ���ϸ���� ���� ���ؾ� �Ѵ�.
// 		// ���� �ҷ��̾ �ٸ� ���Ͽ��� �� �ߺ��̴� ��� �����ϱ� ����..  [6/2/2017 OSC]
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
