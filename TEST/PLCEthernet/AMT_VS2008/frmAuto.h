#pragma once
#include "label.h"
#include "afxwin.h"
#include "MsgBox.h"
#include "isevensegmentintegerx.h"
#include "iswitchledx.h"

enum
{
	BTN_APPLY_PROCESS_INITIALIZE,
	BTN_CAM0_COUNT_RESET,
	BTN_CAM1_COUNT_RESET,
	BTN_CAM2_COUNT_RESET,
	BTN_CAM3_COUNT_RESET,
	BTN_START,           
	BTN_STOP,            
	BTN_PM_MODE,         
	BTN_INITIALIZE,      
	BTN_MODEL_LOADING,   
	BTN_NORMAL,          
	BTN_STAGE,           
	BTN_ALIGN_SKIP,      
	BTN_ALIGN_VI_SKIP,   
	BTN_LIVE,            
	BTN_HALT,            
	BTN_SCRAP,           
	BTN_UNSCRAP,         

	BTN_MAX
};

// CFormAuto 폼 뷰입니다.

class CFormAuto : public CFormView
{
	DECLARE_DYNCREATE(CFormAuto)

protected:
	virtual ~CFormAuto();

public:
	CFormAuto();           // 동적 만들기에 사용되는 protected 생성자입니다.
	enum { IDD = IDD_FORM_AUTO };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();

	CMFCButton m_btnAuto[BTN_MAX];
	CStatic m_pctImageGrap[MAX_CAM_NUM];
	CIsevensegmentintegerx m_segCamCount[MAX_CAM_NUM];
	CLabel		m_lbCurMoDelName;
	CDeviceCtl*					m_pDevice;
	CMsgBox	m_MsgBox;
	BOOL m_bLive;

	void UpdateDisplayData(void);
	
	//////////////////////////////////////////////////////////////////////////
	// User Message
	afx_msg LPARAM OnLabelCaptionChange(WPARAM wParam, LPARAM lParam);
	afx_msg LPARAM OnUpdateVITact(WPARAM wParam, LPARAM lParam);
	afx_msg LPARAM OnUpdateAlignCount(WPARAM wParam, LPARAM lParam);
	afx_msg LPARAM OnUpdateProcess(WPARAM wParam, LPARAM lParam);
	afx_msg LPARAM OnUpdateInspectCount(WPARAM wParam, LPARAM lParam);//  [9/13/2009 janghan]
	afx_msg LPARAM OnChangeUser(WPARAM wParam, LPARAM lParam);
	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnNewModelLoading();
	afx_msg void OnSystemInitialize();
	afx_msg void OnCCDImageLive();
	afx_msg void OnCCDImageHalt();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SeImageLiveView();
	afx_msg void OnBnClickedBtnAutoStart();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnAutoPmMode();
	afx_msg void OnBnClickedBtnAutoStop();
	afx_msg void OnBnClickedRadioAutoNormal();
	afx_msg void OnBnClickedRadioAutoStage();
	afx_msg void OnBnClickedRadioAutoAlignSkip();
	afx_msg void OnBnClickedRadioAutoAlignViSkip();
	afx_msg void OnBnClickedBtnAutoApplyProcessInitialize();
	DECLARE_EVENTSINK_MAP()
	void OnClickIswitchledxAutoLoadingLock();
	CLabel m_lblRun_State;

	// Connection timer
	CLabel m_lblConnectionLC;
	CLabel m_lblConnectionVI;
	BOOL m_bConnectionLC;
	BOOL m_bConnectionVI;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// Run Mode
	CLabel m_lblRunMode;
	CLabel m_lblContactTact;
	CLabel m_lblFlowingTact;
	CLabel m_lblPostGrabTact;
	CLabel m_lblTotalInspTact;
	afx_msg void OnBnClickedBtnAutoCam0CountReset();
	afx_msg void OnBnClickedBtnAutoCam1CountReset();
	afx_msg void OnBnClickedBtnAutoCam2CountReset();
	afx_msg void OnBnClickedBtnAutoCam3CountReset();
	CLabel m_lblRunProcess[ProcMax];
	void OnClickIswitchledxAutoBottomLight();
	void OnClickIswitchledxAutoTopLight();
	CIswitchledxCtrl m_swxAutoTopLight;
	CIswitchledxCtrl m_swxAutoBtmLight;
	afx_msg void OnBnClickedButtonCountReset();
	CIsevensegmentintegerx m_ctlInspection_Counter;
	afx_msg void OnBnClickedBtnPgStart();
	afx_msg void OnBnClickedBtnPgStop();
	afx_msg void OnBnClickedBtnAutoReset();
};


