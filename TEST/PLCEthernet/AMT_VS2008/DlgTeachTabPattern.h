#pragma once
#include "msflexgrid_pos.h"
#include "label.h"


// CDlgTeachTabPattern 대화 상자입니다.

class CDlgTeachTabPattern : public CDialog
{
	DECLARE_DYNAMIC(CDlgTeachTabPattern)

public:
	CDlgTeachTabPattern(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTeachTabPattern();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	double m_dTV_FPCY[MAX_CAM_NUM];	// (1) Y축으로 전진 거리 Image 1.//
	double m_dTV_PCBX[MAX_CAM_NUM];	// (2) 왼쪽 X축으로 이동 Image 2.//
	double m_dTV_StageReady;			// Ready.//

	CMsflexgrid_pos m_cGridOffset;
	virtual BOOL OnInitDialog();
	void SetOffsetData(void);
	void SetGridOffsetData(void);
	CLabel m_lbGrabPosOffset;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnTeachOffsetDataSave();
	void OnSetLabelReadyOffset();
	void ClickPinZaxisOffset();
	CLabel m_lbPinOffset;
	void ClickMsflexgridOffset();
};
