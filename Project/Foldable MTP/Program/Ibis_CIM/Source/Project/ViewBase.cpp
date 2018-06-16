// ViewBase.cpp : implementation file
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "ViewBase.h"


// CViewBase

IMPLEMENT_DYNCREATE(CViewBase, CFormView)

CViewBase::CViewBase()
: CFormView(CViewBase::IDD)
{

}

CViewBase::~CViewBase()
{
}

void CViewBase::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewBase, CFormView)
END_MESSAGE_MAP()


// CViewBase diagnostics

#ifdef _DEBUG
void CViewBase::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewBase::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewBase message handlers
