#include "StdAfx.h"
#include "RecipeOption.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRecipeOption::CRecipeOption(void)
{
}


CRecipeOption::~CRecipeOption(void)
{
}

void CRecipeOption::ReadFile( CString strPath )
{
	CIni ini(strPath);
}

void CRecipeOption::SaveFile( CString strPath, CString strBackupPath )
{
	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	if(CFileSupport::FileCheck(strPath))
	{
		// ���� ���� ���
		::CopyFile(strPath, strBackupPath, FALSE);
		// File ������ ����ϰ� �ϱ� ���� ������ ����� ���� ���´�
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

}
