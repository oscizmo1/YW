// SubView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MT.h"
#include "ViewMain.h"
#include ".\ViewMain.h"


// CViewMain

IMPLEMENT_DYNCREATE(CViewMain, CFormView)

CViewMain::CViewMain()
	: CFormView(CViewMain::IDD)
{
}

CViewMain::~CViewMain()
{
}

void CViewMain::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewMain, CFormView)
END_MESSAGE_MAP()


// CViewMain �����Դϴ�.

#ifdef _DEBUG
void CViewMain::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewMain::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CViewMain �޽��� ó�����Դϴ�.

