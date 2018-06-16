// MT.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "MT.h"
#include "MainFrm.h"
#include "MTView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMTApp

BEGIN_MESSAGE_MAP(CMTApp, CWinApp)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMTApp 생성

CFormCOMM* gl_pComm = NULL;
CMTDoc* gl_pDoc = NULL;
CPLCComm* gl_pPLCComm = NULL;

CMTApp::CMTApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	m_RunStatus = STATUS_ALL;
	m_RunMode = MODE_NORMAL;
	m_bInspectionStart = FALSE;
	m_bInspectionEnd = TRUE;
	m_bInspectionResult = TRUE;
}


// 유일한 CMTApp 개체입니다.

CMTApp theApp;

// CMTApp 초기화

BOOL CMTApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.

	// 중복실행 방지
	HANDLE hEvent; 
    hEvent=CreateEvent(NULL,FALSE,TRUE,AfxGetAppName()); 
    if(GetLastError()==ERROR_ALREADY_EXISTS)
    { 
		AfxMessageBox("이미 실행중입니다!!!");
        return FALSE; 
    }

	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("YWDSP"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	SetTypes();
	SetDefaultPath();

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	// 문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMTDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CMTView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 명령줄에 지정된 명령을 디스패치합니다. 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
// 	((CMainFrame*)m_pMainWnd)->SetImageGrapView();
	GetDoc()->Initial();



	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	// SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.
	return TRUE;
}

// CMTApp 메시지 처리기


void CMTApp::SetDefaultPath()
{
	TCHAR szBuff[256];
	::GetModuleFileName(GetApp()->m_hInstance , szBuff, 256);
	char* pch = ::strrchr(szBuff , '\\');
	m_strExePath = CString(szBuff , pch - szBuff + 1);

	m_strRecipePath  = RECIPE_PATH;
	m_strAlignPath  = ALIGN_PATH;
	m_strLogPath	= LOG_PATH;
	m_strSystemPath = SYSTEM_PATH;
}

void CMTApp::SetTypes()
{
	//////////////////////////////////////////////////////////////////////////
	// 모든 함수 중에서 가장 먼저 호출한다.
	// Type load와 그에 따른 클래스 선정이 이루어지기 때문에 
	// 그 어떠한 것 보다 먼저 선행되어야 한다.

	// Notebook에서 실행하는지 장비에서 실행하는지...
	int nSimulation = this->GetProfileInt(_T("TYPE"), _T("Simulation"), 99);
	if(nSimulation == 99)
	{
		nSimulation = FALSE;
		this->WriteProfileInt(_T("TYPE"), _T("Simulation"), nSimulation);
	}
	SetSimulation(nSimulation);

	CString strEngPassWord = this->GetProfileString(_T("PassWord"), _T("ENGINEER"), _T("MMT"));
	if(strEngPassWord == _T("MMT"))
	{
		strEngPassWord = "MT";
		this->WriteProfileString(_T("PassWord"), _T("ENGINEER"), strEngPassWord);
	}
	SetEngineerPassWord(strEngPassWord);

	CString strSuperPassWord = this->GetProfileString(_T("PassWord"), _T("SUPERUSER"), _T("ywdsp"));
	if(strSuperPassWord == _T("ywdsp"))
	{
		strSuperPassWord = "YWDSP";
		this->WriteProfileString(_T("PassWord"), _T("SUPERUSER"), strSuperPassWord);
	}
	SetSuperUserPassWord(strSuperPassWord);

	// Load LineType
	int nTypeValue = this->GetProfileInt(_T("TYPE"), _T("LineType"), 99);
	if(nTypeValue == 99)
	{
		nTypeValue = SINGLE_ARM_TYPE;
		this->WriteProfileInt(_T("TYPE"), _T("LineType"), nTypeValue);
	}
	SetLineType((LineType)nTypeValue);

	// Load MachinePCType
	nTypeValue = this->GetProfileInt(_T("TYPE"), _T("MachinePCType"), 99);
	if(nTypeValue == 99)
	{
		nTypeValue = A_MMT1;
		this->WriteProfileInt(_T("TYPE"), _T("MachinePCType"), nTypeValue);
	}
	SetMachinePCType((MachinePCType)nTypeValue);

	// Load LangueageType
	nTypeValue = this->GetProfileInt(_T("TYPE"), _T("LangueageType"), 99);
	if(nTypeValue == 99)
	{
		nTypeValue = LAN_ENG;
		this->WriteProfileInt(_T("TYPE"), _T("LangueageType"), nTypeValue);
	}
	SetLanguageType((LangueageType)nTypeValue);

	// D군 부터는 Tilting 제어가 PLC로 인계된다. [5/30/2009 OSC]
	if(GetLineType() == REMOVE_TILTING_TYPE)
		m_MaxAxis = 8;
	else
		m_MaxAxis = 9;
}

int CMTApp::ExitInstance()
{
	GetDeviceCtl()->Release();
	GetAlignCtl()->Release();

	return CWinApp::ExitInstance();
}


void CMTApp::Start()
{
	// 유실내역창 입력 안했으면 START 금지 [1/8/2011 OSC]
	if(GetMainFrame()->m_bShowEngineerDownCodeDlg)
		return;

	if (GetComm()->m_iCurEQState == eFault)
	{
		AfxMessageBox("Alarm Reset Please!.");
		return;
	}
	if(m_RunStatus == STATUS_STOP)
	{
		// Home Flag Check
		if(GetDoc()->m_pMotion->CheckHomeFlagAll() == FALSE)
		{
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_CHECK, CHK_MOTION_INITIALIZE);
			return;
		}
		// Moving중인 Axis Check
		if(GetDeviceCtl()->m_bMoveTilting || GetDoc()->m_pEqMainSeq->m_nSeqContactNo != SEQ_CONTACT_CHK_REQUEST
			|| GetDeviceCtl()->m_nMoveStep != MS_CHK_EQUIPMENT)
		{
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_CHECK, CHK_MOTION_READY);
			return;
		}

		// Operator level로 전환
		GetDoc()->SetSuperUser(FALSE);
		::SendMessage(GetMainFrame()->m_hTop, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);
		::SendMessage(GetMainFrame()->m_hBottom, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);
		::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);

		if(  GetDoc()->m_pEqMainSeq->IsPanelUnloadComplecte() &&
			(GetMelsec()->MelsecNetBitRecv(UNLD_IN_REQ_SUPPLY) ||
			 GetMelsec()->MelsecNetBitRecv(UNLD_IN_DO_SUPPLY)))
		{
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_CHECK, ERR_UNLD_COMMUNICATION_CLEAR);
			return;
		}

//		if( GetMachinePCType() == A_MMT1 )	GetMelsec()->MelsecNetBitRecv( /**/+MEL_REQ_SUPPLY );
		if( !GetDoc()->m_pEqMainSeq->IsPanelUnloadComplecte() && GetDeviceCtl()->ChkPanelExist() )
		{
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_CHECK, CHK_UNLOAD_COMP) ;
			return;
		}

		ChangeStatus(STATUS_READY);

		GetDoc()->m_bAutoTune_RUN = FALSE;//  [6/11/2009 janghan]
		GetDoc()->m_nYaxis_Back = 99;//  [6/11/2009 janghan]
		GetDoc()->m_fAutoDistance_X =  0.; //  [6/15/2009 janghan]
		GetDoc()->m_fPCBmarkFind_X = 0.; //  [6/15/2009 janghan]
		GetDoc()->m_fMarkFind_X = 0.; //  [6/15/2009 janghan]
		GetDoc()->m_strAutoTune_Reason = "";//  [6/21/2009 janghan]

		GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]

		GetDoc()->m_bNonStop_Run = FALSE; //  [8/17/2009 janghan]

		GetMainFrame()->SwitchForm(FORM_AUTO);

		CString strLog;
		strLog.Format("[START]\tStart function.");
		GetDoc()->SetLog(strLog, LOG_EQ);
	}
}

void CMTApp::Stop()
{
	if(IsRunStatus())
	{
		CString strLog;
		strLog.Format("[STOP]\tStop function.");
		GetDoc()->SetLog(strLog, LOG_EQ);
		ChangeStatus(STATUS_STOP);
	}
	GetDoc()->m_bAutoTune_RUN = FALSE;//  [6/11/2009 janghan]
	GetDoc()->m_nYaxis_Back = 99;//  [6/11/2009 janghan]
	GetDoc()->m_fAutoDistance_X =  0.; //  [6/15/2009 janghan]
	GetDoc()->m_fPCBmarkFind_X = 0.; //  [6/15/2009 janghan]
	GetDoc()->m_fMarkFind_X = 0.; //  [6/15/2009 janghan]
	GetDoc()->m_strAutoTune_Reason = "";//  [6/21/2009 janghan]

	GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]

	GetDoc()->m_bNonStop_Run = FALSE; //  [8/17/2009 janghan]

}

void CMTApp::Reset()
{
	if (GetComm()->m_nAlarmState)
	{
		GetComm()->Alarm2LC(0);
	}
	if(m_RunStatus == STATUS_ERROR || m_RunStatus == STATUS_STOP)
	{
		GetDoc()->m_pMotion->ClearStatusAll();
		GetDeviceCtl()->SetAlarmBit(FALSE);
//		GetComm()->Alarm2LC(0);

		if( GetLineType() == REMOVE_TILTING_TYPE )
		{
			GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_ALARM_RST );
			GetDeviceCtl()->m_dToggleTimer = GetTickCount();
			GetMainFrame()->SetTimer(TIMER_BIT_TOGGLE, 1000, NULL);
		}
		///////////////////////////////////////////////////////////////////////////
		// 2011.01.12 hccho
		// 
		// 아래의 ChangeStatus() 내에서 중복되는 부분 있어 필요부분만 처리하는것으로 수정 함.
		//
		// 상태를 STOP으로 변경
		//ChangeStatus(STATUS_STOP);
		int nNewStatus = STATUS_STOP;

		SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_LABEL_CAPTION, (WPARAM)nNewStatus, 0);

		m_nOldStatus = m_RunStatus;
		m_RunStatus = nNewStatus;

		int nTowerLamp[3];
		nTowerLamp[0] = (int)(GetDoc()->strTowerLamp[nNewStatus].GetAt(2) -'0');
		nTowerLamp[1] = (int)(GetDoc()->strTowerLamp[nNewStatus].GetAt(7) -'0');
		nTowerLamp[2] = (int)(GetDoc()->strTowerLamp[nNewStatus].GetAt(12) -'0');
		int nTowerLampIO[3] = 
		{
			GetDoc()->GetPortMap()->oTowerLampRed, GetDoc()->GetPortMap()->oTowerLampYellow, GetDoc()->GetPortMap()->oTowerLampGreen
		};
		int nSwitchLampIO[3] = 
		{
			GetDoc()->GetPortMap()->oResetLamp, GetDoc()->GetPortMap()->oStopLamp, GetDoc()->GetPortMap()->oStartLamp
		};

		for(int i = 0; i < 3; i++)
		{
			if(nTowerLamp[i] == 0)
			{
				DioOutput(nTowerLampIO[i], TRUE);
				DioOutput(nSwitchLampIO[i], TRUE);
			}
			else if(nTowerLamp[i] == 2)
			{
				DioOutput(nTowerLampIO[i], FALSE);
				DioOutput(nSwitchLampIO[i], FALSE);
			}
		}

		//=============================================================
		// 2011.01.12 hccho
		// 이미 알람상태여서 STOP 되어 있을것이므로
		// 아래의 STOP 처리는 무효화 함.
		//
		//if(IsRunStatus() == FALSE)
		//{
		//	// Tilting 이동중에 Stop을 할 경우를 대비해 여기서 FALSE해 준다.
		//	GetDeviceCtl()->m_bMoveTilting = FALSE;
		//	// Align 하다가 끝났을 경우 대비해 Live 일지정지 해제
		//	GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
		//	GetDeviceCtl()->SetPCRun(FALSE);
		//	GetDeviceCtl()->EnableAreaSensor(FALSE);
		//	if(GetMainFrame()->m_nCurrentFormID != FORM_MAX)
		//		GetMainFrame()->m_pForm[GetMainFrame()->m_nCurrentFormID]->EnableWindow(TRUE);
		//}
		//============================================================
		if (GetComm()->m_iCurEQState == eFault)
		{
			GetComm()->EQState2LC(ePM);

			GetMainFrame()->m_strEngineerDownCode = "";
			GetMainFrame()->m_strEngineerDownText = "";

			GetComm()->ProcessState2LC(ePause, GetMainFrame()->m_strEngineerDownCode, 
				GetMainFrame()->m_strEngineerDownText) ;
		}
		///////////////////////////////////////////////////////////////////////////

		GetDoc()->m_bAutoTune_RUN = FALSE;//  [6/11/2009 janghan]
		GetDoc()->m_nYaxis_Back = 99;//  [6/11/2009 janghan]
		GetDoc()->m_fAutoDistance_X =  0.; //  [6/15/2009 janghan]
		GetDoc()->m_fPCBmarkFind_X = 0.; //  [6/15/2009 janghan]
		GetDoc()->m_fMarkFind_X = 0.; //  [6/15/2009 janghan]
		GetDoc()->m_strAutoTune_Reason = "";//  [6/21/2009 janghan]

		GetDoc()->m_bUsed_Tune = FALSE;

		CString strLog;
		strLog.Format("[RESET]\tReset function.");
		GetDoc()->SetLog(strLog, LOG_EQ);
	}
}

void CMTApp::ChangeStatus(int nNewStatus)
{

	if(m_RunStatus == nNewStatus) 
		return;
	else
	{
		m_nOldStatus = m_RunStatus;
		m_RunStatus = nNewStatus;

		int nTowerLamp[3];
		nTowerLamp[0] = (int)(GetDoc()->strTowerLamp[nNewStatus].GetAt(2) -'0');
		nTowerLamp[1] = (int)(GetDoc()->strTowerLamp[nNewStatus].GetAt(7) -'0');
		nTowerLamp[2] = (int)(GetDoc()->strTowerLamp[nNewStatus].GetAt(12) -'0');
		int nTowerLampIO[3] = {
			GetDoc()->GetPortMap()->oTowerLampRed, GetDoc()->GetPortMap()->oTowerLampYellow, GetDoc()->GetPortMap()->oTowerLampGreen};
		int nSwitchLampIO[3] = {
			GetDoc()->GetPortMap()->oResetLamp, GetDoc()->GetPortMap()->oStopLamp, GetDoc()->GetPortMap()->oStartLamp};

		for(int i = 0; i < 3; i++)
		{
			if(nTowerLamp[i] == 0)
			{
				DioOutput(nTowerLampIO[i], TRUE);
				DioOutput(nSwitchLampIO[i], TRUE);
			}
			else if(nTowerLamp[i] == 2)
			{
				DioOutput(nTowerLampIO[i], FALSE);
				DioOutput(nSwitchLampIO[i], FALSE);
			}
		}

		eqStatus = eNormal;
		int eProcess = ePNothing;
		switch (nNewStatus)
		{
		case STATUS_RUN:
			eqStatus = eNormal;
			eProcess = eExecute;
			break;
		case STATUS_READY:
			GetDeviceCtl()->m_bIgnoreInterlock = FALSE;	// 인터락 무시 해제

			eqStatus = eNormal;
			eProcess = eIdle;

			// 2009.06.15
			GetMelsec()->MelsecNetBitOn(LD_OUT_READY);
			GetMelsec()->MelsecNetBitOn(UNLD_OUT_READY);
			if( GetLineType() == REMOVE_TILTING_TYPE )
				GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_MANUAL);
			break;
		case STATUS_STOP:
			eqStatus = eNormal;
			eProcess = ePause;
			if( GetLineType() == REMOVE_TILTING_TYPE )
				GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_MANUAL);
			break;		
		case STATUS_INIT:
			eqStatus = eNormal;
			eProcess = eInit;
			break;		
		case STATUS_ERROR:
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{
				GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_MOVE_INSP );
				GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_MOVE_READY );
			}
			GetDeviceCtl()->m_bMoveTilting = FALSE;
			eqStatus = eFault;
			eProcess = ePause;
			break;		
		case STATUS_EMO:
			GetDeviceCtl()->m_bMoveTilting = FALSE;
			eqStatus = eFault;
			eProcess = ePause;
			break;		
		case STATUS_OPCALL:
			eqStatus = eNormal;
			eProcess = ePause;
			break;		
		}

		if(IsRunStatus() == FALSE)
		{
			// Tilting 이동중에 Stop을 할 경우를 대비해 여기서 FALSE해 준다.
			GetDeviceCtl()->m_bMoveTilting = FALSE;
			// Align 하다가 끝났을 경우 대비해 Live 일지정지 해제
			GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
		//	GetDeviceCtl()->SetPCRun(FALSE);
			GetDeviceCtl()->EnableAreaSensor(FALSE);
			if(GetMainFrame()->m_nCurrentFormID != FORM_MAX)
				GetMainFrame()->m_pForm[GetMainFrame()->m_nCurrentFormID]->EnableWindow(TRUE);
		}

		SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_LABEL_CAPTION, (WPARAM)nNewStatus, 0);
		//////////////////////////////////////////////////////////////////////////
		// LC 상태 보고
		if(eqStatus == eFault)
		{
			// Error중일 때는 MsgBox.cpp에서 보내니까 여기서는 안보낸다. [1/8/2011 OSC]
		}
		else if(nNewStatus == STATUS_STOP && 
			(m_nOldStatus == STATUS_READY || m_nOldStatus == STATUS_RUN))
		{
			// 진행중이었다가 에러 없이 STOP상태로 전환되면 Engineer 유실 코드 입력 창 표시 [1/8/2011 OSC]
			GetMainFrame()->m_bShowEngineerDownCodeDlg = TRUE;
			GetDoc()->SetLog("유실 코드 입력 창 띄움.",LOG_COMM);
			::SendMessage(GetMainFrame()->m_hWnd, WM_SHOW_ENGINEER_DOWN_CODE, NULL, NULL);

			GetComm()->EQState2LC(eqStatus) ;
			GetComm()->ProcessState2LC(eProcess, GetMainFrame()->m_strEngineerDownCode, 
				GetMainFrame()->m_strEngineerDownText) ;
		}
		else
		{
			GetMainFrame()->m_strEngineerDownCode = "";
			GetMainFrame()->m_strEngineerDownText = "";

			GetComm()->EQState2LC(eqStatus) ;
			GetComm()->ProcessState2LC(eProcess, GetMainFrame()->m_strEngineerDownCode, 
				GetMainFrame()->m_strEngineerDownText) ;
		}
		GetMainFrame()->m_strEngineerDownCode = "";
		GetMainFrame()->m_strEngineerDownText = "";
		GetMainFrame()->m_bShowEngineerDownCodeDlg = FALSE;
		//////////////////////////////////////////////////////////////////////////	
	}
}


void CMTApp::ChangeRunMode( RunMode newMode )
{
	if(IsRunStatus() == FALSE)
	{
		// 검사를 안할 때는 로그인 안한걸로 간주 [9/16/2010 OSC]
		if(m_RunMode != MODE_VISION_SKIP && newMode == MODE_VISION_SKIP)
		{
			GetComm()->LoginInfo2LC(eLogIn, "00000000", "00000000");
		}
		if(m_RunMode == MODE_VISION_SKIP && newMode != MODE_VISION_SKIP)
		{
			GetComm()->LoginInfo2LC(eLogIn, GetDoc()->m_pEqMainSeq->m_PGInfo.m_sOPName, 
				GetDoc()->m_pEqMainSeq->m_PGInfo.m_sOPID);
		}

		m_RunMode = newMode;
	}
}

BOOL CMTApp::IsRunStatus()
{
	if(m_RunStatus == STATUS_READY || m_RunStatus == STATUS_RUN)
		return TRUE;
	else
		return FALSE;
}