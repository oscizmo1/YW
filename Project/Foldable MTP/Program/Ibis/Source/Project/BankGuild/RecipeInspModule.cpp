#include "StdAfx.h"
#include "RecipeInspModule.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRecipeInspModule::CRecipeInspModule(void)
{
}


CRecipeInspModule::~CRecipeInspModule(void)
{
}

BOOL CRecipeInspModule::ReadFile( CString strPath )
{
	m_vct_AZone_Bef.clear();
	m_vct_AZone_Must.clear();
	m_vct_AZone_Aft.clear();

	m_vct_CZone_Bef.clear();
	m_vct_CZone_Must.clear();
	m_vct_CZone_Aft.clear();

	// 필수 항목은 고정 [9/6/2017 OSC]
	m_vct_AZone_Must.push_back(INSP_MODULE_CELL_LOADING);
	m_vct_CZone_Must.push_back(INSP_MODULE_MTP_WRITE);
	m_vct_CZone_Must.push_back(INSP_MODULE_MTP_VERIFY);


	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strPath.GetString(), _T("r,ccs=UTF-8"));
	if (e != 0) return FALSE;
	CStdioFile		file(fStream);
	CString strBuf, strListName, strItem;
	int nCount, nIndex = 0;
	CTokenizer t;
	std::vector<CString> *pVector;
	int nTokenCnt;
	BOOL bRet = TRUE;

	while(file.ReadString(strBuf))
	{
		t = CTokenizer(strBuf);

		strListName = t.GetNextToken(_T("="));

		if(		strListName == _T("SET_ZONE_A_BEF"))
			pVector = &m_vct_AZone_Bef;
		else if(strListName == _T("SET_ZONE_A_AFT"))
			pVector = &m_vct_AZone_Aft;
		else if(strListName == _T("SET_ZONE_C_BEF"))
			pVector = &m_vct_CZone_Bef;
		else if(strListName == _T("SET_ZONE_C_AFT"))
			pVector = &m_vct_CZone_Aft;
		else
			continue;

		nCount = _tstoi(t.GetNextToken(_T(",")));
		nTokenCnt = t.GetTokenCount(_T(","));
		// 적힌 숫자와 실제 칸이 다르면 기록 미스로 간주한다 [10/8/2017 OSC]
		if( nCount != nTokenCnt)
			bRet = FALSE;

		for(int i = 0; i < nCount; i++)
		{
			strItem = t.GetNextToken(_T(","));
			strItem.TrimLeft(_T(" "));
			strItem.TrimRight(_T(" "));

			// 중간에 빈칸이 있으면 ERROR [9/6/2017 OSC]
			if(strItem.GetLength() <= 1)
				bRet = FALSE;

			if(bRet)
			{
				pVector->push_back(strItem);
			}
			else
			{
				pVector->push_back(INSP_MODULE_ERROR);
			}
		}
	}
	file.Close();
	return bRet;
}

void CRecipeInspModule::SaveFile( CString strPath )
{
	if(CFileSupport::FileCheck(strPath))
		DeleteFile(strPath);

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strPath.GetString(), _T("w,ccs=UTF-8"));
	if (e != 0) return;
	CStdioFile		file(fStream);
	CString strBuf, strListName;

	strListName = _T("SET_ZONE_A_BEF");
	strBuf.Format(_T("%s=%s\n"), strListName, VectorToString(&m_vct_AZone_Bef));
	file.WriteString(strBuf);

	strListName = _T("SET_ZONE_A_MUST");
	strBuf.Format(_T("%s=%s\n"), strListName, VectorToString(&m_vct_AZone_Must));
	file.WriteString(strBuf);

	strListName = _T("SET_ZONE_A_AFT");
	strBuf.Format(_T("%s=%s\n"), strListName, VectorToString(&m_vct_AZone_Aft));
	file.WriteString(strBuf);

	strListName = _T("SET_ZONE_C_BEF");
	strBuf.Format(_T("%s=%s\n"), strListName, VectorToString(&m_vct_CZone_Bef));
	file.WriteString(strBuf);

	strListName = _T("SET_ZONE_C_MUST");
	strBuf.Format(_T("%s=%s\n"), strListName, VectorToString(&m_vct_CZone_Must));
	file.WriteString(strBuf);

	strListName = _T("SET_ZONE_C_AFT");
	strBuf.Format(_T("%s=%s\n"), strListName, VectorToString(&m_vct_CZone_Aft));
	file.WriteString(strBuf);

	file.Close();
}

CString CRecipeInspModule::VectorToString( std::vector<CString> *pVector )
{
	CString strText;
	int nCount = pVector->size();
	strText.Format(_T("%d"), nCount);
	for(int i = 0; i < nCount; i++)
	{
		// ERROR인게 있으면 기록을 아지 않는다
		if (pVector->at(i) == INSP_MODULE_ERROR)
		{
			strText.Empty();
			break;
		}
		strText += _T(",") + pVector->at(i);
	}

	return strText;
}
