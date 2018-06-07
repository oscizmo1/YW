#include "StdAfx.h"
#include "ConfigBZoneDefect.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigBZoneDefect::CConfigBZoneDefect(void)
{	
}


CConfigBZoneDefect::~CConfigBZoneDefect(void)
{
}

void CConfigBZoneDefect::ReadBZoneDefectCodeFile(CString strPath)
{
	CIni iniFile;
	CString sGroup, sKey, sData, sLine, sTok;
	int i, j, k, nCount, nDefectCount;


	m_rDefectList = TDefectList();

	// 파일이 존재하는지 점검
	if ( CFileSupport::FileCheck( strPath ) == FALSE ) 
	{
		return;
	}

	// 파일이 존재하면 파일에서 데이터를 읽어온다.
	iniFile.SetIniFilename( strPath );			// file이름을 지정
	nCount = iniFile.GetLong( _T("GROUP"), _T("GROUP_COUNT"),	0);

	// Group 최대 수량을 제한한다.
	if ( nCount > TDefectList::eMAX_GoupCount ) nCount = TDefectList::eMAX_GoupCount;

	m_rDefectList.nGroupCount	= nCount;				// 그룹 수

	for (i = 0; i < nCount; i++)
	{
		sGroup.Format( _T("GROUP_%d"), i );			// 그룹이름, 00 ~ 11

		//old m_rDefectList.sGroupName[i]	= iniFile.GetString( sGroup, _T("GROUP_NAME"), _T("") );			// 그룹 이름
		sLine = iniFile.GetString( sGroup, _T("GROUP_NAME"), _T("") );										// 그룹 이름
		// 각 언어별로 3개의 Group Name이 존재한다.
		for (k = 0; k < LAN_MAX; k++)
		{
			// 각 언어별 Defect 명칭을 읽어온다. ',' 기호로 분리하지 못한 경우에는 바로 끝낸다.
			if ( AfxExtractSubString(m_rDefectList.sGroupName[i][k], sLine, k, _T(',')) == FALSE )	break;
		}// for k

		nDefectCount = iniFile.GetLong( sGroup, _T("DEFECT_COUNT"),	0);										// 해당 그룹의 Defect 수

		if ( nDefectCount > TDefectList::eMAX_DefectCount ) nDefectCount = TDefectList::eMAX_DefectCount;	// Defect수 제한

		m_rDefectList.nDefectCount[i] = nDefectCount;			// 그룹내 defect 갯수

		// 그룹에 해당되는 Defect 항목을 읽어온다.
		for (j = 0; j < nDefectCount; j++ )
		{
			sKey.Format(_T("DEF_%d"), j);				// Key
			m_rDefectList.rDefectData[i][j].sLine	= iniFile.GetString( sGroup, sKey, _T("") );				// Defect 이름 정보를 가져온다.

			// DEF_05=USA06>C-Panel들뜸>C-Panel Come Off>C-Panel翘起
			sLine = m_rDefectList.rDefectData[i][j].sLine;

			if ( sLine.IsEmpty() == FALSE )			// 내용이 있는 경우에 실행
			{
				// Code를 가져온다.
				AfxExtractSubString(m_rDefectList.rDefectData[i][j].sCode, sLine, 0, _T(','));	// 첫번째 항목인 Defect Code를 읽어낸다.

				// 각 언어별로 3개의 Defect name이 존재한다.
				for (k = 0; k < LAN_MAX; k++)
				{
					// 각 언어별 Defect 명칭을 읽어온다. ',' 기호로 분리하지 못한 경우에는 바로 끝낸다.
					if ( AfxExtractSubString(m_rDefectList.rDefectData[i][j].sDefect[k], sLine, (k + 1), _T(',')) == FALSE )	break;
				}// for k
			}//of if not emtpy
		}//of for j
	}//of for i


	// 단축 defect을 읽어온다.
	sGroup = _T("SHORTCUT");
	nCount = iniFile.GetLong( sGroup, _T("SHORTCUT_COUNT"),	0);

	// Shortcut 최대 수량을 제한한다.eMAX_ShortcutCount	= 10,			// 단축 defect 수량, 8개
	if ( nCount > TDefectList::eMAX_ShortcutCount ) nCount = TDefectList::eMAX_ShortcutCount;
	m_rDefectList.nShortcutCount = nCount;

	for (i = 0; i < nCount; i++)
	{
		sKey.Format( _T("SHORTCUT_%d"), i );			// Shortcut 항목, 00 ~ 07

		m_rDefectList.rShortcut[i].sLine	= iniFile.GetString( sGroup, sKey, _T("") );				// Shortcut Defect 이름 정보를 가져온다.

		// SHORTCUT_05=USA06>C-Panel들뜸>C-Panel Come Off>C-Panel翘起
		sLine = m_rDefectList.rShortcut[i].sLine;

		if ( sLine.IsEmpty() == FALSE )			// 내용이 있는 경우에 실행
		{
			// Code를 가져온다.
			AfxExtractSubString(m_rDefectList.rShortcut[i].sCode, sLine, 0, _T(','));	// 첫번째 항목인 Defect Code를 읽어낸다.

			// 각 언어별로 3개의 Defect name이 존재한다.
			for (j = 0; j < LAN_MAX; j++)
			{
				// 각 언어별 Defect 명칭을 읽어온다. '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
				if ( AfxExtractSubString(m_rDefectList.rShortcut[i].sDefect[j], sLine, (j + 1), _T(',')) == FALSE )	break;
			}//of for j
		}//of if not empty
	}//of for i

}
