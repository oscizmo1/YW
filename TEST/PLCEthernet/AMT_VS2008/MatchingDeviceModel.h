#pragma once
#include "lbl_blu.h"
#include "msflexgrid_pos.h"
#include "afxwin.h"


// CMatchingDeviceModel dialog

class CMatchingDeviceModel : public CDialog
{
	DECLARE_DYNAMIC(CMatchingDeviceModel)

public:
	CMatchingDeviceModel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMatchingDeviceModel();

// Dialog Data
	enum { IDD = IDD_DIALOG_MATCHING_DEVICE_MODEL };

private:
	CString m_strDeviceID;
	CString m_strModelName;
	long m_nOldSelRow;
	void SelectModel( int nIndex );

public:
	void SetDeviceID(CString strDeviceID);
	CString GetModelName();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CLbl_blu m_lblDeviceID;
	CMsflexgrid_pos m_gridModelName;
	afx_msg void OnBnClickedBtnNotSelect();
	afx_msg void OnBnClickedBtnSelect();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ClickGridModelName();
	CButton m_btnCreate;
	afx_msg void OnBnClickedBtnCreate();
};
