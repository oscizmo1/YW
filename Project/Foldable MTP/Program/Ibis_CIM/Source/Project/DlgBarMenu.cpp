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
	//font.CreateFont( 22, // ��Ʈ ���̸� ���� ������ ����, 0�̸� �⺻ ��)
	//	0, // ��Ʈ ������, 0�̸� ��Ʈdml ������ �����ϴ�.
	//	0, // ��Ʈ ������ 0.1�� ������ ����, 360�й����� 3�� ������ 0���̴�.
	//	0, // ���� �� �ڿ� X����� ���� ����
	//	FW_NORMAL,// ��Ʈ�� �β� 0~1000���� ������ ����(�⺻400)
	//	FALSE,// �����ü�� �Ҳ���?
	//	FALSE, // ������ ĥ����?
	//	0, // ���뼱
	//	ANSI_CHARSET, // ���� ���� ����, ANI_CHARSET(��� ������), HANGEUL_CHARSET, OEM_CHARSET
	//	OUT_DEFAULT_PRECIS,// ��� ��Ȯ��
	//	CLIP_DEFAULT_PRECIS,// Ŭ���� ��Ȯ��
	//	DEFAULT_QUALITY,// ���� ��Ʈ�� ������ ��Ʈ�� ��?? ������ų ���ΰ��� ����
	//	DEFAULT_PITCH | FF_SWISS, // ��Ʈ�� ��ġ�ͱ׷� ����
	//	_T("arial bold")) ; // �۲��� �̸��� ��Ÿ���� ���ڿ��� ����

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


// ȭ�� ��ȯ�� ��û�Ѵ�.
void CDlgBarMenu::ClickGxbtnMenu1()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_CIM, (LPARAM)0);

}

// FDC ȭ��
void CDlgBarMenu::ClickGxbtnMenu2()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_FDC, (LPARAM)0);
}

// RMS ȭ��
void CDlgBarMenu::ClickGxbtnMenu3()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_RMS, (LPARAM)0);
}

// ECM ȭ��
void CDlgBarMenu::ClickGxbtnMenu4()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_ECM, (LPARAM)0);
}

// ���� ȭ��
void CDlgBarMenu::ClickGxbtnMenu5()
{
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_SETTING, (LPARAM)0);
}


// ����
void CDlgBarMenu::ClickGxbtnMenu6()
{
	// �����ϵ��� �̺�Ʈ�� ������.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, WM_CLOSE, 0, 0);
}
