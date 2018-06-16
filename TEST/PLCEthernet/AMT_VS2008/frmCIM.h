#pragma once



// CFormCIM form view

class CFormCIM : public CFormView
{
	DECLARE_DYNCREATE(CFormCIM)

protected:
	virtual ~CFormCIM();

public:
	CFormCIM();           // protected constructor used by dynamic creation
	enum { IDD = IDD_FORM_CIM };
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

	CButton m_chkError[31];
	CButton m_chkHold[31];
	BOOL m_bError[31];
	BOOL m_bHold[31];

	DECLARE_EVENTSINK_MAP()
	void OnClickErrNumSet();
	void OnClickErrNumCnt();
	afx_msg void OnChkErr1();
	afx_msg void OnChkErr2();
	afx_msg void OnChkErr3();
	afx_msg void OnChkErr4();
	afx_msg void OnChkErr5();
	afx_msg void OnChkErr6();
	afx_msg void OnChkErr7();
	afx_msg void OnChkErr8();
	afx_msg void OnChkErr9();
	afx_msg void OnChkErr10();
	afx_msg void OnChkErr11();
	afx_msg void OnChkErr12();
	afx_msg void OnChkErr13();
	afx_msg void OnChkErr14();
	afx_msg void OnChkErr15();
	afx_msg void OnChkErr16();
	afx_msg void OnChkErr17();
	afx_msg void OnChkErr18();
	afx_msg void OnChkErr19();
	afx_msg void OnChkErr20();
	afx_msg void OnChkErr21();
	afx_msg void OnChkErr22();
	afx_msg void OnChkErr23();
	afx_msg void OnChkErr24();
	afx_msg void OnChkErr25();
	afx_msg void OnChkErr26();
	afx_msg void OnChkErr27();
	afx_msg void OnChkErr28();
	afx_msg void OnChkErr29();
	afx_msg void OnChkErr30();
	afx_msg void OnChkErr31();
	void SetDeffectCode(int nErr);
	afx_msg void OnChkHold1();
	afx_msg void OnChkHold2();
	afx_msg void OnChkHold3();
	afx_msg void OnChkHold4();
	afx_msg void OnChkHold5();
	afx_msg void OnChkHold6();
	afx_msg void OnChkHold7();
	afx_msg void OnChkHold8();
	afx_msg void OnChkHold9();
	afx_msg void OnChkHold10();
	afx_msg void OnChkHold11();
	afx_msg void OnChkHold12();
	afx_msg void OnChkHold13();
	afx_msg void OnChkHold14();
	afx_msg void OnChkHold15();
	afx_msg void OnChkHold16();
	afx_msg void OnChkHold17();
	afx_msg void OnChkHold18();
	afx_msg void OnChkHold19();
	afx_msg void OnChkHold20();
	afx_msg void OnChkHold21();
	afx_msg void OnChkHold22();
	afx_msg void OnChkHold23();
	afx_msg void OnChkHold24();
	afx_msg void OnChkHold25();
	afx_msg void OnChkHold26();
	afx_msg void OnChkHold27();
	afx_msg void OnChkHold28();
	afx_msg void OnChkHold29();
	afx_msg void OnChkHold30();
	afx_msg void OnChkHold31();
	void SetHoldCode(int nHold);
};


