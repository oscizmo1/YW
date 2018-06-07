#pragma once

#include "resource.h"
#include "MainFrm.h"
//kjpark 20161025 MCR 구현
#include "UI\GausGUI\GxStaticEx.h"
// CDlgCoRelation2 폼 뷰입니다.

class CDlgCoRelation : public CDialog
{
	DECLARE_DYNCREATE(CDlgCoRelation)

public:
	CDlgCoRelation(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCoRelation();
	virtual BOOL OnInitDialog();
public:
	enum { IDD = IDD_DLG_CORELATION};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 화면 처리에 대한 메세지 함수
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam, LPARAM lParam);			// 표시 언어가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
	void ClickBtnAxis_Y_Load_Jig_1();
	void ClickBtnAxis_Y_Load_Jig_2();
	void ClickBtnAxis_Y_Load_Jig_3();

	void ClickBtnAxis_Y_Inspection_Jig_1();	
	void ClickBtnAxis_Y_Inspection_Jig_2();	
	void ClickBtnAxis_Y_Inspection_Jig_3();	

	void ClickBtnAxis_Z_Up_Jig_1();
	void ClickBtnAxis_Z_Up_Jig_2();
	void ClickBtnAxis_Z_Up_Jig_3();

	void ClickBtnAxis_Z_Inspection_Jig_1();
	void ClickBtnAxis_Z_Inspection_Jig_2();
	void ClickBtnAxis_Z_Inspection_Jig_3();
	
	void ClickBtnCinfirm();
public:	
	BOOL m_bInit;//두번 실행되는 버그 방지

	//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
	UINT m_AxisList[3][4];
	void UpdateShuttleState();
		
	void initial();

	virtual void OnInitialUpdate();

#ifdef NOT_USED
	void ClickGxbtnTeachShuttle1VacCh1();
#endif
};


