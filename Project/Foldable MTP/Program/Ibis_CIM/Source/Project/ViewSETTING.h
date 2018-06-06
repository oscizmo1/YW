#pragma once
#include "afxcmn.h"
//#include "ADOClass.h"
#include "afxwin.h"


// CViewSETTING form view

class CViewSETTING : public CFormView
{
	DECLARE_DYNCREATE(CViewSETTING)

//protected:
public:
	CViewSETTING();           // protected constructor used by dynamic creation
	virtual ~CViewSETTING();

public:
	enum { IDD = IDD_VIEW_SETTING };
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();

public:
	BOOL bEditable;
	CReportCtrl m_lstSetup;
	void GetSetup();




public:	
	afx_msg void OnBnClickedBtnSetupRead();
	afx_msg void OnBnClickedBtnSetupModify();
	afx_msg void OnBnClickedBtnSetupSave();
	
};


