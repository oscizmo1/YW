// MTView.cpp : CMTView 클래스의 구현
//

#include "stdafx.h"
#include "MT.h"

#include "MTView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMTView

IMPLEMENT_DYNCREATE(CMTView, CView)
BEGIN_MESSAGE_MAP(CMTView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMTView 생성/소멸

CMTView::CMTView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CMTView::~CMTView()
{
}

BOOL CMTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMTView 그리기

void CMTView::OnDraw(CDC* /*pDC*/)
{
	CMTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMTView 진단

#ifdef _DEBUG
void CMTView::AssertValid() const
{
	CView::AssertValid();
}

void CMTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMTDoc* CMTView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMTDoc)));
	return (CMTDoc*)m_pDocument;
}
#endif //_DEBUG


// CMTView 메시지 처리기



int CMTView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
