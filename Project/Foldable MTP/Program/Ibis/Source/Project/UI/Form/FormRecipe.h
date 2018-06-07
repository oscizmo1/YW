#pragma once
#include "UI\GridCtrl\GridCtrl.h"
#include "UI\UIValue.h"

// CFormRecipe �� ���Դϴ�.

class CFormRecipe : public CFormView
{
	DECLARE_DYNCREATE(CFormRecipe)

public:
	CFormRecipe();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormRecipe();

public:
	enum { IDD = IDD_FORM_RECIPE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnTeach1Create();

protected:
	BOOL m_bInit;

	std::vector<CString> m_vecRecipeName;
	void RecipeName_InitUI();
	void RecipeName_to_Vector();
	void RecipeName_Update();
	void RecipeName_ResetSelection();	// �ƹ��͵� �������� ���� ���·� �Ѵ� [9/16/2017 OSC]

	void RecipeBank_LoadFile(int nSelectedItem);

	std::vector<CUIValue_Double> m_vecItemDouble;
	std::vector<CUIValue_BOOL> m_vecItemBOOL;
	void RecipeItem_InitValue();
	void RecipeItem_UIClear();
	void RecipeItem_Value_to_UI();
	void RecipeItem_UI_to_Value();
	void RecipeItem_EnableUI(BOOL flag);

	void InspModuleItem_Show();

	CGridCtrl m_GridModelList;
	CRecipeBank		m_RecipeBank;
	LOGFONT	m_lf;
	DWORD	m_dwTextStyle;
	int		m_iSelectedRow;
	int     m_iSetVelNumIndex;
	int		m_iUpDownParam;
	BOOL	m_bEnableUI;
	

protected:
	afx_msg void OnGridRecipeEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
	
public:
	void ClickGxbtnRecipeDelete();
	void ClickGxbtnRecipeMoveUp();
	void ClickGxbtnRecipeMoveDown();
	void ClickGxbtnRecipeEdit();
	void ClickGxbtnRecipeCellWidth();
	void ClickGxbtnRecipeCellHeight();
	void ClickGxbtnRecipeCellFPC();
	void ChangeOnOffValue( int nBtnID, int nIndex );
	void GetUI_to_OnOffValue( BOOL &bValue, int nBtnID, CString strName /*= _T("")*/ );
	void UpdateOnoffValue_to_UI(CString strOnOff, int nBtnID);
	void ClickGxbtnRecipeSave();
};


