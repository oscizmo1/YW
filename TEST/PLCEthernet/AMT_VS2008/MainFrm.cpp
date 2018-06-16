// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MT.h"

#include "MainFrm.h"

#include "ViewTop.h"
#include "ViewMain.h"
#include "ViewBottom.h"

#include "frmAuto.h"
#include "frmManual.h"
#include "frmIO.h"		
#include "frmTeach.h"		
#include "frmAlign.h"		
#include "frmSetting.h"	
#include "frmCOMM.h"		
#include "frmCIM.h"		
#include "frmLog.h"	

#include "DlgCellInfo.h"
#include "MatchingDeviceModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
#define 	TOWER_LED_BLINK_CHECK	2

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_UNSCRAP, &CMainFrame::OnUnScrap)
	ON_MESSAGE(WM_SHOW_MSG_BOX, &CMainFrame::OnShowMsgBox)
	ON_MESSAGE(WM_MATCHING_DEVICE_MODEL, &CMainFrame::OnShowMatchingDeviceModel)
	ON_MESSAGE(WM_SHOW_ENGINEER_DOWN_CODE, &CMainFrame::OnShowEngineerDownCode)
END_MESSAGE_MAP()



// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	if(GetDoc()->m_bPlcEthernetType)
		gl_pPLCComm = &m_PlcEthernet;

	m_bPreInitialize	= false;
	m_nUserLevel		= OPERATOR;

	m_nCurrentFormID = FORM_MAX;
	m_bWatchDog = TRUE;
	m_nTitleNo = MSG_NORMAL;
	m_nErrNo = ERR_NONE;
	m_bShowMsgBox = FALSE;

	m_bMelsecRead = FALSE;	// 2009.07.16 Hui
	m_bMelsecWrite = FALSE;
	m_bShowEngineerDownCodeDlg = FALSE;
}

CMainFrame::~CMainFrame()
{

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style	= WS_POPUP;

	cs.cx		= 1024;
	cs.cy		= 768;
	cs.x		= (GetSystemMetrics(SM_CXSCREEN)-1024)/2;
	cs.y		= (GetSystemMetrics(SM_CYSCREEN)-768)/2;

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetMenu(NULL);

	SetWindowText(MT_TITLE);

// 	if(m_MsgBox.Create(IDD_DIALOG_MSG, this))
// 	{
// // 		m_MsgBox.BringWindowToTop();
// 		m_MsgBox.ShowWindow(SW_HIDE);
// 		m_MsgBox.AlarmBitOff();
// 	}
// 	else
// 	{
// 		AfxMessageBox("Msg Box create Fail!");
// 	}

	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)		return TRUE; 

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::PathInitialize()
{
	CFileFind FileFinder;

	if(!FileFinder.FindFile(MAIN_PATH))
		CreateDirectory(MAIN_PATH,NULL);

	if(!FileFinder.FindFile(SYSTEM_PATH))
		CreateDirectory(SYSTEM_PATH,NULL);
	
	if(!FileFinder.FindFile(EVENT_PATH))
		CreateDirectory(EVENT_PATH,NULL);

	if(!FileFinder.FindFile(RECIPE_PATH))
		CreateDirectory(RECIPE_PATH,NULL);

	if(!FileFinder.FindFile(IMAGE_PATH))
		CreateDirectory(IMAGE_PATH,NULL);

	if(!FileFinder.FindFile(ALIGN_PATH))
		CreateDirectory(ALIGN_PATH,NULL);

	if(!FileFinder.FindFile(LOG_PATH))
		CreateDirectory(LOG_PATH,NULL);

}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	int nRet;
	int nTowerLamp[3] = {2, 7, 12};
	int nTowerLampIO[3] = {
		GetDoc()->GetPortMap()->oTowerLampRed, GetDoc()->GetPortMap()->oTowerLampYellow, GetDoc()->GetPortMap()->oTowerLampGreen};
	int nSwitchLampIO[3] = {
		GetDoc()->GetPortMap()->oResetLamp, GetDoc()->GetPortMap()->oStopLamp, GetDoc()->GetPortMap()->oStartLamp};
		switch(nIDEvent)
		{
		case WATCH_DOG:
			if(m_bWatchDog)
				GetMelsec()->MelsecNetBitOff(MEL_SW_WATCH_DOG);
			else
				GetMelsec()->MelsecNetBitOn(MEL_SW_WATCH_DOG);
			m_bWatchDog = !m_bWatchDog;
			break;
		case CLOCK:
			break;
		case TOWER_LED_BLINK_CHECK:
			bLampState = !bLampState;
			for(int i = 0; i < 3; i++)
			{
				if(GetDoc()->strTowerLamp[GetApp()->m_RunStatus].GetAt(nTowerLamp[i]) -'0' == 1)
				{
					if(bLampState == TRUE)
					{
						DioOutput(nTowerLampIO[i],TRUE);
						DioOutput(nSwitchLampIO[i],TRUE);
					}
					else
					{
						DioOutput(nTowerLampIO[i],FALSE);
						DioOutput(nSwitchLampIO[i],FALSE);
					}
				}
			}
			break;
		case PLC_ETHERNET:
			{
				KillTimer(PLC_ETHERNET);
				GetMelsec()->EthernetProc();
			}
			break;

		case TIMER_TILT_WAIT:
			if( (GetTickCount() - GetDeviceCtl()->m_dTimer) > 500 )
			{
				if( GetLineType() == REMOVE_TILTING_TYPE )
				{
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_MOVE_READY);
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_MOVE_INSP);
					if( GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_BUSY) && GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_PRE_SIGN) )
						GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_PRE_SIGN);
				}

				nRet = GetDeviceCtl()->CheckMoveTilting(GetDeviceCtl()->m_bTilt);
				if(nRet == MOVE_OK_END)
				{
					KillTimer(TIMER_TILT_WAIT);
					if( GetLineType() == REMOVE_TILTING_TYPE )
						if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN ) )
							GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
				}
				else if(nRet < MOVE_ERR_NONE)
				{
					if( GetLineType() == REMOVE_TILTING_TYPE )
						if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN ) )
							GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
					::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_MANUAL, ERR_TILTING_MOTOR_MOVING);
					GetApp()->ChangeStatus(STATUS_ERROR);
					KillTimer(TIMER_TILT_WAIT);
				}
			}
			break;
		case TIMER_BIT_TOGGLE:
			if( (GetTickCount() - GetDeviceCtl()->m_dToggleTimer) > 1000 )
			{
				KillTimer(TIMER_BIT_TOGGLE);
				if(GetMelsec()->MelsecNetBitRecv(MEL_SW_UNLOADER_CLEAR))
				{
					GetMelsec()->MelsecNetBitOff(MEL_SW_UNLOADER_CLEAR);
				}
				if(GetMelsec()->MelsecNetBitRecv(MEL_SW_LOADER_CLEAR))
				{
					GetMelsec()->MelsecNetBitOff(MEL_SW_LOADER_CLEAR);
				}
				if(GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_SERVO_ON))
				{
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_SERVO_ON);
				}
				if(GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_SERVO_OFF))
				{
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_SERVO_OFF);
				}
				if(GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_MOVE_READY))
				{
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_MOVE_READY);
				}
				if(GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_MOVE_INSP))
				{
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_MOVE_INSP);
				}
				if( GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_SAVE_INSP) )
				{
					Sleep(2000);
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_SAVE_INSP);
				}
				if( GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_SAVE_READY) )
				{
					Sleep(2000);
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_SAVE_READY);
				}
				if( GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_ALARM_RST) )
					GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_ALARM_RST);
			}
			break;

		default:
			break;
		}
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	PathInitialize();

	if(CreateForm(pContext) == FALSE)
		return FALSE;
	return TRUE;
}

void CMainFrame::SwitchForm(UINT nFormID)
{
	if(m_nCurrentFormID == nFormID)		return;

	UINT nOldFormID = m_nCurrentFormID;
    m_nCurrentFormID = nFormID;

	CView* pOldActiveForm = NULL;
	CView* pNewActiveForm = NULL;
	//처음 선택시는 이전 Form이 없다
	if(nOldFormID != FORM_MAX)
		pOldActiveForm = m_pForm[nOldFormID];
	pNewActiveForm = m_pForm[nFormID];

	//처음 선택시는 이전 Form이 없다
	if(pOldActiveForm)
	{
		pOldActiveForm->ShowWindow(SW_HIDE);
		pOldActiveForm->SetDlgCtrlID(nOldFormID);
	}
	pNewActiveForm->ShowWindow(SW_SHOWNOACTIVATE);
	pNewActiveForm->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	SetActiveView(pNewActiveForm);

    RecalcLayout();
}

BOOL CMainFrame::DestroyWindow()
{
	GetDoc()->SetLog("Program Exit", LOG_SYSTEM);
	m_MsgBox.AlarmBitOff();
// 	m_MsgBox.DestroyWindow();
	GetDeviceCtl()->SetSafeTilting(FALSE);
	GetDeviceCtl()->SetPCRun(FALSE);
	KillTimer(WATCH_DOG);

	GetDoc()->m_pMotion->HomeFlagResetAll();

	GetMelsec()->Close();

	return CFrameWnd::DestroyWindow();
}

BOOL CMainFrame::CreateForm( CCreateContext* pContext )
{
	if(!m_wndSplitter.CreateStatic(this, 3, 1))
		return FALSE;

	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CViewTop), CSize(1024, 60), pContext))
		return FALSE;

	if(!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CViewMain), CSize(1024, 640), pContext))
		return FALSE;

	if(!m_wndSplitter.CreateView(2, 0, RUNTIME_CLASS(CViewBottom), CSize(1024, 60), pContext))
		return FALSE;
	m_hTop = m_wndSplitter.GetPane(0, 0)->m_hWnd;
	m_hBottom = m_wndSplitter.GetPane(2, 0)->m_hWnd;

	CRect rectMain	= CRect(0,0,1024,640);

	m_pForm[FORM_AUTO] = new CFormAuto;
	m_pForm[FORM_MANUAL] = new CFormManual;
	m_pForm[FORM_IO] = new CFormIO;
	m_pForm[FORM_TEACH] = new CFormTeach;
	m_pForm[FORM_ALIGN] = new CFormAlign;
	m_pForm[FORM_SETTING] = new CFormSetting;
	m_pForm[FORM_COMM] = new CFormCOMM;
	m_pForm[FORM_CIM] = new CFormCIM;
	m_pForm[FORM_LOG] = new CFormLog;


	for(int i=FORM_AUTO;i<FORM_MAX;i++)
	{
		m_pForm[i]->Create(NULL, NULL, WS_CHILD, rectMain, m_wndSplitter.GetPane(1,0), IDD_FORM_AUTO+i, pContext);
		m_pForm[i]->ModifyStyleEx(NULL,WS_EX_STATICEDGE);
		m_pForm[i]->ShowWindow(SW_HIDE);
		m_pForm[i]->SetDlgCtrlID(IDD_FORM_AUTO+i);
	}

	return TRUE;
}

void CMainFrame::StartMainFrmTimer()
{
	if(GetMelsec()->IsConnected())
		SetTimer(WATCH_DOG, 1000, NULL);

	SetTimer(TOWER_LED_BLINK_CHECK, 500, NULL);
}

void CMainFrame::ShowMsgBox( int nTitleNo, int nErrNo )
{
	if(GetApp()->m_RunStatus == STATUS_EMO)
	{
		if(nErrNo == ERR_EMO_SWITCH_ON || nErrNo == ERR_MAIN_POWER || nErrNo == ERR_DOOR_OPEN)
			return;
	}
	// MSG_NORMAL이나 ERR_CANCEL은 기존에 저장되어 있던 것을 불러온다.
	// Error 발생 시점과 MsgBox 보여주는 시점이 틀릴 경우를 대비해 구현
	BOOL bSet = SetMsgNo(nTitleNo, nErrNo);

	m_MsgBox.SetInit(m_nTitleNo, m_nErrNo);
	if(m_MsgBox.IsWindowVisible() == FALSE)
	{
		m_MsgBox.ShowWindow(SW_SHOW);
// 		m_MsgBox.BringWindowToTop();
	}
	else
	{
		if(bSet)
		{
			m_MsgBox.SetMessage();
// 			m_MsgBox.BringWindowToTop();
		}
	}
}

BOOL CMainFrame::SetMsgNo( int nTitleNo, int nErrNo )
{
//	if(nTitleNo == MSG_AUTO_SELECT)	nTitleNo = (GetApp()->IsRunStatus()) ? MSG_ERR_AUTO:MSG_ERR_MANUAL;
	if(nTitleNo == MSG_AUTO_SELECT)	
		nTitleNo = MSG_ERR_AUTO;

	if(m_nTitleNo == nTitleNo && m_nErrNo == nErrNo)
		return FALSE;

	if(nTitleNo != MSG_NORMAL)	m_nTitleNo = nTitleNo;
	if(nErrNo != ERR_CANCEL)	m_nErrNo = nErrNo;

	return TRUE;
}

LRESULT CMainFrame::OnUnScrap( UINT wParam, LPARAM lParam )
{
	CDlgCellInfo dlg;
	dlg.DoModal();
	GetDoc()->m_pEqMainSeq->m_bUnScrapping = FALSE;
	return 0L;
}

LRESULT CMainFrame::OnShowMsgBox( UINT wParam, LPARAM lParam )
{
	int nTitleNo = (int)wParam;
	int nErrNo = (int)lParam;
	if(GetApp()->m_RunStatus == STATUS_EMO)
	{
		if(nErrNo == ERR_EMO_SWITCH_ON || nErrNo == ERR_MAIN_POWER || nErrNo == ERR_DOOR_OPEN)
			return 0L;
	}
	// MSG_NORMAL이나 ERR_CANCEL은 기존에 저장되어 있던 것을 불러온다.
	// Error 발생 시점과 MsgBox 보여주는 시점이 틀릴 경우를 대비해 구현
	BOOL bSet = SetMsgNo(nTitleNo, nErrNo);

	m_MsgBox.SetInit(m_nTitleNo, m_nErrNo);
	if(m_bShowMsgBox == FALSE)
	{
		m_bShowMsgBox = TRUE;
		m_MsgBox.DoModal();
		m_bShowMsgBox = FALSE;
	}
	else
	{
		if(bSet)
			m_MsgBox.SetMessage();
	}
	return 0L;
}

LRESULT CMainFrame::OnShowMatchingDeviceModel( UINT wParam, LPARAM lParam )
{
	CMatchingDeviceModel dlg;
	dlg.SetDeviceID(GetDoc()->m_NonStopDeviceID);
	if(dlg.DoModal() == IDOK)
	{
		GetDoc()->AddModelAtDevice(GetDoc()->m_NonStopDeviceID, dlg.GetModelName());
		GetDoc()->m_bModelMatchingComp = TRUE;
	}
	else
	{
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_AUTO_SELECT, ERR_NON_STOP_DEVICE_MATCH);	
		GetDoc()->m_pEqMainSeq->SetNonStopFail();
		GetApp()->ChangeStatus(STATUS_ERROR);
	}
	return 0L;
}

LRESULT CMainFrame::OnShowEngineerDownCode( UINT wParam, LPARAM lParam )
{
	m_dlgEngineerDownCode.DoModal();
	if(m_dlgEngineerDownCode.m_nCode > 0)
		m_strEngineerDownCode.Format("%d", m_dlgEngineerDownCode.m_nCode);
	else
		m_strEngineerDownCode = "";
	m_strEngineerDownText = m_dlgEngineerDownCode.m_strText;
	return 0;
}