#pragma once
#include "isliderctrl.h"
#include "ijogpad.h"
#include "iswitchslidectrl.h"
#include "msflexgrid_pos.h"
#include "Label.h"
#include "ilineargaugex.h"
#include "afxcmn.h"


//CDlgTeachTabPinCCD ��ȭ �����Դϴ�.

class CDlgTeachTabPinCCD : public CDialog
{
	DECLARE_DYNAMIC(CDlgTeachTabPinCCD)

public:
	CDlgTeachTabPinCCD(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTeachTabPinCCD();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PIN_CAMERA };


public:
	double m_dJogSpeed;
	double m_dJogXOldPos;
	double m_dJogYOldPos;

	BOOL	m_bCurViewShow;	
	BOOL	 m_bInitDlg;
	BOOL	m_bZaxisJogMoveEnable;
	int		 m_iCurSetPos;

	POSITION_DATA m_dGrabPos;
	POSITION_DATA	m_dReadyPos;

	int		m_iZaxisJogSpeedCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CJogPad m_JogPadPogoCam;
	CIswitchslideCtrl m_SliderSpeed;
	CLabel				m_lbCurPos[3];
	CMsflexgrid_pos m_cGridPosVal;


	void SetInitDisplayData(void);

	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void OnJogMoveXPogocam();
	void OnJogMoveYPogocam();
	void SetCurposDisplay(void);
	void SetGridReadyPos(void);
	void SetGridGrabPos(void);
	void OnChangeJogSpeed();
	afx_msg void OnSetCurPos();
	afx_msg void OnSetPosMove();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void OnMouseUpJogPadPogoCam(short Button, short Shift, long X, long Y);
	afx_msg void OnBnClickedRadioGrapPos();
	afx_msg void OnBnClickedRadioReadyPos();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	CIswitchslideCtrl m_SliderZaxisJog;
	void OnPositionMoveFinish();
	void OnClickIswitchsliderxZaxisJog();
	void OnZaxisJogSpeedChange();
	void OnPositionChangeUserIswitchsliderxZaxisJog();
	void OnMouseDownIswitchsliderxZaxisJog(short Button, short Shift, long X, long Y);
};
