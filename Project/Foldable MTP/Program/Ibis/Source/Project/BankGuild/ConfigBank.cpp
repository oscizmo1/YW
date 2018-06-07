#include "StdAfx.h"
#include "ConfigBank.h"
#include "Etc/FileSupport.h"
#include "Etc/Ini.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigBank::CConfigBank(void)
{
}


CConfigBank::~CConfigBank(void)
{
}

void CConfigBank::ReadSystemFile()
{
	CString strPath;
	strPath.Format(_T("%s\\Ibis.ini"), PATH_SYSTEM);
	m_System.ReadSystemFile(strPath);
}
void CConfigBank::ReadLastDataFile()
{
	CString strPath;
	strPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigPath);

	CIni ini(strPath);
	GetMainHandler()->m_nLangIdx  = (LANGUAGE)ini.GetLong(_T("LAST_DATA"),_T("LANGUAGE"), 0);
	theProcBank.m_strEqipID  = ini.GetString(_T("LAST_DATA"),_T("EQUIPID"), _T("B1ASY99N_F11"));
	::PostMessage(GetMainHandler()->m_hWnd, GxMSG_GUI_REQUEST_LANGUAGE, GetMainHandler()->m_nLangIdx, 0);
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top)->m_hWnd, UDMSG_CIM_EQIP_ID_CHANGE, NULL, NULL);		// 연결 이벤트
}

void CConfigBank::SaveLastDataFile()
{
	CString strPath,strBackupPath;
	strPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigBackupPath);
	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

// 	if(CFileSupport::FileCheck(strPath))
// 	{
// 		// 기존 파일 백업
// 		::CopyFile(strPath, strBackupPath, FALSE);
// 		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
// 		::DeleteFile(strPath);
// 	}

	CIni ini(strPath);

	ini.WriteLong(_T("LAST_DATA")		, _T("LANGUAGE"),  GetMainHandler()->m_nLangIdx );
	ini.WriteString(_T("LAST_DATA")		, _T("EQUIPID"),  theProcBank.m_strEqipID );
}
void CConfigBank::ReadOptionFile()
{
	CString strPath;
	strPath.Format(_T("%s\\Option.ini"), m_System.m_strConfigPath);
	m_Option.ReadOptionFile(strPath);
}

void CConfigBank::SaveOptionFile()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
	strPath.Format(_T("%s\\Option.ini"), m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\Option_%02d_%s.ini"), m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.Format(_T("%H%M%S")));
	m_Option.SaveOptionFile(strPath, strBackupPath);
}

//kjpark 20161121 MES code list read
BOOL CConfigBank::LoadDefectNameTable()
{
	CString strFileName;

	strFileName.Format(_T("%s\\NgCodeList.txt"),theConfigBank.m_System.m_strConfigPath);


	// 파일이 존재하는지 점검
	if(CFileSupport::IsFile(strFileName) == FALSE) 	
	{	
		return FALSE;
	}

	// .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..

	CString sLine;						// 파일에서 읽어온 데이터	

	CString		sKey;					// 검색할 때 사용되는 Key값, 공백제거, 대문자 변환
	CString		sName1;					// 파일에서 읽어들인 Defect 글자
	CString		sName2;					// 영문 및 한글 defect
	CString		sName3;					
	CString		sCode;					// MES 보고용 Defect Code
	CString		sRetryAble;				// 1 : 리트라이 불가, 0 : 리트라이 가능 (기본값)

	//파일을 한줄씩 읽어가면서 넣어준다.;
	// Text 파일 UniCode 방식으로 설정 변경 ( 베트남어 읽고 쓰기 가능)
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strFileName.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));
	if (e != 0) return FALSE;

	CStdioFile		file(fStream);
	//map<CString, DEFECT_MES_CODE> iMapDefectList;
	// 파일에서 1줄씩 읽어와서 처리한다.
	int nIndex = 0;
	while ( file.ReadString(sLine) )
	{
		if (sLine.IsEmpty() )	continue;			// 공백일 경우 무시
		if (sLine.Left(1) == _T("?"))	continue;	// 처음 글자가 " ? " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("/"))	continue;	// 처음 글자가 " / " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T(";"))	continue;	// 처음 글자가 " ; " 일경우 다음 문장으로 넘어간다.;

		// 문장 맨뒤에 ;;가 붙어나와서 삭제 [11/10/2017 OSC]
		sLine.Remove(_T(';'));
		::AfxExtractSubString( sName1,		sLine	, 0, ',');		// Matching	Name
		::AfxExtractSubString( sName2,		sLine	, 1, ',');		// Kor Name(미사용)
		::AfxExtractSubString( sName3,		sLine	, 2, ',');		// Eng Name(미사용)
		::AfxExtractSubString( sCode,		sLine	, 3, ',');		// MES Code
// 		::AfxExtractSubString( sCode,		sLine	, 4, ',');		// Unload Tray Port No
// 		::AfxExtractSubString( sCode,		sLine	, 5, ',');		// Group ???
		::AfxExtractSubString( sRetryAble,	sLine	, 6, ',');		// AAB Retry 가능여부 (1 : 가능. 0 : 불가. 바로 'L'보고)

		if ( sName1.IsEmpty() || sCode.IsEmpty() ) continue;			// 공백이라면 무시
				
		sName1.MakeUpper();
		sName2.MakeUpper();
		sName3.MakeUpper();

		DEFECT_MES_CODE MesCode;
		MesCode.strPopup = sName1;
		MesCode.strPopup_KOR = sName2;
		MesCode.strPopup_ENG = sName3;
		MesCode.strMES_CODE = sCode;
		MesCode.bRetryAble = _tstoi(sRetryAble) == 0 ? FALSE:TRUE;	// 1이거나 빈칸(기본값)이면 AB 리트라이 가능
		theProcBank.iMapDefectList.insert(pair<int, DEFECT_MES_CODE>(nIndex, MesCode));
		nIndex++;
	}//of while

	file.Close();
	fclose(fStream);

	return TRUE;
}


//kjpark 20161121 MES code list read
void CConfigBank::ReadNGCodeFile()
{
	LoadDefectNameTable();	
}

void CConfigBank::ReadNGAlarmLimitFile()
{
	CString strPath;
	strPath.Format(_T("%s\\NGAlarmLimit.ini"), m_System.m_strConfigPath);
	m_NGAlarmLimit.ReadNGAlarmFile(strPath);
}

void CConfigBank::SaveNGAlarmLimitFile()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
	strPath.Format(_T("%s\\NGAlarmLimit.ini"), m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\NGMonitorLimit_%02d_%s.ini"), m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.Format(_T("%H%M%S")));
	m_NGAlarmLimit.SaveNGAlarmLimitFile(strPath, strBackupPath);
}

void CConfigBank::ReadDefectPatternFile()
{
	CString strPath;
	strPath.Format(_T("%s\\DefectPattern.csv"), m_System.m_strConfigPath);
	m_DefectPattern.ReadPatternFile(strPath);
}

void CConfigBank::ReadDefectRetestFile()
{
	CString strPath;
	strPath.Format(_T("%s\\DefectRetest.txt"), m_System.m_strConfigPath);
	m_DefectRetest.ReadDefectRetestFile(strPath);
}

void CConfigBank::ReadBZoneDefectCodeFile()
{
	CString strPath;
	strPath.Format(_T("%s\\%s"), m_System.m_strConfigPath, NAME_FILE_DEFECT_LIST);
	m_BZoneDefect.ReadBZoneDefectCodeFile(strPath);
}

void CConfigBank::ReadTimeOutFile()
{
	CString strPath;
	strPath.Format(_T("%s\\TimeOut.ini"), m_System.m_strConfigPath);
	m_TimeOut.ReadTimeOutFile(strPath);
}

void CConfigBank::SaveTimeOutFile()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
	strPath.Format(_T("%s\\TimeOut.ini"), m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\TimeOut_%02d_%s.ini"), m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.Format(_T("%H%M%S")));
	m_TimeOut.SaveTimeOutFile(strPath, strBackupPath);
}

void CConfigBank::ReadCIMFile()
{
	CString strPath;
	strPath.Format(_T("%s\\CIM.ini"), m_System.m_strConfigPath);
	m_CIM.ReadCIMOptionFile(strPath);
}

void CConfigBank::SaveCIMFile()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
	strPath.Format(_T("%s\\CIM.ini"), m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\CIM_%02d_%s.ini"), m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.Format(_T("%H%M%S")));
	m_CIM.SaveCIMOptionFile(strPath, strBackupPath);
}

void CConfigBank::ProgramRestartOptionInit()
{
	// 프로그램 재시작시 항상 강제로 On/Off할 변수 처리하는 함수

	m_Option.m_bUseTMDNameMatch = TRUE;
}