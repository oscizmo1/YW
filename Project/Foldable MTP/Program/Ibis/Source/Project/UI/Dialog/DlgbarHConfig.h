#pragma once
#include "resource.h"

// CDlgbarHConfig ��ȭ �����Դϴ�.

class CDlgbarHConfig : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgbarHConfig)

public:
	CDlgbarHConfig();   // ǥ�� �������Դϴ�.
	virtual ~CDlgbarHConfig();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_BAR_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// ������ ȭ������ ��ȯ�Ѵ�.
	void ChangeForm(int nIdx, BOOL bForce = FALSE);

	// ����� ������ ȭ�鿡 ǥ���Ѵ�.
	void DispCurrentSelect(UINT nID);						// ���� ���õ� ��ư�� ���� �޸����ش�.

	// afx_msg void OnBtnClickMenu1();
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// ���� View ��ȯ �Ϸ� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnClickBtnxSubMenu1();
	void OnClickBtnxSubMenu2();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void ClickBtnxHconfigCim();
	void ClickBtnxHconfigOptions();
	
	void ClickBtnxHconfigNgOption();
};

