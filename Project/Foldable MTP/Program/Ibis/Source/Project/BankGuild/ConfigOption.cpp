#include "StdAfx.h"
#include "ConfigOption.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigOption::CConfigOption(void)
{		

}


CConfigOption::~CConfigOption(void)
{
}

void CConfigOption::ReadOptionFile( CString strPath )
{
	CIni ini(strPath);
	m_OffsetHour			= ini.GetLong(_T("OPTION")		, _T("m_OffsetHour")	, 7);	
	m_bUseLoofTest			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseLoofTest")	, FALSE);
	m_bUseByPass			= ini.GetBoolean(_T("OPTION")	, _T("m_bUseByPass")	, FALSE);
	m_dFDCCheckTime			= ini.GetDouble(_T("OPTION")	, _T("m_dFDCCheckTime")	, 10.);
	m_bFDCAlarm				= ini.GetBoolean(_T("OPTION")	, _T("m_bFDCAlarm")		, FALSE);
	
	//kjpark 20171010 TMD March 구현
	m_bUseTMDNameMatch		= ini.GetBoolean(_T("OPTION")		, _T("m_bUseTMDNameMatch")	, FALSE);

	//20170705 kjpark cell infomation request 사용 옵션
	//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
	m_bUseCellInforRequest	= ini.GetBoolean(_T("OPTION")		, _T("m_bUseCellInforRequest")			, TRUE);	
	//kjpark 20180113 Tmd Version 추가
	m_bUseHostVersionSend	= ini.GetBoolean(_T("OPTION")		, _T("m_bUseHostVersionSend")			, TRUE);	
	
	// CellLoading 실패시 리트라이
	m_bUseRetryAA			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseRetryAA")			, FALSE);
	// AB 리트라이
	m_bUseRetryAB			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseRetryAB")			, TRUE);
	m_bUseNgoption			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseNgoption")			, TRUE);

	//kjpark 20180119 김현규책임 요청 L 보고를 R로 옵션처리
	m_bUseNGChange_R			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseNGChange_R")			, TRUE);
	//kjpark 20180120 Cell Mixing 처리(강원호 선임)
	m_bUseCellMixingA3A4			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseCellMixingA3A4")			, FALSE);
	//kjpark 20180129 스크랩시에도 셀로그 적음
	m_bUseIf_NotUseWriteCellLog			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseIf_NotUseWriteCellLog")			, FALSE);
}

void CConfigOption::SaveOptionFile( CString strPath, CString strBackupPath )
{
	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	CIni ini(strPath);
	// 알람 영역에서 전날 로그 기록을 가져올 offset 시간값
	ini.WriteLong(_T("OPTION")		, _T("m_OffsetHour")			, m_OffsetHour			);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseLoofTest")			, m_bUseLoofTest		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseByPass")			, m_bUseByPass			);
	ini.WriteDouble(_T("OPTION")	, _T("m_dFDCCheckTime")			, m_dFDCCheckTime		);	
	ini.WriteBoolean(_T("OPTION")	, _T("m_bFDCAlarm")				, m_bFDCAlarm			);	
	
	//kjpark 20171010 TMD March 구현
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseTMDNameMatch")		, m_bUseTMDNameMatch	);	
	//20170705 kjpark cell Jobstart 사용 옵션
	//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCellInforRequest")	, m_bUseCellInforRequest);	
	//kjpark 20180113 Tmd Version 추가
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseHostVersionSend")	, m_bUseHostVersionSend);	
	
	// CellLoading 실패시 리트라이
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseRetryAA")			, m_bUseRetryAA			);	
	// AB 리트라이
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseRetryAB")			, m_bUseRetryAB			);	
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseNgoption")			, m_bUseNgoption		);
	//kjpark 20180119 김현규책임 요청 L 보고를 R로 옵션처리
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseNGChange_R")			, m_bUseNGChange_R		);
	//kjpark 20180120 Cell Mixing 처리(강원호 선임)
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCellMixingA3A4")			, m_bUseCellMixingA3A4		);
	//kjpark 20180129 스크랩시에도 셀로그 적음
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseIf_NotUseWriteCellLog")			, m_bUseIf_NotUseWriteCellLog		);
	
	

	
}
