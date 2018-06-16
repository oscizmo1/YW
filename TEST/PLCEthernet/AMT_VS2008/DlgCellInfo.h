#pragma once
#include "label.h"


// CDlgCellInfo dialog

class CDlgCellInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgCellInfo)

public:
	CDlgCellInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCellInfo();

// Dialog Data
	enum { IDD = IDD_DIALOG_CELL_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CLabel m_lblHPanelID;
	CLabel m_lblBatchID;
	CLabel m_lblDeviceID;
	CLabel m_lblPairHID;
	CLabel m_lblPairEID;
	CLabel m_lblInsFlag;
	CLabel m_lblJudgement;
	CLabel m_lblProdKind;
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickLabelHPanelId();
	void ClickLabelBatchId();
	void ClickLabelDeviceId();
	void ClickLabelPairHId();
	void ClickLabelPairEId();
	void ClickLabelInspFlag();
	void ClickLabelJudgement();
	void ClickLabelProdKind();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
