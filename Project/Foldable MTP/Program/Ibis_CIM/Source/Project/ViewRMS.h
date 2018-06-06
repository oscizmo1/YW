#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CViewRMS form view

class CViewRMS : public CFormView
{
	DECLARE_DYNCREATE(CViewRMS)

	//protected:
public:
	CViewRMS();           // protected constructor used by dynamic creation
	virtual ~CViewRMS();

public:
	enum { IDD = IDD_VIEW_RMS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbModel;

	CListCtrl m_ListRecipe;


//	CListCtrl m_lstEC;
//	CListCtrl m_lstRecipe;


public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL GetModel();
	BOOL GetListLoadRecipeParam();


	BOOL EqModelChange(char* pPPID);
	BOOL EqModelSet(char* pFixPPID, char* pFixPPIDMODE);
	CString MakeParaString(CString str, BOOL bUpper);


public:
	afx_msg void OnCbnSelchangeComboModel();
	afx_msg void OnCbnDropdownComboModel();
	afx_msg void OnBnClickedBtnEcReload();


};


