// FormConfig1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormAlarmLamp.h"
#include "UI\GausGUI\GxMsgBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNCREATE(CFormAlarmLamp, CFormView)

CFormAlarmLamp::CFormAlarmLamp()
	: CFormView(CFormAlarmLamp::IDD)
{
	Init();	
}

CFormAlarmLamp::~CFormAlarmLamp()
{
}

void CFormAlarmLamp::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormAlarmLamp, CFormView)
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	//ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
END_MESSAGE_MAP()

//kjpark 20161022 설정->램프 설정 구현
BEGIN_EVENTSINK_MAP(CFormAlarmLamp, CFormView)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_BUTTON_SAVE, DISPID_CLICK, CFormAlarmLamp::OnButtonSave, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INIT_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_INIT_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INIT_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_INIT_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INIT_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_INIT_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_IDLE_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_IDLE_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_IDLE_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_IDLE_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_IDLE_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_IDLE_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_STOP_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_STOP_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_STOP_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_STOP_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_STOP_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_STOP_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_RUN_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_RUN_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_RUN_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_RUN_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_RUN_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_RUN_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_ALARM_RED, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_ALARM_Red, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_ALARM_YELLOW, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_ARARM_Yellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_ALARM_GREEN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_ARARM_Green, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_INIT, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Init, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_IDLE, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Idle, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_STOP, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Stop, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_RUN, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Run, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_ARM, DISPID_CLICK, CFormAlarmLamp::OnButtonClick_Title_Ararm, VTS_NONE)

	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_CHECK, DISPID_CLICK, CFormAlarmLamp::ClickGxbtnAlarmLampTitleCheck, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_MAINT, DISPID_CLICK, CFormAlarmLamp::ClickGxbtnAlarmLampTitleMaint, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_INTERLOCK, DISPID_CLICK, CFormAlarmLamp::ClickGxbtnAlarmLampTitleInterlock, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_CHECK_RED, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleCheckRed, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_CHECK_YELLOW, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleCheckYellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_CHECK_GREEN, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleCheckGreen, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_MAINT_YELLOW, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleMaintYellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_MAINT_GREEN, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleMaintGreen, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INTERLOCK_RED, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleInterlockRed, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INTERLOCK_YELLOW, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleInterlockYellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_INTERLOCK_GREEN, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleInterlockGreen, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_MAINT_RED, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleMaintRed, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_OP_CALL, DISPID_CLICK, CFormAlarmLamp::ClickGxbtnAlarmLampTitleOpCall, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_OP_CALL_RED, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleOpCallRed, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_OP_CALL_YELLOW, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleOpCallYellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_OP_CALL_GREEN, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitleOpCallGreen, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_PMMODE_RED, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitlePmmodeRed, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_PMMODE_YELLOW, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitlePmmodeYellow, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXLBL_ALARM4_TITLE_PMMODE_GREEN, DISPID_CLICK, CFormAlarmLamp::ClickGxlblAlarm4TitlePmmodeGreen, VTS_NONE)
	ON_EVENT(CFormAlarmLamp, IDC_GXBTN_ALARM_LAMP_TITLE_PMMODE, DISPID_CLICK, CFormAlarmLamp::ClickGxbtnAlarmLampTitlePmmode, VTS_NONE)
END_EVENTSINK_MAP()
// CFormConfig1 진단입니다.
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonSave()
{
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("TOWER LAMP 값을 저장하시겠습니까?"), 
		_T("Do you want to save the TOWER LAMP value?"),		
		_T("Bạn có muốn lưu các giá trị TOWER LAMP?") , GetMainHandler()->m_nLangIdx);

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("CONFIG"), _T("LAMP_SAVE"));
	SaveAlarmFile();
	ReadAlarmLampFile();
	setLamp();
}
#ifdef _DEBUG
void CFormAlarmLamp::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormAlarmLamp::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

//kjpark 20161022 설정->램프 설정 구현
LRESULT CFormAlarmLamp::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		ReadAlarmLampFile();
		setLamp();
	}
	else
	{
	}

	return 0;
}


// CFormConfig1 메시지 처리기입니다.

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_INIT_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INIT].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INIT].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_INIT_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INIT].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INIT].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_INIT_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INIT].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INIT].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_IDLE_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_IDLE].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_IDLE].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_IDLE_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_IDLE].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_IDLE].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_IDLE_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_IDLE].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_IDLE].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_STOP_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_STOP_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_STOP_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}


//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_RUN_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_RUN].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_RUN].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_RUN_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_RUN].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_RUN].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_RUN_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_RUN].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_RUN].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
	 
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_ALARM_Red()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_FAULT].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_FAULT].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_ARARM_Yellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_FAULT].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_FAULT].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_ARARM_Green()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_FAULT].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_FAULT].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}
	 
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Init()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INIT].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Idle()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_IDLE].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Stop()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PAUSE].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Run()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_RUN].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::OnButtonClick_Title_Ararm()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_FAULT].bSelect = TRUE;
	setLamp();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::Init()
{
	InitLamp();
	InitList();
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::InitLamp()
{
	iMapLamp.insert(pair<UINT, BOOL>(IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, FALSE));	
	iMapLamp.insert(pair<UINT, BOOL>(IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, FALSE));	
	iMapLamp.insert(pair<UINT, BOOL>(IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, FALSE));

	//파일을 읽어와서 BOOL을 재정의
	if(this->GetSafeHwnd() != NULL)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, GXCOLOR_OFF);
	}
	
	
}
//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::SetDisplay(map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator iter)
{
	CGxUICtrl::SetButtonString(this, iter->second.nResourceID, iter->second.strList);		

	if(iter->second.Red.lamp_state == LAMP_ON)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, GXCOLOR_RED);
		CGxUICtrl::SetButtonColor(this,iter->second.Red.nResourceID, GXCOLOR_ON);		
		CGxUICtrl::SetButtonString(this, iter->second.Red.nResourceID, _T("ON"));		
	}
	else if(iter->second.Red.lamp_state == LAMP_FLICKER)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, GXCOLOR_RED);
		CGxUICtrl::SetButtonColor(this,iter->second.Red.nResourceID, GXCOLOR_YELLOW);		
		CGxUICtrl::SetButtonString(this, iter->second.Red.nResourceID, _T("FLICK"));		
	}
	else
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_RED_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,iter->second.Red.nResourceID, GXCOLOR_OFF);		
		CGxUICtrl::SetButtonString(this,iter->second.Red.nResourceID, _T("OFF"));		
	}

	if(iter->second.Yellow.lamp_state == LAMP_ON)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, GXCOLOR_YELLOW);
		CGxUICtrl::SetButtonColor(this,iter->second.Yellow.nResourceID, GXCOLOR_ON);		
		CGxUICtrl::SetButtonString(this,iter->second.Yellow.nResourceID, _T("ON"));		
	}
	else if(iter->second.Yellow.lamp_state == LAMP_FLICKER)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, GXCOLOR_YELLOW);
		CGxUICtrl::SetButtonColor(this,iter->second.Yellow.nResourceID, GXCOLOR_YELLOW);		
		CGxUICtrl::SetButtonString(this, iter->second.Yellow.nResourceID, _T("FLICK"));		
	}
	else
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_YELLOW_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,iter->second.Yellow.nResourceID, GXCOLOR_OFF);
		CGxUICtrl::SetButtonString(this,iter->second.Yellow.nResourceID, _T("OFF"));
	}

	if(iter->second.Green.lamp_state == LAMP_ON)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, GXCOLOR_GREEN);
		CGxUICtrl::SetButtonColor(this,iter->second.Green.nResourceID, GXCOLOR_ON);		
		CGxUICtrl::SetButtonString(this,iter->second.Green.nResourceID, _T("ON"));		
	}
	else if(iter->second.Green.lamp_state == LAMP_FLICKER)
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, GXCOLOR_GREEN);
		CGxUICtrl::SetButtonColor(this,iter->second.Green.nResourceID, GXCOLOR_YELLOW);		
		CGxUICtrl::SetButtonString(this, iter->second.Green.nResourceID, _T("FLICK"));		
	}
	else
	{
		CGxUICtrl::SetButtonColor(this,IDC_GXLBL_ALARM4_GRAUND_GREEN_BUTTON, GXCOLOR_OFF);
		CGxUICtrl::SetButtonColor(this,iter->second.Green.nResourceID, GXCOLOR_OFF);
		CGxUICtrl::SetButtonString(this,iter->second.Green.nResourceID, _T("OFF"));
	}
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::setLamp()
{
	if(this->GetSafeHwnd() != NULL)
	{
		map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
		for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
		{	
			SetDisplay(itmap);				
		}


		itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
		for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
		{			
			if(itmap->second.bSelect)
			{
				SetDisplay(itmap);				
				CGxUICtrl::SetButtonColor(this,itmap->second.nResourceID, GXCOLOR_ON);
			}
			else
			{
				CGxUICtrl::SetButtonColor(this,itmap->second.nResourceID, GXCOLOR_OFF);
			}
		}
		
	}
	int nState = theProcBank.GetMachineState();
	theUnitFunc.TowerLamp_Change(theConfigBank.m_Option.iMapLampSettingList[(CONST_MACHINE_STATE::ID)nState].Red.lamp_state
		,theConfigBank.m_Option.iMapLampSettingList[(CONST_MACHINE_STATE::ID)nState].Yellow.lamp_state
		, theConfigBank.m_Option.iMapLampSettingList[(CONST_MACHINE_STATE::ID)nState].Green.lamp_state
		,FALSE);
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::InitList()
{
// 	if(GetMainHandler() == NULL)
// 		return;

	LAMP_SETTING_LIST_BUTTON setting;
	LAMP_SETTING_LIST button;
	setting.lamp_state = LAMP_OFF;

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INIT_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INIT_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INIT_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;	
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_INIT].strName;
	button.bSelect = TRUE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_INIT;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_INIT, button));

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_IDLE_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_IDLE_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_IDLE_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_IDLE].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_IDLE;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_IDLE, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_STOP_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_STOP_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_STOP_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_PAUSE].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_STOP;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_PAUSE, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_RUN_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_RUN_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_RUN_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_RUN].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_RUN;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_RUN, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_ALARM_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_ALARM_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_ALARM_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_FAULT].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_ARM;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_FAULT, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_CHECK_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_CHECK_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_CHECK_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_CHECK].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_CHECK;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_CHECK, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_MAINT_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_MAINT_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_MAINT_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_MAINT].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_MAINT;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_MAINT, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INTERLOCK_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INTERLOCK_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_INTERLOCK_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_INTERLOCK].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_INTERLOCK;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_INTERLOCK, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_OP_CALL_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_OP_CALL_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_OP_CALL_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_OPCALL].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_OP_CALL;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_OPCALL, button));	

	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_PMMODE_RED;
	setting.strColor = _T("RED");
	button.Red = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_PMMODE_YELLOW;
	setting.strColor = _T("YELLOW");
	button.Yellow = setting;
	setting.nResourceID = IDC_GXLBL_ALARM4_TITLE_PMMODE_GREEN;
	setting.strColor = _T("GREEN");
	button.Green = setting;
	button.strList = ConstMachineState[CONST_MACHINE_STATE::MACHINE_PMMODE].strName;
	button.bSelect = FALSE;
	button.nResourceID = IDC_GXBTN_ALARM_LAMP_TITLE_PMMODE;
	theConfigBank.m_Option.iMapLampSettingList.insert(pair<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>(CONST_MACHINE_STATE::MACHINE_PMMODE, button));	

	ReadAlarmLampFile();

}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::ReadAlarmLampFile()
{
	CString strPath;
	strPath.Format(_T("%s\\AlarmLamp.ini"), theConfigBank.m_System.m_strConfigPath);
	ReadFile(strPath);
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::SaveAlarmFile()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
	strPath.Format(_T("%s\\AlarmLamp.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\AlarmLamp_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.Format(_T("%H%M%S")));


	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}
		SaveFile(strPath);
}


//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::ReadFile( CString strFilePath )
{
	// 파일 입출력은 티칭값과 속도만 한다.
	CIni ini(strFilePath);

	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{
		itmap->second.Red.lamp_state = (LAMP_STATE)ini.GetLong(itmap->second.strList, itmap->second.Red.strColor);
		itmap->second.Yellow.lamp_state = (LAMP_STATE)ini.GetLong(itmap->second.strList, itmap->second.Yellow.strColor);
		itmap->second.Green.lamp_state = (LAMP_STATE)ini.GetLong(itmap->second.strList, itmap->second.Green.strColor);
	}
}

//kjpark 20161022 설정->램프 설정 구현
void CFormAlarmLamp::SaveFile( CString strFilePath)
{
	// 파일 입출력은 티칭값과 속도속만 한다.
	CIni ini(strFilePath);

	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{
		LAMP_SETTING_LIST list = itmap->second;
		ini.WriteLong(list.strList, list.Red.strColor, list.Red.lamp_state);
		ini.WriteLong(list.strList, list.Yellow.strColor, list.Yellow.lamp_state);
		ini.WriteLong(list.strList, list.Green.strColor, list.Green.lamp_state);
	}	
}


void CFormAlarmLamp::ClickGxbtnAlarmLampTitleCheck()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_CHECK].bSelect = TRUE;
	setLamp();
}


void CFormAlarmLamp::ClickGxbtnAlarmLampTitleMaint()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_MAINT].bSelect = TRUE;
	setLamp();
}


void CFormAlarmLamp::ClickGxlblAlarm4TitleCheckRed()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_CHECK].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_CHECK].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleCheckYellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_CHECK].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_CHECK].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleCheckGreen()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_CHECK].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_CHECK].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}



void CFormAlarmLamp::ClickGxlblAlarm4TitleMaintRed()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_MAINT].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_MAINT].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleMaintYellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_MAINT].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_MAINT].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleMaintGreen()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_MAINT].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_MAINT].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}


//////////////////// INTER_LOCK ////////////////////
void CFormAlarmLamp::ClickGxbtnAlarmLampTitleInterlock()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].bSelect = TRUE;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleInterlockRed()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleInterlockYellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleInterlockGreen()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_INTERLOCK].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}


//////////////////// OP_CALL ////////////////////
void CFormAlarmLamp::ClickGxbtnAlarmLampTitleOpCall()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].bSelect = TRUE;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleOpCallRed()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleOpCallYellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitleOpCallGreen()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_OPCALL].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}


//////////////////// PM_MODE ////////////////////
void CFormAlarmLamp::ClickGxbtnAlarmLampTitlePmmode()
{
	map<CONST_MACHINE_STATE::ID, LAMP_SETTING_LIST>::iterator itmap = theConfigBank.m_Option.iMapLampSettingList.begin();
	for(; itmap != theConfigBank.m_Option.iMapLampSettingList.end(); ++ itmap)
	{	
		itmap->second.bSelect = FALSE;	
	}

	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].bSelect = TRUE;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitlePmmodeRed()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Red.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Red.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitlePmmodeYellow()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Yellow.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Yellow.lamp_state = (LAMP_STATE)nState;
	setLamp();
}

void CFormAlarmLamp::ClickGxlblAlarm4TitlePmmodeGreen()
{
	int nState = theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Green.lamp_state;
	nState = (nState >= LAMP_OFF) ? LAMP_ON:(nState+1);
	theConfigBank.m_Option.iMapLampSettingList[CONST_MACHINE_STATE::MACHINE_PMMODE].Green.lamp_state = (LAMP_STATE)nState;
	setLamp();
}