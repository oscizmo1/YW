// MTView.h : iCMTView Ŭ������ �������̽�
//

#pragma once

#include "MIL71.h"
#include "CIF32.h"
#include "MMC39.h"
#include "ThreadLiveGrap.h"

class CMTView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMTView();
	DECLARE_DYNCREATE(CMTView)

// Ư��
public:
	CMTDoc* GetDocument() const;

// �۾�
public:


// ������
	public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ����
public:
	virtual ~CMTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // MTView.cpp�� ����� ����
inline CMTDoc* CMTView::GetDocument() const
   { return reinterpret_cast<CMTDoc*>(m_pDocument); }
#endif


