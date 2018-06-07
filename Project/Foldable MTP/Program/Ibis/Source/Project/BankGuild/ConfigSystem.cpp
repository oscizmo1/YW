#include "StdAfx.h"
#include "ConfigSystem.h"
#include "Etc/Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigSystem::CConfigSystem(void)
{
}


CConfigSystem::~CConfigSystem(void)
{
}

void CConfigSystem::ReadSystemFile( CString strPath )
{
	CIni ini(strPath);

	// PATH
	//kjpark 20170901 LOG 경로 수정
	m_strDebugLogPath = ini.GetString(_T("PATH"), _T("m_strDebugLogPath"), _T("D:\\Log\\Ibis"));
	m_strRecipePath = ini.GetString(_T("PATH"), _T("m_strRecipePath"), _T("D:\\YWDSP\\Recipe"));
	m_strRecipeBackupPath = ini.GetString(_T("PATH"), _T("m_strRecipeBackupPath"), _T("D:\\Backup\\Recipe"));
	m_strConfigPath = ini.GetString(_T("PATH"), _T("m_strConfigPath"), _T("D:\\YWDSP\\Config"));
	m_strConfigBackupPath = ini.GetString(_T("PATH"), _T("m_strConfigBackupPath"), _T("D:\\Backup\\Config"));
	m_strCellLogPath = ini.GetString(_T("PATH"), _T("m_strCellLogPath"), _T("D:\\auto_test\\Log"));
	m_strLosstimeLogPath = ini.GetString(_T("PATH"), _T("m_strLosstimeLogPath"), _T("D:\\SIRIUS\\LOG\\main"));
	//kjpark 20161026 MCR 리드 이미지 경로 변경
	m_strMCRImageSavePath = ini.GetString(_T("PATH"), _T("m_strMCRImageSavePath"), _T("D:\\Log\\MCRImage"));
	m_strFDCPath = ini.GetString(_T("PATH"), _T("m_strFDCPath"), _T("R:\\FDC"));
	m_strProgramDataPath = ini.GetString(_T("PATH"), _T("m_strProgramDataPath"), _T("D:\\Program\\Ibis\\ProgramData"));
	m_strMachineDataPath = ini.GetString(_T("PATH"), _T("m_strMachineDataPath"), _T("D:\\YWDSP\\MachineData"));
	m_strPGLogPath = ini.GetString(_T("PATH"), _T("m_strPGLogPath"), _T("\\\\192.168.10.140\\Result"));

	// SYSTEM_PARAMETER
	m_strLineNo = ini.GetString(_T("SYSTEM_PARAMETER"), _T("m_nLineNo"), _T("01A"));
	//kjaprk 20170904 통계 UI 12시간 타임으로 변경
	m_bWorkPartTime = ini.GetBoolean(_T("SYSTEM_PARAMETER"), _T("m_bWorkPartTime"), TRUE);
	//kjpark 20170828 MoveWindow 적용
	m_dDefaultWindow_X = ini.GetDouble(_T("SYSTEM_PARAMETER"), _T("m_dDefaultWindow_X"), 0);
	m_dDoorOpenWindow_X = ini.GetDouble(_T("SYSTEM_PARAMETER"), _T("m_dDoorOpenWindow_X"), 1280);	

	// 물류 연동 [10/5/2017 OSC]
	m_bInlineMode = ini.GetBoolean(_T("SYSTEM_PARAMETER"), _T("m_bInlineMode"), FALSE);
	m_MachineIndex = ini.GetLong(_T("SYSTEM_PARAMETER"), _T("m_MachineIndex"), MACHINE_1);

	// 물류 연동시 CIM Qual 모드 [12/15/2017 OSC]
	m_bCIMQualMode = ini.GetBoolean(_T("SYSTEM_PARAMETER"), _T("m_bCIMQualMode"), FALSE);

	// SYSTEM OFFSET
}
