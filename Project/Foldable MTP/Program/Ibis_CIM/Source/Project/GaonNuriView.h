
// GaonNuriView.h : CGaonNuriView Ŭ������ �������̽�
//


#pragma once


class CGaonNuriView : public CView
{
protected: // serialization������ ��������ϴ�.
	CGaonNuriView();
	DECLARE_DYNCREATE(CGaonNuriView)

// Ư���Դϴ�.
public:
	CGaonNuriDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CGaonNuriView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GaonNuriView.cpp�� ����� ����
inline CGaonNuriDoc* CGaonNuriView::GetDocument() const
   { return reinterpret_cast<CGaonNuriDoc*>(m_pDocument); }
#endif

