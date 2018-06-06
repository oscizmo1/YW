// DlgMsgUnitInerlock.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgMsgUnitInerlock.h"
#include "afxdialogex.h"

#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
#include "UI\GausGUI\GxStaticEx.h"

// CDlgMsgUnitInerlock 대화 상자입니다.

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

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgMsgInterlock 메시지 처리기입니다.


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
LRESULT CDlgMsgUnitInerlock::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgMsgUnitInerlock::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.

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
