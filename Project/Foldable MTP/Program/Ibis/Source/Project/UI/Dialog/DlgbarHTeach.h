﻿#pragma once

#include "resource.h"

// CDlgbarHTeach 대화 상자입니다.

class CDlgbarHTeach : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgbarHTeach)

public:
	CDlgbarHTeach();   // 표준 생성자입니다.
	virtual ~CDlgbarHTeach();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_BAR_HTEACH1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 지정한 화면으로 변환한다.
	void ChangeForm(int nIdx, BOOL bForce = FALSE);

	// 변경된 내용을 화면에 표시한다.
	void DispCurrentSelect(UINT nID);						// 현재 선택된 버튼의 색을 달리해준다.


	DECLARE_MESSAGE_MAP()
	// afx_msg void OnBtnClickMenu1();
	afx_msg void OnUpdateCmdUI(CCmdUI* pCmdUI);

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeViewEnd(WPARAM wParam, LPARAM lParam);				// 메인 View 변환 완료 수신
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

public:

	DECLARE_EVENTSINK_MAP()
	//kjpark 20170902 티칭 UI 추가 및 상단탭 동작 
	void ClickBtnxShuttle_1();
	void ClickBtnxShuttle_2();
	void ClickBtnxShuttle_3();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void ClickBtnxHteachPdtIf();
};
