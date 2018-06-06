#pragma once

#include "afxcmn.h"
#include "afxwin.h"


// CViewECM 폼 뷰입니다.

class CViewECM : public CFormView
{
	DECLARE_DYNCREATE(CViewECM)

protected:

public:
	CViewECM();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CViewECM();

	enum { IDD = IDD_VIEW_ECM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:

	CListCtrl m_ListECM;

	BOOL ReadECIDList();
	BOOL ReadECV();
	BOOL ViewECDip();
	CString MakeECString(CString str, BOOL bUpper);


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


	virtual void OnInitialUpdate();

public:
	afx_msg void OnBnClickedBtnEcmReload();
};


