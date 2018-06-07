// FormTeach_Shuttle_22.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormTeach_Shuttle_2.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
//kjpark 20161025 MCR 구현
#include "Etc/FileSupport.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFormTeach_Shuttle_2, CFormView)

	CFormTeach_Shuttle_2::CFormTeach_Shuttle_2()
	: CFormView(CFormTeach_Shuttle_2::IDD)
{
	//kjpark 20161025 MCR 구현
	m_bLiveMode_CH1 = FALSE;
	m_bLiveMode_CH2 = FALSE;
	m_bInit = FALSE;

	m_thisDlgShuttle = JIG_ID_B;

}

CFormTeach_Shuttle_2::~CFormTeach_Shuttle_2()
{


}
void CFormTeach_Shuttle_2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeach_Shuttle_2, CFormView)
	ON_WM_TIMER()
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	//kjpark 20161025 MCR 구현
	ON_MESSAGE(WM_UPDATE_MCRDATA_CH2,		OnUpdateMCRData_CH1)			// MCR이 수신되었다.

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFormTeach_Shuttle_2, CFormView)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_READ_ID_01, DISPID_CLICK, CFormTeach_Shuttle_2::ClickBtnMCRRead_CH1, VTS_NONE)

	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_Y_LOAD, DISPID_CLICK, CFormTeach_Shuttle_2::ClickBtnAxis_Y_Load, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_Y_MANUAL, DISPID_CLICK, CFormTeach_Shuttle_2::ClickBtnAxis_Y_MANUAL, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_Y_INSPECTION, DISPID_CLICK, CFormTeach_Shuttle_2::ClickBtnAxis_Y_Inspection, VTS_NONE)

	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_NSPECTION_X_INTSPECTION, DISPID_CLICK, CFormTeach_Shuttle_2::ClickBtnAxis_X_Inspection, VTS_NONE)

	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_UP, DISPID_CLICK, CFormTeach_Shuttle_2::ClickBtnAxis_Z_Up, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_INSPECTION, DISPID_CLICK, CFormTeach_Shuttle_2::ClickBtnAxis_Z_Inspection, VTS_NONE)


	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_VAC_CH1, DISPID_CLICK, CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2VacCh1, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_ACTIVE_ALIGN_LEFT_X, DISPID_CLICK, CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2ActiveALIGNLeftX, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_ACTIVE_ALIGN_RIGHT_X, DISPID_CLICK, CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2ActiveALIGNRightX, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_BLOW_CH1, DISPID_CLICK, CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2BlowCh1, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_FPCB_VAC_CH1, DISPID_CLICK, CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2FpcbVacCh1, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_FPCB_BLOW_CH2, DISPID_CLICK, CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2FpcbBlowCh2, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_DOWN, DISPID_CLICK, CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2Down, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_2, IDC_GXBTN_TEACH_SHUTTLE_2_UP, DISPID_CLICK, CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2Up, VTS_NONE)
END_EVENTSINK_MAP()

// CFormTeach_Shuttle_22 진단입니다.

#ifdef _DEBUG
void CFormTeach_Shuttle_2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeach_Shuttle_2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeach_Shuttle_22 메시지 처리기입니다.

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
LRESULT CFormTeach_Shuttle_2::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		UpdateShuttleState();
		SetTimer(0x9000, 100, NULL);
	}
	else
	{
		KillTimer(0x9000);
	}
	m_bShow_CH1 = m_bShow_CH2 = bFlag;
	return 0;
}
void CFormTeach_Shuttle_2::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y2;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y2_to_LOAD;

	m_CurID	  = IDC_GXBTN_TEACH_SHUTTLE_2_READ_ID_01;
	m_AxisList[0] = IDC_GXBTN_TEACH_SHUTTLE_2_Y_LOAD;
	m_AxisList[1] = IDC_GXBTN_TEACH_SHUTTLE_2_Y_MANUAL;
	m_AxisList[2] = IDC_GXBTN_TEACH_SHUTTLE_2_Y_INSPECTION;
	m_AxisList[3] = IDC_GXBTN_TEACH_SHUTTLE_2_NSPECTION_X_INTSPECTION;
	m_AxisList[4] = IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_UP;
	m_AxisList[5] = IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_INSPECTION;
	m_AxisList[6] = IDC_GXBTN_TEACH_SHUTTLE_2_ACTIVE_ALIGN_LEFT_X;
	m_AxisList[7] = IDC_GXBTN_TEACH_SHUTTLE_2_ACTIVE_ALIGN_RIGHT_X;

}
void CFormTeach_Shuttle_2::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 0x9000:
		UpdateShuttleState();
		Get_Check();
		break;
	}
	CFormView::OnTimer(nIDEvent);
}
// 표시 언어가 변경되었다.
LRESULT CFormTeach_Shuttle_2::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormTeach_Shuttle_2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}



//CH1
void CFormTeach_Shuttle_2::ClickBtnMCRRead_CH1()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnMCRRead_CH1"));
	theProcBank.m_strLastCellID[m_thisDlgShuttle][JIG_CH_1].Empty();
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TEACH_SHUTTLE_2_VIEW_ID_01, _T(""));	

	theUnitFunc.MCR_Trigger(m_thisDlgShuttle, JIG_CH_1, TRUE);
	Sleep(200);
	theUnitFunc.MCR_Trigger(m_thisDlgShuttle, JIG_CH_1, FALSE);
}

// MCR이 수신되었다.
LRESULT CFormTeach_Shuttle_2::OnUpdateMCRData_CH1(WPARAM wParam, LPARAM lParam)
{
	SetMCRData_CH1(theUnitFunc.GetSoftTriggerData(m_thisDlgShuttle, JIG_CH_1));

	return 0;
}

void CFormTeach_Shuttle_2::SetMCRData_CH1(CString strBarcode)
{
	if(m_bShow_CH1 == FALSE)
		return;

	if ( theUnitFunc.GetSoftTriggerResult(m_thisDlgShuttle, JIG_CH_1) == 1 )
	{
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_TEACH_SHUTTLE_2_VIEW_ID_01, strBarcode);
	}
	else
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_TEACH_SHUTTLE_2_VIEW_ID_01, strBarcode);


	CString sFile = theUnitFunc.GetLastSavedImage(m_thisDlgShuttle, JIG_CH_1);
	if(CFileSupport::FileCheck(sFile)  && m_bShow_CH1 )
	{
		m_imgLive_CH1.LoadImage( sFile );
		m_imgLive_CH1.Invalidate();

	}

}

void CFormTeach_Shuttle_2::SetMCRLive_CH1()
{
	if ( m_bShow_CH1 == FALSE) return ;

	// 취득한 이미지를 표시한다.
	if(CFileSupport::FileCheck(MCRPath_CH3)  && m_bShow_CH1)
	{
		m_imgLive_CH1.LoadImage( MCRPath_CH3);
		m_imgLive_CH1.Invalidate();
	}
}

void CFormTeach_Shuttle_2::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	
	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}

void CFormTeach_Shuttle_2::ClickBtnAxis_Y_Load()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnAxis_Y_Load"));
	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y2;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y2_to_LOAD;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_2_Y_LOAD);
}

void CFormTeach_Shuttle_2::ClickBtnAxis_Y_MANUAL()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnAxis_Y_MANUAL"));
	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y2;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y2_to_MANUAL;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_2_Y_MANUAL);
}

void CFormTeach_Shuttle_2::ClickBtnAxis_Y_Inspection()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnAxis_Y_Inspection"));
	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y2;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y2_to_INSP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_2_Y_INSPECTION);
}

void CFormTeach_Shuttle_2::ClickBtnAxis_X_Inspection()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnAxis_X_Inspection"));
	m_CurAxis = (AXIS_ID)AXIS_INSPECTION_X2;
	m_CurTeach = TEACH_PARAM::INSPECTION_X2_to_INSP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_2_NSPECTION_X_INTSPECTION);
}

void CFormTeach_Shuttle_2::ClickBtnAxis_Z_Up()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnAxis_Z_Up"));
	m_CurAxis = (AXIS_ID)AXIS_INSPECTION_Z2;
	m_CurTeach = TEACH_PARAM::INSPECTION_Z2_to_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_UP);
}

void CFormTeach_Shuttle_2::ClickBtnAxis_Z_Inspection()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnAxis_Z_Inspection"));
	m_CurAxis = (AXIS_ID)AXIS_INSPECTION_Z2;
	m_CurTeach = TEACH_PARAM::INSPECTION_Z2_to_INSP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_INSPECTION);
}

void CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2ActiveALIGNLeftX()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnAxis_ACTIVE_ALIGN_LEFT"));
	m_CurAxis = (AXIS_ID)AXIS_ACTIVE_ALIGN_X2;
	m_CurTeach = TEACH_PARAM::ACTIVE_ALIGN_X2_to_LEFT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_2_ACTIVE_ALIGN_LEFT_X);
}


void CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2ActiveALIGNRightX()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("ClickBtnAxis_ACTIVE_ALIGN_RIGHT"));
	m_CurAxis = (AXIS_ID)AXIS_ACTIVE_ALIGN_X2;
	m_CurTeach = TEACH_PARAM::ACTIVE_ALIGN_X2_to_RIGHT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_2_ACTIVE_ALIGN_RIGHT_X);
}

void CFormTeach_Shuttle_2::Get_Check()
{
	if ( m_CurAxis != GetMainHandler()->GetSelectedAxis() )		
		GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);

	for(int i = 0 ; i < sizeof(m_AxisList); i++ )
	{
		if ( m_AxisList[i] == m_CurID )
			CGxUICtrl::SetButtonColor(this, m_AxisList[i], GXCOLOR_MOTOR_ON);
		else
			CGxUICtrl::SetButtonColor(this, m_AxisList[i], GXCOLOR_OFF);
	}
}

void CFormTeach_Shuttle_2::UpdateShuttleState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_VAC_CH1_LAMP,theUnitFunc.Shuttle_Vac_Check(JIG_ID_B, JIG_CH_1, VAC_ON)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_BLOW_CH1_LAMP,theUnitFunc.Shuttle_Blow_Check(JIG_ID_B, JIG_CH_1, BLOW_ON)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_FPCB_VAC_CH1_LAMP,theUnitFunc.Shuttle_Fpcb_Vac_Check(JIG_ID_B, JIG_CH_1, VAC_ON)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_FPCB_BLOW_CH1_LAMP,theUnitFunc.Shuttle_Fpcb_Blow_Check(JIG_ID_B, JIG_CH_1, BLOW_ON)? GXCOLOR_ON:GXCOLOR_OFF);
	
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_DOWN_LAMP,theUnitFunc.Shuttle_Tilt_Down_Check	(JIG_ID_B, JIG_CH_1, TILT_DOWN)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_UP_LAMP,theUnitFunc.Shuttle_Tilt_Up_Check	(JIG_ID_B, JIG_CH_1, TILT_UP)? GXCOLOR_ON:GXCOLOR_OFF);

	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_Y_LOAD_LAMP,theUnitFunc.Shuttle_Y_LOAD_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_Y_MANUAL_LAMP,theUnitFunc.Shuttle_Y_MANUAL_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_Y_INSPECTION_LAMP,theUnitFunc.Shuttle_Y_INSP_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());

	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_X_INTSPECTION_LAMP,theUnitFunc.Inspection_X_INSP_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_UP_LAMP,theUnitFunc.Inspection_Z_UP_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_INSPECTION_Z_INSPECTION_LAMP,theUnitFunc.Inspection_Z_INSP_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());

	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_ACTIVE_ALIGN_LEFT_X_LAMP,theUnitFunc.Active_ALIGN_X_LEFT_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_2_ACTIVE_ALIGN_RIGHT_X_LAMP,theUnitFunc.Active_ALIGN_X_RIGHT_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
}

void CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2VacCh1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다Shuttle_Vac_OnOff
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("Shuttle_Vacuum_CH2"));
	if(theUnitFunc.Shuttle_VacOut_Check(JIG_ID_B, JIG_CH_1, VAC_ON))
	{
		theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_B, JIG_CH_1, VAC_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_B, JIG_CH_1, VAC_ON);	
	}
}


void CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2BlowCh1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("Shuttle_Blow_CH2"));
	if(theUnitFunc.Shuttle_BlowOut_Check(JIG_ID_B, JIG_CH_1, BLOW_ON))
	{
		theUnitFunc.Shuttle_Blow_OnOff(JIG_ID_B, JIG_CH_1, BLOW_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Blow_OnOff(JIG_ID_B, JIG_CH_1, BLOW_ON);	
	}
}


void CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2FpcbVacCh1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("Shuttle_Fpcb_Vacuum_CH2"));
	if(theUnitFunc.Shuttle_Fpcb_VacOut_Check(JIG_ID_B, JIG_CH_1, VAC_ON))
	{
		theUnitFunc.Shuttle_Fpcb_Vac_OnOff(JIG_ID_B, JIG_CH_1, VAC_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Fpcb_Vac_OnOff(JIG_ID_B, JIG_CH_1, VAC_ON);	
	}
}


void CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2FpcbBlowCh2()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("Shuttle_Fpcb_Blow_CH2"));
	if(theUnitFunc.Shuttle_Fpcb_BlowOut_Check(JIG_ID_B, JIG_CH_1, BLOW_ON))
	{
		theUnitFunc.Shuttle_Fpcb_Blow_OnOff(JIG_ID_B, JIG_CH_1, BLOW_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Fpcb_Blow_OnOff(JIG_ID_B, JIG_CH_1, BLOW_ON);	
	}
}


void CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2Down()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("Shuttle_Tilt_Down_CH2"));
	if(theUnitFunc.Shuttle_Tilt_DownOut_Check(JIG_ID_B,JIG_CH_1,TILT_DOWN))
	{
		//theUnitFunc.Shuttle_Tilt_Down(JIG_ID_B,JIG_CH_1,TILT_UP);
	}
	else
	{
		theUnitFunc.Shuttle_Tilt_Down(JIG_ID_B,JIG_CH_1,TILT_DOWN);
	}
}


void CFormTeach_Shuttle_2::ClickGxbtnTeachShuttle2Up()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 2"), _T("Shuttle_Tilt_Up_CH2"));
	if(theUnitFunc.Shuttle_Tilt_UpOut_Check(JIG_ID_B,JIG_CH_1,TILT_UP))
	{
		//theUnitFunc.Shuttle_Tilt_Up(JIG_ID_B,JIG_CH_1,TILT_DOWN);
	}
	else
	{
		theUnitFunc.Shuttle_Tilt_Up(JIG_ID_B,JIG_CH_1,TILT_UP);
	}
}
