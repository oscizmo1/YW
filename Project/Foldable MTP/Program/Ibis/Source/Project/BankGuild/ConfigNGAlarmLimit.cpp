#include "StdAfx.h"
#include "ConfigNGAlarmLimit.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigNGAlarmLimit::CConfigNGAlarmLimit(void)
{
	InitList();
}


CConfigNGAlarmLimit::~CConfigNGAlarmLimit(void)
{
}

void CConfigNGAlarmLimit::InitList()
{
	m_listAlarm.RemoveAll();
}

void CConfigNGAlarmLimit::AddList(NGaAlarmLimitItem item)
{
	m_listAlarm.AddTail(item);
}

void CConfigNGAlarmLimit::ReadNGAlarmFile(CString strPath)
{
	InitList();

	CIni ini(strPath);
	NGaAlarmLimitItem item;
	int nIndex = 0;
	CString strSection;

	do
	{
		strSection.Format(_T("ITEM%03d"), nIndex+1);
		ini.GetString(strSection		, _T("NAME")	, item.strMatchingName);
		ini.GetBoolean(strSection	, _T("USE")		, item.bUse);
		ini.GetLong(strSection		, _T("COUNT")	, item.nCount);
		ini.GetDouble(strSection		, _T("RATE")	, item.dRate);

		if(item.strMatchingName.IsEmpty() == FALSE)
			m_listAlarm.AddTail(item);

	}while(item.strMatchingName.IsEmpty() == FALSE);
}

void CConfigNGAlarmLimit::SaveNGAlarmLimitFile(CString strPath, CString strBackupPath)
{
	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

	NGaAlarmLimitItem item;
	int nIndex = 0;
	CString strSection;
	POSITION pos = m_listAlarm.GetHeadPosition();
	while(pos)
	{
		item = m_listAlarm.GetNext(pos);
		strSection.Format(_T("ITEM%03d"), nIndex+1);

		ini.WriteString(strSection		, _T("NAME")	, item.strMatchingName);
		ini.WriteBoolean(strSection	, _T("USE")		, item.bUse);
		ini.WriteLong(strSection		, _T("COUNT")	, item.nCount);
		ini.WriteDouble(strSection		, _T("RATE")	, item.dRate);
	}
}
