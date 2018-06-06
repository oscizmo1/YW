// DlgMsgInterlock.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgMsgInterlock.h"

// #include "MainFrm.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
#include "UI\GausGUI\GxStaticEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
// CDlgMsgInterlock 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMsgInterlock, CDialog)

	CDlgMsgInterlock::CDlgMsgInterlock(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgInterlock::IDD, pParent)
{

}

CDlgMsgInterlock::~CDlgMsgInterlock()
{
}

void CDlgMsgInterlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMsgInterlock, CDialog)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
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
LRESULT CDlgMsgInterlock::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgMsgInterlock::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
BEGIN_EVENTSINK_MAP(CDlgMsgInterlock, CDialog)
	ON_EVENT(CDlgMsgInterlock, IDC_GXBTN_CLOSE, DISPID_CLICK, CDlgMsgInterlock::ClickGxbtnOk, VTS_NONE)
END_EVENTSINK_MAP()


//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CDlgMsgInterlock::ClickGxbtnOk()
{	
	KillTimer(1);

	//kjpark 20180101 MAX일때 배열크기 넘어가는 버그 수정
	for (int i = 0; i < INTERLOCKMAX; i++)
		theProcBank.m_strInterlockMsg[i] = _T("");

	theProcBank.m_strInterlockID = _T("");
	theProcBank.m_strOldInterlockID = _T("");
	theProcBank.m_MsgInterlockCnt = 0;


	theSocketInterFace.m_CIM.SendCmdToDataPC(EQUIP_SET_INTERLOCK_EVENT, theProcBank.m_strInterlockType);

	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
	theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
		E_EQST_MATCH_PAUSE, _T(""));


	OnOK();
}


//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CDlgMsgInterlock::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	CString strTitle;
	strTitle.Format(_T("[%s] %s"), theProcBank.m_strEqipID, theProcBank.m_strInterlockID);
	CGxStaticEx* InterlockID = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE);
	InterlockID->SetCaption(strTitle);

	CGxStaticEx* InterlockMsg = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE_LIST);
	InterlockMsg->SetCaption(theProcBank.m_strInterlockMsg[0]);	
	SetTimer(1,100,NULL);
}


//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CDlgMsgInterlock::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);
	KillTimer(1);
	if(nIDEvent == 1)
	{
		CString strTitle;
		strTitle.Format(_T("[%s] %s"), theProcBank.m_strEqipID, theProcBank.m_strInterlockID);
		CGxStaticEx* InterlockID = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE);
		InterlockID->SetCaption(strTitle);

		CGxStaticEx* InterlockMsg = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE_LIST);
		//kjpark 20180101 MAX일때 배열크기 넘어가는 버그 수정
		InterlockMsg->SetCaption(theProcBank.m_strInterlockMsg[0]+_T("\n\r")+theProcBank.m_strInterlockMsg[1]+_T("\n\r")+theProcBank.m_strInterlockMsg[2]+_T("\n\r")
		+theProcBank.m_strInterlockMsg[3]+_T("\n\r")+theProcBank.m_strInterlockMsg[4]+_T("\n\r")+theProcBank.m_strInterlockMsg[5]+_T("\n\r")
		+theProcBank.m_strInterlockMsg[6]+_T("\n\r")+theProcBank.m_strInterlockMsg[7]+_T("\n\r")+theProcBank.m_strInterlockMsg[8]+_T("\n\r")
		+theProcBank.m_strInterlockMsg[9]+_T("\n\r")+theProcBank.m_strInterlockMsg[10]+_T("\n\r")+theProcBank.m_strInterlockMsg[11]+_T("\n\r")
		+theProcBank.m_strInterlockMsg[12]+_T("\n\r")+theProcBank.m_strInterlockMsg[13]+_T("\n\r")+theProcBank.m_strInterlockMsg[14]+_T("\n\r")
		+theProcBank.m_strInterlockMsg[15]+_T("\n\r")+theProcBank.m_strInterlockMsg[16]+_T("\n\r")+theProcBank.m_strInterlockMsg[17]+_T("\n\r")
		+theProcBank.m_strInterlockMsg[18]+_T("\n\r")+theProcBank.m_strInterlockMsg[19]);

		SetTimer(1,100,NULL);
	}
}
