#include "StdAfx.h"
#include "InspModuleBank.h"


CInspModuleBank::CInspModuleBank(void)
{
}


CInspModuleBank::~CInspModuleBank(void)
{
}

CInspModule_Base* CInspModuleBank::GetModule( CString strModuleName, JIG_ID jig )
{
	if(strModuleName == INSP_MODULE_CELL_LOADING)
		return &m_ModuleCellLoading[jig];
	else if(strModuleName == INSP_MODULE_MTP_WRITE)
		return &m_ModuleMTPWrite[jig];
	else if(strModuleName == INSP_MODULE_MTP_VERIFY)
		return &m_ModuleMTPVerify[jig];
	else if(strModuleName == INSP_MODULE_WHITE_CURRENT)
		return &m_ModuleWhiteCurrent[jig];
	else if(strModuleName == INSP_MODULE_SLEEP_CURRENT)
		return &m_ModulesleepCurrent[jig];
	else if(strModuleName == INSP_MODULE_HLPM_CURRENT)
		return &m_ModuleHlpmCurrent[jig];
	else if(strModuleName == INSP_MODULE_TSP_START)
		return &m_ModuleTspStart[jig];
	else if(strModuleName == INSP_MODULE_EVT_VERSION_CHECK)
		return &m_ModuleEvtVersionCheck[jig];
	else if(strModuleName == INSP_MODULE_TE_CHECK)
		return &m_ModuleTeCheck[jig];
	else if(strModuleName == INSP_MODULE_ID_CHECK)
		return &m_ModuleIdCheck[jig];
	else if(strModuleName == INSP_MODULE_OTP_REG_CHECK)
		return &m_ModuleOtpRegCheck[jig];
	else if(strModuleName == INSP_MODULE_ICT_TEST)
		return &m_ModuleIctTest[jig];
	else if(strModuleName == INSP_MODULE_COPR_ICT_TEST)
		return &m_ModuleCoprIctTest[jig];
	else if(strModuleName == INSP_MODULE_POC_ERROR_CHECK)
		return &m_ModulePocErrorCheck[jig];
	else if(strModuleName == INSP_MODULE_DDI_BLOCK_TEST)
		return &m_ModuleDDiBlockTest[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK2)
		return &m_ModuleOptionCheck2[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK3)
		return &m_ModuleOptionCheck3[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK4)
		return &m_ModuleOptionCheck4[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK5)
		return &m_ModuleOptionCheck5[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK6)
		return &m_ModuleOptionCheck6[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK7)
		return &m_ModuleOptionCheck7[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK8)
		return &m_ModuleOptionCheck8[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK9)
		return &m_ModuleOptionCheck9[jig];
	else if(strModuleName == INSP_MODULE_OPTION_CHECK10)
		return &m_ModuleOptionCheck10[jig];

	return NULL;
}

int CInspModuleBank::FindModule( std::vector<CString> *pVector, CString strModuleName )
{
	int nCount = pVector->size();
	for(int i = 0; i < nCount; i++)
	{
		if(strModuleName == pVector->at(i))
			return i;
	}
	return -1;
}

CInspModule_Base* CInspModuleBank::GetNextModule( std::vector<CString> *pVector, CString strModuleName, JIG_ID jig )
{
	// 주어진 모듈이름 다음에 이어서 할 모듈을 반환하는 함수 [8/10/2017 OSC]
	CString strNextModuleName;

	// INSP_MODULE_NONE이면 처음 시작이다 [8/10/2017 OSC]
	if(strModuleName == INSP_MODULE_NONE)
	{
		if(pVector->size())
			strNextModuleName = pVector->at(0);
	}
	else
	{
		BOOL bNext = FALSE;

		// 이전꺼 위치를 먼저 찾은 후 다음꺼 Name을 얻어온다 [8/10/2017 OSC]
		std::vector<CString>::iterator it = pVector->begin();
		for(it; it != pVector->end(); it++ )
		{
			if(bNext)
			{
				strNextModuleName = it->GetString();
				break;
			}
			else
			{
				if(it->GetString() == strModuleName)
				{
					bNext = TRUE;
				}
			}
		}
	}

	// 모듈 이름으로 모듈 포인터를 찾아 반환한다 [8/10/2017 OSC]
	if(strNextModuleName.IsEmpty() == FALSE)
		return GetModule(strNextModuleName, jig);
	else
		return NULL;
}

CInspModule_Base* CInspModuleBank::GetNextModule_AZone_Bef( CString strModuleName, JIG_ID jig )
{
	return GetNextModule(&theRecipeBank.m_Module.m_vct_AZone_Bef, strModuleName, jig);
}

CInspModule_Base* CInspModuleBank::GetNextModule_AZone_Must( CString strModuleName, JIG_ID jig )
{
	return GetNextModule(&theRecipeBank.m_Module.m_vct_AZone_Must, strModuleName, jig);
}

CInspModule_Base* CInspModuleBank::GetNextModule_AZone_Aft( CString strModuleName, JIG_ID jig )
{
	return GetNextModule(&theRecipeBank.m_Module.m_vct_AZone_Aft, strModuleName, jig);
}

CInspModule_Base* CInspModuleBank::GetNextModule_BZone_Bef( CString strModuleName, JIG_ID jig )
{
	return GetNextModule(&theRecipeBank.m_Module.m_vct_BZone_Bef, strModuleName, jig);
}

CInspModule_Base* CInspModuleBank::GetNextModule_BZone_Must( CString strModuleName, JIG_ID jig )
{
	return GetNextModule(&theRecipeBank.m_Module.m_vct_BZone_Must, strModuleName, jig);
}

CInspModule_Base* CInspModuleBank::GetNextModule_BZone_Aft( CString strModuleName, JIG_ID jig )
{
	return GetNextModule(&theRecipeBank.m_Module.m_vct_BZone_Aft, strModuleName, jig);
}

CString CInspModuleBank::FindWorkZone( CString strModuleName )
{
	CString strZone;
	int nTotalIndex = 0;

	//////////////////////////////////////////////////////////////////////////
	// A Zone
	int nIndex = FindModule(&theRecipeBank.m_Module.m_vct_AZone_Bef, strModuleName);
	if(nIndex != -1)
	{
		nTotalIndex += nIndex;
		strZone.Format(_T("A_BEF_%d"), nTotalIndex+1);
		return strZone;
	}
	else
	{
		nIndex = theRecipeBank.m_Module.m_vct_AZone_Bef.size();
	}
	nIndex = FindModule(&theRecipeBank.m_Module.m_vct_AZone_Must, strModuleName);
	if(nIndex != -1)
	{
		nTotalIndex += nIndex;
		strZone.Format(_T("A_MUST_%d"), nTotalIndex+1);
		return strZone;
	}
	else
	{
		nIndex = theRecipeBank.m_Module.m_vct_AZone_Must.size();
	}
	nIndex = FindModule(&theRecipeBank.m_Module.m_vct_AZone_Aft, strModuleName);
	if(nIndex != -1)
	{
		nTotalIndex += nIndex;
		strZone.Format(_T("A_AFT_%d"), nTotalIndex+1);
		return strZone;
	}
	else
	{
		nIndex = theRecipeBank.m_Module.m_vct_AZone_Aft.size();
	}
	//////////////////////////////////////////////////////////////////////////
	// C Zone
	nIndex = FindModule(&theRecipeBank.m_Module.m_vct_BZone_Bef, strModuleName);
	if(nIndex != -1)
	{
		nTotalIndex += nIndex;
		strZone.Format(_T("C_BEF_%d"), nTotalIndex+1);
		return strZone;
	}
	else
	{
		nIndex = theRecipeBank.m_Module.m_vct_BZone_Bef.size();
	}
	nIndex = FindModule(&theRecipeBank.m_Module.m_vct_BZone_Must, strModuleName);
	if(nIndex != -1)
	{
		nTotalIndex += nIndex;
		strZone.Format(_T("C_MUST_%d"), nTotalIndex+1);
		return strZone;
	}
	else
	{
		nIndex = theRecipeBank.m_Module.m_vct_BZone_Must.size();
	}
	nIndex = FindModule(&theRecipeBank.m_Module.m_vct_BZone_Aft, strModuleName);
	if(nIndex != -1)
	{
		nTotalIndex += nIndex;
		strZone.Format(_T("C_AFT_%d"), nTotalIndex+1);
		return strZone;
	}
	else
	{
		nIndex = theRecipeBank.m_Module.m_vct_BZone_Aft.size();
	}
	//////////////////////////////////////////////////////////////////////////

	return _T("NOT");
}
