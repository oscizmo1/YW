﻿// DlgMsgTerminal.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgMsgTerminal.h"

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

// CDlgMsgTerminal 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMsgTerminal, CDialog)

	CDlgMsgTerminal::CDlgMsgTerminal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgTerminal::IDD, pParent)
{

}

CDlgMsgTerminal::~CDlgMsgTerminal()
{
}

void CDlgMsgTerminal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMsgTerminal, CDialog)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	ON_WM_SHOWWINDOW()
	//kjpark 20180101 Teminal MSG 10개 리스트로 표시
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgMsgTerminal 메시지 처리기입니다.


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
LRESULT CDlgMsgTerminal::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
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
LRESULT CDlgMsgTerminal::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

BEGIN_EVENTSINK_MAP(CDlgMsgTerminal, CDialog)

	ON_EVENT(CDlgMsgTerminal, IDC_GXBTN_CLOSE, DISPID_CLICK, CDlgMsgTerminal::ClickGxbtnOk, VTS_NONE)
END_EVENTSINK_MAP()


//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
//kjpark 20180101 Teminal MSG 10개 리스트로 표시
void CDlgMsgTerminal::ClickGxbtnOk()
{
	KillTimer(1);

	for (int  i = 0; i < 10; i++)
		theProcBank.m_strTerminalMsgs[i] = _T("");

	theProcBank.m_strOldTerminal = _T("");
	theProcBank.m_MsgTerminalCnt = 0;
		
	OnOK();
}

//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
void CDlgMsgTerminal::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


	CGxStaticEx* TerminalMsg = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE);
	TerminalMsg->SetCaption(theProcBank.m_strTerminalMsgs[0]);
	//kjpark 20180101 Teminal MSG 10개 리스트로 표시
	SetTimer(1,100,NULL);
}

//kjpark 20180101 Teminal MSG 10개 리스트로 표시
void CDlgMsgTerminal::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);
	KillTimer(1);
	if(nIDEvent == 1)
	{
		CGxStaticEx* OPCallMsg = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_CIM_MESSAGE);
		OPCallMsg->SetCaption(theProcBank.m_strTerminalMsgs[0]+theProcBank.m_strTerminalMsgs[1]+theProcBank.m_strTerminalMsgs[2]
		+theProcBank.m_strTerminalMsgs[3]+theProcBank.m_strTerminalMsgs[4]+theProcBank.m_strTerminalMsgs[5]
		+theProcBank.m_strTerminalMsgs[6]+theProcBank.m_strTerminalMsgs[7]+theProcBank.m_strTerminalMsgs[8]
		+theProcBank.m_strTerminalMsgs[9]);

		SetTimer(1,100,NULL);
	}
}
