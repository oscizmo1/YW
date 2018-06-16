#pragma once

//#include "PublicClass/BtnST.h"

// CDlgBarMenu dialog

class CDlgBarMenu : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarMenu)

public:
	CDlgBarMenu();   // standard constructor
	virtual ~CDlgBarMenu();

	// Dialog Data
	enum { IDD = IDD_BAR_MENU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	//CButtonST m_btnCIM
	//	,m_btnFDC
	//	,m_btnCell
	//	,m_btnTact
	//	,m_btnAlarm
	//	,m_btnAlign
	//	,m_btnRMS
	//	,m_btnSetting
	//	,m_btnExit;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnMenu1();
	void ClickGxbtnMenu2();
	void ClickGxbtnMenu3();
	void ClickGxbtnMenu4();
	void ClickGxbtnMenu5();
	void ClickGxbtnMenu6();
};
