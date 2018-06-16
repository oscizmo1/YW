#pragma once
#include "msflexgrid_pos.h"
#include "label.h"
#include "DlgBluSetting.h"
#include "afxwin.h"
#include "iswitchledx.h"



// CFormSetting form view

class CFormSetting : public CFormView
{
	DECLARE_DYNCREATE(CFormSetting)

protected:
	virtual ~CFormSetting();

public:
	CFormSetting();           // protected constructor used by dynamic creation
	enum { IDD = IDD_FORM_SETTING };
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

	CDlgBluSetting m_dlgBlu;

	CMsflexgrid_pos m_SettingMotorGrid;
	DECLARE_EVENTSINK_MAP()
	void SetMotionGrid(void);
	CMsflexgrid_pos m_SocketGrid;
	void SetSocketGrid(void);
	afx_msg void OnBnClickedBtnMotionsave();
	afx_msg void OnBnClickedBtnSocketsave();
	void ClickSettingMotorgrid();
	void ClickSettingSocketgrid();
	double m_dPosition;
	afx_msg void OnBnClickedBtnReset();
	CComboBox m_cmb_MotionSelect;
	afx_msg void OnBnClickedBtnSpeedsave();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnTowerlamp();
	CLabel m_ctrlALM1;
	CLabel m_ctrlALM2;
	CLabel m_ctrlALM3;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CLabel m_ctrlALM4;
	CLabel m_ctrlALM5;
	CLabel m_ctrlALM6;
	CLabel m_ctrlALM7;
	CLabel m_ctrlALM8;
	CLabel m_ctrlALM9;
	CLabel m_ctrlALM10;
	CLabel m_ctrlALM11;
	CLabel m_ctrlALM12;
	CLabel m_ctrlNegLimit1;
	CLabel m_ctrlNegLimit2;
	CLabel m_ctrlNegLimit3;
	CLabel m_ctrlNegLimit4;
	CLabel m_ctrlNegLimit5;
	CLabel m_ctrlNegLimit6;
	CLabel m_ctrlNegLimit7;
	CLabel m_ctrlNegLimit8;
	CLabel m_ctrlNegLimit9;
	CLabel m_ctrlNegLimit10;
	CLabel m_ctrlNegLimit11;
	CLabel m_ctrlNegLimit12;
	CLabel m_ctrlHome1;
	CLabel m_ctrlHome2;
	CLabel m_ctrlHome3;
	CLabel m_ctrlHome4;
	CLabel m_ctrlHome5;
	CLabel m_ctrlHome6;
	CLabel m_ctrlHome7;
	CLabel m_ctrlHome8;
	CLabel m_ctrlHome9;
	CLabel m_ctrlHome10;
	CLabel m_ctrlHome11;
	CLabel m_ctrlHome12;
	CLabel m_ctrlPosLimit2;
	CLabel m_ctrlPosLimit3;
	CLabel m_ctrlPosLimit4;
	CLabel m_ctrlPosLimit5;
	CLabel m_ctrlPosLimit6;
	CLabel m_ctrlPosLimit7;
	CLabel m_ctrlPosLimit8;
	CLabel m_ctrlPosLimit9;
	CLabel m_ctrlPosLimit10;
	CLabel m_ctrlPosLimit11;
	CLabel m_ctrlPosLimit12;
	CLabel m_ctrlPosLimit1;
	CLabel m_ctrlServo1;
	CLabel m_ctrlServo2;
	CLabel m_ctrlServo3;
	CLabel m_ctrlServo4;
	CLabel m_ctrlServo5;
	CLabel m_ctrlServo6;
	CLabel m_ctrlServo7;
	CLabel m_ctrlServo8;
	CLabel m_ctrlServo9;
	CLabel m_ctrlServo10;
	CLabel m_ctrlServo11;
	CLabel m_ctrlServo12;
	afx_msg void OnBnClickedBtnBluSetting();
	CString strDirtoryName;
	CString strComboName;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SettingMotionParam(void);
	void MotionGrid( int nAxisNum);
	double m_MotorSpeed;
	afx_msg void OnBnClickedBtnPoweron();
	void SetSocketParam(void);
	void GetSocketParam(void);
	afx_msg void OnBnClickedSearchSearch();
	afx_msg void OnBnClickedServoOnoff();
	afx_msg void OnBnClickedBtnInspCountSet();
	int m_nInspectionResetCount;
	CIswitchledxCtrl m_Chk_InspCount;
	void OnChangeUserIswitchledxInspCount();
};


