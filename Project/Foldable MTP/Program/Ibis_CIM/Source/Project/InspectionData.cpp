#include "StdAfx.h"
#include "GaonNuri.h"
#include "InspectionData.h"


CInspectionData::CInspectionData(void)
{
	InitEvent();
}

CInspectionData::~CInspectionData(void)
{
	
}

CString CInspectionData::MakeString(CString str)
{	
	int nTemp;
	nTemp = -1;

	if(str.Find(",")>=0)
	{
		nTemp = str.Find(",");
		str.Delete(nTemp);
	}
	else if(str.Find("mA")>=0)
	{
		nTemp = str.Find("mA");
		str = str.Mid(0, nTemp);
	}
	str.Trim(' ');
	return str;
}

CString CInspectionData::MakeString(CString str, int nLen)
{	
	int nTemp;
	nTemp = -1;

	if(str.Find(",")>=0)
	{
		nTemp = str.Find(",");
		str.Delete(nTemp);
	}
	else if(str.Find("mA")>=0)
	{
		nTemp = str.Find("mA");
		str = str.Mid(0, nTemp);
	}
	str.Trim(' ');

	if (str.GetLength() > nLen-1)
	{
		str = str.Mid(0, nLen-1);
	}
	return str;
}

int CInspectionData::GetChannelNo(UINT uJig, UINT uCellPos)
{
	int nJig=-1;
	int nPos=-1;
	int nChNo=-1;


	switch (uJig)
	{
	case JIG_1:
		nJig = 2;
		break;
	case JIG_2:
		nJig = 4;
		break;
	}

	switch (uCellPos)
	{
	case E_CELL_POS_1:
		nPos = 1;
		break;
	case E_CELL_POS_2:
		nPos = 0;
		break;
	}


	if (nJig>-1 && nPos >-1)
		nChNo = nJig - nPos;

	return nChNo;
}

CString CInspectionData::GetChannelString(UINT uJig, UINT uCellPos)
{
	int nJig=-1;
	int nPos=-1;
	int nChNo=-1;
	CString strRtn;


	switch (uJig)
	{
	case JIG_1:
		nJig = 2;
		break;
	case JIG_2:
		nJig = 4;
		break;
	}

	switch (uCellPos)
	{
	case E_CELL_POS_1:
		nPos = 1;
		break;
	case E_CELL_POS_2:
		nPos = 0;
		break;
	}


	if (nJig>-1 && nPos >-1)
		nChNo = nJig - nPos;

	strRtn.Format("PATTERN_GENERATOR%d", nChNo);	//sms strRtn.Format("PG %d", nChNo);

	return strRtn;
}

BOOL CInspectionData::LoadVTInformIni()
{
	CString strFullPath;
	CString strTemp;
	CFileFind finder;
	char	cUserLog[USER_LOG_LEN];

	strFullPath = CSettingParm::cVT1_INFO_PATH;
	BOOL bExist = finder.FindFile(strFullPath);

	if (!bExist)
	{
		sprintf_s(cUserLog, "LoadVTInformIni - ERROR : File not found(%s)", strFullPath);
		::SendMessage(GetSockStsDlg()->m_hWnd, WM_USER_MSG, (WPARAM)LOG_INSPECT, (LPARAM)&cUserLog);
		return FALSE;
	}

	CIni ini(strFullPath);
	int nResolutionX = ini.GetLong("MODEL INFO", "X Resolution", 1);
	int nResolutionY = ini.GetLong("MODEL INFO", "Y Resolution", 1);
	int nDot = ini.GetLong(_T("MODEL INFO"), "ETC::Dot Count", 11);
	int nDotCountX = nDot/10;
	int nDotCountY = nDot%10;
	int nOrigin = ini.GetLong("MODEL INFO", "CELL ORIGIN", 0);
	int nCellX = nResolutionX * nDotCountX;
	int nCellY = nResolutionY * nDotCountY;

	strTemp.Format("%d", nCellX);	//CIM
	strcpy_s(g_pData->m_cInitCellSizeX, strTemp);	//CIM
	strTemp.Format("%d", nCellY);	//CIM
	strcpy_s(g_pData->m_cInitCellSizeY, strTemp);	//CIM
	strTemp.Format("%d", nOrigin);
	strcpy_s(g_pData->m_cInitOrigin, strTemp);		//CIM

	return TRUE;
}

BOOL CInspectionData::CellIdCheckUp(char* pCellId)
{
	//CELLID 사이즈, 문자열 검사
	if(strlen(pCellId)>=16)
		return TRUE;
	else
		return FALSE;
}

BOOL CInspectionData::CellIdCheckUp(CString strCellId)
{
	//CELLID 사이즈, 문자열 검사
	if(strCellId.GetLength()>=16)
		return TRUE;
	else
		return FALSE;
}
BOOL CInspectionData::InitEvent()
{
	for (int i=0; i<MAX_JIG_CNT; i++)
	{
		for (int j=0; j<MAX_JIG_CELL_CNT; j++)
		{
			bIctEvent[i][j] = FALSE;
			bInfoEvent[i][j] = FALSE;
			bPwrEvent[i][j] = FALSE;
			bMtpWEvent[i][j] = FALSE;
			bTspEvent[i][j] = FALSE;
			bMtpREvent[i][j] = FALSE;
			bVtEvent[i][j] = FALSE;
		}
	}

	return TRUE;
}

//BOOL CInspectionData::SetYmsRtn(UINT uJig, UINT uCellPos, E_RESULT_FILE_TYPE eRType, E_YMS_RETURN eYmsRtn)
BOOL CInspectionData::SetQMSRtn(UINT uJig, UINT uCellPos, E_RESULT_FILE_TYPE eRType, E_QMS_RETURN eQMSRtn)
{
	tag_TestResult* pResult = &g_pData->m_Jig[uJig].tCellData[uCellPos].m_tTestResult;
	BOOL bRtn = TRUE;
	switch (eRType)
	{
	case E_CELL:
		//pResult->m_eCellYmsRtn = eYmsRtn;
		pResult->m_eCellQMSRtn = eQMSRtn;
		break;

	case E_MTPW:
		//pResult->m_eMtpWYmsRtn = eYmsRtn;
		pResult->m_eMtpWQMSRtn = eQMSRtn;
		break;

	case E_TSP:
		//pResult->m_eTspYmsRtn = eYmsRtn;
		pResult->m_eTspQMSRtn = eQMSRtn;
		break;

	case E_MTPR:
		//pResult->m_eMtpRYmsRtn = eYmsRtn;
		pResult->m_eMtpRQMSRtn = eQMSRtn;
		break;

	default:
		bRtn = FALSE;
		break;
	}

	return bRtn;
}

//E_YMS_RETURN CInspectionData::GetYmsRtn(UINT uJig, UINT uCellPos, E_RESULT_FILE_TYPE eRType)
E_QMS_RETURN CInspectionData::GetQMSRtn(UINT uJig, UINT uCellPos, E_RESULT_FILE_TYPE eRType)
{
	tag_TestResult* pResult = &g_pData->m_Jig[uJig].tCellData[uCellPos].m_tTestResult;
	//E_YMS_RETURN eYmsRtn;
	E_QMS_RETURN eQMSRtn;

	switch (eRType)
	{
	case E_CELL:
		//eYmsRtn = pResult->m_eCellYmsRtn;
		eQMSRtn = pResult->m_eCellQMSRtn;
		break;

	case E_MTPW:
		//eYmsRtn = pResult->m_eMtpWYmsRtn;
		eQMSRtn = pResult->m_eMtpWQMSRtn;
		break;

	case E_TSP:
		//eYmsRtn = pResult->m_eTspYmsRtn;
		eQMSRtn = pResult->m_eTspQMSRtn;
		break;

	case E_MTPR:
		//eYmsRtn = pResult->m_eMtpRYmsRtn;
		eQMSRtn = pResult->m_eMtpRQMSRtn;
		break;

	default:
		//eYmsRtn = E_YMS_NONE;
		eQMSRtn = E_QMS_NONE;
		break;
	}

	//return eYmsRtn;
	return eQMSRtn;
}

