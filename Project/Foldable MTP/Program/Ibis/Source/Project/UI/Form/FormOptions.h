#pragma once
#include "resource.h"


// CFormOptions �� ���Դϴ�.

class CFormOptions : public CFormView
{
	DECLARE_DYNCREATE(CFormOptions)

public:
	CFormOptions();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormOptions();

public:
	enum { IDD = IDD_FORM_CONFIG_OPTION };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg LRESULT OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam);			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

public:	
	BOOL m_first_load;
	
	BOOL m_dry_run;
	BOOL m_mcr_read;
 	BOOL m_fdc_alarm;
	BOOL m_InspAging;


	void UpdateOnoffValue_to_UI(BOOL bValue, int nBtnID);
	void UpdateIntValue_to_UI(int nValue, int nBtnID, CString strEndChar = _T(""));
	void UpdateDoubleValue_to_UI(double dValue, int nBtnID, CString strEndChar = _T(""));
	void GetUI_to_OnOffValue(BOOL &bValue, int nBtnID, CString strName = _T(""));
	void GetUI_to_IntValue(int &nValue, int nBtnID, CString strName = _T(""));
	void GetUI_to_DoubleValue(double &dValue, int nBtnID, CString strName = _T(""));
	void ChangeOnOffValue(int nBtnID);
	void ChangeIntValue(int nBtnID, int nMin, int nMax, CString strEndChar = _T(""));
	void ChangeDoubleValue(int nBtnID, double dMin, int dMax, CString strEndChar = _T(""));
	
	CConfigSystem m_configSystem;
	CConfigOption m_configOption;
	void CopyReloadParam();
	void WriteLogReloadParam();
	void WriteLogReloadParam_System();
	void WriteLogReloadParam_Option();

	void LoadOption();
	//20170306 kjpark D�� �۾��� ���� �ð� ����
	void ClickGxbtnOptionSave();
	void ClickGxbtnOptionDryrunOnoff();
//	void ClickGxbtnOptionMcrreadOnoff();
	void ClickGxbtnOptionLoad();
	void ClickGxbtnOptionFdcAlarmOnoff();
	void ClickGxbtnOptionLooftestOnoff();
	void ClickGxbtnOptionTmdmatchOnoff();
//	void ClickGxbtnOptionJobprocessOnoff();
	void ClickGxbtnOptionCellinfoOnoff();
	//kjpark 20180113 Tmd Version �߰�
	void ClickGxbtnOptionHostVersionOnoff();
	void ClickGxbtnOptionAaretryOnoff();
	void ClickGxbtnOptionAbretryOnoff();
	void ClickGxbtnOptionBypassOnoff();
	void ClickGxbtnOptionNgOptionOnoff();
};


