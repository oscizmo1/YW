// FormConfig2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormConfig2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormConfig2

IMPLEMENT_DYNCREATE(CFormConfig2, CFormView)

CFormConfig2::CFormConfig2()
	: CFormView(CFormConfig2::IDD)
{

}

CFormConfig2::~CFormConfig2()
{
}

void CFormConfig2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormConfig2, CFormView)
END_MESSAGE_MAP()


// CFormConfig2 진단입니다.

#ifdef _DEBUG
void CFormConfig2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormConfig2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormConfig2 메시지 처리기입니다.
