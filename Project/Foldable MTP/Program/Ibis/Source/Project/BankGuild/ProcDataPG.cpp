#include "StdAfx.h"
#include "ProcDataPG.h"
#include "Etc/FileSupport.h"

void PG_LOG_DATA_MTP::Init()
{
	// 키워드는 초기화하지 않는다
	m_L.Empty();
	m_X.Empty();
	m_Y.Empty();
	m_TCOLOR.Empty();
	m_MPCD.Empty();
}

CProcDataPG::CProcDataPG(void)
{
	m_S_MID_255_PRE_.strKeyword = _T("S_MID_255_PRE_");
	m_S_MID_255_.strKeyword = _T("S_MID_255_");
	m_MID_255_PRE.strKeyword = _T("MID_255_PRE");
	m_MID_255.strKeyword = _T("MID_255");
	m_300CD_V255_PRE.strKeyword = _T("300CD_V255_PRE");
	m_300CD_V255.strKeyword = _T("300CD_V255");
	Init(JIG_ID_MAX, JIG_CH_MAX);
}


CProcDataPG::~CProcDataPG(void)
{
}

void CProcDataPG::Init(JIG_ID jig, JIG_CH ch)
{
	// 키워드는 초기화하지 않는다
	m_Jig = jig;
	m_Ch = ch;
	m_CORRELATION.Empty();
	m_BEF_255_MTP.Empty();

	m_S_MID_255_PRE_.Init();
	m_S_MID_255_.Init();
	m_MID_255_PRE.Init();
	m_MID_255.Init();
	m_300CD_V255_PRE.Init();
	m_300CD_V255.Init();

	m_pS_MID_255 = NULL;
	m_p300CD_V255 = NULL;

	//kjpark 20180129 MID ALPM 255 IBAT 항목추가
	m_MID_ALPM_255.Empty();

	iMapMTPInspectionValue.clear();
}

//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
BOOL CProcDataPG::ScanLog(CString strCellID, CString strInnerID, SYSTEMTIME time)
{
	//////////////////////////////////////////////////////////////////////////
	// Log를 읽어서 변수에 넣는다

	// 기존 값 초기화
	Init(m_Jig, m_Ch);
	CString strPathCellID;
	CString strPathInnerID;

#if 0
	// \\192.168.10.140\Result\[yyyy]-[mm]-[dd]\[Channel]\MTP\[CellID or InnerID]_[yyyymmdd]_[HHMMSS].csv
	int nFullCh = ((m_Jig*JIG_CH_MAX) + m_Ch)+1;	
	strPath.Format(_T("%s\\%04u-%02u-%02u\\%03d\\MTP\\%s_%04u%02u%02u_%02u%02u%02u.csv"), 
		theConfigBank.m_System.m_strPGLogPath,	// \\192.168.10.140\Result
		time.wYear, time.wMonth, time.wDay,	// [yyyy]-[mm]-[dd]
		nFullCh,								// [Channel]
		strCellID,								// [CellID or InnerID]
		time.wYear, time.wMonth, time.wDay,	// [yyyymmdd]
		time.wHour, time.wMinute, time.wSecond	// [HHMMSS]
		);
#endif
	int nFullCh = ((m_Jig*JIG_CH_MAX) + m_Ch)+1;

	strPathCellID.Format(_T("%s\\%04u-%02u-%02u\\%03d\\MTP\\%s_%04u%02u%02u_*.csv"), 
		theConfigBank.m_System.m_strPGLogPath,	// \\192.168.10.140\Result
		//_T("O:\\Result"),
		time.wYear, time.wMonth, time.wDay,	// [yyyy]-[mm]-[dd]
		nFullCh,								// [Channel]
		strCellID,								// [CellID or InnerID]
		time.wYear, time.wMonth, time.wDay	// [yyyymmdd]		
		);

	strPathInnerID.Format(_T("%s\\%04u-%02u-%02u\\%03d\\MTP\\%s_%04u%02u%02u_*.csv"), 
		theConfigBank.m_System.m_strPGLogPath,	// \\192.168.10.140\Result
		//_T("O:\\Result"),
		time.wYear, time.wMonth, time.wDay,	// [yyyy]-[mm]-[dd]
		nFullCh,								// [Channel]
		strInnerID,								// [CellID or InnerID]
		time.wYear, time.wMonth, time.wDay	// [yyyymmdd]		
		);

	//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
	theLog[LOG_SEQUENCE].AddBuf(_T("[ScanLog] %s, %s"), strPathCellID, strPathInnerID);
	return GetMTPInspectFileSearch(strPathCellID, strPathInnerID);	
}


BOOL CProcDataPG::SelectData()
{
	//////////////////////////////////////////////////////////////////////////
	// S_MID_255하고 300CD_255를 우선순위 판단하여 각각 하나의 값을 체택하여 포인터에 넣는다
	//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
	SetDataMTPValue();	

	// S_MID_255 선택
	if(m_S_MID_255_PRE_.m_L.IsEmpty() == FALSE)		m_pS_MID_255 = &m_S_MID_255_PRE_;
	else if(m_S_MID_255_.m_L.IsEmpty() == FALSE)	m_pS_MID_255 = &m_S_MID_255_;
	else if(m_MID_255_PRE.m_L.IsEmpty() == FALSE)	m_pS_MID_255 = &m_MID_255_PRE;
	else if(m_MID_255.m_L.IsEmpty() == FALSE)		m_pS_MID_255 = &m_MID_255;
	else											m_pS_MID_255 = NULL;

	// 300CD_255 선택
	if(m_300CD_V255_PRE.m_L.IsEmpty() == FALSE)		m_p300CD_V255 = &m_300CD_V255_PRE;
	else if(m_300CD_V255.m_L.IsEmpty() == FALSE)	m_p300CD_V255 = &m_300CD_V255;
	else											m_p300CD_V255 = NULL;

	if(m_pS_MID_255 && m_p300CD_V255)
		return TRUE;
	else 
		return FALSE;
}

void CProcDataPG::SetValueToCellInfo(CCellInfo *pCell)
{
	//////////////////////////////////////////////////////////////////////////
	// 변수값을 CellInfo에 넣는다
	if(pCell == NULL)
		return;

	pCell->m_PG_LOG_INFO.m_CORRELATION = m_CORRELATION;
	pCell->m_PG_LOG_INFO.m_BEF_255_MTP = m_BEF_255_MTP;
	//kjpark 20180129 MID ALPM 255 IBAT 항목추가
	pCell->m_PG_LOG_INFO.m_MID_ALPM_255_IBAT = m_MID_ALPM_255;
	if(m_pS_MID_255)
	{
		pCell->m_PG_LOG_INFO.m_S_MID_255_PRE_L_MTP = m_pS_MID_255->m_L;
		pCell->m_PG_LOG_INFO.m_S_MID_255_PRE_X_MTP = m_pS_MID_255->m_X;
		pCell->m_PG_LOG_INFO.m_S_MID_255_PRE_Y_MTP = m_pS_MID_255->m_Y;
		pCell->m_PG_LOG_INFO.m_S_MID_255_PRE_TCOLOR_MTP = m_pS_MID_255->m_TCOLOR;
		pCell->m_PG_LOG_INFO.m_S_MID_255_PRE_MPCD_MTP = m_pS_MID_255->m_MPCD;
	}
	if(m_p300CD_V255)
	{
		pCell->m_PG_LOG_INFO.m_300CD_V255_L_VERIFY = m_p300CD_V255->m_L;
		pCell->m_PG_LOG_INFO.m_300CD_V255_X_VERIFY = m_p300CD_V255->m_X;
		pCell->m_PG_LOG_INFO.m_300CD_V255_Y_VERIFY = m_p300CD_V255->m_Y;
		pCell->m_PG_LOG_INFO.m_300CD_V255_TCOLOR_VERIFY = m_p300CD_V255->m_TCOLOR;
		pCell->m_PG_LOG_INFO.m_300CD_V255_MPCD_VERIFY = m_p300CD_V255->m_MPCD;
	}
}

//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
BOOL CProcDataPG::GetMTPInspectFileSearch(CString strPathCellID, CString strPathInnerID)
{
	CFileFind finder;

	BOOL  bWorking = finder.FindFile(strPathCellID);
	CString strFilename = _T("");
	BOOL bFileFined = FALSE;

	if(bWorking)
	{
		while(bWorking)
		{
			bWorking = finder.FindNextFile();
			CString strSearchFile = finder.GetFilePath(); 		 		 
			if(strSearchFile.Compare(strPathCellID))
			{
				if(CFileSupport::IsFile(strSearchFile))
				{
					strFilename = strSearchFile;
					bFileFined = TRUE;
					break;
				}			 
			}
		}
	}
	else
	{
		if(bFileFined == FALSE)
		{
			bWorking = finder.FindFile(strPathInnerID);
			while(bWorking)
			{
				bWorking = finder.FindNextFile();
				CString strSearchFile = finder.GetFilePath(); 		 		 
				if(strSearchFile.Compare(strPathInnerID))
				{
					if(CFileSupport::IsFile(strSearchFile))
					{
						strFilename = strSearchFile;
						bFileFined = TRUE;
						break;
					}			 
				}
			}		
		}
	}
	finder.Close();

	if(bFileFined)
		SetAddMTPValue(strFilename);
	

	if(iMapMTPInspectionValue.size() > 0 
		&& bFileFined == TRUE)
		return TRUE;
	else
		return FALSE;
	return FALSE;
}

//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
BOOL CProcDataPG::SetAddMTPValue(CString strFilename)
{
	CString sLine;						// 파일에서 읽어온 데이터	
	CString		sKey;					// 검색할 때 사용되는 Key값, 공백제거, 대문자 변환
	MTP_VALUE StructMTPValue;

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strFilename.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));
	if (e != 0) return FALSE;
	
	CStdioFile		file(fStream);
	while ( file.ReadString(sLine) )
	{
		if (sLine.IsEmpty() )	continue;			
		if (sLine.Left(1) == _T("?"))	continue;	
		if (sLine.Left(1) == _T("/"))	continue;	
		if (sLine.Left(1) == _T("!"))	continue;	
		if (sLine.Left(1) == _T(";"))	continue;	
		sLine.Remove(_T(';'));

		::AfxExtractSubString( sKey,										sLine	, 6, ',');		// Matching	Name
		::AfxExtractSubString( StructMTPValue.strMTPInspection_L,			sLine	, 7, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_x,			sLine	, 8, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_y,			sLine	, 9, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_u,			sLine	, 10, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_v,			sLine	, 11, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_Tcolor,		sLine	, 12, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_Mpcd,		sLine	, 13, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_VBAT,		sLine	, 14, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_IBAT,		sLine	, 15, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_VEXT,		sLine	, 16, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_IEXT,		sLine	, 17, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_VDD,		sLine	, 18, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_IDD,		sLine	, 19, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_VCI,		sLine	, 20, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_ICI,		sLine	, 21, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_VBAT2,		sLine	, 22, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_IBAT2,		sLine	, 23, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_VDD2,		sLine	, 24, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_IDD2,		sLine	, 25, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_R,			sLine	, 26, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_G,			sLine	, 27, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_B,			sLine	, 28, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_ID1,		sLine	, 29, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_ID2,		sLine	, 30, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_ID3,		sLine	, 31, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_GAMMA,		sLine	, 32, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_Ptime,		sLine	, 33, ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_Ttime,		sLine	, 34,  ',');	
		::AfxExtractSubString( StructMTPValue.strMTPInspection_JUDGE,		sLine	, 35, ',');	


		iMapMTPInspectionValue.insert(pair<CString, MTP_VALUE>(sKey, StructMTPValue));
	}
	fclose(fStream);

	return TRUE;
}

//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
void CProcDataPG::SetDataMTPValue()
{
	MTP_VALUE StructMTPValue = iMapMTPInspectionValue[_T("BEF_255")];
	m_BEF_255_MTP = StructMTPValue.strMTPInspection_L;

	StructMTPValue = iMapMTPInspectionValue[_T("BG7000_CAL_DATE")];
	m_CORRELATION = StructMTPValue.strMTPInspection_JUDGE;

	StructMTPValue = iMapMTPInspectionValue[_T("MID_255")];
	m_MID_255.m_L = StructMTPValue.strMTPInspection_L;
	m_MID_255.m_X = StructMTPValue.strMTPInspection_x;
	m_MID_255.m_Y = StructMTPValue.strMTPInspection_y;
	m_MID_255.m_TCOLOR = StructMTPValue.strMTPInspection_Tcolor;
	m_MID_255.m_MPCD = StructMTPValue.strMTPInspection_Mpcd;

	StructMTPValue = iMapMTPInspectionValue[_T("MID_255_PRE")];
	m_MID_255_PRE.m_L = StructMTPValue.strMTPInspection_L;
	m_MID_255_PRE.m_X = StructMTPValue.strMTPInspection_x;
	m_MID_255_PRE.m_Y = StructMTPValue.strMTPInspection_y;
	m_MID_255_PRE.m_TCOLOR = StructMTPValue.strMTPInspection_Tcolor;
	m_MID_255_PRE.m_MPCD = StructMTPValue.strMTPInspection_Mpcd;

	StructMTPValue = iMapMTPInspectionValue[_T("S_MID_255")];
	m_S_MID_255_.m_L = StructMTPValue.strMTPInspection_L;
	m_S_MID_255_.m_X = StructMTPValue.strMTPInspection_x;
	m_S_MID_255_.m_Y = StructMTPValue.strMTPInspection_y;
	m_S_MID_255_.m_TCOLOR = StructMTPValue.strMTPInspection_Tcolor;
	m_S_MID_255_.m_MPCD = StructMTPValue.strMTPInspection_Mpcd;

	StructMTPValue = iMapMTPInspectionValue[_T("S_MID_255_PRE_")];
	m_S_MID_255_PRE_.m_L = StructMTPValue.strMTPInspection_L;
	m_S_MID_255_PRE_.m_X = StructMTPValue.strMTPInspection_x;
	m_S_MID_255_PRE_.m_Y = StructMTPValue.strMTPInspection_y;
	m_S_MID_255_PRE_.m_TCOLOR = StructMTPValue.strMTPInspection_Tcolor;
	m_S_MID_255_PRE_.m_MPCD = StructMTPValue.strMTPInspection_Mpcd;

	StructMTPValue = iMapMTPInspectionValue[_T("300CD_V255")];
	m_300CD_V255.m_L = StructMTPValue.strMTPInspection_L;
	m_300CD_V255.m_X = StructMTPValue.strMTPInspection_x;
	m_300CD_V255.m_Y = StructMTPValue.strMTPInspection_y;
	m_300CD_V255.m_TCOLOR = StructMTPValue.strMTPInspection_Tcolor;
	m_300CD_V255.m_MPCD = StructMTPValue.strMTPInspection_Mpcd;

	StructMTPValue = iMapMTPInspectionValue[_T("300CD_V255_PRE")];
	m_300CD_V255_PRE.m_L = StructMTPValue.strMTPInspection_L;
	m_300CD_V255_PRE.m_X = StructMTPValue.strMTPInspection_x;
	m_300CD_V255_PRE.m_Y = StructMTPValue.strMTPInspection_y;
	m_300CD_V255_PRE.m_TCOLOR = StructMTPValue.strMTPInspection_Tcolor;
	m_300CD_V255_PRE.m_MPCD = StructMTPValue.strMTPInspection_Mpcd;

	//kjpark 20180129 MID ALPM 255 IBAT 항목추가
	StructMTPValue = iMapMTPInspectionValue[_T("MID_ALPM_255")];
	m_MID_ALPM_255 = StructMTPValue.strMTPInspection_IBAT;
}
