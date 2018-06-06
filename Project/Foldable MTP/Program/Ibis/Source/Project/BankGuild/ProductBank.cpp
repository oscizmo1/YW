#include "StdAfx.h"
#include "ProductBank.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CProductBank::CProductBank(void)
{
	m_strCurTime.Empty();
	// ��¥ �ʱ�ȭ - LSH171217
	DateCheck();

	MainCount_Init();
}


CProductBank::~CProductBank(void)
{

}


// ETC
void CProductBank::InitData()
{
	// ������ �ʱ�ȭ - LSH161217
	m_MapData.clear();

	// Map�� ��ü �߰� - LSH171218
	for(int nTime=0 ; nTime<24 ; nTime++)
	{
		ProductInfo infoTemp;
		m_MapData.insert(make_pair(nTime, infoTemp));

		// �ð� �Է� - LSH171218
		m_MapData[nTime].m_nHour = nTime;
	}

}

BOOL CProductBank::DirectoryCheck(BOOL bCreateDirectory)
{
	CString strDateDirectoryPath;
	strDateDirectoryPath.Format(_T("%s\\%s"), theConfigBank.m_System.m_strDebugLogPath, m_strCurTime) ;

	CString strReportDirectoryPath;
	strReportDirectoryPath.Format(_T("%s\\%s\\Report"), theConfigBank.m_System.m_strDebugLogPath, m_strCurTime) ;

	// ���� ��¥�� �ش��ϴ� ������ �����Ѵ� - LSH17121
	if(CFileSupport::DirectoryCheck(strDateDirectoryPath) == FALSE)
	{
		// ��� ���� ���� Ȯ�� - LSH171218
		if(bCreateDirectory)
		{
			if(CFileSupport::CreateDirectory(strDateDirectoryPath) == FALSE)
				return FALSE;
		}
		else
			return FALSE;
	}

	// Report ������ �ִ��� Ȯ�� �� ���ٸ� �����Ѵ� - LSH17121
	if(CFileSupport::DirectoryCheck(strReportDirectoryPath) == FALSE)
	{
		// ��� ���� ���� Ȯ�� - LSH171218
		if(bCreateDirectory)
		{
			if(CFileSupport::CreateDirectory(strReportDirectoryPath) == FALSE)
				return FALSE;
		}
		else
			return FALSE;
	}

	return TRUE;
}

CString CProductBank::CreateSavePath(CString strDate)
{
	CString strResult = _T("");
	strResult.Format(_T("%s\\%s\\Report"), theConfigBank.m_System.m_strDebugLogPath, strDate) ;

	return strResult;
}

void CProductBank::DateCheck()
{
	// ���� ��¥�� �����´� - LSH171217
	SYSTEMTIME time;
	GetLocalTime(&time);
	CTime offsettime(time);
	CEtc::ApplyTimeOffset(offsettime, theConfigBank.m_Option.m_OffsetHour, 0);
	CString szCurTime;
	szCurTime.Format(_T("%04u-%02u-%02u"),offsettime.GetYear(), offsettime.GetMonth(), offsettime.GetDay());	

	// ���� ��¥�� ���� ��¥�� ���Ѵ� - LSH171217
	if(m_strCurTime != szCurTime)
	{
		// ��¥�� �����Ѵ� - LSH171217
		m_strCurTime = szCurTime;
		InitData();
	}
	// ���� �ð��� �Է��Ѵ� - LSH171217
	m_nCurHour = time.wHour;
}


// Load
BOOL CProductBank::LoadReportFileToday()
{
	// ���� �ð��� �����´� - LSH171217
	DateCheck();

	// File ��θ� �����Ѵ� - LSH171217
	CString strPath = _T("");
	strPath = CreateSavePath(m_strCurTime);

	// ���� �о���� - LSH171217
	if(LoadReportFile(strPath))
		return TRUE;
	else
		return FALSE;
}

BOOL CProductBank::LoadReprotFileDate(CString strDate)
{
	m_strCurTime = strDate;

	// File ��θ� �����Ѵ� - LSH171217
	CString strPath = _T("");
	strPath = CreateSavePath(strDate);

	// ���� �о���� - LSH171217
	if(LoadReportFile(strPath))
		return TRUE;
	else
		return FALSE;
}

BOOL CProductBank::LoadReportFile(CString strPath)
{
	// ������ �ʱ�ȭ
	InitData();

	// ���丮�� �����Ǿ� �ִ��� Ȯ���Ѵ� ��ΰ� �����Ǿ� ���� �ʴٸ� �Լ��� ���� - LSH171217
	if(!DirectoryCheck(FALSE))
		return FALSE;

	CStdioFile file;
	CFileException e;

	// File reading ��� ��ȯ ���� - LSH171217
	BOOL bResult = FALSE;

	// 0�ú� 23�� ������ �ݺ��Ѵ�  - LSH171217
	for(int nTime=0 ; nTime<24 ; nTime++ )
	{
		// ���� ���� ��� ����
		CString strFilePath;
		strFilePath.Format(_T("%s\\%d.txt"), strPath, nTime);

		if ( file.Open(strFilePath.GetBuffer(), CFile::modeRead, &e ) )
		{
			// ���� �б� ���� - LSH171218
			bResult = TRUE;
			
			// File reading�� ���Ǵ� ���� - LSH171218
			CString strLine = _T("");
			CString strData = _T("");

			// ���پ� �о�´� - LSH171217
			while(file.ReadString(strLine))
			{
				// ��ū�� ��ü�� ���Ͽ��� ������ ���� �־��ش� - LSH171218
				CTokenizer t(strLine);

				// �ð��� �о�´�
				_ttoi(t.GetNextToken(_T(",")));

				// JIG�� CH����ŭ �ݺ��Ѵ� - LSH171217
				/* JIG */
				for(int nJig=JIG_ID_A ; nJig<JIG_ID_MAX ; nJig++)
				{
					/* CH */
					for(int nCh=JIG_CH_1 ; nCh<JIG_CH_MAX ; nCh++)
					{
						// Column ���� �о�´� - LSH171218
						for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
						{
							m_MapData[nTime].m_vetProductData[nColumn][nJig][nCh] = _ttoi(t.GetNextToken(_T(",")));
							m_MapData[nTime].m_nTotalData[nColumn] += m_MapData[nTime].m_vetProductData[nColumn][nJig][nCh];
						}
					}
				}
			}

			// File�� �ݴ´� - LSH171218
			file.Close();
		}
	}

	// ��� ��ȯ - LSH171217
	return bResult;
}



// Save
BOOL CProductBank::SaveReportFileToday()
{
	// File ��θ� �����Ѵ� - LSH171217
	CString strFilePath = _T("");
	strFilePath = CreateSavePath(m_strCurTime);

	// ���� �о���� - LSH171217
	if(SaveReportFile(strFilePath))
		return TRUE;
	else
		return FALSE;
}

BOOL CProductBank::SaveReportFile(CString strPath)
{
// 	// ���� ��¥�� Ȯ���Ѵ� - LSH171218
// 	DateCheck();

	// ���丮�� �����Ǿ� �ִ��� Ȯ���Ѵ� - LSH171217
	DirectoryCheck(TRUE);

	// File ��ü ���� - LSH171217
	CStdioFile file;
	CFileException e;

	// File reading ��� ��ȯ ���� - LSH171217
	BOOL bResult = FALSE;

	// ���� ���� ��� ����
	CString strFilePath;
	strFilePath.Format(_T("%s\\%d.txt"), strPath, m_nCurHour);

	// File�� �о�´� - LSH171217
	if( file.Open(strFilePath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ))
	{
		// ���� �б� ����
		bResult = TRUE;

		// File�� ������ ������ ���� ���� - LSH171217
		CString strData = _T("");

		// �ӽ� ���ڿ�
		CString strTemp;

		// �ð� �Է� - LSH171218
		strTemp.Format(_T("%d,"),m_MapData[m_nCurHour].m_nHour);
		strData += strTemp;

		// JIG�� CH����ŭ �ݺ��Ѵ� - LSH171217
		/* JIG */
		for(int nJig=JIG_ID_A ; nJig<JIG_ID_MAX ; nJig++)
		{
			/* CH */
			for(int nCh=JIG_CH_1 ; nCh<JIG_CH_MAX ; nCh++)
			{
				// Column ���� �о�´� - LSH171218
				for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
				{
					strTemp.Format(_T("%d,"),m_MapData[m_nCurHour].m_vetProductData[nColumn][nJig][nCh]);
					strData += strTemp;
				}
			}
		}

		// File�� ������ �Է� ���� - LSH171218
		file.WriteString(strData);

		// ������ �ݴ´� - LSH171218
		file.Close();
	}

	// ��� ��ȯ - LSH171217
	return bResult;
}



// Calculate
CString CProductBank::CalcPercent(int iEntire, int iValue)
{
	CString szPer;
	if( (iEntire == 0) || (iValue == 0) )
		szPer = "0%";
	else
	{
		double dPer = (double)iValue/ (double)iEntire;
		szPer.Format(_T("%.1f"), dPer*100.);
		szPer += _T("%");
	}
	return szPer;
}

void CProductBank::GetProductData(ProductInfo* infoProduct, UINT nTime)
{
	// �ð� �Է� - LSH171220
	infoProduct->m_nHour = m_MapData[nTime].m_nHour;

	// Total data �Է� - LSH171218
	for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
		infoProduct->m_nTotalData[nColumn] = m_MapData[nTime].m_nTotalData[nColumn];

	// Jig / Ch data �Է� - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
	{
		for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
		{
			// Column�� �ݺ� ����
			for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
				infoProduct->m_vetProductData[nColumn][nJig][nCh] =  m_MapData[nTime].m_vetProductData[nColumn][nJig][nCh];
		}
	}
}

void CProductBank::GetProductData(ProductInfo* infoProduct, UINT nStartTime, UINT nEndTime)
{
	// �Է¹��� �ð���ŭ �ݺ��Ѵ� - LSH171217
	for(int nTime=nStartTime ; nTime!=(int)nEndTime ; nTime++)
	{
		// ...23->24->0->1... �̷��� ������ �� �ֵ���24�� �� ��� 0�� �־��ش� - LSH171218
		if(nTime >= 24)
			nTime = 0;
	
		// Total data �� ��� - LSH171218
		for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
			infoProduct->m_nTotalData[nColumn] += m_MapData[nTime].m_nTotalData[nColumn];

		// Jig / Ch �� ������ �� ��� - LSH171218
		for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
		{
			for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
			{
				// Column�� �ݺ� ����
				for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
					infoProduct->m_vetProductData[nColumn][nJig][nCh] +=  m_MapData[nTime].m_vetProductData[nColumn][nJig][nCh];
			}
		}
	}
}

CString CProductBank::GetProductColumnName(PRODUCT_COLUMN clumCloumn)
{
	switch(clumCloumn)
	{
	case PRODUCT_COLUMN_PRODUCT:
		return _T("Product");
	case PRODUCT_COLUMN_GOOD:
		return _T("Good");
	case PRODUCT_COLUMN_REJECT:
		return _T("Reject");
	case PRODUCT_COLUMN_RETRY:
		return _T("Retry");
	case PRODUCT_COLUMN_MCRREADING:
		return _T("MCR Reading");
	case PRODUCT_COLUMN_FIRSTCONTACT:
		return _T("First Contact");
	case PRODUCT_COLUMN_FINALCONTACT:
		return _T("Final Contact");
	default:
		return _T("None");
	}
}


// Main UI
void CProductBank::MainCount_Init()
{
	m_MainCountGOOD[JIG_ID_A][JIG_CH_1].iValue = 0;
	m_MainCountGOOD[JIG_ID_A][JIG_CH_1].uID = IDC_GXSTC_MAIN_CH1_GOOD_TEXT;

	m_MainCountGOOD[JIG_ID_B][JIG_CH_1].iValue = 0;
	m_MainCountGOOD[JIG_ID_B][JIG_CH_1].uID = IDC_GXSTC_MAIN_CH2_GOOD_TEXT;

	m_MainCountNG[JIG_ID_A][JIG_CH_1].iValue = 0;
	m_MainCountNG[JIG_ID_A][JIG_CH_1].uID = IDC_GXSTC_MAIN_CH1_NG_TEXT;

	m_MainCountNG[JIG_ID_B][JIG_CH_1].iValue = 0;
	m_MainCountNG[JIG_ID_B][JIG_CH_1].uID = IDC_GXSTC_MAIN_CH2_NG_TEXT;

	m_MainCountMCR[JIG_ID_A][JIG_CH_1].iValue = 0;
	m_MainCountMCR[JIG_ID_A][JIG_CH_1].uID = IDC_GXSTC_MAIN_CH1_MCR_TEXT;

	m_MainCountMCR[JIG_ID_B][JIG_CH_1].iValue = 0;
	m_MainCountMCR[JIG_ID_B][JIG_CH_1].uID = IDC_GXSTC_MAIN_CH2_MCR_TEXT;

	m_MainCountCONTACT[JIG_ID_A][JIG_CH_1].iValue = 0;
	m_MainCountCONTACT[JIG_ID_A][JIG_CH_1].uID = IDC_GXSTC_MAIN_CH1_CONTACTTEXT;

	m_MainCountCONTACT[JIG_ID_B][JIG_CH_1].iValue = 0;
	m_MainCountCONTACT[JIG_ID_B][JIG_CH_1].uID = IDC_GXSTC_MAIN_CH2_CONTACTTEXT;
}

void CProductBank::MainCount_Update( CCellInfo *pCell )
{
	JIG_ID jig = (JIG_ID)pCell->defaultData.m_JigId;
	JIG_CH ch = pCell->defaultData.m_JigCh;
 	if(pCell->defaultData.m_LastClass == GOOD_CELL)
 		m_MainCountGOOD[jig][ch].iValue++;
 	else
 		m_MainCountNG[jig][ch].iValue++;
 
 	if(pCell->defaultData.m_strCellID != FAIL)
 		m_MainCountMCR[jig][ch].iValue++;

	if(pCell->m_CellLoading.m_Class == GOOD_CELL )
		m_MainCountCONTACT[jig][ch].iValue++;

	MainCount_Save();
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_COUNT_UPDATE, (WPARAM)jig, (LPARAM)ch);
}

void CProductBank::MainCount_Save()
{
	CString strPath;
	strPath.Format(_T("%s\\MainCount.ini"), theConfigBank.m_System.m_strConfigPath);

	CIni ini(strPath);
	CString strTitle;
	
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			strTitle.Format(_T("JIG_ID%d_CH%d"),jig,ch);
			ini.WriteLong(strTitle, _T("GOOD"), m_MainCountGOOD[jig][ch].iValue);
			ini.WriteLong(strTitle, _T("NG"), m_MainCountNG[jig][ch].iValue);
			ini.WriteLong(strTitle, _T("MCR"), m_MainCountMCR[jig][ch].iValue);
			ini.WriteLong(strTitle, _T("CONTACT"), m_MainCountCONTACT[jig][ch].iValue);
		}
	}

	ini.WriteLong(_T("TIME"), _T("YEAR"), m_timeMainCount.GetYear());
	ini.WriteLong(_T("TIME"), _T("MONTH"), m_timeMainCount.GetMonth());
	ini.WriteLong(_T("TIME"), _T("DAY"), m_timeMainCount.GetDay());
	ini.WriteLong(_T("TIME"), _T("HOUR"), m_timeMainCount.GetHour());
	ini.WriteLong(_T("TIME"), _T("MIN"), m_timeMainCount.GetMinute());
	ini.WriteLong(_T("TIME"), _T("SEC"), m_timeMainCount.GetSecond());
}

void CProductBank::MainCount_Load()
{
	CString strPath;
	strPath.Format(_T("%s\\MainCount.ini"), theConfigBank.m_System.m_strConfigPath);
	CString strTitle;
	CIni ini(strPath);

	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			strTitle.Format(_T("JIG_ID%d_CH%d"),jig,ch);
 			m_MainCountGOOD[jig][ch].iValue = ini.GetLong(strTitle, _T("GOOD"), 0);
 			m_MainCountNG[jig][ch].iValue = ini.GetLong(strTitle, _T("NG"), 0);
 			m_MainCountMCR[jig][ch].iValue = ini.GetLong(strTitle, _T("MCR"), 0);
			m_MainCountCONTACT[jig][ch].iValue = ini.GetLong(strTitle, _T("CONTACT"), 0);
		}
	}

	int nYear = ini.GetLong(_T("TIME"), _T("YEAR"), 2000);
	int nMonth = ini.GetLong(_T("TIME"), _T("MONTH"), 1);
	int nDay = ini.GetLong(_T("TIME"), _T("DAY"), 1);
	int nHour = ini.GetLong(_T("TIME"), _T("HOUR"), 0);
	int nMin = ini.GetLong(_T("TIME"), _T("MIN"), 0);
	int nSec = ini.GetLong(_T("TIME"), _T("SEC"), 0);

	m_timeMainCount = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
}



// Increase
void CProductBank::IncreaseProductCount(int nJigID, int nJigCh)
{

	// ���� �ð��� �ش��ϴ� �׸� ���� - LSH171217
	m_MapData[m_nCurHour].m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJigID][nJigCh]++;
	m_MapData[m_nCurHour].m_nTotalData[PRODUCT_COLUMN_PRODUCT]++;
}

void CProductBank::IncreaseBinZoreCount(int nJigID, int nJigCh)
{
	// ���� �ð��� �ش��ϴ� �׸� ���� - LSH171217
	m_MapData[m_nCurHour].m_vetProductData[PRODUCT_COLUMN_GOOD][nJigID][nJigCh]++;
	m_MapData[m_nCurHour].m_nTotalData[PRODUCT_COLUMN_GOOD]++;
}

void CProductBank::IncreaseBinOneCount(int nJigID, int nJigCh)
{
	// ���� �ð��� �ش��ϴ� �׸� ���� - LSH171217
	m_MapData[m_nCurHour].m_vetProductData[PRODUCT_COLUMN_REJECT][nJigID][nJigCh]++;
	m_MapData[m_nCurHour].m_nTotalData[PRODUCT_COLUMN_REJECT]++;
}

void CProductBank::IncreaseBinTwoCount(int nJigID, int nJigCh)
{
	// ���� �ð��� �ش��ϴ� �׸� ���� - LSH171217
	m_MapData[m_nCurHour].m_vetProductData[PRODUCT_COLUMN_RETRY][nJigID][nJigCh]++;
	m_MapData[m_nCurHour].m_nTotalData[PRODUCT_COLUMN_RETRY]++;
}

void CProductBank::IncreaseMcrOkCount(int nJigID, int nJigCh)
{
	// ���� �ð��� �ش��ϴ� �׸� ���� - LSH171217
	m_MapData[m_nCurHour].m_vetProductData[PRODUCT_COLUMN_MCRREADING][nJigID][nJigCh]++;
	m_MapData[m_nCurHour].m_nTotalData[PRODUCT_COLUMN_MCRREADING]++;
}

void CProductBank::IncreaseAlignOkCount(int nJigID, int nJigCh)
{
	
}

void CProductBank::IncreaseFirstContactCount(int nJigID, int nJigCh)
{
	// ���� �ð��� �ش��ϴ� �׸� ���� - LSH171217
	m_MapData[m_nCurHour].m_vetProductData[PRODUCT_COLUMN_FIRSTCONTACT][nJigID][nJigCh]++;
	m_MapData[m_nCurHour].m_nTotalData[PRODUCT_COLUMN_FIRSTCONTACT]++;
}

void CProductBank::IncreaseFinalContactCount(int nJigID, int nJigCh)
{
	// ���� �ð��� �ش��ϴ� �׸� ���� - LSH171217
	m_MapData[m_nCurHour].m_vetProductData[PRODUCT_COLUMN_FINALCONTACT][nJigID][nJigCh]++;
	m_MapData[m_nCurHour].m_nTotalData[PRODUCT_COLUMN_FINALCONTACT]++;
}