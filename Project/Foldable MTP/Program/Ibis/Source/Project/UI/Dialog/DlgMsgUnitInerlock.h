#pragma once


// CDlgMsgUnitInerlock ��ȭ �����Դϴ�.

class CDlgMsgUnitInerlock : public CDialog
{
	DECLARE_DYNAMIC(CDlgMsgUnitInerlock)

public:
	CDlgMsgUnitInerlock(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMsgUnitInerlock();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MSG_UNITINTERLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickGxbtnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
