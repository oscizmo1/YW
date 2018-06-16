
// GaonNuriDoc.cpp : CGaonNuriDoc 클래스의 구현
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


// CGaonNuriDoc 생성/소멸

CGaonNuriDoc::CGaonNuriDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CGaonNuriDoc::~CGaonNuriDoc()
{
}

BOOL CGaonNuriDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGaonNuriDoc serialization

void CGaonNuriDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CGaonNuriDoc 진단

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


// CGaonNuriDoc 명령
