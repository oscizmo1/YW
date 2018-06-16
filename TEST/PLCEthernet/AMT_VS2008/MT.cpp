// MT.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMTApp ����

CFormCOMM* gl_pComm = NULL;
CMTDoc* gl_pDoc = NULL;
CPLCComm* gl_pPLCComm = NULL;

CMTApp::CMTApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
	m_RunStatus = STATUS_ALL;
	m_RunMode = MODE_NORMAL;
	m_bInspectionStart = FALSE;
	m_bInspectionEnd = TRUE;
	m_bInspectionResult = TRUE;
}


// ������ CMTApp ��ü�Դϴ�.

CMTApp theApp;

// CMTApp �ʱ�ȭ

BOOL CMTApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.

	// �ߺ����� ����
	HANDLE hEvent; 
    hEvent=CreateEvent(NULL,FALSE,TRUE,AfxGetAppName()); 
    if(GetLastError()==ERROR_ALREADY_EXISTS)
    { 
		AfxMessageBox("�̹� �������Դϴ�!!!");
        return FALSE; 
    }

	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("YWDSP"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	SetTypes();
	SetDefaultPath();

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	// ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMTDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CMTView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ����ٿ� ������ ����� ����ġ�մϴ�. ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
// 	((CMainFrame*)m_pMainWnd)->SetImageGrapView();
	GetDoc()->Initial();



	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	// SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	return TRUE;
}

// CMTApp �޽��� ó����


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
	// ��� �Լ� �߿��� ���� ���� ȣ���Ѵ�.
	// Type load�� �׿� ���� Ŭ���� ������ �̷������ ������ 
	// �� ��� �� ���� ���� ����Ǿ�� �Ѵ�.

	// Notebook���� �����ϴ��� ��񿡼� �����ϴ���...
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

	// D�� ���ʹ� Tilting ��� PLC�� �ΰ�ȴ�. [5/30/2009 OSC]
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
	// ���ǳ���â �Է� �������� START ���� [1/8/2011 OSC]
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
		// Moving���� Axis Check
		if(GetDeviceCtl()->m_bMoveTilting || GetDoc()->m_pEqMainSeq->m_nSeqContactNo != SEQ_CONTACT_CHK_REQUEST
			|| GetDeviceCtl()->m_nMoveStep != MS_CHK_EQUIPMENT)
		{
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_CHECK, CHK_MOTION_READY);
			return;
		}

		// Operator level�� ��ȯ
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
		// �Ʒ��� ChangeStatus() ������ �ߺ��Ǵ� �κ� �־� �ʿ�κи� ó���ϴ°����� ���� ��.
		//
		// ���¸� STOP���� ����
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
		// �̹� �˶����¿��� STOP �Ǿ� �������̹Ƿ�
		// �Ʒ��� STOP ó���� ��ȿȭ ��.
		//
		//if(IsRunStatus() == FALSE)
		//{
		//	// Tilting �̵��߿� Stop�� �� ��츦 ����� ���⼭ FALSE�� �ش�.
		//	GetDeviceCtl()->m_bMoveTilting = FALSE;
		//	// Align �ϴٰ� ������ ��� ����� Live �������� ����
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
			GetDeviceCtl()->m_bIgnoreInterlock = FALSE;	// ���Ͷ� ���� ����

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
			// Tilting �̵��߿� Stop�� �� ��츦 ����� ���⼭ FALSE�� �ش�.
			GetDeviceCtl()->m_bMoveTilting = FALSE;
			// Align �ϴٰ� ������ ��� ����� Live �������� ����
			GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
		//	GetDeviceCtl()->SetPCRun(FALSE);
			GetDeviceCtl()->EnableAreaSensor(FALSE);
			if(GetMainFrame()->m_nCurrentFormID != FORM_MAX)
				GetMainFrame()->m_pForm[GetMainFrame()->m_nCurrentFormID]->EnableWindow(TRUE);
		}

		SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_LABEL_CAPTION, (WPARAM)nNewStatus, 0);
		//////////////////////////////////////////////////////////////////////////
		// LC ���� ����
		if(eqStatus == eFault)
		{
			// Error���� ���� MsgBox.cpp���� �����ϱ� ���⼭�� �Ⱥ�����. [1/8/2011 OSC]
		}
		else if(nNewStatus == STATUS_STOP && 
			(m_nOldStatus == STATUS_READY || m_nOldStatus == STATUS_RUN))
		{
			// �������̾��ٰ� ���� ���� STOP���·� ��ȯ�Ǹ� Engineer ���� �ڵ� �Է� â ǥ�� [1/8/2011 OSC]
			GetMainFrame()->m_bShowEngineerDownCodeDlg = TRUE;
			GetDoc()->SetLog("���� �ڵ� �Է� â ���.",LOG_COMM);
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
		// �˻縦 ���� ���� �α��� ���Ѱɷ� ���� [9/16/2010 OSC]
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