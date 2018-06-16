#include "StdAfx.h"
#include "LogCtrl.h"
#include "../GaonNuri.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

// MakeSureDirectoryPathExists()
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

CLogUnit::CLogUnit(void)
{
	m_nWriteType = LogType_Buffer;
	m_pBuf = NULL;
	m_nWriteStart = 0;
	m_strBuf = _T("");
	m_nBufferSize = 4096;
	SetBufferSize(m_nBufferSize);
	m_strFolderPath = _T("");
	m_strFileName = _T("");
	m_bAddTime = TRUE;
}

CLogUnit::~CLogUnit(void)
{
	// 종료시 버퍼에 Data가 남아있으면 기록 후 종료
	WriteBuf();
}

void CLogUnit::SetType( int nType )
{
	m_nWriteType = nType;
}

void CLogUnit::SetFolderPath( CString strFolderPath )
{
	m_strFolderPath = strFolderPath;
}

void CLogUnit::SetFileName( CString szFileName )
{
	m_strFileName = szFileName;
}

void CLogUnit::SetBufferSize( UINT nBufferSize )
{
	// new와 delete로 관리하면 귀찮으니 CString을 사용해서 알아서 관리하게 만든다.
	m_nBufferSize = nBufferSize;
	m_strBuf = _T("");
	m_strBuf.ReleaseBuffer();
	m_pBuf = m_strBuf.GetBuffer(nBufferSize);
	m_nWriteStart = 0;
}

void CLogUnit::SetAddTime( BOOL bAdd )
{
	m_bAddTime = bAdd;
}

void CLogUnit::AddBuf( int nLogID,  LPCTSTR szLogText )
{
	CString strViewLog;

	if(strlen(szLogText) < 1)
		return;
	CString strLogText = _T("");

	if(m_bAddTime)
	{
		SYSTEMTIME	time;
		GetLocalTime(&time);
		strLogText.Format("%02u:%02u:%02u:%03u\t%s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szLogText);
		strViewLog.Format(_T("%02u:%02u:%02u:%03u-%s"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szLogText);
		szLogText = strLogText.GetString();
	}
	else
	{
		strLogText.Format("%s\n", szLogText);
		szLogText = strLogText.GetString();
	}

	// 바로 적는 거면 여기서 바로 적는다.
	if(m_nWriteType == LogType_NoWait)
	{
		Write(szLogText);
	
		if (nLogID==LOG_COMM)
		{
			((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->ViewCOMWriteDip(strViewLog);
		}
		else if (nLogID==LOG_CIM)	//VIEW_CIM
		{
			((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->ViewCIMWriteDip(strViewLog);
		}
	}
	else
	{
		int nLength = strlen(szLogText);
		ASSERT(m_nBufferSize >= nLength);

		// Buffer가 꽉 찼을 경우 WriteType에 관계없이 무조건 적고 Buffer를 비운다.
		if(m_nWriteStart + nLength > m_nBufferSize)
		{
			WriteBuf(TRUE);
		}

		memcpy(m_pBuf+m_nWriteStart, szLogText , nLength+1);
		// 		_tcscpy_s(m_pBuf+m_nWriteStart, nLength, _szLogText);
		m_nWriteStart += nLength;
	}
}

void CLogUnit::WriteBuf(BOOL bOverFlow /*= FALSE*/)
{
	if(strlen(m_pBuf) < 1 || strlen(m_strFolderPath) < 1 || strlen(m_strFileName) < 1)
		return SetBufferSize(m_nBufferSize);

	// Directory Check
	SYSTEMTIME	time;
	GetLocalTime(&time);

	if (time.wHour == GetApp()->GetLogOffsetHour())
	{
		if(time.wMinute < GetApp()->GetLogOffsetMin())
		{
			CTimeSpan timeOffset(1, 0, 0, 0);
			CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

			time.wYear=tmYesterday.GetYear(); 
			time.wMonth=tmYesterday.GetMonth(); 
			time.wDay=tmYesterday.GetDay(); 
		}
	}
	else if(time.wHour < GetApp()->GetLogOffsetHour())
	{
		CTimeSpan timeOffset(1, 0, 0, 0);
		CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

		time.wYear=tmYesterday.GetYear(); 
		time.wMonth=tmYesterday.GetMonth(); 
		time.wDay=tmYesterday.GetDay(); 
	}

	CString strFolderPath;
	strFolderPath.Format("%s\\%04u-%02u-%02u\\", m_strFolderPath, time.wYear, time.wMonth, time.wDay);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		// ASSERT(MakeSureDirectoryPathExists(strFolderPath));
		MakeSureDirectoryPathExists(strFolderPath);
	}

	// 끝에 빈 공간 제거
	m_strBuf.ReleaseBuffer();

	// File에 기록
	FILE *stream;
	CString strFilePath ;
	strFilePath.Format(_T("%s%s"), strFolderPath, m_strFileName);
	if(fopen_s( &stream, strFilePath, "a+t" ) == 0 )
	{
		fprintf(stream, "%s", m_strBuf);

		// WriteType_Buffer가 아닌 데도 Buffer 용량 초과로 인해 적인 경우 파일에 Error 기록
		if(m_nWriteType != LogType_Buffer && bOverFlow == TRUE)
		{
			m_strBuf = _T("\nBuffer Overflow!\n");
			fprintf(stream, "%s", m_strBuf);
		}
		fclose(stream);
	}
	else
	{
		TRACE("Log File open Fail! (%s)", strFilePath);
	}

	// 새로 할당
	SetBufferSize(m_nBufferSize);
}

void CLogUnit::WriteDaily()
{
	if(m_nWriteType == LogType_Daily)
		WriteBuf();
}

void CLogUnit::WriteTime()
{
	if(m_nWriteType == LogType_Time)
		WriteBuf();
}

void CLogUnit::WriteLotEnd()
{
	if(m_nWriteType == LogType_LotEnd)
		WriteBuf();
}
void CLogUnit::WriteCstEnd()
{
	if(m_nWriteType == LogType_CstEnd)
		WriteBuf();
}

void CLogUnit::Write(LPCTSTR szLogText)
{
	if(m_nWriteType != LogType_NoWait)
		return;
	if(strlen(szLogText) < 1 || strlen(m_strFolderPath) < 1 || strlen(m_strFileName) < 1)
		return;

	// Directory Check
	// 	CTime time = CTime::GetCurrentTime();
	// 	CString strFolderPath = m_szFolderPath;
	// 	strFolderPath += time.Format("\\%Y-%m-%d\\");

	SYSTEMTIME	time;
	GetLocalTime(&time);

	if (time.wHour == GetApp()->GetLogOffsetHour())
	{
		if(time.wMinute < GetApp()->GetLogOffsetMin())
		{
			CTimeSpan timeOffset(1, 0, 0, 0);
			CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

			time.wYear=tmYesterday.GetYear(); 
			time.wMonth=tmYesterday.GetMonth(); 
			time.wDay=tmYesterday.GetDay(); 
		}
	}
	else if(time.wHour < GetApp()->GetLogOffsetHour())
	{
		CTimeSpan timeOffset(1, 0, 0, 0);
		CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

		time.wYear=tmYesterday.GetYear(); 
		time.wMonth=tmYesterday.GetMonth(); 
		time.wDay=tmYesterday.GetDay(); 
	}

	CString strFolderPath;
	strFolderPath.Format("%s\\%04u-%02u-%02u\\", m_strFolderPath, time.wYear, time.wMonth, time.wDay);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		// ASSERT(MakeSureDirectoryPathExists(strFolderPath));
		MakeSureDirectoryPathExists(strFolderPath);
	}

	// File에 기록
	FILE *stream;
	CString strFilePath ;
	strFilePath.Format(_T("%s%s"), strFolderPath, m_strFileName);
	if(fopen_s( &stream, strFilePath, "a+t" ) == 0 )
	{
		fprintf(stream, "%s", szLogText);
		fclose(stream);
	}
	else
	{
		TRACE("Log File open Fail! (%s)", strFilePath);
	}
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CLogCtrl

//////////////////////////////////////////////////////////////////////////
// Singleton
CLogCtrl* CLogCtrl::_instance=NULL;
CLogCtrl* CLogCtrl::Instance()
{
	if (_instance == NULL) {
		_instance = new CLogCtrl;
	}
	return _instance;
}

void CLogCtrl::Release()
{
	if (_instance != NULL) {
		delete _instance;
	}
}
//////////////////////////////////////////////////////////////////////////
CLogCtrl::CLogCtrl( void )
{
	Initialize();
}

CLogCtrl::~CLogCtrl(void)
{
}

void CLogCtrl::AddBuf( int nLogID, LPCTSTR fmt, ... )
{
	char buf[4096];
	va_list vlist;
	va_start(vlist, fmt);
	vsprintf_s(buf, fmt, vlist);
	va_end(vlist);
	//m_Log[nLogID].AddBuf(buf);
	m_Log[nLogID].AddBuf(nLogID, buf);
}

void CLogCtrl::WriteDaily()
{
	for(int i = 0; i < LOG_MAX; i++)
		m_Log[i].WriteDaily();
}
void CLogCtrl::WriteTime()
{
	for(int i = 0; i < LOG_MAX; i++)
		m_Log[i].WriteTime();
}
void CLogCtrl::WriteLotEnd()
{
	for(int i = 0; i < LOG_MAX; i++)
		m_Log[i].WriteLotEnd();
}
void CLogCtrl::WriteCstEnd()
{
	for(int i = 0; i < LOG_MAX; i++)
		m_Log[i].WriteCstEnd();
}

void CLogCtrl::WriteNowAll()
{
	for(int i = 0; i < LOG_MAX; i++)
		m_Log[i].WriteBuf();
}

void CLogCtrl::WriteNowAt( int nLogID )
{
	m_Log[nLogID].WriteBuf();
}

void CLogCtrl::Initialize()
{
	m_Log[LOG_SYSTEM].SetType(LogType_NoWait);
	m_Log[LOG_SYSTEM].SetFileName(_T("System.log"));
	m_Log[LOG_SYSTEM].SetFolderPath(GetApp()->GetUserLogPath());

	m_Log[LOG_SEQUENCE].SetType(LogType_NoWait);
	m_Log[LOG_SEQUENCE].SetFileName(_T("Sequence.log"));
	m_Log[LOG_SEQUENCE].SetFolderPath(GetApp()->GetUserLogPath());

	m_Log[LOG_OPERATION].SetType(LogType_NoWait);
	m_Log[LOG_OPERATION].SetFileName(_T("Operation.log"));
	m_Log[LOG_OPERATION].SetFolderPath(GetApp()->GetUserLogPath());

	m_Log[LOG_LOSSTIME].SetType(LogType_NoWait);
	m_Log[LOG_LOSSTIME].SetFileName(_T("Loss.log"));
	m_Log[LOG_LOSSTIME].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_LOSSTIME].SetAddTime(FALSE);

	m_Log[LOG_ALIGN].SetType(LogType_NoWait);
	m_Log[LOG_ALIGN].SetFileName(_T("Align.log"));
	m_Log[LOG_ALIGN].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_ALIGN].SetAddTime(TRUE);

	m_Log[LOG_STATUS].SetType(LogType_NoWait);
	m_Log[LOG_STATUS].SetFileName(_T("Status.log"));
	m_Log[LOG_STATUS].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_STATUS].SetAddTime(TRUE);

	m_Log[LOG_COMM].SetType(LogType_NoWait);
	m_Log[LOG_COMM].SetFileName(_T("Comm.log"));
	m_Log[LOG_COMM].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_COMM].SetAddTime(TRUE);

	m_Log[LOG_ERR].SetType(LogType_NoWait);
	m_Log[LOG_ERR].SetFileName(_T("Error.log"));
	m_Log[LOG_ERR].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_ERR].SetAddTime(TRUE);

	m_Log[LOG_ERROR_TYPE].SetType(LogType_NoWait);
	m_Log[LOG_ERROR_TYPE].SetFileName(_T("ErrorType.log"));
	m_Log[LOG_ERROR_TYPE].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_ERROR_TYPE].SetAddTime(TRUE);

	m_Log[LOG_MCR].SetType(LogType_NoWait);
	m_Log[LOG_MCR].SetFileName(_T("MCR.log"));
	m_Log[LOG_MCR].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_MCR].SetAddTime(TRUE);

	m_Log[LOG_MAINTACT].SetType(LogType_NoWait);
	m_Log[LOG_MAINTACT].SetFileName(_T("Main Tact.log"));
	m_Log[LOG_MAINTACT].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_MAINTACT].SetAddTime(TRUE);

	m_Log[LOG_TACTTIME].SetType(LogType_Buffer);
	m_Log[LOG_TACTTIME].SetFileName(_T("Tact Time.log"));
	m_Log[LOG_TACTTIME].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_TACTTIME].SetAddTime(TRUE);

	m_Log[LOG_INSPECT].SetType(LogType_Buffer);
	m_Log[LOG_INSPECT].SetFileName(_T("Inspect.log"));
	m_Log[LOG_INSPECT].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_INSPECT].SetAddTime(TRUE);

	m_Log[LOG_CELL].SetType(LogType_NoWait);
	m_Log[LOG_CELL].SetFileName(_T("Cell.log"));
	m_Log[LOG_CELL].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_CELL].SetAddTime(TRUE);

	m_Log[LOG_CIM].SetType(LogType_NoWait);//LogType_Buffer);
	m_Log[LOG_CIM].SetFileName(_T("Cim.log"));
	m_Log[LOG_CIM].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_CIM].SetAddTime(TRUE);

	m_Log[LOG_TRACKING].SetType(LogType_NoWait);						// Track In/Out 상위 보고용
	m_Log[LOG_TRACKING].SetFileName(_T("Tracking.log"));
	m_Log[LOG_TRACKING].SetFolderPath(GetApp()->GetUserLogPath());
	m_Log[LOG_TRACKING].SetAddTime(TRUE);

}
