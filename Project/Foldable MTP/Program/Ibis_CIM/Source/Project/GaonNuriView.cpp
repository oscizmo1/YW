
// GaonNuriView.cpp : CGaonNuriView Ŭ������ ����
//

#include "stdafx.h"
#include "GaonNuri.h"

#include "GaonNuriDoc.h"
#include "GaonNuriView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGaonNuriView

IMPLEMENT_DYNCREATE(CGaonNuriView, CView)

BEGIN_MESSAGE_MAP(CGaonNuriView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGaonNuriView ����/�Ҹ�

CGaonNuriView::CGaonNuriView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CGaonNuriView::~CGaonNuriView()
{
}

BOOL CGaonNuriView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CGaonNuriView �׸���

void CGaonNuriView::OnDraw(CDC* /*pDC*/)
{
	CGaonNuriDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CGaonNuriView �μ�

BOOL CGaonNuriView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CGaonNuriView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CGaonNuriView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CGaonNuriView ����

#ifdef _DEBUG
void CGaonNuriView::AssertValid() const
{
	CView::AssertValid();
}

void CGaonNuriView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGaonNuriDoc* CGaonNuriView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGaonNuriDoc)));
	return (CGaonNuriDoc*)m_pDocument;
}
#endif //_DEBUG


// CGaonNuriView �޽��� ó����
