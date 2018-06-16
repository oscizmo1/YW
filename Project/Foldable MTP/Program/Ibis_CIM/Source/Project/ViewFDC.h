#pragma once
#include "publicclass\reportctrl.h"
#include "ieditx1.h"



// CViewFDC form view

class CViewFDC : public CFormView
{
	DECLARE_DYNCREATE(CViewFDC)

//protected:
public:
	CViewFDC();           // protected constructor used by dynamic creation
	virtual ~CViewFDC();

public:
	enum { IDD = IDD_VIEW_FDC };
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual void OnInitialUpdate();
	
	CReportCtrl m_ListFDC;								// FDC �׸� View List		

	
public:
	BOOL ReadingToFDCMap();								// FDC_INFO_MAP ���� ���

	void CreateFDCList();								// FDC �׸��� List�� �����Ѵ�
	void InitData();									// Data �ʱ�ȭ

	void ReadFdcSV();									// FDC_SV.par ���Ͽ� �ִ� �� ���

	BOOL ViewFdcListSVDip();							// ReadFdcSV�� ���ؼ� ���� SV���� List�� �ۼ�
	BOOL SetFdcSV();									// FDC_List�� SV �ۼ��ϴ� Timer ����

	void ViewFDC_SV();									// FDC �׸��� SV ���� ǥ���Ѵ�

	void CreateThread();								// Thread ����
	void DeleteThread();								// Thread ����

public:

	CWinThread* m_pFdcReadDataThread;
	HANDLE		m_hFdcReadDataThread;
	static UINT thrFdcReadDataThread(LPVOID pParam);
	BOOL m_bFdcReadDataThreadRun;


public:
	

	
	
	
};


