// MTView.cpp : CMTView Ŭ������ ����
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

// CMTView ����/�Ҹ�

CMTView::CMTView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CMTView::~CMTView()
{
}

BOOL CMTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMTView �׸���

void CMTView::OnDraw(CDC* /*pDC*/)
{
	CMTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMTView ����

#ifdef _DEBUG
void CMTView::AssertValid() const
{
	CView::AssertValid();
}

void CMTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMTDoc* CMTView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMTDoc)));
	return (CMTDoc*)m_pDocument;
}
#endif //_DEBUG


// CMTView �޽��� ó����



int CMTView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}
