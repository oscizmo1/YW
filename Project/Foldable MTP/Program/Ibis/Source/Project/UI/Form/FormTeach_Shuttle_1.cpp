// FormTeach_Shuttle_12.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FormTeach_Shuttle_1.h"

#include "GUIDefine.h"

//GxGUIFrame ���
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
//kjpark 20161025 MCR ����
#include "Etc/FileSupport.h"
#include "UI/Dialog/DlgMCRManualRead.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFormTeach_Shuttle_1, CFormView)

CFormTeach_Shuttle_1::CFormTeach_Shuttle_1()
	: CFormView(CFormTeach_Shuttle_1::IDD)
{
	//kjpark 20161025 MCR ����
	m_bLiveMode_CH1 = FALSE;
	m_bLiveMode_CH2 = FALSE;
	m_bInit = FALSE;

	//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
	m_thisDlgShuttle = JIG_ID_A;
	
}

CFormTeach_Shuttle_1::~CFormTeach_Shuttle_1()
{
	

}
void CFormTeach_Shuttle_1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormTeach_Shuttle_1, CFormView)
	ON_WM_TIMER()
	// ����� ���� �޼��� ó��
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// ǥ�� �� �ٲ����.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	//kjpark 20161025 MCR ����
	ON_MESSAGE(WM_UPDATE_MCRDATA_CH1,		OnUpdateMCRData_CH1)			// MCR�� ���ŵǾ���.

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFormTeach_Shuttle_1, CFormView)
	//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_READ_ID_01, DISPID_CLICK, CFormTeach_Shuttle_1::ClickBtnMCRRead_CH1, VTS_NONE)

	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_Y_LOAD, DISPID_CLICK, CFormTeach_Shuttle_1::ClickBtnAxis_Y_Load, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_Y_MANUAL, DISPID_CLICK, CFormTeach_Shuttle_1::ClickBtnAxis_Y_MANUAL, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_Y_INSPECTION, DISPID_CLICK, CFormTeach_Shuttle_1::ClickBtnAxis_Y_Inspection, VTS_NONE)

	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_NSPECTION_X_INTSPECTION, DISPID_CLICK, CFormTeach_Shuttle_1::ClickBtnAxis_X_Inspection, VTS_NONE)

	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_UP, DISPID_CLICK, CFormTeach_Shuttle_1::ClickBtnAxis_Z_Up, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_INSPECTION, DISPID_CLICK, CFormTeach_Shuttle_1::ClickBtnAxis_Z_Inspection, VTS_NONE)
	

	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_VAC_CH1, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1VacCh1, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_ACTIVE_ALIGN_LEFT_X, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1ActiveALIGNLeftX, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_ACTIVE_ALIGN_RIGHT_X, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1ActiveALIGNRightX, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_BLOW_CH1, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1BlowCh1, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_FPCB_VAC_CH1, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1FpcbVacCh1, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_FPCB_BLOW_CH1, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1FpcbBlowCh1, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_DOWN, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1Down, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_UP, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1Up, VTS_NONE)
	ON_EVENT(CFormTeach_Shuttle_1, IDC_GXBTN_TEACH_SHUTTLE_1_GRAB, DISPID_CLICK, CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1Grab, VTS_NONE)
END_EVENTSINK_MAP()

// CFormTeach_Shuttle_12 �����Դϴ�.

#ifdef _DEBUG
void CFormTeach_Shuttle_1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeach_Shuttle_1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeach_Shuttle_12 �޽��� ó�����Դϴ�.

// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame���� ShowDialog�� �����ϸ� �Ʒ� ������� �޼����� ȣ���Ѵ�.
//
// ShowDialog() ȣ���
// 1. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() ȣ���
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
LRESULT CFormTeach_Shuttle_1::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
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
void CFormTeach_Shuttle_1::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate �ι� ����Ǵ� ���� ���� [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();

	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y1;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD;

	//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
	m_CurID	  = IDC_GXBTN_TEACH_SHUTTLE_1_READ_ID_01;
	m_AxisList[0] = IDC_GXBTN_TEACH_SHUTTLE_1_Y_LOAD;
	m_AxisList[1] = IDC_GXBTN_TEACH_SHUTTLE_1_Y_MANUAL;
	m_AxisList[2] = IDC_GXBTN_TEACH_SHUTTLE_1_Y_INSPECTION;
	m_AxisList[3] = IDC_GXBTN_TEACH_SHUTTLE_1_NSPECTION_X_INTSPECTION;
	m_AxisList[4] = IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_UP;
	m_AxisList[5] = IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_INSPECTION;
	m_AxisList[6] = IDC_GXBTN_TEACH_SHUTTLE_1_ACTIVE_ALIGN_LEFT_X;
	m_AxisList[7] = IDC_GXBTN_TEACH_SHUTTLE_1_ACTIVE_ALIGN_RIGHT_X;

}
void CFormTeach_Shuttle_1::OnTimer(UINT_PTR nIDEvent)
{
	//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
	switch(nIDEvent)
	{
	case 0x9000:
		UpdateShuttleState();
		Get_Check();
		break;
	}
	CFormView::OnTimer(nIDEvent);
}
// ǥ�� �� ����Ǿ���.
LRESULT CFormTeach_Shuttle_1::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ǥ�ø� �����ϵ��� �Ѵ�.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// ������ �������� ����� �޸� �����Ѵ�.
BOOL CFormTeach_Shuttle_1::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// �׵θ��� ���ش�.

	return CFormView::PreCreateWindow(cs);
}



//CH1
//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::ClickBtnMCRRead_CH1()
{
	//20170501 LMS OPERATION �߰�
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnMCRRead_CH1"));
	theProcBank.m_strLastCellID[m_thisDlgShuttle][JIG_CH_1].Empty();
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_TEACH_SHUTTLE_1_VIEW_ID_01, _T(""));	

	theUnitFunc.MCR_Trigger(m_thisDlgShuttle, JIG_CH_1, TRUE);
	Sleep(200);
	theUnitFunc.MCR_Trigger(m_thisDlgShuttle, JIG_CH_1, FALSE);
}

// MCR�� ���ŵǾ���.
LRESULT CFormTeach_Shuttle_1::OnUpdateMCRData_CH1(WPARAM wParam, LPARAM lParam)
{
	//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
	SetMCRData_CH1(theUnitFunc.GetSoftTriggerData(m_thisDlgShuttle, JIG_CH_1));

	return 0;
}

void CFormTeach_Shuttle_1::SetMCRData_CH1(CString strBarcode)
{
	if(m_bShow_CH1 == FALSE)
		return;

	//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
	if ( theUnitFunc.GetSoftTriggerResult(m_thisDlgShuttle, JIG_CH_1) == 1 )
	{
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_TEACH_SHUTTLE_1_VIEW_ID_01, strBarcode);
	}
	else
		CGxUICtrl::SetStaticString(this, IDC_GXSTC_TEACH_SHUTTLE_1_VIEW_ID_01, strBarcode);


	CString sFile = theUnitFunc.GetLastSavedImage(m_thisDlgShuttle, JIG_CH_1);
	if(CFileSupport::FileCheck(sFile)  && m_bShow_CH1 )
	{
		m_imgLive_CH1.LoadImage( sFile );
		m_imgLive_CH1.Invalidate();

	}

}

void CFormTeach_Shuttle_1::SetMCRLive_CH1()
{
	if ( m_bShow_CH1 == FALSE) return ;

	// ����� �̹����� ǥ���Ѵ�.
	if(CFileSupport::FileCheck(MCRPath_CH1)  && m_bShow_CH1)
	{
		m_imgLive_CH1.LoadImage( MCRPath_CH1);
		m_imgLive_CH1.Invalidate();
	}
}

void CFormTeach_Shuttle_1::SetCurPageAxis(AXIS_ID axis, TEACH_PARAM::ID param, UINT nID)
{
	m_CurAxis = axis;
	m_CurTeach = param;
	m_CurID	  = nID;	
	GetMainHandler()->SetSelectedAxis(m_CurAxis, m_CurTeach);
}

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::ClickBtnAxis_Y_Load()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Y_Load"));
	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y1;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_1_Y_LOAD);
}

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::ClickBtnAxis_Y_MANUAL()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Y_MANUAL"));
	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y1;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y1_to_MANUAL;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_1_Y_MANUAL);
}

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::ClickBtnAxis_Y_Inspection()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Y_Inspection"));
	m_CurAxis = (AXIS_ID)AXIS_JIG_SHUTTLE_Y1;
	m_CurTeach = TEACH_PARAM::JIG_SHUTTLE_Y1_to_INSP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_1_Y_INSPECTION);
}

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::ClickBtnAxis_X_Inspection()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_X_Inspection"));
	m_CurAxis = (AXIS_ID)AXIS_INSPECTION_X1;
	m_CurTeach = TEACH_PARAM::INSPECTION_X1_to_INSP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_1_NSPECTION_X_INTSPECTION);
}

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::ClickBtnAxis_Z_Up()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Z_Up"));
	m_CurAxis = (AXIS_ID)AXIS_INSPECTION_Z1;
	m_CurTeach = TEACH_PARAM::INSPECTION_Z1_to_UP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_UP);
}

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::ClickBtnAxis_Z_Inspection()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_Z_Inspection"));
	m_CurAxis = (AXIS_ID)AXIS_INSPECTION_Z1;
	m_CurTeach = TEACH_PARAM::INSPECTION_Z1_to_INSP;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_INSPECTION);
}

void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1ActiveALIGNLeftX()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_ACTIVE_ALIGN_LEFT"));
	m_CurAxis = (AXIS_ID)AXIS_ACTIVE_ALIGN_X1;
	m_CurTeach = TEACH_PARAM::ACTIVE_ALIGN_X1_to_LEFT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_1_ACTIVE_ALIGN_LEFT_X);
}


void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1ActiveALIGNRightX()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("ClickBtnAxis_ACTIVE_ALIGN_RIGHT"));
	m_CurAxis = (AXIS_ID)AXIS_ACTIVE_ALIGN_X1;
	m_CurTeach = TEACH_PARAM::ACTIVE_ALIGN_X1_to_RIGHT;
	SetCurPageAxis(m_CurAxis,m_CurTeach,(UINT)IDC_GXBTN_TEACH_SHUTTLE_1_ACTIVE_ALIGN_RIGHT_X);
}

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::Get_Check()
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

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
void CFormTeach_Shuttle_1::UpdateShuttleState()
{
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_VAC_CH1_LAMP,theUnitFunc.Shuttle_Vac_Check(JIG_ID_A, JIG_CH_1, VAC_ON)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_BLOW_CH1_LAMP,theUnitFunc.Shuttle_Blow_Check(JIG_ID_A, JIG_CH_1, BLOW_ON)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_FPCB_VAC_CH1_LAMP,theUnitFunc.Shuttle_Fpcb_Vac_Check(JIG_ID_A, JIG_CH_1, VAC_ON)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_FPCB_BLOW_CH1_LAMP,theUnitFunc.Shuttle_Fpcb_Blow_Check(JIG_ID_A, JIG_CH_1, BLOW_ON)? GXCOLOR_ON:GXCOLOR_OFF);

	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_DOWN_LAMP,theUnitFunc.Shuttle_Tilt_UpDown_Check(JIG_ID_A, TILT_DOWN)? GXCOLOR_ON:GXCOLOR_OFF);
	CGxUICtrl::SetButtonColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_UP_LAMP,theUnitFunc.Shuttle_Tilt_UpDown_Check(JIG_ID_A, TILT_UP)? GXCOLOR_ON:GXCOLOR_OFF);

	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_Y_LOAD_LAMP,theUnitFunc.Shuttle_Y_LOAD_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_Y_MANUAL_LAMP,theUnitFunc.Shuttle_Y_MANUAL_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_Y_INSPECTION_LAMP,theUnitFunc.Shuttle_Y_INSP_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());

	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_X_INTSPECTION_LAMP,theUnitFunc.Inspection_X_INSP_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_UP_LAMP,theUnitFunc.Inspection_Z_UP_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_INSPECTION_Z_INSPECTION_LAMP,theUnitFunc.Inspection_Z_INSP_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());

	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_ACTIVE_ALIGN_LEFT_X_LAMP,theUnitFunc.Active_ALIGN_X_LEFT_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
	CGxUICtrl::SetStaticColor(this, IDC_GXBTN_TEACH_SHUTTLE_1_ACTIVE_ALIGN_RIGHT_X_LAMP,theUnitFunc.Active_ALIGN_X_RIGHT_Check(m_thisDlgShuttle)? Color(Color::Lime).ToCOLORREF():Color(GXCOLOR_OFF).ToCOLORREF());
}

void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1VacCh1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("Shuttle_Vacuum_CH1"));
	if(theUnitFunc.Shuttle_VacOut_Check(JIG_ID_A, JIG_CH_1, VAC_ON))
	{
		theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_OFF, BLOW_ON);	
		Sleep(200);
		theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_OFF, BLOW_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_ON, BLOW_OFF);	
	}
}


void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1BlowCh1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("Shuttle_Blow_CH1"));
	if(theUnitFunc.Shuttle_BlowOut_Check(JIG_ID_A, JIG_CH_1, BLOW_ON))
	{
		theUnitFunc.Shuttle_Blow_OnOff(JIG_ID_A, JIG_CH_1, BLOW_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Blow_OnOff(JIG_ID_A, JIG_CH_1, BLOW_ON);	
	}
}


void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1FpcbVacCh1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("Shuttle_Fpcb_Vacuum_CH1"));
	if(theUnitFunc.Shuttle_Fpcb_VacOut_Check(JIG_ID_A, JIG_CH_1, VAC_ON))
	{
		theUnitFunc.Shuttle_Fpcb_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_OFF, BLOW_ON);
		Sleep(200);
		theUnitFunc.Shuttle_Fpcb_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_OFF, BLOW_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Fpcb_Vac_OnOff(JIG_ID_A, JIG_CH_1, VAC_ON, BLOW_OFF);	
	}
}


void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1FpcbBlowCh1()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("Shuttle_Fpcb_Blow_CH1"));
	if(theUnitFunc.Shuttle_Fpcb_BlowOut_Check(JIG_ID_A, JIG_CH_1, BLOW_ON))
	{
		theUnitFunc.Shuttle_Fpcb_Blow_OnOff(JIG_ID_A, JIG_CH_1, BLOW_OFF);	
	}
	else
	{
		theUnitFunc.Shuttle_Fpcb_Blow_OnOff(JIG_ID_A, JIG_CH_1, BLOW_ON);	
	}
}


void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1Down()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("Shuttle_Tilt_Down_CH1"));
	theUnitFunc.Shuttle_Tilt_UpDown(JIG_ID_A,TILT_DOWN);
}


void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1Up()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("TEACH 1"), _T("Shuttle_Tilt_Up_CH1"));
	if(theUnitFunc.Shuttle_Y_MANUAL_Check(JIG_ID_A))
	{
		theUnitFunc.Shuttle_Tilt_UpDown(JIG_ID_A, TILT_UP);
	}
	else
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
		dlgMsgBox.SetTitle(_T("�˸�"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("Shuttle�� MANUAL��ġ�� �ƴմϴ�."), 
			_T("Shuttle is not MANUAL position."),		
			_T("Shuttle is not MANUAL position.") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}
}


void CFormTeach_Shuttle_1::ClickGxbtnTeachShuttle1Grab()
{
	// Ÿ�̸� ���� [5/11/2018 LSH]
	m_swAlignTimer[JIG_CH_1].Start();

	// Align value�� �ʱ�ȭ �Ѵ� [5/11/2018 LSH]
	m_Align_Value[JIG_CH_1].clear();
	m_dAlignIntervalTime[JIG_CH_1] = 0.0;
	// Align Grab Send [5/11/2018 LSH]
	theSocketInterFace.m_ActiveAlign.ActiveAlignGrab(JIG_CH_1);
}
