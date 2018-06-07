// SocketDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SocketDialog.h"
#include "afxdialogex.h"
//kjpark 20161021 Setup ��ȣ�� UI ����
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
	//kjpark 20160919 Topdlg�� connect status 
	ON_MESSAGE(WM_SOCKET_CONNECT, OnSocketConnect)
	ON_MESSAGE(WM_SOCKET_CLOSE, OnSocketClose)
	// View ȭ���� �ʱ�ȭ�ϰ� ����� Ȱ��ȭ / ��Ȱ��ȭ �Ѵ�.
	
	ON_WM_TIMER()
END_MESSAGE_MAP()


LRESULT CSocketDialog::OnReConnect( WPARAM wParam, LPARAM lParam )
{

	CDataSocket* pSocket = (CDataSocket *)wParam;
	BOOL bCreate = (BOOL)lParam;

	//kjpark 20170919 MCR �翬�� 
	if (pSocket->GetSocketName() == SOCKET_MCR_1)
 		m_MCR[JIG_ID_A][JIG_CH_1].CreateSocket(JIG_ID_A, JIG_CH_1);	
	else if (pSocket->GetSocketName() == SOCKET_MCR_3)
		m_MCR[JIG_ID_B][JIG_CH_1].CreateSocket(JIG_ID_B, JIG_CH_1);
	else if (pSocket->GetSocketName() == SOCKET_CIM)	//SONIC
 		m_CIM.CreateSocket();
 
 	SetTimer(TIMER_RE_CONNECT, 3000, NULL);

	return 0;
}

// ������ �õ� Ÿ�̸�
//kjpark 20160919 Topdlg�� connect status 
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
// 		// ����� �� ���� ���� ���� ����ȭ [11/28/2016 OSC]
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
	//������ ������ ��� ó��(Message print)

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
	//kjpark 20170901 ���� �ڵ鷯 ����
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

// ������ �̸��� ������ Index�� ��´�, Main Frame�� ���� ���� ǥ�ÿ�

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


// ���� TCP/IP ����� ���� ���¸� ��ȸ�Ѵ�.
//kjpark 20160919 Topdlg�� connect status 
BOOL CSocketDialog::GetCommConnected(int nIdx)
{
	BOOL bResult = FALSE;

	switch (nIdx)
	{
	case eCOMM_SignalPC :	// ��ȣ�� PC
		bResult = m_PGHost.IsConnected();
		break;

	case eCOMM_DataPC :		// ������ PC ���� ���
		bResult = m_CIM.IsConnected();
		break;
	case eCOMM_ALIGNPC :		// ALIGN���� ���
		bResult = m_ActiveAlign.IsConnected();
		break;
	}

	return bResult;			// ���ӿ��� ȸ��
}

//kjpark 20160919 Topdlg�� connect status 
LRESULT CSocketDialog::OnSocketConnect( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;

	if(pSocket->IsConnected())
	{
		//!		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s][IP: %s , %d] ���� ����."), pSocket->GetSocketName(), pSocket->GetIP(), pSocket->GetPort());
		CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
		::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pSocket->GetSocketName()), 1);		// ���� �̺�Ʈ

		if(pSocket->GetSocketName() == SOCKET_CIM)
		{
			// ����� �� ���� ���� ���� ����ȭ [11/28/2016 OSC]
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

	//kjpark 20161109 Defect Sockt ����
	if(pSocket->IsServer())
	{
		CClientSocket* pClientSocket = (CClientSocket*)lParam;
		if(pClientSocket->IsConnected() == FALSE)
		{
			CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
			::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pSocket->GetSocketName()), 0);		// ���� �̺�Ʈ
		}
		
	}
	else
	{
		if(pSocket->IsConnected() == FALSE)
		{
			//!		GetLogCtrl()->AddBuf(LOG_COMM, _T("[%s][IP: %s , %d] ���� ����."), pSocket->GetSocketName(), pSocket->GetIP(), pSocket->GetPort());
			CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
			::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pSocket->GetSocketName()), 0);		// ���� �̺�Ʈ


			//kjpark 20170831 MCR ���� ä�� �� ���� �Ϸ�
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
	

	// DataPC�� ������ �Ǿ��� �� ó�� �� �����
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
			//kjpark 20170912 MCR ���� ���� �� �ڵ鷯 ���� �����Ǵ� ���� ����
			m_MCR[jig][ch].SetParentHWnd(GetSafeHwnd());
			//kjpark 20170831 MCR ���� ä�� �� ���� �Ϸ�
			m_MCR[jig][ch].CreateSocket((JIG_ID)jig, (JIG_CH)ch);
		}
	}

	m_PGHost.SetParentHWnd(GetSafeHwnd());
	m_PGHost.CreateSocket();

	m_CIM.SetParentHWnd(GetSafeHwnd());
	m_CIM.CreateSocket();
}

