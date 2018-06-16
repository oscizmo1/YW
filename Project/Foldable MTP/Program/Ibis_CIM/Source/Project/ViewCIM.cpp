// ViewCIM.cpp : implementation file
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "ViewCIM.h"

#include "GxGUI\GxStaticEx.h"
#include "GxGUI\GxButtonEx.h"
#include "GxGUI\GxUICtrl.h"


BOOL g_bStatHistoryCheck = FALSE;

// CViewCIM

IMPLEMENT_DYNCREATE(CViewCIM, CFormView)

CViewCIM::CViewCIM()
	: CFormView(CViewCIM::IDD)
{
	m_bXComInit = FALSE;
}

CViewCIM::~CViewCIM()
{
}

void CViewCIM::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIEW_CIM			, m_lstbxMsg);
	DDX_Control(pDX, IDC_LIST_VIEW_COMM			, m_lstbxCommMsg);
	DDX_Control(pDX, IDC_XCOMPROCTRL1, m_XComPro);
}

BEGIN_MESSAGE_MAP(CViewCIM, CFormView)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()

	ON_MESSAGE(GxMSG_GUI_EQ_CONNECT,		OnEQConnect)					// 운영PC와 연결 
	ON_MESSAGE(GxMSG_GUI_EQ_DISCONNECT,	OnEQDisconnect)				// 운영PC와 연결 끊김
	ON_MESSAGE(GxMSG_GUI_COMM_UPDATE,	OnHostCommUpdate)				// HOST 통신 데이터 수신 표시

	

END_MESSAGE_MAP()


// CViewCIM diagnostics

#ifdef _DEBUG
void CViewCIM::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewCIM::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewCIM message handlers
BEGIN_EVENTSINK_MAP(CViewCIM, CFormView)
	ON_EVENT(CViewCIM, IDC_XCOMPROCTRL1, 1, CViewCIM::SecsEventXcomproctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CViewCIM, IDC_XCOMPROCTRL1, 2, CViewCIM::SecsMsgXcomproctrl1, VTS_NONE)

	ON_EVENT(CViewCIM, IDC_GXBTN_COMM_MODE1, DISPID_CLICK, CViewCIM::ClickGxbtnCommMode1, VTS_NONE)
	ON_EVENT(CViewCIM, IDC_GXBTN_COMM_MODE2, DISPID_CLICK, CViewCIM::ClickGxbtnCommMode2, VTS_NONE)
	ON_EVENT(CViewCIM, IDC_GXBTN_COMM_MODE3, DISPID_CLICK, CViewCIM::ClickGxbtnCommMode3, VTS_NONE)
	ON_EVENT(CViewCIM, IDC_GXBTN_COMM_CONNECT, DISPID_CLICK, CViewCIM::ClickGxbtnCommConnect, VTS_NONE)
END_EVENTSINK_MAP()


// 운영 PC와 연결
LRESULT CViewCIM::OnEQConnect(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;			

	if(m_bXComInit == FALSE)
	{
		// 운영이랑 연결되면 그때 호스트랑 붙인다 [11/25/2017 OSC]
		SetConnect_XComComm();					// XCom 연결 체크
		m_bXComInit = TRUE;
	}


	return 0;
}

// 운영 PC와의 연결이 끊어짐
LRESULT CViewCIM::OnEQDisconnect(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;			

	// CGxUICtrl::SetStaticIcon(this, IDC_GXLBL_CIM_COMM_STATE2, 0);					// EQ Icon OFF

	return 0;
}

// 통신 데이터 수신 표시
LRESULT CViewCIM::OnHostCommUpdate(WPARAM wParam, LPARAM lParam)
{
	UINT nIdx	= wParam;			
	UINT nFlag	= lParam;

	switch ( nIdx )
	{
		case eIDX_HostConnect :		// HOST와의 연결
			if ( nFlag == 0 )			// OFF
			{
				// OFFLINE 표시
				CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE1, _T("Disconnect"));
				CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE2, _T("OFF Line"));

				CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_CONNECT,	GXCOLOR_OFF);		// CONNECT button OFF
				CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE1,		GXCOLOR_OFF);		// REMOTE button OFF
				CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE2,		GXCOLOR_OFF);		// LOCAL button OFF
				CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3,		GXCOLOR_ON);		// OFFLINE button ON

			}
			else	// ON
			{
				CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE1, _T("Connect"));
				CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_CONNECT,	GXCOLOR_ON);			// CONNECT button ON
			}
			break;


		case eIDX_HostOnline :		// SELECT / ONLINE 상태 변경
			if ( nFlag == 0 )			// OFF
			{
				CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE2, _T("OFF Line"));
				CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3,		GXCOLOR_ON);		// OFFLINE button ON
			}
			else
			{
				CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE2, _T("ON Line"));
				CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE1,		GXCOLOR_ON);		// REMOTE button OFF
				CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3,		GXCOLOR_OFF);			// OFFLINE button OFF
			}
			break;

		case eIDX_HostRecevie :		// 메세지 수신 처리
			CGxUICtrl::SetStaticColor(this, IDC_GXLBL_CIM_CAP1, ( nFlag == 0 ) ? GXCOLOR_OFF : GXCOLOR_ON);
			break;
	} //of switch


	return 0;
}	


void CViewCIM::SecsEventXcomproctrl1(short nEventId, long lParam)
{
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();

	if( nEventId == ALMXC_T3)	// T3 Timeout	
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "[ALARM] T3 timeout alarm occurs" );
	}
	else if( nEventId == EVTXC_NOT_SELECTED)	// change to "NOT SELECTED" (HSMS)	
	{
		// m_ledHost.put_Active(false);	// Host UI 색상 변경
		// CGxUICtrl::SetStaticIcon(this, IDC_GXLBL_CIM_COMM_STATE3, 0);			// HOST Icon OFF

		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_COMM_UPDATE, eIDX_HostOnline, 0);		// SELECT 해제 (OFFLINE)


		GetLogCtrl()->AddBuf(LOG_CIM, "[ALARM] HSMS not selected event happens" );
	}
	else if ( nEventId == EVTXC_NOT_CONNECTED)	// change to "NOT CONNECTED" (HSMS) 	
	{
		// CRST (=Cim Status) 상태 갱신
		strcpy_s(g_pData->m_cOldCRST, g_pData->m_cCRST); // Old CIM State
		strcpy_s(g_pData->m_cCRST, CRST_OFF);			 // Current CIM State

		pMainFrm->m_pViewCIM->SetControlState();		// 운영측에 CIM 상태를 전송하기 위해 데이터 처리를 진행한다.

		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_COMM_UPDATE, eIDX_HostConnect, 0);		// CONNECT 해제
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_COMM_UPDATE, eIDX_HostOnline, 0);			// SELECT 해제 (OFFLINE)

		//		m_lblHostStatus.put_Caption("Disconnected");
		//		m_lblControlStatus.put_Caption("OFFLINE");
		//!		pMainFrm->m_wndMainTitle.m_lblStatus.SetText("OFFLINE");

		// UI 색상 변경

		// m_swcRemote.put_Active(false);
		// m_swcLocal.put_Active(false);
		// m_swcOffline.put_Active(true);

		// m_ledHost.put_Active(false);
		// CGxUICtrl::SetStaticIcon(this, IDC_GXLBL_CIM_COMM_STATE1, 0);			// HOST Icon OFF

		m_XComPro.Stop();
		m_XComPro.Close();
		if( m_XComPro.Start() == 0 ) 
		{
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom Restarted successfully" );
		}


		GetLogCtrl()->AddBuf(LOG_CIM, "[EVENT] HSMS not connected event happens" );
	}
	else if( nEventId == EVTXC_SELECTED)	// change to "SELECTED" (HSMS)	
	{    
		m_Automation.TxS1F1();				// Trans Msg S1F1 

		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_COMM_UPDATE, eIDX_HostConnect, 1);		// CONNECT 
		::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_COMM_UPDATE, eIDX_HostOnline, 1);			// SELECT 

		GetLogCtrl()->AddBuf(LOG_CIM, "[EVENT] HSMS selected event happens" );
		((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetTimer(S2F18REP_TIMER_PTRID, 2000, NULL);
	}
	else 
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "[EVENT] Event Code = %d, Parameter = %d", nEventId, lParam );
	}
}


void CViewCIM::SecsMsgXcomproctrl1()
{
	short	nCRST;

	TRY
	{
		nCRST = atoi(g_pData->m_cCRST);
		switch ( nCRST)	//0:OFF-LINE, 1:REMOTE, 2:LOCAL
		{

			// 		case 1:
			// 		case 2:
			// 		case 0:

		case E_CRST_REMOTE:
		case E_CRST_LOCAL:
		case E_CRST_OFFLINE:

			// m_ledHSMS.put_Active(true);
			// CGxUICtrl::SetStaticIcon(this, IDC_GXLBL_CIM_COMM_STATE3, 1);			// HSMS Icon ON
			::PostMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_COMM_UPDATE, eIDX_HostRecevie, 1);				// HSMS 메세지 수신 표시

			m_Automation.Msg_Proc();

			// m_ledHSMS.put_Active(false);
			// CGxUICtrl::SetStaticIcon(this, IDC_GXLBL_CIM_COMM_STATE3, 0);			// HSMS Icon OFF
			::PostMessage(((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_COMM_UPDATE, eIDX_HostRecevie, 0);				// HSMS 메세지 수신 처리 완료 표시

			break;
		}

	}
	CATCH_ALL (e) 
	{

	}
	END_CATCH_ALL
}

void CViewCIM::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	m_bLogByMesgPass = FALSE;

	//LoadConfig(); //미사용

	EquipmentStatusChange(EQST_INIT, "3"); // 장비상태 INIT 지정
	
	SetConfig_XComLog();					// XCom Log 설정 지정

	m_Automation.m_pXCom = &m_XComPro;

	for (int i=0; i<MAX_JIG_CNT; i++)
	{
		for (int j=0; j<MAX_JIG_CELL_CNT; j++)
		{
			bWriteDataEvent[i][j] = FALSE;
		}
	}

	m_UnitStatusBank.SetUnitID(g_pData->m_cEQPID);

	SetTimer(TRUE_LOG_BY_MESG_TIMER, 10*60*1000, NULL); // Message Log 체크 Timer
}

void CViewCIM::EventEquipFunctionUpdate(tag_Message *pMsg)
{
	if(m_EquipFunction.UpdateEquipFunction(pMsg))
	{
		// FunctionList 변경됬음을 보고
	}
}

void CViewCIM::EventEquipFunctionChange(EQUIP_FUNCTION* function, UINT nIndexSize, CString strBYWHO)
{
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
	{
		m_Automation.TxS6F11_CEID111(function, nIndexSize, strBYWHO);
	}
	else
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "SetUnitStatusChange - OFF-LINE");
	}
}

void CViewCIM::SetUnitStatusChange(int nJig, int nCh)
{
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
	{
		m_Automation.TxS6F11_CEID102(nJig, nCh);

	}
	else
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "SetUnitStatusChange - OFF-LINE");
	}

}

void CViewCIM::SetUnitInterlockConfirm(int nJig, int nCh, CString strCellID, CString strProductID, CString strStepID)
{
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
	{
		m_Automation.TxS6F11_CEID514(nJig, nCh, strCellID, strProductID, strStepID);

	}
	else
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "SetUnitInterlockConfirm - OFF-LINE");
	}
}

void CViewCIM::SetUnitOPCallConfirm(int nJig, int nCh, CString strCellID, CString strProductID, CString strStepID)
{
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
	{
		m_Automation.TxS6F11_CEID514(nJig, nCh, strCellID, strProductID, strStepID);

	}
	else
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "SetUnitInterlockConfirm - OFF-LINE");
	}
}

void CViewCIM::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_bLogByMesgPass = FALSE;
	m_XComPro.Stop();
	m_XComPro.Close();
	CFormView::OnClose();
}

void CViewCIM::OnTimer(UINT_PTR nIDEvent)
{
	// CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();

	//g_pData->m_nAliveCount = 0;

	// HOST 연결 시도
	if ( nIDEvent == HOST_CONNECT_TIMER )
	{
		KillTimer( nIDEvent );

		int nRet = m_XComPro.Start();						// 연결 시도

		if( nRet == 0 ) 
		{
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom started successfully" );
		}
		else 
		{
			GetLogCtrl()->AddBuf(LOG_CIM,"Fail to start XCom (%d)", nRet );
		}
	}//of HOST 연결
	else if (nIDEvent==S2F18REP_TIMER_PTRID)	// REMOTE 변경
	{
		ControlStateChange(E_CRST_REMOTE);

		EventEquipmentStatusChange();
		KillTimer(S2F18REP_TIMER_PTRID);
	}
	else if (nIDEvent == TRUE_LOG_BY_MESG_TIMER)	// XCOM Log 설정
	{
		KillTimer(nIDEvent);
		if (m_bLogByMesgPass)
		{
			if (SetUseLogByMesg(FALSE))
				GetLogCtrl()->AddBuf(LOG_CIM, "XCom SetUseLogByMesg - false");
		}
		else
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom Can't SetUseLogByMesg , m_bLogByMesgPass - false");
		
		SetTimer(FALSE_LOG_BY_MESG_TIMER, 10*1000, NULL);
	}
	else if (nIDEvent == FALSE_LOG_BY_MESG_TIMER)	// XCOM Log 설정
	{
		KillTimer(nIDEvent);
		if (m_bLogByMesgPass)
		{
			if (SetUseLogByMesg(TRUE))
				GetLogCtrl()->AddBuf(LOG_CIM, "XCom SetUseLogByMesg - true");
		}
		else
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom Can't SetUseLogByMesg , m_bLogByMesgPass - false");

		SetTimer(TRUE_LOG_BY_MESG_TIMER, 10*60*1000, NULL);
	}
	else
	{
		CString	strTemp;
		int nIndex;

		nIndex= -1;
		strTemp.Format("%d", nIDEvent);

		// FDC SVID 값 HOST 보고
		if(g_pData->m_FDC.GetTraceData(strTemp)>=0)
		{
			nIndex = g_pData->m_FDC.GetTraceData(strTemp);

			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				m_Automation.TxS6F1(nIndex);			// Trace data Send

			else
			{
				KillTimer(nIDEvent);
				g_pData->m_FDC.TraceSendClear(nIndex);
			}
		}
	}

	//// 운영PC와의 연결 상태 체크
	//if (nIDEvent == SOCKET_STATUS_TIMER)
	//{
	//	CDataSocket* pSocket;
	//	pSocket = GetSockStsDlg()->m_pServerSocket;
	//	if(pSocket!=NULL)
	//	{
	//		if(m_ledEquip.get_Active()!=pSocket->IsConnected())
	//		{
	//			if(pSocket->IsConnected())
	//			{
	//				m_ledEquip.put_Active(true);
	//				pMainFrm->m_wndMainTitle.m_lblSocketEqStatus.SetBkColor(RGB(0, 255, 0));
	//				pMainFrm->m_wndMainTitle.m_lblSocketEqStatus.SetText("Eqp Connected");
	//			}
	//			else
	//			{
	//				m_ledEquip.put_Active(false);
	//				pMainFrm->m_wndMainTitle.m_lblSocketEqStatus.SetBkColor(RGB(255, 0, 0));
	//				pMainFrm->m_wndMainTitle.m_lblSocketEqStatus.SetText("Eqp Disconnected");
	//			}
	//		}
	//	}
	//}



	CFormView::OnTimer(nIDEvent);
}

void CViewCIM::ViewCIMWriteDip(LPCTSTR szLogText)
{
	m_lstbxMsg.InsertString(0, szLogText);
// 	m_lstbxMsg.AddString(szLogText);
// 	m_lstbxMsg.SetTopIndex( m_lstbxMsg.GetCount()-1);
	if (m_lstbxMsg.GetCount() > 1000 )
	{
		m_lstbxMsg.DeleteString( m_lstbxMsg.GetCount()-1 ); 
	}
}

void CViewCIM::ViewCOMWriteDip(LPCTSTR szLogText)
{
	m_lstbxCommMsg.InsertString(0, szLogText);
// 	m_lstbxCommMsg.AddString(szLogText);
// 	m_lstbxCommMsg.SetTopIndex( m_lstbxMsg.GetCount()-1);
	if (m_lstbxCommMsg.GetCount() > 1000 )
	{
		m_lstbxCommMsg.DeleteString( m_lstbxMsg.GetCount()-1 ); 
	}
}



BOOL CViewCIM::SetCellInformation(CString strCellID, CString strProductID, CString strCellInfoResult)
{
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_CELL_INFORMATION;

	strcpy_s(pData->cCellID, strCellID);

	strcpy_s(pData->cMsgData[2], strProductID);
	strcpy_s(pData->cMsgData[3], strCellInfoResult);

	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetCellTrackingInformation] - Cellid(%s), Productid(%s), LotinfoResult(%s), RETURN(%d)"
		, pData->cCellID
		, pData->cMsgData[2]
		, pData->cMsgData[3]
		, nReturn);

	delete pData;

	return TRUE;
}


// 2016-01-03, jhLee,
//
// POCH 처리를 위한 CAutomation::RxS3F109() 에서 호출한 함수
//
BOOL CViewCIM::SetCellLotInformation(CString strCellID, CString sName, CString sValue)
{
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_CELL_LOT_INFORMATION;						// Cell Lot Information 응답 수신

	strcpy_s(pData->cCellID, strCellID);

	strcpy_s(pData->cMsgData[2], sName);									// 응답 항목명
	strcpy_s(pData->cMsgData[3], sValue);									// 응답 값

	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetCellLotInformation] - Cellid(%s), ItemName(%s), ItemValue(%s), RETURN(%d)"
		, pData->cCellID
		, sName
		, sValue
		, nReturn);

	delete pData;

	return TRUE;
}

BOOL CViewCIM::SetOPCall(CString strModuleID)
{
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	// ModuleID Check
	BOOL bAble = FALSE;
	if (strcmp(_T(""), strModuleID)==0)
		bAble = TRUE;
	if (strcmp(_T("-"), strModuleID)==0)
		bAble = TRUE;
	if (strcmp(g_pData->m_cEQPID, strModuleID)==0)
		bAble = TRUE;

	if(m_UnitStatusBank.GetUnit(strModuleID) != NULL)
		bAble = TRUE;

	if(bAble == FALSE)
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_OPCALL;

	int nLen=0; int nSplit=0; int nTail=0; CString strSplitMsg[4];
	strcpy_s(pData->cMsgData[0], strModuleID);
	strcpy_s(pData->cMsgData[7], g_pData->m_cOPCallID);
	strMsg = g_pData->m_cOPCallMSG;
	nLen = strMsg.GetLength();
	nSplit = nLen/(MAX_MES_SIZE-1);
	nTail = nLen%(MAX_MES_SIZE-1);

	if (nLen>MAX_MES_SIZE)
	{
		for (int i=0; i<nSplit; i++)
		{
			strSplitMsg[i] = strMsg.Mid(MAX_MES_SIZE*i-i, MAX_MES_SIZE-1);
			strcpy_s(pData->cMsgData[8+i], strSplitMsg[i]);	
			if (nTail>0 && i+1==nSplit)
			{
				i++;
				strSplitMsg[i] = strMsg.Right(nTail);
				strcpy_s(pData->cMsgData[8+i], strSplitMsg[i]);	
			}
		}
	}
	else
	{
		strcpy_s(pData->cMsgData[8], strMsg);
	}


	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetOPCall] - OPCallId(%s), OPCallOriginMsg(%s), OPCallMsg(%s), RETURN(%d)"
		, g_pData->m_cOPCallID
		, g_pData->m_cOPCallMSG
		, strMsg
		, nReturn);

	delete pData;

	return TRUE;
}

BOOL CViewCIM::SetInterlock(CString strInterlockMode)
{
	//SetEqStatus(E_EQST_MATCH_INTERLOCK);
	g_bStatHistoryCheck = TRUE;

		// 이건 대체 왜 해놓은 거임....? 알람도 아닌데 알람 카운트 1로 고정해놓고 인터락 해제할 때 카운트 초기화도 안하고.. [9/27/2017 OSC]
	g_pData->m_tAlarm.Clear();
//	strcpy_s(g_pData->m_tAlarm.m_cALST,_T(""));
	strcpy_s(g_pData->m_tCurAlarmList[0].m_cALCD, _T("2"));
//Cim스펙확인후 자리수 늘리기.

//	strcpy_s(g_pData->m_tCurAlarmList[0].m_cALID, g_pData->m_cInterlockID);
	memcpy(g_pData->m_tCurAlarmList[0].m_cALID,g_pData->m_cInterlockID,sizeof(g_pData->m_tCurAlarmList[0].m_cALID));
	strcpy_s(g_pData->m_tCurAlarmList[0].m_cALTX, g_pData->m_cInterlockMSG);

	//g_pData->m_nCurAlarmListCnt = 1;
	

	if(SetEqStatus(E_EQST_MATCH_INTERLOCK))
	{
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{
			m_Automation.TxS6F11(101);
			g_pData->m_tAlarm.Clear();
		}
	}
	else
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "SetInterlock - OFF-LINE");
	}

	g_bStatHistoryCheck = FALSE;

	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;
	
	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_INTERLOCK;


	int nLen=0; int nSplit=0; int nTail=0; CString strSplitMsg[4];
	strcpy_s(pData->cMsgData[7], g_pData->m_cInterlockID);
	strMsg = g_pData->m_cInterlockMSG;
	nLen = strMsg.GetLength();
	nSplit = nLen/(MAX_MES_SIZE-1);
	nTail = nLen%(MAX_MES_SIZE-1);

	if (nLen>MAX_MES_SIZE)
	{
		for (int i=0; i<nSplit; i++)
		{
			strSplitMsg[i] = strMsg.Mid(MAX_MES_SIZE*i-i, MAX_MES_SIZE-1);
			strcpy_s(pData->cMsgData[8+i], strSplitMsg[i]);	
			if (nTail>0 && i+1==nSplit)
			{
				i++;
				strSplitMsg[i] = strMsg.Right(nTail);
				strcpy_s(pData->cMsgData[8+i], strSplitMsg[i]);	
			}
		}
		
	}
	else
	{
		strcpy_s(pData->cMsgData[8], strMsg);
	}


	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetInterlock] - InterlockId(%s), InterlockOriginMsg(%s), InterlockMsg(%s), RETURN(%d)"
		, g_pData->m_cInterlockID
		, g_pData->m_cInterlockMSG
		, strMsg
		, nReturn);

	delete pData;

	return TRUE;
}

BOOL CViewCIM::SetInterlockRequest(int RCMD, CString strUnitID)
{
	// 설비에 ACK, NACK여부를 물어본다
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	CString strType;
	if(RCMD == 2)
		strType = EFST_STEP;
	if(RCMD == 11)
		strType = EFST_TRANSFER;
	if(RCMD == 12)
		strType = EFST_LOADING;
	if(RCMD == 13)
		strType = EFST_STEP;
	if(RCMD == 14)
		strType = EFST_OWN;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;

	strcpy_s(g_pData->m_tCurAlarmList[0].m_cALCD, _T("2"));


	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_INTERLOCK_REQ;

	strcpy_s(pData->cMsgData[0], CStringSupport::N2C(RCMD));
	strcpy_s(pData->cMsgData[1], strType);
	strcpy_s(pData->cMsgData[2], g_pData->m_cInterlockID);
	strcpy_s(pData->cMsgData[3], strUnitID);	// EQPID or UNITID
	strcpy_s(pData->cLongMsgData[0], g_pData->m_cInterlockMSG);
	
	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetInterlockRequest] - RCMD(%d), InterlockType(%s), InterlockId(%s), InterlockMsg(%s), RETURN(%d)"
		, RCMD
		, strType
		, g_pData->m_cInterlockID
		, g_pData->m_cInterlockMSG
		, nReturn);

	delete pData;

	return TRUE;
}

void CViewCIM::EventInterlockRequestAck(CString RCMD, BOOL bAck)
{
	int nAck;
	if(bAck)
		nAck = 0;
	else
		nAck = 5;
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.AckS2F42(RCMD, nAck);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventInterlockRequestAck - OFF-LINE");
}

void CViewCIM::SetPPIDChange(void)
{

}

BOOL CViewCIM::SetTerminalMessage()
{
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_TERMINAL;

	
	int nLen=0; int nSplit=0; int nTail=0; CString strSplitMsg[4];
	strMsg = g_pData->m_cTerminalMsg;
	nLen = strMsg.GetLength();
	nSplit = nLen/(MAX_MES_SIZE-1);
	nTail = nLen%(MAX_MES_SIZE-1);

	if (nLen>=MAX_MES_SIZE)
	{
		for (int i=0; i<nSplit; i++)
		{
			strSplitMsg[i] = strMsg.Mid(MAX_MES_SIZE*i-i, MAX_MES_SIZE-1);
			strcpy_s(pData->cMsgData[8+i], strSplitMsg[i]);	
			if (nTail>0 && i+1==nSplit)
			{
				i++;
				strSplitMsg[i] = strMsg.Right(nTail);
				strcpy_s(pData->cMsgData[8+i], strSplitMsg[i]);	
			}
		}
	}
	else
	{
		strcpy_s(pData->cMsgData[8], strMsg);
	}

	//kjpark 20170929 Terminel TID 출력
	strMsg = g_pData->m_cTerminalID;
	strcpy_s(pData->cMsgData[7], strMsg);
	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetTerminalMessage] - TerminalID(%s), TerminalOriginMsg(%s), TerminalMsg(%s), RETURN(%d)"
		, g_pData->m_cTerminalID
		, g_pData->m_cTerminalMsg
		, strMsg
		, nReturn);

	delete pData;

	return TRUE;
}

// 운영PC에 CIM 상태를 보고하기 위한 Data 처리 
BOOL CViewCIM::SetControlState(void)
{
	
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;
	
	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_CONTROL;

	strcpy_s(pData->cMsgData[9], g_pData->m_cCRST); //0:OFF-LINE, 1:ON-LINE REMOTE, 2:ON-LINE LOCAL


	if (strcmp(g_pData->m_cCRST, CRST_REMOTE)==0)
	{
		strcpy_s(pData->cMsgData[10], "REMOTE");
	}
	else if (strcmp(g_pData->m_cCRST, CRST_LOCAL)==0)
	{
		strcpy_s(pData->cMsgData[10], "LOCAL");
	}
	else if (strcmp(g_pData->m_cCRST, CRST_OFF)==0)
	{
		strcpy_s(pData->cMsgData[10], "OFFLINE");
	}
	
	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetControlState] - CRST(%s), CRSTSTR(%s), RETURN(%d)"
		, pData->cMsgData[9]
		, pData->cMsgData[10]
		, nReturn);

	delete pData;

	return TRUE;
}

// 운영PC에 가온누리 프로그램 Version을 보고하기 위한 Data 처리 
BOOL CViewCIM::SetDataPCVersion(void)
{
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_DATAPC_VERSION;

	strcpy_s(pData->cMsgData[8], g_pData->m_cEQPID);			// EQUIP ID
	strcpy_s(pData->cMsgData[9], g_pData->m_cVersionUseCellLog); // GaonNuri Program Version

	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetDataPCVersion] - Version(%s) EquipID(%s), RETURN(%d)"
		, pData->cMsgData[9]
		, pData->cMsgData[8]
		, nReturn);

	delete pData;

	return TRUE;
}

// 운영PC에 가온누리 프로그램 Line ID를 보고하기 위한 Data 처리 
BOOL CViewCIM::GetLineID(void)
{
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_LINE_NO;

	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[GetLineID] - RETURN(%d)"
	, nReturn);

	delete pData;

	return TRUE;
}

//AMT 운영프로그램에서 CRST 변경시...
void CViewCIM::EventChangeControlState(void)
{
	// 	short	nCRST;
	// 
	// 	nCRST = atoi(g_pData->m_cCRST);
	// 	switch ( nCRST)	//0:OFF-LINE, 1:REMOTE, 2:LOCAL
	// 	{
	// 	case 1:
	// 		m_lblControlStatus.put_Caption("REMOTE");
	// 		m_swcRemote.put_Active(true);
	// 		m_swcLocal.put_Active(false);
	// 		m_swcOffline.put_Active(false);
	// 
	// 		m_Automation.TxS6F11(106);
	// 		break;
	// 
	// 	case 2:
	// 		m_lblControlStatus.put_Caption("LOCAL");
	// 		m_swcRemote.put_Active(false);
	// 		m_swcLocal.put_Active(true);
	// 		m_swcOffline.put_Active(false);
	// 
	// 		m_Automation.TxS6F11(105);
	// 		break;
	// 
	// 	case 0:
	// 		m_lblControlStatus.put_Caption("OFFLINE");
	// 		m_swcRemote.put_Active(false);
	// 		m_swcLocal.put_Active(false);
	// 		m_swcOffline.put_Active(true);
	// 
	// 		m_Automation.TxS6F11(104);
	// 		break;
	// 
	// 	default :
	// 		AddLog("! EQUIP EVENT Abnormal Control status : %d", nCRST);		
	// 	}	
}

void CViewCIM::EventCellProcessStart()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
	{
		m_Automation.TxS6F11(401, 0, 0);

	}
	else
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "EventCellProcessStart - OFF-LINE");
	}
}

void CViewCIM::EventCellProcessComplete(UINT nUnit, UINT nCellPos)
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
	{
		m_Automation.TxS6F11(406, nUnit, nCellPos);
	}
	else
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "EventCellProcessComplete - OFF-LINE");
	}
}

void CViewCIM::EventCellEndInspection(UINT nJig, UINT nCellPos)
{
	// QMS 삭제

	//CString			strPath1;
	//CString			strPath2;
	//CString			strEqpPathDir;
	//CString			strHostPathDir;
	//CString			strMonitorPathDir;
	//CString			strEqpBackupDelDir;
	//CString			strHostImgPahtDir;

	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	//{
	//	if(CSettingParm::bQmsWriteAmt)
	//	{
	//		GetLogCtrl()->AddBuf(LOG_CIM, "WriteAmtDataFile - Write Event CELL ID( %s )", g_pData->m_Jig[nJig].tCellData[nCellPos].m_cCellID);
	//		bWriteDataEvent[nJig][nCellPos] = TRUE;
	//	}
	//	else
	//	{
	//		theApp.m_InsData.SetQMSRtn(nJig, nCellPos, E_CELL, E_QMS_ERR_SYSTEM);
	//		theApp.m_InsData.SetQMSRtn(nJig, nCellPos, E_MTPW, E_QMS_ERR_SYSTEM);
	//		theApp.m_InsData.SetQMSRtn(nJig, nCellPos, E_TSP, E_QMS_ERR_SYSTEM);
	//		theApp.m_InsData.SetQMSRtn(nJig, nCellPos, E_MTPR, E_QMS_ERR_SYSTEM);

	//		WriteQMSLog(nJig, nCellPos);
	//		GetLogCtrl()->AddBuf(LOG_CIM, "WriteAmtDataFile - bQmsWriteAmt(FALSE)");
	//	}
	//}
	//else
	//{
	//	GetLogCtrl()->AddBuf(LOG_CIM, "EventCellEndInspection - OFF-LINE");
	//}
}

void CViewCIM::EventEquipmentStatusChange()
{
	if (g_pData->m_sEqstMatch == E_EQST_MATCH_NONE)
	{
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			m_Automation.TxS6F11(101);
		else
			GetLogCtrl()->AddBuf(LOG_CIM, "EventEquipmentStatusChange - OFF-LINE");
	}
	else
	{
		g_bStatHistoryCheck = TRUE;

		if(SetEqStatus(g_pData->m_sEqstMatch))
		{
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0 && g_pData->m_sEqstMatch!=E_EQST_MATCH_PAUSE)
				m_Automation.TxS6F11(101);
			else
				GetLogCtrl()->AddBuf(LOG_CIM, "EventEquipmentStatusChange - OFF-LINE");
		}

		g_bStatHistoryCheck = FALSE;

	}

	// ViewCIMEqStateDip();
}

void CViewCIM::EventLossCode()		//20141223 sms 
{
	g_pData->m_sOldEqstMatch = g_pData->m_sEqstMatch;
	g_pData->m_sEqstMatch = E_EQST_MATCH_PAUSE;
	SetEqStatus(g_pData->m_sEqstMatch);
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F11(606);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventEquipmentLossCode - OFF-LINE");
}

void CViewCIM::EventLogIn_Out()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F11(607);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventEquipmentLossCode - OFF-LINE");
}

BOOL CViewCIM::SetEqStatus(short sEqst)
{
	CString str;
	BOOL bEqstCheck[E_EQST_MAX];

	for (int i=0; i<E_EQST_MAX; i++)
	{
		bEqstCheck[i] = FALSE;
	}




	switch (sEqst)
	{
	case E_EQST_MATCH_PAUSE :
		if(EquipmentStatusCheck(E_MAINT, EQST_MAINT_OFF))
		{
			EquipmentStatusChange(E_MAINT, EQST_MAINT_OFF);
			bEqstCheck[E_MAINT] = TRUE;
		}
		if(EquipmentStatusCheck(E_AVAILABILITY, EQST_AVAILABILITY_UP))
		{
			EquipmentStatusChange(E_AVAILABILITY, EQST_AVAILABILITY_UP);
			bEqstCheck[E_AVAILABILITY] = TRUE;
		}
		if(EquipmentStatusCheck(E_MOVE, EQST_MOVE_PAUSE))
		{
			EquipmentStatusChange(E_MOVE, EQST_MOVE_PAUSE);	//Move Status(Pause)
			bEqstCheck[E_MOVE] = TRUE;
		}

		str = "PAUSE";
		break;

	case E_EQST_MATCH_EXECUTING :
		// 			if(EquipmentStatusCheck(E_MOVE, EQST_OFF))
		// 				EquipmentStatusChange(E_MOVE, EQST_OFF);	//Move Status(Running)
		// 			if(EquipmentStatusCheck(E_INTERLOCK, EQST_OFF))
		// 				EquipmentStatusChange(E_INTERLOCK, EQST_OFF);	//INTERLOCK
		if(EquipmentStatusCheck(E_MAINT, EQST_MAINT_OFF))
		{
			EquipmentStatusChange(E_MAINT, EQST_MAINT_OFF);
			bEqstCheck[E_MAINT] = TRUE;
		}
		if(EquipmentStatusCheck(E_AVAILABILITY, EQST_AVAILABILITY_UP))
		{
			EquipmentStatusChange(E_AVAILABILITY, EQST_AVAILABILITY_UP);
			bEqstCheck[E_AVAILABILITY] = TRUE;
		}
		if (EquipmentStatusCheck(E_MOVE, EQST_MOVE_RUNNING) || EquipmentStatusCheck(E_INTERLOCK, EQST_INTERLOCK_OFF))
		{
			EquipmentStatusChange(E_MOVE_N_INTERLOCK, EQST_MOVE_RUNNING);
			bEqstCheck[E_MOVE_N_INTERLOCK] = TRUE;
		}
		str = "EXECUTING";
		break;

	case E_EQST_MATCH_FAULT :
		// 			EquipmentStatusCheck(E_MOVE, EQST_OFF))
		// 				EquipmentStatusChange(E_MOVE, EQST_OFF);	//Move Status(Pause)
		// 			if(EquipmentStatusCheck(E_AVAILABILITY, EQST_OFF))
		// 				EquipmentStatusChange(E_AVAILABILITY, EQST_OFF);	//Availability(Down)

		if (EquipmentStatusCheck(E_MOVE, EQST_MOVE_PAUSE) || EquipmentStatusCheck(E_AVAILABILITY, EQST_AVAILABILITY_DOWN))
		{
			EquipmentStatusChange(E_MOVE_N_AVAILABILITY, EQST_MOVE_PAUSE);
			bEqstCheck[E_MOVE_N_AVAILABILITY] = TRUE;
		}

		str = "FAULT";

		break;

	case E_EQST_MATCH_INTERLOCK:

		if (EquipmentStatusCheck(E_MOVE, EQST_MOVE_PAUSE) 
			|| EquipmentStatusCheck(E_INTERLOCK, EQST_INTERLOCK_ON))
		{
			EquipmentStatusChange(E_MOVE_N_INTERLOCK, EQST_MOVE_PAUSE);
			bEqstCheck[E_MOVE_N_INTERLOCK] = TRUE;
		}
// 
// 		if(EquipmentStatusCheck(E_INTERLOCK, EQST_INTERLOCK_ON))
// 		{
// 			EquipmentStatusChange(E_INTERLOCK, EQST_INTERLOCK_ON);	//INTERLOCK
// 			bEqstCheck[E_INTERLOCK] = TRUE;
// 		}
// 
// 		if (EquipmentStatusCheck(E_MOVE, EQST_MOVE_PAUSE))
// 		{
// 			EquipmentStatusChange(E_MOVE, EQST_MOVE_PAUSE);
// 			bEqstCheck[E_MOVE] = TRUE;
// 		}
		str = "INTERLOCK";
		break;

	case E_EQST_MATCH_CELL_EXIST:
		if(EquipmentStatusCheck(E_RUN, EQST_RUN_IS_CELL))
		{
			EquipmentStatusChange(E_RUN, EQST_RUN_IS_CELL);
			bEqstCheck[E_RUN] = TRUE;
		}
		break;

	case E_EQST_MATCH_CELL_NOT_EXIST:
		if(EquipmentStatusCheck(E_RUN, EQST_RUN_NOT_CELL))
		{
			EquipmentStatusChange(E_RUN, EQST_RUN_NOT_CELL);
			bEqstCheck[E_RUN] = TRUE;
		}
		break;

	case E_EQST_MATCH_NONE:
		break;

	default:
		break;
	}

	for (int i=0; i<E_EQST_MAX; i++)
	{
		if(bEqstCheck[i])
			return TRUE;
	}

	return FALSE;

};

BOOL CViewCIM::EquipmentStatusCheck(E_EQST eEqst, CString cOn_Off)
{

	switch (eEqst)
	{
	case E_MAINT :
		if( (strcmp(g_pData->m_cMaint, cOn_Off)!=0) && g_bStatHistoryCheck )
		{
			strcpy_s(g_pData->m_cOldMaint, g_pData->m_cMaint);
			strcpy_s(g_pData->m_cOldAvilability, g_pData->m_cAvilability);
			strcpy_s(g_pData->m_cOldInterlock, g_pData->m_cInterlock);
			strcpy_s(g_pData->m_cOldMove, g_pData->m_cMove);
			strcpy_s(g_pData->m_cOldRun, g_pData->m_cRun);
			strcpy_s(g_pData->m_cOldFront, g_pData->m_cFront);
			strcpy_s(g_pData->m_cOldRear, g_pData->m_cRear);
			strcpy_s(g_pData->m_cOldSampleLot, g_pData->m_cSampleLot);
			g_bStatHistoryCheck = FALSE;
			return TRUE;
		}
		break;

	case E_AVAILABILITY :
		if( (strcmp(g_pData->m_cAvilability, cOn_Off)!=0) && g_bStatHistoryCheck )
		{
			strcpy_s(g_pData->m_cOldMaint, g_pData->m_cMaint);
			strcpy_s(g_pData->m_cOldAvilability, g_pData->m_cAvilability);
			strcpy_s(g_pData->m_cOldInterlock, g_pData->m_cInterlock);
			strcpy_s(g_pData->m_cOldMove, g_pData->m_cMove);
			strcpy_s(g_pData->m_cOldRun, g_pData->m_cRun);
			strcpy_s(g_pData->m_cOldFront, g_pData->m_cFront);
			strcpy_s(g_pData->m_cOldRear, g_pData->m_cRear);
			strcpy_s(g_pData->m_cOldSampleLot, g_pData->m_cSampleLot);
			g_bStatHistoryCheck = FALSE;
			return TRUE;
		}
		break;

	case E_INTERLOCK :
		if( (strcmp(g_pData->m_cInterlock, cOn_Off)!=0)&& g_bStatHistoryCheck )
		{
			strcpy_s(g_pData->m_cOldMaint, g_pData->m_cMaint);
			strcpy_s(g_pData->m_cOldAvilability, g_pData->m_cAvilability);
			strcpy_s(g_pData->m_cOldInterlock, g_pData->m_cInterlock);
			strcpy_s(g_pData->m_cOldMove, g_pData->m_cMove);
			strcpy_s(g_pData->m_cOldRun, g_pData->m_cRun);
			strcpy_s(g_pData->m_cOldFront, g_pData->m_cFront);
			strcpy_s(g_pData->m_cOldRear, g_pData->m_cRear);
			strcpy_s(g_pData->m_cOldSampleLot, g_pData->m_cSampleLot);
			g_bStatHistoryCheck = FALSE;
			return TRUE;
		}
		break;

	case E_MOVE :
		if( strcmp(g_pData->m_cMove, cOn_Off)!=0 ) //sms 조건문 수정 if( (strcmp(g_pData->m_cMove, cOn_Off)!=0)&& g_bStatHistoryCheck )
		{
			strcpy_s(g_pData->m_cOldMaint, g_pData->m_cMaint);
			if( g_bStatHistoryCheck )	//sms 조건문 추가
			{
				strcpy_s(g_pData->m_cOldAvilability, g_pData->m_cAvilability);
			}

			strcpy_s(g_pData->m_cOldInterlock, g_pData->m_cInterlock);
			strcpy_s(g_pData->m_cOldMove, g_pData->m_cMove);
			strcpy_s(g_pData->m_cOldRun, g_pData->m_cRun);
			strcpy_s(g_pData->m_cOldFront, g_pData->m_cFront);
			strcpy_s(g_pData->m_cOldRear, g_pData->m_cRear);
			strcpy_s(g_pData->m_cOldSampleLot, g_pData->m_cSampleLot);
			g_bStatHistoryCheck = FALSE;
			return TRUE;
		}
		break;

	case E_RUN :
		if( (strcmp(g_pData->m_cRun, cOn_Off)!=0)&& g_bStatHistoryCheck )
		{
			strcpy_s(g_pData->m_cOldMaint, g_pData->m_cMaint);
			strcpy_s(g_pData->m_cOldAvilability, g_pData->m_cAvilability);
			strcpy_s(g_pData->m_cOldInterlock, g_pData->m_cInterlock);
			strcpy_s(g_pData->m_cOldMove, g_pData->m_cMove);
			strcpy_s(g_pData->m_cOldRun, g_pData->m_cRun);
			strcpy_s(g_pData->m_cOldFront, g_pData->m_cFront);
			strcpy_s(g_pData->m_cOldRear, g_pData->m_cRear);
			strcpy_s(g_pData->m_cOldSampleLot, g_pData->m_cSampleLot);
			g_bStatHistoryCheck = FALSE;
			return TRUE;
		}
		break;
	case E_FRONT :
		if( (strcmp(g_pData->m_cRun, cOn_Off)!=0)&& g_bStatHistoryCheck )
		{
			strcpy_s(g_pData->m_cOldMaint, g_pData->m_cMaint);
			strcpy_s(g_pData->m_cOldAvilability, g_pData->m_cAvilability);
			strcpy_s(g_pData->m_cOldInterlock, g_pData->m_cInterlock);
			strcpy_s(g_pData->m_cOldMove, g_pData->m_cMove);
			strcpy_s(g_pData->m_cOldRun, g_pData->m_cRun);
			strcpy_s(g_pData->m_cOldFront, g_pData->m_cFront);
			strcpy_s(g_pData->m_cOldRear, g_pData->m_cRear);
			strcpy_s(g_pData->m_cOldSampleLot, g_pData->m_cSampleLot);
			g_bStatHistoryCheck = FALSE;
			return TRUE;
		}
		break;
	case E_REAR :
		if( (strcmp(g_pData->m_cRun, cOn_Off)!=0)&& g_bStatHistoryCheck )
		{
			strcpy_s(g_pData->m_cOldMaint, g_pData->m_cMaint);
			strcpy_s(g_pData->m_cOldAvilability, g_pData->m_cAvilability);
			strcpy_s(g_pData->m_cOldInterlock, g_pData->m_cInterlock);
			strcpy_s(g_pData->m_cOldMove, g_pData->m_cMove);
			strcpy_s(g_pData->m_cOldRun, g_pData->m_cRun);
			strcpy_s(g_pData->m_cOldFront, g_pData->m_cFront);
			strcpy_s(g_pData->m_cOldRear, g_pData->m_cRear);
			strcpy_s(g_pData->m_cOldSampleLot, g_pData->m_cSampleLot);
			g_bStatHistoryCheck = FALSE;
			return TRUE;
		}
		break;
	case E_SAMPLE :
		if( (strcmp(g_pData->m_cSampleLot, cOn_Off)!=0)&& g_bStatHistoryCheck )
		{
			strcpy_s(g_pData->m_cOldMaint, g_pData->m_cMaint);
			strcpy_s(g_pData->m_cOldAvilability, g_pData->m_cAvilability);
			strcpy_s(g_pData->m_cOldInterlock, g_pData->m_cInterlock);
			strcpy_s(g_pData->m_cOldMove, g_pData->m_cMove);
			strcpy_s(g_pData->m_cOldRun, g_pData->m_cRun);
			strcpy_s(g_pData->m_cOldFront, g_pData->m_cFront);
			strcpy_s(g_pData->m_cOldRear, g_pData->m_cRear);
			strcpy_s(g_pData->m_cOldSampleLot, g_pData->m_cSampleLot);
			g_bStatHistoryCheck = FALSE;
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void CViewCIM::EquipmentStatusChange(E_EQST eEqst, CString cOn_Off)
{
	CString	str;

	switch(eEqst)
	{
	case E_MAINT		:	strcpy_s(g_pData->m_cMaint			, cOn_Off);	str="MAINT";		break;
	case E_AVAILABILITY :	strcpy_s(g_pData->m_cAvilability	, cOn_Off);	str="AVAILABILITY"; break;	//Availability(Equipment Status), 0:Down, 1:Up
	case E_INTERLOCK	:	strcpy_s(g_pData->m_cInterlock		, cOn_Off);	str="INTERLOCK";	break;	//Interlock(Equipment Status), 0:I/L On, 1:I/L Off
	case E_MOVE			:	strcpy_s(g_pData->m_cMove			, cOn_Off);	str="MOVE";			break;	//Move Status(Equipment Status), 0:Pause, 1:Running
	case E_RUN			:	strcpy_s(g_pData->m_cRun			, cOn_Off);	str="RUN";			break;	//Run Status(Equipment Status), 0:Idle, 1:Run
	case E_FRONT		:	strcpy_s(g_pData->m_cFront			, cOn_Off);	str="FRONT";		break;
	case E_REAR			:	strcpy_s(g_pData->m_cRear			, cOn_Off);	str="REAR";			break;
	case E_SAMPLE		:	strcpy_s(g_pData->m_cSampleLot		, cOn_Off);	str="SAMPLE";		break;

	case EQST_INIT		: 
		{	
			strcpy_s(g_pData->m_cMaint			,  EQST_MAINT_OFF);
			strcpy_s(g_pData->m_cAvilability	,  EQST_AVAILABILITY_UP);
			strcpy_s(g_pData->m_cInterlock		,  EQST_INTERLOCK_OFF);
			strcpy_s(g_pData->m_cMove			,  EQST_MOVE_PAUSE);
			strcpy_s(g_pData->m_cRun			,  EQST_RUN_NOT_CELL);

			strcpy_s(g_pData->m_cFront			, EQST_FRONT_DOWN);
			strcpy_s(g_pData->m_cRear			, EQST_REAR_DOWN);	
			strcpy_s(g_pData->m_cSampleLot		, EQST_SAMPLE_OFF);

			str="INIT";
			break;
		}
		
	case E_MOVE_N_INTERLOCK : 
		{
			strcpy_s(g_pData->m_cMove,  cOn_Off);
			strcpy_s(g_pData->m_cInterlock, cOn_Off);
			str="MOVE n INTERLOCK";
			break;
		}

	case E_MOVE_N_AVAILABILITY :
		{
			strcpy_s(g_pData->m_cMove,  cOn_Off);
			strcpy_s(g_pData->m_cAvilability, cOn_Off);
			str="MOVE n AVAILABILITY";
			break;
		}
	default :
		return ;
	}

	GetLogCtrl()->AddBuf(LOG_CIM, "Equipment status changed (%s) - State(%s)", str, cOn_Off);

}

void CViewCIM::EventPPIDCreate()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS7F107();
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventPPIDCreate - OFF-LINE");
}

void CViewCIM::EventPPIDDelete()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS7F107();
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventPPIDDelete - OFF-LINE");
}

void CViewCIM::EventPPIDModify()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS7F107();
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventPPIDModify - OFF-LINE");
}

void CViewCIM::EventPPIDChange()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F11(107);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventPPIDChange - OFF-LINE");
}

void CViewCIM::EventPPIDOperation()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS7F107();
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventPPIDOperation - OFF-LINE");
};

void CViewCIM::EventMaterialChange()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F11(200);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventMaterialChange - OFF-LINE");
}

void CViewCIM::EventOPCall()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F11(501);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventOPCall - OFF-LINE");
}

void CViewCIM::EventInterlock()
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F11(502);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventInterlock - OFF-LINE");
}

void CViewCIM::EventCellInfoRequest()
{
	CString strCellID;

	strCellID = g_pData->m_CellInformation.GetLastCellID();
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F207(strCellID);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventCellInfoRequest - OFF-LINE");
}

// TPR
// void CViewCIM::EventRepairInfoRequest()
// {
// 	CString strCellID;
// 
// 	strCellID = g_pData->m_cRepairInfoReq;
// 	//if(strcmp(g_pData->m_cCRST, "0")!=0)
// 	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
// 		m_Automation.TxS6F205(strCellID);
// 	else
// 		GetLogCtrl()->AddBuf(LOG_CIM, "EventRepairInfoRequest - OFF-LINE");
// }

//
// 2016-01-04, jhLee, POCH 처리를 위한 Cell Lot Information 요청 전송
//
void CViewCIM::EventCellLotInfoRequest(CString cEventCode)
{
	CString strCellID;

	strCellID = g_pData->m_CellLotInformation.GetLastCellID();

	if ( strcmp(g_pData->m_cCRST, "0") !=0 )
	{
		m_Automation.TxS6F205(strCellID, cEventCode);			// Cell ID와 POCH를 전송하여 Cell lot information을 요청한다.
	}
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventCellLotInfoRequest - OFF-LINE");
}


// 2016-11-19, jhLee, S6F11, CEID 609, 주요 검사 완료 시점에 이벤트를 보내도록 한다.
void CViewCIM::EventInspectionResultReport()
{
	if ( strcmp(g_pData->m_cCRST, "0") !=0 )
	{
		//V1 Not Use
		m_Automation.TxS6F11(609, 0, 0); // Event Report Send
	}
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventCellInspectionFinish - OFF-LINE");
}

void CViewCIM::EventCodeInfoRequest()
{
	CString strCodeType;

	strCodeType = g_pData->m_cCodeInfoReq;
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F209(strCodeType);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventCodeInfoRequest - OFF-LINE");
}

void CViewCIM::EventERMS(void)
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.TxS6F11(603);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventERMS - OFF-LINE");
}

void CViewCIM::SetEquipFunctionChange(CString strEFID, CString strEFST, CString strMSG)
{
	if(!GetSockStsDlg()->GetSocketStatus())
		return;

	// 설비로 기능 변경 요청 [11/15/2017 OSC]
	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_EQUIP_FUNCTION_CHANGE_REQ;

	strcpy_s(pData->cMsgData[0], strEFID);
	strcpy_s(pData->cMsgData[1], strEFST);
	strcpy_s(pData->cLongMsgData[0], strMSG);


	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);

	GetLogCtrl()->AddBuf(LOG_CIM, "[SetEquipFunctionChange] - EFID(%s), EFST(%s), Msg(%s), RETURN(%d)"
		, pData->cMsgData[0]
		, pData->cMsgData[1]
		, pData->cMsgData[2]
		, nReturn);

	delete pData;

	return;
}

void CViewCIM::EventEquipFunctionChangeAck(int nAck)
{
	//if(strcmp(g_pData->m_cCRST, "0")!=0)
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		m_Automation.AckS2F42(_T("10"), nAck);
	else
		GetLogCtrl()->AddBuf(LOG_CIM, "EventEquipFunctionChangeAck - OFF-LINE");
}

void CViewCIM::LoadConfig(void)
{

}

void CViewCIM::SaveConfig(void)
{

}

void CViewCIM::ControlStateChange(E_CRST eCRST)
{
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();


	// S6F11 : 장비상태 보고

	switch (eCRST)
	{
	case E_CRST_REMOTE :
		strcpy_s(g_pData->m_cCRST, CRST_REMOTE);

		CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE2, _T("REMOTE"));

//		m_lblControlStatus.put_Caption("REMOTE");
//!		pMainFrm->m_wndMainTitle.m_lblStatus.SetText("REMOTE");

		//m_swcRemote.put_Active(true);
		//m_swcLocal.put_Active(false);
		//m_swcOffline.put_Active(false);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE1, GXCOLOR_ON);			// REMOTE button ON
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE2, GXCOLOR_OFF);			// LOCAL button OFF
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3, GXCOLOR_OFF);			// OFFLINE button OFF



		m_Automation.TxS6F11(106);

		break;

	case E_CRST_LOCAL :
		strcpy_s(g_pData->m_cCRST, CRST_LOCAL);

		CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE2, _T("LOCAL"));
//		m_lblControlStatus.put_Caption("LOCAL");
//!		pMainFrm->m_wndMainTitle.m_lblStatus.SetText("LOCAL");


		//m_swcRemote.put_Active(false);
		//m_swcLocal.put_Active(true);
		//m_swcOffline.put_Active(false);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE1, GXCOLOR_OFF);			// REMOTE button OFF
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE2, GXCOLOR_ON);			// LOCAL button ON
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3, GXCOLOR_OFF);			// OFFLINE button OFF


		m_Automation.TxS6F11(105);

		break;

	case E_CRST_OFFLINE :
		strcpy_s(g_pData->m_cCRST, CRST_OFF);

		CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE2, _T("OFFLILE"));
//		m_lblControlStatus.put_Caption("OFFLINE");
//!		pMainFrm->m_wndMainTitle.m_lblStatus.SetText("OFFLINE");

		//m_swcRemote.put_Active(false);
		//m_swcLocal.put_Active(false);
		//m_swcOffline.put_Active(true);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE1, GXCOLOR_OFF);			// REMOTE button OFF
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE2, GXCOLOR_OFF);			// LOCAL button OFF
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3, GXCOLOR_ON);			// OFFLINE button ON

		m_Automation.TxS6F11(104);
		break;

	default :
		GetLogCtrl()->AddBuf(LOG_CIM,"ControlStateChange() Abnormal Control status : %d", eCRST);

	}


	pMainFrm->m_pViewCIM->SetControlState();
	pMainFrm->m_pViewCIM->SetDataPCVersion();
	pMainFrm->m_pViewCIM->GetLineID();
}

void CViewCIM::AlarmReportSend()
{
	CString str = g_pData->m_cCRST;
	str.Trim(' ');
	
	if (strcmp(str, CRST_REMOTE)==0)
	{
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			m_Automation.TxS5F1(g_pData->m_tAlarm);
		else
			GetLogCtrl()->AddBuf(LOG_CIM, "AlarmReportSend - OFF-LINE");
	}

	else if(strcmp(str, CRST_LOCAL)==0)
	{
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			m_Automation.TxS5F1(g_pData->m_tAlarm);
		else
			GetLogCtrl()->AddBuf(LOG_CIM, "AlarmReportSend - OFF-LINE");
	}
}
//void CViewCIM::OnMouseUpIswitchledxRemote(short Button, short Shift, long X, long Y)
//{
//	// TODO: Add your message handler code here
//	GetLogCtrl()->AddBuf(LOG_OPERATION, "Remote Button Click!" );
//
//	// TODO: Add your message handler code here
//	if ( m_XCom.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "1")!=0)
//	{
//		m_swcRemote.put_Active(false);
//		ControlStateChange(E_CRST_REMOTE);
//	}
//	else if ( m_XCom.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "1")==0)
//	{
//		m_swcRemote.put_Active(true);
//		GetLogCtrl()->AddBuf(LOG_OPERATION, "Remote Button Click! But it is already Remote state" );
//		AfxMessageBox("Already Remote state");
//	}
//	else
//	{
//		m_swcRemote.put_Active(false);
//	}
//}

//void CViewCIM::OnMouseUpIswitchledxRemote2(short Button, short Shift, long X, long Y)
//{
//	// TODO: Add your message handler code here
//	GetLogCtrl()->AddBuf(LOG_OPERATION, "Local Button Click!" );
//
//	// TODO: Add your message handler code here
//	if ( m_XCom.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "2")!=0)
//	{
//		m_swcLocal.put_Active(false);
//		ControlStateChange(E_CRST_LOCAL);
//	}
//	else if ( m_XCom.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "2")==0)
//	{
//		m_swcLocal.put_Active(true);
//		GetLogCtrl()->AddBuf(LOG_OPERATION, "Local Button Click! But it is already Local state" );
//		AfxMessageBox("Already Local state");
//	}
//	else
//	{
//		m_swcLocal.put_Active(false);
//	}
//}

//void CViewCIM::OnMouseUpIswitchledxRemote3(short Button, short Shift, long X, long Y)
//{
//	// TODO: Add your message handler code here
//	GetLogCtrl()->AddBuf(LOG_OPERATION,  "OffLine Button Click!" );
//
// 	m_swcOffline.put_Active(false);
// 	if(strcmp(g_pData->m_cCRST, "0")!=0)	//OFF이 아닐경우
// 		ControlStateChange(E_CRST_OFFLINE);
//
//	if ( m_XCom.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "0")!=0)
//	{
//		m_swcOffline.put_Active(false);
//		ControlStateChange(E_CRST_OFFLINE);
//	}
//	else if ( m_XCom.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "0")==0)
//	{
//		m_swcOffline.put_Active(true);
//		GetLogCtrl()->AddBuf(LOG_OPERATION,  "OffLine Button Click! But it is already Offline state" );
//		AfxMessageBox("Already Offline state");
//	}
//	else
//	{
//		m_swcOffline.put_Active(false);
//	}
//}

//void CViewCIM::OnMouseUpIswitchledxConnect(short Button, short Shift, long X, long Y)
//{
//	// TODO: Add your message handler code here
//	int         nReturn;
//	CString str;
//
//	GetLogCtrl()->AddBuf(LOG_OPERATION, "Connect Button Click!" );
//
//	str = m_XCom.GetParam("HSMS.STATE");
//	if ( str == "SELECTED" )
//	{
//		//  Start the XCom1 control
//		if( (nReturn = m_XCom.Stop()) == 0 ) {
//			GetLogCtrl()->AddBuf(LOG_CIM, "XCom stopped successfully" );
//		}
//		else 
//		{
//			GetLogCtrl()->AddBuf(LOG_CIM,"Fail to stop XCom  ret = %d", nReturn );
//			return;
//		}
//	}
//	else
//	{
//		m_XCom.Stop();
//		m_XCom.Close();
//
//		if( (nReturn = m_XCom.Initialize( CSettingParm::cHsmsConfig )) == 0 ) 
//		{
//			GetLogCtrl()->AddBuf(LOG_CIM, "XCom cfg Path(%s)", CSettingParm::cHsmsConfig);
//			GetLogCtrl()->AddBuf(LOG_CIM, "XCom initialized successfully" );
//			GetLogCtrl()->AddBuf(LOG_CIM, "XCom HSMS IP(%s)", m_XCom.GetParam("IP"));
//			GetLogCtrl()->AddBuf(LOG_CIM, "XCom HSMS Port(%s)", m_XCom.GetParam("Port"));
//
//			// m_lblIP.put_Caption(m_XCom.GetParam("IP"));
//			// m_lblPort.put_Caption(m_XCom.GetParam("Port"));
//			CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE4, m_XCom.GetParam("IP"));
//			CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE5, m_XCom.GetParam("Port"));
//
//
//			Sleep(3000); //2015.03.25 GR.PARK - 운영프로그램이 3초마다 연결 시도 하므로 딜리이를 3초 줌. (임시조치) 종료직전 레시피를 저장하도록 수정 예정
//			if( (nReturn = m_XCom.Start()) == 0 ) 
//			{
//				GetLogCtrl()->AddBuf(LOG_CIM, "XCom started successfully" );
//			}
//			else 
//			{
//				GetLogCtrl()->AddBuf(LOG_CIM,"Fail to start XCom (%d)", nReturn );
//			}
//
//		}
//		else 
//		{
//			AfxMessageBox( "Fail to initialize XCom (%d)", nReturn );
//			GetLogCtrl()->AddBuf(LOG_CIM, "Fail to initialize XCom (%d)", nReturn );
//		}
//	}
//}

int CViewCIM::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CViewCIM::ViewCIMEqStateDip()
{
	//if (strcmp(g_pData->m_cMaint, EQST_MAINT_ON)==0)	{m_LedRectMaintOn.put_Active(TRUE); m_LedRectMaintOff.put_Active(FALSE);}
	//else {m_LedRectMaintOn.put_Active(FALSE); m_LedRectMaintOff.put_Active(TRUE);}

	//if (strcmp(g_pData->m_cAvilability, EQST_AVAILABILITY_UP)==0)	{m_LedRectAvailabilityUp.put_Active(TRUE); m_LedRectAvailabilityDown.put_Active(FALSE);}
	//else {m_LedRectAvailabilityUp.put_Active(FALSE); m_LedRectAvailabilityUp.put_Active(TRUE);}

	//if (strcmp(g_pData->m_cInterlock, EQST_INTERLOCK_ON)==0)	{m_LedRectInterlockOn.put_Active(TRUE); m_LedRectInterlockOff.put_Active(FALSE);}
	//else {m_LedRectInterlockOn.put_Active(FALSE); m_LedRectInterlockOff.put_Active(TRUE);}

	//if (strcmp(g_pData->m_cMove, EQST_MOVE_RUNNING)==0)	{m_LedRectMoveRunning.put_Active(TRUE); m_LedRectMovePause.put_Active(FALSE);}
	//else {m_LedRectMoveRunning.put_Active(FALSE); m_LedRectMovePause.put_Active(TRUE);}

	//if (strcmp(g_pData->m_cRun, EQST_RUN_IS_CELL)==0)	{m_LedRectRun.put_Active(TRUE); m_LedRectIdle.put_Active(FALSE);}
	//else {m_LedRectRun.put_Active(FALSE); m_LedRectIdle.put_Active(TRUE);}
	//
	//if (strcmp(g_pData->m_cFront, EQST_FRONT_DOWN)==0)	{m_LedRectFrontDown.put_Active(TRUE); m_LedRectFrontUp.put_Active(FALSE);}
	//else {m_LedRectFrontDown.put_Active(FALSE); m_LedRectFrontUp.put_Active(TRUE);}

	//if (strcmp(g_pData->m_cRear, EQST_REAR_DOWN)==0)	{m_LedRectRearDown.put_Active(TRUE); m_LedRectRearUp.put_Active(FALSE);}
	//else {m_LedRectRearDown.put_Active(FALSE); m_LedRectRearUp.put_Active(TRUE);}

	//if (strcmp(g_pData->m_cSampleLot, EQST_SAMPLE_ON)==0)	{m_LedRectSampleOn.put_Active(TRUE); m_LedRectSampleOff.put_Active(FALSE);}
	//else {m_LedRectSampleOn.put_Active(FALSE); m_LedRectSampleOff.put_Active(TRUE);}


}

BOOL CViewCIM::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return TRUE;

	return CFormView::PreTranslateMessage(pMsg);
}

void CViewCIM::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
}


//
// 2016-06-03, jhLee, QMS 및 비가동 관련 Log 기록 Thread 삭제
//
//void CViewCIM::CreateThread()
//{
//	m_bWriteDataThreadRun = TRUE;
//	m_pWriteDataThread = AfxBeginThread(thrWriteDataThread, (LPVOID)this);
//	if(m_pWriteDataThread!=NULL)
//		m_hWriteDataThread = m_pWriteDataThread->m_hThread;
//	else
//		AfxMessageBox("!!! Attention !!! \n Cannot created Write Data Thread");
//}
//
//void CViewCIM::DeleteThread()
//{
//	if (m_hWriteDataThread)
//	{
//		m_bWriteDataThreadRun = false;
//
//		DWORD dwExitCode = 0;
//		::GetExitCodeThread(m_hWriteDataThread, &dwExitCode);
//		if(dwExitCode != 0)
//		{
//			if(::WaitForSingleObject(m_hWriteDataThread, 5000) != WAIT_OBJECT_0)
//			{
//				::TerminateThread(m_hWriteDataThread, 0);
//			}
//		}
//	}
//}



//
// 2016-06-03, jhLee
// QMS 및 비가동 Log 기록 기능 삭제
//
//
//UINT CViewCIM::thrWriteDataThread(LPVOID pParam)
//{
//	CViewCIM *pOwner = (CViewCIM*)pParam;
//	pOwner->m_bWriteDataThreadRun = TRUE;
//	char cUserLog[STR_DEF_LEN];
//	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
//	tag_TestResult* pResult;
//
//	while ( pOwner->m_bWriteDataThreadRun )
//	{
//		for (int i=0; i<MAX_JIG_CNT; i++)
//		{
//			for (int j=0; j<MAX_JIG_CELL_CNT; j++)
//			{
//				if (pOwner->bWriteDataEvent[i][j])
//				{	
//					sprintf_s(cUserLog, "WriteAmtDataFile - Begin CELL ID( %s )", g_pData->m_Jig[i].tCellData[j].m_cCellID);
//					::SendMessage(GetSockStsDlg()->m_hWnd, WM_USER_MSG, (WPARAM)LOG_CIM, (LPARAM)&cUserLog);
//					
//					//if (!pMainFrm->CheckYmsNetworkDriverConnection())
//					if (!pMainFrm->CheckQMSNetworkDriverConnection())
//					{
//						//2015-09-21 JH-PARK DELETE
//// 						theApp.m_InsData.SetYmsRtn(i, j, E_CELL, E_YMS_FAIL);
//// 						theApp.m_InsData.SetYmsRtn(i, j, E_MTPR, E_YMS_FAIL);
//// 						theApp.m_InsData.SetYmsRtn(i, j, E_MTPW, E_YMS_FAIL);
//// 						theApp.m_InsData.SetYmsRtn(i, j, E_TSP, E_YMS_FAIL);
//
//						theApp.m_InsData.SetQMSRtn(i, j, E_CELL, E_QMS_FAIL);
//						theApp.m_InsData.SetQMSRtn(i, j, E_MTPR, E_QMS_FAIL);
//						theApp.m_InsData.SetQMSRtn(i, j, E_MTPW, E_QMS_FAIL);
//						theApp.m_InsData.SetQMSRtn(i, j, E_TSP, E_QMS_FAIL);
//					}
//					else if (WriteAmtDataFile(i, j))	//Thread 처리.
//					{
//						pResult = &g_pData->m_Jig[i].tCellData[j].m_tTestResult;
//
//						//2015-09-21 JH-PARK DELETE
//// 						if (!CSettingParm::bMtpWUsed && theApp.m_InsData.GetYmsRtn(i, j, E_MTPW)!=E_YMS_SKIP)
//// 							theApp.m_InsData.SetYmsRtn(i, j, E_MTPW, E_YMS_SKIP);
//// 						if (!CSettingParm::bMtpRUsed && theApp.m_InsData.GetYmsRtn(i, j, E_MTPR)!=E_YMS_SKIP)
//// 							theApp.m_InsData.SetYmsRtn(i, j, E_MTPR, E_YMS_SKIP);
//// 						if (!CSettingParm::bTspUsed && theApp.m_InsData.GetYmsRtn(i, j, E_TSP)!=E_YMS_SKIP)
//// 							theApp.m_InsData.SetYmsRtn(i, j, E_TSP, E_YMS_SKIP);
//// 					
//// 						if (theApp.m_InsData.GetYmsRtn(i, j, E_CELL)==E_YMS_NONE)
//// 							theApp.m_InsData.SetYmsRtn(i, j, E_CELL, E_YMS_SUCESS);
//// 						if (theApp.m_InsData.GetYmsRtn(i, j, E_MTPW)==E_YMS_NONE)
//// 							theApp.m_InsData.SetYmsRtn(i, j, E_MTPW, E_YMS_SUCESS);
//// 						if (theApp.m_InsData.GetYmsRtn(i, j, E_TSP)==E_YMS_NONE)
//// 							theApp.m_InsData.SetYmsRtn(i, j, E_TSP, E_YMS_SUCESS);
//// 						if (theApp.m_InsData.GetYmsRtn(i, j, E_MTPR)==E_YMS_NONE)
//// 							theApp.m_InsData.SetYmsRtn(i, j, E_MTPR, E_YMS_SUCESS);
//
//						if (!CSettingParm::bMtpWUsed && theApp.m_InsData.GetQMSRtn(i, j, E_MTPW)!=E_QMS_SKIP)
//							theApp.m_InsData.SetQMSRtn(i, j, E_MTPW, E_QMS_SKIP);
//						if (!CSettingParm::bMtpRUsed && theApp.m_InsData.GetQMSRtn(i, j, E_MTPR)!=E_QMS_SKIP)
//							theApp.m_InsData.SetQMSRtn(i, j, E_MTPR, E_QMS_SKIP);
//						if (!CSettingParm::bTspUsed && theApp.m_InsData.GetQMSRtn(i, j, E_TSP)!=E_QMS_SKIP)
//							theApp.m_InsData.SetQMSRtn(i, j, E_TSP, E_QMS_SKIP);
//
//						if (theApp.m_InsData.GetQMSRtn(i, j, E_CELL)==E_QMS_NONE)
//							theApp.m_InsData.SetQMSRtn(i, j, E_CELL, E_QMS_SUCESS);
//						if (theApp.m_InsData.GetQMSRtn(i, j, E_MTPW)==E_QMS_NONE)
//							theApp.m_InsData.SetQMSRtn(i, j, E_MTPW, E_QMS_SUCESS);
//						if (theApp.m_InsData.GetQMSRtn(i, j, E_TSP)==E_QMS_NONE)
//							theApp.m_InsData.SetQMSRtn(i, j, E_TSP, E_QMS_SUCESS);
//						if (theApp.m_InsData.GetQMSRtn(i, j, E_MTPR)==E_QMS_NONE)
//							theApp.m_InsData.SetQMSRtn(i, j, E_MTPR, E_QMS_SUCESS);
//	
//						sprintf_s(cUserLog, "WriteAmtDataFile - Pass CELL ID( %s )",g_pData->m_Jig[i].tCellData[j].m_cCellID);
//						::SendMessage(GetSockStsDlg()->m_hWnd, WM_USER_MSG, (WPARAM)LOG_CIM, (LPARAM)&cUserLog);
//					}
//					else
//					{
//						//2015-09-21 JH-PARK DELETE
//// 						theApp.m_InsData.SetYmsRtn(i, j, E_CELL, E_YMS_FAIL);
//// 						theApp.m_InsData.SetYmsRtn(i, j, E_MTPR, E_YMS_FAIL);
//// 						theApp.m_InsData.SetYmsRtn(i, j, E_MTPW, E_YMS_FAIL);
//// 						theApp.m_InsData.SetYmsRtn(i, j, E_TSP, E_YMS_FAIL);
//
//						theApp.m_InsData.SetQMSRtn(i, j, E_CELL, E_QMS_FAIL);
//						theApp.m_InsData.SetQMSRtn(i, j, E_MTPR, E_QMS_FAIL);
//						theApp.m_InsData.SetQMSRtn(i, j, E_MTPW, E_QMS_FAIL);
//						theApp.m_InsData.SetQMSRtn(i, j, E_TSP, E_QMS_FAIL);
//						
//						sprintf_s(cUserLog, "WriteAmtDataFile - Fail CELL ID( %s )",g_pData->m_Jig[i].tCellData[j].m_cCellID);
//						::SendMessage(GetSockStsDlg()->m_hWnd, WM_USER_MSG, (WPARAM)LOG_CIM, (LPARAM)&cUserLog);
//					}
//					//pOwner->WriteYMSLog(i, j);
//					pOwner->WriteQMSLog(i, j);
//					pOwner->bWriteDataEvent[i][j] = FALSE;
//				}
//			}
//		}
//
//		Sleep(100);
//	}
//
//	return 0;
//}

//void CViewCIM::WriteQMSLog(UINT nJig, UINT nCellPos)
//{
//	CString strTemp;
//	//CString strCellYmsRtn, strMtpWYmsRtn, strTspYmsRtn, strMtpRYmsRtn; 
//	CString strCellQMSRtn, strMtpWQMSRtn, strTspQMSRtn, strMtpRQMSRtn; 
//	char cFullPath[512];
//	char cLineName[256];
//	CTime TodayTime,YesterDayTime;						
//	TodayTime = CTime::GetCurrentTime();
//
//	CTimeSpan TimeSpan(1,0,0,0);
//	YesterDayTime =TodayTime - TimeSpan;	
//
//	tag_CellData* pCellData = &g_pData->m_Jig[nJig].tCellData[nCellPos];
//	tag_TestResult* pResult = &g_pData->m_Jig[nJig].tCellData[nCellPos].m_tTestResult;
//
//	if(strlen(g_pData->m_cLineID)<=0)
//	{
//		strcpy_s(cLineName, "NoLineID");
//	}
//	else
//	{
//		strcpy_s(cLineName, g_pData->m_cLineID);
//	}
//	
//// 	strCellYmsRtn = GetYmsReturnString(pResult->m_eCellYmsRtn);
//// 	strMtpWYmsRtn = GetYmsReturnString(pResult->m_eMtpWYmsRtn);
//// 	strTspYmsRtn = GetYmsReturnString(pResult->m_eTspYmsRtn);
//// 	strMtpRYmsRtn = GetYmsReturnString(pResult->m_eMtpRYmsRtn);
//
//	strCellQMSRtn = GetQMSReturnString(pResult->m_eCellQMSRtn);
//	strMtpWQMSRtn = GetQMSReturnString(pResult->m_eMtpWQMSRtn);
//	strTspQMSRtn = GetQMSReturnString(pResult->m_eTspQMSRtn);
//	strMtpRQMSRtn = GetQMSReturnString(pResult->m_eMtpRQMSRtn);
//
//	strcpy_s(cLineName, g_pData->m_cLineID);
//	memset(cFullPath, NULL, sizeof(cFullPath));	
//	sprintf_s(cFullPath, "D:\\SystemUpload\\");
//
//	if(GetFileAttributes(cFullPath) == -1)		::CreateDirectory(cFullPath,NULL);
//
//	if(TodayTime.GetHour() >= 7)
//	{
//		sprintf_s(cFullPath, "D:\\SystemUpload\\%04d%02d%02d_SystemUpload_%s.csv", TodayTime.GetYear(),TodayTime.GetMonth(),TodayTime.GetDay(),cLineName);
//	}
//	else if (TodayTime.GetHour() < 7)
//	{
//		sprintf_s(cFullPath, "D:\\SystemUpload\\%04d%02d%02d_SystemUpload_%s.csv", YesterDayTime.GetYear(),YesterDayTime.GetMonth(),YesterDayTime.GetDay(),cLineName);
//	}
//	
//	if(GetFileAttributes(cFullPath) == -1)
//	{
//		strTemp = 
//			"TIME,CELL ID,SYSTEM DEFECT CODE,CIM_LOG,CIM_MTP,CIM_TSP"; 		
//	}
//	else	strTemp = "";	
//
//	FILE *file;
//
//	fopen_s(&file, cFullPath, "a+");
//
//	if(file == NULL)	return;
//	fseek(file, 0, SEEK_END);
//
//	if(strTemp != "")		fprintf(file,"%s\n",strTemp);
//	else					fprintf(file, "\n");
//
//
//	SYSTEMTIME sTime;
//	GetLocalTime(&sTime);
//
//	strTemp.Format("%u-%02u-%02u %u:%02u:%02u:%03u",sTime.wYear,sTime.wMonth,sTime.wDay,sTime.wHour,sTime.wMinute,sTime.wSecond,sTime.wMilliseconds);
//	fprintf(file,"%s,", strTemp);	
//
//	//fprintf(file, "%s,%s,%s,%s,%s",pCellData->m_cCellID,pCellData->m_cReasonCode, strCellYmsRtn, strMtpRYmsRtn, strTspYmsRtn);
//	fprintf(file, "%s,%s,%s,%s,%s",pCellData->m_cCellID,pCellData->m_cReasonCode, strCellQMSRtn, strMtpRQMSRtn, strTspQMSRtn);
//
//
//	fclose(file);		
//}



//
//void CViewCIM::WriteDownLog()
//{
//	CString strTemp;
//
//	char cFullPath[512];
//	char cLineName[255];
//	CTime TodayTime,YesterDayTime;						
//	TodayTime = CTime::GetCurrentTime();
//
//	CTimeSpan TimeSpan(1,0,0,0);
//	YesterDayTime =TodayTime - TimeSpan;	
//
//	if(strlen(g_pData->m_cLineID)<=0)
//	{
//		strcpy_s(cLineName, "NoLineID");
//	}
//	else
//	{
//		strcpy_s(cLineName, g_pData->m_cLineID);
//	}
//	memset(cFullPath, NULL, sizeof(cFullPath));	
//	sprintf_s(cFullPath, "D:\\MES_DownLog\\");
//
//	if(GetFileAttributes(cFullPath) == -1)		::CreateDirectory(cFullPath, NULL);
//
//	if(TodayTime.GetHour() >= 7)
//	{
//		sprintf_s(cFullPath, "D:\\MES_DownLog\\%04d%02d%02d_DownLog_%s.csv", TodayTime.GetYear(),TodayTime.GetMonth(),TodayTime.GetDay(),cLineName);
//	}
//	else if (TodayTime.GetHour() < 7)
//	{
//		sprintf_s(cFullPath, "D:\\MES_DownLog\\%04d%02d%02d_DownLog_%s.csv", YesterDayTime.GetYear(),YesterDayTime.GetMonth(),YesterDayTime.GetDay(),cLineName);
//	}
//
//	if(GetFileAttributes(cFullPath) == -1)
//	{
//		strTemp = 
//			"START TIME,END TIME,ALARM NAME"; 
//	}
//	else	strTemp = "";	
//
//	FILE *file;
//
//	fopen_s(&file, cFullPath, "a+");
//
//	if(file == NULL)	return;
//	fseek(file, 0, SEEK_END);
//
//	if(strTemp != "")		fprintf(file,"%s\n",strTemp);
//	else					fprintf(file, "\n");
//
//
//	//StartTime
//	strTemp.Format("%u-%02u-%02u %u:%02u:%02u:%03u",
//		g_pData->m_tAlarmInsert.m_systmTimeSet.wYear,g_pData->m_tAlarmInsert.m_systmTimeSet.wMonth,
//		g_pData->m_tAlarmInsert.m_systmTimeSet.wDay,g_pData->m_tAlarmInsert.m_systmTimeSet.wHour,
//		g_pData->m_tAlarmInsert.m_systmTimeSet.wMinute,g_pData->m_tAlarmInsert.m_systmTimeSet.wSecond,
//		g_pData->m_tAlarmInsert.m_systmTimeSet.wMilliseconds);
//	fprintf(file,"%s,", strTemp);
//
//	//EndTime
//	strTemp.Format("%u-%02u-%02u %u:%02u:%02u:%03u",
//		g_pData->m_tAlarmInsert.m_systmTimeRun.wYear,g_pData->m_tAlarmInsert.m_systmTimeRun.wMonth,
//		g_pData->m_tAlarmInsert.m_systmTimeRun.wDay,g_pData->m_tAlarmInsert.m_systmTimeRun.wHour,
//		g_pData->m_tAlarmInsert.m_systmTimeRun.wMinute,g_pData->m_tAlarmInsert.m_systmTimeRun.wSecond,
//		g_pData->m_tAlarmInsert.m_systmTimeRun.wMilliseconds);
//	fprintf(file,"%s,", strTemp);
//	//AlarmName
//	fprintf(file,"%s", g_pData->m_tAlarmInsert.m_cALTX);	
//	fclose(file);	
//}
//
//void CViewCIM::WriteStateChangeLog()
//{
//	CString strTemp;
//
//	char cFullPath[512];
//	char cLineName[255];
//	CTime TodayTime,YesterDayTime;						
//	TodayTime = CTime::GetCurrentTime();
//
//	CTimeSpan TimeSpan(1,0,0,0);
//	YesterDayTime =TodayTime - TimeSpan;	
//
//	if(strlen(g_pData->m_cLineID)<=0)
//	{
//		strcpy_s(cLineName, "NoLineID");
//	}
//	else
//	{
//		strcpy_s(cLineName, g_pData->m_cLineID);
//	}
//
//	memset(cFullPath, NULL, sizeof(cFullPath));	
//	sprintf_s(cFullPath, "D:\\Machine_State\\");
//
//	if(GetFileAttributes(cFullPath) == -1)		::CreateDirectory(cFullPath,NULL);
//
//	if(TodayTime.GetHour() >= 7)
//	{
//		sprintf_s(cFullPath, "D:\\Machine_State\\%04d%02d%02d_StateLog_%s.csv", TodayTime.GetYear(),TodayTime.GetMonth(),TodayTime.GetDay(),cLineName);
//	}
//	else if (TodayTime.GetHour() < 7)
//	{
//		sprintf_s(cFullPath, "D:\\Machine_State\\%04d%02d%02d_StateLog_%s.csv", YesterDayTime.GetYear(),YesterDayTime.GetMonth(),YesterDayTime.GetDay(),cLineName);
//	}
//
//	if(GetFileAttributes(cFullPath) == -1)
//	{
//		strTemp = 
//			"START TIME,END TIME"; 
//	}
//	else	strTemp = "";	
//
//	FILE *file;
//
//	fopen_s(&file, cFullPath, "a+");
//
//	if(file == NULL)	return;
//	fseek(file, 0, SEEK_END);
//
//	if(strTemp != "")		fprintf(file,"%s\n",strTemp);
//	else					fprintf(file, "\n");
//
//
//	//StartTime
//	strTemp.Format("%u-%02u-%02u %u:%02u:%02u:%03u",
//		g_pData->m_tLossTime.m_systmTimeStop.wYear,g_pData->m_tLossTime.m_systmTimeStop.wMonth,
//		g_pData->m_tLossTime.m_systmTimeStop.wDay,g_pData->m_tLossTime.m_systmTimeStop.wHour,
//		g_pData->m_tLossTime.m_systmTimeStop.wMinute,g_pData->m_tLossTime.m_systmTimeStop.wSecond,
//		g_pData->m_tLossTime.m_systmTimeStop.wMilliseconds);
//	fprintf(file,"%s,", strTemp);
//
//	//EndTime
//	strTemp.Format("%u-%02u-%02u %u:%02u:%02u:%03u",
//		g_pData->m_tLossTime.m_systmTimeRun.wYear,g_pData->m_tLossTime.m_systmTimeRun.wMonth,
//		g_pData->m_tLossTime.m_systmTimeRun.wDay,g_pData->m_tLossTime.m_systmTimeRun.wHour,
//		g_pData->m_tLossTime.m_systmTimeRun.wMinute,g_pData->m_tLossTime.m_systmTimeRun.wSecond,
//		g_pData->m_tLossTime.m_systmTimeRun.wMilliseconds);
//	fprintf(file,"%s", strTemp);
//
//	fclose(file);	
//}
//
//









// CString CViewCIM::GetYmsReturnString(E_YMS_RETURN eYmsRtn)
// {
// 	CString strRtn;
// 
// 	switch (eYmsRtn)
// 	{
// 	case E_YMS_NONE:
// 		strRtn = "DOWN";
// 		break;
// 
// 	case E_YMS_SKIP:
// 		strRtn = "SKIP";
// 		break;
// 
// 	case E_YMS_SUCESS:
// 		strRtn = "SUCESS";
// 		break;
// 
// 	case E_YMS_FAIL:
// 		strRtn = "FAIL";
// 		break;
// 
// 	case E_YMS_SIGNAL_NOT_CONNECTION:
// 		strRtn = "NOT CONNECTION";
// 		break;
// 
// 	case E_YMS_NO_FILE:
// 		strRtn = "NO FILE";
// 		break;
// 
// 	case E_YMS_ERR_FORMAT:
// 		strRtn = "ERR FORMAT";
// 		break;
// 
// 	case E_YMS_ERR_OPEN:
// 		strRtn = "ERR OPEN";
// 		break;
// 
// 	case E_YMS_ERR_SYSTEM:
// 		strRtn = "ERR SYSTEM";
// 		break;
// 
// 	default:
// 		strRtn = "ERR SYSTEM";
// 		break;
// 	}
// 	
// 	return strRtn;
// }
//
//CString CViewCIM::GetQMSReturnString(E_QMS_RETURN eQMSRtn)
//{
//	CString strRtn;
//
//	switch (eQMSRtn)
//	{
//	case E_QMS_NONE:
//		strRtn = "DOWN";
//		break;
//
//	case E_QMS_SKIP:
//		strRtn = "SKIP";
//		break;
//
//	case E_QMS_SUCESS:
//		strRtn = "SUCESS";
//		break;
//
//	case E_QMS_FAIL:
//		strRtn = "FAIL";
//		break;
//
//	case E_QMS_SIGNAL_NOT_CONNECTION:
//		strRtn = "NOT CONNECTION";
//		break;
//
//	case E_QMS_NO_FILE:
//		strRtn = "NO FILE";
//		break;
//
//	case E_QMS_ERR_FORMAT:
//		strRtn = "ERR FORMAT";
//		break;
//
//	case E_QMS_ERR_OPEN:
//		strRtn = "ERR OPEN";
//		break;
//
//	case E_QMS_ERR_SYSTEM:
//		strRtn = "ERR SYSTEM";
//		break;
//
//	default:
//		strRtn = "ERR SYSTEM";
//		break;
//	}
//
//	return strRtn;
//}
//



BOOL CViewCIM::SetUseLogByMesg(BOOL bOption)
{
	BOOL bReturn = FALSE;
	CString strParamName, strSetVal, strParaVal;
	
	strParamName = "UseLogByMesg";
	strSetVal = bOption ? "true" : "false";

	strParaVal = m_XComPro.GetParam(strParamName);
	if (strParaVal.Compare(strSetVal)!=0)
	{
		if (m_XComPro.SetParam(strParamName, strSetVal)==0)
		{
			strParaVal = m_XComPro.GetParam(strParamName);
			if (strParaVal.Compare(strSetVal)==0)
			{
				bReturn = TRUE;
			}
			else
				bReturn = FALSE;
		}
		else
			bReturn = FALSE;
	}
	else
		bReturn = TRUE;
	
	return bReturn;
}

BOOL CViewCIM::MakeFileUseLogingByMessage()
{
	BOOL bReturn = FALSE;
	CString strFilePath;
	CStdioFile file;
	CFileException	ex;
	enum {log_default=0, log_list, nolog_list, MAX_WRITE_CNT,};
	CString* strWrite;
	strWrite = new CString[MAX_WRITE_CNT];
	strWrite[log_default] = "log_default=true\n";
	strWrite[log_list] = "log_list=0\n";
	strWrite[nolog_list] = "nolog_list=1 S6F1\n";

	strFilePath.Format("%s\\%s", CGaonNuriApp::GetSetupPath(), "UseLogingByMessage.lbm");

	if (!CFileSupport::FileCheck(strFilePath))
	{
		if(!file.Open((LPCTSTR)strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite/* | CFile::shareDenyWrite*/, &ex))
		{
			char szErr[255];
			ex.GetErrorMessage(szErr, 255);
			delete[] strWrite;
			return bReturn;
		}

		if (!CFileSupport::FileCheck(strFilePath))
		{
			delete[] strWrite;
			return bReturn;
		}

		file.SeekToEnd();
		for (int i=0; i<MAX_WRITE_CNT; i++)
		{
			file.WriteString(strWrite[i]);
		}
		file.Close();
		
		bReturn = TRUE;
	}
	else
		bReturn = TRUE;
	
	delete[] strWrite;

	return bReturn;
}

BOOL CViewCIM::SetInitUseLogByMesg()
{
	BOOL bReturn = FALSE;
	CString strFullPath, strSection, strParamName, strSetVal, strParaVal;

	strSection = "Log";
	strParamName = "UseLogByMesg";
	strSetVal = "true";

	strFullPath = CSettingParm::cHsmsConfig;
	CIni ini(strFullPath);
	if (ini.GetString(strSection, strParamName, "").GetLength()>0)
	{
		bReturn = TRUE;
	}
	else
	{
		ini.WriteString(strSection, strParamName, strSetVal);
		if (ini.GetString(strSection, strParamName, "").GetLength()>0)
		{
			bReturn = TRUE;
		}
		else
			bReturn = FALSE;
	}

	return bReturn;
}

BOOL CViewCIM::SetLogByMesgFlagPath()
{
	BOOL bReturn = FALSE;
	CString strFullPath, strSection, strParamName, strSetVal, strParaVal;

	strSection = "Log";
	strParamName = "LogByMesgFlagPath";
	strSetVal = "UseLogingByMessage.lbm";

	strFullPath = CSettingParm::cHsmsConfig;
	CIni ini(strFullPath);
	if (ini.GetString(strSection, strParamName, "").GetLength()>0)
	{
		bReturn = TRUE;
	}
	else
	{
		ini.WriteString(strSection, strParamName, strSetVal);
		if (ini.GetString(strSection, strParamName, "").GetLength()>0)
		{
			bReturn = TRUE;
		}
		else
			bReturn = FALSE;
	}

	return bReturn;
}


// XCom Log 설정 지정
void CViewCIM::SetConfig_XComLog()
{
	if (SetInitUseLogByMesg())	//SetInitUseLogByMesg → SetLogByMesgFlagPath → MakeFileUseLogingByMessage
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "XCom SetInitUseLogByMesg - Pass");

		if (SetLogByMesgFlagPath())
		{
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom SetLogByMesgFlagPath - Pass");

			if (MakeFileUseLogingByMessage())
			{
				GetLogCtrl()->AddBuf(LOG_CIM, "XCom MakeFileUseLogingByMessage - Pass / m_bLogByMesgPass = TRUE");
				m_bLogByMesgPass = TRUE;
			}
			else
				GetLogCtrl()->AddBuf(LOG_CIM, "XCom MakeFileUseLogingByMessage - Fail");
		}
		else
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom SetLogByMesgFlagPath - Fail");
	}
	else
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "XCom SetInitUseLogByMesg - Fail");
	}
}

// XCom 연결 체크
void CViewCIM::SetConnect_XComComm()
{
	long nReturn;

	if( (nReturn = m_XComPro.Initialize( CSettingParm::cHsmsConfig)) == 0 ) 
	{

		GetLogCtrl()->AddBuf(LOG_CIM, "XCom cfg Path(%s)", CSettingParm::cHsmsConfig);
		GetLogCtrl()->AddBuf(LOG_CIM, "XCom initialized successfully" );
		GetLogCtrl()->AddBuf(LOG_CIM, "XCom HSMS IP(%s)", m_XComPro.GetParam("IP"));
		GetLogCtrl()->AddBuf(LOG_CIM, "XCom HSMS Port(%s)", m_XComPro.GetParam("Port"));

		// m_lblIP.put_Caption(m_XCom.GetParam("IP"));
		// m_lblPort.put_Caption(m_XCom.GetParam("Port"));
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE3, m_XComPro.GetParam("IP"));
		CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE4, m_XComPro.GetParam("Port"));

		strcpy_s(CSettingParm::cXComIP, m_XComPro.GetParam("IP"));

		if (m_bLogByMesgPass)
		{
			if (SetUseLogByMesg(TRUE))
			{
				GetLogCtrl()->AddBuf(LOG_CIM, "XCom SetUseLogByMesg - true");
			}
		}
		else
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom Can't SetUseLogByMesg , m_bLogByMesgPass - false");



		//  Start the XCom1 control
		if( (nReturn = m_XComPro.Start()) == 0 ) 
		{
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom started successfully" );
		}
		else 
		{
			GetLogCtrl()->AddBuf(LOG_CIM,"Fail to start XCom (%d)", nReturn );
			GetLogCtrl()->AddBuf(LOG_CIM,"Make sure equipment IP");
		}

	}
	else 
	{
		AfxMessageBox( "Fail to initialize XCom (%d)", nReturn );
		GetLogCtrl()->AddBuf(LOG_CIM, "Fail to initialize XCom (%d)", nReturn );
	}
}

// 프로그램을 최소화 시킨다.
void CViewCIM::OnBnClickedBtnViewCimMinimize()
{
		// 창 최소화 메세지 전송
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);

}

// Remote 설정
void CViewCIM::ClickGxbtnCommMode1()
{
	GetLogCtrl()->AddBuf(LOG_OPERATION, "Remote Button Click!" );

	// TODO: Add your message handler code here
	if ( m_XComPro.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "1") != 0)
	{
		//d m_swcRemote.put_Active(false);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE1, GXCOLOR_OFF);			// REMOTE button OFF
		ControlStateChange(E_CRST_REMOTE);
	}
	else if ( m_XComPro.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "1") == 0)
	{
		// m_swcRemote.put_Active(true);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE1, GXCOLOR_ON);			// REMOTE button ON
		GetLogCtrl()->AddBuf(LOG_OPERATION, "Remote Button Click! But it is already Remote state" );
		AfxMessageBox("Already Remote state");
	}
	else
	{
		// m_swcRemote.put_Active(false);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE1, GXCOLOR_OFF);			// REMOTE button OFF
	}
}

// LOCAL 설정
void CViewCIM::ClickGxbtnCommMode2()
{
	GetLogCtrl()->AddBuf(LOG_OPERATION, "Local Button Click!" );

	// TODO: Add your message handler code here
	if ( m_XComPro.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "2") != 0)
	{
		// m_swcLocal.put_Active(false);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE2, GXCOLOR_OFF);			// LOCAL button OFF
		ControlStateChange(E_CRST_LOCAL);
	}
	else if ( m_XComPro.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "2")==0)
	{
		// m_swcLocal.put_Active(true);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE2, GXCOLOR_ON);			// LOCAL button ON
		GetLogCtrl()->AddBuf(LOG_OPERATION, "Local Button Click! But it is already Local state" );
		AfxMessageBox("Already Local state");
	}
	else
	{
		// m_swcLocal.put_Active(false);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE2, GXCOLOR_OFF);			// LOCAL button OFF

	}
}

// OFF Line 설정
void CViewCIM::ClickGxbtnCommMode3()
{
// 	// aaaaaaaaaaaaaaaaaaaaaaaaaaa [11/26/2016 OSC]
// 	strcpy_s(g_pData->m_cOPCallID, "aa");
// 	strcpy_s(g_pData->m_cOPCallMSG, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
// 	g_pData->m_cOPCallID;
// 	g_pData->m_cOPCallMSG;
// 	SetOPCall();
// 	return;

	GetLogCtrl()->AddBuf(LOG_OPERATION,  "OffLine Button Click!" );

	// 	m_swcOffline.put_Active(false);
	// 	if(strcmp(g_pData->m_cCRST, "0")!=0)	//OFF이 아닐경우
	// 		ControlStateChange(E_CRST_OFFLINE);

	if ( m_XComPro.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "0")!=0)
	{
		// m_swcOffline.put_Active(false);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3, GXCOLOR_OFF);			// OFFLINE button OFF
		ControlStateChange(E_CRST_OFFLINE);
	}
	else if ( m_XComPro.GetParam("HSMS.SELECTED") == "1" && strcmp(g_pData->m_cCRST, "0")==0)
	{
		// m_swcOffline.put_Active(true);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3, GXCOLOR_OFF);			// OFFLINE button ON
		GetLogCtrl()->AddBuf(LOG_OPERATION,  "OffLine Button Click! But it is already Offline state" );
		AfxMessageBox("Already Offline state");
	}
	else
	{
		// m_swcOffline.put_Active(false);
		CGxUICtrl::SetButtonColor(this, IDC_GXBTN_COMM_MODE3, GXCOLOR_OFF);			// OFFLINE button OFF
	}
}

// Connect 시도
void CViewCIM::ClickGxbtnCommConnect()
{
	return;
	int         nReturn;
	CString str;

	GetLogCtrl()->AddBuf(LOG_OPERATION, "Connect Button Click!" );

	str = m_XComPro.GetParam("HSMS.STATE");
	if ( str == "SELECTED" )
	{
		//  Start the XCom1 control
		if( (nReturn = m_XComPro.Stop()) == 0 ) {
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom stopped successfully" );
		}
		else 
		{
			GetLogCtrl()->AddBuf(LOG_CIM,"Fail to stop XCom  ret = %d", nReturn );
			return;
		}
	}
	else
	{
		m_XComPro.Stop();
		m_XComPro.Close();

		if( (nReturn = m_XComPro.Initialize( CSettingParm::cHsmsConfig )) == 0 ) 
		{
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom cfg Path(%s)", CSettingParm::cHsmsConfig);
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom initialized successfully" );
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom HSMS IP(%s)", m_XComPro.GetParam("IP"));
			GetLogCtrl()->AddBuf(LOG_CIM, "XCom HSMS Port(%s)", m_XComPro.GetParam("Port"));

			// m_lblIP.put_Caption(m_XCom.GetParam("IP"));
			// m_lblPort.put_Caption(m_XCom.GetParam("Port"));
			CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE3, m_XComPro.GetParam("IP"));
			CGxUICtrl::SetStaticString(this, IDC_GXLBL_CIM_STATE4, m_XComPro.GetParam("Port"));

			SetTimer(HOST_CONNECT_TIMER, 3000, NULL);

			//Sleep(3000); //2015.03.25 GR.PARK - 운영프로그램이 3초마다 연결 시도 하므로 딜리이를 3초 줌. (임시조치) 종료직전 레시피를 저장하도록 수정 예정

			//if( (nReturn = m_XCom.Start()) == 0 ) 
			//{
			//	GetLogCtrl()->AddBuf(LOG_CIM, "XCom started successfully" );
			//}
			//else 
			//{
			//	GetLogCtrl()->AddBuf(LOG_CIM,"Fail to start XCom (%d)", nReturn );
			//}

		}
		else 
		{
			AfxMessageBox( "Fail to initialize XCom (%d)", nReturn );
			GetLogCtrl()->AddBuf(LOG_CIM, "Fail to initialize XCom (%d)", nReturn );
		}
	}}

//kjpark 20180123 CELL INFO, JOB PROCESS Cellid 추가
BOOL CViewCIM::SetCellJobPrecessState( BOOL bJobState , CString strCellID, CString strProductID, CString strStepID, CString strJobState )
{
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;	
	pData->sMsgType = HOST_SET_CELL_JOB_PROCESS_STATE;					

	if (bJobState)
	{
		strcpy_s(pData->cMsgData[0], _T("JOBSTART"));	//Jobstart
	}
	else
		strcpy_s(pData->cMsgData[0], _T("INVALID"));	//JobCancel	
	
	strcpy_s(pData->cCellID, strCellID);
	strcpy_s(pData->cMsgData[1],strStepID);
	strcpy_s(pData->cMsgData[2], strProductID);
	//kjpark 20180123 CELL INFO, JOB PROCESS Cellid 추가
	strcpy_s(pData->cMsgData[3], strJobState);

	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);
	GetLogCtrl()->AddBuf(LOG_CIM, "[SetCellJobPrecessState] - OptionCode(%s), Cellid(%s), StepID(%s) Job State(%s)"
		, pData->cMsgData[0]
		, pData->cCellID
		, pData->cMsgData[1]
		, pData->cMsgData[3]);

	delete pData;
	return TRUE;
}

BOOL CViewCIM::SetPPIDCreate( CString strRecipeName )
{
	// 상위로부터 신규 레시피 생성 [9/27/2017 OSC]
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;
	if(strRecipeName.IsEmpty())
		return FALSE;

	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;	
	pData->sMsgType = HOST_SET_PPID_CREATE;					

	pData->sUnit = 0;
	pData->sCellPos = 0;

	strcpy_s(pData->cMsgData[0], strRecipeName.GetString());	//Jobstart

	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);
	GetLogCtrl()->AddBuf(LOG_CIM, "[SetPPIDCreate] - RecipeName(%s)"
		, pData->cMsgData[0]);

	delete pData;
	return TRUE;
}

BOOL CViewCIM::SetCellLotDefectInformation( CString strCellID, int nValCnt )
{
	CString snValCnt;
	int i=0;
	int j=2;
	if(!GetSockStsDlg()->GetSocketStatus())
		return FALSE;
	if(nValCnt>5)
	{
		return FALSE;
	}
	snValCnt.Format(_T("%d"),nValCnt);
	tag_Message* pData = new tag_Message;
	ZeroMemory(pData, sizeof(tag_Message));
	int nReturn;
	CString	strMsg;

	pData->sMsgClass = CLASS_CONTROL;
	pData->sMsgType = HOST_SET_CELL_LOT_INFORMATION;						// Cell Lot Information 응답 수신

	strcpy_s(pData->cMsgData[0], _T("DFCT"));	// OPTIONCODE

	//old strcpy_s(pData->cMsgData[1], pCellData->m_cCellID);
	strcpy_s(pData->cCellID, strCellID);
	strcpy_s(pData->cMsgData[1], snValCnt);	
	for(i=0; i<nValCnt; i++)
	{
		//20170502 BKH, 글자수가 39자를 넘어서면 더이상 받지 못하도록 예외처리
		m_Automation.m_sItemDefectNameValue[i] = m_Automation.GetCorrectString(m_Automation.m_sItemDefectNameValue[i], 39);
		strcpy_s(pData->cMsgData[j], m_Automation.m_sItemDefectNameValue[i]);								// 디펙트 이름
		j++;
		strcpy_s(pData->cMsgData[j], m_Automation.m_sItemXY_Value[i]+_T(",")+ m_Automation.m_sItemPatternNameValue[i]);	// X,Y value+ 패턴Name
		j++;
	}
	
	nReturn = GetSockStsDlg()->SendMsgToControlPC(pData);
	GetLogCtrl()->AddBuf(LOG_CIM, "[SetCellLotDefectInformation] - OptionCode(%s), Cellid(%s)"
		, pData->cMsgData[0]
	, pData->cCellID);
	for(i=0; i<j; i++)
	{
		GetLogCtrl()->AddBuf(LOG_CIM, "[SetCellLotDefectInformation cMsgData] - cMsgData[%d]:,%s"
		, i, pData->cMsgData[i]);
	}
	
	delete pData;

	return TRUE;
}

