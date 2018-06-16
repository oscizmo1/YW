#pragma once
#include "label.h"


// CDlgContactor dialog

class CDlgContactor : public CDialog
{
	DECLARE_DYNAMIC(CDlgContactor)

public:
	CDlgContactor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgContactor();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTACTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString strContactor;

	CLabel m_lblContactor1;
	CLabel m_lblContactor2;
	CLabel m_lblContactor3;
	CLabel m_lblContactor4;
	DECLARE_EVENTSINK_MAP()
	void ClickLabelContactor1();
	void ClickLabelContactor2();
	void ClickLabelContactor3();
	void ClickLabelContactor4();
};
