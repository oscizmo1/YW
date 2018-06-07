#pragma once

#include "resource.h"
#include "MainFrm.h"
//kjpark 20161025 MCR 구현
#include "UI\GausGUI\GxImage.h"
#include "UI\GausGUI\GxStaticEx.h"
// CFormTeach2 폼 뷰입니다.

class CFormTeach : public CFormView
{
	DECLARE_DYNCREATE(CFormTeach)

public:
	CFormTeach();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormTeach();

public:
	enum { IDD = IDD_FORM_TEACH};
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
	afx_msg LRESULT OnUpdateMCRData(WPARAM wParam, LPARAM lParam);				// MCR Data가 수신되었다.
	afx_msg LRESULT OnUpdateMCRLive(WPARAM wParam, LPARAM lParam);				// MCR Live 영상 수신
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	

	enum { CELLTR_CH1 = 0, CELLTR_CH2, CELLTR_CH3 };

protected:
public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateCellTRState();
	
	void ClickBtnBufStg();
	void ClickBtnMCRCh3();
	void ClickBtnMCRCh2();
	void ClickBtnMCRCh1();
	void ClickBtnPreAlign();
	//kjpark 20161025 MCR 구현
	CGxImage			m_imgLive;
	
public:
	//kjpark 20161025 MCR 구현
	BOOL m_bShow;
	BOOL m_bLiveMode;
	AXIS_ID			m_CurAxis;
	TEACH_PARAM::ID	m_CurTeach;
	UINT			m_CurID;
	UINT m_AxisList[4];
	BOOL m_bInit;//두번 실행되는 버그 방지
	void ClickBtnMCRRead();
	//void ClickBtnReConnect();
	void ClickBtnLiveMode();
	void SetMCRLive();
	void SetMCRData(CString strBarcode = _T(""));
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID);
	void SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID, int nChaenl);
	virtual void OnInitialUpdate();
};


