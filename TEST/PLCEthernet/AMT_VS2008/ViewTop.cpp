// MTTop.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "ViewTop.h"
#include "UserChange.h"
#include "DlgSuperUserLogin.h"

#include "MainFrm.h"


// CViewTop

IMPLEMENT_DYNCREATE(CViewTop, CFormView)

CViewTop::CViewTop()
	: CFormView(CViewTop::IDD)
{
	m_bMouseDown	= false;
}

CViewTop::~CViewTop()
{
}

void CViewTop::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ILABELX_USERLEVEL, m_lbxUserLevel);
}

BEGIN_MESSAGE_MAP(CViewTop, CFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_USER_CHANGE, &CViewTop::OnBnClickedBtnUserChange)
	ON_MESSAGE(WM_CHANGE_USER, &CViewTop::OnChangeUser)
END_MESSAGE_MAP()


// CViewTop 진단입니다.

#ifdef _DEBUG
void CViewTop::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewTop::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CViewTop 메시지 처리기입니다.

void CViewTop::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ChangeUserDisplay();
}

BEGIN_EVENTSINK_MAP(CViewTop, CFormView)
	ON_EVENT(CViewTop, IDC_IMAGE_TITLEBAR, DISPID_MOUSEDOWN, CViewTop::MouseDownImageTitlebar, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
	ON_EVENT(CViewTop, IDC_IMAGE_TITLEBAR, DISPID_MOUSEUP, CViewTop::MouseUpImageTitlebar, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
	ON_EVENT(CViewTop, IDC_LABEL_SUPERUSER_LOGIN, 1, CViewTop::OnClickLabelSuperuserLogin, VTS_NONE)
	ON_EVENT(CViewTop, IDC_LABEL_SUPERUSER_LOGIN, 2, CViewTop::OnDblClickLabelSuperuserLogin, VTS_NONE)
END_EVENTSINK_MAP()

void CViewTop::MouseDownImageTitlebar(short Button, short Shift, float X, float Y)
{
	if(Button == 1)
	{
		m_bMouseDown	= true;
		m_fOriginX		= X;
		m_fOriginY		= Y;
		((CMainFrame*)AfxGetMainWnd())->GetWindowRect(m_rectOrigin);
	}
}

void CViewTop::MouseUpImageTitlebar(short Button, short Shift, float X, float Y)
{
 	if(Button == 1 && m_bMouseDown)
	{
		m_bMouseDown = false;
		((CMainFrame*)AfxGetMainWnd())->MoveWindow(m_rectOrigin.left - (LONG)(m_fOriginX - X), m_rectOrigin.top - (LONG)(m_fOriginY - Y), 1024, 768);
	}
}

void CViewTop::OnBnClickedBtnUserChange()
{
	CUserChange dlg;
	if(dlg.DoModal() == IDOK)
		ChangeUserDisplay();
}

void CViewTop::OnClickLabelSuperuserLogin()
{
// 	CDlgSuperUserLogin dlg;
// 	dlg.DoModal();
}

void CViewTop::OnDblClickLabelSuperuserLogin()
{
 	CDlgSuperUserLogin dlg;
 	dlg.DoModal();
}

void CViewTop::ChangeUserDisplay()
{
	// 일단 super user는 숨긴다.
	if(GetDoc()->m_bSuperLogin)
		m_lbxUserLevel.put_Caption("SUPER USER");
	else
	{
		if(GetDoc()->m_bEngLogin)
			m_lbxUserLevel.put_Caption("ENGINEER");
		else
			m_lbxUserLevel.put_Caption("OPERATOR");
	}
}

LPARAM CViewTop::OnChangeUser(WPARAM wParam, LPARAM lParam)
{
	ChangeUserDisplay();
	return 0L;
}

