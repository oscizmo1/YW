#pragma once

// CDlgDoor ��ȭ �����Դϴ�.

class CDlgMCRManualRead : public CDialog
{
	DECLARE_DYNAMIC(CDlgMCRManualRead)

public:
	CDlgMCRManualRead(CCellInfo* pCell, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMCRManualRead();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MCR_READ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void ClickBtnMCRReadOK();	
	void ClickBtnMCRReadCancle();

	CCellInfo* m_pCell;
	CEdit pCellID;

};
