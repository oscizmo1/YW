
// GaonNuriDoc.cpp : CGaonNuriDoc Ŭ������ ����
//

#include "stdafx.h"
#include "GaonNuri.h"

#include "GaonNuriDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGaonNuriDoc

IMPLEMENT_DYNCREATE(CGaonNuriDoc, CDocument)

BEGIN_MESSAGE_MAP(CGaonNuriDoc, CDocument)
END_MESSAGE_MAP()


// CGaonNuriDoc ����/�Ҹ�

CGaonNuriDoc::CGaonNuriDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CGaonNuriDoc::~CGaonNuriDoc()
{
}

BOOL CGaonNuriDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CGaonNuriDoc serialization

void CGaonNuriDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CGaonNuriDoc ����

#ifdef _DEBUG
void CGaonNuriDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGaonNuriDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGaonNuriDoc ���
