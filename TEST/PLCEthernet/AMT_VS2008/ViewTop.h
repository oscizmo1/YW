#pragma once
#include "afxwin.h"
#include "lbl_blu.h"


// CViewTop 폼 뷰입니다.

class CViewTop : public CFormView
{
	DECLARE_DYNCREATE(CViewTop)

protected:
	CViewTop();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CViewTop();

public:
	enum { IDD = IDD_VIEW_TOP };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	DECLARE_EVENTSINK_MAP()
	void MouseDownImageTitlebar(short Button, short Shift, float X, float Y);
	void MouseUpImageTitlebar(short Button, short Shift, float X, float Y);

	bool	m_bMouseDown;
	float	m_fOriginX;
	float	m_fOriginY;
	CRect	m_rectOrigin;
	afx_msg void OnBnClickedBtnUserChange();
	afx_msg LPARAM OnChangeUser(WPARAM wParam, LPARAM lParam);
	void OnClickLabelSuperuserLogin();
	void OnDblClickLabelSuperuserLogin();
	void ChangeUserDisplay();
	CLbl_blu m_lbxUserLevel;
};
