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
	m_pGxServer = NULL;							// ������ ��ٸ��� Server socket pointer
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

	ON_MESSAGE(GxMSG_COMM_EQ_CONNECT,		OnTCPConnect)				// �PC�� ���� 
	ON_MESSAGE(GxMSG_COMM_EQ_DISCONNECT,	OnTCPDisconnect)			// �PC�� ���� ����
	ON_MESSAGE(GxMSG_COMM_EQ_RECEIVE,		OnTCPReceive)				// �PC���� ������ ����


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

	// Main Frame���� �PC���� ������ ���������� �˸���.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_EQ_DISCONNECT, 0, 0);


	return 0;
}

LRESULT CDlgSockStatus::OnSocketConnect( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;

	if(pSocket->IsConnected())
	{
		// Main Frame���� �PC���� ������ �˸���.
		::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_EQ_CONNECT, 0, 0);
	}


	return 0;
}



// Socket ��� ����ó��
BOOL CDlgSockStatus::SetGxSocketListen()
{
	BOOL bRet = TRUE;

	if ( !m_pGxServer )
	{
		m_pGxServer = new CGxSockServer( 0 );		// ��ȣ�� PC ����, Server
		m_pGxServer->m_nID		= 0;
		m_pGxServer->m_sName	= _T("EQ");	
		m_pGxServer->SetMsgInfo(this, GxMSG_COMM_EQ_CONNECT, GxMSG_COMM_EQ_DISCONNECT, GxMSG_COMM_EQ_RECEIVE);
		//m_pGxServer->SetCallbackFunction(pConnect, pClose, m_pFuncReceive);

		m_pGxServer->SetMultiConnect(FALSE);	// ���� ���� ��� ����
		m_pGxServer->SetMultiPeer(FALSE);		// ���� IP ���� ���� ����
		m_pGxServer->SetPort( 6000 );				// Listen�� ��Ʈ ����
		m_pGxServer->SetPacketInfo(eGxCOMM_PKT_COUNT, NULL, 0, sizeof( tag_Message ) );	// ���ű��̷� ��Ŷ ����

		bRet = m_pGxServer->Listen();				// ������ �����Ѵ�.
	}

	return bRet;
}


// � PC�� ����
LRESULT CDlgSockStatus::OnTCPConnect(WPARAM wParam, LPARAM lParam)
{
	// Main Frame���� �PC���� ������ �˸���.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_EQ_CONNECT, 0, 0);

	return 0;
}


// � PC���� ������ ������
LRESULT CDlgSockStatus::OnTCPDisconnect(WPARAM wParam, LPARAM lParam)
{
	// ��� Title�� CIM ȭ�鿡�� ������ ���������� �˷��ش�.
	::SendMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_EQ_DISCONNECT, 0, 0);

	return 0;
}

// � PC���� ������ ����
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

	int nLen = pClient->IsPacket();									// ���ŵ� ����
	int nSize = sizeof( tag_Message );
	int nRead;
	tag_Message* pBlock;												// ����ü ������ ������
// 	GetLogCtrl()->AddBuf(LOG_COMM, "Recv Length %d / %d",   nLen, nSize);


	// ���ŵ� ��Ŷ�� �����ϴ� ��� ��� ó���� �ش�.
	while ( nLen >= nSize )
	{
		BYTE* byBuff = new BYTE[ nSize ];							// �����͸� ���� �� ���� ����

		nRead = pClient->GetPacket( byBuff, nSize );				// �����͸� �޾ƿ´�.

		// �о�� �����Ϳ� �Ҵ�� Block�� Size�� ���� �����ؾ� ó���� �����Ѵ�.
		if ( nRead == nSize )
		{
			pBlock = (tag_Message*)byBuff;							// Typecasting

			RcvMsgFromAftClient( pBlock );							// ���� ������ ó��

			//m_listRcvData.AddTail( pBlock );								// ���ŵ� ������ ���� ����ü ������ list
			//m_log.Write(_T(",<-,%d,Class:%d Type:%d"), nRead, pBlock->sMsgClass, pBlock->sMsgType);		// Log ���
		}

		nLen = pClient->IsPacket();									// �ڿ��� ���ŵ� �����Ͱ� �����ϴ°� ?
// 		GetLogCtrl()->AddBuf(LOG_COMM, "Recv Length %d / %d",   nLen, nSize);
		//m_evtDataReceive.Set();											// �����Ͱ� ���ŵǾ����� �˸���.
		delete[] byBuff;														// ����� ��ģ �޸� ��ȯ
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


	// 2016-01-20, ���� Cell Data�� ������ Clear �ϴ� ��û
	case EQUIP_SET_CELL_DATA_CLEAR_REQUEST :			// ���� Cell Data�� ������ Clear
		// ���� Cell�� GaonNuri���� �������� ������ �ʿ����
		GetLogCtrl()->AddBuf(LOG_COMM, "JIG(%d), CELLPOS(%d), INNERID(%s) Cell Data Clear request",  pData->sUnit+1, pData->sCellPos+1, pData->cMsgData[0]);
// 		nJig		= pData->sUnit;
// 		nCellPos	= pData->sCellPos;
// 		pCellData	= &g_pData->m_Jig[nJig].tCellData[nCellPos];
// 		pCellData->Clear();														// ������ ������ �����Ѵ�.
// 
// 		// 2016-02-03, jhLee, FDC INSPECTION �׸��� ����� ���� Inner ID�� �����صд�. ���� Cell ID�� �� �� ����.
// 		strcpy_s(pCellData->m_cInnerID,	pData->cMsgData[0]);	// Inner ID ����

		break;


	// 2015-08-12, jhLee, Cell Tracking Information ��� �߰�, ���LOT (FAB HOLD) ó�� �� TMD Validation ó���� ���� ���� ���
	case EQUIP_SET_CELL_INFORMATION_REQUEST:
		GetLogCtrl()->AddBuf(LOG_COMM, "INNERID(%s), CELLID(%s), Cell Tracking Info request",  pData->cMsgData[0], pData->cCellID);
		g_pData->m_CellInformation.AddCellID(pData->cCellID);

		pMainFrm->m_pViewCIM->EventCellInfoRequest();


		break;


	// 2016-01-04, jhLee, POCHó���� ���� Cell Lot Information ��� �߰�
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

		strcpy_s(pCellData->m_cProcessFlag,	pData->cMsgData[0]);				// �����˻� ���� ���� ����
		pMainFrm->m_pViewCIM->EventInspectionResultReport();


		break;

	case EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD:
		GetLogCtrl()->AddBuf(LOG_COMM, "JIG(%d), CELLPOS(%d), INNERID(%s), CELLID(%s), Track-Out",  pData->sUnit+1, pData->sCellPos+1, pData->cMsgData[0], pData->cCellID);
		nJig = pData->sUnit;
		nCellPos = pData->sCellPos;
		pCellData = &g_pData->m_CellDataTrackOut;

		strcpy_s(pCellData->m_cCellID,					pData->cCellID);
		
		// TrackOut�� �� Judge ���� ���� [9/27/2017 OSC]
		memcpy(pCellData->m_cJudge, pData->cMsgData[1], sizeof(pCellData->m_cJudge));
		strcpy_s(pCellData->m_cDescription, pData->cMsgData[2]);

		// Defect ��ǥ������ ���� ����Ѵ�.
		strcpy_s(pCellData->m_cDefectPos, pData->cMsgData[3]);

		// MTP_WRITE ���
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
			GetLocalTime(&g_pData->m_tAlarmInsert.m_systmTimeSet);	//MiliSecond������ ����ϱ�����.

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
			GetLocalTime(&g_pData->m_tAlarmInsert.m_systmTimeRun);	//MiliSecond������ ����ϱ�����.


			// 2016-06-02, jhLee, ����
			//�񰡵� ���ռ� �߰�Log
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

		// ���Ͷ��̸� ����  �߰��Ѱ� Ȱ��ȭ��Ų��
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
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// �⺻���� ���� ������ ǥ���Ѵ�.
		break;

	case EQUIP_SET_PPID_OPERATION:
		strcpy_s(g_pData->m_cPPID, pData->cMsgData[8]);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// �⺻���� ���� ������ ǥ���Ѵ�.
		break;

	case EQUIP_SET_INS1_USER_LOGIN:
		strcpy_s(g_pData->m_cOperatorID1, pData->cMsgData[5]);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// �⺻���� ���� ������ ǥ���Ѵ�.

		break;

	case EQUIP_SET_INS1_USER_LOGOUT:
		strcpy_s(g_pData->m_cOperatorID1, g_pData->m_cEQPID);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// �⺻���� ���� ������ ǥ���Ѵ�.
		break;

	case EQUIP_SET_INS2_USER_LOGIN:
		strcpy_s(g_pData->m_cOperatorID2, pData->cMsgData[5]);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// �⺻���� ���� ������ ǥ���Ѵ�.
		break;

	case EQUIP_SET_INS2_USER_LOGOUT:
		strcpy_s(g_pData->m_cOperatorID2, g_pData->m_cEQPID);
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// �⺻���� ���� ������ ǥ���Ѵ�.
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
			// 9�� �׸����� ���� Interlock type�� �޴´� - LSH171128
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

	case EQUIP_SET_USER_LOGIN_REQUEST:		//GT.PARK - Log IN ��û
		strcpy_s(g_pData->m_cOperatorID1, pData->cMsgData[5]);
		strcpy_s(g_pData->m_cPassword, pData->cMsgData[6]);
		strcpy_s(g_pData->m_cOptionInfo, "LOGIN");
		strcpy_s(g_pData->m_cComment, "");

		pMainFrm->m_pViewCIM->EventLogIn_Out();
		break;

	case EQUIP_SET_USER_LOGOUT_REQUEST:		//GT.PARK - Log OUT ��û
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

			// Change list�� ���� �Է��Ѵ� - LSH171130
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[_tstoi(pData->cMsgData[4])/* Index */].EFID = _tstoi(pData->cMsgData[0]/* EFID */);
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[_tstoi(pData->cMsgData[4])/* Index */].EFNAME = pData->cMsgData[1]/* EFNAME */;
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[_tstoi(pData->cMsgData[4])/* Index */].EFST = pData->cMsgData[2]/* EFST */;
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer[_tstoi(pData->cMsgData[4])/* Index */].EFST_Old = pData->cMsgData[3]/* OLD_EFST */;

			// Gaonnuri�� �����ִ� List�� ���¸� Update�Ѵ� - LSH171130
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionList[_tstoi(pData->cMsgData[0]) - 1/* Index */].EFNAME = pData->cMsgData[1]/* EFNAME */;
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionList[_tstoi(pData->cMsgData[0]) - 1/* Index */].EFST = pData->cMsgData[2]/* EFST */;
			pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionList[_tstoi(pData->cMsgData[0]) - 1/* Index */].EFST_Old = pData->cMsgData[3]/* OLD_EFST */;

			// List�� ������ �κ����� Ȯ�� - LSH171130
			if(_tstoi(pData->cMsgData[5])/* LastIndexFlag */)
			{
				// Function change���� - LSH171130
				pMainFrm->m_pViewCIM->EventEquipFunctionChange(pMainFrm->m_pViewCIM->m_EquipFunction.m_FunctionChangeBuffer, _tstoi(pData->cMsgData[4]) +	 1/* LastIndex */, pData->cMsgData[6]/* ByWho */);
				
				// List������ ���� - LSH171130
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
	//Socket data �˻� �Լ�

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
// 	m_clsMcc->WriteMccLog(strMsg);		//���� �α׸� ����ϴ� class ȣ��

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
		// ������ ����
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

// Socket�� ������ ������ �������� ��ȸ�Ѵ�.
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