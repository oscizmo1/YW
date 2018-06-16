// frmLog.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "frmLog.h"


// CFormLog

IMPLEMENT_DYNCREATE(CFormLog, CFormView)

CFormLog::CFormLog()
	: CFormView(CFormLog::IDD)
{

}

CFormLog::~CFormLog()
{
}

void CFormLog::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormLog, CFormView)
END_MESSAGE_MAP()


// CFormLog diagnostics

#ifdef _DEBUG
void CFormLog::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormLog::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormLog message handlers

BOOL CFormLog::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormLog::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}
