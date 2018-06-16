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
	
	CReportCtrl m_ListFDC;								// FDC 항목 View List		

	
public:
	BOOL ReadingToFDCMap();								// FDC_INFO_MAP 파일 취득

	void CreateFDCList();								// FDC 항목을 List로 구현한다
	void InitData();									// Data 초기화

	void ReadFdcSV();									// FDC_SV.par 파일에 있는 값 취득

	BOOL ViewFdcListSVDip();							// ReadFdcSV를 통해서 얻은 SV값을 List에 작성
	BOOL SetFdcSV();									// FDC_List에 SV 작성하는 Timer 시작

	void ViewFDC_SV();									// FDC 항목의 SV 값을 표시한다

	void CreateThread();								// Thread 구현
	void DeleteThread();								// Thread 해지

public:

	CWinThread* m_pFdcReadDataThread;
	HANDLE		m_hFdcReadDataThread;
	static UINT thrFdcReadDataThread(LPVOID pParam);
	BOOL m_bFdcReadDataThreadRun;


public:
	

	
	
	
};


