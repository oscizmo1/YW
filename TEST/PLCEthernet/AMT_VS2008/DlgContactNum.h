#pragma once
#include "label.h"


// CDlgContactNum dialog

class CDlgContactNum : public CDialog
{
	DECLARE_DYNAMIC(CDlgContactNum)

public:
	CDlgContactNum(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgContactNum();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTACTNUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString strContactNum;

	CLabel m_lbl4ContactNum;
	CLabel m_lbl2ContactNum;
	CLabel m_lbl1ContactNum;
	DECLARE_EVENTSINK_MAP()
	void ClickLabelContact4();
	void ClickLabelContact2();
	void ClickLabelContact1();
};
