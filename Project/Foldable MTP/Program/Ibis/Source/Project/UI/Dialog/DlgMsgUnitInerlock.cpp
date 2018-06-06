// DlgMsgUnitInerlock.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgMsgUnitInerlock.h"
#include "afxdialogex.h"

#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
//kjpark 20170831 Interlock, Opcoall, Terminal UI �� ��� ����
#include "UI\GausGUI\GxStaticEx.h"

// CDlgMsgUnitInerlock ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMsgUnitInerlock, CDialog)

CDlgMsgUnitInerlock::CDlgMsgUnitInerlock(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgUnitInerlock::IDD, pParent)
{

}

CDlgMsgUnitInerlock::~CDlgMsgUnitInerlock()
{
}

void CDlgMsgUnitInerlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgMsgUnitInerlock, CDialog)

	// ����� ���� �޼��� ó��
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// ǥ�� �� �ٲ����.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//kjpark 20170831 Interlock, Opcoall, Terminal UI �� ��� ����
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgMsgInterlock �޽��� ó�����Դϴ�.


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
LRESULT CDlgMsgUnitInerlock::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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

// ǥ�� �� ����Ǿ���.
LRESULT CDlgMsgUnitInerlock::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ǥ�ø� �����ϵ��� �Ѵ�.

	return 0;
}BEGIN_EVENTSINK_MAP(CDlgMsgUnitInerlock, CDialog)
	ON_EVENT(CDlgMsgUnitInerlock, IDC_GXBTN_CLOSE, DISPID_CLICK, CDlgMsgUnitInerlock::ClickGxbtnClose, VTS_NONE)
	END_EVENTSINK_MAP()


void CDlgMsgUnitInerlock::ClickGxbtnClose()
{
	KillTimer(1);

	theUnitStatusBank.SetInterlockCinform();

	for (int i = 0; i < 10; i++)
		theProcBank.m_strUnitInterlockMsg[i] = _T("");

	theProcBank.m_strUnitInterlockID = _T("");
	theProcBank.m_UnitInterlockCnt = 0;
	theProcBank.m_strInterlockedUnitID.Empty();

	theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[theProcBank.GetMachineState()].Red.lamp_state
		,theConfigBank.m_Option.iMapLampSettingList[theProcBank.GetMachineState()].Yellow.lamp_state
		, theConfigBank.m_Option.iMapLampSettingList[theProcBank.GetMachineState()].Green.lamp_state
		,FALSE);

	OnOK();
}


void CDlgMsgUnitInerlock::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	KillTimer(1);
	if(nIDEvent == 1)
	{
		CString strTitle;
		strTitle.Format(_T("[%s] %s"), theProcBank.m_strInterlockedUnitID, theProcBank.m_strUnitInterlockID);
		CGxStaticEx* InterlockID = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE);
		InterlockID->SetCaption(strTitle);

		CGxStaticEx* InterlockMsg = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE_LIST);
		InterlockMsg->SetCaption(theProcBank.m_strUnitInterlockMsg[0]+_T("\n")+theProcBank.m_strUnitInterlockMsg[1]+_T("\n")+theProcBank.m_strUnitInterlockMsg[2]+_T("\n")
			+theProcBank.m_strUnitInterlockMsg[3]+_T("\n")+theProcBank.m_strUnitInterlockMsg[4]+_T("\n")+theProcBank.m_strUnitInterlockMsg[5]+_T("\n")
			+theProcBank.m_strUnitInterlockMsg[6]+_T("\n")+theProcBank.m_strUnitInterlockMsg[7]+_T("\n")+theProcBank.m_strUnitInterlockMsg[8]+_T("\n")
			+theProcBank.m_strUnitInterlockMsg[9]);

		SetTimer(1,100,NULL);
	}

	CDialog::OnTimer(nIDEvent);
}


void CDlgMsgUnitInerlock::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	CString strTitle;
	strTitle.Format(_T("[%s] %s"), theProcBank.m_strInterlockedUnitID, theProcBank.m_strUnitInterlockID);
	CGxStaticEx* InterlockID = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE);
	InterlockID->SetCaption(strTitle);

	CGxStaticEx* InterlockMsg = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE_LIST);
	InterlockMsg->SetCaption(theProcBank.m_strUnitInterlockMsg[0]);	
	SetTimer(1,100,NULL);
}
