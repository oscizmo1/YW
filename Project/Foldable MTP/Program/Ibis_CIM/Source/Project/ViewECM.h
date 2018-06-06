#pragma once

#include "afxcmn.h"
#include "afxwin.h"


// CViewECM �� ���Դϴ�.

class CViewECM : public CFormView
{
	DECLARE_DYNCREATE(CViewECM)

protected:

public:
	CViewECM();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


	virtual void OnInitialUpdate();

public:
	afx_msg void OnBnClickedBtnEcmReload();
};


