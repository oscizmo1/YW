#pragma once
#include "label.h"
#include "msflexgrid_pos.h"
#include "afxwin.h"
#include "iswitchledx.h"


// CDlgModelChange dialog

class CDlgModelChange : public CDialog
{
	DECLARE_DYNAMIC(CDlgModelChange)

public:
	CDlgModelChange(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgModelChange();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODEL_CHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CLabel m_lblModelName;
	afx_msg void OnBnClickedBtnModelLoading();
	CMsflexgrid_pos m_gridModelList;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();
	virtual BOOL OnInitDialog();
	CLabel m_lblPanelSize;
	CComboBox m_cmbContactType;
	CComboBox m_cmbContactNum;
	CComboBox m_cmbContactSel;
	CLabel m_lblContactType;
	CLabel m_lblContactNum;
	CLabel m_lblContactSel;
	void GetModelToFile(void);
	void ClickLabelPanelSize();
	afx_msg void OnCbnSelchangeComboContactType();
	afx_msg void OnCbnSelchangeComboContactNum();
	afx_msg void OnCbnSelchangeComboContactSelction();
	CIswitchledxCtrl m_chkAlignTiltUse;
	void OnClickBtnAlignTiltUse();

	BOOL bTiltChk;
	BOOL bModelCreate;
	BOOL bModelChange;

	CString		m_strModelName;
	CString		m_strDeviceName;
	CString		m_strPanelSize;
	int			m_nContactType;
	CString		m_strResource;
	int			m_nContactNum;
	int			m_nContactSel;
	int			m_nBLUModel;
	CString		m_strAlignScore;

	CString		m_strGripper1;
	CString		m_strGripper2;
	CString		m_strGripper3;
	CString		m_strGripper4;


	void ClickLabelScoreAlign();
	CLabel m_lblScoreAlign;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnModelDelet();

	BOOL DeleteAllFile(CString strPath);
	BOOL DeletelINIFile(CString strPath);

	void ClickLabelComment();
	CComboBox m_cmbBLUModel;
	CIswitchledxCtrl m_chkModelCreate;
	CIswitchledxCtrl m_chkModelChange;
	void InitModel(void);
	void OnClickBtnModelCreate();
	void OnClickBtnModelChange();
	afx_msg void OnBnClickedBtnApply();
	CLabel m_lblDeviceName;
	CLabel m_lblGripper1;
	CLabel m_lblGripper2;
	CLabel m_lblGripper3;
	CLabel m_lblGripper4;
	void ClickLabelHandleScale1();
	void ClickLabelHandleScale2();
	void ClickLabelHandleScale3();
	void ClickLabelHandleScale4();
	void ApplyData(void);
	afx_msg void OnBnClickedOk();

	void CreateGridTitle();
	void SelectModel(int nIndex);
	long m_nOldSelRow;
};
