#include "StdAfx.h"
#include "ConfigTimeOut.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigTimeOut::CConfigTimeOut(void)
{		
}


CConfigTimeOut::~CConfigTimeOut(void)
{
}

void CConfigTimeOut::ReadTimeOutFile( CString strPath )
{
	CIni ini(strPath);
	m_dCellLoading		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dCellLoading")		, 5.);	
	m_dMTPWrite			= ini.GetDouble(_T("TIME_OUT")		, _T("m_dMTPWrite")			, 30.);	
	m_dMTPVerify		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dMTPVerify")		, 30.);	
	m_dCoprIctTest		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dCoprIctTest")		, 10.);	
	m_dDDIBlockTest		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dDDIBlockTest")		, 10.);	
	m_dEvtVersionCheck	= ini.GetDouble(_T("TIME_OUT")		, _T("m_dEvtVersionCheck")	, 10.);	
	m_dHlpmCurrent		= ini.GetDouble(_T("TIME_OUT")		, _T("m_HlpmCurrent")		, 10.);	
	m_dIctTest			= ini.GetDouble(_T("TIME_OUT")		, _T("m_IctTest")			, 10.);	
	m_dIdCheck			= ini.GetDouble(_T("TIME_OUT")		, _T("m_dIdCheck")			, 10.);	
	m_dOptionCheck10	= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck10")	, 10.);	
	m_dOptionCheck2		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck2")		, 10.);	
	m_dOptionCheck3		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck3")		, 10.);	
	m_dOptionCheck4		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck4")		, 10.);	
	m_dOptionCheck5		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck5")		, 10.);	
	m_dOptionCheck6		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck6")		, 10.);	
	m_dOptionCheck7		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck7")		, 10.);	
	m_dOptionCheck8		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck8")		, 10.);	
	m_dOptionCheck9		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOptionCheck9")		, 10.);
	m_dOtpRegCheck		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dOtpRegCheck")		, 10.);
	m_dPocErrorCheck	= ini.GetDouble(_T("TIME_OUT")		, _T("m_dPocErrorCheck")	, 10.);
	m_dSleepCurrent		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dSleepCurrent")		, 10.);
	m_dTeCheck			= ini.GetDouble(_T("TIME_OUT")		, _T("m_dTeCheck")			, 10.);
	m_dTspStart			= ini.GetDouble(_T("TIME_OUT")		, _T("m_dTspStart")			, 10.);
	m_dWhiteCurrent		= ini.GetDouble(_T("TIME_OUT")		, _T("m_dWhiteCurrent")		, 10.);
}

void CConfigTimeOut::SaveTimeOutFile( CString strPath, CString strBackupPath )
{
	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dCellLoading")		, m_dCellLoading		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dMTPWrite")			, m_dMTPWrite			);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dMTPVerify")		, m_dMTPVerify			);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dCoprIctTest")		, m_dCoprIctTest		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dDDIBlockTest")		, m_dDDIBlockTest		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dEvtVersionCheck")	, m_dEvtVersionCheck	);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dHlpmCurrent")		, m_dHlpmCurrent		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dIctTest")			, m_dIctTest			);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dIdCheck")			, m_dIdCheck			);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck10")	, m_dOptionCheck10		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck2")		, m_dOptionCheck2		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck3")		, m_dOptionCheck3		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck4")		, m_dOptionCheck4		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck5")		, m_dOptionCheck5		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck6")		, m_dOptionCheck6		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck7")		, m_dOptionCheck7		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck8")		, m_dOptionCheck8		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOptionCheck9")		, m_dOptionCheck9		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dOtpRegCheck")		, m_dOtpRegCheck		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dPocErrorCheck")	, m_dPocErrorCheck		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dSleepCurrent")		, m_dSleepCurrent		);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dTeCheck")			, m_dTeCheck			);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dTspStart")			, m_dTspStart			);
	ini.WriteDouble(_T("TIME_OUT")	, _T("m_dWhiteCurrent")		, m_dWhiteCurrent		);
}
