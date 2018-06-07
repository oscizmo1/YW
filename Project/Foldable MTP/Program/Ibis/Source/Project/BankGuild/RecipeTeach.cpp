#include "StdAfx.h"
#include "RecipeTeach.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRecipeTeach::CRecipeTeach(void)
{
	m_dShuttleY1_MCR = 0.;
	m_dShuttleY2_MCR = 0.;

	m_doldShuttleY1_MCR = m_dShuttleY1_MCR;
	m_doldShuttleY2_MCR = m_dShuttleY2_MCR;
}


CRecipeTeach::~CRecipeTeach(void)
{
}

void CRecipeTeach::ReadFile( CString strPath )
{
	CIni ini(strPath);
	m_dShuttleY1_MCR = ini.GetDouble(_T("TEACH")	, _T("m_dShuttleY1_MCR")	, 0.);
	m_dShuttleY2_MCR = ini.GetDouble(_T("TEACH")	, _T("m_dShuttleY2_MCR")	, 0.);

	m_doldShuttleY1_MCR = m_dShuttleY1_MCR;
	m_doldShuttleY2_MCR = m_dShuttleY2_MCR;
}

BOOL CRecipeTeach::SaveFile( CString strPath, CString strBackupPath )
{
	// 값이 변경됬으면 TRUE [9/26/2017 OSC]
	BOOL bRet = FALSE;

	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return FALSE;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return FALSE;
	}

	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	if(m_doldShuttleY1_MCR != m_dShuttleY1_MCR)
	{
		m_doldShuttleY1_MCR = m_dShuttleY1_MCR;
		bRet = TRUE;
	}
	if(m_doldShuttleY2_MCR != m_dShuttleY2_MCR)
	{
		m_doldShuttleY2_MCR = m_dShuttleY2_MCR;
		bRet = TRUE;
	}

	CIni ini(strPath);
	ini.WriteDouble(_T("TEACH")		, _T("m_dShuttleY1_MCR")	, m_dShuttleY1_MCR	);
	ini.WriteDouble(_T("TEACH")		, _T("m_dShuttleY2_MCR")	, m_dShuttleY2_MCR	);
	// 값이 변경됬으면 TRUE [9/26/2017 OSC]
	return bRet;
}
