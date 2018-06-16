#pragma once
#include "ieditx.h"
#include "iphonepadx.h"


// CDlgNumPad ��ȭ �����Դϴ�.

class CDlgNumPad : public CDialog
{
	DECLARE_DYNAMIC(CDlgNumPad)

public:
	CDlgNumPad(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgNumPad();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_KEYPAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPasswordType;
	CString m_strEditText;
	CIeditx m_IeditNum;
	CIphonepadx m_NumPadCtrl;
	void OnKeyClickIphonepad(long KeyCode, short AChar);
	afx_msg void OnEditDataClear();
	afx_msg void OnDataEnterCancel();
	afx_msg void OnEditDataEnter();

	DECLARE_EVENTSINK_MAP()
	void OnKeyClickNumPad(long KeyCode, short AChar);

public:
	void SetPasswordType();
	virtual BOOL OnInitDialog();
};
