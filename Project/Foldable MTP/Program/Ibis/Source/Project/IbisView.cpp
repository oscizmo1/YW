
// IbisView.cpp : CIbisView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

// CIbisView ����/�Ҹ�

CIbisView::CIbisView()
	: CFormView(CIbisView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
	cs.style = WS_OVERLAPPED | WS_CHILD;	// ���� �׵θ��� ���ش�.

	return CFormView::PreCreateWindow(cs);
}

void CIbisView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CIbisView ����

#ifdef _DEBUG
void CIbisView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIbisView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CIbisDoc* CIbisView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIbisDoc)));
	return (CIbisDoc*)m_pDocument;
}
#endif //_DEBUG


// CIbisView �޽��� ó����
