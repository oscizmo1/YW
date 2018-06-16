#pragma once
#include "msflexgrid_pos.h"
#include "afxwin.h"
#include "label.h"
#include "iJogPad.h"
#include "iSliderCtrl.h"
#include "iSwitchSlideCtrl.h"
#include "prev_port.h"


// CDlgTeachTabNormalCCD 대화 상자입니다.
enum
{
	BTN_GRAP_POS,
	BTN_READY_POS,
	BTN_PCB_CHANGE_POS,
	
	BTN_NORMAL_MAX
};

enum
{
	LABEL_CUR_POS_LX,
	LABEL_CUR_POS_RX,
	LABEL_CUR_POS_LY,
	LABEL_CUR_POS_RY,

	LABEL_POS_MAX
};
class CDlgTeachTabNormalCCD : public CDialog
{
	DECLARE_DYNAMIC(CDlgTeachTabNormalCCD)

public:
	CDlgTeachTabNormalCCD(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTeachTabNormalCCD();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NORMAL_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid_pos m_cGridPosVal;
	virtual BOOL OnInitDialog();
	void SetInitDisplayData(void);

	BOOL		m_bInitDlg;
	BOOL		m_bLeftUpEvent;
	BOOL		m_bLeftLeftEvent;
	BOOL		m_bLeftRightEvent;
	BOOL		m_bLeftDownEvent;
	BOOL		m_bRightUpEvent;
	BOOL		m_bRightLeftEvent;
	BOOL		m_bRightDownEvent;
	BOOL		m_bRightRightEvent;
	POSITION_DATA	m_dGrabPos[MAX_CAM_NUM];
	POSITION_DATA	m_dReadyPos[MAX_CAM_NUM];
	POSITION_DATA	m_dPCBChangePos[MAX_CAM_NUM];

	double m_dTiltingPos;
	double m_dUnTiltingPos;
	int		m_iCurSetPos;

	double m_dJogSpeed;
	double m_dJogLXOldPos;
	double m_dJogLYOldPos;
	double m_dJogRXOldPos;
	double m_dJogRYOldPos;

	void SetCurposDisplay(void);
	void SetGridPos();
	void SetGridReadyPos(void);
	void SetGridGrabPos(void);
	void SetGridPCBChangePos(void);
	BOOL CheckLimit(int nAxis, double dPos);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	CMFCButton m_btnNormalCCD[BTN_NORMAL_MAX];
	CMFCButton m_btnSetPosition[2];
	CLabel m_lbCurPos[LABEL_POS_MAX];
	CJogPad m_JogPadLeftCam;
	CJogPad m_JogPadRightCam;
	CIswitchslideCtrl m_SliderSpeed;

	afx_msg void OnSetGrapPos();
	afx_msg void OnSetReadyPos();
	afx_msg void OnSetPCBChangePos();
	afx_msg void OnSetCurPos();
	afx_msg void OnSetPosMove();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	

	DECLARE_EVENTSINK_MAP()
	void OnChangeJogSpeed();
	void OnMouseDownBtnLeftjogup(short Button, short Shift, long X, long Y);
	void OnMouseUpBtnLeftjogup(short Button, short Shift, long X, long Y);
	void OnMouseDownBtnLeftjogleft(short Button, short Shift, long X, long Y);
	void OnMouseUpBtnLeftjogleft(short Button, short Shift, long X, long Y);
	CPrev_port m_ctrlLeftJogUp;
	CPrev_port m_ctrlLeftJogLeft;
	void OnMouseDownBtnLeftjogdown(short Button, short Shift, long X, long Y);
	CPrev_port m_ctrlLeftJogDown;
	void OnMouseUpBtnLeftjogdown(short Button, short Shift, long X, long Y);
	void OnMouseDownBtnLeftjogright(short Button, short Shift, long X, long Y);
	CPrev_port m_ctrlLeftJogRight;
	void OnMouseUpBtnLeftjogright(short Button, short Shift, long X, long Y);
	CPrev_port m_ctrlRightJogUp;
	CPrev_port m_ctrlRightJogLeft;
	CPrev_port m_ctrlRightJogDown;
	CPrev_port m_ctrlRightJogRight;
	void OnMouseDownBtnRightjogup(short Button, short Shift, long X, long Y);
	void OnMouseUpBtnRightjogup(short Button, short Shift, long X, long Y);
	void OnMouseDownBtnRightjogdown(short Button, short Shift, long X, long Y);
	void OnMouseUpBtnRightjogdown(short Button, short Shift, long X, long Y);
	void OnMouseDownBtnRightjogleft(short Button, short Shift, long X, long Y);
	void OnMouseUpBtnRightjogleft(short Button, short Shift, long X, long Y);
	void OnMouseDownBtnRightjogright(short Button, short Shift, long X, long Y);
	void OnMouseUpBtnRightjogright(short Button, short Shift, long X, long Y);
	CButton m_ctrlTiltChk;
	afx_msg void OnBnClickedChkTilt();
	void OnClickGrid1();
	CLabel m_lblAutoCurrentModel;
	CLabel m_lblAutoCurrentModel2;
};
