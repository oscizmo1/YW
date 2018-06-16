#pragma once
#include "afxwin.h"
#include "ijogpad.h"
#include "iswitchledx.h"
#include "msflexgrid_pos.h"
#include "label.h"
#include "afxcmn.h"
#include "isliderctrl.h"


#define MOVE_RECT		0
#define MOVE_CENTER		1
//최소 사각형 크기. //
#define MIN_RECT_SIZE   10
// 최대 사각형 크기. //
#define MAX_RECT_SIZE   600


// 연속 이동 변수. //
#define MOVE_X_UP		0	
#define MOVE_X_DOWN     1
#define MOVE_Y_UP		2
#define MOVE_Y_DOWN		3
#define SIZE_X_INFLATE	4	// 팽창
#define SIZE_X_DEFLATE	5	// 축소
#define SIZE_Y_INFLATE		6
#define SIZE_Y_DEFLATE		7

enum
{
	STATIC_ALIGN_GRAP_VIEW,
	STATIC_PATTERN_VIEW,
	STATIC_ALIGN_VIEW_MAX
};

enum
{
	ALIGN_SWITCH_TOP_LAMP,
	ALIGN_SWITCH_BOTT_LAMP,

	ALIGN_SWITCH_MAX
};

enum
{
	ALIGN_BTN_PTR_SEARCH,
	ALIGN_BTN_PTR_EDIT_DONTCARE,
	ALIGN_BTN_CLAMP_ON,
	ALIGN_BTN_CLAMP_OFF,
	ALIGN_BTN_MOVE_READY,
	ALIGN_BTN_MOVE_GRAB,
	ALIGN_BTN_ALIGN,
	ALIGN_BTN_BLOW_ON,
	ALIGN_BTN_BLOW_OFF,
	ALIGN_BTN_GRAB_START,
	ALIGN_BTN_GRAB_END,
	ALIGN_BTN_PTR_SAVE,
	ALIGN_BTN_REMOVE_PTN,

	ALIGN_BTN_MAX
};
// CFormAlign form view

class CFormAlign : public CFormView
{
	DECLARE_DYNCREATE(CFormAlign)

protected:
	virtual ~CFormAlign();

public:
	CFormAlign();           // protected constructor used by dynamic creation
	enum { IDD = IDD_FORM_ALIGN };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	double m_dJogXOldPos;
	double m_dJogYOldPos;
	double m_dJogSizeXOldPos;
	double m_dJogSizeYOldPos;

	int		 m_iMoveSpeed;
	int      m_iMoveTarget;
	int      m_iSelPattern;
	int	     m_iCurPatternPos;
	int		 m_iSelCamera;
	int		 m_iJogMode;
	int      m_iMoveAction;
	BOOL  m_bPatReg;
	BOOL  m_bMouseMoveEnable;
	BOOL  m_bPosMoving;
	BOOL	m_bLive;

private:
	CRect m_StaticAlignRect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	CStatic m_pctPatternGrap[STATIC_ALIGN_VIEW_MAX];
	CJogPad m_JogPadPosition;
	CJogPad m_JogPadRectSize;
	CIswitchledxCtrl m_SwitchCtrl[ALIGN_SWITCH_MAX];
	CMFCButton m_btnAlign[ALIGN_BTN_MAX];

	DECLARE_EVENTSINK_MAP()
	void OnJogMoveXPosition();
	void OnJogMoveYPosition();
	void OnJogXRectSize();
	void OnJogYRectSize();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnAlignUpsideLamp();
	void OnTopLampOnOff();
	void OnBottomLampOnOff();
	afx_msg void OnPatternSearch();
	void SetDisplayAlignData();
	CPoint CalcCenter(int iCenX, int iCenY, int iWidth, int iHeight);
	afx_msg void OnPatternSave();
	CMsflexgrid_pos m_cGridAlignRet;
	void SetGridStructure();
	void SetCamSelectButton(void);
	void SetPatternSelectButton(void);
	afx_msg void OnSeleteCam1();
	afx_msg void OnSeleteCam0();
	afx_msg void OnSeleteCam2();
	afx_msg void OnSeleteCam3();
	afx_msg void OnSeleteFPC_0();
	afx_msg void OnSeletePCB();
	afx_msg void OnCCDImageGrabLiveStart();
	afx_msg void OnCCDImageGrabLiveEnd();
	void OnMouseDownMovePos(short Button, short Shift, long X, long Y);
	void OnMouseUpMovePos(short Button, short Shift, long X, long Y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetPatternDrawLine(void);
	void OnMouseDownRectSize(short Button, short Shift, long X, long Y);
	void OnMouseUpRectSize(short Button, short Shift, long X, long Y);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMoveToReadyPos();
	afx_msg void OnMoveToGrabPos();
	afx_msg void OnAlignAction();
	afx_msg void OnDrawRectCenter();
	afx_msg void OnBlowOn();
	afx_msg void OnBlowOff();
	afx_msg void OnBnClickedBtnClampOn();
	afx_msg void OnBnClickedBtnClampOff();
	CIswitchledxCtrl m_SwitchCtrlSpeed;
	CIswitchledxCtrl m_SwitchCtrlTarget;
	void OnSetChangeSpeed();
	void OnSetChangeTarget();
	void SeImageLiveView(void);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRadioFpc1();
	afx_msg void OnBnClickedRadioFpc2();
private:
	CMFCButton m_btnLoadImage;
public:
	afx_msg void OnBnClickedBtnLoadImage();
	CMFCButton m_btnFpc0;
	CMFCButton m_btnFpc1;
	CMFCButton m_btnFpc2;
	CMFCButton m_btnPcb;
	CMFCButton m_radCam0;
	CMFCButton m_radCam1;
	CMFCButton m_radCam2;
	CMFCButton m_radCam3;
	CMFCButton m_btnUncontactAll;
	afx_msg void OnBnClickedBtnRemovePtn();
	CLabel m_lblModel;
	afx_msg void OnBnClickedBtnEditDontCare();
	afx_msg void OnBnClickedBtnUncontactAll();
	CEdit m_EditCondition;
	void SetSuperUserControl();
	CIsliderControl m_slxCondition;
	void OnPositionChangeIsliderx1();
	afx_msg void OnEnChangeEditCondition();
	BOOL SetAlignPos();
	BOOL CheckLimit( int nAxis, double dPos );
	int m_nFFC_edgeLevel_Min;
	int m_nFFC_edgeLevel_Max;
	int m_nFFC_markLevel_Min;
	int m_nFFC_markLevel_Max;
	int m_nPCB_edgeLevel_Min;
	int m_nPCB_edgeLevel_Max;
	int m_nPCB_markLevel_Min;
	int m_nPCB_markLevel_Max;
	int m_nLevelStep;
	afx_msg void OnBnClickedButtonLevelSet();
	afx_msg void OnBnClickedButtonResetPcbx();
	BOOL m_bCheck_AutoTune;
};


