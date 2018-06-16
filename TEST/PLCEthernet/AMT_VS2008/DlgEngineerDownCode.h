//{{AFX_INCLUDES()
#include "msflexgrid_pos.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGENGINEERDOWNCODE_H__FE661706_FC67_413D_8169_8BAD6A794029__INCLUDED_)
#define AFX_DLGENGINEERDOWNCODE_H__FE661706_FC67_413D_8169_8BAD6A794029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEngineerDownCode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEngineerDownCode dialog

class CDlgEngineerDownCode : public CDialog
{
// Construction
public:
	CDlgEngineerDownCode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEngineerDownCode)
	enum { IDD = IDD_ENGINEER_DOWN_CODE };
	CMsflexgrid_pos	m_gridCode;
	int		m_nCode;
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEngineerDownCode)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadEngineerDownCode();
	void SaveEngineerDownCode();
	int m_nTotalNum;
	CIni m_ini;
	unsigned long m_nBackColor;

	// Generated message map functions
	//{{AFX_MSG(CDlgEngineerDownCode)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickCmdInsert();
	afx_msg void OnClickCmdDelete();
	afx_msg void OnClickCmdEdit();
	afx_msg void OnClickCmdSelect();
	afx_msg void OnClickMsflexgridCode();
	afx_msg void OnDblClickMsflexgridCode();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENGINEERDOWNCODE_H__FE661706_FC67_413D_8169_8BAD6A794029__INCLUDED_)
