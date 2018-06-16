#pragma once
#include "afxwin.h"
#include "lbl_blu.h"


// CUserSet 대화 상자입니다.

class CUserSet : public CDialog
{
	DECLARE_DYNAMIC(CUserSet)

public:
	CUserSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserSet();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_USER_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeDialogUserSetComboList();
	afx_msg void OnLbnSelchangeDialogUserSetList();
	afx_msg void OnBnClickedDialogUserSetButtonAdd();
	afx_msg void OnBnClickedDialogUserSetButtonDelete();
	afx_msg void OnBnClickedDialogUserSetButtonSave();

	CComboBox	m_cmbListLevel;
	CComboBox	m_cmbSelectedLevel;
	CListBox	m_listUser;
	CEdit		m_editName;
	CEdit		m_editPwd;

	USER_INFO	m_UserInfo;

	void		UserInfoLoad();
	void		UserInfoSave();

	void		EventWrite(CString strEvent,bool bPopup=false,bool bLog=false);
	
	afx_msg void OnBnClickedCancel();
	DECLARE_EVENTSINK_MAP()
	void OnClickLabelUserSelect();
	CLbl_blu m_lblUserSelect;
	CLbl_blu m_lblPassWordChange;
	CLbl_blu m_lblPassWord;
	afx_msg void OnBnClickedBtnChange();
};
