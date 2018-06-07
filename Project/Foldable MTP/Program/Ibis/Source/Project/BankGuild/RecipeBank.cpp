#include "StdAfx.h"
#include "RecipeBank.h"
#include "IbisApp.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRecipeBank::CRecipeBank(void)
{
	//kjaprk 20161014 FPBC 사용 유무 확인, Vacuum 에서 사용하기 위해
	m_Cell.m_bFPCBuse = FALSE;
}


CRecipeBank::~CRecipeBank(void)
{
}

void CRecipeBank::ReadLastRecipeFile()
{
	CString strPath;
	strPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigPath);

	CIni ini(strPath);
	CString strName = ini.GetString(_T("LAST_DATA"), _T("RECIPE_NAME"));

	// 지금 불러오는걸 현재 Recipe로 지정한다
	SetMainRecipeName(strName);
	ReadRecipeFile(strName);
}

void CRecipeBank::ReadRecipeFile( CString strRecipeName )
{
	CString strPath;

	strPath.Format(_T("%s\\%s\\Cell.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName);
	m_Cell.ReadFile(strPath);	
	strPath.Format(_T("%s\\%s\\Teach.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName);
	m_Teach.ReadFile(strPath);
	strPath.Format(_T("%s\\%s\\Option.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName);
	m_Option.ReadFile(strPath);
	
	//kjpark 20171016 레시피 파일 끝에 버전 표기 _VER** 높은 수 반영 
	strPath = GetHigherVersionRecipe(strRecipeName);
	// 파일이 없으면 00VER으로 저장 [10/17/2017 OSC]
	if(strPath.GetLength() == 0)
		strPath.Format(_T("%s\\%s\\MTP_RECIPE_%s_00VER.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName, strRecipeName);
	m_bModule_LoadSuccess = m_Module.ReadFile(strPath);
}

//kjpark 20171016 레시피 파일 끝에 버전 표기 _VER** 높은 수 반영 
CString CRecipeBank::GetHigherVersionRecipe(CString strRecipe)
{
	CString path, file_path, file_name, strHigherVersion;
	int nHigherVersion;
	path.Format(_T("%s\\%s\\*.*"), theConfigBank.m_System.m_strRecipePath, strRecipe);
	CFileFind finder;
	BOOL bRes, bFirst = TRUE;;
	CString strVersion;
	bRes = finder.FindFile(path);
	while(bRes)
	{
		bRes = finder.FindNextFileW();
		if(!finder.IsDirectory())
		{
			BOOL bHigherVer = FALSE;
			/*
			file_name = finder.GetFileName();
			file_path.Format(_T("%s\\%s\\%s"), theConfigBank.m_System.m_strRecipePath, strRecipe, file_name);
			strVersion = file_name.Mid(file_name.GetLength()-6, 2);

			if(strVersion.GetLength() == 2 && (_ttoi(strVersion) >= 0 && _ttoi(strVersion) <= 99) && file_name.Find(_T("MTP_RECIPE_")) > -1)
			{
				if(bFirst)
				{
					nHigherVersion = _ttoi(strVersion);
					strHigherVersion = file_path;
					bFirst = FALSE;
				}

				int nNew = _ttoi(strVersion);
				if(nNew > nHigherVersion)
				{
					strHigherVersion = file_path;
					nHigherVersion = nNew;
				}
			}*/

			file_name = finder.GetFileTitle();

			// 'MTP_RECIPE_' 글자가 포함되있고, 마지막 글자가 'VER'로 끝나야 함 [10/17/2017 OSC]
			if( (file_name.Find(_T("MTP_RECIPE_")) != -1) 
				&& (file_name.Right(3) == _T("VER")) )
			{
				strVersion = file_name.Mid(file_name.GetLength()-5, 2);
				if( (_ttoi(strVersion) >= 0 && _ttoi(strVersion) <= 99) )
				{
					file_path = finder.GetFilePath();
					if(bFirst)
					{
						nHigherVersion = _ttoi(strVersion);
						strHigherVersion = file_path;
						bFirst = FALSE;
					}

					int nNew = _ttoi(strVersion);
					if(nNew > nHigherVersion)
					{
						strHigherVersion = file_path;
						nHigherVersion = nNew;
					}
				}
			}
		}
	}
	return strHigherVersion;
}

void CRecipeBank::SaveMainRecipeFile()
{
	SaveRecipeFile(m_strRecipeName);
}

void CRecipeBank::SaveRecipeFile( CString strRecipeName )
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;

	strPath.Format(_T("%s\\%s\\Cell.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName);
	strBackupPath.Format(_T("%s\\%d_%d\\%s\\Cell_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), strRecipeName, time.GetDay(), time.Format(_T("%H%M%S")));
	m_Cell.SaveFile(strPath, strBackupPath);

// 	SaveMainRecipeTeachFile();
	strPath.Format(_T("%s\\%s\\Teach.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName);
	strBackupPath.Format(_T("%s\\%d_%d\\%s\\Teach_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), strRecipeName, time.GetDay(), time.Format(_T("%H%M%S")));
	m_Teach.SaveFile(strPath, strBackupPath);

	strPath.Format(_T("%s\\%s\\Option.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName);
	strBackupPath.Format(_T("%s\\%d_%d\\%s\\Option_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), strRecipeName, time.GetDay(), time.Format(_T("%H%M%S")));
	m_Option.SaveFile(strPath, strBackupPath);

	//kjpark 20170912 Inspection Module 레시피 수정	
	//kjpark 20171016 레시피 파일 끝에 버전 표기 _VER** 높은 수 반영 
	strPath = GetHigherVersionRecipe(strRecipeName);
	// 파일이 없으면 00VER으로 저장 [10/17/2017 OSC]
	if(strPath.GetLength() == 0)
		strPath.Format(_T("%s\\%s\\MTP_RECIPE_%s_00VER.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName, strRecipeName);
// 	strBackupPath.Format(_T("%s\\%d_%d\\%s\\InspModule_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
// 		time.GetYear(), time.GetMonth(), strRecipeName, time.GetDay(), time.Format(_T("%H%M%S")));
	m_Module.SaveFile(strPath);

	SaveRMSDataFile();
}

void CRecipeBank::SaveMainRecipeTeachFile()
{
	// 티칭값만 따로 저장하는 함수. CTeachBank::SaveTeachFile에서 호출한다
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;

	strPath.Format(_T("%s\\%s\\Teach.ini"), theConfigBank.m_System.m_strRecipePath, m_strRecipeName);
	strBackupPath.Format(_T("%s\\%d_%d\\%s\\Teach_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), m_strRecipeName, time.GetDay(), time.Format(_T("%H%M%S")));
	if(m_Teach.SaveFile(strPath, strBackupPath))
	{
		// Recipe 수정 보고 [11/30/2016 OSC]
		SaveRMSDataFile();
		theSocketInterFace.m_CIM.SendCmdRecipeToDataPC(EQUIP_SET_PPID_MODIFY, theRecipeBank.m_strRecipeName);
	}
}

void CRecipeBank::SaveRMSDataFile()
{
	// CIM 프로그램에서 읽을 수 있도록 적는다 [9/26/2017 OSC]
	CString strPath, strSection, strValue;
	strPath.Format(_T("%s\\%s\\RMSData.ini"), theConfigBank.m_System.m_strRecipePath, m_strRecipeName);

	if(CFileSupport::FileCheck(strPath))
	{
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

	ini.WriteDouble(_T("SHUTTLE1_MCR"), _T("Data"), m_Teach.m_dShuttleY1_MCR);
	ini.WriteDouble(_T("SHUTTLE2_MCR"), _T("Data"), m_Teach.m_dShuttleY2_MCR);

	// 검사모듈은 파일수정으로 관리하니 CIM으로 관리를 할 수 없다(Modify 이벤트를 발생시킬 수 없다)... [9/28/2017 OSC]
// 	int nCount = m_Module.m_vct_AZone_Bef.size();
// 	for(int i = 0; i < 10; i++)
// 	{
// 		if(i < nCount)
// 			strValue.Format(_T("%s"), m_Module.m_vct_AZone_Bef[i]);
// 		else
// 			strValue.Empty();
// 
// 		strSection.Format(_T("AZONE_BEF_ITEM_%02d"), i+1);
// 		ini.WriteString(strSection	, _T("Data")				, strValue);
// 	}
// 
// 	nCount = m_Module.m_vct_AZone_Aft.size();
// 	for(int i = 0; i < 10; i++)
// 	{
// 		if(i < nCount)
// 			strValue.Format(_T("%s"), m_Module.m_vct_AZone_Aft[i]);
// 		else
// 			strValue.Empty();
// 
// 		strSection.Format(_T("AZONE_AFT_ITEM_%02d"), i+1);
// 		ini.WriteString(strSection	, _T("Data")				, strValue);
// 	}
// 
// 	nCount = m_Module.m_vct_BZone_Bef.size();
// 	for(int i = 0; i < 10; i++)
// 	{
// 		if(i < nCount)
// 			strValue.Format(_T("%s"), m_Module.m_vct_BZone_Bef[i]);
// 		else
// 			strValue.Empty();
// 
// 		strSection.Format(_T("BZONE_BEF_ITEM_%02d"), i+1);
// 		ini.WriteString(strSection	, _T("Data")				, strValue);
// 	}
// 
// 	nCount = m_Module.m_vct_BZone_Aft.size();
// 	for(int i = 0; i < 10; i++)
// 	{
// 		if(i < nCount)
// 			strValue.Format(_T("%s"), m_Module.m_vct_BZone_Aft[i]);
// 		else
// 			strValue.Empty();
// 
// 		strSection.Format(_T("BZONE_AFT_ITEM_%02d"), i+1);
// 		ini.WriteString(strSection	, _T("Data")				, strValue);
// 	}

}

void CRecipeBank::CreateRecipe_fromRMSData( CString strRecipeName )
{
	// CIM 상위에서 역으로 만든 RMSData.ini를 가지고 신규 모델을 만든다 [9/27/2017 OSC]
	CString strPath, strSection, strValue;
	strPath.Format(_T("%s\\%s\\RMSData.ini"), theConfigBank.m_System.m_strRecipePath, strRecipeName);
	if(CFileSupport::FileCheck(strPath) == FALSE)
	{
		return;
	}

	CIni ini(strPath);

	m_Teach.m_dShuttleY1_MCR = ini.GetDouble(_T("SHUTTLE1_MCR"), _T("Data"), m_Teach.m_dShuttleY1_MCR);
	m_Teach.m_dShuttleY2_MCR = ini.GetDouble(_T("SHUTTLE2_MCR"), _T("Data"), m_Teach.m_dShuttleY2_MCR);

	// 검사모듈은 파일수정으로 관리하니 CIM으로 관리를 할 수 없다(Modify 이벤트를 발생시킬 수 없다)... [9/28/2017 OSC]
// 	m_Module.m_vct_AZone_Bef.clear();
// 	for(int i = 0; i < 10; i++)
// 	{
// 		strSection.Format(_T("AZONE_BEF_ITEM_%02d"), i+1);
// 		strValue = ini.GetString(strSection	, _T("Data"));
// 		if(strValue.IsEmpty())
// 			break;
// 		else
// 			m_Module.m_vct_AZone_Bef.push_back(strValue);
// 	}
// 
// 	m_Module.m_vct_AZone_Aft.clear();
// 	for(int i = 0; i < 10; i++)
// 	{
// 		strSection.Format(_T("AZONE_AFT_ITEM_%02d"), i+1);
// 		strValue = ini.GetString(strSection	, _T("Data"));
// 		if(strValue.IsEmpty())
// 			break;
// 		else
// 			m_Module.m_vct_AZone_Aft.push_back(strValue);
// 	}
// 
// 	m_Module.m_vct_BZone_Bef.clear();
// 	for(int i = 0; i < 10; i++)
// 	{
// 		strSection.Format(_T("BZONE_BEF_ITEM_%02d"), i+1);
// 		strValue = ini.GetString(strSection	, _T("Data"));
// 		if(strValue.IsEmpty())
// 			break;
// 		else
// 			m_Module.m_vct_BZone_Bef.push_back(strValue);
// 	}
// 
// 	m_Module.m_vct_BZone_Aft.clear();
// 	for(int i = 0; i < 10; i++)
// 	{
// 		strSection.Format(_T("BZONE_AFT_ITEM_%02d"), i+1);
// 		strValue = ini.GetString(strSection	, _T("Data"));
// 		if(strValue.IsEmpty())
// 			break;
// 		else
// 			m_Module.m_vct_BZone_Aft.push_back(strValue);
// 	}

	SaveRecipeFile(strRecipeName);
}

void CRecipeBank::SaveLastRecipeName()
{
	CString strPath,strBackupPath;
	strPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigBackupPath);
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

// 	if(CFileSupport::FileCheck(strPath))
// 	{
// 		// 기존 파일 백업
// 		::CopyFile(strPath, strBackupPath, FALSE);
// 		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
// 		::DeleteFile(strPath);
// 	}

	CIni ini(strPath);

	ini.WriteString(_T("LAST_DATA")		, _T("RECIPE_NAME")			, theRecipeBank.m_strRecipeName	);

}
