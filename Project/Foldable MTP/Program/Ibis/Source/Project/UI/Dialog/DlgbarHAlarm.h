#pragma once
#include "Resource.h"



// CDlgbarHAlarm ��ȭ �����Դϴ�.

class CDlgbarHAlarm : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgbarHAlarm)

public:
	CDlgbarHAlarm();   // ǥ�� �������Դϴ�.
	virtual ~CDlgbarHAlarm();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_BAR_HALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// ����� ������ ȭ�鿡 ǥ���Ѵ�.
	void DispCurrentSelect(UINT nID);						// ���� ���õ� ��ư�� ���� �޸����ش�.

	void OnClickBtnxSubMenu1();
	void OnClickBtnxSubMenu2();
	void OnClickBtnxSubMenu3();
	void OnClickBtnxSubMenu4();

	// ������ ȭ������ ��ȯ�Ѵ�.
	void ChangeForm(int nIdx, BOOL bForce = FALSE);

	DECLARE_MESSAGE_MAP()

	DECLARE_EVENTSINK_MAP()

public:
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// ���� View ��ȯ �Ϸ� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
};
