#pragma once

#include "resource.h"
#include <vector>

using namespace std;

// CFormReport1 폼 뷰입니다.

class CFormReport1 : public CFormView
{
	DECLARE_DYNCREATE(CFormReport1)

public:
	CFormReport1();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormReport1();

public:
	enum { IDD = IDD_FORM_REPORT1 };
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


public:
	DECLARE_EVENTSINK_MAP()

protected:
	void Init();
	void UpdateUI();

	void ClickDay();
	void ClickSW();
	void ClickGY();
	void ClickPrev();
	void ClickNext();


	void SetDateTime(int iValue = 0,BOOL btoday=FALSE);
	void SetStaticUIString(ProductInfo tInfo);
	void SetStaticUIString(ProductInfo tInfo, UINT IDCount, UINT IDDefect, UINT IDReject, UINT IDPerMCR, UINT IDPerAlign, UINT IDCont );

protected:
	WorkPart	m_iPart;

	SYSTEMTIME		m_CurDate;
	CString			m_szCurrent_time;

	CProductBank	m_ProductBank;	// 생산현황 UI에서만 쓰는 거

public:
	void ClickGxlblReportToday();

	void ClickContact01();
	void ClickContact02();
	void ClickContact03();
	void ClickContact04();
	void ClickContact05();
	void ClickContact06();
	void ClickContact07();
	void ClickContact08();
	void ClickContactTotal();

	void ClickContactFinal01();
	void ClickContactFinal02();
	void ClickContactFinal03();
	void ClickContactFinal04();
	void ClickContactFinal05();
	void ClickContactFinal06();
	void ClickContactFinal07();
	void ClickContactFinal08();
	void ClickContactFinalTotal();

	void ClickContactFirstTeamTotal();
	void ClickContactFirstDAY();
	void ClickContactFirstSW();
	void ClickContactFirstGY();

	void ClickContactFinalTeamTotal();
	void ClickContactFinalDAY();
	void ClickContactFinalSW();
	void ClickContactFinalGY();

	void DisplayContactDlg(int nHour, BOOL bFirstContact);
	void DisplayContactDlg(int nStart_Hour, int nEnd_Hour, BOOL bFirstContact);

};


