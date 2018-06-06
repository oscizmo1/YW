#include "StdAfx.h"
#include "FTPHandler.h"


CFTPHandler::CFTPHandler(void)
{
	m_hOpen = NULL;
	m_hConnect = NULL;
}


CFTPHandler::~CFTPHandler(void)
{
	CloseFtp();
}

BOOL CFTPHandler::OpenFtp(LPCTSTR lpAddr, LPCTSTR lpID, LPCTSTR lpPW)
{
	if(m_hOpen!=NULL || m_hConnect!=NULL)	
		CloseFtp();

	m_hOpen = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	if(NULL == m_hOpen) 
	{
		return FALSE;
	}
	m_hConnect = InternetConnect(m_hOpen, 
		lpAddr,
		INTERNET_DEFAULT_FTP_PORT, 
		lpID,
		lpPW,
		INTERNET_SERVICE_FTP, 
		0, 0);
	if(NULL == m_hConnect)
	{
		InternetCloseHandle(m_hOpen);
		return FALSE;
	}
	return TRUE;
}
void CFTPHandler::CloseFtp()
{
	if(m_hOpen)
		InternetCloseHandle(m_hOpen);
	if(m_hConnect)
		InternetCloseHandle(m_hConnect);
}
BOOL CFTPHandler::SetCurrentFtpDir(LPCTSTR lpDirName)
{
	if(m_hOpen==NULL)	return FALSE;
	if(m_hConnect==NULL)	return FALSE;

	//MakeFtpDir(lpDirName);

	if(!FtpSetCurrentDirectory(m_hConnect, lpDirName ))
	{
		if(!MakeFtpDir(lpDirName))
		{
			TRACE(_T("FtpPutFile error\n"));
			return FALSE;
		}
		if(!FtpSetCurrentDirectory(m_hConnect, lpDirName ))
			return FALSE;
	}
	return TRUE;
}

BOOL CFTPHandler::GetFtpFile(LPCTSTR lpFtpFileName, LPCTSTR lpLocalFileName)
{
	// 사용 안해봤음.. 검증필요 [12/20/2017 OSC]
	if(m_hOpen==NULL)	return FALSE;
	if(m_hConnect==NULL)	return FALSE;

	if(!FtpGetFile(m_hConnect, lpFtpFileName, lpLocalFileName, TRUE, NULL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CFTPHandler::PutFtpFile(LPCTSTR lpFtpFileName, LPCTSTR lpLocalFileName)
{
	if(m_hOpen==NULL)	return FALSE;
	if(m_hConnect==NULL)	return FALSE;

	if(!FtpPutFile(m_hConnect, lpLocalFileName, lpFtpFileName, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CFTPHandler::MakeFtpDir(LPCTSTR lpDirName, BOOL bIncludeFileName)
{
	TCHAR	cDir[1024];
	TCHAR*	pOrg = (TCHAR*)lpDirName;
	TCHAR*	p0, *pS;


	if(m_hOpen==NULL)	return FALSE;
	if(m_hConnect==NULL)	return FALSE;

	if(_tcslen(pOrg)<=0)
		return FALSE;

	_tcscpy_s(cDir, 1024, pOrg);
	pS = cDir;

	if(pS[0]==_T('/') || pS[0]==_T('\\'))
		pS++;

	while(_tcslen(pS)>0)
	{
		if((p0=_tcschr(pS, _T('/')))==NULL)
			if((p0=_tcschr(pS, _T('\\')))==NULL)
				break;

		*p0=0x00;
		FtpCreateDirectory(m_hConnect, cDir);
		*p0=_T('/');
		pS = p0+1;
	}

	if(!bIncludeFileName)
	{
		FtpCreateDirectory(m_hConnect, cDir);
	}
	return TRUE;
}
CString CFTPHandler::GetLastFtpRespons()
{
	CString strVal=_T("");
	DWORD dwError,dwBufferLength; 
	TCHAR cRsp[1024];
	if(!InternetGetLastResponseInfo(&dwError,cRsp, &dwBufferLength))
		return strVal;

	strVal.Format(_T("%s"), cRsp);
	return strVal;
}
