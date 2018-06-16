// MTTop.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "ViewBottom.h"
#include "MsgBox.h"
#include "DlgSuperUserLogin.h"

#include "MainFrm.h"

#define ENGINEERLOGIN		1
#define SUPER_USERLOGIN		2

#define BTNCOLOR			3
extern void Log(LPCSTR format, int iLogID);
// CViewBottom

IMPLEMENT_DYNCREATE(CViewBottom, CFormView)

CViewBottom::CViewBottom()
	: CFormView(CViewBottom::IDD)
{
	m_LastButtonID = IDC_BTN_BOTTOM_MANU_AUTO;
}

CViewBottom::~CViewBottom()
{
}

void CViewBottom::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	for(int i=0;i<MANU_MAX;i++)
		DDX_Control(pDX, IDC_BTN_BOTTOM_MANU_AUTO + i, m_btnManu[i]);
}

BEGIN_MESSAGE_MAP(CViewBottom, CFormView)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_AUTO, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_MANUAL, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_IO, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_TEACH, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_ALIGN, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_SETTING, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_COMM, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_CIM, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_LOG, &CViewBottom::OnBnClickedBtnBottomManu)
	ON_BN_CLICKED(IDC_BTN_BOTTOM_MANU_AUTO + MANU_EXIT, &CViewBottom::OnBnClickedBtnBottomManu)
	///////////////////////////////Message 처리 함수////////////////////////////////////////

	ON_MESSAGE(WM_CHANGE_USER, &CViewBottom::OnChangeUser)

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CViewBottom 진단입니다.

#ifdef _DEBUG
void CViewBottom::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewBottom::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CViewBottom 메시지 처리기입니다.

void CViewBottom::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	for(int i=0;i<MANU_MAX;i++)
	{
		m_btnManu[i].SetFaceColor(COLOR_BLUE);
		m_btnManu[i].SetTextColor(COLOR_WHITE);
	}
	m_btnManu[MANU_AUTO].SetFaceColor(COLOR_RED); 
}
void CViewBottom::OnBnClickedBtnBottomManu()
{
	// TODO: Add your control notification handler code here
	CDlgSuperUserLogin dlg;
	const MSG* pMsg = GetCurrentMessage();
	WPARAM wParam = pMsg->wParam;

	switch(wParam)
	{
	case IDC_BTN_BOTTOM_MANU_AUTO:
		((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_AUTO);
		break;
	case IDC_BTN_BOTTOM_MANU_MANUAL:
		((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_MANUAL);
		break;
	case IDC_BTN_BOTTOM_MANU_TEACH:
		if(GetDoc()->m_bEngLogin == TRUE)
		{
			((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_TEACH);
		}
		else
			wParam = m_LastButtonID;
		break;
	case IDC_BTN_BOTTOM_MANU_ALIGN:
		if(GetDoc()->m_bEngLogin == TRUE)
		{
			((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_ALIGN);
		}
		else
			wParam = m_LastButtonID;
		break;
	case IDC_BTN_BOTTOM_MANU_SETTING:
		if(GetDoc()->m_bEngLogin == TRUE)
		{
			((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_SETTING);
		}
		else
			wParam = m_LastButtonID;
		break;
	case IDC_BTN_BOTTOM_MANU_COMM:
		if(GetDoc()->m_bEngLogin == TRUE)
		{
			((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_COMM);
		}
		else
			wParam = m_LastButtonID;
		break;
	case IDC_BTN_BOTTOM_MANU_IO:
		if(GetDoc()->m_bEngLogin == TRUE)
		{
			((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_IO);
		}
		else
			wParam = m_LastButtonID;
		break;
	case IDC_BTN_BOTTOM_MANU_CIM:
		if(GetDoc()->m_bEngLogin == TRUE)
		{
			((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_CIM);
		}
		else
			wParam = m_LastButtonID;
		break;
	case IDC_BTN_BOTTOM_MANU_LOG:
		if(GetDoc()->m_bEngLogin == TRUE)
		{
			((CMainFrame*)AfxGetMainWnd())->SwitchForm(FORM_LOG);
		}
		else
			wParam = m_LastButtonID;
		break;
	case IDC_BTN_BOTTOM_MANU_EXIT:
		if(GetApp()->IsRunStatus() || GetApp()->m_RunStatus == STATUS_INIT)	// 진행중이면 종료 안한다.
		{
			wParam = m_LastButtonID;
			return;
		}
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_EXIT, 0);
		break;
	default:
		break;
	}

	for(int i=0;i<MANU_MAX;i++)
	{
		// Operator는 Auto와 Manual만 공개
		if(GetDoc()->m_bEngLogin == FALSE && i != MANU_AUTO && i != MANU_MANUAL && i != MANU_EXIT)
			m_btnManu[i].SetFaceColor(COLOR_GRAY); 
		else
			m_btnManu[i].SetFaceColor((IDC_BTN_BOTTOM_MANU_AUTO+i) == wParam ? COLOR_RED : COLOR_BLUE); 
	}
	m_LastButtonID = wParam;
}

LPARAM CViewBottom::OnChangeUser(WPARAM wParam, LPARAM lParam)
{
	for(int i=0;i<MANU_MAX;i++)
	{
		// Operator는 Auto와 Manual만 공개
		if(GetDoc()->m_bEngLogin == FALSE && i != MANU_AUTO && i != MANU_MANUAL && i != MANU_EXIT)
			m_btnManu[i].SetFaceColor(COLOR_GRAY); 
		else
			m_btnManu[i].SetFaceColor(i == GetMainFrame()->m_nCurrentFormID ? COLOR_RED : COLOR_BLUE); 
	}
	if(GetDoc()->m_bEngLogin == FALSE)
		GetMainFrame()->SwitchForm(FORM_AUTO);
	return 0L;
}

void CViewBottom::OnTimer(UINT_PTR nIDEvent)
{
	CFormView::OnTimer(nIDEvent);
}
