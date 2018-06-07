#pragma once

#include "resource.h"

// CFormMain 폼 뷰입니다.
//kjpark 20161021 Main Form Cell Status view 구현
#include <vector>
#include <map>
using namespace std; 
#include "UI\GausGUI\GxImage.h"
#include "StateMachine\StateCorelation.h"


struct SKIP_BUTTON
{
	UINT nResourceID;
	BOOL *pbSet;
	BOOL *pbNotUseEnable;
	BOOL *pbAZoneEnd;
};


struct CELL_STATUS
{
	UINT uIDCellInfoResult;
	UINT uIDJobProcess;
	UINT uIDCellLoading;
	UINT uIDInnerID;
	UINT uIDCellID;
	UINT uIDProductID;
	UINT uIDMCRImage;
	UINT uIDAZoneDefect;
	UINT uIDWriteDefect;
	UINT uIDVerifyDefect;
	UINT uIDBZoneDefect;
	UINT uIDFinalDefect;

	CELL_POS posCell;
	CString strOldCellID;
	CGxImage *pImage;
};


struct OPTION_MONITOR
{
	UINT uID;
	BOOL *pbOption;
	BOOL bOnGreen;	// TRUE : ON(Green),OFF(RED)	FALSE : ON(RED), OFF(GRAY)
};

class CFormMain : public CFormView
{
	DECLARE_DYNCREATE(CFormMain)

public:
	CFormMain();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormMain();

public:
	enum { IDD = IDD_FORM_MAIN };
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
	afx_msg LRESULT OnCountUpdate(WPARAM wParam, LPARAM lParam);			
	afx_msg LRESULT OnUpdateMCRData_CH1(WPARAM wParam, LPARAM lParam);				// MCR Data가 수신되었다.
	afx_msg LRESULT OnUpdateMCRData_CH3(WPARAM wParam, LPARAM lParam);				// MCR Data가 수신되었다.

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	BOOL m_bFormInit;
	CGxImage m_imgLive_CH1;
	CGxImage m_imgLive_CH2;
	CGxImage m_imgLive_CH3;
	CGxImage m_imgLive_CH4;
	CGxImage m_imgLive_CH5;
	CGxImage m_imgLive_CH6;

	// Channel 선택 변수 - LSH171215
	BOOL m_bCH1lSelect;
	BOOL m_bCH2lSelect;
	BOOL m_bCH3lSelect;
	BOOL m_bCH4lSelect;
	BOOL m_bCH5lSelect;
	BOOL m_bCH6lSelect;

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void PMButton_UpdateState();
	void ChangeForm(int nIdx, BOOL bForce = FALSE);

	void MoniStopStaticStatus();

	void AZoneCellNGOnOff(JIG_ID jig, JIG_CH ch, BOOL bAZoneDefectCheck = FALSE);
	void RemoveCell(JIG_ID jig, JIG_CH ch);

	std::vector<SKIP_BUTTON> m_vct_AZoneSkipButton;
	void AZoneSkiipButton_Init();
	void AZoneNotUseButton_Update();

	std::vector<CELL_STATUS> m_vct_CellStatus;
	void CellStatus_Init();
	void CellStatus_Update();

	std::vector<OPTION_MONITOR> m_vct_OptionMonitor;
	void OptionMonitor_Init();
	void OptionMonitor_Update();

	void UpdateCellCreateButton();

	CStateCorelation		m_StateCorelation;
	

	void ClickBtnModePM();
	void ClickGxbtnStop();
	void ClickGxbtnStart();
	//kjpark 20171016 계측기 코릴레이션 하강 / 원복 버튼
	void ClickGxbtnCorelation();

	void ClickGxbtnMainCh1NotUse();
	void ClickGxbtnMainCh1Skip();
	void ClickGxbtnMainCh3NotUse();
	void ClickGxbtnMainCh3Skip();
	void ClickGxstcMainCountClear();
	void ClickGxbtnProductEnd();
	void ClickGxstcMainCh1();
	void ClickGxstcMainCh3();
	void ClickGxbtnCreatecellinfo();
	void ClickGxbtnLoadingStop();
};


