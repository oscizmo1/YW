#pragma once



// CFormIP �� ���Դϴ�.

class CFormIP : public CFormView
{
	DECLARE_DYNCREATE(CFormIP)

protected:
	CFormIP();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormIP();

public:
	enum { IDD = IDD_FORM_IP };
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


