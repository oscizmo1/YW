#pragma once
#include "label.h"


// CDlgPCBType dialog

class CDlgPCBType : public CDialog
{
	DECLARE_DYNAMIC(CDlgPCBType)

public:
	CDlgPCBType(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPCBType();

// Dialog Data
	enum { IDD = IDD_DIALOG_PCB_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString strPCBType;

	CLabel m_lblNormal;
	CLabel m_lblSNB;
	CLabel m_lblLED;
	DECLARE_EVENTSINK_MAP()
	void ClickLabelNormal();
	void ClickLabelSnb();
	void ClickLabelLed();
};
