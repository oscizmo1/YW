// DlgbarTop.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarTop.h"

//GxGUIFrame 사용
#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "BankGuild\MCDefine.h"
#include "MainFrm.h"
#include "UI\Dialog\DlgUserLogin.h"
#include "UI\Dialog\DIgCimMsg.h"
#include "UI\Dialog\DIgRecipeSelect.h"
#include "UI\GausGUI\GxMsgBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
// CDlgbarTop 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgbarTop, CDialogBar)


enum
{
	TIMER_TMID_LOGOUT_CHECK = 200
};

CDlgbarTop::CDlgbarTop()
{

}

CDlgbarTop::~CDlgbarTop()
{
}

void CDlgbarTop::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarTop, CDialogBar)
	// ON_BN_CLICKED(IDC_BTN_BOTTOM_MENU1, OnBtnClickMenu1)
	// ON_UPDATE_COMMAND_UI(IDC_BTN_BOTTOM_MENU1, OnUpdateCmdUI)

	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()


	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	//kjpark 20160919 Topdlg에 connect status 
	ON_MESSAGE(UDMSG_COMM_STATE,			OnChangeCommState)		//  통신 상태가 변경되었다.
	ON_MESSAGE(UDMSG_STATE_CHANGE,			OnChangeMachineState)		//  설비 상태가 변경되었다.
	ON_MESSAGE(UDMSG_CIM_STATE_CHANGE,		OnChangeCimState)		//  CIM 상태가 변경되었다.
	ON_MESSAGE(UDMSG_CIM_EQIP_ID_CHANGE,	OnChangeEqipIDState)		//  CIM 상태가 변경되었다.
	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	ON_MESSAGE(MSG_CIM_MESSAGE_UPDATE,		OnChangeCIMMessageState)		// 인터락, 오피콜등 상태가 변경되었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// Active-X Event
BEGIN_EVENTSINK_MAP(CDlgbarTop, CDialogBar)
	ON_EVENT(CDlgbarTop, IDC_LBLX_TOPBAR_CLOCK, DISPID_CLICK, CDlgbarTop::ClickLblxTopbarClock, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXBTN_TOPBAR_LOGIN1, DISPID_CLICK, CDlgbarTop::ClickProductLogin, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXBTN_TOPBAR_OPERATOR_LOGIN, DISPID_CLICK, CDlgbarTop::ClickInSepectionLogin, VTS_NONE)

	ON_EVENT(CDlgbarTop, IDC_GXSTC_TOPBAR_MODEL, DISPID_CLICK, CDlgbarTop::ClickGxstcTopbarModel, VTS_NONE)
	ON_EVENT(CDlgbarTop, IDC_GXSTC_TOPBAR_CIM_LABEL, DISPID_CLICK, CDlgbarTop::ClickGxstcTopbarCimLabel, VTS_NONE)
END_EVENTSINK_MAP()


// CDlgbarTop 메시지 처리기입니다.
void CDlgbarTop::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
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
LRESULT CDlgbarTop::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		SetTimer(100, 1000, NULL);				// 시계를 표시하는 타이머를 구동시켜준다.

		if(theConfigBank.m_System.m_bInlineMode == FALSE)
		{
			SetTimer(TIMER_TMID_LOGOUT_CHECK, 500, NULL);	
		}
	}
	else
	{
		KillTimer(100);							// 시계 표시 타이머 중지
		KillTimer(TIMER_TMID_LOGOUT_CHECK);	
	}

	return 0;
}

// 메인 메뉴 선택 화면 전환 완료
LRESULT CDlgbarTop::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	// ToDo : 화면을 표시하기전에 하위 메뉴에 대한 동작을 기술

	InitializeVer();		//	버전표시

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CDlgbarTop::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_LBLX_TOPBAR_CLOCK);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 왼쪽 버튼을 누른 채 드래그하면 윈도우의 위치를 변경할 수 있다.
void CDlgbarTop::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
   CWnd *pWnd = AfxGetMainWnd();
   HWND hUIWnd = pWnd->m_hWnd;
   ::DefWindowProc(hUIWnd, WM_SYSCOMMAND, SC_MOVE+1,MAKELPARAM(point.x,point.y));   

	CDialogBar::OnLButtonDown(nFlags, point);
}


// 시계표시 타이머 동작
void CDlgbarTop::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ( nIDEvent == 100 )
	{
		CString sTime;
		CString sVetClock;
		CString sRecipe;
		SYSTEMTIME tmKorNow;
		::GetLocalTime( &tmKorNow );			// 현재 시각
		CTime tmVetNow = (CTime)tmKorNow;		// [W] 베트남 시간 적용
		tmVetNow -= CTimeSpan(0,2,0,0);
		sRecipe=theRecipeBank.m_strRecipeName; // 현재 사용중인 레시피
		// 시각 표시
		sTime.Format(_T("%04u-%02u-%02u\n K: %02u:%02u:%02u\n V: %02u:%02u:%02u"), 
			tmKorNow.wYear, tmKorNow.wMonth, tmKorNow.wDay, tmKorNow.wHour, tmKorNow.wMinute, tmKorNow.wSecond, 
			tmVetNow.GetHour(),tmVetNow.GetMinute(), tmVetNow.GetSecond());
		CGxUICtrl::SetStaticString(this, IDC_LBLX_TOPBAR_CLOCK, sTime);				// 현재 시각을 표시한다.

		//현재 사용중인 레시피를 보여준다.
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_MODEL_TEXT, sRecipe);			
	}

	else if ( nIDEvent == TIMER_TMID_LOGOUT_CHECK )
	{
		// 아무도 로그인 안되있으면 로그인 창 띄우고 접근 불가 [9/29/2017 OSC]
		KillTimer(nIDEvent);
		if(GetMainHandler()->m_pGUIFrame->GetCurrentViewID() != eFORM_PM)
		{
			if( (GetMainHandler()->GetCurEngeneerUserOperatorData().nLevel == eUSER_NONE)
				&& (GetMainHandler()->GetCurOperatorUserInspectorData().nLevel == eUSER_NONE) )
			{
				ClickInSepectionLogin();
			}
		}

		SetTimer(nIDEvent, 500, NULL);
	}

	CDialogBar::OnTimer(nIDEvent);
}


// 시계를 클릭하면 창을 최소화 시킨다.
void CDlgbarTop::ClickLblxTopbarClock()
{
	// 창 최소화 메세지 전송
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("CLOCK"));
}

// 윈도우 생성시 테두리 제거 작업
BOOL CDlgbarTop::PreCreateWindow(CREATESTRUCT& cs)
{
	GxDEF_REMOVE_FRAME

	return CDialogBar::PreCreateWindow(cs);
}

//kjpark 20160919 Topdlg에 connect status 
// 통신 상태가 변경되었다.
LRESULT CDlgbarTop::OnChangeCommState(WPARAM wParam, LPARAM lParam)
{
	UINT nID = 0;
	BOOL bFlag;

	for (int i = eCOMM_SignalPC; i < eMAX_COMMCount; i++)
	{
		nID = 0;
		switch ( i )
		{
		//case eCOMM_AVCamera :	nID = IDC_GXSTC_TOPBAR_VISION; break;			// AVCamera PC와의 통신
		case eCOMM_SignalPC :	nID = IDC_GXSTC_TOPBAR_PG; break;			// 신호기 PC
		case eCOMM_DataPC :		nID = IDC_GXSTC_TOPBAR_CIM; break;			// 데이터 PC 와의 통신
		case eCOMM_ALIGNPC :	nID = IDC_GXSTC_TOPBAR_ALIGN; break;//ALIGN PC 

		//case eCOMM_MCR_1 :		nID = IDC_GXSTC_TOPBAR_MCR_SHUTTL_1; break;			// UVMCR #1과의 통신
		//case eCOMM_MCR_2 :		nID = IDC_GXSTC_TOPBAR_MCR_SHUTTL_2; break;			// UVMCR #2과의 통신
		//case eCOMM_MCR_3 :		nID = IDC_GXSTC_TOPBAR_MCR_SHUTTL_3; break;			// UVMCR #3과의 통신
		//case eCOMM_MCR_4 :		nID = IDC_GXSTC_TOPBAR_MCR_SHUTTL_4; break;			// UVMCR #4과의 통신
		//case eCOMM_MCR_5 :		nID = IDC_GXSTC_TOPBAR_MCR_SHUTTL_5; break;			// UVMCR #5과의 통신
		//case eCOMM_MCR_6 :		nID = IDC_GXSTC_TOPBAR_MCR_SHUTTL_6; break;			// UVMCR #6과의 통신
		
			
		case eCOMM_AccuraGPS:	nID = 0; break;		// GPS 전력량계, 현재 미할당
		case eCOMM_AccuraUPS:	nID = 0; break;		// UPS 전력량계, 현재 미할당
		}

		// 잘못된 통신 Index는 Skip
		if ( nID <= 0 ) continue;
		//GetSocketIndex
		CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
		//kjpark 20161109 Defect Sockt 구현
		if(i == eCOMM_SignalPC && theSocketInterFace.m_PGHost.m_Socket.IsConnected() == TRUE)
		{
			bFlag = theSocketInterFace.m_PGHost.IsConnected();
		}
		else if(i == eCOMM_DataPC)
		{
			bFlag = theSocketInterFace.m_CIM.IsConnected();
		}
		else if(i == eCOMM_ALIGNPC)
		{
			bFlag = theSocketInterFace.m_ActiveAlign.IsConnected();
		}
		else
			bFlag = theSocketInterFace.GetCommConnected(i);		
		CGxUICtrl::SetStaticColor(this, nID, bFlag ? GXCOLOR_ON : GXCOLOR_RED);
	}//of for i

	return 0;
}


LRESULT CDlgbarTop::OnChangeMachineState( WPARAM wParam, LPARAM lParam )
{
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_STATE, ConstMachineState[theProcBank.GetMachineState()].strName);
 	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_PAUSE)
	{
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_STATE, GXCOLOR_OFF);
	}
	else if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_INIT)
	{
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_STATE, Color::Blue);
	}
	else if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_STATE, Color::Lime);
	}
	else if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_FAULT)
	{
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_STATE, GXCOLOR_RED);
	}
	else if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN)
	{
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_STATE, Color::Lime);
	}
	else
	{
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_STATE, Color::Yellow);

	}
	return 0;
}
LRESULT CDlgbarTop::OnChangeCimState( WPARAM wParam, LPARAM lParam )
{
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_CIM_STATE, ConstCimState[theProcBank.GetCimState()].strName);

	//kjpark 20180107 Remote Mode 가 아니면 팝업 발생
	if(theProcBank.GetCimState() != CONST_CIM_STATE::CIM_REMOTE && theProcBank.MachineIsRunState() == TRUE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("REMOTE Change To OFFLINE"), 
			_T("REMOTE Change To OFFLINE"), 
			_T("REMOTE Change To OFFLINE"),  GetMainHandler()->m_nLangIdx );

		dlgMsgBox.DoModal();	
	}

	return 0;
}

LRESULT CDlgbarTop::OnChangeEqipIDState( WPARAM wParam, LPARAM lParam )
{
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_CIM_LABEL, theProcBank.m_strEqipID);
	return 0;
}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
LRESULT CDlgbarTop::OnChangeCIMMessageState( WPARAM wParam, LPARAM lParam )
{
	switch((CIM_MESSAGE_STATE)lParam)
	{
	case CIM_MESSAGE_INTERLOCK:
 		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_INTERLOCK, GXCOLOR_RED);
 		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_OPCALL, GXCOLOR_WHITE);
 		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_TERMINAL, GXCOLOR_WHITE);
 		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_JOBSTART, GXCOLOR_WHITE);
		break;
	case CIM_MESSAGE_OPCALL:
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_INTERLOCK, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_OPCALL, GXCOLOR_RED);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_TERMINAL, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_JOBSTART, GXCOLOR_WHITE);
		break;
	case CIM_MESSAGE_TEMINAL:
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_INTERLOCK, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_OPCALL, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_TERMINAL, GXCOLOR_RED);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_JOBSTART, GXCOLOR_WHITE);
		break;
	case CIM_MESSAGE_JOBPROCESS:
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_INTERLOCK, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_OPCALL, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_TERMINAL, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_JOBSTART, GXCOLOR_RED);
		break;
	default:
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_INTERLOCK, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_OPCALL, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_TERMINAL, GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, IDC_GXSTC_TOPBAR_JOBSTART, GXCOLOR_WHITE);
		break;
	}	
	return 0;
}

void CDlgbarTop::InitializeVer()				//	버전 가져와서 보여주기
{
	//	IDC_GXSTC_TOPBAR_VER_TEXT

	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);		//버전 리소스에서 정보를 가져옴

	if (hRsrc != NULL)
	{
		HGLOBAL hGlobalMemory = LoadResource(NULL, hRsrc);			//	글로벌핸들에 리소스넣음
		if (hGlobalMemory != NULL)
		{
			void *pVersionResouece = LockResource(hGlobalMemory);	//	핸들에서 정보 긁어옴
			void *pVersion;
			UINT uLength;

			// 아래줄에 041204B0는 리소스 파일(*.rc)에서 가져옴.

			if( VerQueryValue(pVersionResouece, _T("StringFileInfo\\041204B0\\ProductVersion"), &pVersion, &uLength) != 0 )
			{
				CString szVer = reinterpret_cast<wchar_t*>(pVersion);

				//CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_VER_TEXT, szVer);				// 버전을 표시함
				CGxUICtrl::SetStaticString(this, IDC_GXSTC_TOPBAR_VER_TEXT, PROGRAM_VERSION);				// 버전을 표시함
								
			}
			FreeResource(hGlobalMemory);
		}
	}

}
//엔지니어
void CDlgbarTop::ClickProductLogin()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	//kjpark 20170920 LOGIN/OUT 구현
	if(pMain->GetCurEngeneerUserOperatorData().sID.IsEmpty())
	{
		CDlgUserLogin dlgLogin;

		dlgLogin.SetEngineerMode(TRUE);
		if ( dlgLogin.DoModal() == IDOK )
		{
			if(dlgLogin.IsEngineerMode())
			{
				CGxUICtrl::SetStaticString(this, IDC_GXBTN_TOPBAR_LOGIN_BZONE, pMain->GetCurEngeneerUserOperatorData().sID);
				theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("Engineer LOGIN"));
			}
			else
			{
				theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("Operator LOGIN"));
			}
		}
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("Engineer 로그아웃 하시겠습니까?"), 
			_T("Logout Engineer now?"),		
			_T("Ban muon Logout Engineer bay gio?"), GetMainHandler()->m_nLangIdx );

		int nRet = dlgMsgBox.DoModal();
		if(nRet != IDOK)
			return;
		CUserAccount user;
		pMain->SetCurUserOperatorData(user);
		CGxUICtrl::SetStaticString(this, IDC_GXBTN_TOPBAR_LOGIN_BZONE, _T("LOGOUT"));
	}
}

//오퍼레이터
void CDlgbarTop::ClickInSepectionLogin()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain->GetCurOperatorUserInspectorData().sID.IsEmpty())
	{
		CDlgUserLogin dlgLogin;

		dlgLogin.SetEngineerMode(FALSE);
		if ( dlgLogin.DoModal() == IDOK )
		{
			if(dlgLogin.IsEngineerMode())
			{
				CGxUICtrl::SetStaticString(this, IDC_GXBTN_TOPBAR_LOGIN_BZONE, pMain->GetCurEngeneerUserOperatorData().sID);
				theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("Engineer LOGIN"));
			}
			else
			{
				theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("Operator LOGIN"));
			}
		}
	}
	else
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(TRUE, 
			_T("검사자 로그아웃 하시겠습니까?"), 
			_T("Logout Inspector now?"),		
			_T("Logout Inspector now?") , GetMainHandler()->m_nLangIdx);

		int nRet = dlgMsgBox.DoModal();
		if(nRet != IDOK)
			return;
		theSocketInterFace.m_CIM.SendCmdLoginToDataPC(EQUIP_SET_USER_LOGOUT_REQUEST, 
		//kjpark 20170925 LOGOUT 시 로그인 정보 삭제 안함
		pMain->GetCurOperatorUserInspectorData().sID, pMain->GetCurOperatorUserInspectorData().sPassword);
		//CGxUICtrl::SetStaticString(this, IDC_GXBTN_TOPBAR_LOGINER2, _T("LOGOUT"));
	}
}


void CDlgbarTop::ClickGxstcTopbarModel()
{
	CMainFrame* pMain = GetMainHandler();

	if(theProcBank.MachineIsRunState())
		return;
	// 1. 로그아웃 실행
	/// 2. 유저가 관리자로 로그인을 하면
	//kjpark 20170920 LOGIN/OUT 구현
	if(pMain->GetCurEngeneerUserOperatorData().sID.IsEmpty())
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Notify"), _T("Notify"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Engineer 로그인해 주세요."), 
			_T("Please loginEngineer operator"),		
			_T("Please login Engineer operator") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click"), _T("Model Change"));
	CDIgRecipeSelect dlgRecipeSel;

	INT_PTR iRet = dlgRecipeSel.DoModal();
// 	int iPgCount = (MAX_IN_ID % 16 == 0) ? MAX_IN_ID/16 : MAX_IN_ID/16 + 1;
// 
// 	dlgPgIn.SetBoolInput(TRUE);
// 	dlgPgIn.SetPageCount( iPgCount );
// 
// 	INT_PTR iRet = dlgPgIn.DoModal();
}


void CDlgbarTop::ClickGxstcTopbarCimLabel()
{
	return;
//  	theProcBank.m_strOPCallID=_T("aaaa");
//  	theProcBank.m_strOPCallMsg=_T("TEST_OPCALL_1234567890_ABCDEFG_____________________________________________________________________________________--end), OPCallMsg(TEST_OPCALL_1234567890_ABCDEFG_____________________________________________________________________________________--end");
//  	theProcBank.SetOPCall();
	
	
}