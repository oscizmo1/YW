#pragma once
#include "ikeyboardx.h"
#include "ieditx.h"


// CDlgKeyBoard 대화 상자입니다.

class CDlgKeyBoard : public CDialog
{
	DECLARE_DYNAMIC(CDlgKeyBoard)

public:
	CDlgKeyBoard(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgKeyBoard();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_KEYBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	BOOL m_bPasswordType;
	CString m_strEditText;

	CIkeyboardx m_KeyBoardCtrl;
	afx_msg void OnCancel();
	afx_msg void OnDataEnter();
	virtual BOOL OnInitDialog();
	CIeditx m_EditKeyboardCtrl;
	void SetPasswordType();
};
