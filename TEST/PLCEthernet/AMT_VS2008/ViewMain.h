#pragma once



// CViewMain 폼 뷰입니다.

class CViewMain : public CFormView
{
	DECLARE_DYNCREATE(CViewMain)

protected:
	virtual ~CViewMain();

public:
	CViewMain();           // 동적 만들기에 사용되는 protected 생성자입니다.
	enum { IDD = IDD_VIEW_MAIN };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};


