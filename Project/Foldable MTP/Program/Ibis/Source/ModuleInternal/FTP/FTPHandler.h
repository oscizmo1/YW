#pragma once
#include <Wininet.h>

class CFTPHandler
{
public:
	CFTPHandler(void);
	~CFTPHandler(void);

protected:
	HINTERNET m_hOpen, m_hConnect;

public:
	BOOL OpenFtp(LPCTSTR lpAddr, LPCTSTR lpID, LPCTSTR lpPW);
	void CloseFtp();
	BOOL SetCurrentFtpDir(LPCTSTR lpDirName);
	BOOL GetFtpFile(LPCTSTR lpFtpFileName, LPCTSTR lpLocalFileName);
	BOOL PutFtpFile(LPCTSTR lpFtpFileName, LPCTSTR lpLocalFileName);
	BOOL MakeFtpDir(LPCTSTR lpDir, BOOL bIncludeFileName=FALSE);
	CString GetLastFtpRespons();

};

