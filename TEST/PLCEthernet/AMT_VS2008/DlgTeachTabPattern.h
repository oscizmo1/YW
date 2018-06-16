#pragma once
#include "msflexgrid_pos.h"
#include "label.h"


// CDlgTeachTabPattern ��ȭ �����Դϴ�.

class CDlgTeachTabPattern : public CDialog
{
	DECLARE_DYNAMIC(CDlgTeachTabPattern)

public:
	CDlgTeachTabPattern(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTeachTabPattern();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	double m_dTV_FPCY[MAX_CAM_NUM];	// (1) Y������ ���� �Ÿ� Image 1.//
	double m_dTV_PCBX[MAX_CAM_NUM];	// (2) ���� X������ �̵� Image 2.//
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
