#pragma once
#include "label.h"


// CDlgContactType dialog

class CDlgContactType : public CDialog
{
	DECLARE_DYNAMIC(CDlgContactType)

public:
	CDlgContactType(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgContactType();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTACT_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString strContactType;
	CLabel m_lblFFC;
	DECLARE_EVENTSINK_MAP()
	void ClickLabelFfc();
	void ClickLabelFpc();
	CLabel m_lblFPC;
	CLabel m_lblPin;
	virtual BOOL OnInitDialog();
	void ClickLabelPin();
};
