#pragma once
#include "label.h"
#include "iswitchledx.h"



// CFormManual form view

class CFormManual : public CFormView
{
	DECLARE_DYNCREATE(CFormManual)

protected:
	virtual ~CFormManual();

public:
	CFormManual();           // protected constructor used by dynamic creation
	enum { IDD = IDD_FORM_MANUAL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CPLCComm* m_pMelsec;

	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_LableColor[2];

	// PLC �δ� �غ� ��ȣ
	CLabel m_lblPlcReady;
	// PLC �δ� ���� �䱸
	CLabel m_lblPlcReq;
	// PLC �δ� ���� ��
	CLabel m_lblPlcDo;
	// PLC �δ� ���� �Ϸ�
	CLabel m_lblPlcEnd;
	// PLC �δ� PM ��� ����
	CLabel m_lblPlcPm;
	// PLC �δ� �Ϲ� �˶�
	CLabel m_lblPlcAlarm;
	// PLC �δ� EMO / Door �˶�
	CLabel m_lblPlcEmo;
	// PC �δ� �غ� ��ȣ
	CLabel m_lblPcReady;
	// PC �δ� ���� �䱸
	CLabel m_lblPcReq;
	// PC �δ� ���� ��
	CLabel m_lblPcDo;
	// PC �δ� ���� �Ϸ�
	CLabel m_lblPcEnd;
	// PC �δ� PM ��� ����
	CLabel m_lblPcPm;
	// PC �δ� �Ϲ� �˶�
	CLabel m_lblPcAlarm;
	// PC �δ� EMO / Door �˶�	
	CLabel m_lblPcEmo;

	// PLC ��δ� �غ� ��ȣ
	CLabel m_lblPlcUnloadReady;
	// PLC ��δ� ���� �䱸
	CLabel m_lblPlcUnloadReq;
	// PLC ��δ� ���� ��
	CLabel m_lblPlcUnloadDo;
	// PLC ��δ� ���� �Ϸ�
	CLabel m_lblPlcUnloadEnd;
	// PLC ��δ� Panel Sort
	CLabel m_lblPlcUnloadSort;
	// PC ��δ� �غ� ��ȣ
	CLabel m_lblPcUnloadReady;
	// PC ��δ� ���� �䱸
	CLabel m_lblPcUnloadReq;
	// PC ��δ� ���� ��
	CLabel m_lblPcUnloadDo;
	// PC ��δ� ���� �Ϸ�
	CLabel m_lblPcUnloadEnd;
	// PC ��δ� Panel Sort
	CLabel m_lblPcUnloadSort;

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	DECLARE_EVENTSINK_MAP()
	void OnSetRetryCnt();
	void OnSetInspTimeOut();
	afx_msg void OnBnClickedBtnManuCmpOn();
	afx_msg void OnBnClickedBtnMenuCmpOff();
	afx_msg void OnBnClickedBtnMoveAlignPos();
	afx_msg void OnBnClickedBtnMenuFullCont();
	afx_msg void OnBnClickedBtnMenuBlowOn();
	afx_msg void OnBnClickedBtnMenuBlowOff();
	afx_msg void OnBnClickedBtnMenuContOn();
	afx_msg void OnBnClickedBtnMenuContOff();
	afx_msg void OnBnClickedBtnMenuMoveReadyPos();
	afx_msg void OnBnClickedBtnMenuReleaseAll();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	// PLC Handler
	CLabel m_lblPlcLock;
	CLabel m_lblPcLock;
	CLabel m_lblPcRun;
	CLabel m_lblPlcPickLo;
	afx_msg void OnBnClickedBtnLoaderCommClr();
	afx_msg void OnBnClickedBtnTiltingPos();
	afx_msg void OnBnClickedBtnUntiltingPos();

private:
	BOOL m_iMoveAction;
	BOOL m_bPosMoving;
public:
	afx_msg void OnBnClickedBtnUnloaderCommClr();
	afx_msg void OnBnClickedBtnInterlockPass();
	afx_msg void OnBnClickedBtnMelsecIocheck();
	afx_msg void OnBnClickedRadioGrabEnd();
	afx_msg void OnBnClickedRadioInspEnd();
	void OnChangeUserIswitchledxManualTopLight();
	void OnChangeUserIswitchledxManualBtmLight();
	void OnChangeUserIswitchledxManualBlu();
	CIswitchledxCtrl m_iswxManualTopLight;
	CIswitchledxCtrl m_iswxManualBtmLight;
	CIswitchledxCtrl m_iswxManualBLU;
	CLabel m_lblCleanset;
	void ClickLabelCleanPopup();
	afx_msg void OnBnClickedBtnCleanAlppy();

	DWORD dTiltReplyTiemr;

	CString m_strCleanning;
	CIswitchledxCtrl m_iswxUseAMC;
	CIswitchledxCtrl m_iswxUseCreate;
	void OnChangeUserIswitchledxUseMc();
	void OnChangeUserIswitchledxUseMc2();
	afx_msg void OnBnClickedBtnInitViComm();
	void ClickLabelContactDelay();
	CLabel m_lblContactDelay;
	CLabel m_lbEqstatus;
	CLabel m_lbProstatus;
};


