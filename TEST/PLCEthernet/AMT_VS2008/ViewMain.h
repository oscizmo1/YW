#pragma once



// CViewMain �� ���Դϴ�.

class CViewMain : public CFormView
{
	DECLARE_DYNCREATE(CViewMain)

protected:
	virtual ~CViewMain();

public:
	CViewMain();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	enum { IDD = IDD_VIEW_MAIN };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};


