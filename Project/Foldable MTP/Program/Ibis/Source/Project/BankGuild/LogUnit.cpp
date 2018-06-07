#include "StdAfx.h"
#include "LogUnit.h"
#include "ETC/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLogUnit::CLogUnit(void)
{
	m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
	m_WriteType = WriteType_Buffer;
	m_pBuf = NULL;
	m_nWriteStart = 0;
	m_strBuf = _T("");
	m_nBufferSize = 4096;
	SetBufferSize(m_nBufferSize);
	m_strFolderPath = _T("");
	m_strFileName = _T("");
	m_bAddTime = TRUE;
	m_bAddDay = TRUE;
	m_bPartition = FALSE;
	m_nOffsetHour = 0;
	m_nOffsetMin = 0;
}

CLogUnit::CLogUnit( WRITE_TYPE logType, CString strFileName, CString strPath, int nOffsetHour, int nOffsetMin, 
	BOOL bAddDay /*= FALSE*/, BOOL bAddTime /*= TRUE*/, BOOL bPartition /*= FALSE*/ )
{
	m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
	m_WriteType = logType;
	m_pBuf = NULL;
	m_nWriteStart = 0;
	m_strBuf = _T("");
	m_nBufferSize = 4096;
	SetBufferSize(m_nBufferSize);
	m_strFolderPath = strPath;
	m_strFileName = strFileName;
	m_bAddTime = bAddTime;
	m_bAddDay = bAddDay;
	m_bPartition = bPartition;
	m_nOffsetHour = nOffsetHour;
	m_nOffsetMin = nOffsetMin;
}

CLogUnit::~CLogUnit(void)
{
	// ����� ���ۿ� Data�� ���������� ��� �� ����
	WriteBuf();
	::CloseHandle(m_hMutex);
}

void CLogUnit::SetType( WRITE_TYPE type )
{
	m_WriteType = type;
}

void CLogUnit::SetFolderPath( CString strFolderPath )
{
	m_strFolderPath = strFolderPath;
}

void CLogUnit::SetSubFolderName( CString strFolderName )
{
	m_strSubFolderName = strFolderName;
}

void CLogUnit::SetFileName( CString szFileName )
{
	m_strFileName = szFileName;
}

void CLogUnit::SetBufferSize( UINT nBufferSize )
{
	// new�� delete�� �����ϸ� �������� CString�� ����ؼ� �˾Ƽ� �����ϰ� �����.
	m_nBufferSize = nBufferSize;
	m_strBuf = _T("");
	m_strBuf.ReleaseBuffer();
	m_pBuf = m_strBuf.GetBuffer(nBufferSize);
	m_nWriteStart = 0;
}

void CLogUnit::SetOffsetHour( int nOffsetHour )
{
	m_nOffsetHour = nOffsetHour;
}

void CLogUnit::SetOffsetMin( int nOffsetMin )
{
	m_nOffsetMin = nOffsetMin;
}
void CLogUnit::SetAddTime( BOOL bAdd )
{
	m_bAddTime = bAdd;
}

void CLogUnit::SetAddDay( BOOL bAdd )
{
	m_bAddDay = bAdd;
}

void CLogUnit::SetPartition( BOOL bUse )
{
	m_bPartition = bUse;
}

void CLogUnit::AddBuf( LPCTSTR fmt, ... )
{
	if( WaitForSingleObject(m_hMutex, 10000) != WAIT_OBJECT_0)
	{
		TRACE(_T("AddBuf Mutex Error"));
		return;
	}

	CString strLogText;
	va_list argList;
	va_start( argList, fmt );
	strLogText.FormatV( fmt, argList );
	va_end( argList );

	if(strLogText.GetLength() < 1)
	{
		::ReleaseMutex(m_hMutex);
		return;
	}
	CString strLogHeader = _T("");

	if(m_bAddTime)
	{
		if (m_bAddDay)
		{
			SYSTEMTIME	time;
			GetLocalTime(&time);
			strLogHeader.Format(_T("%04u-%02u-%02u\t%02u:%02u:%02u:%03u\t%s\n"),time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strLogText);
			strLogText = strLogHeader;
		}
		else
		{
			SYSTEMTIME	time;
			GetLocalTime(&time);
			strLogHeader.Format(_T("%02u:%02u:%02u:%03u\t%s\n"), time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strLogText);
			strLogText = strLogHeader;
		}

	}
	else
	{
		strLogHeader.Format(_T("%s\n"), strLogText);
		strLogText = strLogHeader;
	}

	// �ٷ� ���� �Ÿ� ���⼭ �ٷ� ���´�.
	if(m_WriteType == WriteType_NoWait)
	{
		Write(strLogText);
	}
	else
	{
		int nLength = strLogText.GetLength();
		ASSERT(m_nBufferSize >= nLength);

		// Buffer�� �� á�� ��� WriteType�� ������� ������ ���� Buffer�� ����.
		if(m_nWriteStart + nLength > m_nBufferSize)
		{
			WriteBuf(TRUE);
		}

		memcpy(m_pBuf+m_nWriteStart, strLogText.GetString() , nLength+1);
// 		_tcscpy_s(m_pBuf+m_nWriteStart, nLength, _szLogText);
		m_nWriteStart += nLength;
	}
//  #ifdef _DEBUG
//  	TRACE(strLogText);
//  #endif

	::ReleaseMutex(m_hMutex);
}

void CLogUnit::WriteBuf(BOOL bOverFlow /*= FALSE*/)
{
	if(_tcslen(m_pBuf) < 1 || _tcslen(m_strFolderPath) < 1 || _tcslen(m_strFileName) < 1)
		return SetBufferSize(m_nBufferSize);

	// Directory Check
	SYSTEMTIME time;
	CString strFolderPath;
	if(GetFullPath(strFolderPath, time) == FALSE)
		return;

	// ���� �� ���� ����
	m_strBuf.ReleaseBuffer();

	// File�� ���
	FILE *stream;
	CString strFilePath ;
	if(m_bPartition)
	{
		int nExtIndex = m_strFileName.ReverseFind('.');
		strFilePath.Format(_T("%s\\%s_%02u%s"), strFolderPath, m_strFileName.Left(nExtIndex), time.wHour, m_strFileName.Mid(nExtIndex));
	}
	else
		strFilePath.Format(_T("%s\\%s"), strFolderPath, m_strFileName);

#ifdef _UNICODE
	// ó�� ���� �� �����ڵ�� ���� ������ ���� ���� ��� ������.
	// �׸��� �����ڵ� ���� �����Ϸ��� w�ιۿ� ���� ���Ѵ� -_-; [4/9/2013 OSC]
	if( CFileSupport::FileCheck(strFilePath) )
	{
		if(_tfopen_s( &stream, strFilePath, _T("a+t, ccs=UTF-8") ) != 0 )
		{
			TRACE(_T("Log File open Fail! (%s)"), strFilePath);
		}
	}
	else
	{
		if(_tfopen_s( &stream, strFilePath, _T("w, ccs=UTF-8") ) != 0 )
		{
			TRACE(_T("Log File open Fail! (%s)"), strFilePath);
		}
	}
#else
	if(_tfopen_s( &stream, strFilePath, _T("a+t") ) != 0 )
	{
		TRACE(_T("Log File open Fail! (%s)"), strFilePath);
	}
#endif

	// WriteType_Buffer�� �ƴ� ���� Buffer �뷮 �ʰ��� ���� ���� ��� ���Ͽ� Error ���
	if(m_WriteType != WriteType_Buffer && bOverFlow == TRUE)
	{
		m_strBuf = _T("\nBuffer Overflow!\n");
		_ftprintf(stream, _T("%s"), m_strBuf);
	}
	fclose(stream);


	// ���� �Ҵ�
	SetBufferSize(m_nBufferSize);
}

void CLogUnit::Write(LPCTSTR szLogText)
{
	if(m_WriteType != WriteType_NoWait)
		return;
	//if(_tcslen((LPCSTR)szLogText) < 1 || _tcslen(m_strFolderPath) < 1 || _tcslen(m_strFileName) < 1)
	if(_tcslen(szLogText) < 1 || _tcslen(m_strFolderPath) < 1 || _tcslen(m_strFileName) < 1)
		return;

	// Directory Check
	SYSTEMTIME time;
	CString strFolderPath;
	if(GetFullPath(strFolderPath, time) == FALSE)
		return;

	// File�� ���
	FILE *stream;
	CString strFilePath ;
	if(m_bPartition)
	{
		int nExtIndex = m_strFileName.ReverseFind('.');
		strFilePath.Format(_T("%s\\%s_%02u%s"), strFolderPath, m_strFileName.Left(nExtIndex), time.wHour, m_strFileName.Mid(nExtIndex));
	}
	else
		strFilePath.Format(_T("%s\\%s"), strFolderPath, m_strFileName);

// 	if(_tfopen_s( &stream, strFilePath, _T("a+t") ) == 0 )
// 	{
// 		_ftprintf(stream, _T("%s"), szLogText);
// 		TRACE(_T("%d. %s\n"), stream, szLogText);
// 		fclose(stream);
// 	}
#ifdef _UNICODE
	// ó�� ���� �� �����ڵ�� ���� ������ ���� ���� ��� ������.
	// �׸��� �����ڵ� ���� �����Ϸ��� w�ιۿ� ���� ���Ѵ� -_-; [4/9/2013 OSC]
	if( CFileSupport::FileCheck(strFilePath) )
	{
		if(_tfopen_s( &stream, strFilePath, _T("a+t, ccs=UTF-8") ) != 0 )
		{
			TRACE(_T("Log File open Fail! (%s)"), strFilePath);
			return;
		}
	}
	else
	{
		if(_tfopen_s( &stream, strFilePath, _T("w, ccs=UTF-8") ) != 0 )
		{
			TRACE(_T("Log File open Fail! (%s)"), strFilePath);
			return;
		}
	}
#else
	if(_tfopen_s( &stream, strFilePath, _T("a+t") ) != 0 )
	{
		TRACE(_T("Log File open Fail! (%s)"), strFilePath);
		return;
	}
#endif

	_ftprintf(stream, _T("%s"), szLogText);
// 	TRACE(_T("%d. %s\n"), stream, szLogText);
	fclose(stream);
}

BOOL CLogUnit::GetFullPath( CString &strPath, SYSTEMTIME &time )
{
	GetLocalTime(&time);

	if (time.wHour == m_nOffsetHour)
	{
		if(time.wMinute < m_nOffsetMin)
		{
			CTimeSpan timeOffset(1, 0, 0, 0);
			CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

			time.wYear=tmYesterday.GetYear(); 
			time.wMonth=tmYesterday.GetMonth(); 
			time.wDay=tmYesterday.GetDay(); 
		}
	}
	else if(time.wHour < m_nOffsetHour)
	{
		CTimeSpan timeOffset(1, 0, 0, 0);
		CTime tmYesterday = CTime::GetCurrentTime() - timeOffset; 

		time.wYear=tmYesterday.GetYear(); 
		time.wMonth=tmYesterday.GetMonth(); 
		time.wDay=tmYesterday.GetDay(); 
	}

	CString strFolderPath;
	if(m_strSubFolderName.GetLength() > 0)
		strFolderPath.Format(_T("%s\\%04u-%02u-%02u\\%s"), m_strFolderPath, time.wYear, time.wMonth, time.wDay, m_strSubFolderName);
	else
		strFolderPath.Format(_T("%s\\%04u-%02u-%02u"), m_strFolderPath, time.wYear, time.wMonth, time.wDay);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return FALSE;
	}
	strPath = strFolderPath;
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
