#pragma once

#include "resource.h"
#include <map>

// CFormReport2 폼 뷰입니다.

class CFormReport2 : public CFormView
{
	DECLARE_DYNCREATE(CFormReport2)

public:
	CFormReport2();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormReport2();
	int m_eIDX_Shift;// 확인할 작업자 상태
	CString m_sel_time;
	map<int, ALARM_LOG_DATA> m_alarm_map;

	//20170620 bkh total span 시간값 추가
	CTimeSpan total_span;
public:
	enum { IDD = IDD_FORM_REPORT2 };
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

	BOOL m_bHideLevelCHECK;

public:
	DECLARE_EVENTSINK_MAP()
	int sel_date;
	CTime time;
	alarm_current alarm_input;
	void ListUpdate();
	void ClearData();
	void AlarmUdate(int sel);
	void ClickGxlblAlarmDay();
	void ClickGxlblAlarmSw();
	void ClickGxlblAlarmGy();
	void ClickGxlblAlarmPrev();
	void ClickGxlblAlarmNext();
	void ClickGxlblAlarmToday();
	void ClickGxlblAlarmTotal();
	//void ClickGxbtnHideCheck();
};


