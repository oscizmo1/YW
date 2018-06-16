#pragma once
#include "msflexgrid_pos.h"



// CFormIO form view

class CFormIO : public CFormView
{
	DECLARE_DYNCREATE(CFormIO)

protected:
	virtual ~CFormIO();

public:
	CFormIO();           // protected constructor used by dynamic creation
	enum { IDD = IDD_FORM_IO };
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
	BOOL bIOState[0x20];
	BOOL bIOStateOld[0x20];
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	CMsflexgrid_pos m_InPutGrid1;
	CMsflexgrid_pos m_InPutGrid2;
	CMsflexgrid_pos m_OutPutGrid1;
	CMsflexgrid_pos m_OutPutGrid2;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridOutput1();
	void ClickMsflexgridOutput2();
};


