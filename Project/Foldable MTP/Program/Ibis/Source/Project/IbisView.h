
// IbisView.h : CIbisView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"


class CIbisView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CIbisView();
	DECLARE_DYNCREATE(CIbisView)

public:
	enum{ IDD = IDD_FORM_BASE };

// Ư���Դϴ�.
public:
	CIbisDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CIbisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // IbisView.cpp�� ����� ����
inline CIbisDoc* CIbisView::GetDocument() const
   { return reinterpret_cast<CIbisDoc*>(m_pDocument); }
#endif

