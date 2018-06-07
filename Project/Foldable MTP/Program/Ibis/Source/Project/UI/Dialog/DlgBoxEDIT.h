#pragma once
#include "Resource.h"
#include <afxbutton.h>
#include "afxwin.h"


// CDlgBoxEdit dialog

class CDlgBoxEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgBoxEdit)

public:
	CDlgBoxEdit(CWnd* pParent = NULL, CString strCaption=_T("Number Pad"), CString strValue=_T(""));   // standard constructor
	virtual ~CDlgBoxEdit();

// Dialog Data
	enum { IDD = IDD_DLG_BOX_NG_NAME_EDIT };

	CEdit				m_EditNum;
	CEdit				m_Count;

	CMFCButton			m_btnClear;
	CMFCButton			m_btnCancel;
	CMFCButton			m_btnEnter;
	
	CString 			m_strCaption;
	CString 			m_strValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
CFont	m_fontBold;

	DECLARE_MESSAGE_MAP()

public:
	
	CString m_strReturnText;
	CString m_strReturnCount;

public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClkClear();
	afx_msg void OnBnClkCancel();
	afx_msg void OnBnClkEnter();

	virtual BOOL OnInitDialog();
};
