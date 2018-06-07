// FormInitialize.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormInitialize.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxButtonEx.h"

// CFormInitialize

enum TIMER_INDEX
{
	TIMER_MONITOR,
	TIMER_STEP
};
IMPLEMENT_DYNCREATE(CFormInitialize, CFormView)

CFormInitialize::CFormInitialize()
	: CFormView(CFormInitialize::IDD)
{
	m_bInit = FALSE;
}

CFormInitialize::~CFormInitialize()
{
}

void CFormInitialize::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INITIALIZE_LOGLIST, m_ListLog);
}

BEGIN_MESSAGE_MAP(CFormInitialize, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFormInitialize 진단입니다.

#ifdef _DEBUG
void CFormInitialize::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormInitialize::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormInitialize 메시지 처리기입니다.

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
LRESULT CFormInitialize::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		AddAxis_to_List();
		m_Step = stepIdle;
		InitStatus();
		SetTimer(TIMER_MONITOR, 100, NULL);

	}
	else
	{
		KillTimer(TIMER_MONITOR);
		KillTimer(TIMER_STEP);
	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormInitialize::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	// CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormInitialize::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormInitialize, CFormView)
	ON_EVENT(CFormInitialize, IDC_GXBTN_INITIALIZE_INITIALIZED, DISPID_CLICK, CFormInitialize::ClickGxbtnInitializeInitialized, VTS_NONE)
	ON_EVENT(CFormInitialize, IDC_GXBTN_INITIALIZE_CANCEL, DISPID_CLICK, CFormInitialize::ClickGxbtnInitializeCancel, VTS_NONE)
END_EVENTSINK_MAP()


void CFormInitialize::OnInitialUpdate()
{
	if(m_bInit)	return;	// OnInitialUpdate 두번 실행되는 버그 방지 [10/24/2016 OSC]
	m_bInit = TRUE;
	CFormView::OnInitialUpdate();


	m_Color[ORIGIN_NONE] = Color(Color::LightGray).ToCOLORREF();
	m_Color[ORIGIN_START] = Color(Color::Yellow).ToCOLORREF();
	m_Color[ORIGIN_FINISH] = Color(Color::Lime).ToCOLORREF();
	m_Color[ORIGIN_FAIL] = Color(Color::Red).ToCOLORREF();

	LV_COLUMN lCol;
	lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

	lCol.fmt = LVCFMT_CENTER;   // 칼럼의 정렬을 왼쪽정렬로 지정(LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_RIGHT중 선택)
	lCol.pszText = _T("NO");    //  칼럼의 제목을 지정
	lCol.iSubItem = 0;    
	CRect rect;//  서브아이템의 인덱스를 지정
	m_ListLog.GetWindowRect(rect);
	lCol.cx = rect.Width()-30;               //  칼럼의 넓이를 지정(Scroll Bar를 안생기게 하기 위해 -5 한다)
	m_ListLog.InsertColumn(0, &lCol);  //  LVCOLUMN구조체로 만들어진 값을 토대로 리스트 컨트롤에 칼럼을 삽입
}


void CFormInitialize::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	BOOL bContinue = FALSE;
	switch(nIDEvent)
	{
	case TIMER_MONITOR:
		UpdateAllStatus();
		SetTimer(nIDEvent, 100, NULL);
		break;
	case TIMER_STEP:
		if(CheckInterlock())
		{
			if(Initializing())
			{
				bContinue = TRUE;
			}
		}
		if(bContinue)
		{
			SetTimer(nIDEvent, 100, NULL);
		}
		else if(m_StepOriginStatus[LED_FINISH] == ORIGIN_FINISH)
		{
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);

			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Inform"), _T("Inform"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("초기화 완료"), 
				_T("Initialize Success"), 
				_T("khởi tạo thành công") , GetMainHandler()->m_nLangIdx);

			dlgMsgBox.DoModal();
		}
		else
		{
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
			AxisAllStop();

			if(theProcBank.m_strLastKorMsg.IsEmpty() == FALSE)
			{
				CGxMsgBox	dlgMsgBox;
				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
				dlgMsgBox.SetMessage(FALSE, 
					theProcBank.m_strLastKorMsg, 
					theProcBank.m_strLastEngMsg,		
					theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

				dlgMsgBox.DoModal();
			}
		}
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CFormInitialize::AddAxis_to_List()
{
	m_list1stPart.RemoveAll();
	m_list2ndPart.RemoveAll();

	//첫번째 원점 잡을 축
	m_list1stPart.AddTail(AXIS_INSPECTION_Z1);
	m_list1stPart.AddTail(AXIS_INSPECTION_Z2);
	
	// 두번째 원점 잡을 축
	m_list2ndPart.AddTail(AXIS_INSPECTION_X1);
	m_list2ndPart.AddTail(AXIS_INSPECTION_X2);
	m_list2ndPart.AddTail(AXIS_JIG_SHUTTLE_Y1);
	m_list2ndPart.AddTail(AXIS_JIG_SHUTTLE_Y2);
	m_list2ndPart.AddTail(AXIS_ACTIVE_ALIGN_X1);
	m_list2ndPart.AddTail(AXIS_ACTIVE_ALIGN_X2);
}

void CFormInitialize::InitStatus()
{
	for(int i = 0; i < AXIS_ID_MAX; i++)
		m_AxisOriginStatus[i] = ORIGIN_NONE;

	for(int i = 0; i < STEP_LED_MAX; i++)
		m_StepOriginStatus[i] = ORIGIN_NONE;

	m_strLastMsg = _T("");
}

BOOL CFormInitialize::CheckPreInterlock( CString &strKorMsg, CString &strEngMsg, CString &strVnmMsg )
{
	// Origin 시작전 Tray 유무 및 Door, Light Curtain 등의 Interlock Sensor Check  [10/22/2016 OSC]
	if(theUnitFunc.DoorClose_Check() == FALSE)
	{
		strKorMsg = theProcBank.m_strLastKorMsg;
		strEngMsg = theProcBank.m_strLastEngMsg;
		strVnmMsg = theProcBank.m_strLastVnmMsg;
		return FALSE;
	}

	if(theUnitFunc.LightCurtain_Check() == FALSE)
	{
		strKorMsg = theProcBank.m_strLastKorMsg;
		strEngMsg = theProcBank.m_strLastEngMsg;
		strVnmMsg = theProcBank.m_strLastVnmMsg;
		return FALSE;
	}
	return TRUE;
}

BOOL CFormInitialize::CheckInterlock()
{
	// Origin 진행중 Door, Light Curtain 등의 Interlock Sensor Check  [10/22/2016 OSC]
	if(theUnitFunc.DoorClose_Check() == FALSE)
	{
		return FALSE;
	}

	if(theUnitFunc.LightCurtain_Check() == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

void CFormInitialize::InitCylinder()
{
	//yjkim 05.30
}

BOOL CFormInitialize::CheckCylinder()
{
	CString strMsg = _T("");

	////////////////////////////////////////////////////////////////////////
	 //검사부
	//yjkim 05.30
 //	if(theUnitFunc.GetInPutIOCheck(X_INSP_A_JIG_TILTING_UP) == FALSE)
 //	{
 //		m_strLastMsg.Format(_T("A Jig Tilt Up Fail"));
 //		return FALSE;
 //	}
	//if(theUnitFunc.GetInPutIOCheck(X_INSP_A_JIG_TILTING_UP) == FALSE)
	//{
	//	m_strLastMsg.Format(_T("B Jig Tilt Up Fail"));
	//	return FALSE;
	//}

	return TRUE;
}

BOOL CFormInitialize::ServoOff()
{
	CString strMsg = _T("");
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if( theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i).bAlarmOn
			|| (theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i).bServoOn == FALSE) )
		{
			if(theDeviceMotion.ServoOff(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i) == FALSE)
			{
				m_AxisOriginStatus[i] = ORIGIN_FAIL;
				strMsg.Format(_T("%s Servo Off Fail"), theAxisBank.m_Axis[i].m_strAxisName);
				break;
			}
		}
	}

	if(strMsg.GetLength() > 0)
	{
		SetLogListUpdate(strMsg);
		return FALSE;
	}

	return TRUE;
}

BOOL CFormInitialize::AlarmReset()
{
	CString strMsg = _T("");
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i).bAlarmOn)
		{
			if(theDeviceMotion.ResetAlarm(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i) == FALSE)
			{
				m_AxisOriginStatus[i] = ORIGIN_FAIL;
				strMsg.Format(_T("%s Alarm Reset Fail"), theAxisBank.m_Axis[i].m_strAxisName);
				break;
			}
		}
	}

	if(strMsg.GetLength() > 0)
	{
		SetLogListUpdate(strMsg);
		return FALSE;
	}

	return TRUE;
}

BOOL CFormInitialize::ServoOn()
{
	CString strMsg = _T("");
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i).bServoOn == FALSE)
		{
			if(theDeviceMotion.ServoOn(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i) == FALSE)
			{
				m_AxisOriginStatus[i] = ORIGIN_FAIL;
				strMsg.Format(_T("%s Servo On Fail"), theAxisBank.m_Axis[i].m_strAxisName);
				break;
			}
		}
	}

	if(strMsg.GetLength() > 0)
	{
		SetLogListUpdate(strMsg);
		return FALSE;
	}

	return TRUE;
}

BOOL CFormInitialize::InitAxisList( CList <AXIS_ID, AXIS_ID> *pList )
{
	POSITION pos = pList->GetHeadPosition();
	AXIS_ID axis;
	CAxisParam param;
	while (pos)
	{
		axis = pList->GetNext(pos);
		theAxisBank.GetParamNode((AXIS_ID)axis, param);
		if(param.m_bUse)
		{
			if(theDeviceMotion.OriginRetrun(MP2100_THREAD_MAIN_PROCESS, axis))
			{
				m_AxisOriginStatus[axis] = ORIGIN_START;
			}
			else
			{
				m_AxisOriginStatus[axis] = ORIGIN_FAIL;
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CFormInitialize::CheckAxisOriginComplete( CList <AXIS_ID, AXIS_ID> *pList )
{
	POSITION pos = pList->GetHeadPosition();
	AXIS_ID axis;
	BOOL bReturn = TRUE;
	CAxisParam param;
	while (pos)
	{
		axis = pList->GetNext(pos);
		theAxisBank.GetParamNode((AXIS_ID)axis, param);
		if(param.m_bUse)
		{
			if(theDeviceMotion.IsAxisReady(MP2100_THREAD_MAIN_PROCESS, axis, FALSE))
			{
				m_AxisOriginStatus[axis] = ORIGIN_FINISH;
			}
			else if(theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, axis).bAlarmOn)
			{
				m_AxisOriginStatus[axis] = ORIGIN_FAIL;
			}
			else
			{
				bReturn = FALSE;
			}
		}
	}
	return bReturn;
}

BOOL CFormInitialize::CheckAxisOriginFail()
{
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(m_AxisOriginStatus[i] == ORIGIN_FAIL)
		{
			CString strMsg;
			strMsg.Format(_T("%s is Fail"), theAxisBank.m_Axis[i].m_strAxisName);
			SetLogListUpdate(strMsg);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CFormInitialize::Initializing()
{
	int nStep = (int)m_Step;
	switch(nStep)
	{
	case stepStart:
		if(m_Timer.Stop(FALSE) > 1.)
		{
			m_Timer.Start();
			nStep++;
		}
		break;
	case stepServoOff:
		SetLogListUpdate(_T("Alarmed Axis Servo Off"));
		if(ServoOff())
		{
			m_StepOriginStatus[LED_SERVO_OFF] = ORIGIN_FINISH;
			m_Timer.Start();
			nStep++;
		}
		else
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_SERVO_OFF] = ORIGIN_FAIL;
			SetLogListUpdate(m_strLastMsg);
		}
		break;
	case stepAlarmReset:
		if(m_Timer.Stop(FALSE) > 1.5)
		{
			SetLogListUpdate(_T("Alarm Reset"));
			if(AlarmReset())
			{
				m_StepOriginStatus[LED_ALARM_RESET] = ORIGIN_FINISH;
				m_Timer.Start();
				nStep++;
			}
			else
			{
				nStep = stepAlarm;
				m_StepOriginStatus[LED_ALARM_RESET] = ORIGIN_FAIL;
				SetLogListUpdate(m_strLastMsg);
			}
		}
		break;
	case stepServoOn:
		if(m_Timer.Stop(FALSE) > 2.)
		{
			

			SetLogListUpdate(_T("Servo On"));
			if(ServoOn())
			{
				CGxButtonEx *pBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_CANCEL);
				pBtn->SetEnable(FALSE);					//07.15.khs
				m_StepOriginStatus[LED_SERVO_ON] = ORIGIN_FINISH;
				m_Timer.Start();
				nStep++;
			}
			else
			{
				nStep = stepAlarm;
				m_StepOriginStatus[LED_SERVO_ON] = ORIGIN_FAIL;
				SetLogListUpdate(m_strLastMsg);
			}
		}
		break;
	case stepInitCylinder:
		m_StepOriginStatus[LED_CYLINDER_INIT] = ORIGIN_START;
		SetLogListUpdate(_T("Cylinder Init Start"));
		m_Timer.Start();
		nStep++;
		break;
	case stepCheckCylinder:
		if(CheckCylinder())
		{
			m_StepOriginStatus[LED_CYLINDER_INIT] = ORIGIN_FINISH;
			SetLogListUpdate(_T("Cylinder Init Finish"));
			nStep++;
		}
		else if(m_Timer.Stop(FALSE) > 10.)
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_CYLINDER_INIT] = ORIGIN_FAIL;
			SetLogListUpdate(m_strLastMsg);
		}
		break;
	//초기화 시퀀스 추가(20161228 BKH)
	case stepInit1stList:
		if(m_Timer.Stop(FALSE) > 2.)
		{
			

			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_START;
			SetLogListUpdate(_T("1st Axis Origin Start"));
			if(InitAxisList(&m_list1stPart))
			{
				CGxButtonEx *pBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_CANCEL);
				pBtn->SetEnable(TRUE);					//07.15.KHS
				m_Timer.Start();
				nStep++;
			}
			else
			{
				m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
				nStep = stepAlarm;
			}
		}
		break;
	case stepCheck1stList:
		if(CheckAxisOriginComplete(&m_list1stPart))
		{
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FINISH;
			SetLogListUpdate(_T("1st Axis Origin Finish"));
			nStep++;
		}
		else if( (m_Timer.Stop(FALSE) > 60.) || CheckAxisOriginFail() )
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_1ST_ORIGIN] = ORIGIN_FAIL;
		}
		break;
	case stepInit2ndList:
		m_StepOriginStatus[LED_2ND_ORIGIN] = ORIGIN_START;
		SetLogListUpdate(_T("2nd Axis Origin Start"));
		if(InitAxisList(&m_list2ndPart))
		{
			m_Timer.Start();
			nStep++;
		}
		else
		{
			theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
			m_StepOriginStatus[LED_2ND_ORIGIN] = ORIGIN_FAIL;
			nStep = stepAlarm;
		}
		break;
	case stepCheck2ndList:
		if(CheckAxisOriginComplete(&m_list2ndPart))
		{
			m_StepOriginStatus[LED_2ND_ORIGIN] = ORIGIN_FINISH;
			SetLogListUpdate(_T("2nd Axis Origin Finish"));
			nStep++;
		}
		else if( (m_Timer.Stop(FALSE) > 60.) || CheckAxisOriginFail() )
		{
			nStep = stepAlarm;
			m_StepOriginStatus[LED_2ND_ORIGIN] = ORIGIN_FAIL;
		}
		break;
	//case stepInitCylinder:
	//		m_Timer.Start();
	//		nStep++;
	//	break;
	//case stepCheckCylinder:
	//	if(CheckCylinder())
	//	{
	//		m_StepOriginStatus[LED_CYLINDER_INIT] = ORIGIN_FINISH;
	//		SetLogListUpdate(_T("Cylinder Init Finish"));
	//		nStep++;
	//	}
	//	else if(m_Timer.Stop(FALSE) > 10.)
	//	{
	//		nStep = stepAlarm;
	//		m_StepOriginStatus[LED_CYLINDER_INIT] = ORIGIN_FAIL;
	//		SetLogListUpdate(m_strLastMsg);
	//	}
	//	break;
	case stepEnd:
		nStep = stepIdle;
		m_StepOriginStatus[LED_FINISH] = ORIGIN_FINISH;
		SetLogListUpdate(_T("All Initialize Finish"));
		CGxButtonEx *pBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_INITIALIZED);
		pBtn->SetEnable(TRUE);
// 		CGxButtonEx *pServoBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_SERVO_ON);
// 		pServoBtn->SetEnable(TRUE);
		break;

	}
	m_Step = (INITIALIZE_STEP)nStep;

	// 계속 진행 가능하면 TRUE [10/22/2016 OSC]
	if(m_Step > stepEnd)
		return FALSE;
	else
		return TRUE;
}

void CFormInitialize::AxisAllStop()
{
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(theDeviceMotion.IsAxisReady(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i, FALSE) == FALSE)
			theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, (AXIS_ID)i);
	}
}

void CFormInitialize::UpdateAllStatus()
{
	m_pAxisOriginLed[AXIS_INSPECTION_Z1			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSPECTION_Z1 				);
	m_pAxisOriginLed[AXIS_INSPECTION_X1			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSPECTION_X1 				);
	m_pAxisOriginLed[AXIS_ACTIVE_ALIGN_X1		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_ACTIVE_ALIGN_X1				);
	m_pAxisOriginLed[AXIS_JIG_SHUTTLE_Y1		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_JIG_SHUTTLE_Y1				);

	m_pAxisOriginLed[AXIS_INSPECTION_Z2			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSPECTION_Z2				);
	m_pAxisOriginLed[AXIS_INSPECTION_X2			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_INSPECTION_X2				);
	m_pAxisOriginLed[AXIS_ACTIVE_ALIGN_X2		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_ACTIVE_ALIGN_X2				);
	m_pAxisOriginLed[AXIS_JIG_SHUTTLE_Y2		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_AXIS_JIG_SHUTTLE_Y2				);


	
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		if(theAxisBank.MotorIsUsed((AXIS_ID)i))
		{
			if(m_pAxisOriginLed[i]->GetBackColor() != m_Color[m_AxisOriginStatus[i]])
				m_pAxisOriginLed[i]->SetBackColor(m_Color[m_AxisOriginStatus[i]]);
		}
		// 나중에 보완 예정 [6/13/2017 OSC]
// 		else if(m_pAxisOriginLed[i])
// 		{
// 			if(m_pAxisOriginLed[i]->GetSafeHwnd())
// 			{
// 				if(m_pAxisOriginLed[i]->GetBackColor() != m_Color[m_AxisOriginStatus[i]])
// 					m_pAxisOriginLed[i]->SetBackColor(m_Color[m_AxisOriginStatus[i]]);
// 			}
// 		}
	}

	m_pInitStepLed[LED_CYLINDER_INIT	] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_CYLINDER_CHECK	);
	m_pInitStepLed[LED_SERVO_OFF		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_SERVO_OFF		);
	m_pInitStepLed[LED_ALARM_RESET		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_ALARM_RESET		);
	m_pInitStepLed[LED_SERVO_ON			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_SERVO_ON		);
	m_pInitStepLed[LED_1ST_ORIGIN		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_1ST_ORIGIN		);
	m_pInitStepLed[LED_2ND_ORIGIN		] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_2ND_ORIGIN		);
	m_pInitStepLed[LED_FINISH			] = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LED_STEP_FINISH			);

	MC_POWER_ON_CHECK		   = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_MC_POWER_CHECK		         	);  // jypark0921 MC파워 체크
	X_SHUTTLE_TABLE_STO1_CHECK = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_X_SHUTTLE_TABLE_STO1			);  // jypark0921 셔틀 테이블 STO1_CHECK 01 체크
	X_SHUTTLE_TABLE_STO2_CHECK = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_X_SHUTTLE_TABLE_STO2			);  // jypark0921 셔틀 테이블 ST01_CHECK 02 체크 


	for(int i = 0; i < STEP_LED_MAX; i++)
	{
		if(m_pInitStepLed[i]->GetBackColor() != m_Color[m_StepOriginStatus[i]])
			m_pInitStepLed[i]->SetBackColor(m_Color[m_StepOriginStatus[i]]);
	}

	if(theUnitFunc.GetInPutIOCheck(X_MC_ON_CHECK) == TRUE)  // jypark0921 MC파워 io 온되어잇으면 어두운 시얀색
	{
		MC_POWER_ON_CHECK ->SetBackColor(GXCOLOR_ON);
	}
	else
	{
		MC_POWER_ON_CHECK ->SetBackColor(GXCOLOR_RED);      // jypark0921 MC파워 io 오프 되어있으면 빨간색
	}



	if(theUnitFunc.GetInPutIOCheck(X_SHUTTLE_TABLE_STO1) == TRUE) // jypark0921 셔틀 테이블 STO1 io 온 되어있으면 회색
	{
		X_SHUTTLE_TABLE_STO1_CHECK -> SetBackColor(GXCOLOR_RED);
	}
	else														   // jypark0921 셔틀 테이블 STO1 io 오프 되어있으면 빨간색
	{
		X_SHUTTLE_TABLE_STO1_CHECK -> SetBackColor(GXCOLOR_ON);
	}



	if(theUnitFunc.GetInPutIOCheck(X_SHUTTLE_TABLE_STO2) == FALSE) // jypark0921 셔틀 테이블 STO2 io 온 되어있으면 회색
	{
		X_SHUTTLE_TABLE_STO2_CHECK -> SetBackColor(GXCOLOR_ON);
	}
	else														    // jypark0921 셔틀 테이블 STO2 io 온 되어있으면 빨간색
	{
		X_SHUTTLE_TABLE_STO2_CHECK -> SetBackColor(GXCOLOR_RED);
	}

}

void CFormInitialize::SetLogListUpdate( CString strMessage )
{
	if(strMessage.GetLength() < 1)
		return;
	m_ListLog.InsertItem(m_ListLog.GetItemCount(), strMessage);

	int nCount = m_ListLog.GetItemCount();
	if (nCount > 0)
		m_ListLog.EnsureVisible(nCount-1, FALSE);
}

void CFormInitialize::ClickGxbtnInitializeInitialized()
{
	if(m_Step != stepIdle)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("초기화 진행중입니다."), 
			_T("Now initializing..."), 
			_T("Đang tiến hành khởi tạo ...") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	CString strKor, strEng, strVnm;
	BOOL bRet = CheckPreInterlock(strKor, strEng, strVnm);
	if(bRet == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, strKor, strEng, strVnm, GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	if(theUnitFunc.GetInPutIOCheck(X_MC_ON_CHECK) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("MC Power가 Off되어 있습니다."), 
			_T("Please Turn On MC Power."),		
			_T("Xin vui lòng bật ON MC Power.") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	STO_STATE sto = theUnitFunc.STO_Check();
	if(sto != STO_READY)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();

		// STO_WARNING 상태면 진행은 가능하니 메세지만 표시하고 넘어간다 [9/19/2017 OSC]
		if(sto == STO_ALARM)
			return;
	}


	if(theUnitFunc.AutoTeachKey_AutoCheck() == FALSE)
	{
		if(theUnitFunc.DoorLockOn_Check() == FALSE)
		{
			CGxMsgBox	dlgMsgBox;
			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				theProcBank.m_strLastKorMsg, 
				theProcBank.m_strLastEngMsg,		
				theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

			dlgMsgBox.DoModal();
			return;
		}
	}

	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("확인"), _T("Question"), _T("Question"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("초기화를 진행하시겠습니까?"), 
		_T("Initializing Now?"), 
		_T("Bạn muốn tiến hành khởi tạo lại?") , GetMainHandler()->m_nLangIdx);

	if(dlgMsgBox.DoModal() != IDOK)
		return;


	InitStatus();
	m_Step = stepStart;
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("INITIALIZED"));
	CGxButtonEx *pBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_INITIALIZED);
	pBtn->SetEnable(FALSE);

	SetLogListUpdate(_T("Initialize Start"));

	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_INIT);

	SetTimer(TIMER_STEP, 100, NULL);
}


void CFormInitialize::ClickGxbtnInitializeCancel()
{
	//20170501 LMS OPERATION 추가
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("SETUP"), _T("INITIALIZED_CANCEL"));
	KillTimer(TIMER_STEP);
	m_Step = stepIdle;
	CGxButtonEx *pBtn = (CGxButtonEx*)GetDlgItem(IDC_GXBTN_INITIALIZE_INITIALIZED);
	pBtn->SetEnable(TRUE);
	AxisAllStop();
	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
}

