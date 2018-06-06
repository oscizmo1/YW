#pragma once

#define MAX_MODULE_COUNT	10

class CRecipeInspModule
{
public:
	CRecipeInspModule(void);
	~CRecipeInspModule(void);

	std::vector<CString> m_vct_AZone_Bef;
	std::vector<CString> m_vct_AZone_Must;
	std::vector<CString> m_vct_AZone_Aft;

	std::vector<CString> m_vct_CZone_Bef;
	std::vector<CString> m_vct_CZone_Must;
	std::vector<CString> m_vct_CZone_Aft;


	BOOL ReadFile(CString strPath);		// 무결성 확인하기 위해 return을 BOOL형으로... [9/6/2017 OSC]
	void SaveFile(CString strPath);	

	CString VectorToString(std::vector<CString> *pVector);
};

