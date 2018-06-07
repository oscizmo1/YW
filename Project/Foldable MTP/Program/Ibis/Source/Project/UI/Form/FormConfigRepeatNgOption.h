#pragma once
#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"



// CFormConfigNgOption �� ���Դϴ�.

class CFormConfigNgOption : public CFormView
{
	DECLARE_DYNCREATE(CFormConfigNgOption)

public:
	CFormConfigNgOption();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormConfigNgOption();

public:
	enum { IDD = IDD_FORM_CONFIG_REPEAT_NG_OPTION };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	BOOL bFirstInit;

public:
	void ChangeOnOffValue(int nBtnID);
	void NGGrid_Init();
	void SetColor(int clr);

	//void NGGrid_Refresh();

protected:

	BOOL		m_bEnableUI;


	CGridCtrl	m_Grid_Ng_List;
	int			m_iSetVelNumIndex;
	int			m_iSelectedRow;
	BOOL		m_bEditMode;

	afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);

public:
	DECLARE_EVENTSINK_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickGxbtnConfigNgAdd();
	void ClickGxbtnConfigNgDelete();
	void ClickGxbtnConfigNgOff();
	void ClickGxbtnConfigNgEdit();
	void ClickGxbtnConfigNgSave();
	void ClickGxbtnConfigNgRead();
};