#pragma once
#include "afxwin.h"
#include "lbl_blu.h"


// CUserChange 대화 상자입니다.

class CUserChange : public CDialog
{
	DECLARE_DYNAMIC(CUserChange)

public:
	CUserChange(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserChange();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_USER_CHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	// User Setting 재설정을 위하여 변수 추가  [3/30/2009 RomanticBoy]
	CString		m_strUserClass;

	int			m_nUserClass;
	
	USER_INFO	m_UserInfo;


	DECLARE_EVENTSINK_MAP()
	void OnClickLabelPasswordInsert();
	CLbl_blu m_lblPassWord;
	CLbl_blu m_lblUserClass;
	void OnClickLabelClassSelect();
};
