
// GaonNuri.h : GaonNuri 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

//header file
#include "PublicClass/LogCtrl.h"
#include "Define.h"
#include "CimClass/DataStructure.h"
#include "CimClass/SettingParam.h"
#include "PublicClass/Ini.h"
//#include "PublicClass/BtnST.h"
#include "PublicClass/Token.h"
#include "PublicClass/FileSupport.h"
#include "PublicClass/ReportCtrl.h"
//#include "PublicClass/XML/XMLSettings.h"
//#include "PublicClass/XML/XmlArchive.h"
//#include "PublicClass/XML/XMLFile.h"
//#include "PublicClass/Graph/OScopeCtrl.h"
#include "PublicClass/Etc.h"
#include "Automation.h"
#include "MainFrm.h"
#include <math.h>


// CGaonNuriApp:
// 이 클래스의 구현에 대해서는 GaonNuri.cpp을 참조하십시오.
//

class CGaonNuriApp : public CWinApp
{
public:
	CGaonNuriApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:

	int GetLogOffsetHour() {return m_nLogOffsetHour; }
	void SetLogOffsetHour(int nValue) { m_nLogOffsetHour = nValue; }
	int GetLogOffsetMin() {return m_nLogOffsetMin; }
	void SetLogOffsetMin(int nValue) { m_nLogOffsetMin = nValue; }

	static const char *const GetApplicationPath(void);

	static inline CString GetSetupPath() 
	{
		CString setupPath;
		setupPath.Format("%s\\%s" , CGaonNuriApp::m_strMainPath, SETUP_PATH);
		::CreateDirectoryA(setupPath, NULL);
		return setupPath;
	}

	static inline CString GetUserLogPath() 
	{
		CString logPath;
		logPath.Format("%s" , USER_LOG_PATH);
		::CreateDirectoryA(logPath, NULL);

		return logPath;
	}
	static inline CString GetTrackOutInfoPath() 
	{
		CString logPath;
		logPath.Format("%s" , TRACKOUT_INFO_PATH);
		::CreateDirectoryA(logPath, NULL);

		return logPath;
	}
	BOOL m_bUseLossTime;
	LONG m_LossTime;
	int m_nLogOffsetHour;
	int m_nLogOffsetMin;

	static char	m_strAppPath[_MAX_PATH]; // Most rescent used PCB model name
	static char	m_strExePath[_MAX_PATH]; // Most rescent used PCB model name

	
public:
	BOOL Initialize(void);


	static CString m_strMainPath;
	void ReadSystemFile();
};

inline CGaonNuriApp *GetApp() {	return ((CGaonNuriApp*)AfxGetApp());	}
inline CLogCtrl* GetLogCtrl() {	return CLogCtrl::Instance();	}

extern CGaonNuriApp theApp;

