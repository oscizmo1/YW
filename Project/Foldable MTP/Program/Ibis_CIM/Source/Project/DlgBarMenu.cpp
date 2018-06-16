// DlgBarMenu.cpp : implementation file
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "DlgBarMenu.h"

#include "Define.h"

// CDlgBarMenu dialog

IMPLEMENT_DYNAMIC(CDlgBarMenu, CDialogBar)

CDlgBarMenu::CDlgBarMenu()
{

}

CDlgBarMenu::~CDlgBarMenu()
{
}

void CDlgBarMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUTTON_MENU_CIM, m_btnCIM);
	//DDX_Control(pDX, IDC_BUTTON_MENU_FDC, m_btnFDC);
	//DDX_Control(pDX, IDC_BUTTON_MENU_CELL, m_btnCell);
	//DDX_Control(pDX, IDC_BUTTON_MENU_TACTTIME, m_btnTact);
	//DDX_Control(pDX, IDC_BUTTON_MENU_ALARM, m_btnAlarm);
	//DDX_Control(pDX, IDC_BUTTON_MENU_ALIGN, m_btnAlign);
	//DDX_Control(pDX, IDC_BUTTON_MENU_RMS, m_btnRMS);
	//DDX_Control(pDX, IDC_BUTTON_MENU_SETTING, m_btnSetting);
	//DDX_Control(pDX, IDC_BUTTON_MENU_EXIT, m_btnExit);
}


BEGIN_MESSAGE_MAP(CDlgBarMenu, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

LRESULT CDlgBarMenu::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	OnInitDialog();

	return CDialogBar::HandleInitDialog(wParam, lParam);
}

BOOL CDlgBarMenu::OnInitDialog()
{
	UpdateData(FALSE);

	// TODO:  Add extra initialization here

	//CFont font;
	//font.CreateFont( 22, // 폰트 높이를 논리적 단위로 지정, 0이면 기본 값)
	//	0, // 폰트 폭지정, 0이면 폰트dml 장평이 일정하다.
	//	0, // 폰트 각도를 0.1도 단위로 설정, 360분법으로 3시 방향이 0도이다.
	//	0, // 글자 한 자와 X축과의 각도 지정
	//	FW_NORMAL,// 폰트의 두께 0~1000사이 값으로 지정(기본400)
	//	FALSE,// 기울임체를 할꺼냐?
	//	FALSE, // 밑줄을 칠꺼냐?
	//	0, // 관통선
	//	ANSI_CHARSET, // 문자 셋을 설정, ANI_CHARSET(통상 윈도우), HANGEUL_CHARSET, OEM_CHARSET
	//	OUT_DEFAULT_PRECIS,// 출력 정확도
	//	CLIP_DEFAULT_PRECIS,// 클리핑 정확도
	//	DEFAULT_QUALITY,// 논리적 폰트를 물리적 폰트에 얼마?? 근접시킬 것인가를 지정
	//	DEFAULT_PITCH | FF_SWISS, // 폰트의 피치와그룹 설정
	//	_T("arial bold")) ; // 글꼴의 이름을 나타내는 문자열을 설정

	//m_btnCIM.SetIcon(IDI_ICON_MONITOR);
	//m_btnCIM.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnCIM.SetFlat(false);
	//m_btnCIM.SetFont(&font);

	//m_btnFDC.SetIcon(IDI_ICON_FDC);
	//m_btnFDC.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnFDC.SetFlat(false);
	//m_btnFDC.SetFont(&font);

	////m_btnCell.SetIcon(IDI_ICON_CELL);
	//m_btnCell.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnCell.SetFlat(false);
	//m_btnCell.SetFont(&font);

	////m_btnTact.SetIcon(IDI_ICON_TACTTIME);
	//m_btnTact.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnTact.SetFlat(false);
	//m_btnTact.SetFont(&font);

	////m_btnAlarm.SetIcon(IDI_ICON_ALARM);
	//m_btnAlarm.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnAlarm.SetFlat(false);
	//m_btnAlarm.SetFont(&font);

	////m_btnAlign.SetIcon(IDI_ICON_ALIGN);
	//m_btnAlign.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnAlign.SetFlat(false);
	//m_btnAlign.SetFont(&font);

	//m_btnRMS.SetIcon(IDI_ICON_RMS);
	//m_btnRMS.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnRMS.SetFlat(false);
	//m_btnRMS.SetFont(&font);

	//m_btnSetting.SetIcon(IDI_ICON_SETTING);
	//m_btnSetting.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnSetting.SetFlat(false);
	//m_btnSetting.SetFont(&font);

	//m_btnExit.SetIcon(IDI_ICON_EXIT);
	//m_btnExit.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_btnExit.SetFlat(false);
	//m_btnExit.SetFont(&font);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CDlgBarMenu message handlers

BOOL CDlgBarMenu::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush backBrush(RGB(222, 231, 242));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush); 
	CRect rect; pDC->GetClipBox(&rect); 
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush); 
	return TRUE; 

	return CDialogBar::OnEraseBkgnd(pDC);
}

void CDlgBarMenu::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	// TODO: Add your specialized code here and/or call the base class
}

BOOL CDlgBarMenu::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogBar::OnCommand(wParam, lParam);
}
BEGIN_EVENTSINK_MAP(CDlgBarMenu, CDialogBar)
	ON_EVENT(CDlgBarMenu, IDC_GXBTN_MENU1, DISPID_CLICK, CDlgBarMenu::ClickGxbtnMenu1, VTS_NONE)
	ON_EVENT(CDlgBarMenu, IDC_GXBTN_MENU2, DISPID_CLICK, CDlgBarMenu::ClickGxbtnMenu2, VTS_NONE)
	ON_EVENT(CDlgBarMenu, IDC_GXBTN_MENU3, DISPID_CLICK, CDlgBarMenu::ClickGxbtnMenu3, VTS_NONE)
	ON_EVENT(CDlgBarMenu, IDC_GXBTN_MENU4, DISPID_CLICK, CDlgBarMenu::ClickGxbtnMenu4, VTS_NONE)
	ON_EVENT(CDlgBarMenu, IDC_GXBTN_MENU5, DISPID_CLICK, CDlgBarMenu::ClickGxbtnMenu5, VTS_NONE)
	ON_EVENT(CDlgBarMenu, IDC_GXBTN_MENU6, DISPID_CLICK, CDlgBarMenu::ClickGxbtnMenu6, VTS_NONE)
END_EVENTSINK_MAP()


// 화면 변환을 요청한다.
void CDlgBarMenu::ClickGxbtnMenu1()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_CIM, (LPARAM)0);

}

// FDC 화면
void CDlgBarMenu::ClickGxbtnMenu2()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_FDC, (LPARAM)0);
}

// RMS 화면
void CDlgBarMenu::ClickGxbtnMenu3()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_RMS, (LPARAM)0);
}

// ECM 화면
void CDlgBarMenu::ClickGxbtnMenu4()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_ECM, (LPARAM)0);
}

// 설정 화면
void CDlgBarMenu::ClickGxbtnMenu5()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_SETTING, (LPARAM)0);
}


// 종료
void CDlgBarMenu::ClickGxbtnMenu6()
{
	// 종료하도록 이벤트를 보낸다.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, WM_CLOSE, 0, 0);
}
