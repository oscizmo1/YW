
// DlgbarBottom.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarBottom.h"


#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgbarBottom 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgbarBottom, CDialogBar)

CDlgbarBottom::CDlgbarBottom()
{	
	m_pMain = NULL;
}

void CDlgbarBottom::OnTimer(UINT_PTR nIDEvent)
{

	CDialogBar::OnTimer(nIDEvent);
}

CDlgbarBottom::~CDlgbarBottom()
{
}

void CDlgbarBottom::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarBottom, CDialogBar)
	// ON_BN_CLICKED(IDC_BTN_BOTTOM_MENU1, OnBtnClickMenu1)
	// ON_UPDATE_COMMAND_UI(IDC_BTN_BOTTOM_MENU1, OnUpdateCmdUI)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)				// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)				// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	ON_MESSAGE(UDMSG_STATE_CHANGE,			OnChangeMachineState)		//  설비 상태가 변경되었다.
	ON_WM_TIMER()
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// ActivX Event 처리
BEGIN_EVENTSINK_MAP(CDlgbarBottom, CDialogBar)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU1, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu1, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU2, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu2, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU3, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu3, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU4, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu4, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU5, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu5, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU6, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu6, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_MENU7, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomMenu7, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_QUIT, DISPID_CLICK, CDlgbarBottom::OnClickBtnxBottomQuit, VTS_NONE)
	ON_EVENT(CDlgbarBottom, IDC_BTNX_BOTTOM_LANG, DISPID_CLICK, CDlgbarBottom::ClickBtnxBottomLang, VTS_NONE)
END_EVENTSINK_MAP()




// CDlgbarBottom 메시지 처리기입니다.
void CDlgbarBottom::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

BOOL CDlgbarBottom::PreCreateWindow(CREATESTRUCT& cs)
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

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CDlgbarBottom::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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


// 화면 변경이 완료되었다.
LRESULT CDlgbarBottom::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	m_pMain = GetMainHandler();

	DispCurrentSelect( (UINT)wParam );		// 선택된 메뉴 반전

	//20161212 BKH, 쓰레드에서 체킹하는 걸로 변경
	//UpdateRunBtnState();
	MainMenuEnableChange();
	
	return 0;
}


// 표시 언어가 변경되었다.
LRESULT CDlgbarBottom::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	// 국기를 표시하는 버튼은 특별히 처리를 해준다.
	CGxUICtrl::SetButtonIcon(this, IDC_BTNX_BOTTOM_LANG, wParam);	// 각 언어의 Index에 해당하는 아이콘을 표시한다.
	theConfigBank.SaveLastDataFile();
	return 0;
}


LRESULT CDlgbarBottom::OnChangeMachineState(WPARAM wParam, LPARAM lParam)
{
	MainMenuEnableChange();
	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 지정 화면으로 폼을 변경한다.
void CDlgbarBottom::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 화면 전환을 지시한다.
	// ::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);		// #include "MainFrm.h" 필요
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}

// 현재 선택된 버튼의 색을 달리해준다.
void CDlgbarBottom::DispCurrentSelect(UINT nID)
{
	// 현재 변경된 화면이 해당 버튼에 해당된다면 버튼의 색을 달리 해준다.

	// 단일 화면 메뉴의 경우'

	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU1, ((nID == eFORM_Main) ?		GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU2, ((nID == eFORM_Alarm) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU7, ((nID == eFORM_Recipe) ?	GXCOLOR_ON : GXCOLOR_OFF));

	// 하위 메뉴가 있는 경우
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU3, ((nID >= eFORM_Report1 && nID <= eFORM_Report6) ? GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU4, ((nID >= eFORM_Teach_Shuttle_1	&& nID <= eFORM_Teach_PDT_IF) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU5, ((nID >= eFORM_Setup1	&& nID <= eFORM_Setup4) ?	GXCOLOR_ON : GXCOLOR_OFF));
	CGxUICtrl::SetButtonColor(this, IDC_BTNX_BOTTOM_MENU6, ((nID >= eFORM_Config_Option	&& nID <= eFORM_Config_NG_Option) ?	GXCOLOR_ON : GXCOLOR_OFF));	//[W]


	
}



// 메인 화면 선택
void CDlgbarBottom::OnClickBtnxBottomMenu1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ChangeForm( eFORM_Main );	
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("MAIN"));// Main 화면
}


// Alarm 화면
void CDlgbarBottom::OnClickBtnxBottomMenu2()
{
	ChangeForm( eFORM_Alarm);		
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("ALARM"));// Alarm 표시 화면
}
// Report 화면
void CDlgbarBottom::OnClickBtnxBottomMenu3()
{	
	ChangeForm(m_pMain->GetCurPageRep());	
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("REPORT"));
	// 각종 Report 표시 화면
}

// Teach 화면
void CDlgbarBottom::OnClickBtnxBottomMenu4()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"),  _T("TEACH"));
	if(theProcBank.MachineIsRunState())
		ChangeForm(eFORM_Teach_PDT_IF);	
	else
		ChangeForm(m_pMain->GetCurPageTea1());	
	// 모듈별 티칭 화면
}

// Setup 화면
void CDlgbarBottom::OnClickBtnxBottomMenu5()
{
	ChangeForm(m_pMain->GetCurPageSet());	
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("SETUP"));// 디바이스 설정 화면
}

// Config 화면
void CDlgbarBottom::OnClickBtnxBottomMenu6()			
{	
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("CONFIG"));
	if(theProcBank.MachineIsRunState())
		ChangeForm(eFORM_Config_Option);	
	else
		ChangeForm(m_pMain->GetCurPageCon());	
}


//  화면
void CDlgbarBottom::OnClickBtnxBottomMenu7()
{
	ChangeForm( eFORM_Recipe );	
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("RECIPE"));
	// 레시피 등록,편집 화면
}


// 종료
void CDlgbarBottom::OnClickBtnxBottomQuit()
{
	// 종료하도록 이벤트를 보낸다.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, WM_CLOSE, 0, 0);
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("QUIT"));
}



// 표시되는 언어를 변경해준다.
void CDlgbarBottom::ClickBtnxBottomLang()
{
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();

	pMainFrm->m_nLangIdx = (LANGUAGE)(( (int)pMainFrm->m_nLangIdx + 1 ) % 3);							// 표시되는 언어 인덱스, 0:한국어, 1:영어, 2:베트남어 ...

	// 표시 언어가 변경되었다고 알려준다.
	::PostMessage(pMainFrm->m_hWnd, GxMSG_GUI_REQUEST_LANGUAGE, pMainFrm->m_nLangIdx, 0);
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("CHANGE LANGUAGE"));
}




/*
	// 모든 View와 Dialog에 표시 언어를 변경하도록 전파한다.
	m_pGUIData->SendMessageView(FALSE, UM_DISP_LANG_CHANGE, wParam, lParam);
	m_pGUIData->SendMessageDialog(FALSE, UM_DISP_LANG_CHANGE, wParam, lParam);
	m_pGUIData->SendMessageDialogBar(FALSE, UM_DISP_LANG_CHANGE, wParam, lParam);

	*/

void CDlgbarBottom::MainMenuEnableChange()
{
	if(m_pMain == NULL)
		return;
	if(theProcBank.MachineIsRunState())
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU1, TRUE);	// Main
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU2, TRUE);	// Alarm
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU3, TRUE);	// Report
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU7, FALSE);	// Recipe
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU4, TRUE);	// Teach1		
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU5, FALSE);		// Setup
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU6, TRUE);		// Config
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_QUIT, FALSE);		// Quit
	}
	else if(m_pMain->getModePM())
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU1, FALSE);	// Main
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU2, FALSE);	// Alarm
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU3, FALSE);	// Report
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU7, FALSE);	// Recipe
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU4, FALSE);	// Teach1
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU5, FALSE);		// Setup
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU6, FALSE);		// Config
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_QUIT, TRUE);		// Quit
	}
	else if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT)
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU1, FALSE);	// Main
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU2, FALSE);	// Alarm
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU3, FALSE);	// Report
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU7, FALSE);	// Recipe
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU4, FALSE);	// Teach1
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU5, FALSE);		// Setup
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU6, FALSE);		// Config
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_QUIT, FALSE);		// Quit
	}
	else
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU1, TRUE);	// Main
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU2, TRUE);	// Alarm
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU3, TRUE);	// Report
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU7, TRUE);	// Recipe
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU4, TRUE);	// Teach1
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU5, TRUE);		// Setup
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_MENU6, TRUE);		// Config
		CGxUICtrl::SetButtonEnabled(this, IDC_BTNX_BOTTOM_QUIT, TRUE);		// Quit
	}
}
