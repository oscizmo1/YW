#pragma once
#include "resource.h"



// CFormConfig2 �� ���Դϴ�.

class CFormConfig2 : public CFormView
{
	DECLARE_DYNCREATE(CFormConfig2)

public:
	CFormConfig2();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormConfig2();

public:
	enum { IDD = IDD_FORM_CONFIG2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


