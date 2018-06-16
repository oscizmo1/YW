#pragma once
#include "afxwin.h"
#include "lbl_blu.h"


// CUserChange ��ȭ �����Դϴ�.

class CUserChange : public CDialog
{
	DECLARE_DYNAMIC(CUserChange)

public:
	CUserChange(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserChange();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_USER_CHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	// User Setting �缳���� ���Ͽ� ���� �߰�  [3/30/2009 RomanticBoy]
	CString		m_strUserClass;

	int			m_nUserClass;
	
	USER_INFO	m_UserInfo;


	DECLARE_EVENTSINK_MAP()
	void OnClickLabelPasswordInsert();
	CLbl_blu m_lblPassWord;
	CLbl_blu m_lblUserClass;
	void OnClickLabelClassSelect();
};
