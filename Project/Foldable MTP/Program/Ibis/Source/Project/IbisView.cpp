
// IbisView.cpp : CIbisView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "IbisApp.h"
#endif

#include "IbisDoc.h"
#include "IbisView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CIbisView

IMPLEMENT_DYNCREATE(CIbisView, CFormView)

BEGIN_MESSAGE_MAP(CIbisView, CFormView)
END_MESSAGE_MAP()

// CIbisView 생성/소멸

CIbisView::CIbisView()
	: CFormView(CIbisView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CIbisView::~CIbisView()
{
}

void CIbisView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CIbisView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;	// 폼의 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

void CIbisView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CIbisView 진단

#ifdef _DEBUG
void CIbisView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIbisView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CIbisDoc* CIbisView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIbisDoc)));
	return (CIbisDoc*)m_pDocument;
}
#endif //_DEBUG


// CIbisView 메시지 처리기
