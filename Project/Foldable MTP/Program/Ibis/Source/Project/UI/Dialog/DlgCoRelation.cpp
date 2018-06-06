// DlgCoRelation2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgCoRelation.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDlgCoRelation, CDialog)

CDlgCoRelation::CDlgCoRelation(CWnd* pParent)
	: CDialog(CDlgCoRelation::IDD, pParent)
{
	m_bInit = FALSE;
}

CDlgCoRelation::~CDlgCoRelation()
{
}

BOOL CDlgCoRelation::OnInitDialog()
{
	CDialog::OnInitDialog();
	initial();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgCoRelation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CDlgCoRelation, CDialog)
	ON_WM_TIMER()
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgCoRelation, CDialog)
	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_1_Y_LOAD, DISPID_CLICK, CDlgCoRelation::ClickBtnAxis_Y_Load_Jig_1, VTS_NONE)
	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_2_Y_LOAD, DISPID_CLICK, CDlgCoRelation::ClickBtnAxis_Y_Load_Jig_2, VTS_NONE)
	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_1_Y_INSPECTION, DISPID_CLICK, CDlgCoRelation::ClickBtnAxis_Y_Inspection_Jig_1, VTS_NONE)
	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_2_Y_INSPECTION, DISPID_CLICK, CDlgCoRelation::ClickBtnAxis_Y_Inspection_Jig_2, VTS_NONE)

	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_UP, DISPID_CLICK, CDlgCoRelation::ClickBtnAxis_Z_Up_Jig_1, VTS_NONE)
	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_UP, DISPID_CLICK, CDlgCoRelation::ClickBtnAxis_Z_Up_Jig_2, VTS_NONE)


	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_INSPECTION, DISPID_CLICK, CDlgCoRelation::ClickBtnAxis_Z_Inspection_Jig_1, VTS_NONE)
	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_INSPECTION, DISPID_CLICK, CDlgCoRelation::ClickBtnAxis_Z_Inspection_Jig_2, VTS_NONE)

	ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_COFIRM, DISPID_CLICK, CDlgCoRelation::ClickBtnCinfirm, VTS_NONE)
	
	

	//ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_1_VAC_CH1, DISPID_CLICK, CDlgCoRelation::ClickGxbtnTeachShuttle1VacCh1, VTS_NONE)
	//ON_EVENT(CDlgCoRelation, IDC_GXBTN_TEACH_SHUTTLE_1_VAC_CH2, DISPID_CLICK, CDlgCoRelation::ClickGxbtnTeachShuttle1VacCh2, VTS_NONE)
END_EVENTSINK_MAP()

// CDlgCoRelation2 메시지 처리기입니다.

// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame에서 ShowDialog를 수행하면 아래 순서대로 메세지를 호출한다.
//
// ShowDialog() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CDlgCoRelation::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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

void CDlgCoRelation::initial()
{
	m_AxisList[0][0] = IDC_GXBTN_TEACH_SHUTTLE_1_Y_LOAD;
	m_AxisList[0][1] = IDC_GXBTN_TEACH_SHUTTLE_1_Y_INSPECTION;
	m_AxisList[0][2] = IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_UP;
	m_AxisList[0][3] = IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_INSPECTION;

	m_AxisList[1][0] = IDC_GXBTN_TEACH_SHUTTLE_2_Y_LOAD;
	m_AxisList[1][1] = IDC_GXBTN_TEACH_SHUTTLE_2_Y_INSPECTION;
	m_AxisList[1][2] = IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_UP;
	m_AxisList[1][3] = IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_INSPECTION;

	UpdateShuttleState();
	SetTimer(0x9000, 100, NULL);
}

void CDlgCoRelation::OnInitialUpdate()
{
	

}
void CDlgCoRelation::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 0x9000:
		UpdateShuttleState();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
// 표시 언어가 변경되었다.
LRESULT CDlgCoRelation::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 
// // 윈도우 생성전에 모양을 달리 지정한다.
// BOOL CDlgCoRelation::PreCreateWindow(CREATESTRUCT& cs)
// {
// 	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
// 	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.
// 
// 	return CDialog::PreCreateWindow(cs);
// }


void CDlgCoRelation::ClickBtnAxis_Y_Load_Jig_1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Y_Load_JIG1"));
	theUnitFunc.TeachMove(TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD);	
}

void CDlgCoRelation::ClickBtnAxis_Y_Load_Jig_2()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Y_Load_JIG2"));
	theUnitFunc.TeachMove(TEACH_PARAM::JIG_SHUTTLE_Y2_to_LOAD);
}

void CDlgCoRelation::ClickBtnAxis_Y_Inspection_Jig_1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Y_Inspection_Jig_1"));
	theUnitFunc.TeachMove(TEACH_PARAM::JIG_SHUTTLE_Y1_to_INSP);
}

void CDlgCoRelation::ClickBtnAxis_Y_Inspection_Jig_2()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Y_Inspection_Jig_2"));
	theUnitFunc.TeachMove(TEACH_PARAM::JIG_SHUTTLE_Y2_to_INSP);
}

void CDlgCoRelation::ClickBtnAxis_Z_Up_Jig_1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Z_Up_Jig_1"));
	theUnitFunc.TeachMove(TEACH_PARAM::INSPECTION_Z1_to_UP);
}

void CDlgCoRelation::ClickBtnAxis_Z_Up_Jig_2()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Z_Up_Jig_2"));
	theUnitFunc.TeachMove(TEACH_PARAM::INSPECTION_Z2_to_UP);
}

void CDlgCoRelation::ClickBtnAxis_Z_Inspection_Jig_1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Z_Inspection_Jig_1"));
	theUnitFunc.TeachMove(TEACH_PARAM::INSPECTION_Z1_to_INSP);
}

void CDlgCoRelation::ClickBtnAxis_Z_Inspection_Jig_2()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Z_Inspection_Jig_2"));
	theUnitFunc.TeachMove(TEACH_PARAM::INSPECTION_Z2_to_INSP);
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
void CDlgCoRelation::UpdateShuttleState()
{
#ifdef NOT_USED
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_VAC_CH1_LAMP,theUnitFunc.Shuttle_Vac_Check(JIG_ID_A, JIG_CH_1, VAC_ON)? GXCOLOR_ON:GXCOLOR_OFF);
#endif

 	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_Y_LOAD_LAMP,theUnitFunc.Shuttle_Y_LOAD_Check(JIG_ID_A)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_Y_LOAD_LAMP,theUnitFunc.Shuttle_Y_LOAD_Check(JIG_ID_B)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());

	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_Y_INSPECTION_LAMP,theUnitFunc.Shuttle_Y_INSP_Check(JIG_ID_A)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_Y_INSPECTION_LAMP,theUnitFunc.Shuttle_Y_INSP_Check(JIG_ID_B)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
 
 	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_UP_LAMP,theUnitFunc.Inspection_Z_UP_Check(JIG_ID_A)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_UP_LAMP,theUnitFunc.Inspection_Z_UP_Check(JIG_ID_B)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());

 	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_INSPECTION_LAMP,theUnitFunc.Inspection_Z_INSP_Check(JIG_ID_A)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_INSPECTION_LAMP,theUnitFunc.Inspection_Z_INSP_Check(JIG_ID_B)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
}

void CDlgCoRelation::ClickBtnCinfirm()
{
	KillTimer(0x9000);
	OnOK();
}

#ifdef NOT_USED
void CDlgCoRelation::ClickGxbtnTeachShuttle1VacCh1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다Shuttle_Vac_OnOff
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("Shuttle_Vacuum_CH1"));
	if(theUnitFunc.Shuttle_VacOut_Check(JIG_ID_A, JIG_CH_1, VAC_ON))
	{
		theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_ON);	
	}
}

#endif