﻿// FormPM.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormPM.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "..\Dialog\DlgDoor.h"
#include "Etc\FileSupport.h"
#include "UI\GausGUI\GxMsgBox.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
// CFormPM

IMPLEMENT_DYNCREATE(CFormPM, CFormView)

CFormPM::CFormPM()
	: CFormView(CFormPM::IDD)
{

}

CFormPM::~CFormPM()
{
}

void CFormPM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GXSTC_PM_IMAGE, m_imgPM);
}

BEGIN_MESSAGE_MAP(CFormPM, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFormPM, CFormView)
	ON_EVENT(CFormPM, IDC_GXBTN_PM_RELEASE, DISPID_CLICK, CFormPM::ClickBtnReleasePM, VTS_NONE)
	ON_EVENT(CFormPM, IDC_GXBTN_PM_DOOR_OPEN, DISPID_CLICK, CFormPM::ClickBtnOpenDoor, VTS_NONE)
END_EVENTSINK_MAP()



// CFormPM 진단입니다.

#ifdef _DEBUG
void CFormPM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormPM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormPM 메시지 처리기입니다.

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
LRESULT CFormPM::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		CString szPath;
		if(GetMainHandler()->m_nLangIdx == LAN_KOR)
			szPath = theConfigBank.m_System.m_strProgramDataPath + _T("\\PMWarning_KOR.PNG");
		if(GetMainHandler()->m_nLangIdx == LAN_ENG)
			szPath = theConfigBank.m_System.m_strProgramDataPath + _T("\\PMWarning_ENG.PNG");
		if(GetMainHandler()->m_nLangIdx == LAN_VNM)
			szPath = theConfigBank.m_System.m_strProgramDataPath + _T("\\PMWarning_VNM.PNG");

		if(CFileSupport::FileCheck(szPath)  == TRUE )
		{
			m_imgPM.ClearImage();
			m_imgPM.LoadImage(szPath.GetBuffer());
			m_imgPM.Invalidate();	
		}

		//kjpark 20161028 OPCALL, PMMode Tower Lamp 추가
		//kjpark 20161105 옵션 알람 타이틍 선택 안되는 버그 수정
// 		if(theConfigBank.m_Option.iMapLampSettingList.size() > 0)
// 		{
// 			theUnitFunc.LampChange(theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Red.lamp_state
// 				,theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Yellow.lamp_state
// 				, theConfigBank.m_Option.iMapLampSettingList[LAMP_PM].Green.lamp_state
// 				,FALSE);
// 		}		
	}
	else
	{
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormPM::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormPM::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}





//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

// 지정 화면으로 폼을 변경한다.
void CFormPM::ChangeForm(int nIdx, BOOL bForce)
{
	// CMainFrame에게 Message로 호면 전환을 지시한다.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)nIdx, (LPARAM)bForce);
}


void CFormPM::ClickBtnReleasePM()			//	PM 모드 해제
{
	CMainFrame* pMain = GetMainHandler();
	
// 	if(theUnitFunc.DoorInterLockChkStart() == FALSE)
// 		return;
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
			_T("Please login Engineer"),		
			_T("Vui long dang nhap Engineer"), GetMainHandler()->m_nLangIdx );

		dlgMsgBox.DoModal();
		return;
	}
// 	if(theConfigBank.m_Option.iMapLampSettingList.size() > 0)
// 	{
// 		theUnitFunc.LampChange(theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Red.lamp_state
// 			,theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Yellow.lamp_state
// 			, theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Green.lamp_state
// 			,FALSE);
// 	}		

	pMain->setModePM(FALSE);

	// 현재 Tower lamp의 상태를 PM mode로 바꾼다 - LSH171128
	theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[theProcBank.GetMachineState()].Red.lamp_state
		,theConfigBank.m_Option.iMapLampSettingList[theProcBank.GetMachineState()].Yellow.lamp_state
		, theConfigBank.m_Option.iMapLampSettingList[theProcBank.GetMachineState()].Green.lamp_state
		,FALSE);

	ChangeForm(eFORM_Main);
}


void CFormPM::ClickBtnOpenDoor()			//	문 열기
{
	CDlgDoor dlgDoor;			

	dlgDoor.DoModal();
}

