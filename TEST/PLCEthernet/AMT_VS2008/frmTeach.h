#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DlgTeachTabNormalCCD.h"
#include "DlgTeachTabPattern.h"
#include "DlgTeachTabPinCCD.h"
#include "label.h"
#include "iswitchledx.h"

enum
{
	STATIC_TEACH_LEFT_CAM,
	STATIC_TEACH_RIGHT_CAM,

	STATIC_TEACH_VIEW_MAX
};

// CFormTeach form view

class CFormTeach : public CFormView
{
	DECLARE_DYNCREATE(CFormTeach)

protected:
	virtual ~CFormTeach();

public:
	CFormTeach();           // protected constructor used by dynamic creation
	enum { IDD = IDD_FORM_TEACH };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();

	CImageList				m_imageList;
	CDlgTeachTabNormalCCD* m_pTabDlgNorCCD;
	CDlgTeachTabPinCCD*		 m_pTabDlgPinCCD;
	CDlgTeachTabPattern*		 m_pTabDlgPattern;
	
	
	int		m_iMoveAction;
	BOOL	m_bPosMoving;
	BOOL	m_bLive;

	CStatic m_pctTeachGrap[STATIC_TEACH_VIEW_MAX];
	CTabCtrl m_ctrlTabTeach;
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTabTeaching(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangingTabTeaching(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTeachViewLiveGrabStart();
	afx_msg void OnTeachViewLiveGrabStop();
	afx_msg void OnMoveToReadyPos();
	afx_msg void OnMoveToGrabPos();
	afx_msg void OnBlowOn();
	afx_msg void OnBlowOff();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCurPosModelSave();
	afx_msg void OnBnClickedBtnTopClampOn();
	void SeImageLiveView(void);
	afx_msg void OnTestBtn1();
	afx_msg void OnBnClickedRadioCenter();
	afx_msg void OnBnClickedRadioSide();
	afx_msg void OnBnClickedBtnTopClampOff();
	afx_msg void OnBnClickedCmdCylinderOn();
	afx_msg void OnBnClickedCmdCylinderOff();
//	afx_msg void OnBnClickedBtnNtherModelSave();
	afx_msg void OnBnClickedCmdTiltingUpPos();
	afx_msg void OnBnClickedCmdTiltDownPos();
	CLabel m_ctrlModelName;
	afx_msg void OnBnClickedBtnContactAlignCheck();
	DECLARE_EVENTSINK_MAP()
	void OnClickIswitchledxTopLamp();
	CIswitchledxCtrl m_iswxTeachTopLight;
	CIswitchledxCtrl m_iswxTeachBtmLight;
	afx_msg void OnBnClickedBtnMovePcbChangePos();
};


