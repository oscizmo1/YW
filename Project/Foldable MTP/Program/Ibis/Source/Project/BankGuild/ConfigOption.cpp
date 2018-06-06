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
	
	//kjpark 20171010 TMD March ����
	m_bUseTMDNameMatch		= ini.GetBoolean(_T("OPTION")		, _T("m_bUseTMDNameMatch")	, FALSE);

	//20170705 kjpark cell infomation request ��� �ɼ�
	//kjpark 20170907 Tracking CellInfomation, JobProcess �߰�
	m_bUseCellInforRequest	= ini.GetBoolean(_T("OPTION")		, _T("m_bUseCellInforRequest")			, TRUE);	
	//kjpark 20180113 Tmd Version �߰�
	m_bUseHostVersionSend	= ini.GetBoolean(_T("OPTION")		, _T("m_bUseHostVersionSend")			, TRUE);	
	
	// CellLoading ���н� ��Ʈ����
	m_bUseRetryAA			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseRetryAA")			, FALSE);
	// AB ��Ʈ����
	m_bUseRetryAB			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseRetryAB")			, TRUE);
	m_bUseNgoption			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseNgoption")			, TRUE);

	//kjpark 20180119 ������å�� ��û L ���� R�� �ɼ�ó��
	m_bUseNGChange_R			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseNGChange_R")			, TRUE);
	//kjpark 20180120 Cell Mixing ó��(����ȣ ����)
	m_bUseCellMixingA3A4			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseCellMixingA3A4")			, FALSE);
	//kjpark 20180129 ��ũ���ÿ��� ���α� ����
	m_bUseIf_NotUseWriteCellLog			= ini.GetBoolean(_T("OPTION")		, _T("m_bUseIf_NotUseWriteCellLog")			, FALSE);
}

void CConfigOption::SaveOptionFile( CString strPath, CString strBackupPath )
{
	if(CFileSupport::FileCheck(strPath))
	{
		// ���� ���� ���
		::CopyFile(strPath, strBackupPath, FALSE);
		// File ������ ����ϰ� �ϱ� ���� ������ ����� ���� ���´�
		::DeleteFile(strPath);
	}

	CIni ini(strPath);
	// �˶� �������� ���� �α� ����� ������ offset �ð���
	ini.WriteLong(_T("OPTION")		, _T("m_OffsetHour")			, m_OffsetHour			);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseLoofTest")			, m_bUseLoofTest		);
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseByPass")			, m_bUseByPass			);
	ini.WriteDouble(_T("OPTION")	, _T("m_dFDCCheckTime")			, m_dFDCCheckTime		);	
	ini.WriteBoolean(_T("OPTION")	, _T("m_bFDCAlarm")				, m_bFDCAlarm			);	
	
	//kjpark 20171010 TMD March ����
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseTMDNameMatch")		, m_bUseTMDNameMatch	);	
	//20170705 kjpark cell Jobstart ��� �ɼ�
	//kjpark 20170907 Tracking CellInfomation, JobProcess �߰�
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCellInforRequest")	, m_bUseCellInforRequest);	
	//kjpark 20180113 Tmd Version �߰�
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseHostVersionSend")	, m_bUseHostVersionSend);	
	
	// CellLoading ���н� ��Ʈ����
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseRetryAA")			, m_bUseRetryAA			);	
	// AB ��Ʈ����
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseRetryAB")			, m_bUseRetryAB			);	
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseNgoption")			, m_bUseNgoption		);
	//kjpark 20180119 ������å�� ��û L ���� R�� �ɼ�ó��
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseNGChange_R")			, m_bUseNGChange_R		);
	//kjpark 20180120 Cell Mixing ó��(����ȣ ����)
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseCellMixingA3A4")			, m_bUseCellMixingA3A4		);
	//kjpark 20180129 ��ũ���ÿ��� ���α� ����
	ini.WriteBoolean(_T("OPTION")	, _T("m_bUseIf_NotUseWriteCellLog")			, m_bUseIf_NotUseWriteCellLog		);
	
	

	
}
