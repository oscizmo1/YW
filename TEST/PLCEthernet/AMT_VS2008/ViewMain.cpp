// SubView.cpp : 구현 파일입니다.
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


// CViewMain 진단입니다.

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


// CViewMain 메시지 처리기입니다.

