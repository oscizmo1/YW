// DlgbarHTeach.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarHTeach.h"

//GxGUIFrame 사용
#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "MainFrm.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgbarHTeach 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgbarHTeach, CDialogBar)

CDlgbarHTeach::CDlgbarHTeach()
{

}

CDlgbarHTeach::~CDlgbarHTeach()
{
}

void CDlgbarHTeach::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarHTeach, CDialogBar)
	//d ON_UPDATE_COMMAND_UI(IDC_BTN_HTEACH_SUBMENU1, OnUpdateCmdUI)
	//d ON_BN_CLICKED(IDC_BTN_HTEACH_SUBMENU1, &CDlgbarHTeach::OnBnClickedBtnHteachSubmenu1)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// Active-X Event 처리
BEGIN_EVENTSINK_MAP(CDlgbarHTeach, CDialogBar)
	//kjpark 20170902 티칭 UI 추가 및 상단탭 동작 
	ON_EVENT(CDlgbarHTeach, IDC_BTNX_HTEACH_SHUTTLE_1, DISPID_CLICK, CDlgbarHTeach::ClickBtnxShuttle_1, VTS_NONE)
	ON_EVENT(CDlgbarHTeach, IDC_BTNX_HTEACH_SHUTTLE_2, DISPID_CLICK, CDlgbarHTeach::ClickBtnxShuttle_2, VTS_NONE)
	ON_EVENT(CDlgbarHTeach, IDC_BTNX_HTEACH_SHUTTLE_3, DISPID_CLICK, CDlgbarHTeach::ClickBtnxShuttle_3, VTS_NONE)

	ON_EVENT(CDlgbarHTeach, IDC_BTNX_HTEACH_PDT_IF, DISPID_CLICK, CDlgbarHTeach::ClickBtnxHteachPdtIf, VTS_NONE)
END_EVENTSINK_MAP()




// CDlgbarHTeach 메시지 처리기입니다.
void CDlgbarHTeach::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


BOOL CDlgbarHTeach::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CHILD;	// 폼의 테두리를 없앤다.

	return CDialogBar::PreCreateWindow(cs);
}


// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame에서 ShowDialog를 수행하면 아래 순서대로 메세지를 호출한다.
//
// ShowDlgBar() 호출시
// 2. GxMSG_GUI_CHANGE_SUBMENU,	nSubID
// 3. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// 메인 메뉴 선택 화면 전환 완료
LRESULT CDlgbarHTeach::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	DispCurrentSelect( (UINT)wParam );		// 선택된 메뉴 반전

	return 0;
}

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CDlgbarHTeach::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{

	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CDlgbarHTeach::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_HTEACH_SHUTTLE_1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 지정 화면으로 폼을 변경한다.
void CDlgbarHTeach::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);

	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
	pMain->SetCurPageTea1(nIdx);
}

// 현재 선택된 버튼의 색을 달리해준다.
void CDlgbarHTeach::DispCurrentSelect(UINT nID)
{
	// 현재 변경된 화면이 해당 버튼에 해당된다면 버튼의 색을 달리 해준다.
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH_SHUTTLE_1, ((nID == eFORM_Teach_Shuttle_1) ?	GXCOLOR_ON : GXCOLOR_OFF));
	//kjpark 20170902 티칭 UI 추가 및 상단탭 동작 
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH_SHUTTLE_2, ((nID == eFORM_Teach_Shuttle_2) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH_SHUTTLE_3, ((nID == eFORM_Teach_Shuttle_3) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_HTEACH_PDT_IF, ((nID == eFORM_Teach_PDT_IF) ?	GXCOLOR_ON : GXCOLOR_OFF));

}


//kjpark 20170902 티칭 UI 추가 및 상단탭 동작 
void CDlgbarHTeach::ClickBtnxShuttle_1()
{
	if(theProcBank.MachineIsRunState() == FALSE)
	{
		ChangeForm( eFORM_Teach_Shuttle_1 );
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("SHUTTLE 1"));
	}
}

//kjpark 20170902 티칭 UI 추가 및 상단탭 동작 
void CDlgbarHTeach::ClickBtnxShuttle_2()
{
	if(theProcBank.MachineIsRunState() == FALSE)
	{
		ChangeForm( eFORM_Teach_Shuttle_2 );
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("SHUTTLE 2"));
	}
}

//kjpark 20170902 티칭 UI 추가 및 상단탭 동작 
void CDlgbarHTeach::ClickBtnxShuttle_3()
{
	if(theProcBank.MachineIsRunState() == FALSE)
	{
		ChangeForm( eFORM_Teach_Shuttle_3 );
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("SHUTTLE 3"));
	}
}



void CDlgbarHTeach::ClickBtnxHteachPdtIf()
{
	ChangeForm( eFORM_Teach_PDT_IF );
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH"), _T("PDT IF"));
}
