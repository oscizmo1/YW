#pragma once
#include "ieditx.h"
#include "iphonepadx.h"


// CDlgNumPad 대화 상자입니다.

class CDlgNumPad : public CDialog
{
	DECLARE_DYNAMIC(CDlgNumPad)

public:
	CDlgNumPad(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgNumPad();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_KEYPAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
