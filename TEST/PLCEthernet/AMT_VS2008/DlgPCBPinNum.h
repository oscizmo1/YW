#pragma once
#include "label.h"


// CDlgPCBPinNum dialog

class CDlgPCBPinNum : public CDialog
{
	DECLARE_DYNAMIC(CDlgPCBPinNum)

public:
	CDlgPCBPinNum(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPCBPinNum();

// Dialog Data
	enum { IDD = IDD_DIALOG_PCB_PIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString strPinNum;
	CLabel m_lbl68Pin;
	CLabel m_lbl80Pin;
	CLabel m_lbl96Pin;
	DECLARE_EVENTSINK_MAP()
	void ClickLabelPcb68pin();
	void ClickLabelPcb80pin();
	void ClickLabelPcb96pin();
};
