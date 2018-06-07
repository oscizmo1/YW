// SocketDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SocketDialog.h"
#include "afxdialogex.h"
//kjpark 20161021 Setup 신호기 UI 구현
#include "UI\Form\FormSignal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum 
{
	TIMER_DATAPC_SYNC = 10,
};

IMPLEMENT_DYNAMIC(CSocketDialog, CDialog)

CSocketDialog::CSocketDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketDialog::IDD, pParent)
{
}

CSocketDialog::~CSocketDialog()
{	
}

void CSocketDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSocketDialog, CDialog)
	ON_MESSAGE(WM_PACKET_RECEIVE, OnPacketReceive)
	ON_MESSAGE(WM_RE_CONNECT, OnReConnect)
	//kjpark 20160919 Topdlg에 connect status 
	ON_MESSAGE(WM_SOCKET_CONNECT, OnSocketConnect)
	ON_MESSAGE(WM_SOCKET_CLOSE, OnSocketClose)
	// View 화면을 초기화하고 사용을 활성화 / 비활성화 한다.
	
	ON_WM_TIMER()
END_MESSAGE_MAP()


LRESULT CSocketDialog::OnReConnect( WPARAM wParam, LPARAM lParam )
{

	CDataSocket* pSocket = (CDataSocket *)wParam;
	BOOL bCreate = (BOOL)lParam;

	//kjpark 20170919 MCR 재연결 
	if (pSocket->GetSocketName() == SOCKET_MCR_1)
 		m_MCR[JIG_ID_A][JIG_CH_1].CreateSocket(JIG_ID_A, JIG_CH_1);	
	else if (pSocket->GetSocketName() == SOCKET_MCR_3)
		m_MCR[JIG_ID_B][JIG_CH_1].CreateSocket(JIG_ID_B, JIG_CH_1);
	else if (pSocket->GetSocketName() == SOCKET_CIM)	//SONIC
 		m_CIM.CreateSocket();
 
 	SetTimer(TIMER_RE_CONNECT, 3000, NULL);

	return 0;
}

// 재접속 시도 타이머
//kjpark 20160919 Topdlg에 connect status 
void CSocketDialog::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);

	if(nIDEvent == TIMER_RE_CONNECT)
	{
		for(int jig = 0; jig < JIG_ID_MAX; jig++)
		{
			for(int ch = 0; ch < JIG_CH_MAX; ch++)
			{
				if ( m_MCR[jig][ch].IsConnected() == FALSE )
				{
					m_MCR[jig][ch].ConnectSocket();
				}
			}
		}

		if(m_CIM.IsConnected() == FALSE)
		{
			m_CIM.m_Socket.ConnectToServer();
		}
	}
// 	else if(nIDEvent == TIMER_DATAPC_SYNC)
// 	{
// 		// 연결될 때 마다 현재 상태 동기화 [11/28/2016 OSC]
// 		m_CIM.SendCmdToDataPC(EQUIP_SET_CONTROL);
// 		m_CIM.SendCmdToDataPC(EQUIP_SET_DATAPC_VERSION);
// 		m_CIM.SendCmdRecipeToDataPC(EQUIP_SET_PPID_OPERATION, theRecipeBank.m_strRecipeName);
// 		m_CIM.SendCmdEqpFunctionReportToDataPC();
// 		theUnitFunc.CIM_CellExistInMachine();
// 	}

	CDialog::OnTimer(nIDEvent);
}

void CSocketDialog::SocketClosed(CDataSocket *pSocket, CClientSocket* pClientSocket)
{
	//연결이 끊겼을 경우 처리(Message print)

}


LRESULT CSocketDialog::OnPacketReceive( WPARAM wParam, LPARAM lParam )
{
	int nRtn = 0;
	CDataSocket* pSocket = (CDataSocket *)wParam;
	int nMsgLen = pSocket->GetLastLength();

	if(pSocket->GetSocketName() == SOCKET_PGPC)
	{
		m_PGHost.PreparseMessage(pSocket);
	}
	//kjpark 20170901 소켓 핸들러 셋팅
	else if(pSocket->GetSocketName() == SOCKET_CIM)
	{
		m_CIM.PreparseMessage(pSocket);
	}
	else if (pSocket->GetSocketName() == SOCKET_MCR_1)
	{
		m_MCR[JIG_ID_A][JIG_CH_1].PreparseMessage(pSocket);		
	}
	else if (pSocket->GetSocketName() == SOCKET_MCR_3)
	{
		m_MCR[JIG_ID_B][JIG_CH_1].PreparseMessage(pSocket);		
	}
	return nRtn;
}

// 소켓의 이름을 가지고 Index를 얻는다, Main Frame에 접속 여부 표시용

int CSocketDialog::GetSocketIndex(CString sName)
{
	int nIdx = 99;

	if ( sName == SOCKET_ACTIVEALIGN_PC )		// _T("ALIGN")
		nIdx = eCOMM_ALIGNPC;
	else if ( sName == SOCKET_PGPC )		// _T("PGPC")
		nIdx = eCOMM_SignalPC;
	else if ( sName == SOCKET_CIM )	//	_T("DATAPC")	//SONIC
		nIdx = eCOMM_DataPC;

	return nIdx;
}


// 지정 TCP/IP 통신의 연결 상태를 조회한다.
//kjpark 20160919 Topdlg에 connect status 
BOOL CSocketDialog::GetCommConnected(int nIdx)
{
	BOOL bResult = FALSE;

	switch (nIdx)
	{
	case eCOMM_SignalPC :	// 신호기 PC
		bResult = m_PGHost.IsConnected();
		break;

	case eCOMM_DataPC :		// 데이터 PC 와의 통신
		bResult = m_CIM.IsConnected();
		break;
	case eCOMM_ALIGNPC :		// ALIGN과의 통신
		bResult = m_ActiveAlign.IsConnected();
		break;
	}

	return bResult;			// 접속여부 회신
}

//kjpark 20160919 Topdlg에 connect status 
LRESULT CSocketDialog::OnSocketConnect( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;

	if(pSocket->IsConnected())
	{
		//!		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s][IP: %s , %d] 연결 성공."), pSocket->GetSocketName(), pSocket->GetIP(), pSocket->GetPort());
		CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
		::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pSocket->GetSocketName()), 1);		// 연결 이벤트

		if(pSocket->GetSocketName() == SOCKET_CIM)
		{
			// 연결될 때 마다 현재 상태 동기화 [11/28/2016 OSC]
			m_CIM.SendCmdToDataPC(EQUIP_SET_CONTROL);
			m_CIM.SendCmdToDataPC(EQUIP_SET_DATAPC_VERSION);
			m_CIM.SendCmdRecipeToDataPC(EQUIP_SET_PPID_OPERATION, theRecipeBank.m_strRecipeName);
			m_CIM.SendCmdEqpFunctionReportToDataPC();
			theUnitFunc.CIM_CellExistInMachine();
			m_CIM.SendUnitStatusReport();

// 			SetTimer(TIMER_DATAPC_SYNC, 100, NULL);
		}
	}
	return 0;
}

LRESULT CSocketDialog::OnSocketClose( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;

	//kjpark 20161109 Defect Sockt 구현
	if(pSocket->IsServer())
	{
		CClientSocket* pClientSocket = (CClientSocket*)lParam;
		if(pClientSocket->IsConnected() == FALSE)
		{
			CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
			::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pSocket->GetSocketName()), 0);		// 연결 이벤트
		}
		
	}
	else
	{
		if(pSocket->IsConnected() == FALSE)
		{
			//!		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s][IP: %s , %d] 연결 성공."), pSocket->GetSocketName(), pSocket->GetIP(), pSocket->GetPort());
			CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
			::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pSocket->GetSocketName()), 0);		// 연결 이벤트


			//kjpark 20170831 MCR 구현 채널 별 구현 완료
			if(pSocket->GetSocketName() == SOCKET_MCR_1)
			{
				theSocketInterFace.m_MCR[JIG_ID_A][JIG_CH_1].SetMCRDisconnectSDK();
			}
			else if(pSocket->GetSocketName() == SOCKET_MCR_3)
			{
				theSocketInterFace.m_MCR[JIG_ID_B][JIG_CH_1].SetMCRDisconnectSDK();
			}
			else if(pSocket->GetSocketName() == SOCKET_MCR_5)
			{
				theSocketInterFace.m_MCR[JIG_ID_B][JIG_CH_1].SetMCRDisconnectSDK();
			}
		}
	}
	

	// DataPC와 연결이 되었을 때 처리 할 내용들
	if (pSocket->GetSocketName() == SOCKET_CIM)	//SONIC - 2013.06.28.
	{
		theProcBank.ChangeCimState(CONST_CIM_STATE::CIM_OFFLINE);
	}

	return 0;
}

void CSocketDialog::SetSocketConnect()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			//kjpark 20170912 MCR 소켓 연결 시 핸들러 없어 문제되는 버그 수정
			m_MCR[jig][ch].SetParentHWnd(GetSafeHwnd());
			//kjpark 20170831 MCR 구현 채널 별 구현 완료
			m_MCR[jig][ch].CreateSocket((JIG_ID)jig, (JIG_CH)ch);
		}
	}

	m_PGHost.SetParentHWnd(GetSafeHwnd());
	m_PGHost.CreateSocket();

	m_CIM.SetParentHWnd(GetSafeHwnd());
	m_CIM.CreateSocket();
}

