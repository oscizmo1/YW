#pragma once
#include "msflexgrid_pos.h"
#include "lbl_blu.h"


// CDlgModleLoading dialog

class CDlgModleLoading : public CDialog
{
	DECLARE_DYNAMIC(CDlgModleLoading)

public:
	CDlgModleLoading(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgModleLoading();

	BOOL DeleteAllFile(CString strPath);
	BOOL DeletelINIFile(CString strPath);

// Dialog Data
	enum { IDD = IDD_DIALOG_MOEDL_LOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMsflexgrid_pos m_gridDeviceName;
	CLbl_blu m_lblPanelSize;
	CLbl_blu m_lblFPCType;
	CLbl_blu m_lblContactNum;
	CLbl_blu m_lblContactor;
	CLbl_blu m_lblFPCLength;
	CLbl_blu m_lblPinNum;
	CLbl_blu m_lblAlignSuccess;
	CLbl_blu m_lblComment;
	CLbl_blu m_lblDeviceName;
	CLbl_blu m_lblNewPanelSize;
	void SetLoadingData(void);
	void OpenModelFile(CString strModelName);

	CString m_strDeviceName;
	int		m_nFPCType;
	int		m_nPanelSize;
	int		m_nContactNum;
	int		m_nContactor;
	int		m_nAlignSuccess;
	int		m_nFPCLength;
	int		m_nContactPinNum;
	
	DECLARE_EVENTSINK_MAP()
	void ClickGridDeviceName();
	void CreateGridTitle(void);
};
