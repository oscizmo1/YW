#pragma once

// CDlgDoor 대화 상자입니다.

class CDlgMCRManualRead : public CDialog
{
	DECLARE_DYNAMIC(CDlgMCRManualRead)

public:
	CDlgMCRManualRead(CCellInfo* pCell, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMCRManualRead();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MCR_READ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
