#pragma once
#include "afxwin.h"

enum 
{
	MANU_AUTO,
	MANU_MANUAL,
	MANU_IO,
	MANU_TEACH,
	MANU_ALIGN,
	MANU_SETTING,
	MANU_COMM,
	MANU_CIM,
	MANU_LOG,
	MANU_EXIT,

	MANU_MAX
};

// CViewTop 폼 뷰입니다.

class CViewBottom : public CFormView
{
	DECLARE_DYNCREATE(CViewBottom)

protected:
	CViewBottom();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CViewBottom();

public:
	enum { IDD = IDD_VIEW_BOTTOM };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	WPARAM m_LastButtonID;
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedBtnBottomManu();
	
	afx_msg LPARAM OnChangeUser(WPARAM wParam, LPARAM lParam);

	CMFCButton m_btnManu[MANU_MAX];
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

