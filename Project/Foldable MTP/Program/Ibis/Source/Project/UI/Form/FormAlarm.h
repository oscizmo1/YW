#pragma once

#include "resource.h"

#include "UI\GausGUI\GxImage.h"

// CFormAlarm1 �� ���Դϴ�.

class CFormAlarm : public CFormView
{
	DECLARE_DYNCREATE(CFormAlarm)

public:
	CFormAlarm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormAlarm();

public:
	enum { IDD = IDD_FORM_ALARM1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	


protected:

	CGxImage		m_imgUnit;									// ���ȭ�� �̹���

	void ViewAlarmMsg();


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ȭ�� ó���� ���� �޼��� �Լ�
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// ǥ�� �� ����Ǿ���.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	//kjpark 20161021 Setup ��ȣ�� UI ����
	void ClickBtnAlarm_Reset();
	void ClickBtnAlarm_Retry();
	void ClickBtnAlarm_Buzzer_Mute();
	DECLARE_EVENTSINK_MAP()
};


