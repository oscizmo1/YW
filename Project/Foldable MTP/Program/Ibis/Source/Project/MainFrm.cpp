
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "IbisApp.h"
#include "MainFrm.h"


#include "UI\GausGUI\GxUICtrl.h"

// DLL에 존재하는 각종 Dialog 사용
#include "UI\GausGUI\GxNumPad.h"
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\GausGUI\GxMsgBox.h"


// 관리할 FormView / Dialog header file
#include "GUIDefine.h"									// 화면 Index 정의


#include "UI\Form\FormMain.h"
#include "UI\Form\FormPM.h"
#include "UI\Form\FormReport1.h"
//kjaprk 20170904 통계 UI 12시간 타임으로 변경
#include "UI\Form\FormReport1_Sub.h"
#include "UI\Form\FormReport2.h"
//kjaprk 20170911 통계 Alarm UI 12시간 타임으로 변경
#include "UI\Form\FormReport2_Sub.h"
#include "UI\Form\FormReport3.h"
#include "UI\Form\FormReport4_Opcall_History.h"
//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
#include "UI\Form\FormReport5_Interlock_History.h"
#include "UI\Form\FormReport6_Terminel_History.h"
//kjpark 20180114 NG 판정 통계 UI 추가
#include "UI\Form\FormReportStatisticsNGLastResult.h"
#include "UI\Form\FormSignal.h"
#include "UI\Form\FormSetupIO.h"
#include "UI\Form\FormInitialize.h"
#include "UI\Form\FormMotorUnitTest.h"
#include "UI\Form\FormTeach_Shuttle_1.h"
//kjpark 20170902 티칭 UI 추가 및 상단탭 동작 
#include "UI\Form\FormTeach_Shuttle_2.h"
#include "UI\Form\FormTeach_PDT_IF.h"
#include "UI\Dialog\DlgbarJog.h"
#include "UI\Dialog\DlgbarBottom.h"
#include "UI\Dialog\DlgbarTop.h"
#include "UI\Dialog\DlgbarHReport.h"			// 상단 서브 메뉴
#include "UI\Dialog\DlgbarHSetup.h"
#include "UI\Dialog\DlgbarHTeach.h"
#include "UI\Dialog\DlgConfirm.h"
#include "UI\Dialog\DlgInform.h"
#include "UI\Dialog\DlgInitial.h"
#include "UI\Dialog\DlgMsgInterlock.h"
#include "UI\Dialog\DlgMsgUnitInerlock.h"
#include "UI\Dialog\DlgMsgOPCall.h"
#include "UI\Dialog\DlgMsgTerminal.h"
#include "UI\Form\FormAlarm.h"
#include "UI\Form\FormRecipe.h"
#include "UI\Dialog\DlgbarHConfig.h"
#include "UI\Form\FormAlarmLamp.h"
#include "UI\Form\FormConfig2.h"
#include "UI\Form\FormConfigRepeatNgOption.h"
#include "UI\Form\FormOptions.h"
#include "UI\Form\FormCIM.h"
#include "UI\Dialog\DlgDoor.h"





#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()

	// 사용자 정의 메세지 처리
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW,		OnViewChange)					// 메인 화면을 변경하라는 지령
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnViewChangeEnd)				// 메인 화면이 변경되었다.
	ON_MESSAGE(GxMSG_GUI_REQUEST_LANGUAGE,	OnChangeRequestLanguage)	// 표시 언어를 바꾸라는 요청
	//kjpark 20160919 Topdlg에 connect status 
	ON_MESSAGE(UDMSG_COMM_STATE,			OnChangeCommState)					//  통신 상태가 변경되었다.
	//20160930 kjpark Serial Data 메세지 방식 구현
	ON_MESSAGE(WM_SERIAL,	OnMSGEvent)	
	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	ON_WM_TIMER()

	//kjpark 20161025 MCR 구현
	ON_MESSAGE(WM_UPDATE_MCRCONNECT,	OnUpdateMCRConnect)			// MCR이 연결/해제 되었다.
	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	ON_MESSAGE(WM_UPDATE_MCRDATA_CH1,		OnUpdateMCRData_CH1)			// MCR이 수신되었다.
	ON_MESSAGE(WM_UPDATE_MCRDATA_CH2,		OnUpdateMCRData_CH3)			// MCR이 수신되었다.

	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	ON_MESSAGE(MSG_CIM_MESSAGE_UPDATE,			OnChangeCIMMessageState)

	ON_MESSAGE(MSG_WARNING_SHOW,			OnWarningShow)

	ON_MESSAGE(MSG_SHOW_DIALOG,			OnShowDialog)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_bOnlyOnce			= TRUE;
	m_bTerminateFlag	= FALSE;		// 프로그램을 종료해도 되는지 여부
	m_bModePM			= FALSE;		// PM 모드 여부.

	//kjpark 20161021 Setup 신호기 UI 구현
	m_nLangIdx			= LAN_KOR;	
													// 표시되는 언어 인덱스, 0:한국어, 1:영어, 2:베트남어 ...
	m_iCurPageRep = eFORM_Report1;
	m_iCurPageSet = eFORM_Setup1;
	m_iCurPageTea1 = eFORM_Teach_Shuttle_1;
	m_iCurPageCon = eFORM_Config_Option;

	m_iSelectedAxis = AXIS_ID_MAX;
	m_iTeachAxis = TEACH_PARAM::PARAM_MAX;
	m_dTeachOffset = 0.;

	//kjpark 20161019 양수버튼 체크
	bZone_L_TimeCk = 0;
	bZone_R_TimeCk = 0;
	dZone_L_TimeCk = 0;
	dZone_R_TimeCk = 0;

	m_bAZoneOperatorReady[JIG_ID_A] = FALSE;
	m_bAZoneOperatorReady[JIG_ID_B] = FALSE;
	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	m_bTeachMode = FALSE;
	//20170412 kjpark OTP REG, ID Check 끝날때 까지 Next 못누름
	//m_bOTPCheck_Finish = FALSE;

	m_bLamp_FLick = TRUE;
	//SerialOpen();
	m_bSTODetectFlag = FALSE;


	m_bEnableLightCurtain = FALSE;
	m_checkAxisLightCurtain = AXIS_ID_MAX;

	m_bBackDoorOpen = FALSE;

	m_bWarningMsgShow =FALSE;
}


CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// Set Frame size & Remove Menu
	//kjpark 20170828 MoveWindow 적용
	cs.x			= (int)theConfigBank.m_System.m_dDefaultWindow_X;											// 화면 시작위치
	cs.y			= 0;
	cs.cx			= 1280;											// 화면 크기
	cs.cy			= 1024;
	cs.hMenu		= NULL;											// 메뉴 제거
	// OLD cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME;
	cs.style		= WS_OVERLAPPED | WS_POPUP;				// 윈도우 테두리 및 타이틀바를 없앤다.
	cs.lpszName	= _T("YW Ibis");							// 타이틀바에 표시되는 프로그램 이름
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	// Icon 변경
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT);	// IDR_MAINFRAME); 				// Icon
	SetIcon(hIcon, FALSE);

	// 3D 효과를 제거하여 완전한 Client 영역을 확보한다.
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED); 


	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	
	// GxGUIFrame객체를 생성한다.
	m_pGUIFrame = CGxGUIFrame::GetInstance();

	// Dialogbar를 추가한다.
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_Top,		CDlgbarTop::IDD,		new CDlgbarTop(),			CGxGUIFrame::ePOS_DlgBarTop);		// 상위 메뉴
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_Bottom,	CDlgbarBottom::IDD,		new CDlgbarBottom(),		CGxGUIFrame::ePOS_DlgBarBottom);	// 하단 메뉴

	// 왼쪽 측면 서브 메뉴 예
	// m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubReport,CDlgbarVReport::IDD,	new CDlgbarVReport(),	CGxGUIFrame::ePOS_DlgBarLeft);	// Report 하위 메뉴 선택 (왼쪽 메뉴)

	//상단면 서브 메뉴 예
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubReport,CDlgbarHReport::IDD,	new CDlgbarHReport(),		CGxGUIFrame::ePOS_DlgBarTop);		// Report 하위 메뉴 선택 (상부 메뉴)
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubTeach, CDlgbarHTeach::IDD,		new CDlgbarHTeach(),		CGxGUIFrame::ePOS_DlgBarTop);		// Teaching 하위 메뉴 선택
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubSetup, CDlgbarHSetup::IDD,		new CDlgbarHSetup(),		CGxGUIFrame::ePOS_DlgBarTop);		// Setup 하위 메뉴 선택
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_SubConfig,CDlgbarHConfig::IDD,	new	CDlgbarHConfig(),		CGxGUIFrame::ePOS_DlgBarTop);		// Config 하위 메뉴 선택 [W]
	m_pGUIFrame->AddDlgBar( this, eDLGBAR_Jog,		CDlgbarJog::IDD,		new CDlgbarJog(),			CGxGUIFrame::ePOS_DlgBarRight);	// Jog 메뉴 선택

	// m_pGUIFrame->AssignDefaultView() 함수는 OnActivate() 함수에서 호출해준다.
	m_pGUIFrame->AddView(this, eFORM_Main,		new CFormMain(),		eDLGBAR_None,			eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Alarm,		new CFormAlarm(),		eDLGBAR_None,			eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_PM,		new CFormPM(),			eDLGBAR_None,			eDLGBAR_None,	TRUE);	
	m_pGUIFrame->AddView(this, eFORM_Recipe,	new CFormRecipe(),		eDLGBAR_None,			eDLGBAR_None,	TRUE);	//	[W]	레시피 페이지

	m_pGUIFrame->AddView(this, eFORM_Alarm,	new CFormAlarm(),		eDLGBAR_None,		eDLGBAR_None,	TRUE);	// [W] 알람 페이지

	//kjaprk 20170904 통계 UI 12시간 타임으로 변경
	if(theConfigBank.m_System.m_bWorkPartTime)
		m_pGUIFrame->AddView(this, eFORM_Report1,	new CFormReport1_Sub(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);	// 하위 메뉴 사용, 부가 기능은 없음
	else
		m_pGUIFrame->AddView(this, eFORM_Report1,	new CFormReport1(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);	// 하위 메뉴 사용, 부가 기능은 없음

	//kjpark 20180114 NG 판정 통계 UI 추가
	m_pGUIFrame->AddView(this, eFORM_Report2,	new CFormReportStatisticsNGLastResult(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);	// 하위 메뉴 사용, 부가 기능은 없음

	//kjaprk 20170911 통계 Alarm UI 12시간 타임으로 변경
	if(theConfigBank.m_System.m_bWorkPartTime)
		m_pGUIFrame->AddView(this, eFORM_Report3,	new CFormReport2_Sub(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);	// 하위 메뉴 사용, 부가 기능은 없음
	else
		m_pGUIFrame->AddView(this, eFORM_Report3,	new CFormReport2(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);

	m_pGUIFrame->AddView(this, eFORM_Report4,	new CFormReport3(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Report5,	new CFormReport4_Opcall_History(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);
	//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
	m_pGUIFrame->AddView(this, eFORM_Report6,	new CFormReport5_Interlock_History(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Report7,	new CFormReport6_Terminel_History(),		eDLGBAR_SubReport,		eDLGBAR_None,	TRUE);

	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	m_pGUIFrame->AddView(this, eFORM_Teach_Shuttle_1,		new CFormTeach_Shuttle_1(),			eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);
	//kjpark 20170902 티칭 UI 추가 및 상단탭 동작 
	m_pGUIFrame->AddView(this, eFORM_Teach_Shuttle_2,		new CFormTeach_Shuttle_2(),			eDLGBAR_SubTeach,		eDLGBAR_Jog,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Teach_PDT_IF,		new CFormTeach_PDT_IF(),			eDLGBAR_SubTeach,		eDLGBAR_None,	TRUE);



	m_pGUIFrame->AddView(this, eFORM_Setup1,	new CFormSignal(),		eDLGBAR_SubSetup,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Setup2,	new CFormSetupIO(),		eDLGBAR_SubSetup,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Setup3,	new CFormInitialize(),	eDLGBAR_SubSetup,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Setup4,	new CFormMotorUnitTest(),		eDLGBAR_SubSetup,		eDLGBAR_None,	TRUE);

	m_pGUIFrame->AddView(this, eFORM_Config_Option,	new CFormOptions(),		eDLGBAR_SubConfig,		eDLGBAR_None,	TRUE);	//[W]
	m_pGUIFrame->AddView(this, eFORM_Config_Lamp,	new CFormAlarmLamp(),	eDLGBAR_SubConfig,		eDLGBAR_None,	TRUE);	//[W]
	m_pGUIFrame->AddView(this, eFORM_Config_CIM,	new CFormCIM(),	eDLGBAR_SubConfig,		eDLGBAR_None,	TRUE);	//[W]
	m_pGUIFrame->AddView(this, eFORM_Config_System,	new CFormConfig2(),		eDLGBAR_SubConfig,		eDLGBAR_None,	TRUE);
	m_pGUIFrame->AddView(this, eFORM_Config_NG_Option,	new CFormConfigNgOption(),		eDLGBAR_SubConfig,		eDLGBAR_None,	TRUE);
	


	// Dialog 추가
	m_pGUIFrame->AddDialog(this, eDLG_Confirm,			CDlgConfirm::IDD,			new CDlgConfirm());
	m_pGUIFrame->AddDialog(this, eDLG_Inform,			CDlgInform::IDD,			new CDlgInform());
	m_pGUIFrame->AddDialog(this, eDLG_Initial,			CDlgInitial::IDD,			new CDlgInitial());
	m_pGUIFrame->AddDialog(this, eDLG_MsgInterlock,		CDlgMsgInterlock::IDD,		new CDlgMsgInterlock());
	m_pGUIFrame->AddDialog(this, eDLG_MsgUnitInterlock,	CDlgMsgUnitInerlock::IDD,	new CDlgMsgUnitInerlock());
	m_pGUIFrame->AddDialog(this, eDLG_MsgOPCall,		CDlgMsgOPCall::IDD,			new CDlgMsgOPCall());
	m_pGUIFrame->AddDialog(this, eDLG_MsgTerminal,		CDlgMsgTerminal::IDD,		new CDlgMsgTerminal());


	// 생성된 기본적인 Dialogbar를 화면에 보여준다.
	m_pGUIFrame->ShowDlgBar( eDLGBAR_Top,		0 );				// 상부 Top menu
	m_pGUIFrame->ShowDlgBar( eDLGBAR_Bottom,	0 );				// 하단 Bottom Menu

	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	SetTimer(TMID_DOOR_INTERLOCK, 100, NULL);
	//kjparkk 20180131 PDT AB RULE 체크 기능 추가
#if 0
	SetTimer(TMID_PDT_AB_RULE_CHECK, 30*1000, NULL);	
#endif
	SetTimer(TMID_FLICK_LAMP,400,NULL);

	// 비가동 로그 07시 기준 시간 분리하기 위해 07시에 이전날짜 기록을 한다
	SetTimer(TMID_LOSS_TIME,60000,NULL);	// 1분마다...

	return 0;
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 생성하였던 GUI 관리용 객체를 파괴한다.
	m_pGUIFrame->ReleaseInstance();
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	// 최초 1회 실행
	if ( m_bOnlyOnce ) {
		m_bOnlyOnce = FALSE;

		// Start of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
		// 기본 View 지정
		m_pGUIFrame->AssignDefaultView();		// 기본적으로 생성되는 FormView Pointer를 획득한다.
		m_pGUIFrame->ChangeView(eFORM_Main, FALSE);	// 프로그램 처음 시작하면 Main 화면을 표시한다 [7/5/2016 OSC]
		// end of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

		SetMainWnd();

	}

}


void CMainFrame::OnClose()
{
	// 종료 여부를 아직 확인받지 않았다면
	if ( m_bTerminateFlag != TRUE )
	{
		// int nRet = AfxMessageBox(_T("Are you sure you want to exit ?"), MB_YESNO);
		//if ( nRet != IDYES ) return;		// 종료 취소

		CGxMsgBox	dlgMsgBox;
	
		//참고 dlgMsgBox.SetLangName(2, _T("CHN"));		// 해당 순번 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("시스템 종료"), _T("System Terminate"), _T("kết thúc chương trình"));
		dlgMsgBox.SetMessage(TRUE, 
						_T("프로그램을 종료하시겠습니까 ?"), 
						_T("Do you want to terminate program ?"),		
						_T("Bạn có muốn kết thúc chương trình ?"), m_nLangIdx );

		int nRet = dlgMsgBox.DoModal();

		// 종료를 원하지 않는다면 종료 처리 중지
		if ( nRet != IDOK ) return;

		AllCellTrackOut();
		// 
		m_bTerminateFlag = TRUE;			// 종료 가능
		for(int jig = 0; jig < JIG_ID_MAX; jig++)
		{
			for(int ch = 0; ch < JIG_CH_MAX; ch++)
			{
				if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader )
				{
					delete theSocketInterFace.m_MCR[jig][ch].m_pIDReader;
					theSocketInterFace.m_MCR[jig][ch].m_pIDReader =NULL;
				}
			}
		}

		// 별로 중요하지 않을때니 바꿀 때 일일이 저장하지 말고 마지막에만 저장하자
		theUnitStatusBank.SaveStatus();
		//////////////////////////////////////////////////////////////////////////
		// Release Thread
		for(int i = 0; i < THREAD_INDEX_MAX; i++)
			theThreadInitializer.ReleaseThread( (THREAD_INDEX)i );
		//////////////////////////////////////////////////////////////////////////
		// Close Device
		//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
		theSerialInterFace.SerialClose(SERIAL_LABELPRINT);		
		theDeviceMotion.Close_Mp2100();
		theDeviceMotion.Close_NMC2();
		theDeviceIO.Close();
		//////////////////////////////////////////////////////////////////////////
		// Delete User List
		ClearUserAccountMap();
	}

	CFrameWnd::OnClose();
}



// Message를 통해 화면 변경 요청이 들어왔다.
//
// wParam = MainMenu Select Button 
// lParam = SubMenu Select Button, 무시
//
LRESULT CMainFrame::OnViewChange(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;					// 변경하고자 하는 화면 Index
	BOOL bForce = (BOOL)lParam;				// 강제로 화면을 변경할 것인가 ?

	if ( m_pGUIFrame )
	{
		m_pGUIFrame->ChangeView( nIdx, bForce );			// 화면 변경 명령
	}

	return 0;
}

// Message를 통해 화면 변경 요청에 대한 수행 완료가 들어왔다.
//
// wParam = MainMenu Select Button 
// lParam = SubMenu Select Button, 무시
//
LRESULT CMainFrame::OnViewChangeEnd(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;					// 변경하고자 하는 화면 Index
	BOOL bForce = (BOOL)lParam;			// 강제로 화면을 변경할 것인가 ?

	if ( m_pGUIFrame )
	{
		// 메인/Sub 메뉴를 가지고 있는 모든 Dialog Bar에게 메뉴가 바뀌었음을 알려준다.
		m_pGUIFrame->SendMessageDlgBar(TRUE, GxMSG_GUI_CHANGE_VIEW_END, wParam, lParam);		// 지정 DialogBar에게 지정 Message를 보낸다.
	}

	return 0;
}


// 화면 표시 언어를 변경하도록 요청받았다. 
LRESULT CMainFrame::OnChangeRequestLanguage(WPARAM wParam, LPARAM lParam)
{
	// 등록된 모든 화면에 언어 변경 요청을 전송한다.
	if ( m_pGUIFrame )
	{
		m_pGUIFrame->SendMessageDialog(TRUE, GxMSG_GUI_CHANGE_LANGUAGE, wParam, lParam);		// Dialog들에게 표시 언어 변경을 통보한다.
		m_pGUIFrame->SendMessageDlgBar(TRUE, GxMSG_GUI_CHANGE_LANGUAGE, wParam, lParam);		// Dialog Bar 들에게 표시 언어 변경을 통보한다.
		m_pGUIFrame->SendMessageView(TRUE,	GxMSG_GUI_CHANGE_LANGUAGE, wParam, lParam);			// FormView들에게 표시 언어 변경을 통보한다.
	}

	return 0;
}

// 통신 상태가 변경되었다.
//kjpark 20160919 Topdlg에 connect status 
LRESULT CMainFrame::OnChangeCommState(WPARAM wParam, LPARAM lParam)
{
	// 각 Form View에게 변경 event을 통보한다.
	if ( m_pGUIFrame )
	{
		m_pGUIFrame->SendMessageDlgBar(eDLGBAR_Top, UDMSG_COMM_STATE, wParam, lParam);		// 상위 상태바에 변경 내용을 적용한다.
	}
#if 0
	// 2014-12-10 통신이 끊어졌으면 Alarm을 울리도록한다.
	// 통신 상태가 변경되었을 때 처리 내용
	//
	int nCommID		= (int) wParam;			// 어떤 통신이 변경되었나 ?
	int nState		= (int) lParam;			// 어떠한 상태인가 ?
	int nErrCode	= ERR_NONE;				// 초기값은 Alarm 없음.

	if ( nState == 0 )							// 끊어졌을 경우 Alarm 발생
	{
		switch ( nCommID )
		{
		//case eCOMM_AVCamera :	nErrCode = ERR_COMM_VISIONPC;		break;		// Vision PC와의 통신 단절
		case eCOMM_SignalPC :	nErrCode = ERR_COMM_PGPC;			break;		// 신호기 PC
		case eCOMM_DataPC :		nErrCode = ERR_COMM_DATAPC;			break;		// 데이터 PC 와의 통신
		case eCOMM_ALIGNPC :		nErrCode = ERR_COMM_ALIGN;			break;		// ALIGN 과의 통신		
		}

		if ( nErrCode != ERR_NONE )				// 오류가 발생하였다면,
		{
			CGxAlarmCtrl* pAlarmCtrl	= CGxAlarmCtrl::GetInstance();

			// 지정 Alarm을 발생시킨다.
			pAlarmCtrl->SetError(nErrCode, eQUERY_NONE);						// 중지를 한다. 응답은 받지 않는다.
		}


	}//of if ( nState == 0 )					// 끊어졌을 경우 Alarm 발생
#endif
	return 0;

}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
LRESULT CMainFrame::OnChangeCIMMessageState(WPARAM wParam, LPARAM lParam)
{
	// 각 Form View에게 변경 event을 통보한다.
	if ( m_pGUIFrame )
	{
		m_pGUIFrame->SendMessageDlgBar(eDLGBAR_Top, MSG_CIM_MESSAGE_UPDATE, wParam, lParam);		// 상위 상태바에 변경 내용을 적용한다.
	}

	return 0;
}

LRESULT CMainFrame::OnWarningShow( WPARAM wParam, LPARAM lParam )
{
	ALARM_ID id = (ALARM_ID)wParam;
	m_WarningMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	m_WarningMsgBox.SetTitle(_T("경알람"), _T("Light Alarm"), _T("Light Alarm"));
	m_WarningMsgBox.SetMessage(FALSE, 
		theProcBank.m_AlarmMsg[id].strName[LAN_KOR], 
		theProcBank.m_AlarmMsg[id].strName[LAN_ENG],		
		theProcBank.m_AlarmMsg[id].strName[LAN_VNM], GetMainHandler()->m_nLangIdx );


	theUnitFunc.SetOutPutIO(Y_BUZZER_K1,ON);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K2,OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K3,OFF);
	theUnitFunc.SetOutPutIO(Y_BUZZER_K4,OFF);

	if(m_bWarningMsgShow == FALSE)
	{
		m_bWarningMsgShow = TRUE;
		m_WarningMsgBox.DoModal();
		m_bWarningMsgShow = FALSE;

		theUnitFunc.SetOutPutIO(Y_BUZZER_K1,OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K2,OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K3,OFF);
		theUnitFunc.SetOutPutIO(Y_BUZZER_K4,OFF);
		theProcBank.WarningClear();
	}
	return 0;
}

LRESULT CMainFrame::OnShowDialog(WPARAM wParam, LPARAM lParam)
{
	UINT nDialogNum = (UINT)wParam;
	m_pGUIFrame->ShowDialog(nDialogNum);

	return 0;
}

void CMainFrame::LoadLastUserInspector()
{
	CString strPath;
	strPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigPath);
	CIni ini(strPath);
	CUserAccount user;
	user.sID = ini.GetString(_T("OPERATOR_USER"), _T("ID"));
	user.nLevel = ini.GetLong(_T("OPERATOR_USER"), _T("LEVEL"));
	user.sPassword = ini.GetString(_T("OPERATOR_USER"), _T("PASSWORD"));
	SetCurUserInspectorData(user);
}

void CMainFrame::SetCurUserInspectorData(CUserAccount val)
{
	 m_UserInspectorData = val;
	 if(m_UserInspectorData.sID.IsEmpty())
		CGxUICtrl::SetStaticString(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top), IDC_GXBTN_TOPBAR_LOGIN_DZONE, _T("LOGOUT"));
	 else
		 CGxUICtrl::SetStaticString(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top), IDC_GXBTN_TOPBAR_LOGIN_DZONE, m_UserInspectorData.sID);

	 CString strPath;
	 strPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigPath);
	 CIni ini(strPath);
	//kjpark 20170920 LOGIN/OUT 구현
	 ini.WriteString(_T("OPERATOR_USER"), _T("ID"), m_UserInspectorData.sID);
	 ini.WriteLong(_T("OPERATOR_USER"), _T("LEVEL"), m_UserInspectorData.nLevel);
	 ini.WriteString(_T("OPERATOR_USER"), _T("PASSWORD"), m_UserInspectorData.sPassword);
}

void CMainFrame::SetCurUserOperatorData(CUserAccount val)
{
	m_UserOperatorData = val;
	if(m_UserOperatorData.sID.IsEmpty())
		CGxUICtrl::SetStaticString(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top), IDC_GXBTN_TOPBAR_LOGIN_BZONE, _T("LOGOUT"));
	else
		CGxUICtrl::SetStaticString(GetMainHandler()->m_pGUIFrame->GetDialogBar(eDLGBAR_Top), IDC_GXBTN_TOPBAR_LOGIN_BZONE, m_UserOperatorData.sID);
}



//---------------------------------------------------LOGIN 관련

int CMainFrame::getUserType(CString szID, CString szPW)		//	유저맵에서 ID와 PW가 있는지 체크후 존재한다면 해당 UserData의 Type을 return함.
{
	int nUserType = 0;

	CUserAccount* pUserData = NULL;


	if ( m_mapUserAccount.Lookup(szID, pUserData) == TRUE )	//유저맵에서 Find == TRUE
	{
		if (pUserData != NULL )		//	유저데이터가 존재시			
		{
			if ( pUserData->sPassword == szPW )
			{
				nUserType = pUserData->nLevel;
			}
		}			
	}

	return nUserType;
}


BOOL CMainFrame::FindUser(CString szID)
{
	CUserAccount* pUserData = NULL;

	if ( m_mapUserAccount.Lookup(szID, pUserData) == TRUE)
	{
		return TRUE;
	}

	return FALSE;

}

// UserInfo.ini를 가져와서 Map에 넣어준다.;
BOOL CMainFrame::LoadUserAccountMap()
{
	CString strFileName;
	//strFileName.Format(_T("%s\\Profile\\%s") , _T("O:\\") ,NAME_FILE_USER_LIST);			// 읽어 올 파일
	strFileName.Format(_T("%s\\%s") , theConfigBank.m_System.m_strConfigPath, NAME_FILE_USER_LIST);
	//D:\Ibis\Config

	// 파일이 존재하는지 점검
	if ( PathFileExists( strFileName ) == FALSE )		//  파일 존재여부
	{

		///MODIFY NEED	로그 뿌려줘야함
		//m_pLogSeq->Log(_T("[MAIN] User Account file read ERROR"));
		return FALSE;
	}
	ClearUserAccountMap();

	CUserAccount*	pData;

	CString strUserID = _T("");			// ID
	CString strKorName = _T("");		// 한글이름
	CString strEngName = _T("");		// 영어이름
	CString strPw = _T("");				// 비밀번호
	CString strUserIDNo = _T("");		// 사원번호
	CString strUserLevel = _T("");		// 권한 ( 오퍼레이터:1 엔지니어:2 관리자:3)


	//파일을 한줄씩 읽어가면서 넣어준다.;
	// Text 파일 UniCode 방식으로 설정 변경 ( 베트남어 읽고 쓰기 가능)
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strFileName.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));
	if (e != 0) return FALSE;
	CStdioFile		file(fStream);

	while(file.ReadString(strFileName))
	{
		if(strFileName.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;
		if(strFileName.Left(1) == _T(";"))	continue;	// 처음 글자가 " ; " 일경우 다음 문장으로 넘어간다.;

		::AfxExtractSubString( strUserID,		strFileName	, 0, ',');			
		::AfxExtractSubString( strKorName,		strFileName	, 1, ',');			
		::AfxExtractSubString( strEngName,		strFileName	, 2, ',');			
		::AfxExtractSubString( strUserIDNo,		strFileName	, 3, ',');			
		::AfxExtractSubString( strPw,				strFileName	, 4, ',');			
		::AfxExtractSubString( strUserLevel,	strFileName	, 5,',');			

		pData = new CUserAccount();			// 새로운 Data 생성

		// Data 할당		
		pData->sID		= strUserID;
		pData->sKorName		= strKorName;
		pData->sEngName	= strEngName;
		pData->sIDNo		= strUserIDNo;	// _wtoi(strUserIDNo);		// 사원번호는 절대 숫자가 아니고, 문자로 취급해야 한다.
		pData->sPassword		= strPw;
		pData->nLevel		= _wtoi(strUserLevel);

		if( strUserID.Left(1) == _T(";") || strUserID == _T("") || strUserID == _T("NONE")) // ID값이 이상하면 지운다.
		{
			delete pData;
		}
		else	// 정상적인 ID라면
		{
			m_mapUserAccount.SetAt(pData->sID, pData);		// Map에 추가
		}
	}

	file.Close();
	fclose(fStream);

	return TRUE;
}

void CMainFrame::ClearUserAccountMap()
{
	CString strKey;
	CUserAccount* pUsr = NULL;
	POSITION pos = m_mapUserAccount.GetStartPosition();
	while(pos)
	{
		m_mapUserAccount.GetNextAssoc(pos, strKey, pUsr);
		delete pUsr;
		pUsr = NULL;
	}
	m_mapUserAccount.RemoveAll();
}

//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	KillTimer(nIDEvent);

	switch(nIDEvent)
	{
	case TMID_DOOR_INTERLOCK:
		{
			if(theProcBank.MachineIsRunState())
			{
				theUnitFunc.DoorClose_Check(TRUE);
				if(theUnitFunc.LightCurtainMuteLamp_Check(JIG_ID_A) == FALSE)
					theUnitFunc.LightCurtain_Check(JIG_ID_A,TRUE);
				if(theUnitFunc.LightCurtainMuteLamp_Check(JIG_ID_B) == FALSE)
					theUnitFunc.LightCurtain_Check(JIG_ID_B,TRUE);
			}
			ManualLightCurtainCheck();
			//kjpark 201709 BackDoor 모니터 전환
			//if(m_bBackDoorOpen != theUnitFunc.GetInPutIOCheck(X_PC_RACK_DOOR_OPEN))
			//{
			//	m_bBackDoorOpen = theUnitFunc.GetInPutIOCheck(X_PC_RACK_DOOR_OPEN);
			//	SetWindowMove();
			//}
// 			if(theUnitFunc.GetInPutIOCheck(X_PC_RACK_DOOR_OPEN))
// 			{
// 				m_bBackDoorOpen = TRUE;
// 			}
// 			else
// 			{
// 				m_bBackDoorOpen = FALSE;
// 			}
// 			SetWindowMove();
			SetTimer(TMID_DOOR_INTERLOCK, 100,NULL);
		}
		break;
	//kjparkk 20180131 PDT AB RULE 체크 기능 추가
	case TMID_PDT_AB_RULE_CHECK:
		{
			if(theProcBank.MachineIsRunState())
			{
				if(theUnitFunc.PDT_IF_RETRY_AB_Check() != theConfigBank.m_Option.m_bUseRetryAB)
				{
					theUnitFunc.SetError(ALM_PDT_AB_RULE_MISMATCH);
				}
			}
			
		}
		SetTimer(TMID_PDT_AB_RULE_CHECK, 30*1000 ,NULL);
		break;
	case TMID_FLICK_LAMP:
		{
			if(m_bChkLampR_Flick == TRUE)
			{
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_RED, m_bLamp_FLick);
			}

			if(m_bChkLampY_Flick == TRUE)
			{
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_YELLOW, m_bLamp_FLick);
			}

			if(m_bChkLampG_Flick == TRUE)
			{
				theUnitFunc.SetOutPutIO(Y_TOWER_LAMP_GREEN, m_bLamp_FLick);
			}
			m_bLamp_FLick = !m_bLamp_FLick;

			SetTimer(TMID_FLICK_LAMP,500,NULL);
		}
		break;
	case TMID_LOSS_TIME:
		{
			// 날짜가 바뀌면 24시간 구분하기 위해 여태까지는 자재다운으로 적는다 [6/19/2017 OSC]
			if(theProcBank.GetMachineState() != CONST_MACHINE_STATE::MACHINE_RUN)
			{
				CTime time = CTime::GetCurrentTime();
				CTime timeLoss = theLossTimeLog.GetMachineStopTime().ToCTime();
				CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
				CEtc::ApplyTimeOffset(timeLoss, theConfigBank.m_Option.m_OffsetHour, 0);
				if(time.GetDay() != timeLoss.GetDay())
				{
					theLossTimeLog.ChangeDay();
				}
			}
			SetTimer(TMID_LOSS_TIME,60000,NULL);	// 1분마다...
		}
		break;
	}
	
	// 상위에서 Function change명령이 내려왔었을 경우 - LSH171201
	if(theSocketInterFace.m_CIM.FunctionChangeBuffer.bChangeFlag)
	{
		// 설비가 Loading stop을 진행 할 수 있는 상태일 경우 - LSH171201
		if(theUnitFunc.LoadingStop_IsAble())
		{
			// 값을 Set한 후 상위에 보고한다 - LSH
			theConfigBank.m_CIM.SetFunction((EFID_INDEX)(theSocketInterFace.m_CIM.FunctionChangeBuffer.EFID - 1), theSocketInterFace.m_CIM.FunctionChangeBuffer.EFST_Get());
			theConfigBank.m_CIM.TransitionOfEFListState();
			theConfigBank.m_CIM.ConfirmCIMFunction(BYWHO_HOST);

			// 현재 설정을 갱신한다 - LSH171201
			CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
			CFormCIM* pFormCIM = (CFormCIM*)pMainFrm->m_pGUIFrame->GetView(eFORM_Config_CIM);
			pFormCIM->m_configCIM = theConfigBank.m_CIM;
			pFormCIM->FunctionVector_Value_to_UI();

			theSocketInterFace.m_CIM.FunctionChangeBuffer.Clear();
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

// STO 발동 여부 플랙 초기화
void CMainFrame::SetSTODetectClear()
{
	m_bSTODetectFlag = FALSE;
}


// 라이트 커튼 감지에 의해 모션 동작이 중지되었나 ?
//
BOOL CMainFrame::GetSTODetected()
{
	return m_bSTODetectFlag;					// STO가 발동되었는가 ? 해제는 CSysPanel에서 수행한다.
}

// MCR 연결 해제
//kjpark 20161025 MCR 구현
LRESULT CMainFrame::OnUpdateMCRConnect(WPARAM wParam, LPARAM lParam)
{
	SOCKET_TYPE type = (SOCKET_TYPE)wParam;
	::PostMessage( this->m_hWnd, UDMSG_COMM_STATE, (WPARAM)eCOMM_MCR_1, lParam);

	return 0;
}

//MCR Barcode Read
//kjpark 20170831 MCR 구현 채널 별 구현 완료
LRESULT CMainFrame::OnUpdateMCRData_CH1(WPARAM wParam, LPARAM lParam)
{
	if(GetGUIFrame())
	{		
		theProcBank.m_strLastCellID[JIG_ID_A][JIG_CH_1] = theSocketInterFace.m_MCR[JIG_ID_A][JIG_CH_1].m_pIDReader->GetReadString();
		if(theProcBank.m_strLastCellID[JIG_ID_A][JIG_CH_1].GetLength() < 5)
			theProcBank.m_strLastCellID[JIG_ID_A][JIG_CH_1] = TEXT_FAIL;
		theUnitFunc.SaveResultImage(JIG_ID_A, JIG_CH_1, MCRPath_CH1 ); 
		theProcBank.m_bMCRResultRecive[JIG_ID_A][JIG_CH_1] = TRUE;
		GetGUIFrame()->SendMessageView(eFORM_Teach_Shuttle_1, TRUE, WM_UPDATE_MCRDATA_CH1, wParam, lParam);		
		GetGUIFrame()->SendMessageView(eFORM_Main, TRUE, WM_UPDATE_MCRDATA_CH1, wParam, lParam);		
	}

	return 0;
}

//kjpark 20170831 MCR 구현 채널 별 구현 완료
LRESULT CMainFrame::OnUpdateMCRData_CH3(WPARAM wParam, LPARAM lParam)
{
	if(GetGUIFrame())
	{		
		theProcBank.m_strLastCellID[JIG_ID_B][JIG_CH_1] = theSocketInterFace.m_MCR[JIG_ID_B][JIG_CH_1].m_pIDReader->GetReadString();
		if(theProcBank.m_strLastCellID[JIG_ID_B][JIG_CH_1].GetLength() < 5)
			theProcBank.m_strLastCellID[JIG_ID_B][JIG_CH_1] = TEXT_FAIL;
		theUnitFunc.SaveResultImage(JIG_ID_B, JIG_CH_1, MCRPath_CH3 ); 
		theProcBank.m_bMCRResultRecive[JIG_ID_B][JIG_CH_1] = TRUE;
		GetGUIFrame()->SendMessageView(eFORM_Teach_Shuttle_2, TRUE, WM_UPDATE_MCRDATA_CH2, wParam, lParam);		
		GetGUIFrame()->SendMessageView(eFORM_Main, TRUE, WM_UPDATE_MCRDATA_CH2, wParam, lParam);		
	}

	return 0;
}


void CMainFrame::SetSelectedAxis(AXIS_ID val1, TEACH_PARAM::ID val2, double dOffset /*= 0.*/)		
{
	m_iSelectedAxis = val1; 
	m_iTeachAxis = val2; 
	m_dTeachOffset = dOffset;

	if ( m_pGUIFrame )
	{
		CDlgbarJog* pBarJog = (CDlgbarJog*)m_pGUIFrame->GetDialogBar(eDLGBAR_Jog);

		if ( pBarJog->GetSafeHwnd() == NULL )		return;
		
		pBarJog->SendMessage(CHANGE_MOTOR_AXIS );
	}
}

void CMainFrame::ManualLightCurtainCheck()
{
	if(m_checkAxisLightCurtain == AXIS_ID_MAX)
		return;
	if(m_bEnableLightCurtain)
	{
		if(theDeviceMotion.IsAxisStop(MP2100_THREAD_MAIN_PROCESS, m_checkAxisLightCurtain))
		{
			// 멈췄으면 Muting Lamp 원복
			theUnitFunc.LightCurtainMuteLamp_OnOff(JIG_ID_MAX, ON);
			m_bEnableLightCurtain = FALSE;
		}
		else
		{
			if(theUnitFunc.LightCurtain_Check() == FALSE)
			{
				// 감지했으면 축 정지하고 Muting Lamp 원복
				theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_checkAxisLightCurtain);
				theUnitFunc.LightCurtainMuteLamp_OnOff(JIG_ID_MAX, ON);
				m_bEnableLightCurtain = FALSE;

				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
				dlgMsgBox.SetMessage(FALSE, 
					theProcBank.m_strLastKorMsg,	//한
					theProcBank.m_strLastEngMsg,		//영
					theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);		//베

				dlgMsgBox.DoModal();
			}
			if(theUnitFunc.DoorClose_Check() == FALSE)
			{
				// 감지했으면 축 정지하고 Muting Lamp 원복
				theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_checkAxisLightCurtain);
				theUnitFunc.LightCurtainMuteLamp_OnOff(JIG_ID_MAX, ON);
				m_bEnableLightCurtain = FALSE;

				CGxMsgBox	dlgMsgBox;
				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
				dlgMsgBox.SetMessage(FALSE, 
					theProcBank.m_strLastKorMsg, 
					theProcBank.m_strLastEngMsg,		
					theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

				dlgMsgBox.DoModal();
			}
		}
	}
}

void CMainFrame::SetWindowMove()
{
	if(m_bBackDoorOpen)
	{
		//kjpark 20170828 MoveWindow 적용
		MoveWindow((int)theConfigBank.m_System.m_dDoorOpenWindow_X, 0, 1280, 1024);
		//m_bBackDoorOpen = FALSE;
	}
	else
	{
		//kjpark 20170828 MoveWindow 적용
		MoveWindow((int)theConfigBank.m_System.m_dDefaultWindow_X, 0, 1280, 1024);
		//m_bBackDoorOpen = TRUE;
	}
}

void CMainFrame::AllCellTrackOut()
{
	CCellInfo *pCell;
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		// R 보고 하면 안되니 리트라이 취소 [12/22/2017 OSC]
		pCell = theCellBank.GetCellInfo((JIG_ID)jig, JIG_CH_1);
		if(pCell)
		{
			pCell->defaultData.m_bRetryAble = FALSE;
		}

		// Track Out 못한게 있으면 한다 [12/22/2017 OSC]
		theUnitFunc.CIM_CellCimJudge((JIG_ID)jig);
		theUnitFunc.CIM_CellAllInspectionEnd((JIG_ID)jig);
		theUnitFunc.CIM_CellTrackOut((JIG_ID)jig);

		// Retry 쟁여둔 것도 처리..
		theProcBank.RetryCellInfo_AllTrackOut((JIG_ID)jig, JIG_CH_1);

		// Cell 삭제
		theCellBank.RemoveCellTag((JIG_ID)jig, JIG_CH_1);

		// Vac Off
		theUnitFunc.Shuttle_Vac_OnOff((JIG_ID)jig, JIG_CH_1, VAC_OFF);
	}

}
