#pragma once

// #include "PublicClass/label.h"
// CDlgBarTitle dialog

class CDlgBarTitle : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarTitle)

public:
	CDlgBarTitle();   // standard constructor
	virtual ~CDlgBarTitle();

	// Dialog Data
	enum { IDD = IDD_BAR_TITLE };

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT HandleInitDialog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnEQConnect(WPARAM wParam, LPARAM lParam);					// �PC�� ���� 
	afx_msg LRESULT OnEQDisconnect(WPARAM wParam, LPARAM lParam);				// �PC�� ���� ����
	afx_msg LRESULT OnHostCommUpdate(WPARAM wParam, LPARAM lParam);			// HOST ��� ������ ���� ǥ��
	afx_msg LRESULT OnInfoUpdate(WPARAM wParam, LPARAM lParam);					// ���� ������ ����Ǿ���. ȭ�鿡 �ݿ��Ѵ�.

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:

	//CLabel m_lblStatus;	
	//CLabel m_lblDate;
	//CLabel m_lblTime;
	//CLabel m_lblEqpID;
	//CLabel m_lblSocketEqStatus;
	//CLabel m_lblSocket;
	//CLabel m_lblVersion;
	//
	//CLabel m_lblUserTitle;
	//CLabel m_lblUserTitle1;
	//CLabel m_lblUserTitle2;
	//CLabel m_lblUserID1;
	//CLabel m_lblUserID2;

	//void CreateThread();
	//void DeleteThread();
	//CWinThread* m_pTitleTimeThread;
	//HANDLE		m_hTitleTimeThread;
	//static UINT thrTitleTimeThread(LPVOID pParam);
	//BOOL m_bTitleTimeThreadRun;
	
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	virtual BOOL OnInitDialog();

	void DispInfo();						// ���� ������ �����ش�.

	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnTopClock();
};
