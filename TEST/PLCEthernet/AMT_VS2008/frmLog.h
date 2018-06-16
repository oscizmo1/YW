#pragma once



// CFormLog form view

class CFormLog : public CFormView
{
	DECLARE_DYNCREATE(CFormLog)

protected:
	virtual ~CFormLog();

public:
	CFormLog();           // protected constructor used by dynamic creation
	enum { IDD = IDD_FORM_LOG };
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
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
};


