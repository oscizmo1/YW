#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgMelSecMonitor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CdlgMelSecMonitor dialog

class CdlgMelSecMonitor : public CDialog
{
// Construction
public:
	void DispUpdate();
	CdlgMelSecMonitor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CdlgMelSecMonitor)
	enum { IDD = IDD_DIALOG_MELSEC_MONITOR };
	int		m_StationNo;
	//}}AFX_DATA

	CButton m_Check0[16];
	CButton m_Check1[16];
	CButton m_Check2[16];
	CButton m_Check3[16];
	CButton m_Check4[16];
	CButton m_Check5[16];
	CButton m_Check6[16];
	void SetBitRtn(int s_ad, int e_ad);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdlgMelSecMonitor)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CdlgMelSecMonitor)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnCheck00 ();
	afx_msg void OnCheck01 ();
	afx_msg void OnCheck02 ();
	afx_msg void OnCheck03 ();
	afx_msg void OnCheck04 ();
	afx_msg void OnCheck05 ();
	afx_msg void OnCheck06 ();
	afx_msg void OnCheck07 ();
	afx_msg void OnCheck08 ();
	afx_msg void OnCheck09 ();
	afx_msg void OnCheck010();
	afx_msg void OnCheck011();
	afx_msg void OnCheck012();
	afx_msg void OnCheck013();
	afx_msg void OnCheck014();
	afx_msg void OnCheck015();
	afx_msg void OnCheck10 ();
	afx_msg void OnCheck11 ();
	afx_msg void OnCheck12 ();
	afx_msg void OnCheck13 ();
	afx_msg void OnCheck14 ();
	afx_msg void OnCheck15 ();
	afx_msg void OnCheck16 ();
	afx_msg void OnCheck17 ();
	afx_msg void OnCheck18 ();
	afx_msg void OnCheck19 ();
	afx_msg void OnCheck110();
	afx_msg void OnCheck111();
	afx_msg void OnCheck112();
	afx_msg void OnCheck113();
	afx_msg void OnCheck114();
	afx_msg void OnCheck115();
	afx_msg void OnCheck20 ();
	afx_msg void OnCheck21 ();
	afx_msg void OnCheck22 ();
	afx_msg void OnCheck23 ();
	afx_msg void OnCheck24 ();
	afx_msg void OnCheck25 ();
	afx_msg void OnCheck26 ();
	afx_msg void OnCheck27 ();
	afx_msg void OnCheck28 ();
	afx_msg void OnCheck29 ();
	afx_msg void OnCheck210();
	afx_msg void OnCheck211();
	afx_msg void OnCheck212();
	afx_msg void OnCheck213();
	afx_msg void OnCheck214();
	afx_msg void OnCheck215();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

