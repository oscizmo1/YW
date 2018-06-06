// DlgCoRelation2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgCoRelation.h"

#include "GUIDefine.h"

//GxGUIFrame ���
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
	// ����� ���� �޼��� ó��
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// ǥ�� �� �ٲ����.
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

// CDlgCoRelation2 �޽��� ó�����Դϴ�.

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
LRESULT CDlgCoRelation::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
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
// ǥ�� �� ����Ǿ���.
LRESULT CDlgCoRelation::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ǥ�ø� �����ϵ��� �Ѵ�.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 
// // ������ �������� ����� �޸� �����Ѵ�.
// BOOL CDlgCoRelation::PreCreateWindow(CREATESTRUCT& cs)
// {
// 	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
// 	cs.style = WS_OVERLAPPED | WS_CHILD;				// �׵θ��� ���ش�.
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

//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�Shuttle_Vac_OnOff
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