#include "StdAfx.h"
#include "UnitStatusBank.h"
#include "../PublicClass/FileSupport.h"
#include "../PublicClass/Ini.h"


CUnitStatusBank::CUnitStatusBank(void)
{
}


CUnitStatusBank::~CUnitStatusBank(void)
{
}

int CUnitStatusBank::GetUnitCount()
{
	return (JIG_ID_MAX*JIG_CH_MAX);
}

CUnitStatus * CUnitStatusBank::GetUnit(CString strUnitID)
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			if(m_Unit[jig][ch].m_strUnitID == strUnitID)
				return &m_Unit[jig][ch];
		}
	}
	return NULL;
}

void CUnitStatusBank::SetUnitID(CString strEQPID)
{
	int nFullCh;
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			nFullCh = ((jig*JIG_CH_MAX) + ch)+1;
			m_Unit[jig][ch].m_strUnitID.Format(_T("%s_CN%02d"), strEQPID, nFullCh);
		}
	}
}

// void CUnitStatusBank::SetMoveStatus(JIG_ID jig, JIG_CH ch, STATUS_MOVE value)
// {
// 	if(m_Unit[jig][ch].SetMove(value))
// 	{
// 		theSocketInterFace.m_CIM.SendUnitStatusChange(m_Unit[jig][ch]);
// 	}
// }

// void CUnitStatusBank::SaveStatus()
// {
// 	CString strPath,strBackupPath;;
// 	strPath.Format(_T("%s\\UnitStatus.ini"), theConfigBank.m_System.m_strConfigPath);
// 	CString strFolderPath = CFileSupport::GetPathName(strPath);
// 	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
// 	{
// 		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
// 			return;
// 	}
// 
// 	CIni ini(strPath);
// 	CString strKey;
// 	for(int jig = 0; jig < JIG_ID_MAX; jig++)
// 	{
// 		for(int ch = 0; ch < JIG_CH_MAX; ch++)
// 		{
// 			strKey.Format(_T("%cJIG_CH%d"), jig+_T('A'), ch+1);
// 			ini.WriteString(_T("STATUS")	,strKey,  m_Unit[jig][ch].ToString() );
// 		}
// 	}
// }
// 
// void CUnitStatusBank::ReadStatus()
// {
// 	CString strPath,strBackupPath;;
// 	strPath.Format(_T("%s\\UnitStatus.ini"), theConfigBank.m_System.m_strConfigPath);
// 	CString strFolderPath = CFileSupport::GetPathName(strPath);
// 	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
// 	{
// 		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
// 			return;
// 	}
// 
// 	CIni ini(strPath);
// 	CString strKey, strValue;
// 	for(int jig = 0; jig < JIG_ID_MAX; jig++)
// 	{
// 		for(int ch = 0; ch < JIG_CH_MAX; ch++)
// 		{
// 			strKey.Format(_T("%cJIG_CH%d"), jig+_T('A'), ch+1);
// 			strValue = ini.GetString(_T("STATUS")	,strKey);
// 			if(strValue.IsEmpty() == FALSE)
// 				m_Unit[jig][ch].FromString(strValue);
// 		}
// 	}
// }
