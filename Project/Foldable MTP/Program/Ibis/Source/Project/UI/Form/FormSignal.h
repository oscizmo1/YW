#pragma once

#include "resource.h"
#include "UI\GausGUI\GxButtonEx.h"
// CFormSetup1 폼 뷰입니다.

class CFormSignal : public CFormView
{
	DECLARE_DYNCREATE(CFormSignal)

public:
	CFormSignal();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormSignal();

public:
	enum { IDD = IDD_FORM_SETUP_SIGNAL };
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
	//kjpark 20161021 Setup 신호기 UI 구현
	//kjpark 20170908 Setup 시그널 페이지 수정
	BOOL m_bButtonCheck[3][2];
	UINT m_nCellButton[3][2];
	
	int m_nPatternNo;//20170503 BKH D존 Draw할 패턴넘버 변수 추가
	int m_nSquareThickness;//20170503 BKH Draw할 사각형 두께 변수 추가
	int m_nSquareSize;//20170503 BKH Draw할 사각형 크기 변수 추가
	int m_nSquareColorLineRed;//20170503 BKH Draw할 사각형 선 색깔 변수 추가
	int m_nSquareColorLineGreen;//20170503 BKH Draw할 사각형 선 색깔 변수 추가
	int m_nSquareColorLineBlue;//20170503 BKH Draw할 사각형 선 색깔 변수 추가
	int m_nSquareXPoint;
	int m_nSquareYPoint;
	//20170504 BKH DrawMassage 관련 변수들 추가
	int m_nMSGBackColorRed;
	int m_nMSGBackColorGreen;
	int m_nMSGBackColorBlue;
	int m_nMSGFontColorRed;
	int m_nMSGFontColorGreen;
	int m_nMSGFontColorBlue;
	int m_nMSGXPoint;
	int m_nMSGYPoint;
	int m_nMSGFontSize;
	CString m_strMSG;

	BOOL GetSelectButton(ZONE_ID zone, JIG_CH nCh);
	//kjpark 20170908 Setup 시그널 페이지 수정
	BOOL GetSelectButton(JIG_ID shttle, JIG_CH nCh);
	BOOL m_bIsMutli;
	BOOL m_bIsShwoWindow;
	void SetLogListUpdate(CString strMessage);	

	void ClickBtnSelect_A1();
	void ClickBtnSelect_A2();
	void ClickBtnSelect_A3();
	void ClickBtnSelect_A4();
	void ClickBtnSelect_A5();
	void ClickBtnSelect_B1();
	void ClickBtnSelect_B2();
	void ClickBtnSelect_B3();
	void ClickBtnSelect_B4();
	void ClickBtnSelect_B5();
	void ClickBtnSelect_C1();
	void ClickBtnSelect_C2();
	void ClickBtnSelect_C3();
	void ClickBtnSelect_C4();
	void ClickBtnSelect_C5();
	void ClickBtnSelect_D1();
	void ClickBtnSelect_D2();
	void ClickBtnSelect_D3();
	void ClickBtnSelect_D4();
	void ClickBtnSelect_D5();
	void ClickBtnSelect_All();
	void ClickBtnSelect_Clear();
	void ClickBtnSelect_Single();
	void ClickBtnSelect_Multi();
	void ClickBtnZoneSelect_A();
	void ClickBtnZoneSelect_B();
	void ClickBtnZoneSelect_C();
	void ClickBtnZoneSelect_D();

	void ClickBtnPGSelect_ON();
	void ClickBtnPGSelect_OFF();
	void ClickBtnPGSelect_PREV();
	void ClickBtnPGSelect_NEXT();

	void ClickBtnPGSelect_Reset();
	void ClickBtnPGSelect_CellLoading();
	void ClickBtnPGSelect_TMDInfo();
	//kjpark 20180113 Tmd Version 추가
	void ClickBtnPGSelect_HostVersion();
	void ClickBtnPGSelect_ClientVersion();
	//kjpark 20171016 하이페리온 커넷트 체크 CMD send 기능
	void ClickBtnPGSelect_HyperionChk();
	void ClickBtnPGSelect_TSPStart();
	void ClickBtnPGSelect_IDCheck();
	void ClickBtnPGSelect_OTPREG();

	//kjpark 20170908 Setup 시그널 페이지 수정
	void ClickBtnPGSelect_EVT_Version();
	void ClickBtnPGSelect_TECheck();
	void ClickBtnPGSelect_COPR_ICT();
	void ClickBtnPGSelect_POC_Error();
	void ClickBtnPGSelect_DDI_Block_Test();

	//kjpark 20161114 C Zone Foce 추가
	void ClickBtnPGSelect_Force();
	void ClickBtnPGSelect_Force2();
	void ClickBtnPGSelect_Force3();
	
	DECLARE_EVENTSINK_MAP()

	

	CListCtrl m_PGLog;
	
//	void ClickGxbtnTeach1FunctionForceBef();
	void ClickGxbtnTeach1FunctionForce1Bef();
	void ClickGxbtnTeach1FunctionForce2Bef();
	void ClickGxbtnTeach1FunctionForce3Bef();
	void ClickGxbtnTeach1FunctionIctCheck();
	void ClickGxbtnTeach1FunctionOptionCheck2();
	//kjpark 20170908 Setup 시그널 페이지 수정
	void ClickGxbtnTeach1FunctionOptionCheck3();
	void ClickGxbtnTeach1FunctionOptionCheck4();
	void ClickGxbtnTeach1FunctionOptionCheck5();
	void ClickGxbtnTeach1FunctionOptionCheck6();
	void ClickGxbtnTeach1FunctionOptionCheck7();
	void ClickGxbtnTeach1FunctionOptionCheck8();
	void ClickGxbtnTeach1FunctionOptionCheck9();
	void ClickGxbtnTeach1FunctionOptionCheck10();
	//kjpark 20171010 신호기 UI 에 White Circle 추가
	void ClickGxbtnTeach1FunctionWhiteCircle();
	//kjpark 20170911 신호기 UI MTP COMMAND 추가
	void ClickGxbtnTeach1FunctionMTPWrite();
	void ClickGxbtnTeach1FunctionMTPVerify();
	void ClickGxbtnTeach1FunctionHlpmCheck();
	void ClickGxbtnTeach1FunctionWhiteCurrent();
	void ClickGxbtnTeach1FunctionSleepCurrent();

	//20170503 BKH 신호기 특정 존 및 특정 채널만 선택하는 기능 추가
	void ClickGxstcZoneA();
	BOOL CheckSelectAllZonePG(ZONE_ID zone);
	//kjpark 20170908 Setup 시그널 페이지 수정
	BOOL CheckSelectAllZonePG(JIG_ID jig);
	BOOL CheckSelectAllChanelPG(JIG_CH ch);
	void ClickGxstcZoneB();
	void ClickGxstcZoneC();
	void ClickGxstcZoneD();
	void ClickGxstcCh1();
	void ClickGxstcCh2();
	void ClickGxstcCh3();
	void ClickGxstcCh4();
	void ClickGxstcCh5();
	void ClickGxlblPatternNo();//20170503 BKH D존에 보여줄 패턴 넘버 값 입력.
	void ClickGxbtnDzonePatternShow();//20170503 BKH  입력된 패턴 넘버 값을 보여줘라.
	void ClickGxlblSquareThickness();
	void ClickGxlblSquareSize();
	void ClickGxlblSquareColorline();
	void ClickGxlblSquareX();
	void ClickGxlblSquareY();
	void ClickGxbtnSquareAdd();
	void ClickGxbtnSquareDraw();
	void ClickGxlblMsgBackcolor();
	void ClickGxlblMsgFontcolor();
	void ClickGxlblMsgX();
	void ClickGxlblMsgY();
	void ClickGxlblMSGFontsize();
	void ClickGxbtnMsgAdd();
	void ClickGxbtnMsgDraw();
	void ClickGxbtnTeach1FunctionOptionSignalPowerReset();
};


