#pragma once



// CViewBase form view

class CViewBase : public CFormView
{
	DECLARE_DYNCREATE(CViewBase)

protected:
	CViewBase();           // protected constructor used by dynamic creation
	virtual ~CViewBase();

public:
	enum { IDD = IDD_VIEW_BASE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


