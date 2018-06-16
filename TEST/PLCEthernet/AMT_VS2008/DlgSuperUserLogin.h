#pragma once
#include "lbl_blu.h"


// CDlgSuperUserLogin dialog

class CDlgSuperUserLogin : public CDialog
{
	DECLARE_DYNAMIC(CDlgSuperUserLogin)

public:
	CDlgSuperUserLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSuperUserLogin();

// Dialog Data
	enum { IDD = IDD_DIALOG_SUPER_USER_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnClickLabelName();
	CLbl_blu m_lblName;
	CLbl_blu m_lblPassWord;
	void OnClickLabelPassword();
	afx_msg void OnBnClickedOk();
};
