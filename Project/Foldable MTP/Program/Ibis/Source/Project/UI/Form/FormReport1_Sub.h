#pragma once

#include "resource.h"
#include <vector>

using namespace std;

// CFormReport1_Sub 폼 뷰입니다.

#define TIMER1		9599


#define ColumnMax	7
#define MainRowMax	13
#define TotalRowMax	3

class CFormReport1_Sub : public CFormView
{
	DECLARE_DYNCREATE(CFormReport1_Sub)

public:
	CFormReport1_Sub();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormReport1_Sub();

public:
	enum { IDD = IDD_FORM_REPORT1_SUB };
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

public:
	//	UI화면에 표시하는 데이터들을 업데이트 - LSH
	void UpdateUIData();
	//	UI화면에 표시하는 Button/Static 의 상태를 업데이트 - LSH
	void UpdateDisplay();

protected:
	// Main Record 집합 배열 - LSH171218
	UINT m_nMainRecord[MainRowMax][ColumnMax];

	// Total record 집합 배열 - LSH171218
	UINT m_nTotalRecord[TotalRowMax][ColumnMax];

	void Init();

	// 현재 날짜를 입력한다 - LSH171217
	void SetDateTime(int iValue = 0,BOOL btoday=FALSE);

	// 입력할 데이터와 UI Record을 입력하여 화면의 데이터를 갱신한다 - LSH171217
	void SetStaticUIString(ProductInfo* infoProduct, UINT* pRecord);

	// 화면 갱신 - LSH171217
	void DisplayUpdate();

protected:
	// 정보를 저장할 변수 - LSH171218
	ProductInfo m_infoMainProductInfo[MainRowMax];
	ProductInfo m_infoTotalProductInfo[TotalRowMax];

	// 현재 선택된 Part - LSH171217
	UINT m_nCurSelectPart;

	SYSTEMTIME		m_CurDate;
	CString			m_szCurrent_time;

	CProductBank	m_ProductBank;	// 생산현황 UI에서만 쓰는 거

public:
	void ClickGxlblReportToday();
	void ClickDay();
	void ClickNight();	
	void ClickPrev();
	void ClickNext();

	/* Product */ 
	void ClickGxlblReportNum1();
	void ClickGxlblReportNum2();
	void ClickGxlblReportNum3();
	void ClickGxlblReportNum4();
	void ClickGxlblReportNum5();
	void ClickGxlblReportNum6();
	void ClickGxlblReportNum7();
	void ClickGxlblReportNum8();
	void ClickGxlblReportNum9();
	void ClickGxlblReportNum10();
	void ClickGxlblReportNum11();
	void ClickGxlblReportNum12();
	void ClickGxlblReportTotal1();

	/* GOOD */
	void ClickGxlblReportGood1();
	void ClickGxlblReportGood2();
	void ClickGxlblReportGood3();
	void ClickGxlblReportGood4();
	void ClickGxlblReportGood5();
	void ClickGxlblReportGood6();
	void ClickGxlblReportGood7();
	void ClickGxlblReportGood8();
	void ClickGxlblReportGood9();
	void ClickGxlblReportGood10();
	void ClickGxlblReportGood11();
	void ClickGxlblReportGood12();
	void ClickGxlblReportTotal2();

	/* Reject */
	void ClickGxlblReportReject1();
	void ClickGxlblReportReject2();
	void ClickGxlblReportReject3();
	void ClickGxlblReportReject4();
	void ClickGxlblReportReject5();
	void ClickGxlblReportReject6();
	void ClickGxlblReportReject7();
	void ClickGxlblReportReject8();
	void ClickGxlblReportReject9();
	void ClickGxlblReportReject10();
	void ClickGxlblReportReject11();
	void ClickGxlblReportReject12();
	void ClickGxlblReportTotal3();

	/* MCR */
	void ClickGxlblReportMcr1();
	void ClickGxlblReportMcr2();
	void ClickGxlblReportMcr3();
	void ClickGxlblReportMcr4();
	void ClickGxlblReportMcr5();
	void ClickGxlblReportMcr6();
	void ClickGxlblReportMcr7();
	void ClickGxlblReportMcr8();
	void ClickGxlblReportMcr9();
	void ClickGxlblReportMcr10();
	void ClickGxlblReportMcr11();
	void ClickGxlblReportMcr12();
	void ClickGxlblReportTotal4();

	/* First contact */
	void ClickGxlblReportFirst1();
	void ClickGxlblReportFirst2();
	void ClickGxlblReportFirst3();
	void ClickGxlblReportFirst4();
	void ClickGxlblReportFirst5();
	void ClickGxlblReportFirst6();
	void ClickGxlblReportFirst7();
	void ClickGxlblReportFirst8();
	void ClickGxlblReportFirst9();
	void ClickGxlblReportFirst10();
	void ClickGxlblReportFirst11();
	void ClickGxlblReportFirst12();
	void ClickGxlblReportTotal5();

	/* Final contact */
	void ClickGxlblReportFinal1();
	void ClickGxlblReportFinal2();
	void ClickGxlblReportFinal3();
	void ClickGxlblReportFinal4();
	void ClickGxlblReportFinal5();
	void ClickGxlblReportFinal6();
	void ClickGxlblReportFinal7();
	void ClickGxlblReportFinal8();
	void ClickGxlblReportFinal9();
	void ClickGxlblReportFinal10();
	void ClickGxlblReportFinal11();
	void ClickGxlblReportFinal12();
	void ClickGxlblReportTotal6();

	/* Total prodect */
	void ClickGxlblReportNumSun1();
	void ClickGxlblReportNumSun2();
	void ClickGxlblReportNumSun3();

	/* Total good */
	void ClickGxlblReportGoodSun1();
	void ClickGxlblReportGoodSun2();
	void ClickGxlblReportGoodSun3();

	/* Total reject */
	void ClickGxlblReportRejectSun1();
	void ClickGxlblReportRejectSun2();
	void ClickGxlblReportRejectSun3();

	/* Total MCR */
	void ClickGxlblReportMcrSun1();
	void ClickGxlblReportMcrSun2();
	void ClickGxlblReportMcrSun3();

	/* Total first contact */
	void ClickGxlblReportFirstSun1();
	void ClickGxlblReportFirstSun2();
	void ClickGxlblReportFirstSun3();

	/* Total final contact */
	void ClickGxlblReportFinalSun1();
	void ClickGxlblReportFinalSun2();
	void ClickGxlblReportFinalSun3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


