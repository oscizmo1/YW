#pragma once

#include "RecipeCell.h"
#include "RecipeTeach.h"
#include "RecipeOption.h"
#include "RecipeInspModule.h"

class CRecipeBank
{
public:
	CRecipeBank(void);
	~CRecipeBank(void);

	CString m_strRecipeName;		// 현재 Read되어 있는 Recipe Name
	CRecipeCell m_Cell;	
	CRecipeTeach m_Teach;
	CRecipeOption m_Option;
	CRecipeInspModule m_Module;

	BOOL m_bModule_LoadSuccess;
	
	void ReadLastRecipeFile();	// 마지막 불러왔던 Recipe Read. m_strRecipeName가 자동으로 마지막 불러온 것으로 바뀐다
	void SaveLastRecipeName();	// 마지막 불러왔던 Recipe Read을 저장한다.
	void SetMainRecipeName(CString strName) { m_strRecipeName = strName; }
	void ReadRecipeFile(CString strRecipeName);	// Name으로 Recipe를 찾아 그걸 불러온다
	void SaveMainRecipeFile();
	void SaveRecipeFile(CString strRecipeName);
	void SaveMainRecipeTeachFile();		// 티칭값만 따로 저장하는 함수. CTeachBank::SaveTeachFile에서 호출한다
	void SaveRMSDataFile();			// RMS 보고용 파일 별도 생성 [9/26/2017 OSC]
	void CreateRecipe_fromRMSData(CString strRecipeName);	// CIM 상위에서 역으로 만든 RMSData.ini를 가지고 신규 모델을 만든다 [9/27/2017 OSC]

	//kjpark 20171016 레시피 파일 끝에 버전 표기 _VER** 높은 수 반영 
	CString GetHigherVersionRecipe(CString strRecipe);
};

