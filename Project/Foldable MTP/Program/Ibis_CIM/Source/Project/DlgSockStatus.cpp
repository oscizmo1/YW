// DlgSockStatus.cpp : implementation file
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "DlgSockStatus.h"

#include "GxSocket\GxSockClient.h"

// CDlgSockStatus dialog

IMPLEMENT_DYNAMIC(CDlgSockStatus, CDialog)

CDlgSockStatus::CDlgSockStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSockStatus::IDD, pParent)
{
	m_pServerSocket = NULL;
	m_pGxServer = NULL;							// 수신을 기다리는 Server socket pointer
	m_bOnlyOnce	= TRUE;

}

CDlgSockStatus::~CDlgSockStatus()
{
	if ( m_pGxServer ) 
	{
		delete m_pGxServer;
	}
}

void CDlgSockStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSockStatus, CDialog)
	ON_MESSAGE(WM_PACKET_RECEIVE, OnPacketReceive)
	ON_MESSAGE(WM_RE_CONNECT, OnReConnect)
	ON_MESSAGE(WM_SOCKET_CLOSE, OnSocketClose)
	ON_MESSAGE(WM_SOCKET_CONNECT, OnSocketConnect)

	ON_MESSAGE(GxMSG_COMM_EQ_CONNECT,		OnTCPConnect)				// 운영PC와 연결 
	ON_MESSAGE(GxMSG_COMM_EQ_DISCONNECT,	OnTCPDisconnect)			// 운영PC와 연결 끊김
	ON_MESSAGE(GxMSG_COMM_EQ_RECEIVE,		OnTCPReceive)				// 운영PC에서 데이터 수신


	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_MSG, OnUserMessage)
END_MESSAGE_MAP()


// CDlgSockStatus message handlers

BOOL CDlgSockStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	if ( m_bOnlyOnce )
	{
		m_bOnlyOnce = FALSE;

		SetGxSocketListen();
		// CreateServerSocket();
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSockStatus::CreateServerSocket()
{
	m_pServerSocket	= new CDataSocket;
	m_pServerSocket->SetServer(TRUE);
	m_pServerSocket->SetSocketName(SOCKET_NAME_SERVER);

	CString strIP;
	strIP = CSettingParm::cMachineIP;
	int nPort = CSettingParm::iMachinePort;
	m_pServerSocket->SetPort(nPort);

	ClientSocketName Socketname;
	Socketname.Name = SOCKET_NAME_CLIENT_EQ;
	Socketname.IP = strIP;

	m_pServerSocket->listClientName.AddTail(Socketname);
	m_pServerSocket->SetReciveHwnd(this->m_hWnd);

	if(m_pServerSocket->Create(m_pServerSocket->GetPort()))
	{
		m_pServerSocket->SetCreated(TRUE);
		m_pServerSocket->Listen();
	}
}

void CDlgSockStatus::DeleteServerSocket()
{
	if(m_pServerSocket!=NULL)	//2013.03.21.
	{
		//m_pServerSocket->OnClose();
		m_pServerSocket->DeleteClient();
		m_pServerSocket->Close();
		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}
}

LRESULT CDlgSockStatus::OnPacketReceive( WPARAM wParam, LPARAM lParam )
{
	int nRtn = 0;
	CDataSocket* pSocket = (CDataSocket *)wParam;
	int nMsgLen = (int)lParam;
	
	if(pSocket->GetSocketName() == SOCKET_NAME_CLIENT_EQ)
	{
		tag_Message* pData = (tag_Message *)lParam;
		
		if (pData->sMsgClass == CLASS_CONTROL)
		{
			RcvMsgFromAftClient(pData);
		}
		else if (pData->sMsgClass == CLASS_MCC)
		{
			RcvMsgFromMccClient(pData);
		}
		
	}
	return nRtn;
}

LRESULT CDlgSockStatus::OnReConnect( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;
	BOOL bCreate = (BOOL)lParam;

	if(bCreate)
	{
	
 	}

	return 0;
}

LRESULT CDlgSockStatus::OnSocketClose( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;

	// Main Frame에게 운영PC와의 접속이 끊어졌음을 알린다.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_EQ_DISCONNECT, 0, 0);


	return 0;
}

LRESULT CDlgSockStatus::OnSocketConnect( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;

	if(pSocket->IsConnected())
	{
		// Main Frame에게 운영PC와의 접속을 알린다.
		::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_EQ_CONNECT, 0, 0);
	}


	return 0;
}



// Socket 통신 수신처리
BOOL CDlgSockStatus::SetGxSocketListen()
{
	BOOL bRet = TRUE;

	if ( !m_pGxServer )
	{
		m_pGxServer = new CGxSockServer( 0 );		// 신호기 PC 연결, Server
		m_pGxServer->m_nID		= 0;
		m_pGxServer->m_sName	= _T("EQ");	
		m_pGxServer->SetMsgInfo(this, GxMSG_COMM_EQ_CONNECT, GxMSG_COMM_EQ_DISCONNECT, GxMSG_COMM_EQ_RECEIVE);
		//m_pGxServer->SetCallbackFunction(pConnect, pClose, m_pFuncReceive);

		m_pGxServer->SetMultiConnect(FALSE);	// 다중 접속 허용 여부
		m_pGxServer->SetMultiPeer(FALSE);		// 동일 IP 다중 접속 여부
		m_pGxServer->SetPort( 6000 );				// Listen할 포트 지정
		m_pGxServer->SetPacketInfo(eGxCOMM_PKT_COUNT, NULL, 0, sizeof( tag_Message ) );	// 수신길이로 패킷 지정

		bRet = m_pGxServer->Listen();				// 수신을 시작한다.
	}

	return bRet;
}


// 운영 PC와 연결
LRESULT CDlgSockStatus::OnTCPConnect(WPARAM wParam, LPARAM lParam)
{
	// Main Frame에게 운영PC와의 접속을 알린다.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_EQ_CONNECT, 0, 0);

	return 0;
}


// 운영 PC와의 연결이 끊어짐
LRESULT CDlgSockStatus::OnTCPDisconnect(WPARAM wParam, LPARAM lParam)
{
	// 상부 Title과 CIM 화면에게 연결이 끊어졌음을 알려준다.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_EQ_DISCONNECT, 0, 0);

	return 0;
}

// 운영 PC에서 데이터 수신
LRESULT CDlgSockStatus::OnTCPReceive(WPARAM wParam, LPARAM lParam)
{
	//int nRtn = 0;
	//CDataSocket* pSocket = (CDataSocket *)wParam;
	//int nMsgLen = (int)lParam;
	//
	//if(pSocket->GetSocketName() == SOCKET_NAME_CLIENT_EQ)
	//{
	//	tag_Message* pData = (tag_Message *)lParam;
	//	
	//	if (pData->sMsgClass == CLASS_CONTROL)
	//	{
	//		RcvMsgFromAftClient(pData);
	//	}
	//	else if (pData->sMsgClass == CLASS_MCC)
	//	{
	//		RcvMsgFromMccClient(pData);
	//	}
	//	
	//}
	//return nRtn;

	CGxSockClient* pClient = (CGxSockClient*)wParam;

	int nLen = pClient->IsPacket();									// 수신된 길이
	int nSize = sizeof( tag_Message );
	int nRead;
	tag_Message* pBlock;												// 구조체 데이터 포인터
// 	GetLogCtrl()->AddBuf(LOG_COMM, "Recv Length %d / %d",   nLen, nSize);


	// 수신된 패킷이 존재하는 경우 모두 처리해 준다.
	while ( nLen >= nSize )
	{
		BYTE* byBuff = new BYTE[ nSize ];							// 데이터를 가져 올 버퍼 생성

		nRead = pClient->GetPacket( byBuff, nSize );				// 데이터를 받아온다.

		// 읽어온 데이터와 할당된 Block의 Size가 서로 동일해야 처리가 가능한다.
		if ( nRead == nSize )
		{
			pBlock = (tag_Message*)byBuff;							// Typecasting

			RcvMsgFromAftClient( pBlock );							// 수신 데이터 처리

			//m_listRcvData.AddTail( pBlock );								// 수신된 데이터 보관 구조체 포인터 list
			//m_log.Write(_T(",<-,%d,Class:%d Type:%d"), nRead, pBlock->sMsgClass, pBlock->sMsgType);		// Log 기록
		}

		nLen = pClient->IsPacket();									// 뒤에도 수신된 데이터가 존재하는가 ?
// 		GetLogCtrl()->AddBuf(LOG_COMM, "Recv Length %d / %d",   nLen, nSize);
		//m_evtDataReceive.Set();											// 데이터가 수신되었음을 알린다.
		delete[] byBuff;														// 사용을 마친 메모리 반환
	}//of while
	GetLogCtrl()->AddBuf(LOG_COMM, "Remain Length %d / %d",   nLen, nSize);

	return 0;
}

// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==




void CDlgSockStatus::OnClose()
{

	CDialog::OnClose();
}

void CDlgSockStatus::OnDestroy()
{
	CDialog::OnDestroy();

	if ( m_pGxServer )
	{
		if ( m_pGxServer->GetClientCount() > 0 )
		{
			m_pGxServer->Close();
		}
	}

	// TODO: Add your message handler code here
	// DeleteServerSocket();
}

BOOL CDlgSockStatus::RcvMsgFromAftClient(tag_Message* pData)
{

	GetLogCtrl()->AddBuf(LOG_COMM, "[%s]",   g_strMyMessageType[pData->sMsgType]);
	
	int nJig, nCellPos/*, nUnit*/;

	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	tag_CellData* pCellData;

	switch (pData->sMsgType)
	{
	case EQUIP_SET_CONTROL :
		pMainFrm->m_pViewCIM->SetControlState();
		break;

	case EQUIP_SET_DATAPC_VERSION :
		{
			int nLen = min(6, strlen(pData->cMsgData[0]));
			memset(g_pData->m_cSoftRev, NULL, 7);
			memcpy(g_pData->m_cSoftRev, pData->cMsgData[0], nLen);
			pMainFrm->m_pViewCIM->SetDataPCVersion();
		}
		break;


	// 2016-01-20, 지정 Cell Data의 내용을 Clear 하는 요청
	case EQUIP_SET_CELL_DATA_CLEAR_REQUEST :			// 지정 Cell Data의 내용을 Clear
		// 이제 Cell은 GaonNuri에서 갖고있지 않으니 필요없다
		GetLogCtrl()->AddBuf(LOG_COMM, "JIG(%d), CELLPOS(%d), INNERID(%s) Cell Data Clear request",  pData->sUnit+1, pData->sCellPos+1, pData->cMsgData[0]);
// 		nJig		= pData->sUnit;
// 		nCellPos	= pData->sCellPos;
// 		pCellData	= &g_pData->m_Jig[nJig].tCellData[nCellPos];
// 		pCellData->Clear();														// 이전의 내용을 삭제한다.
// 
// 		// 2016-02-03, jhLee, FDC INSPECTION 항목의 취득을 위해 Inner ID를 지정해둔다. 아직 Cell ID는 알 수 없다.
// 		strcpy_s(pCellData->m_cInnerID,	pData->cMsgData[0]);	// Inner ID 대입

		break;


	// 2015-08-12, jhLee, Cell Tracking Information 기능 추가, 사고LOT (FAB HOLD) 처러 및 TMD Validation 처리를 위한 정보 취득
	case EQUIP_SET_CELL_INFORMATION_REQUEST:
		GetLogCtrl()->AddBuf(LOG_COMM, "INNERID(%s), CELLID(%s), Cell Tracking Info request",  pData->cMsgData[0], pData->cCellID);
		g_pData->m_CellInformation.AddCellID(pData->cCellID);

		pMainFrm->m_pViewCIM->EventCellInfoRequest();


		break;


	// 2016-01-04, jhLee, POCH처리를 위한 Cell Lot Information 기능 추가
	case EQUIP_SET_CELL_LOT_INFORMATION_REQUEST:
		GetLogCtrl()->AddBuf(LOG_COMM, "CELLID(%s), OptionCode(%s), Cell Lot Info request",  pData->cCellID, pData->cMsgData[0]);

		g_pData->m_CellLotInformation.AddCellID(pData->cCellID);
		g_pData->m_CellLotInformation.SetOptionCode(pData->cMsgData[0]);	// OPTIONCODE

		pMainFrm->m_pViewCIM->EventCellLotInfoRequest(pData->cMsgData[0]);

		break;

	case EQUIP_SET_CELL_PROCESS_START_LOAD :
		GetLogCtrl()->AddBuf(LOG_COMM, "JIG(%d), CELLPOS(%d), INNERID(%s), CELLID(%s), Track-In",  pData->sUnit+1, pData->sCellPos+1, pData->cMsgData[0], pData->cCellID);
// 		nJig			= pData->sUnit;
// 		nCellPos		= pData->sCellPos;
		pCellData		= g_pData->m_CellTrackIn.GetCellDataPtr();
		pCellData->Clear();	
		
		strcpy_s(pCellData->m_cInnerID,	pData->cMsgData[0]);
		strcpy_s(pCellData->m_cCellID,					pData->cCellID);
		// Step ID
		strcpy_s(pCellData->m_cStepID, pData->cMsgData[1]);
		// Product ID
		strcpy_s(pCellData->m_cProductID, pData->cMsgData[2]);

		strcpy_s(pCellData->m_cReaderResultCode,		pData->cMsgData[6]);
		strcpy_s(pCellData->m_cReaderID, pData->cMsgData[7]);
		g_pData->m_CellTrackIn.AddCellID(pData->cCellID);

		pMainFrm->m_pViewCIM->EventCellProcessStart();
		break;

	case EQUIP_SET_CELL_ENDINS:
		GetLogCtrl()->AddBuf(LOG_COMM, "INNERID(%s), CELLID(%s), JUDGE(%s), REASONCODE(%s), C-JUDGE(%s), D-JUDGE(%s)",  pData->cMsgData[0], pData->cCellID, pData->cMsgData[3], pData->cMsgData[4], pData->cMsgData[5], pData->cMsgData[6]);
// 		nJig = pData->sUnit;
// 		nCellPos = pData->sCellPos;
// 		pCellData = &g_pData->m_Jig[nJig].tCellData[nCellPos];
		pCellData = &g_pData->m_CellDataInspectionEnd;

		strcpy_s(pCellData->m_cCellID,					pData->cCellID);

		strcpy_s(pCellData->m_cOperatorID1, g_pData->m_cOperatorID1);
		strcpy_s(pCellData->m_cOperatorID2, g_pData->m_cOperatorID2);
		strcpy_s(pCellData->m_cJudge, pData->cMsgData[3]);

		strcpy_s(pCellData->m_cReasonCode, pData->cMsgData[4]);
		strcpy_s(pCellData->m_cJudgeOper1, pData->cMsgData[5]);
		strcpy_s(pCellData->m_cJudgeOper2, pData->cMsgData[6]);
		strcpy_s(pCellData->m_cReasonCodeOper1, pData->cMsgData[7]);
		strcpy_s(pCellData->m_cReasonCodeOper2, pData->cMsgData[8]);
		strcpy_s(pCellData->m_cDescription, pData->cMsgData[9]);

		strcpy_s(pCellData->m_cProcessFlag,	pData->cMsgData[0]);				// 공정검사 진행 여부 지정
		pMainFrm->m_pViewCIM->EventInspectionResultReport();


		break;

	case EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD:
		GetLogCtrl()->AddBuf(LOG_COMM, "JIG(%d), CELLPOS(%d), INNERID(%s), CELLID(%s), Track-Out",  pData->sUnit+1, pData->sCellPos+1, pData->cMsgData[0], pData->cCellID);
		nJig = pData->sUnit;
		nCellPos = pData->sCellPos;
		pCellData = &g_pData->m_CellDataTrackOut;

		strcpy_s(pCellData->m_cCellID,					pData->cCellID);
		
		// TrackOut할 때 Judge 번복 가능 [9/27/2017 OSC]
		memcpy(pCellData->m_cJudge, pData->cMsgData[1], sizeof(pCellData->m_cJudge));
		strcpy_s(pCellData->m_cDescription, pData->cMsgData[2]);

		// Defect 좌표에대한 값을 취득한다.
		strcpy_s(pCellData->m_cDefectPos, pData->cMsgData[3]);

		// MTP_WRITE 결과
		strcpy_s(pCellData->m_cMTPWrite, pData->cMsgData[4]);

		// Product ID
		strcpy_s(pCellData->m_cProductID, pData->cMsgData[5]);

		// Step ID
		strcpy_s(pCellData->m_cStepID, pData->cMsgData[6]);

		// Defect Code
		strcpy_s(pCellData->m_cReasonCode, pData->cMsgData[7]);

		pMainFrm->m_pViewCIM->EventCellProcessComplete(nJig, nCellPos);
		break;
	
	case EQUIP_SET_ALARM:
		g_pData->m_tAlarm.Clear();
		strcpy_s(g_pData->m_tAlarm.m_cALST, pData->cMsgData[0]);
		strcpy_s(g_pData->m_tAlarm.m_cALCD, pData->cMsgData[1]);
		strcpy_s(g_pData->m_tAlarm.m_cALID, pData->cMsgData[2]);
		strcpy_s(g_pData->m_tAlarm.m_cALTX, pData->cMsgData[3]);

		if (strcmp(g_pData->m_tAlarm.m_cALST, ALST_SET)==0)
		{
			g_pData->m_tAlarmInsert.Clear();
			g_pData->m_tAlarmInsert.bInsert = TRUE;
			g_pData->m_tAlarmInsert.timeSet = CTime::GetCurrentTime();
			GetLocalTime(&g_pData->m_tAlarmInsert.m_systmTimeSet);	//MiliSecond단위로 계산하기위해.

			g_pData->m_tCurAlarmList[g_pData->m_nCurAlarmListCnt] = g_pData->m_tAlarm;
			g_pData->m_nCurAlarmListCnt++;
			pMainFrm->m_pViewCIM->AlarmReportSend();
		}
		else if (strcmp(g_pData->m_tAlarm.m_cALST, ALST_CLEAR)==0)
		{
			g_pData->m_tAlarmInsert.timeReset = CTime::GetCurrentTime();
			strcpy_s(g_pData->m_tAlarmInsert.m_cALCD, pData->cMsgData[1]);
			strcpy_s(g_pData->m_tAlarmInsert.m_cALID, pData->cMsgData[2]);
			strcpy_s(g_pData->m_tAlarmInsert.m_cALTX, pData->cMsgData[3]);

			for (int i=0; i<g_pData->m_nCurAlarmListCnt; i++)
			{
				strcpy_s(g_pData->m_tAlarm.m_cALCD, g_pData->m_tCurAlarmList[i].m_cALCD);
				strcpy_s(g_pData->m_tAlarm.m_cALID, g_pData->m_tCurAlarmList[i].m_cALID);
				strcpy_s(g_pData->m_tAlarm.m_cALTX, g_pData->m_tCurAlarmList[i].m_cALTX);
				pMainFrm->m_pViewCIM->AlarmReportSend();
			}

			g_pData->m_nCurAlarmListCnt = 0;
			for (int i=0; i<MAX_ALARM; i++)
			{
				g_pData->m_tCurAlarmList[i].Clear();
			}
		}
		

		break;

	case EQUIP_SET_EQUIPMENT_STATUS_CHANGE:
		if (pData->sEqst == E_EQST_MATCH_EXECUTING && g_pData->m_tAlarmInsert.bInsert)
		{
			g_pData->m_tAlarmInsert.timeRun = CTime::GetCurrentTime();
			GetLocalTime(&g_pData->m_tAlarmInsert.m_systmTimeRun);	//MiliSecond단위로 계산하기위해.


			// 2016-06-02, jhLee, 삭제
			//비가동 정합성 추가Log
			// GetLocalTime(&g_pData->m_tLossTime.m_systmTimeRun);	
			// pMainFrm->m_pViewCIM->WriteDownLog();
			// pMainFrm->m_pViewCIM->WriteStateChangeLog();
			//
			// g_pData->m_tLossTime.Clear();
			g_pData->m_tAlarmInsert.Clear();
		}

		// GetLocalTime(&g_pData->m_tLossTime.m_systmTimeStop);	

		g_pData->m_sOldEqstMatch = g_pData->m_sEqstMatch;
		g_pData->m_sEqstMatch = pData->sEqst;

		// 인터락이면 기존  추가한걸 활성화시킨다
		if(g_pData->m_sEqstMatch == E_EQST_MATCH_INTERLOCK)
		{
			CString strInterlockType = pData->cMsgData[2];
			int nType;
			if(strInterlockType == EFST_STEP)
				nType = 2;
			else if(strInterlockType == EFST_TRANSFER)
				nType = 11;
			else if(strInterlockType == EFST_LOADING)
				nType = 12;
			else
				nType = 2;
			for (int i=0; i<MAX_INTERLOCK_MES ; i++)
			{
				if(g_pData->m_tInterlockMES[i].m_cInterlockType == nType)
				{
					if(g_pData->m_tInterlockMES[i].m_cInterlockState == INTERLOCK_ADD)
						g_pData->m_tInterlockMES[i].m_cInterlockState = INTERLOCK_HAPPEN;
				}
			}

		}
		pMainFrm->m_pViewCIM->EventEquipmentStatusChange();
		break;

	case EQUIP_SET_PPID_CREATE:
		strcpy_s(g_pData->m_cFixPPID, pData->cMsgData[7]);
		strcpy_s(g_pData->m_cFixPPIDMode, PPID_MODE_CREATE);
		pMainFrm->m_pViewCIM->EventPPIDCreate();
		pMainFrm->m_pViewRMS->EqModelSet(g_pData->m_cFixPPID, g_pData->m_cFixPPIDMode);
		break;

	case EQUIP_SET_PPID_MODIFY:
		strcpy_s(g_pData->m_cFixPPID, pData->cMsgData[7]);
		strcpy_s(g_pData->m_cFixPPIDMode, PPID_MODE_MODIFY);
		pMainFrm->m_pViewCIM->EventPPIDModify();
		pMainFrm->m_pViewRMS->EqModelSet(g_pData->m_cFixPPID, g_pData->m_cFixPPIDMode);
		break;

	case EQUIP_SET_PPID_DELETE:
		strcpy_s(g_pData->m_cFixPPID, pData->cMsgData[7]);
		strcpy_s(g_pData->m_cFixPPIDMode, PPID_MODE_DELETE);
		pMainFrm->m_pViewCIM->EventPPIDDelete();
		pMainFrm->m_pViewRMS->EqModelSet(g_pData->m_cFixPPID, g_pData->m_cFixPPIDMode);
		break;

	case EQUIP_SET_PPID_CHANGE:
		strcpy_s(g_pData->m_cOldPPID, pData->cMsgData[7]);
		strcpy_s(g_pData->m_cPPID, pData->cMsgData[8]);
		strcpy_s(g_pData->m_cPPIDST, PPIDST_CHANGED);

		pMainFrm->m_pViewCIM->EventPPIDChange();
		pMainFrm->m_pViewRMS->EqModelChange(g_pData->m_cPPID);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// 기본적인 설비 정보를 표시한다.
		break;

	case EQUIP_SET_PPID_OPERATION:
		strcpy_s(g_pData->m_cPPID, pData->cMsgData[8]);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// 기본적인 설비 정보를 표시한다.
		break;

	case EQUIP_SET_INS1_USER_LOGIN:
		strcpy_s(g_pData->m_cOperatorID1, pData->cMsgData[5]);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// 기본적인 설비 정보를 표시한다.

		break;

	case EQUIP_SET_INS1_USER_LOGOUT:
		strcpy_s(g_pData->m_cOperatorID1, g_pData->m_cEQPID);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// 기본적인 설비 정보를 표시한다.
		break;

	case EQUIP_SET_INS2_USER_LOGIN:
		strcpy_s(g_pData->m_cOperatorID2, pData->cMsgData[5]);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// 기본적인 설비 정보를 표시한다.
		break;

	case EQUIP_SET_INS2_USER_LOGOUT:
		strcpy_s(g_pData->m_cOperatorID2, g_pData->m_cEQPID);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// 기본적인 설비 정보를 표시한다.
		break;

	case EQUIP_SET_UV_READING_MCR:
	
		break;

	case EQUIP_SET_ALIGN_MARK_FIND:
	
		break;

	case EQUIP_SET_LINE_NO:
		strcpy_s(g_pData->m_cLineID, pData->cMsgData[0]);
		break;

	case EQUIP_SET_CELL_EXIST:
		if (strcmp(pData->cMsgData[0], EQST_RUN_IS_CELL)==0)
		{
			g_pData->m_sEqstMatch = E_EQST_MATCH_CELL_EXIST;
			pMainFrm->m_pViewCIM->EventEquipmentStatusChange();
		}
		else
		{
			g_pData->m_sEqstMatch = E_EQST_MATCH_CELL_NOT_EXIST;
			pMainFrm->m_pViewCIM->EventEquipmentStatusChange();
		}
		break;

	case EQUIP_SET_OPCALL_EVENT:
		if (g_pData->m_nOPCallCount>0)
			pMainFrm->m_pViewCIM->EventOPCall();
		break;
	
	case EQUIP_SET_INTERLOCK_EVENT:
		if (g_pData->GetInterlockCount()>0)
		{
			// 9번 항목으로 보낸 Interlock type을 받는다 - LSH171128
			strcpy_s(g_pData->m_strInterlockClearType, pData->cMsgData[9]);
			
			pMainFrm->m_pViewCIM->EventInterlock();
		}
		break;

	case EQUIP_SET_LOSS_CODE:	//20141223 sms
		g_pData->m_tLossCode.Clear();
		strcpy_s(g_pData->m_tLossCode.m_cLossCode, pData->cMsgData[0]);
		strcpy_s(g_pData->m_tLossCode.m_cLossDesc, pData->cMsgData[1]);

		pMainFrm->m_pViewCIM->EventLossCode();
		break;

	case EQUIP_SET_USER_LOGIN_REQUEST:		//GT.PARK - Log IN 요청
		strcpy_s(g_pData->m_cOperatorID1, pData->cMsgData[5]);
		strcpy_s(g_pData->m_cPassword, pData->cMsgData[6]);
		strcpy_s(g_pData->m_cOptionInfo, "LOGIN");
		strcpy_s(g_pData->m_cComment, "");

		pMainFrm->m_pViewCIM->EventLogIn_Out();
		break;

	case EQUIP_SET_USER_LOGOUT_REQUEST:		//GT.PARK - Log OUT 요청
		strcpy_s(g_pData->m_cOperatorID1, pData->cMsgData[5]);
		strcpy_s(g_pData->m_cPassword, pData->cMsgData[6]);
		strcpy_s(g_pData->m_cOptionInfo, "LOGOUT");
		strcpy_s(g_pData->m_cComment, "");

		pMainFrm->m_pViewCIM->EventLogIn_Out();
		break;


	case EQUIP_SET_EQUIP_FUNCTION_CHANGE_REQ_ACK :
		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s] ACK : %d"), 
			_T("EQUIP_SET_EQUIP_FUNCTION_CHANGE_ACK"), pData->sEqst);

		pMainFrm->m_pViewCIM->EventEquipFunctionChangeAck(pData->sEqst);	
		break;

	case EQUIP_SET_EQUIP_FUNCTION_REPORT :
		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s] EFST1 %s\nEFST2 %s\nEFST3 %s\nEFST4 %s\nEFST5 %s\nEFST6 %s\nEFST7 %s\nEFST8 %s\nEFST9 %s\nEFST10 %s"), 
			_T("EQUIP_SET_EQUIP_FUNCTION_REPORT"), pData->cMsgData[0],
			pData->cMsgData[1], pData->cMsgData[2], pData->cMsgData[3],
			pData->cMsgData[3], pData->cMsgData[5], pData->cMsgData[6],
			pData->cMsgData[7], pData->cMsgData[8], pData->cMsgData[9]);

		pMainFrm->m_pViewCIM->EventEquipFunctionUpdate(pData);	
		break;

	case EQUIP_SET_EQUIP_FUNCTION_CHANGE :
		{
			GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s] EFID : %s, EFNAME : %s, EFST : %s, EFST_Old : %s, BYWHO : %s"), 
				_T("EQUIP_SET_EQUIP_FUNCTION_CHANGE"), pData->cMsgData[0], pData->cMsgData[1],
				pData->cMsgData[2], pData->cMsgData[3], pData->cMsgData[4]);

			// Change list에 값을 입력한다 - LSH171130
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[_tstoi(pData->cMsgData[4])/* Index */].EFID = _tstoi(pData->cMsgData[0]/* EFID */);
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[_tstoi(pData->cMsgData[4])/* Index */].EFNAME = pData->cMsgData[1]/* EFNAME */;
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[_tstoi(pData->cMsgData[4])/* Index */].EFST = pData->cMsgData[2]/* EFST */;
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[_tstoi(pData->cMsgData[4])/* Index */].EFST_Old = pData->cMsgData[3]/* OLD_EFST */;

			// Gaonnuri가 갖고있는 List의 상태를 Update한다 - LSH171130
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionList[_tstoi(pData->cMsgData[0]) - 1/* Index */].EFNAME = pData->cMsgData[1]/* EFNAME */;
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionList[_tstoi(pData->cMsgData[0]) - 1/* Index */].EFST = pData->cMsgData[2]/* EFST */;
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionList[_tstoi(pData->cMsgData[0]) - 1/* Index */].EFST_Old = pData->cMsgData[3]/* OLD_EFST */;

			// List의 마지막 부분인지 확인 - LSH171130
			if(_tstoi(pData->cMsgData[5])/* LastIndexFlag */)
			{
				// Function change실행 - LSH171130
				pMainFrm->m_pViewCIM->EventEquipFunctionChange(pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer, _tstoi(pData->cMsgData[4]) +	 1/* LastIndex */, pData->cMsgData[6]/* ByWho */);
				
				// List정보를 비운다 - LSH171130
				for(int itor=0 ; itor<MAX_EQUIP_FUNCTION_LIST ; itor++)
					pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[itor].Clear();
			}
		}
		break;

	case EQUIP_SET_INTERLOCK_REQ_ACK :
		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s] RCMD : %s, ACK : %d"), 
			_T("EQUIP_SET_INTERLOCK_REQ_ACK"), pData->cMsgData[0], pData->sEqst);

		pMainFrm->m_pViewCIM->EventInterlockRequestAck(pData->cMsgData[0], pData->sEqst ? TRUE:FALSE);	
		break;

	case EQUIP_SET_UNIT_STATUS_REPORT :
		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s]"), 
			_T("EQUIP_SET_UNIT_STATUS_REPORT"));

		pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[JIG_ID_A][JIG_CH_1].FromString(pData->cMsgData[0]);
		pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[JIG_ID_A][JIG_CH_2].FromString(pData->cMsgData[1]);
		pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[JIG_ID_B][JIG_CH_1].FromString(pData->cMsgData[2]);
		pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[JIG_ID_B][JIG_CH_2].FromString(pData->cMsgData[3]);
		pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[JIG_ID_C][JIG_CH_1].FromString(pData->cMsgData[4]);
		pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[JIG_ID_C][JIG_CH_2].FromString(pData->cMsgData[5]);
		break;

	case EQUIP_SET_UNIT_STATUS_CHANGE :
		{
			GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s]"), 
				_T("EQUIP_SET_UNIT_STATUS_CHANGE"));

			CUnitStatus *pUnit = &pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[pData->sUnit][pData->sCellPos];

			pUnit->FromString(pData->cMsgData[0]);

			if(pUnit->m_InterlockState == STATUS_INTERLOCK_ON)
			{
				for (int i=0; i<MAX_INTERLOCK_MES ; i++)
				{
					if(pUnit->m_tUnitInterlockMES[i].m_cInterlockState == INTERLOCK_ADD)
						pUnit->m_tUnitInterlockMES[i].m_cInterlockState = INTERLOCK_HAPPEN;
				}
			}
			pMainFrm->m_pViewCIM->SetUnitStatusChange(pData->sUnit, pData->sCellPos);
		}
		break;

	case EQUIP_SET_UNIT_INTERLOCK_COMFIRM :
		{
			GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s]"), 
				_T("EQUIP_SET_UNIT_INTERLOCK_COMFIRM"));

			CUnitStatus *pUnit = &pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[pData->sUnit][pData->sCellPos];

			pUnit->FromString(pData->cMsgData[0]);
			pMainFrm->m_pViewCIM->SetUnitInterlockConfirm(pData->sUnit, pData->sCellPos, pData->cCellID, pData->cMsgData[1], pData->cMsgData[2]);
		}

		break;

	case EQUIP_SET_UNIT_OPCALL_COMFIRM :
		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s]"), 
			_T("EQUIP_SET_UNIT_OPCALL_COMFIRM"));

		pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[pData->sUnit][pData->sCellPos].FromString(pData->cMsgData[0]);
		pMainFrm->m_pViewCIM->SetUnitOPCallConfirm(pData->sUnit, pData->sCellPos, pData->cCellID, pData->cMsgData[1], pData->cMsgData[2]);

		break;
	}

	return true;
}


void CDlgSockStatus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default


	CDialog::OnTimer(nIDEvent);
}

BOOL CDlgSockStatus::RcvMsgFromMccClient(tag_Message* pData)
{
	//Socket data 검사 함수

// 	CString strMsg;
// 
// 	if (strcmp(pData->cLogType, "A")==0)
// 	{
// 		strMsg.Format("%s,%s,%s,%s,%s,%s,%s,%s=%s=%s=%s"
// 			, pData->cEventTime
// 			, pData->cModuleId
// 			, pData->cLogType
// 			, pData->cStepId
// 			, pData->cCellId
// 			, pData->cLotId
// 			, pData->cPpId
// 			, pData->cData1
// 			, pData->cData2
// 			, pData->cData3
// 			, pData->cData4
// 			);
// 	}
// 	else if (strcmp(pData->cLogType, "I")==0)
// 	{
// 		strMsg.Format("%s,%s,%s,%s,%s,%s,%s,%s=%s"
// 			, pData->cEventTime
// 			, pData->cModuleId
// 			, pData->cLogType
// 			, pData->cStepId
// 			, pData->cCellId
// 			, pData->cLotId
// 			, pData->cPpId
// 			, pData->cData1
// 			, pData->cData2
// 			);
// 	}
// 	else if (strcmp(pData->cLogType, "E")==0)
// 	{
// 		strMsg.Format("%s,%s,%s,%s,%s,%s,%s,%s=%s=%s"
// 			, pData->cEventTime
// 			, pData->cModuleId
// 			, pData->cLogType
// 			, pData->cStepId
// 			, pData->cCellId
// 			, pData->cLotId
// 			, pData->cPpId
// 			, pData->cData1
// 			, pData->cData2
// 			, pData->cData3
// 			);
// 	}
// 	else
// 		return FALSE;
// 
// 	m_clsMcc->WriteMccLog(strMsg);		//실제 로그를 기록하는 class 호출

	return true;
}

LRESULT CDlgSockStatus::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	char *pMsg;
	pMsg = (char*)lParam;
	GetLogCtrl()->AddBuf(wParam, "%s",pMsg);

	return 0;
}

BOOL CDlgSockStatus::SendMsgToControlPC(tag_Message* pData)
{
	BOOL m_bSendOK = FALSE;
	int nReturn;
	int nRetry;
	nRetry = 0;

	while(!m_bSendOK)
	{
		// 데이터 전송
		nReturn = m_pGxServer->Send((void*)pData, sizeof( *pData )); 
		// nReturn = m_pServerSocket->SendClient(SOCKET_NAME_CLIENT_EQ, pData, sizeof(*pData) );

		if (nReturn == SOCKET_ERROR)
		{
			if(GetLastError() == WSAEWOULDBLOCK)
			{	
				Sleep(100);
				if(++nRetry > 20) break;
			}
			else
			{
				if(++nRetry > 20) break;
			}
		}
		else
			m_bSendOK = TRUE;
	}

	return TRUE;
}

// Socket이 전송이 가능한 상태인지 조회한다.
BOOL CDlgSockStatus::GetSocketStatus()
{
	if ( m_pGxServer )
	{
		if ( m_pGxServer->GetClientCount() > 0 )
		{
			return TRUE;
		}
	}

	return FALSE;

	//if(m_pServerSocket!=NULL && m_pServerSocket->IsCreated() && m_pServerSocket->IsConnected())
	//	return TRUE;
	//else
	//	return FALSE;
}