#pragma once
#include "ikeyboardx.h"
#include "ieditx.h"


// CDlgKeyBoard ��ȭ �����Դϴ�.

class CDlgKeyBoard : public CDialog
{
	DECLARE_DYNAMIC(CDlgKeyBoard)

public:
	CDlgKeyBoard(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgKeyBoard();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_KEYBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
