#pragma once

#include "resource.h"
#include "MainFrm.h"
//kjpark 20161025 MCR 구현
#include "UI\GausGUI\GxImage.h"
#include "UI\GausGUI\GxStaticEx.h"
// CFormTeach_Shuttle_12 폼 뷰입니다.

class CFormTeach_Shuttle_1 : public CFormView
{
	DECLARE_DYNCREATE(CFormTeach_Shuttle_1)

public:
	CFormTeach_Shuttle_1();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeach_Shuttle_1();

public:
	enum { IDD = IDD_FORM_TEACH_SHUTTLE_1};
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
	//kjpark 20161025 MCR 구현
	afx_msg LRESULT OnUpdateMCRData_CH1(WPARAM wParam, LPARAM lParam);				// MCR Data가 수신되었다.
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
protected:
public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
	void ClickBtnMCRRead_CH1();	
	
	//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
	void ClickBtnAxis_Y_Load();
	void ClickBtnAxis_Y_MANUAL();
	void ClickBtnAxis_Y_Inspection();
	void ClickBtnAxis_X_Inspection();
	void ClickBtnAxis_Z_Up();
	void ClickBtnAxis_Z_Inspection();

	void ClickGxbtnTeachShuttle1ActiveALIGNLeftX();
	void ClickGxbtnTeachShuttle1ActiveALIGNRightX();
	
public:	
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;
	BOOL m_bInit;//두번 실행되는 버그 방지
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);


	CGxImage m_imgLive_CH1;
	BOOL m_bShow_CH1;
	BOOL m_bLiveMode_CH1;

	CGxImage m_imgLive_CH2;
	BOOL m_bShow_CH2;	
	BOOL m_bLiveMode_CH2;

	void SetMCRLive_CH1();
	void SetMCRData_CH1(CString strBarcode = _T(""));

	//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
	UINT m_AxisList[8];
	void Get_Check();
	void UpdateShuttleState();
	
	JIG_ID m_thisDlgShuttle;
	virtual void OnInitialUpdate();
	void ClickGxbtnTeachShuttle1VacCh1();
	void ClickGxbtnTeachShuttle1BlowCh1();
	void ClickGxbtnTeachShuttle1FpcbVacCh1();
	void ClickGxbtnTeachShuttle1FpcbBlowCh1();
	void ClickGxbtnTeachShuttle1Down();
	void ClickGxbtnTeachShuttle1Up();
};


