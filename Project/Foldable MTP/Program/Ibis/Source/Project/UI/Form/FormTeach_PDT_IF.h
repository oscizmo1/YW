
#pragma once

#include "resource.h"
#include "UI\GridCtrl\GridCtrl.h"

// CFormSetup2 폼 뷰입니다.

class CFormTeach_PDT_IF : public CFormView
{
	DECLARE_DYNCREATE(CFormTeach_PDT_IF)

public:
	CFormTeach_PDT_IF();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeach_PDT_IF();

public:
	enum { IDD = IDD_FORM_TEACH_PDT_IF };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	BOOL bFirstInit;

public:
	CGridCtrl m_GridMTP_LB;
	CGridCtrl m_GridMTP_LW;
	CGridCtrl m_GridPDT_LB;
	CGridCtrl m_GridPDT_LW;
	CGridCtrl m_GridHotLine;

	void LBGrid_Init();
	void LBGrid_Refresh();

	void LWGrid_Init();
	void LWGrid_Refresh();

	void HotLineGrid_Init();
	void HotLineGrid_Refresh();

	void ShuttleBtn_Refresh();

protected:
	JIG_ID m_Jig;
	BOOL m_bEditMode;

public:
	DECLARE_EVENTSINK_MAP()
	virtual void OnInitialUpdate();

	afx_msg void OnGridClickMTP_LB(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClickMTP_LW(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClickHotLine(NMHDR *pNotifyStruct, LRESULT* pResult);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickGxbtnShuttle1();
	void ClickGxbtnShuttle2();
	void ClickGxbtnEditMode();
	void ClickGxbtnStateClear();
};


