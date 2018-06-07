#pragma once

#include "resource.h"
#include <map>
#include "UI\GridCtrl\GridCtrl.h"
#include <vector>
using namespace std;
// CFormReportStatisticsNGLastResult 폼 뷰입니다.

class CFormReportStatisticsNGLastResult : public CFormView
{
	DECLARE_DYNCREATE(CFormReportStatisticsNGLastResult)

public:
	CFormReportStatisticsNGLastResult();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormReportStatisticsNGLastResult();
	int m_eIDX_Shift;// 확인할 작업자 상태
	CString m_sel_time;

public:
	enum { IDD = IDD_FORM_REPORT_STAICES_NG_LASTRESULT };
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
	//void SetHideLevelCHECK(BOOL bHide);

public:
	DECLARE_EVENTSINK_MAP()
	int sel_date;
	CTime time;
	alarm_current alarm_input;
	void ListUpdate_CH1();
	void ListUpdate_CH2();
	void ClearData();
	void UpdateLastResultGrid_CH1();
	void UpdateLastResultGrid_CH2();
	void NGListUdate(int sel);
	void ClickGxlblAlarmDay();
	void ClickGxlblAlarmNight();	
	void ClickGxlblAlarmPrev();
	void ClickGxlblAlarmNext();
	void ClickGxlblAlarmToday();
	void ClickGxlblAlarmTotal();	

	void ClickGxlblLastReultA();
	void ClickGxlblLastReultB();

	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	void ClickGxlblLastReultAzone();
	void ClickGxlblLastReultMTPWrite();
	void ClickGxlblLastReultMTPVerify();
	JIG_ID m_JigSelect;
	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	DEFECT_TYPE m_defectType;

	CGridCtrl m_NGResultStaticGrid_CH1;
	CGridCtrl m_NGResultStaticGrid_CH2;
	map<CString, int> m_mapLastResult_CH1;
	map<CString, int> m_mapLastResult_CH2;
};


