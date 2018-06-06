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

	CString m_strRecipeName;		// ���� Read�Ǿ� �ִ� Recipe Name
	CRecipeCell m_Cell;	
	CRecipeTeach m_Teach;
	CRecipeOption m_Option;
	CRecipeInspModule m_Module;

	BOOL m_bModule_LoadSuccess;
	
	void ReadLastRecipeFile();	// ������ �ҷ��Դ� Recipe Read. m_strRecipeName�� �ڵ����� ������ �ҷ��� ������ �ٲ��
	void SaveLastRecipeName();	// ������ �ҷ��Դ� Recipe Read�� �����Ѵ�.
	void SetMainRecipeName(CString strName) { m_strRecipeName = strName; }
	void ReadRecipeFile(CString strRecipeName);	// Name���� Recipe�� ã�� �װ� �ҷ��´�
	void SaveMainRecipeFile();
	void SaveRecipeFile(CString strRecipeName);
	void SaveMainRecipeTeachFile();		// ƼĪ���� ���� �����ϴ� �Լ�. CTeachBank::SaveTeachFile���� ȣ���Ѵ�
	void SaveRMSDataFile();			// RMS ����� ���� ���� ���� [9/26/2017 OSC]
	void CreateRecipe_fromRMSData(CString strRecipeName);	// CIM �������� ������ ���� RMSData.ini�� ������ �ű� ���� ����� [9/27/2017 OSC]

	//kjpark 20171016 ������ ���� ���� ���� ǥ�� _VER** ���� �� �ݿ� 
	CString GetHigherVersionRecipe(CString strRecipe);
};

